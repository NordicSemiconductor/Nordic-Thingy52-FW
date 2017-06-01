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

/** @file Accelerometer
 *
 * @defgroup driver_acc Accelerometer
 * @{
 * @ingroup drivers
 * @brief Low power accelerometer driver API.
 *
 */
#ifndef __DRV_ACC_H__
#define __DRV_ACC_H__

#include <stdbool.h>
#include <stdint.h>
#include "nrf_drv_twi.h"

/**@brief The return status codes.
 */
enum
{
    DRV_ACC_STATUS_CODE_SUCCESS,            ///< Successful.
    DRV_ACC_STATUS_CODE_INVALID_PARAM,      ///< Invalid parameters.
    DRV_ACC_STATUS_WRONG_DEVICE,            ///< Wrong device at I2C (TWI) address.
    DRV_ACC_STATUS_UNINITALIZED,            ///< The driver is unitialized, please initialize.
};

/**@brief LIS3DH accelerometer configuration struct.
 */
typedef struct
{
    uint8_t                      twi_addr;        ///< TWI (I2C) address of device.
    nrf_drv_twi_t        const * p_twi_instance;  ///< The instance of TWI master to be used for transactions.
    nrf_drv_twi_config_t const * p_twi_cfg;       ///< The TWI configuration to use while the driver is enabled.
    uint8_t                      cpu_wake_pin;    ///< Pin on the nRF used to wake from sleep.
} drv_acc_cfg_t;

/**@brief Prepare for sleep and subsequent wakeup.
 *
 * @param[in] p_wakeup  If false, go to deepest sleep and do not prepare to wake up the system.
 *
 * @note   When the nRF prepares for sleep, the LIS3DH will wake up.
 *
 * @retval DRV_ACC_STATUS_CODE_SUCCESS
 * @retval DRV_ACC_STATUS_CODE_INVALID_PARAM
 * @retval DRV_ACC_STATUS_UNINITALIZED
 * @return Other codes from the underlying drivers.
 */
ret_code_t drv_acc_wakeup_prepare(bool p_wakeup);

/**@brief Accelerometer driver initialization.
 *
 * @param[in] p_cfg     Initialization struct.
 *
 * @retval DRV_ACC_STATUS_CODE_SUCCESS
 * @retval DRV_ACC_STATUS_WRONG_DEVICE
 * @retval NRF_ERROR_NULL
 * @return Other codes from the underlying drivers.
 */
ret_code_t drv_acc_init(drv_acc_cfg_t const * const p_cfg);

#endif /* __DRV_ACC_H__ */

/** @} */
