/*
  Copyright (c) 2010 - 2017, Nordic Semiconductor ASA
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

  2. Redistributions in binary form, except as embedded into a Nordic
     Semiconductor ASA integrated circuit in a product or a software update for
     such product, must reproduce the above copyright notice, this list of
     conditions and the following disclaimer in the documentation and/or other
     materials provided with the distribution.

  3. Neither the name of Nordic Semiconductor ASA nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

  4. This software, with or without modification, must only be used with a
     Nordic Semiconductor ASA integrated circuit.

  5. Any software provided in binary form under this license must not be reverse
     engineered, decompiled, modified and/or disassembled.

  THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
  OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "pca20020.h"
#include "drv_motion.h"
#include "drv_mpu9250.h"
#include "nrf_error.h"
#include "app_timer.h"
#include "drv_acc.h"
#include "app_timer.h"
#include "app_scheduler.h"
#include "thingy_config.h"
#include "nrf_delay.h"
#include "drv_ext_gpio.h"
#include "nrf_drv_gpiote.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "invensense.h"
#include "invensense_adv.h"
#include "eMPL_outputs.h"
#include "mltypes.h"
#include "mpu.h"

#define PEDO_READ_MS    (1000UL)
#define TEMP_READ_MS     (500UL)
#define COMPASS_READ_MS  (500UL)
#define DEFAULT_MPU_HZ    (10UL)

#ifdef MOTION_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

#define RETURN_IF_INV_ERROR(PARAM)                                                                \
        if ((PARAM) != INV_SUCCESS)                                                               \
        {                                                                                         \
            return NRF_ERROR_INTERNAL;                                                            \
        }

unsigned char *mpl_key = (unsigned char*)"eMPL 5.1";

/**@brief Platform-specific information. Kinda like a boardfile.
 */
typedef struct
{
    signed char orientation[9];
} platform_data_t;

/**@brief Acclerometer rotation matrix.
 *
 * @note Accellerometer inverted to get positive readings when axis is aligned with g (down).
 */
static platform_data_t s_accel_pdata =
{
    .orientation = { -1,  0,  0,
                      0, -1,  0,
                      0,  0, -1}
};

/**@brief Gyroscope rotation matrix.
 */
static platform_data_t s_gyro_pdata =
{
    .orientation = {  1,  0,  0,
                      0,  1,  0,
                      0,  0,  1}
};

/**@brief Compass (magnetometer) rotation matrix.
 */
static platform_data_t s_compass_pdata =
{
    .orientation = {  0,  1,  0,
                      1,  0,  0,
                      0,  0, -1}
};

static struct
{
    bool                      lp_accel_mode;
    bool                      running;
    uint8_t                   sensors;
    bool                      dmp_on;
    uint16_t                  dmp_features;
    drv_motion_feature_mask_t features;
    drv_motion_evt_handler_t  evt_handler;
    bool                      do_temp;
    bool                      do_compass;
    bool                      do_pedo;
    uint16_t                  pedo_interval_ms;
    uint16_t                  temp_interval_ms;
    uint16_t                  compass_interval_ms;
    uint16_t                  motion_freq_hz;
    uint8_t                   wake_on_motion;
} m_motion;

APP_TIMER_DEF(m_temp_timer_id);
APP_TIMER_DEF(m_compass_timer_id);
APP_TIMER_DEF(m_pedo_timer_id);

static void mpulib_data_send(void)
{
    inv_time_t       timestamp;
    int8_t           accuracy;
    int32_t          data[9];
    drv_motion_evt_t evt;

    if (m_motion.features & DRV_MOTION_FEATURE_MASK_RAW)
    {
        bool valid_raw = false;
        if (m_motion.features & DRV_MOTION_FEATURE_MASK_RAW_ACCEL)
        {
            if (inv_get_sensor_type_accel((long *)&data[0], &accuracy, &timestamp))
            {
                // X, Y, and Z
                valid_raw = true;
            }
            else
            {
                data[0] = 0;
                data[1] = 0;
                data[2] = 0;
            }
        }

        if (m_motion.features & DRV_MOTION_FEATURE_MASK_RAW_GYRO)
        {
            if (inv_get_sensor_type_gyro((long *)&data[3], &accuracy, &timestamp))
            {
                // X, Y, and Z
                valid_raw = true;
            }
            else
            {
                data[3] = 0;
                data[4] = 0;
                data[5] = 0;
            }
        }

        if (m_motion.features & DRV_MOTION_FEATURE_MASK_RAW_COMPASS)
        {
            if (inv_get_sensor_type_compass((long *)&data[6], &accuracy, &timestamp))
            {
                // X, Y, and Z
                valid_raw = true;
            }
            else
            {
                data[6] = 0;
                data[7] = 0;
                data[8] = 0;
            }
        }

        if (valid_raw)
        {
            evt = DRV_MOTION_EVT_RAW;
            m_motion.evt_handler(&evt, data, sizeof(long) * 9);
        }
    }

    if (m_motion.features & DRV_MOTION_FEATURE_MASK_QUAT)
    {
        if (inv_get_sensor_type_quat((long *)data, &accuracy, &timestamp))
        {
            evt = DRV_MOTION_EVT_QUAT;
            // W, X, Y, and Z
            m_motion.evt_handler(&evt, data, sizeof(long) * 4);
        }
    }

    if (m_motion.features & DRV_MOTION_FEATURE_MASK_EULER)
    {
        if (inv_get_sensor_type_euler((long *)data, &accuracy, &timestamp))
        {
            evt = DRV_MOTION_EVT_EULER;
            // Pitch, roll and yaw
            m_motion.evt_handler(&evt, data, sizeof(long) * 3);
        }
    }

    if (m_motion.features & DRV_MOTION_FEATURE_MASK_ROT_MAT)
    {
        if (inv_get_sensor_type_rot_mat((long *)data, &accuracy, &timestamp))
        {
            evt = DRV_MOTION_EVT_ROT_MAT;
            m_motion.evt_handler(&evt, data, sizeof(long) * 9);
        }
    }

    if (m_motion.features & DRV_MOTION_FEATURE_MASK_HEADING)
    {
        if (inv_get_sensor_type_heading((long *)data, &accuracy, &timestamp))
        {
            evt = DRV_MOTION_EVT_HEADING;
            m_motion.evt_handler(&evt, data, sizeof(long));
        }
    }

    if (m_motion.features & DRV_MOTION_FEATURE_MASK_GRAVITY_VECTOR)
    {
        float gravity[3];

        if (inv_get_sensor_type_gravity(gravity, &accuracy, &timestamp))
        {
            evt = DRV_MOTION_EVT_GRAVITY;
            // x, y and z
            m_motion.evt_handler(&evt, gravity, sizeof(float) * 3);
        }
    }

    if (m_motion.features & DRV_MOTION_FEATURE_MASK_PEDOMETER && m_motion.do_pedo)
    {
        static unsigned long s_prev_pedo[2] = {0}; //step_count, walk_time;
        unsigned long pedometer[2] = {0}; //step_count, walk_time;
        m_motion.do_pedo = 0;

        (void)dmp_get_pedometer_step_count(&pedometer[0]);
        (void)dmp_get_pedometer_walk_time(&pedometer[1]);

        if ((pedometer[0] > 0) && ((pedometer[0] != s_prev_pedo[0]) || (pedometer[1] != s_prev_pedo[1])))
        {
            evt = DRV_MOTION_EVT_PEDOMETER;
            // step_count and walk_time
            m_motion.evt_handler(&evt, pedometer, sizeof(unsigned long) * 2);

            s_prev_pedo[0] = pedometer[0];
            s_prev_pedo[1] = pedometer[1];
        }
    }
}


static void mpulib_data_handler(void * p_event_data, uint16_t event_size)
{
    bool more_todo;

    do
    {
        unsigned long sensor_timestamp;
        int new_data = 0;
        more_todo = false;

        if (m_motion.lp_accel_mode)
        {
            short accel_short[3];
            long accel[3];

            (void)mpu_get_accel_reg(accel_short, &sensor_timestamp);

            accel[0] = (long)accel_short[0];
            accel[1] = (long)accel_short[1];
            accel[2] = (long)accel_short[2];

            (void)inv_build_accel(accel, 0, sensor_timestamp);
            new_data = 1;
        }
        else if (m_motion.dmp_on)
        {
            short gyro[3], accel_short[3], sensors;
            unsigned char more;
            long accel[3], quat[4], temperature;
            /* This function gets new data from the FIFO when the DMP is in
             * use. The FIFO can contain any combination of gyro, accel,
             * quaternion, and gesture data. The sensors parameter tells the
             * caller which data fields were actually populated with new data.
             * For example, if sensors == (INV_XYZ_GYRO | INV_WXYZ_QUAT), then
             * the FIFO isn't being filled with accel data.
             * The driver parses the gesture data to determine if a gesture
             * event has occurred; on an event, the application will be notified
             * via a callback (assuming that a callback function was properly
             * registered). The more parameter is non-zero if there are
             * leftover packets in the FIFO.
             */
            (void)dmp_read_fifo(gyro, accel_short, quat, &sensor_timestamp, &sensors, &more);

            if (more)
            {
                more_todo = true;
                DEBUG_PRINTF(0, "mpulib_data_handler: more_todo\r\n");
            }

            if (sensors & INV_XYZ_GYRO) {
                /* Push the new data to the MPL. */
                (void)inv_build_gyro(gyro, sensor_timestamp);
                new_data = 1;
                if (m_motion.do_temp) {
                    m_motion.do_temp = 0;
                    /* Temperature only used for gyro temp comp. */
                    (void)mpu_get_temperature(&temperature, &sensor_timestamp);
                    (void)inv_build_temp(temperature, sensor_timestamp);
                }
            }
            if (sensors & INV_XYZ_ACCEL) {
                accel[0] = (long)accel_short[0];
                accel[1] = (long)accel_short[1];
                accel[2] = (long)accel_short[2];
                (void)inv_build_accel(accel, 0, sensor_timestamp);
                new_data = 1;
            }
            if (sensors & INV_WXYZ_QUAT) {
                (void)inv_build_quat(quat, 0, sensor_timestamp);
                new_data = 1;
            }
        }
        else
        {
            short gyro[3], accel_short[3];
            unsigned char sensors, more;
            long accel[3], temperature;
            /* This function gets new data from the FIFO. The FIFO can contain
             * gyro, accel, both, or neither. The sensors parameter tells the
             * caller which data fields were actually populated with new data.
             * For example, if sensors == INV_XYZ_GYRO, then the FIFO isn't
             * being filled with accel data. The more parameter is non-zero if
             * there are leftover packets in the FIFO. The HAL can use this
             * information to increase the frequency at which this function is
             * called.
             */

            (void)mpu_read_fifo(gyro, accel_short, &sensor_timestamp, &sensors, &more);

            if (more)
            {
                more_todo = true;
                DEBUG_PRINTF(0, "mpulib_data_handler: more_todo\r\n");
            }

            if (sensors & INV_XYZ_GYRO)
            {
                /* Push the new data to the MPL. */
                (void)inv_build_gyro(gyro, sensor_timestamp);
                new_data = 1;
                if (m_motion.do_temp)
                {
                    m_motion.do_temp = 0;
                    /* Temperature only used for gyro temp comp. */
                    (void)mpu_get_temperature(&temperature, &sensor_timestamp);
                    (void)inv_build_temp(temperature, sensor_timestamp);
                }
            }
            if (sensors & INV_XYZ_ACCEL)
            {
                accel[0] = (long)accel_short[0];
                accel[1] = (long)accel_short[1];
                accel[2] = (long)accel_short[2];
                (void)inv_build_accel(accel, 0, sensor_timestamp);
                new_data = 1;
            }
        }

        if (m_motion.do_compass)
        {
            short compass_short[3];
            long compass[3];
            m_motion.do_compass = 0;
            /* For any MPU device with an AKM on the auxiliary I2C bus, the raw
             * magnetometer registers are copied to special gyro registers.
             */
            if (!mpu_get_compass_reg(compass_short, &sensor_timestamp))
            {
                compass[0] = (long)compass_short[0];
                compass[1] = (long)compass_short[1];
                compass[2] = (long)compass_short[2];
                /* NOTE: If using a third-party compass calibration library,
                 * pass in the compass data in uT * 2^16 and set the second
                 * parameter to INV_CALIBRATED | acc, where acc is the
                 * accuracy from 0 to 3.
                 */
                (void)inv_build_compass(compass, 0, sensor_timestamp);
            }
            new_data = 1;
        }

        if (new_data) {
            (void)inv_execute_on_data();
            mpulib_data_send();
        }
    }while (more_todo == true);
}


static void mpulib_data_handler_cb(void)
{
    mpulib_data_handler(0, 0);
}


static void mpulib_tap_cb(unsigned char direction, unsigned char count)
{
    if (m_motion.features & DRV_MOTION_FEATURE_MASK_TAP)
    {
        drv_motion_evt_t evt     = DRV_MOTION_EVT_TAP;
        uint8_t          data[2] = {direction, count};

        m_motion.evt_handler(&evt, data, sizeof(data));
    }

#ifdef MOTION_DEBUG
    switch (direction)
    {
        case TAP_X_UP:
            DEBUG_PRINTF(0, "drv_motion: tap x+ ");
            break;
        case TAP_X_DOWN:
            DEBUG_PRINTF(0, "drv_motion: tap x- ");
            break;
        case TAP_Y_UP:
            DEBUG_PRINTF(0, "drv_motion: tap y+ ");
            break;
        case TAP_Y_DOWN:
            DEBUG_PRINTF(0, "drv_motion: tap y- ");
            break;
        case TAP_Z_UP:
            DEBUG_PRINTF(0, "drv_motion: tap z+ ");
            break;
        case TAP_Z_DOWN:
            DEBUG_PRINTF(0, "drv_motion: tap z- ");
            break;
        default:
            return;
    }

    DEBUG_PRINTF(0, "x%d\r\n", count);
#endif
}


static void mpulib_orient_cb(unsigned char orientation)
{
    if (m_motion.features & DRV_MOTION_FEATURE_MASK_ORIENTATION)
    {
        drv_motion_evt_t evt     = DRV_MOTION_EVT_ORIENTATION;

        m_motion.evt_handler(&evt, &orientation, 1);
    }

#ifdef MOTION_DEBUG
	switch (orientation)
    {
        case ANDROID_ORIENT_PORTRAIT:
            DEBUG_PRINTF(0, "Portrait\n");
            break;
        case ANDROID_ORIENT_LANDSCAPE:
            DEBUG_PRINTF(0, "Landscape\n");
            break;
        case ANDROID_ORIENT_REVERSE_PORTRAIT:
            DEBUG_PRINTF(0, "Reverse Portrait\n");
            break;
        case ANDROID_ORIENT_REVERSE_LANDSCAPE:
            DEBUG_PRINTF(0, "Reverse Landscape\n");
            break;
        default:
            return;
	}
#endif
}


static uint32_t mpulib_init(void)
{
    inv_error_t result;
    struct int_param_s int_param;

    int_param.cb = mpulib_data_handler_cb;

    result = mpu_init(&int_param);
    APP_ERROR_CHECK(result);

    result = inv_init_mpl();
    APP_ERROR_CHECK(result);

    /* Compute 6-axis and 9-axis quaternions. */
    (void)inv_enable_quaternion();
    (void)inv_enable_9x_sensor_fusion();
    /* Update gyro biases when not in motion. */
    (void)inv_enable_fast_nomot();
    /* Update gyro biases when temperature changes. */
    (void)inv_enable_gyro_tc();
    /* This algorithm updates the accel biases when in motion. A more accurate
     * bias measurement can be made when running the self-test. */
    (void)inv_enable_in_use_auto_calibration();
    /* Compass calibration algorithms. */
    (void)inv_enable_vector_compass_cal();
    (void)inv_enable_magnetic_disturbance();

    /* Allows use of the MPL APIs in read_from_mpl. */
    (void)inv_enable_eMPL_outputs();

    result = inv_start_mpl();
    APP_ERROR_CHECK(result);

    return NRF_SUCCESS;
}


static uint32_t mpulib_config(void)
{
    unsigned char accel_fsr;
    unsigned short gyro_rate;
    unsigned short gyro_fsr;
    unsigned short compass_fsr;

    (void)mpu_set_sensors(m_motion.sensors);

    /* Push both gyro, accel and compass data into the FIFO. */
    (void)mpu_configure_fifo(m_motion.sensors);
    (void)mpu_set_sample_rate(m_motion.motion_freq_hz);

    /* The compass sampling rate can be less than the gyro/accel sampling rate.
     * Use this function for proper power management. */
    (void)mpu_set_compass_sample_rate(1000UL / m_motion.compass_interval_ms);

    /* Read back configuration in case it was set improperly. */
    (void)mpu_get_sample_rate(&gyro_rate);
    (void)mpu_get_gyro_fsr(&gyro_fsr);
    (void)mpu_get_accel_fsr(&accel_fsr);
    (void)mpu_get_compass_fsr(&compass_fsr);

    /* Sync driver configuration with MPL. */
    /* Sample rate expected in microseconds. */
    (void)inv_set_gyro_sample_rate(1000000L / gyro_rate);
    (void)inv_set_accel_sample_rate(1000000L / gyro_rate);

    /* The compass rate is independent of the gyro and accel rates. As long as
     * inv_set_compass_sample_rate is called with the correct value, the 9-axis
     * fusion algorithm's compass correction gain will work properly. */
    (void)inv_set_compass_sample_rate(m_motion.compass_interval_ms * 1000L);

    /* Set chip-to-body orientation matrix.
     * Set hardware units to dps/g's/degrees scaling factor. */
    (void)inv_set_gyro_orientation_and_scale(
            inv_orientation_matrix_to_scalar(s_gyro_pdata.orientation),
            (long)gyro_fsr<<15);
    (void)inv_set_accel_orientation_and_scale(
            inv_orientation_matrix_to_scalar(s_accel_pdata.orientation),
            (long)accel_fsr<<15);
    (void)inv_set_compass_orientation_and_scale(
            inv_orientation_matrix_to_scalar(s_compass_pdata.orientation),
            (long)compass_fsr<<15);

    return NRF_SUCCESS;
}


static uint32_t dmp_init(void)
{
    /* Initialize DMP. */
    (void)dmp_load_motion_driver_firmware();
    (void)dmp_set_orientation(
        inv_orientation_matrix_to_scalar(s_gyro_pdata.orientation));
    (void)dmp_register_tap_cb(mpulib_tap_cb);
    (void)dmp_register_android_orient_cb(mpulib_orient_cb);

    m_motion.dmp_features = DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
        DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
        DMP_FEATURE_GYRO_CAL;
    (void)dmp_enable_feature(m_motion.dmp_features);
    (void)dmp_set_fifo_rate(m_motion.motion_freq_hz);
    (void)mpu_set_dmp_state(1);

    return NRF_SUCCESS;
}


static uint32_t mpu9250_power(bool enable)
{
    uint32_t err_code;

    if (enable)
    {
        #if defined(THINGY_HW_v0_7_0)
            err_code = drv_ext_gpio_pin_clear(SX_MPU_PWR_CTRL);
        #elif defined(THINGY_HW_v0_8_0)
            err_code = drv_ext_gpio_pin_clear(SX_MPU_PWR_CTRL);
        #elif defined(THINGY_HW_v0_9_0)
            err_code = drv_ext_gpio_pin_clear(SX_MPU_PWR_CTRL);
        #else
            err_code = drv_ext_gpio_pin_set(SX_MPU_PWR_CTRL);
        #endif
        RETURN_IF_ERROR(err_code);
    }
    else
    {
        #if defined(THINGY_HW_v0_7_0)
            err_code = drv_ext_gpio_pin_set(SX_MPU_PWR_CTRL);
        #elif defined(THINGY_HW_v0_8_0)
            err_code = drv_ext_gpio_pin_set(SX_MPU_PWR_CTRL);
        #elif defined(THINGY_HW_v0_9_0)
            err_code = drv_ext_gpio_pin_set(SX_MPU_PWR_CTRL);
        #else
            err_code = drv_ext_gpio_pin_clear(SX_MPU_PWR_CTRL);
        #endif
        RETURN_IF_ERROR(err_code);
    }

    return NRF_SUCCESS;
}


/**@brief Function for handling temperature timer timeout event.
 */
static void temp_timeout_handler(void * p_context)
{
    m_motion.do_temp = true;
}


/**@brief Function for handling compass timer timeout event.
 */
static void compass_timeout_handler(void * p_context)
{
    m_motion.do_compass = true;
}


/**@brief Function for handling pedometer timer timeout event.
 */
static void pedo_timeout_handler(void * p_context)
{
    m_motion.do_pedo = true;
}


uint32_t drv_motion_enable(drv_motion_feature_mask_t feature_mask)
{
    uint32_t err_code;

    if ( (feature_mask & ~(DRV_MOTION_FEATURE_MASK)) ||
         (feature_mask == 0) )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    /* Set features bits. */
    m_motion.features |= feature_mask;
    /* Set enabled sensors. */
    m_motion.sensors |= (INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS | INV_WXYZ_QUAT);

    if (!m_motion.running)
    {
        m_motion.running = true;

        err_code = mpu9250_power(true);
        APP_ERROR_CHECK(err_code);

        nrf_delay_ms(20);

        m_motion.dmp_on = true;
        err_code = mpulib_init();
        APP_ERROR_CHECK(err_code);
        err_code = mpulib_config();
        APP_ERROR_CHECK(err_code);
        err_code = dmp_init();
        APP_ERROR_CHECK(err_code);

        (void)drv_mpu9250_enable(true);

        err_code = app_timer_start(m_temp_timer_id,
                                   APP_TIMER_TICKS(m_motion.temp_interval_ms, APP_TIMER_PRESCALER),
                                   NULL);
        APP_ERROR_CHECK(err_code);

        err_code = app_timer_start(m_compass_timer_id,
                                   APP_TIMER_TICKS(m_motion.compass_interval_ms, APP_TIMER_PRESCALER),
                                   NULL);
        APP_ERROR_CHECK(err_code);

        err_code = app_timer_start(m_pedo_timer_id,
                                   APP_TIMER_TICKS(m_motion.pedo_interval_ms, APP_TIMER_PRESCALER),
                                   NULL);
        APP_ERROR_CHECK(err_code);
    }

    return NRF_SUCCESS;
}


uint32_t drv_motion_disable(drv_motion_feature_mask_t feature_mask)
{
    uint32_t err_code;

    if ( (feature_mask & ~(DRV_MOTION_FEATURE_MASK)) ||
         (feature_mask == 0) )
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    /* Clear feature bits. */
    m_motion.features &= ~feature_mask;

    if (!m_motion.features)
    {
        (void)drv_mpu9250_enable(false);

        err_code = mpu9250_power(false);
        RETURN_IF_ERROR(err_code);

        m_motion.running = false;

        err_code = app_timer_stop(m_temp_timer_id);
        RETURN_IF_ERROR(err_code);
        err_code = app_timer_stop(m_compass_timer_id);
        RETURN_IF_ERROR(err_code);
        err_code = app_timer_stop(m_pedo_timer_id);
        RETURN_IF_ERROR(err_code);
    }

    return NRF_SUCCESS;
}


uint32_t drv_motion_config(drv_motion_cfg_t * p_cfg)
{
    uint32_t err_code;

    NULL_PARAM_CHECK(p_cfg);

    m_motion.pedo_interval_ms      = p_cfg->pedo_interval_ms;
    m_motion.temp_interval_ms      = p_cfg->temp_interval_ms;
    m_motion.compass_interval_ms   = p_cfg->compass_interval_ms;
    m_motion.motion_freq_hz        = p_cfg->motion_freq_hz;
    m_motion.wake_on_motion        = p_cfg->wake_on_motion;

    if (m_motion.running)
    {
        drv_motion_feature_mask_t enabled_features = m_motion.features;

        err_code = drv_motion_disable(enabled_features);
        APP_ERROR_CHECK(err_code);
        return drv_motion_enable(enabled_features);
    }
    else
    {
        return NRF_SUCCESS;
    }
}


uint32_t drv_motion_sleep_prepare(bool wakeup)
{
    return drv_acc_wakeup_prepare(wakeup);
}


uint32_t drv_motion_init(drv_motion_evt_handler_t evt_handler, drv_motion_twi_init_t * p_twi_params_mpu, drv_motion_twi_init_t * p_twi_params_lis)
{
    uint32_t            err_code;
    drv_mpu9250_init_t  mpu_init_params;
    drv_acc_cfg_t       lis_init_params;

    NULL_PARAM_CHECK(evt_handler);

    NULL_PARAM_CHECK(p_twi_params_mpu);
    NULL_PARAM_CHECK(p_twi_params_mpu->p_twi_instance);

    NULL_PARAM_CHECK(p_twi_params_lis);
    NULL_PARAM_CHECK(p_twi_params_lis->p_twi_instance);

    lis_init_params.p_twi_instance = p_twi_params_lis->p_twi_instance;
    lis_init_params.p_twi_cfg      = p_twi_params_lis->p_twi_cfg;
    lis_init_params.twi_addr       = LIS2DH12_ADDR;
    lis_init_params.cpu_wake_pin   = LIS_INT1;

    mpu_init_params.p_twi_instance = p_twi_params_mpu->p_twi_instance;
    mpu_init_params.p_twi_cfg      = p_twi_params_mpu->p_twi_cfg;

    m_motion.evt_handler           = evt_handler;
    m_motion.features              = 0;
    m_motion.sensors               = 0;
    m_motion.dmp_features          = 0;
    m_motion.dmp_on                = false;
    m_motion.running               = false;
    m_motion.lp_accel_mode         = false;
    m_motion.do_temp               = false;
    m_motion.do_compass            = false;
    m_motion.do_pedo               = false;
    m_motion.pedo_interval_ms      = PEDO_READ_MS;
    m_motion.temp_interval_ms      = TEMP_READ_MS;
    m_motion.compass_interval_ms   = COMPASS_READ_MS;
    m_motion.motion_freq_hz        = DEFAULT_MPU_HZ;
    m_motion.wake_on_motion        = 1;

    err_code = drv_acc_init(&lis_init_params);
    RETURN_IF_ERROR(err_code);

    err_code = drv_mpu9250_init(&mpu_init_params);
    RETURN_IF_ERROR(err_code);

    /* Init power pin and power off the mpu9250 chip */
    err_code = drv_ext_gpio_cfg_output(SX_MPU_PWR_CTRL);
    RETURN_IF_ERROR(err_code);

    err_code = mpu9250_power(false);
    RETURN_IF_ERROR(err_code);

    /**@brief Init application timers */
    err_code = app_timer_create(&m_temp_timer_id, APP_TIMER_MODE_REPEATED, temp_timeout_handler);
    RETURN_IF_ERROR(err_code);

    err_code = app_timer_create(&m_compass_timer_id, APP_TIMER_MODE_REPEATED, compass_timeout_handler);
    RETURN_IF_ERROR(err_code);

    err_code = app_timer_create(&m_pedo_timer_id, APP_TIMER_MODE_REPEATED, pedo_timeout_handler);
    RETURN_IF_ERROR(err_code);


    return NRF_SUCCESS;
}
