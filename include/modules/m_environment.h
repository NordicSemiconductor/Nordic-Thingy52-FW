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

 /** @file Environment module
 *
 * @defgroup m_environment Environment
 * @{
 * @ingroup modules
 * @brief Environment module API.
 *
 */

#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include <stdint.h>
#include <stdbool.h>
#include "m_ble.h"
#include "ble_tes.h"
#include "nrf_drv_twi.h"

/**@brief Initialization parameters. */
typedef struct
{
    const nrf_drv_twi_t * p_twi_instance;
} m_environment_init_t;

/**@brief Weather station default configuration. */
#define ENVIRONMENT_CONFIG_DEFAULT {        \
    .temperature_interval_ms = 2000,        \
    .pressure_interval_ms    = 2000,        \
    .humidity_interval_ms    = 2000,        \
    .color_interval_ms       = 1500,        \
    .color_config =                         \
    {                                       \
        .led_red             = 103,         \
        .led_green           = 78,          \
        .led_blue            = 29           \
    },                                      \
    .gas_interval_mode       = GAS_MODE_10S \
}

#define ENVIRONMENT_BASELINE_DEFAULT {      \
        .mode_250ms = 0,                    \
        .mode_1s    = 0,                    \
        .mode_10s   = 0,                    \
        .mode_60s   = 0,                    \
}

/**@brief Function for starting the environment module.
 *
 * @details This function should be called after m_environment_init to start the environment module.
 *
 * @retval NRF_SUCCESS If initialization was successful.
 */
uint32_t m_environment_start(void);

/**@brief Function for stopping the environment module.
 *
 * @details This function should be called after m_environment_start to stop the environment module.
 *
 * @retval NRF_SUCCESS If initialization was successful.
 */
uint32_t m_environment_stop(void);

/**@brief Function for initializing the environment module.
 *
 * @param[in] p_handle    Pointer to the location to store the service handle.
 * @param[in] p_params    Pointer to the init parameters.
 */
uint32_t m_environment_init(m_ble_service_handle_t * p_handle, m_environment_init_t * p_params);
#endif

/** @} */
