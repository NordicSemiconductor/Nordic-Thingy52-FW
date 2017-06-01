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

 /** @file Pressure sensor
 *
 * @defgroup press_driver Pressure sensor
 * @{
 * @ingroup drivers
 * @brief Pressure sensor API.
 *
 */

#ifndef __DRV_PRESSURE_H__
#define __DRV_PRESSURE_H__

#include "nrf_drv_twi.h"
#include <stdint.h>

/**@brief Pressure driver event types.
 */
typedef enum
{
    DRV_PRESSURE_EVT_DATA,    /**<Converted value ready to be read*/
    DRV_PRESSURE_EVT_ERROR    /**<HW error on the communication bus*/
}drv_pressure_evt_type_t;

/**@brief Pressure modes of operation.
 */
typedef enum
{
    DRV_PRESSURE_MODE_ALTIMETER,
    DRV_PRESSURE_MODE_BAROMETER
}drv_pressure_mode_t;

/**@brief Pressure event struct.
 */
typedef struct
{
    drv_pressure_evt_type_t type;
    drv_pressure_mode_t     mode;
}drv_pressure_evt_t;

/**@brief Pressure driver event handler callback type.
 */
typedef void (*drv_pressure_evt_handler_t)(drv_pressure_evt_t const * p_evt);

/**@brief Initialization struct for pressure driver.
 */
typedef struct
{
    uint8_t                      twi_addr;          ///< TWI address.
    uint32_t                     pin_int;           ///< Interrupt pin.
    nrf_drv_twi_t        const * p_twi_instance;    ///< The instance of TWI master to be used for transactions.
    nrf_drv_twi_config_t const * p_twi_cfg;         ///< The TWI configuration to use while the driver is enabled.
    drv_pressure_evt_handler_t   evt_handler;       ///< Event handler - called after a pin interrupt has been detected.
    drv_pressure_mode_t          mode;              ///< Current mode of operation.
}drv_pressure_init_t;

/**@brief Function for initializing the pressure driver.
 *
 * @param[in] p_params      Pointer to init parameters.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 * @retval NRF_ERROR_INVALID_STATE If the driver is in invalid state.
 */
uint32_t drv_pressure_init(drv_pressure_init_t * p_params);

/**@brief Function for enabling the pressure sensor.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 */
uint32_t drv_pressure_enable(void);

/**@brief Function for disabling the pressure sensor.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 */
uint32_t drv_pressure_disable(void);

/**@brief Function for resetting the chip to all default register values.
*
* @retval NRF_SUCCESS             If operation was successful.
* @retval NRF_ERROR_BUSY          If TWI bus was busy.
*/
uint32_t drv_pressure_reset(void);

/**@brief Function for changing the mode of the pressure sensor.
 *
 * @param[in] mode                 Altimeter or Barometer.
 *
 * @retval NRF_SUCCESS             If configuration was successful.
 * @retval NRF_ERROR_BUSY          If the TWI drivers are busy.
 */
uint32_t drv_pressure_mode_set(drv_pressure_mode_t mode);

/**@brief Function for getting the pressure data [hPa].
 *
 * @retval Pressure data.
 */
float drv_pressure_get(void);

/**@brief Function to start sampling.
 *
 * @retval NRF_SUCCESS             If start sampling was successful.
 */
uint32_t drv_pressure_sample(void);

/**@brief Function to put sensor in sleep.
 *
 * @retval NRF_SUCCESS             If sleep was successful.
 */
uint32_t drv_pressure_sleep(void);

#endif

/** @} */
