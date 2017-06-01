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

#include "twi_manager.h"
#include "nrf_error.h"

#ifdef TWI_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

static app_irq_priority_t s_context_limit = APP_IRQ_PRIORITY_HIGHEST;
static uint32_t           s_collisions    = 0;

uint32_t twi_manager_request(nrf_drv_twi_t const *        p_instance,
                             nrf_drv_twi_config_t const * p_config,
                             nrf_drv_twi_evt_handler_t    event_handler,
                             void *                       p_context)
{
    uint32_t err_code;
    uint8_t current_context = current_int_priority_get();

    if (current_context < s_context_limit)
    {
        DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_RED"twi_manager_request"RTT_CTRL_RESET": current_context < s_context_limit %d\r\n", current_context);
        return NRF_ERROR_FORBIDDEN;
    }

    err_code = nrf_drv_twi_init(p_instance,
                                p_config,
                                event_handler,
                                p_context);
    if (err_code != NRF_SUCCESS)
    {
        s_collisions++;

        DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_RED"twi_manager_request"RTT_CTRL_RESET": collision %d\r\n", s_collisions);
        return err_code;
    }

    return NRF_SUCCESS;
}


uint32_t twi_manager_release(nrf_drv_twi_t const * p_instance)
{
    nrf_drv_twi_uninit(p_instance);

    return NRF_SUCCESS;
}


uint32_t twi_manager_collision_get(void)
{
    return s_collisions;
}


uint32_t twi_manager_collision_reset(void)
{
    s_collisions = 0;

    return NRF_SUCCESS;
}


uint32_t twi_manager_init(app_irq_priority_t context_limit)
{
    s_context_limit = context_limit;
    s_collisions    = 0;

    return NRF_SUCCESS;
}
