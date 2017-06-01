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

/** @file GPIO extender
 *
 * @defgroup gpio_ext_driver GPIO extender
 * @{
 * @ingroup drivers
 * @brief GPIO extender API.
 *
 */

#ifndef __DRV_EXT_GPIO_H__
#define __DRV_EXT_GPIO_H__

#include <stdint.h>
#include "nrf_error.h"
#include "drv_sx1509.h"

/**@brief The GPIO extender status codes.
 */
enum
{
    DRV_EXT_GPIO_STATUS_CODE_SUCCESS = NRF_SUCCESS,  ///< Successfull.
    DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM,          ///< Invalid parameters.
    DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN             ///< Invalid GPIO pin or pin mask selected.
};

#define DRV_EXT_GPIO_PIN_HIGHEST_ID DRV_SX1509_DATA_PIN15_Pos           ///< Highest pin ID.
#define DRV_EXT_GPIO_NUM_PINS_TOTAL (DRV_EXT_GPIO_PIN_HIGHEST_ID + 1)   ///< Number of pins total.

/**@brief Pin direction definitions.
 */
typedef enum
{
    DRV_EXT_GPIO_PIN_DIR_OUTPUT,    ///< Output.
    DRV_EXT_GPIO_PIN_DIR_INPUT      ///< Input.
} drv_ext_gpio_pin_dir_t;

/**@brief Connection of input buffer.
 */
typedef enum
{
    DRV_EXT_GPIO_PIN_INPUT_BUF_ENABLED,   ///< Connect input buffer.
    DRV_EXT_GPIO_PIN_INPUT_BUF_DISABLED   ///< Disconnect input buffer.
} drv_ext_gpio_pin_input_buf_t;

/**@brief Pin to be pulled down, up, or no-pull.
 */
typedef enum
{
    DRV_EXT_GPIO_PIN_NOPULL,    ///< No pin pullup or pulldown.
    DRV_EXT_GPIO_PIN_PULLDOWN,  ///< Pin pulldown resistor enabled.
    DRV_EXT_GPIO_PIN_PULLUP     ///< Pin pullup resistor enabled.
} drv_ext_gpio_pin_pull_t;

/**@brief Push-pull or open drain operation.
 */
typedef enum
{
    DRV_EXT_GPIO_PIN_DRIVE_PUSHPULL,    ///< Standard push-pull operation.
    DRV_EXT_GPIO_PIN_DRIVE_OPENDRAIN    ///< Open drain.
}drv_ext_gpio_pin_drive_type_t;

/**@brief Regular or increased slew rate.
 */
typedef enum
{
    DRV_EXT_GPIO_PIN_INCREASED_SLEWRATE_DISABLED, ///< No slew rate alteration.
    DRV_EXT_GPIO_PIN_INCREASED_SLEWRATE_ENABLED   ///< Increased slew rate.
}drv_ext_gpio_pin_slew_rate_t;

/**@brief Pin configuration function.
 *
 * @note The main pin configuration function.
 * This function allows to set any aspect in PIN_CNF register.
 *
 * @param[in] pin_number  Specifies the pin number (Allowed values: 0 to DRV_EXT_GPIO_PIN_HIGHEST_ID).
 * @param[in] dir         Pin direction.
 * @param[in] input       Connect or disconnect input buffer.
 * @param[in] pull        Pull configuration.
 * @param[in] drive type  Push-pull or open drain.
 * @param[in] slew_rate   Normal or increased slew rate.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM   Invalid parameters supplied.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_cfg(
    uint32_t                      pin_number,
    drv_ext_gpio_pin_dir_t        dir,
    drv_ext_gpio_pin_input_buf_t  input_buf,
    drv_ext_gpio_pin_pull_t       pull_config,
    drv_ext_gpio_pin_drive_type_t drive_type,
    drv_ext_gpio_pin_slew_rate_t  slew_rate);

/**@brief GPIO extender driver configuration.
 */
typedef struct
{
    drv_sx1509_cfg_t const * p_cfg;
} drv_ext_gpio_init_t;

/**@brief Function for resetting all the registers of the GPIO extender to their default values.
 *
 * @note  This reset applies to all registers, regardless if they were set by another driver outside of drv_ext_gpio.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_reset(void);

/**@brief Function for configuring the given GPIO pin_number as output with given initial value set, hiding inner details.
 *
 * @param[in] pin_number    The pin number (allowed values 0-DRV_EXT_GPIO_PIN_HIGHEST_ID).
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_cfg_output(uint32_t pin_number);

/**@brief Function for configuring the given GPIO pin number as input with given initial value set, hiding inner details.
 *
 * @param[in] pin_number    The pin number (allowed values 0-DRV_EXT_GPIO_PIN_HIGHEST_ID).
 * @param[in] pull_config   State of the pin range pull resistor (no pull, pulled down, or pulled high).
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM   Invalid parameters supplied.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_cfg_input(uint32_t pin_number, drv_ext_gpio_pin_pull_t pull_config);

/**@brief Function for setting the direction of a GPIO pin.
 *
 * @param[in] pin_number    The pin number (allowed values 0-DRV_EXT_GPIO_PIN_HIGHEST_ID).
 * @param[in] direction     The direction.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM   Invalid parameters supplied.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pin_dir_modify(uint32_t pin_number, drv_ext_gpio_pin_dir_t direction);

/**@brief Function for enabling or disabling increased slew rate for a given GPIO pin.
 *
 * @param[in] pin_number    The pin number (allowed values 0-DRV_EXT_GPIO_PIN_HIGHEST_ID).
 * @param[in] direction     The direction.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM   Invalid parameters supplied.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pin_slew_rate_modify(uint32_t pin_number, drv_ext_gpio_pin_slew_rate_t slew_rate);

/**@brief Function for setting the drive type a given GPIO pin.
 *
 * @param[in] pin_number    The pin number (allowed values 0-DRV_EXT_GPIO_PIN_HIGHEST_ID).
 * @param[in] drive type    Pin drive: push-pull or open drain.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM   Invalid parameters supplied.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pin_drive_type_modify(uint32_t pin_number, drv_ext_gpio_pin_drive_type_t drive_type);

/**@brief Function for enabling and disabling the input buffer.
 *
 * @param[in] pin_number    The pin number (allowed values 0-DRV_EXT_GPIO_PIN_HIGHEST_ID).
 * @param[in] input buffer  Disabled or enabled.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PARAM   Invalid parameters supplied.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pin_input_buffer_modify(uint32_t pin_number, drv_ext_gpio_pin_input_buf_t input_buf);

/**@brief Function for setting a GPIO pin.
 *
 * @note The pin must be configured as an output for this function to have any effect.
 *
 * @param[in] pin_number    The pin number (0-DRV_EXT_GPIO_PIN_HIGHEST_ID) to set.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pin_set(uint32_t pin_number);

/**@brief Function for setting GPIO pins.
 *
 * @note Pins must be configured as outputs for this function to have any effect.
 *
 * @param[in] pin_mask  Specifies the pins to set.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pins_set(uint32_t pin_mask);

/**@brief Function for clearing a GPIO pin.
 *
 * @note The pin must be configured as an output for this function to have any effect.
 *
 * @param[in] pin_number    The pin number (0-DRV_EXT_GPIO_PIN_HIGHEST_ID) to clear.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pin_clear(uint32_t pin_number);

/**@brief Function for clearing GPIO pins.
 *
 * @note Pins must be configured as outputs for this function to have any effect.
 *
 * @param[in] pin_mask  Pins to clear.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pins_clear(uint32_t pin_mask);

/**@brief Function for toggling a GPIO pin.
 *
 * @note The pin must be configured as an output for this function to have any effect.
 *
 * @param[in] pin_number    Pin number (0-DRV_EXT_GPIO_PIN_HIGHEST_ID) to toggle.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pin_toggle(uint32_t pin_number);

/**@brief Function for toggling GPIO pins.
 *
 * @note Pins must be configured as outputs for this function to have any effect.
 *
 * @param[in] pin_mask  Pins to toggle.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pins_toggle(uint32_t pin_mask);

/**@brief Function for writing a value to a GPIO pin.
 *
 * @note The pin must be configured as an output for this function to have any effect.
 *
 * @param[in] pin_number    The pin number (0-DRV_EXT_GPIO_PIN_HIGHEST_ID) to write.
 * @param[in] value         Specifies the value to be written to the pin.
 * @arg 0 clears the pin.
 * @arg >=1 sets the pin.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pin_write(uint32_t pin_number, uint32_t value);

/**@brief Function for reading the input level of a GPIO pin.
 *
 * @note The pin must have input connected for the value returned from this function to be valid.
 *
 * @param[in] pin_number    The pin number (0-DRV_EXT_GPIO_PIN_HIGHEST_ID) to read.
 * @param[out] p_pin        Pin value.
 * @retval 0 if the pin input level is low.
 * @retval 1 if the pin input level is high.
 * @retval > 1 should never occur.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pin_read(uint32_t pin_number, uint32_t * const p_pin);

/**@brief Function for reading the input level of all GPIO pins.
 *
 * @note The pins must have input connected for the value returned from this function to be valid.
 * @note The most significant 16 bits will always be zero, due to 16 pins on the IO extender.
 *
 * @param[out] p_pins   State of all pins.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return DRV_EXT_GPIO_STATUS_CODE_INVALID_PIN     Pin number not available.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_pins_read(uint32_t * const p_pins);

/**@brief Function for initializing the GPIO extender.
 *
 * @param[in] drv_ext_gpio_init_t   Configuration struct.
 * @param[in] on_init_reset         If true, the IO extender will be reset on init. This will put the
 * IO extender in a known state, but will delete any previous configurations, even from other drivers.
 *
 * @return DRV_EXT_GPIO_STATUS_CODE_SUCCESS         If the call was successful.
 * @return Other codes from the underlying driver.
 */
uint32_t drv_ext_gpio_init(drv_ext_gpio_init_t const * const p_init, bool on_init_reset);

#endif

/** @} */
