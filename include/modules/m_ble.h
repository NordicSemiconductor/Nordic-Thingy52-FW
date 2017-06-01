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

 /** @file BLE handling module
 *
 * @defgroup m_ble BLE
 * @{
 * @ingroup modules
 * @brief BLE handling module API.
 *
 */

#ifndef __THINGY_BLE_H__
#define __THINGY_BLE_H__

#include <stdint.h>
#include "thingy_config.h"
#include "ble.h"

/**@brief BLE event types.
 */
typedef enum
{
    thingy_ble_evt_connected,
    thingy_ble_evt_disconnected,
    thingy_ble_evt_timeout
}m_ble_evt_type_t;

/**@brief BLE event structure.
 */
typedef struct
{
    m_ble_evt_type_t evt_type;
    void             * p_data;
    uint32_t           size;
}m_ble_evt_t;

typedef void (*m_ble_evt_handler_t)(m_ble_evt_t * p_evt);

/**@brief  BLE service callback definitions
*/
typedef void (*m_ble_service_evt_cb_t)(ble_evt_t * p_ble_evt);
typedef uint32_t (*m_ble_service_init_cb_t)(void);

/**@brief BLE service handle structure.
*/
typedef struct
{
    m_ble_service_init_cb_t    init_cb;
    m_ble_service_evt_cb_t  ble_evt_cb;
}m_ble_service_handle_t;

/**@brief Initialization parameters.
*/
typedef struct
{
    m_ble_evt_handler_t      evt_handler;
    m_ble_service_handle_t * p_service_handles;
    uint32_t                 service_num;
}m_ble_init_t;

/**@brief Function for initializing the BLE handling module..
 *
 * @param[in] p_params    Pointer to the init parameters.
 */
uint32_t m_ble_init(m_ble_init_t * p_params);

/**@brief Function for starting the BLE handling module.
 *
 * @details This function should be called after thingy_ble_init to start the BLE module.
 *
 * @retval NRF_SUCCESS If initialization was successful.
 */
uint32_t m_ble_start(void);

/**@brief Function for stopping the BLE handling module.
 *
 * @details This function should be called after thingy_ble_start to stop the BLE module.
 *
 * @retval NRF_SUCCESS If initialization was successful.
 */
uint32_t m_ble_stop(void);

#endif

/** @} */
