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

#include "support_func.h"
#include "softdevice_handler.h"
#include "macros_common.h"
#include "sdk_errors.h"
#include "nrf.h"
#include "SEGGER_RTT.h"
#include <stdint.h>
#include <stdbool.h>

ret_code_t support_func_ble_mac_address_print_rtt(void)
{
    ret_code_t err_code;

    uint8_t softdevice_enabled;
    err_code = sd_softdevice_is_enabled(&softdevice_enabled);
    RETURN_IF_ERROR(err_code);

    if(!softdevice_enabled) // The softdevice needs to be enabled.
    {
        return NRF_ERROR_SOFTDEVICE_NOT_ENABLED;
    }

    ble_gap_addr_t device_addr;
    err_code = sd_ble_gap_addr_get(&device_addr);
    RETURN_IF_ERROR(err_code);

    (void)SEGGER_RTT_printf(0, "Device MAC addr (hex): ");

    for (uint8_t i = BLE_GAP_ADDR_LEN; i > 0; i--) // Reverse MAC address.
    {
        if (i == BLE_GAP_ADDR_LEN)
        {
            (void)SEGGER_RTT_printf(0, " ");
        }
        else
        {
            (void)SEGGER_RTT_printf(0, ":");
        }
        
        (void)SEGGER_RTT_printf(0, "%02x", device_addr.addr[i-1]);
    }

    (void)SEGGER_RTT_printf(0, "\n");

    return NRF_SUCCESS;
}

bool support_func_sys_halt_debug_enabled(void)
{
    return(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk);
}
