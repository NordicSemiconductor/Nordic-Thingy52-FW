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

 /** @file Motion module
 *
 * @defgroup m_motion Motion
 * @{
 * @ingroup modules
 * @brief Motion module API.
 *
 */

#ifndef __M_MOTION_H__
#define __M_MOTION_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "m_ble.h"
#include "nrf_drv_twi.h"

/**@brief Motion module default configuration.
 */
#define MOTION_DEFAULT_CONFIG     \
{                                 \
    .pedo_interval_ms    = 1000,  \
    .temp_interval_ms    = 500,   \
    .compass_interval_ms = 500,   \
    .motion_freq_hz      = 10,    \
    .wake_on_motion      = 1      \
}

/**@brief TWI configuration struct.
 */
typedef struct
{
    nrf_drv_twi_t         const * p_twi_instance;
}m_motion_init_t;

/**@brief Function to initialize motion module.
 *
 * @param[out] p_handle   Handle for BLE services.
 * @param[in]  p_params   Init parameters.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t m_motion_init(m_ble_service_handle_t * p_handle, m_motion_init_t * p_params);

/**@brief Function to prepare for sleep mode.
 *
 * @param[in] wakeup   Boolean indicating wakeup or sleep for ever.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t m_motion_sleep_prepare(bool wakeup);

#endif

/** @} */
