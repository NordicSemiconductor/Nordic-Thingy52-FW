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
#include "sdk_errors.h"
#include "nrf.h"
#include <stdint.h>
#include <stdbool.h>
#define  NRF_LOG_MODULE_NAME "support_func  "
#include "nrf_log.h"
#include "nrf_delay.h"
#include "pca20020.h"
#include "drv_ext_gpio.h"
#include "macros_common.h"

ret_code_t support_func_ble_mac_address_get(char * p_mac_addr)
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
    
    char mac_addr[SUPPORT_FUNC_MAC_ADDR_STR_LEN] = {'\0'};

    for (uint8_t i = BLE_GAP_ADDR_LEN; i > 0; i--) // Reverse MAC address.
    {
        if (i != BLE_GAP_ADDR_LEN)
        {
            sprintf(mac_addr + strlen(mac_addr),":");
        }
        
        sprintf(mac_addr + strlen(mac_addr),"%02x", device_addr.addr[i-1]);
    }
    
    strcpy(p_mac_addr, mac_addr);

    return NRF_SUCCESS;
}


bool support_func_sys_halt_debug_enabled(void)
{
    return(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk);
}


/**@brief Configures a single IO extender pin.
 */
static ret_code_t ioext_pin_cfg(uint8_t pin, sx_gpio_cfg_t ioext_sys_pin_cfg)
{
    ret_code_t err_code;
    
    err_code = drv_ext_gpio_cfg(pin,
                     ioext_sys_pin_cfg.dir,
                     ioext_sys_pin_cfg.input_buf,
                     ioext_sys_pin_cfg.pull_config,
                     ioext_sys_pin_cfg.drive_type,
                     ioext_sys_pin_cfg.slew_rate);
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


/**@brief Configures a single nRF pin.
 */
static ret_code_t nrf_pin_cfg(uint8_t pin, nrf_gpio_cfg_t  nrf_sys_pin_cfg)
{
    if (pin != VDD_PWR_CTRL) // Do not alter the state of the VDD_PWR pin.
    {
        nrf_gpio_cfg(pin,
                     nrf_sys_pin_cfg.dir,
                     nrf_sys_pin_cfg.input,
                     nrf_sys_pin_cfg.pull,
                     nrf_sys_pin_cfg.drive,
                     nrf_sys_pin_cfg.sense);

        switch(nrf_sys_pin_cfg.state)
        {
            case PIN_CLEAR:
                nrf_gpio_pin_clear(pin);
                break;

            case PIN_SET:
                nrf_gpio_pin_set(pin);
                break;

            case PIN_NO_OUTPUT:
                // Do nothing.
                break;

            default:
                RETURN_IF_ERROR(NRF_ERROR_NOT_SUPPORTED);
        }
    }

    return NRF_SUCCESS;
}    


/**@brief Traverses all IO extender pins and calls ioext_pin_cfg.
 */
static ret_code_t configure_default_ioext_gpio_state(bool boot)
{
    ret_code_t err_code;
    
    sx_gpio_cfg_t  ioext_sys_pin_cfg[SX_IOEXT_NUM_PINS] = IOEXT_SYSTEM_DEFAULT_PIN_CFG;

    /* Set all IO extender pins in default state. IO extender will be powered down as well,
    Hence, this config will not be retained when VDD is turned off. */
    
    // Prior to setting direction, ensure that all ouput data buffers contains the correct value.
    if (boot)
    {
        uint16_t ext_gpio_init_pin_state = 0;
        
        for (uint8_t i = 0; i < SX_IOEXT_NUM_PINS; i++)
        {
            if (ioext_sys_pin_cfg[i].state == PIN_SET)
            {
                ext_gpio_init_pin_state |= (1 << i);
            }
        }
        
        err_code = drv_ext_gpio_reg_data_init(ext_gpio_init_pin_state);
        RETURN_IF_ERROR(err_code);
    }
    
    for (uint8_t i = 0; i < SX_IOEXT_NUM_PINS; i++)
    {
        err_code = ioext_pin_cfg(i, ioext_sys_pin_cfg[i]); 
        RETURN_IF_ERROR(err_code);
    }

    return NRF_SUCCESS;
}


/**@brief Traverses all nRF pins and calls nrf_pin_cfg.
 */
static ret_code_t configure_default_nrf_gpio_state(void)
{
    ret_code_t err_code;
        
    nrf_gpio_cfg_t   nrf_sys_pin_cfg[NRF_NUM_GPIO_PINS] = NRF_SYSTEM_DEFAULT_PIN_CFG;

    // Set all nRF pins in desired default state.
    for (uint8_t i = 0; i < NRF_NUM_GPIO_PINS; i++)
    {
        err_code = nrf_pin_cfg(i, nrf_sys_pin_cfg[i]);
        RETURN_IF_ERROR(err_code);
    }
    
    return NRF_SUCCESS;
}


/**@brief Traverses all nRF pins in reverse order and calls nrf_pin_cfg.
 */
static ret_code_t configure_default_nrf_gpio_state_reversed(void)
{
    ret_code_t err_code;
    
    nrf_gpio_cfg_t   nrf_sys_pin_cfg[NRF_NUM_GPIO_PINS] = NRF_SYSTEM_DEFAULT_PIN_CFG;
    
    // Set all nRF pins in desired default state.
    for (uint8_t i = NRF_NUM_GPIO_PINS; i > 0; i--)
    {
        err_code = nrf_pin_cfg(i - 1, nrf_sys_pin_cfg[i - 1]);
        RETURN_IF_ERROR(err_code);
    }
    
    return NRF_SUCCESS;
}


ret_code_t support_func_configure_io_startup(drv_ext_gpio_init_t const * const p_ext_gpio_init)
{
    ret_code_t err_code;
    
    err_code = configure_default_nrf_gpio_state();
    RETURN_IF_ERROR(err_code);
    
    nrf_gpio_cfg_output(VDD_PWR_CTRL);
    nrf_gpio_pin_set(VDD_PWR_CTRL);
    nrf_delay_ms(5);
    
    err_code = drv_ext_gpio_init(p_ext_gpio_init, true);
    RETURN_IF_ERROR(err_code);

    err_code = configure_default_ioext_gpio_state(true);
    RETURN_IF_ERROR(err_code);
    
    return NRF_SUCCESS;
}


ret_code_t support_func_configure_io_shutdown(void)
{
    ret_code_t err_code;
    
    err_code = configure_default_ioext_gpio_state(false);
    
    #if defined(THINGY_HW_v0_7_0) ||  defined(THINGY_HW_v0_8_0) || defined(THINGY_HW_v0_9_0)
        RETURN_IF_ERROR(err_code);
    #else
        if (err_code) // Ignore errors and continue shutdown procedure.
        {
            NRF_LOG_ERROR("configure_default_ioext_gpio_state returned with code %d \r\n", err_code);
        }
    #endif
    
    err_code = configure_default_nrf_gpio_state_reversed();
    
    #if defined(THINGY_HW_v0_7_0) ||  defined(THINGY_HW_v0_8_0) || defined(THINGY_HW_v0_9_0)
        RETURN_IF_ERROR(err_code);
    #else
        if (err_code) // Ignore errors and continue shutdown procedure.
        {
            NRF_LOG_ERROR("configure_default_nrf_gpio_state_reversed returned with code %d \r\n", err_code);
        }
    #endif

    nrf_gpio_cfg_output(VDD_PWR_CTRL);
    
    #if defined(THINGY_HW_v0_7_0) ||  defined(THINGY_HW_v0_8_0) || defined(THINGY_HW_v0_9_0)
        nrf_gpio_pin_set(VDD_PWR_CTRL);     // For earlier HW versions, keep VDD on to the low power accelerometer.
    #else
        nrf_gpio_pin_clear(VDD_PWR_CTRL);
    #endif
    
    return NRF_SUCCESS;
}
