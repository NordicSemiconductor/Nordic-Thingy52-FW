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

/** @file Humidity sensor
 *
 * @defgroup humidity_driver Humidity sensor
 * @{
 * @ingroup drivers
 * @brief Humidity sensor API.
 *
 */

#ifndef __DRV_HUMIDITY_H__
#define __DRV_HUMIDITY_H__

#include "nrf_drv_twi.h"
#include <stdint.h>

/**@brief Humidity driver event types.
 */
typedef enum
{
    DRV_HUMIDITY_EVT_DATA,    /**<Converted value ready to be read.*/
    DRV_HUMIDITY_EVT_ERROR    /**<Hardware error on the communication bus.*/
}drv_humidity_evt_t;

/**@brief Humidity driver event handler callback type.
 */
typedef void (*drv_humidity_evt_handler_t)(drv_humidity_evt_t evt);

/**@brief Initialization struct for the humidity driver.
 */
typedef struct
{
    uint8_t                      twi_addr;
    uint32_t                     pin_int;
    nrf_drv_twi_t        const * p_twi_instance;  ///< The instance of TWI master to be used for transactions.
    nrf_drv_twi_config_t const * p_twi_cfg;       ///< The TWI configuration to use while the driver is enabled.
    drv_humidity_evt_handler_t   evt_handler;
}drv_humidity_init_t;

/**@brief Function for initializing the humidity driver.
 *
 * @param[in] p_params      Pointer to init parameters.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 * @retval NRF_ERROR_INVALID_STATE If the driver is in invalid state.
 */
uint32_t drv_humidity_init(drv_humidity_init_t * p_params);

/**@brief Function for enabling the humidity sensor.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 */
uint32_t drv_humidity_enable(void);

/**@brief Function for disabling the humidity sensor.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 */
uint32_t drv_humidity_disable(void);

/**@brief Function for resetting the chip to all default register values
*
* @retval NRF_SUCCESS             If operation was successful
* @retval NRF_ERROR_BUSY          If TWI bus was busy
*/
uint32_t drv_humidity_reset(void);

/**@brief Function for getting the humidity data.
 *
 * @retval Humidity data.
 */
int16_t drv_humidity_get(void);

/**@brief Function for getting the temperature data.
 *
 * @retval Temperature data.
 */
float drv_humidity_temp_get(void);

/**@brief Function to start sampling.
 *
 * @retval NRF_SUCCESS             If start sampling was successful.
 */
uint32_t drv_humidity_sample(void);

#endif

/** @} */
