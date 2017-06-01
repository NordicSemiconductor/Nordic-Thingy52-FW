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

#include "drv_gas_sensor.h"
#include "pca20020.h"
#include "drv_ext_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"
#include "app_scheduler.h"
#include "sdk_errors.h"
#include "drv_ccs811.h"
#include "drv_ccs811_bitfields.h"

#ifdef GAS_SENSOR_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

#define GAS_SENSOR_ID                       (0x81)   ///< HW ID of the gas sensor.
#define GAS_SENSOR_TDWAKE_ENABLE_DELAY_US     (20)   ///< minimum time between sensor I2C accesses.
#define GAS_SENSOR_TAWAKE_ENABLE_DELAY_US     (50)   ///< us for sensor to become active after nWAKE signal.
#define GAS_SENSOR_PWR_ON_DELAY_MS            (30)   ///< ms for sensor to become active after pwr on. NB: May be up tp 70 ms for first boot after gas sensor firmware download. 20 ms from datasheet + margin.

/**@brief Reads the ERROR_ID register from the gas sensor.
 */
#define CCS811_PRINT_IF_SENSOR_ERROR                                                                                                                           \
{                                                                                                                                                              \
    uint8_t err_id;                                                                                                                                            \
    if (drv_ccs811_err_id_get(&err_id))                                                                                                                        \
    {                                                                                                                                                          \
        DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_RED"Error from CCS811, could not read error ID. file: %s, line: %d  "RTT_CTRL_RESET"\n", __FILE__, __LINE__);     \
    }                                                                                                                                                          \
                                                                                                                                                               \
    if (err_id)                                                                                                                                                \
    {                                                                                                                                                          \
        DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_RED"Error from CCS811, code: 0x%x, file: %s, line: %d "RTT_CTRL_RESET"\n", err_id, __FILE__, __LINE__);           \
    }                                                                                                                                                          \
}


static drv_ccs811_cfg_t              m_drv_ccs811_cfg = {0}; ///< Driver configuration.
static drv_gas_sensor_data_handler_t m_data_handler;         ///< Gas sensor data handler.
static bool                          m_started = false;      ///< Indicates if the gas sensor is started.

/**@brief Open gas sensor TWI interface.
 */
static __inline ret_code_t ccs811_open(void)
{
    ret_code_t err_code;

    nrf_delay_us(GAS_SENSOR_TDWAKE_ENABLE_DELAY_US);

    err_code = drv_ext_gpio_pin_clear(SX_CCS_WAKE);
    RETURN_IF_ERROR(err_code);

    nrf_delay_us(GAS_SENSOR_TAWAKE_ENABLE_DELAY_US);  // Delay for gas sensor to become active according to datasheet.

    return drv_ccs811_open(&m_drv_ccs811_cfg);
}


/**@brief Close gas sensor TWI interface.
 */
static __inline ret_code_t ccs811_close(void)
{
    ret_code_t err_code;

    err_code = drv_ccs811_close();
    RETURN_IF_ERROR(err_code);

    err_code = drv_ext_gpio_pin_set(SX_CCS_WAKE);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


/**@brief Verfies the hardware ID of the gas sensor.
 */
static ret_code_t hw_id_verify(void)
{
    uint8_t  hw_id = 0;
    bool     hw_id_correct = false;

    if(ccs811_open() == NRF_SUCCESS)
    {
        if( drv_ccs811_hw_id_get(&hw_id) == NRF_SUCCESS )
        {
            CCS811_PRINT_IF_SENSOR_ERROR;
            hw_id_correct = ( hw_id == GAS_SENSOR_ID ) ? true : false;
        }
    }

    (void)ccs811_close();

    return hw_id_correct == true ? NRF_SUCCESS : NRF_ERROR_NOT_FOUND;
}


/**@brief Verifes that a valid app version is running on the gas sensor.
 */
static ret_code_t valid_app_verify(void)
{
    ret_code_t  err_code;
    uint8_t     status;

    err_code = ccs811_open();
    RETURN_IF_ERROR(err_code);

    err_code = drv_ccs811_status_get(&status);
    RETURN_IF_ERROR(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    err_code = ccs811_close();
    RETURN_IF_ERROR(err_code);

    if (status & DRV_CCS811_STATUS_APP_VALID_Msk)
    {
        return NRF_SUCCESS;
    }
    return NRF_ERROR_NOT_FOUND;
}

#ifdef GAS_SENSOR_DEBUG
    /**@brief Gets all hardware and firmware version information from the gas sensor.
    */
    static ret_code_t gas_sensor_info_get(uint8_t * p_hw_id, uint8_t * p_hw_version, uint16_t * p_fw_boot_version, uint16_t * p_fw_app_version)
    {
        ret_code_t  err_code;

        err_code = ccs811_open();
        RETURN_IF_ERROR(err_code);

        err_code = drv_ccs811_hw_id_get(p_hw_id);
        RETURN_IF_ERROR(err_code);
        CCS811_PRINT_IF_SENSOR_ERROR;

        err_code = drv_ccs811_hw_version_get(p_hw_version);
        RETURN_IF_ERROR(err_code);
        CCS811_PRINT_IF_SENSOR_ERROR;

        err_code = drv_ccs811_fw_boot_version_get(p_fw_boot_version);
        RETURN_IF_ERROR(err_code);
        CCS811_PRINT_IF_SENSOR_ERROR;

        err_code = drv_ccs811_fw_app_version_get(p_fw_app_version);
        RETURN_IF_ERROR(err_code);
        CCS811_PRINT_IF_SENSOR_ERROR;

        err_code = ccs811_close();
        RETURN_IF_ERROR(err_code);

        return NRF_SUCCESS;
    }
#endif


/**@brief Get result from gas sensor.
 */
static void gpiote_evt_sceduled(void * p_event_data, uint16_t event_size)
{
    ret_code_t                      err_code;
    drv_ccs811_alg_result_descr_t   result_descr = DRV_CCS811_ALG_RESULT_DESCR_ALL;
    static drv_ccs811_alg_result_t  s_result;

    DEBUG_PRINTF(0, "Gas sensor pin interrupt detected. \r\n");

    err_code = ccs811_open();
    APP_ERROR_CHECK(err_code);

    err_code = drv_ccs811_alg_result_data_get(result_descr, &s_result);
    APP_ERROR_CHECK(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    err_code = ccs811_close();
    APP_ERROR_CHECK(err_code);

    m_data_handler((drv_gas_sensor_data_t *)&s_result);
}


/**@brief GPIOTE event handler for gas sensor. Called when the gas sensor signals a pin interrupt (sample finished).
 */
static void gpiote_evt_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    ret_code_t err_code;

    if ((pin == CCS_INT) && (nrf_gpio_pin_read(CCS_INT) == 0)) // Check that pin is low (interrupt has occured);
    {
        err_code = app_sched_event_put(0, 0, gpiote_evt_sceduled);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Initlialize the GPIOTE for capturing gas sensor pin interrupts.
 */
static ret_code_t gpiote_init(uint32_t pin)
{
    ret_code_t err_code;

    if (!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();
        RETURN_IF_ERROR(err_code);
    }

    nrf_drv_gpiote_in_config_t gpiote_in_config;

    gpiote_in_config.is_watcher  = false;
    gpiote_in_config.hi_accuracy = false;
    gpiote_in_config.pull        = NRF_GPIO_PIN_PULLUP;
    gpiote_in_config.sense       = NRF_GPIOTE_POLARITY_TOGGLE;
    err_code = nrf_drv_gpiote_in_init(pin, &gpiote_in_config, gpiote_evt_handler);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}

/**@brief Uninitialize the GPIOTE system.
 */
static ret_code_t gpiote_uninit(uint32_t pin)
{
    nrf_drv_gpiote_in_uninit(pin);

    return NRF_SUCCESS;

}


/**@brief Configures the IO pins of the host controller.
 */
static ret_code_t io_pins_init(void)
{
    ret_code_t    err_code;

    err_code = drv_ext_gpio_cfg_output(SX_CCS_PWR_CTRL);
    RETURN_IF_ERROR(err_code);

    err_code = drv_ext_gpio_cfg_output(SX_CCS_RESET);
    RETURN_IF_ERROR(err_code);

    err_code = drv_ext_gpio_cfg_output(SX_CCS_WAKE);
    RETURN_IF_ERROR(err_code);

    #if defined(THINGY_HW_v0_7_0)
        while (drv_ext_gpio_pin_clear(SX_CCS_PWR_CTRL) != NRF_SUCCESS);
    #elif defined(THINGY_HW_v0_8_0)
        while (drv_ext_gpio_pin_clear(SX_CCS_PWR_CTRL) != NRF_SUCCESS);
    #elif defined(THINGY_HW_v0_9_0)
        while (drv_ext_gpio_pin_clear(SX_CCS_PWR_CTRL) != NRF_SUCCESS);
    #else
        while (drv_ext_gpio_pin_set(SX_CCS_PWR_CTRL)   != NRF_SUCCESS);
    #endif

    err_code = drv_ext_gpio_pin_set(SX_CCS_RESET);
    RETURN_IF_ERROR(err_code);

    err_code = drv_ext_gpio_pin_clear(SX_CCS_WAKE);
    RETURN_IF_ERROR(err_code);

    nrf_delay_ms(GAS_SENSOR_PWR_ON_DELAY_MS); // Allow the CCS811 to power up.

    return NRF_SUCCESS;
}


ret_code_t drv_gas_sensor_calibrate_humid_temp(uint16_t rh_ppth, int32_t temp_mdeg)
{
    ret_code_t err_code;

    err_code = ccs811_open();
    RETURN_IF_ERROR(err_code);

    err_code = drv_ccs811_env_data_set(rh_ppth, temp_mdeg);
    RETURN_IF_ERROR(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    err_code = ccs811_close();
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


ret_code_t drv_gas_sensor_baseline_get(uint16_t * p_baseline)
{
    ret_code_t err_code;

    err_code = ccs811_open();
    RETURN_IF_ERROR(err_code);

    err_code = drv_ccs811_baseline_get(p_baseline);
    RETURN_IF_ERROR(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    err_code = ccs811_close();
    RETURN_IF_ERROR(err_code);

    DEBUG_PRINTF(0, "Gas sensor baseline get:, 0x%x \n", *p_baseline);

    return NRF_SUCCESS;
}


ret_code_t drv_gas_sensor_baseline_set(uint16_t baseline)
{
    ret_code_t err_code;

    err_code = ccs811_open();
    RETURN_IF_ERROR(err_code);

    err_code = drv_ccs811_baseline_set(baseline);
    RETURN_IF_ERROR(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    err_code = ccs811_close();
    RETURN_IF_ERROR(err_code);

    DEBUG_PRINTF(0, "Gas sensor baseline set:, 0x%x \n", baseline);

    return NRF_SUCCESS;
}


ret_code_t drv_gas_sensor_raw_data_get(uint8_t * p_current_selected, uint16_t * p_raw_adc_reading)
{
    ret_code_t err_code;

    err_code = ccs811_open();
    RETURN_IF_ERROR(err_code);

    uint16_t raw_data;
    err_code = drv_ccs811_raw_data_get(&raw_data);
    RETURN_IF_ERROR(err_code);

    err_code = ccs811_close();
    RETURN_IF_ERROR(err_code);

    *p_current_selected = (raw_data >> DRV_CCS811_RAW_DATA_CURRSEL_Pos);
    *p_raw_adc_reading = (raw_data & DRV_CCS811_RAW_DATA_RAWADC_Msk);

    DEBUG_PRINTF(0, "Gas sensor raw data get. Current: %d [uA], ADC read: %d \n", *p_current_selected, *p_raw_adc_reading);

    return NRF_SUCCESS;
}


ret_code_t drv_gas_sensor_start(drv_gas_sensor_mode_t mode)
{
    ret_code_t  err_code;
    uint8_t     status;
    uint8_t     meas_mode_reg;
    uint8_t     drive_mode = DRV_CCS811_MEAS_MODE_DRIVE_MODE_Idle;

    if (m_started)
    {
        return NRF_SUCCESS;
    }

    m_started = true;

    switch (mode)
    {
        case DRV_GAS_SENSOR_MODE_250MS:
            return NRF_ERROR_NOT_SUPPORTED;
        case DRV_GAS_SENSOR_MODE_1S:
            drive_mode = DRV_CCS811_MEAS_MODE_DRIVE_MODE_ConstPwr;
            break;
        case DRV_GAS_SENSOR_MODE_10S:
            drive_mode = DRV_CCS811_MEAS_MODE_DRIVE_MODE_PulseHeat;
            break;
        case DRV_GAS_SENSOR_MODE_60S:
            drive_mode = DRV_CCS811_MEAS_MODE_DRIVE_MODE_LowPwrPulseHeat;
            break;
        default:
            return NRF_ERROR_INVALID_PARAM;
    }

    // Power on gas sensor.
    #if defined(THINGY_HW_v0_7_0)
        while (drv_ext_gpio_pin_clear(SX_CCS_PWR_CTRL) != NRF_SUCCESS);
    #elif defined(THINGY_HW_v0_8_0)
        while (drv_ext_gpio_pin_clear(SX_CCS_PWR_CTRL) != NRF_SUCCESS);
    #elif defined(THINGY_HW_v0_9_0)
        while (drv_ext_gpio_pin_clear(SX_CCS_PWR_CTRL) != NRF_SUCCESS);
    #else
        while (drv_ext_gpio_pin_set(SX_CCS_PWR_CTRL)   != NRF_SUCCESS);
    #endif

    err_code = drv_ext_gpio_pin_clear(SX_CCS_WAKE);
    APP_ERROR_CHECK(err_code);

    err_code = drv_ext_gpio_pin_set(SX_CCS_RESET);
    APP_ERROR_CHECK(err_code);

    nrf_delay_ms(GAS_SENSOR_PWR_ON_DELAY_MS); // Allow the CCS811 to power up.

    err_code = gpiote_init(CCS_INT);
    APP_ERROR_CHECK(err_code);
    nrf_drv_gpiote_in_event_enable(CCS_INT, true);

    err_code = ccs811_open();
    APP_ERROR_CHECK(err_code);

    err_code = drv_ccs811_app_start();
    APP_ERROR_CHECK(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    err_code = drv_ccs811_status_get(&status);
    APP_ERROR_CHECK(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    if (!(status & DRV_CCS811_STATUS_FW_MODE_Msk))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (status & DRV_CCS811_STATUS_DATA_READY_Msk)
    {
        // Dummy readout
        drv_ccs811_alg_result_descr_t result_descr = DRV_CCS811_ALG_RESULT_DESCR_ALL;
        drv_ccs811_alg_result_t       result;

        err_code = drv_ccs811_alg_result_data_get(result_descr, &result);
        APP_ERROR_CHECK(err_code);
        CCS811_PRINT_IF_SENSOR_ERROR;
    }

    err_code = drv_ccs811_meas_mode_get(&meas_mode_reg);
    RETURN_IF_ERROR(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    meas_mode_reg |= (drive_mode << DRV_CCS811_MEAS_MODE_DRIVE_MODE_Pos) |
                     (DRV_CCS811_MEAS_MODE_INTERRUPT_Enable << DRV_CCS811_MEAS_MODE_INTERRUPT_Pos);

    err_code = drv_ccs811_meas_mode_modify(meas_mode_reg, 0);
    RETURN_IF_ERROR(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    err_code = ccs811_close();
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


ret_code_t drv_gas_sensor_stop(void)
{
    ret_code_t  err_code;
    uint8_t     status;

    if (m_started == false)
    {
        return NRF_SUCCESS;
    }

    m_started = false;

    nrf_drv_gpiote_in_event_disable(CCS_INT);
    err_code = gpiote_uninit(CCS_INT);
    RETURN_IF_ERROR(err_code);

    err_code = ccs811_open();
    RETURN_IF_ERROR(err_code);

    err_code = drv_ccs811_status_get(&status);
    RETURN_IF_ERROR(err_code);
    CCS811_PRINT_IF_SENSOR_ERROR;

    if (status & DRV_CCS811_STATUS_FW_MODE_Msk) // Chech that the device is in "application mode" and not "boot mode".
    {
        // Disable interrupt and set idle mode.
        err_code = drv_ccs811_meas_mode_modify(0, DRV_CCS811_MEAS_MODE_DRIVE_MODE_Msk | DRV_CCS811_MEAS_MODE_INTERRUPT_Msk);
        RETURN_IF_ERROR(err_code);
        CCS811_PRINT_IF_SENSOR_ERROR;
    }

    err_code = ccs811_close();
    RETURN_IF_ERROR(err_code);

    err_code = drv_ext_gpio_pin_clear(SX_CCS_RESET);
    RETURN_IF_ERROR(err_code);

    err_code = drv_ext_gpio_pin_clear(SX_CCS_WAKE);
    RETURN_IF_ERROR(err_code);

    // Power off gas sensor.
    #if defined(THINGY_HW_v0_7_0)
        err_code = drv_ext_gpio_pin_set(SX_CCS_PWR_CTRL);
    #elif defined(THINGY_HW_v0_8_0)
        err_code = drv_ext_gpio_pin_set(SX_CCS_PWR_CTRL);
    #elif defined(THINGY_HW_v0_9_0)
        err_code = drv_ext_gpio_pin_set(SX_CCS_PWR_CTRL);
    #else
        err_code = drv_ext_gpio_pin_clear(SX_CCS_PWR_CTRL);
    #endif
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}

ret_code_t drv_gas_sensor_init(drv_gas_init_t * p_init)
{
    ret_code_t err_code;

    NULL_PARAM_CHECK(p_init);
    NULL_PARAM_CHECK(p_init->p_twi_instance);
    NULL_PARAM_CHECK(p_init->p_twi_cfg);
    NULL_PARAM_CHECK(p_init->data_handler);

    m_drv_ccs811_cfg.p_twi_cfg      = p_init->p_twi_cfg;
    m_drv_ccs811_cfg.p_twi_instance = p_init->p_twi_instance;
    m_drv_ccs811_cfg.twi_addr       = p_init->twi_addr;
    m_data_handler                  = p_init->data_handler;

    drv_ccs811_init();

    err_code = io_pins_init();
    RETURN_IF_ERROR(err_code);

    err_code = hw_id_verify();
    RETURN_IF_ERROR(err_code);

    err_code = valid_app_verify();
    RETURN_IF_ERROR(err_code);

    #ifdef GAS_SENSOR_DEBUG // Print all info from the gas sensor.
        uint8_t  hw_id;
        uint8_t  hw_version;
        uint16_t fw_boot_version;
        uint16_t fw_app_version;

        err_code = gas_sensor_info_get(&hw_id, &hw_version, &fw_boot_version, &fw_app_version);
        RETURN_IF_ERROR(err_code);

        DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_GREEN"Gas sensor: HW ID: 0x%x, HW version: 0x%x, FW boot version: 0x%x, FW app version: 0x%x "RTT_CTRL_RESET"\n", hw_id, hw_version, fw_boot_version, fw_app_version);
    #endif

    err_code = drv_ext_gpio_pin_clear(SX_CCS_RESET);
    RETURN_IF_ERROR(err_code);

    err_code = drv_ext_gpio_pin_clear(SX_CCS_WAKE);
    RETURN_IF_ERROR(err_code);

    #if defined(THINGY_HW_v0_7_0)
        err_code = drv_ext_gpio_pin_set(SX_CCS_PWR_CTRL);
    #elif defined(THINGY_HW_v0_8_0)
        err_code = drv_ext_gpio_pin_set(SX_CCS_PWR_CTRL);
    #elif defined(THINGY_HW_v0_9_0)
        err_code = drv_ext_gpio_pin_set(SX_CCS_PWR_CTRL);
    #else
        err_code = drv_ext_gpio_pin_clear(SX_CCS_PWR_CTRL);
    #endif
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}
