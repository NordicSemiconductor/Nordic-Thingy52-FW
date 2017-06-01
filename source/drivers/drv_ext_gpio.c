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

#include "drv_ext_gpio.h"
#include <stdint.h>
#include "drv_sx1509.h"
#include "app_util_platform.h"
#include "nrf_delay.h"

#ifdef EXT_GPIO_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

static drv_ext_gpio_init_t m_drv_ext_gpio;

#define RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code)                                                   \
if ((err_code) != NRF_SUCCESS)                                                                     \
{                                                                                                  \
    (void)SEGGER_RTT_printf(0, RTT_CTRL_TEXT_BRIGHT_RED                                            \
    "ERROR. Returned in file: %s, line: %d, with error code %d \r\n"RTT_CTRL_RESET,                \
    __FILE__, __LINE__, err_code);                                                                 \
    (void)drv_sx1509_close();                                                                      \
    return err_code;                                                                               \
}

#define VALID_PIN_CHECK(PIN)                              \
if ((PIN) > DRV_EXT_GPIO_PIN_HIGHEST_ID)                  \
{                                                         \
    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_RED"Pin number out of range. "RTT_CTRL_RESET": pin number: %d\r\n", PIN); \
    return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN;          \
}

#define VALID_PIN_MASK_CHECK(PIN_MASK)                    \
if ((PIN_MASK) >= (1 << DRV_EXT_GPIO_NUM_PINS_TOTAL))     \
{                                                         \
    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_RED"Pin mask out of range. "RTT_CTRL_RESET": pin mask: %x\r\n", PIN_MASK); \
    return(DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN);         \
}                                                         \
if(PIN_MASK == 0)                                         \
{                                                         \
    DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_RED"Pin mask is zero. "RTT_CTRL_RESET": pin mask: %x\r\n", PIN_MASK); \
    return(DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN);         \
}


uint32_t drv_ext_gpio_cfg(
    uint32_t                      pin_number,
    drv_ext_gpio_pin_dir_t        dir,
    drv_ext_gpio_pin_input_buf_t  input_buf,
    drv_ext_gpio_pin_pull_t       pull_config,
    drv_ext_gpio_pin_drive_type_t drive_type,
    drv_ext_gpio_pin_slew_rate_t  slew_rate)
{
    uint32_t err_code;
    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    if (pull_config == DRV_EXT_GPIO_PIN_NOPULL)
    {
        err_code = drv_sx1509_pullup_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

        err_code = drv_sx1509_pulldown_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (pull_config == DRV_EXT_GPIO_PIN_PULLDOWN)
    {
        err_code = drv_sx1509_pullup_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

        err_code = drv_sx1509_pulldown_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (pull_config == DRV_EXT_GPIO_PIN_PULLUP )
    {
        err_code = drv_sx1509_pulldown_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

        err_code = drv_sx1509_pullup_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    if (input_buf == DRV_EXT_GPIO_PIN_INPUT_BUF_ENABLED)
    {
        err_code = drv_sx1509_inpbufdisable_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (input_buf == DRV_EXT_GPIO_PIN_INPUT_BUF_DISABLED)
    {
        err_code = drv_sx1509_inpbufdisable_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    if (drive_type == DRV_EXT_GPIO_PIN_DRIVE_PUSHPULL)
    {
        err_code = drv_sx1509_opendrain_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (drive_type == DRV_EXT_GPIO_PIN_DRIVE_OPENDRAIN)
    {
        err_code = drv_sx1509_opendrain_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    if (slew_rate == DRV_EXT_GPIO_PIN_INCREASED_SLEWRATE_DISABLED)
    {
        err_code = drv_sx1509_longslewrate_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (slew_rate == DRV_EXT_GPIO_PIN_INCREASED_SLEWRATE_ENABLED)
    {
        err_code = drv_sx1509_longslewrate_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    if (dir == DRV_EXT_GPIO_PIN_DIR_OUTPUT)
    {
        err_code = drv_sx1509_dir_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (dir == DRV_EXT_GPIO_PIN_DIR_INPUT)
    {
        err_code = drv_sx1509_dir_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_reset(void)
{
    uint32_t err_code;

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_reset();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_cfg_output(uint32_t pin_number)
{
    uint32_t err_code;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_dir_modify(0, (1UL << pin_number));
    //(void)SEGGER_RTT_printf(0, RTT_CTRL_TEXT_BRIGHT_RED"drv_ext_gpio err code: %d pin: %d "RTT_CTRL_RESET"\n", err_code, pin_number);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_cfg_input(uint32_t pin_number, drv_ext_gpio_pin_pull_t pull_config)
{
    uint32_t err_code;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    if (pull_config == DRV_EXT_GPIO_PIN_NOPULL)
    {
        err_code = drv_sx1509_pullup_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

        err_code = drv_sx1509_pulldown_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (pull_config == DRV_EXT_GPIO_PIN_PULLDOWN)
    {
        err_code = drv_sx1509_pullup_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

        err_code = drv_sx1509_pulldown_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    }
    else if (pull_config == DRV_EXT_GPIO_PIN_PULLUP)
    {
        err_code = drv_sx1509_pulldown_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

        err_code = drv_sx1509_pullup_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    err_code = drv_sx1509_dir_modify((1UL << pin_number), 0);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pin_dir_modify(uint32_t pin_number, drv_ext_gpio_pin_dir_t direction)
{
    uint32_t err_code;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    if (direction == DRV_EXT_GPIO_PIN_DIR_OUTPUT)
    {
        err_code = drv_sx1509_dir_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (direction == DRV_EXT_GPIO_PIN_DIR_INPUT)
    {
        err_code = drv_sx1509_dir_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pin_slew_rate_modify(uint32_t pin_number, drv_ext_gpio_pin_slew_rate_t slew_rate)
{
    uint32_t err_code;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    if (slew_rate == DRV_EXT_GPIO_PIN_INCREASED_SLEWRATE_DISABLED)
    {
        err_code = drv_sx1509_longslewrate_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (slew_rate == DRV_EXT_GPIO_PIN_INCREASED_SLEWRATE_ENABLED)
    {
        err_code = drv_sx1509_longslewrate_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pin_drive_type_modify(uint32_t pin_number, drv_ext_gpio_pin_drive_type_t drive_type)
{
    uint32_t err_code;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    if (drive_type == DRV_EXT_GPIO_PIN_DRIVE_PUSHPULL)
    {
        err_code = drv_sx1509_opendrain_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (drive_type == DRV_EXT_GPIO_PIN_DRIVE_OPENDRAIN)
    {
        err_code = drv_sx1509_opendrain_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pin_input_buffer_modify(uint32_t pin_number, drv_ext_gpio_pin_input_buf_t input_buf)
{
    uint32_t err_code;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    if (input_buf == DRV_EXT_GPIO_PIN_INPUT_BUF_ENABLED)
    {
        err_code = drv_sx1509_inpbufdisable_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else if (input_buf == DRV_EXT_GPIO_PIN_INPUT_BUF_DISABLED)
    {
        err_code = drv_sx1509_inpbufdisable_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM;
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pin_set(uint32_t pin_number)
{
    uint32_t err_code;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_data_modify((1UL << pin_number), 0);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pins_set(uint32_t pin_mask)
{
    uint32_t err_code;

    VALID_PIN_MASK_CHECK(pin_mask);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_data_modify(pin_mask, 0);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pin_clear(uint32_t pin_number)
{
    uint32_t err_code;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_data_modify(0, (1UL << pin_number));
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pins_clear(uint32_t pin_mask)
{
    uint32_t err_code;

    VALID_PIN_MASK_CHECK(pin_mask);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_data_modify(0, pin_mask);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pin_toggle(uint32_t pin_number)
{
    uint32_t err_code;
    uint16_t port_data;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_data_get(&port_data);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    if (port_data & (1UL << pin_number))
    {
        err_code = drv_sx1509_data_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = drv_sx1509_data_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pins_toggle(uint32_t pin_mask)
{
    uint32_t err_code;
    uint16_t port_data;
    uint16_t set_mask;
    uint16_t clr_mask;

    VALID_PIN_MASK_CHECK(pin_mask);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_data_get(&port_data);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    clr_mask = port_data & pin_mask;
    set_mask = ( (~clr_mask) & pin_mask);

    err_code = drv_sx1509_data_modify(set_mask, clr_mask);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pin_write(uint32_t pin_number, uint32_t value)
{
    uint32_t err_code;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    if (value == 0)
    {
        err_code = drv_sx1509_data_modify(0, (1UL << pin_number));
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }
    else
    {
        err_code = drv_sx1509_data_modify((1UL << pin_number), 0);
        RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);
    }

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pin_read(uint32_t pin_number, uint32_t * const p_pin)
{
    uint32_t err_code;
    uint16_t raw_data;

    VALID_PIN_CHECK(pin_number);

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_data_get(&raw_data);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    *p_pin = (raw_data >> pin_number) & 0x0001;

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_pins_read(uint32_t * const p_pins)
{
    uint32_t err_code;

    err_code = drv_sx1509_open(m_drv_ext_gpio.p_cfg);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    *p_pins = 0;

    err_code = drv_sx1509_data_get((uint16_t*)p_pins);
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    err_code = drv_sx1509_close();
    RETURN_IF_ERROR_EXT_GPIO_CLOSE(err_code);

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}


uint32_t drv_ext_gpio_init(drv_ext_gpio_init_t const * const p_init, bool on_init_reset)
{
    uint32_t err_code;

    NULL_PARAM_CHECK(p_init);
    m_drv_ext_gpio.p_cfg = p_init->p_cfg;
    drv_sx1509_init();

    if (on_init_reset)
    {
        err_code = drv_ext_gpio_reset();
        if (err_code != DRV_SX1509_STATUS_CODE_SUCCESS)
        {
            return err_code;            // Usual RETURN_IF_ERROR_EXT_GPIO_CLOSE not used as this would call drv_sx1509_close() twice
        }
    }

    return DRV_EXT_GPIO_STATUS_CODE_SUCCESS;
}
