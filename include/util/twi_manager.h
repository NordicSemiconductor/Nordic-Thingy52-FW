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

#ifndef __TWI_MANAGER_H__
#define __TWI_MANAGER_H__

#include "nrf_drv_twi.h"
#include "app_util_platform.h"

/**@brief Init TWI manager
 *
 * @param[in] context_limit  Protecting TWI driver init function not to be called from a higher priority than specified.
 *
 * @return NRF_SUCCESS upon success.
 */
uint32_t twi_manager_init(app_irq_priority_t context_limit);

/**@brief Wrapper for TWI driver init.
 *
 * @param[in] p_instance     pointer to TWI instance.
 * @param[in] p_config       pointer to TWI configuration.
 * @param[in] event_handler  TWI event handler.
 * @param[in] p_context      Context passed to TWI event handler.
 *
 * @return NRF_SUCCESS upon success.
 */
uint32_t twi_manager_request(nrf_drv_twi_t const *        p_instance,
                             nrf_drv_twi_config_t const * p_config,
                             nrf_drv_twi_evt_handler_t    event_handler,
                             void *                       p_context);

/**@brief Wrapper for TWI driver uninit.
*
* @param[in] p_instance     pointer to TWI instance.
*
* @return NRF_SUCCESS upon success.
*/
uint32_t twi_manager_release(nrf_drv_twi_t const * p_instance);

/**@brief Function for getting number of TWI collisions.
*
* @return Number of collisions.
*/
uint32_t twi_manager_collision_get(void);

/**@brief Function for resetting the collision counter.
*
* @return NRF_SUCCESS upon success.
*/
uint32_t twi_manager_collision_reset(void);

#endif
