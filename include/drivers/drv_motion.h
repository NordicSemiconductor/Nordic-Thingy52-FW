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

 /** @file Motion sensor
 *
 * @defgroup motion_driver Motion sensor
 * @{
 * @ingroup drivers
 * @brief Motion sensor API.
 *
 */

#ifndef __DRV_MOTION_H__
#define __DRV_MOTION_H__

#include <stdint.h>
#include <stddef.h>
#include "nrf_drv_twi.h"

/**@brief Motion features.
 */
typedef enum
{
    DRV_MOTION_FEATURE_RAW_ACCEL,
    DRV_MOTION_FEATURE_RAW_GYRO,
    DRV_MOTION_FEATURE_RAW_COMPASS,
    DRV_MOTION_FEATURE_QUAT,
    DRV_MOTION_FEATURE_EULER,
    DRV_MOTION_FEATURE_ROT_MAT,
    DRV_MOTION_FEATURE_HEADING,
    DRV_MOTION_FEATURE_GRAVITY_VECTOR,
    DRV_MOTION_FEATURE_TAP,
    DRV_MOTION_FEATURE_ORIENTATION,
    DRV_MOTION_FEATURE_PEDOMETER,
    DRV_MOTION_FEATURE_WAKE_ON_MOTION
}drv_motion_feature_t;

typedef uint32_t drv_motion_feature_mask_t;

#define DRV_MOTION_FEATURE_MASK_RAW               ((1UL << DRV_MOTION_FEATURE_RAW_ACCEL) | (1UL << DRV_MOTION_FEATURE_RAW_COMPASS) | (1UL << DRV_MOTION_FEATURE_RAW_GYRO))
#define DRV_MOTION_FEATURE_MASK_RAW_ACCEL         (1UL << DRV_MOTION_FEATURE_RAW_ACCEL)
#define DRV_MOTION_FEATURE_MASK_RAW_GYRO          (1UL << DRV_MOTION_FEATURE_RAW_GYRO)
#define DRV_MOTION_FEATURE_MASK_RAW_COMPASS       (1UL << DRV_MOTION_FEATURE_RAW_COMPASS)
#define DRV_MOTION_FEATURE_MASK_QUAT              (1UL << DRV_MOTION_FEATURE_QUAT)
#define DRV_MOTION_FEATURE_MASK_EULER             (1UL << DRV_MOTION_FEATURE_EULER)
#define DRV_MOTION_FEATURE_MASK_ROT_MAT           (1UL << DRV_MOTION_FEATURE_ROT_MAT)
#define DRV_MOTION_FEATURE_MASK_HEADING           (1UL << DRV_MOTION_FEATURE_HEADING)
#define DRV_MOTION_FEATURE_MASK_GRAVITY_VECTOR    (1UL << DRV_MOTION_FEATURE_GRAVITY_VECTOR)
#define DRV_MOTION_FEATURE_MASK_TAP               (1UL << DRV_MOTION_FEATURE_TAP)
#define DRV_MOTION_FEATURE_MASK_ORIENTATION       (1UL << DRV_MOTION_FEATURE_ORIENTATION)
#define DRV_MOTION_FEATURE_MASK_PEDOMETER         (1UL << DRV_MOTION_FEATURE_PEDOMETER)
#define DRV_MOTION_FEATURE_MASK_WAKE_ON_MOTION    (1UL << DRV_MOTION_FEATURE_WAKE_ON_MOTION)

#define DRV_MOTION_FEATURE_MASK                   (DRV_MOTION_FEATURE_MASK_RAW_ACCEL      |     \
                                                   DRV_MOTION_FEATURE_MASK_RAW_GYRO       |     \
                                                   DRV_MOTION_FEATURE_MASK_RAW_COMPASS    |     \
                                                   DRV_MOTION_FEATURE_MASK_QUAT           |     \
                                                   DRV_MOTION_FEATURE_MASK_EULER          |     \
                                                   DRV_MOTION_FEATURE_MASK_ROT_MAT        |     \
                                                   DRV_MOTION_FEATURE_MASK_HEADING        |     \
                                                   DRV_MOTION_FEATURE_MASK_GRAVITY_VECTOR |     \
                                                   DRV_MOTION_FEATURE_MASK_TAP            |     \
                                                   DRV_MOTION_FEATURE_MASK_ORIENTATION    |     \
                                                   DRV_MOTION_FEATURE_MASK_PEDOMETER      |     \
                                                   DRV_MOTION_FEATURE_MASK_WAKE_ON_MOTION)

#define DRV_MOTION_FEATURE_DMP_MASK               (DRV_MOTION_FEATURE_MASK_QUAT           |     \
                                                   DRV_MOTION_FEATURE_MASK_EULER          |     \
                                                   DRV_MOTION_FEATURE_MASK_ROT_MAT        |     \
                                                   DRV_MOTION_FEATURE_MASK_HEADING        |     \
                                                   DRV_MOTION_FEATURE_MASK_GRAVITY_VECTOR |     \
                                                   DRV_MOTION_FEATURE_MASK_TAP            |     \
                                                   DRV_MOTION_FEATURE_MASK_ORIENTATION    |     \
                                                   DRV_MOTION_FEATURE_MASK_PEDOMETER)

#define DRV_MOTION_FEATURE_GESTURE_MASK           (DRV_MOTION_FEATURE_MASK_TAP            |     \
                                                   DRV_MOTION_FEATURE_MASK_ORIENTATION    |     \
                                                   DRV_MOTION_FEATURE_MASK_PEDOMETER)

#define DRV_MOTION_FEATURE_CONTINUOS_MASK         (DRV_MOTION_FEATURE_MASK_RAW_ACCEL      |     \
                                                   DRV_MOTION_FEATURE_MASK_RAW_GYRO       |     \
                                                   DRV_MOTION_FEATURE_MASK_RAW_COMPASS    |     \
                                                   DRV_MOTION_FEATURE_MASK_QUAT           |     \
                                                   DRV_MOTION_FEATURE_MASK_EULER          |     \
                                                   DRV_MOTION_FEATURE_MASK_ROT_MAT        |     \
                                                   DRV_MOTION_FEATURE_MASK_HEADING        |     \
                                                   DRV_MOTION_FEATURE_MASK_GRAVITY_VECTOR)

/**@brief Motion event types.
 */
typedef enum
{
    DRV_MOTION_EVT_RAW,
    DRV_MOTION_EVT_QUAT,
    DRV_MOTION_EVT_EULER,
    DRV_MOTION_EVT_ROT_MAT,
    DRV_MOTION_EVT_HEADING,
    DRV_MOTION_EVT_GRAVITY,
    DRV_MOTION_EVT_TAP,
    DRV_MOTION_EVT_ORIENTATION,
    DRV_MOTION_EVT_PEDOMETER
}drv_motion_evt_t;

/**@brief Motion driver event handler callback type.
 */
typedef void (*drv_motion_evt_handler_t)(drv_motion_evt_t const * p_evt, void * p_data, uint32_t size);

/**@brief Motion configuration struct.
 */
typedef struct
{
    uint16_t pedo_interval_ms;
    uint16_t temp_interval_ms;
    uint16_t compass_interval_ms;
    uint16_t motion_freq_hz;
    uint8_t  wake_on_motion;
}drv_motion_cfg_t;

/**@brief Motion initialization struct.
 */
typedef struct
{
    nrf_drv_twi_t         const * p_twi_instance;
    nrf_drv_twi_config_t  const * p_twi_cfg;
}drv_motion_twi_init_t;


/**@brief Function for initializing the motion driver.
 *
 * @param[in] evt_handler       The motion event handler.
 * @param[in] p_params_mpu      Pointer to the init parameter structure for the MPU9250.
 * @param[in] p_params_lis      Pointer to the init parameter structure for the LIS2DH12.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t drv_motion_init(drv_motion_evt_handler_t evt_handler, drv_motion_twi_init_t * p_params_mpu, drv_motion_twi_init_t * p_params_lis);

/**@brief Function for enabling features in the motion driver.
 *
 * @param[in] feature_mask      Feature mask telling what features to enable.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t drv_motion_enable(drv_motion_feature_mask_t feature_mask);

/**@brief Function to disable features in the motion driver.
 *
 * @param[in] feature_mask      Feature mask telling what features to disable.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t drv_motion_disable(drv_motion_feature_mask_t feature_mask);

/**@brief Function to configure the motion driver.
 *
 * @param[in] cfg      Configuration structure.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t drv_motion_config(drv_motion_cfg_t * p_cfg);

/**@brief Function to prepare for sleep mode.
 *
 * @param[in] wakeup   Boolean indicating wakeup or sleep for ever.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t drv_motion_sleep_prepare(bool wakeup);
#endif

/** @} */
