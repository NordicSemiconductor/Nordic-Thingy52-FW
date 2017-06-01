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

 /** @file GPIO extender LED driver
 *
 * @defgroup gpio_ext_driver_led GPIO extender LED driver
 * @{
 * @ingroup gpio_ext_driver
 * @brief GPIO extender LED driver API.
 *
 */

#ifndef __DRV_EXT_LIGHT_H__
#define __DRV_EXT_LIGHT_H__

#include <stdint.h>
#include "app_error.h"
#include "drv_sx1509.h"
#include "sx150x_led_drv_regs.h"
#include "sx150x_led_drv_calc.h"
#include "app_timer.h"

#define DRV_EXT_LIGHT_INTENSITY_MAX ((uint8_t)(0xFF))   ///< Maximum light intensity value.
#define DRV_EXT_LIGHT_INTENSITY_OFF (0)                 ///< Minimum light intensity value (off).

#define DRV_EXT_LIGHT_MAX_PIN_ID_ALLOWED        16      ///< Highest I/O extender pin ID possible for a connected light. Dependent on driver implementation.
#define DRV_EXT_LIGHT_NUM_LIGHTS_MAX            (((DRV_SX1509_HIGHINPMODE_PIN15_Pos + 1) < (16)) ? \
                                                (DRV_SX1509_HIGHINPMODE_PIN15_Pos + 1) : (16))          ///< Maximum number of lights, depends on the IO extender and maximum of current implementation (16).
#define DRV_EXT_LIGHT_IOEXT_CLKX_DIV_MAX        (DRV_SX1509_MISC_CLKX_Msk >> DRV_SX1509_MISC_CLKX_Pos)  ///< Maximum ClkX divider.

/**@brief The GPIO extender LED driver status codes.
 */
enum
{
    DRV_EXT_LIGHT_STATUS_CODE_SUCCESS,            ///< Successful.
    DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM,      ///< Invalid parameters.
    DRV_EXT_LIGHT_STATUS_CODE_NOT_SUPPORTED,      ///< Not supported.
    DRV_EXT_LIGHT_STATUS_CODE_INVALID_LIGHT_TYPE, ///< The type of light is not supported.
    DRV_EXT_LIGHT_STATUS_CODE_INVALID_TIMER_VALUE ///< The desired time is too long. Increase the ClkX divider.
};

/**@brief Defines the frequency of the LED driver clock (ClkX) of the SX1509 according to ClkX = fOSC/2^(DRV_EXT_LIGHT_CLKX_DIV_n-1).
 * See SX1509 datasheet for details.
 */
typedef enum
{
    DRV_EXT_LIGHT_CLKX_DIV_1 = 1,
    DRV_EXT_LIGHT_CLKX_DIV_2 ,
    DRV_EXT_LIGHT_CLKX_DIV_4 ,
    DRV_EXT_LIGHT_CLKX_DIV_8 ,
    DRV_EXT_LIGHT_CLKX_DIV_16,
    DRV_EXT_LIGHT_CLKX_DIV_32,
    DRV_EXT_LIGHT_CLKX_DIV_64,
}drv_ext_light_clkx_div_t;

/**@brief The following color combinations are possible when using @ref drv_ext_light_rgb_sequence_t.
 */
typedef enum
{
    DRV_EXT_LIGHT_COLOR_NONE,
    DRV_EXT_LIGHT_COLOR_RED,
    DRV_EXT_LIGHT_COLOR_GREEN,
    DRV_EXT_LIGHT_COLOR_YELLOW,
    DRV_EXT_LIGHT_COLOR_BLUE,
    DRV_EXT_LIGHT_COLOR_PURPLE,
    DRV_EXT_LIGHT_COLOR_CYAN,
    DRV_EXT_LIGHT_COLOR_WHITE
}drv_ext_light_color_mix_t;

/**@brief The state of the IO extender oscillator for each light.
 */
typedef enum
{
    EXTENDER_OSC_UNUSED,            ///< Ext osc not used for this light.
    EXTENDER_OSC_USED_RUNNING,      ///< Ext osc running (Light currently on).
    EXTENDER_OSC_USED_PAUSED,       ///< Ext osc paused (Light currently off).
    EXTENDER_OSC_USED_PERM,         ///< Ext osc used on a permanent basis (light dimmed).
    IOEXT_OSC_STATUS_T_SIZE         ///< Number of different possible IO extender oscillator statuses.
}drv_ext_light_ioext_osc_status_t;

/**@brief Struct used by the app_timer handler, mainly for power saving.
 */
typedef struct
{
    uint32_t active_time_ms;                            ///< Total time for a light to be on or dimmed.
    uint32_t inactive_time_ms;                          ///< Total time for a light to be off.
    drv_ext_light_color_mix_t colors;                   ///< Color mix used for breathe, single-shot, or blink.
    drv_ext_light_ioext_osc_status_t ioext_osc_status;  ///< Status, holds information on the current use of the IO extender oscillator.
}drv_ext_light_status_t;

/**@brief Default initial value configuration for each light.
 */
#define DRV_EXT_LIGHT_STATUS_INIT                    \
{                                                    \
    .active_time_ms   = 0,                           \
    .inactive_time_ms = 0,                           \
    .colors           = DRV_EXT_LIGHT_COLOR_NONE,    \
    .ioext_osc_status = EXTENDER_OSC_UNUSED          \
}

/**@brief App timer and internal light timing struct.
 */
typedef struct
{
    drv_ext_light_status_t * p_status;      ///< Light status information.
    app_timer_id_t           timer;         ///< App timer information.
}drv_ext_light_data_t;

/**@brief Define to create timers associated with each light source.
 */
#define DRV_EXT_LIGHT_DEF(light_id)                                                                             \
    static drv_ext_light_status_t light_id##_timings = DRV_EXT_LIGHT_STATUS_INIT;                               \
    static app_timer_t light_id##_timer_data = { {0} };                                                         \
    static drv_ext_light_data_t light_id = {.p_status = &light_id##_timings, .timer = &light_id##_timer_data}  \

/**@brief Monochrome (single) or RGB light.
 */
typedef enum
{
    DRV_EXT_LIGHT_TYPE_MONO,        ///< Monochrome (single) LED.
    DRV_EXT_LIGHT_TYPE_RGB          ///< RGB LED (three IOs used).
}drv_ext_light_reg_type_t;

/**@brief Struct that defines the type of light (monochrome or RGB) and the corresponding connected pins.
 All lights are per definition sinked (The IO extender provides ground to the lights).*/
typedef struct
{
    drv_ext_light_reg_type_t type;  ///< Monochrome or RGB.
    union
    {
        uint8_t mono;               ///< Monochrome pin.
        struct
        {
            uint8_t r;              ///< RGB R pin.
            uint8_t g;              ///< RGB G pin.
            uint8_t b;              ///< RGB B pin.
        }rgb;
    }pin;
    drv_ext_light_data_t * p_data;        ///< Associated data for this light (app timer and timings).
}drv_ext_light_conf_t;

/**@brief Struct for initializing the light driver.
*/
typedef struct
{
    drv_ext_light_conf_t const * p_light_conf;          ///< Array of lights, each with their specific configuraion.
    uint8_t                      num_lights;            ///< Number of connected lights.
    drv_ext_light_clkx_div_t     clkx_div;              ///< IO extender clock divider.
    drv_sx1509_cfg_t const     * p_twi_conf;            ///< pointer to TWI (I2C) configuration for communication between master and IO extender.
    uint8_t                      app_timer_prescaler;   ///< Prescaler used for the app timer.
} drv_ext_light_init_t;

/**@brief Simplified struct for setting RGB intensity directly.
*/
typedef struct
{
    uint8_t r;          ///< Red intensity.
    uint8_t g;          ///< Green intensity.
    uint8_t b;          ///< Blue intensity.
}drv_ext_light_rgb_intensity_t;

/**@brief The color mix and sequence variables if a sequence (e.g. breathe) is used.
 */
typedef struct
{
    drv_ext_light_color_mix_t color;            ///< Color mix used for the sequence.
    drv_ext_light_sequence_t  sequence_vals;    ///< Defines sequence values: on/off/fade_in/fade_out times, and on/off intensities.
}drv_ext_light_rgb_sequence_t;

/**@brief Macro to initialize the sequence struct with default values.
 */
#define SEQUENCE_REAL_VAL_INIT(PARAM) drv_ext_light_sequence_t PARAM = SEQUENCE_DEFAULT_VAL

/**@brief Function for resetting all the registers of the IO extender to their default values.
 *
 * @warning  This reset applies to all registers, regardless if they were set by another driver outside of drv_ext_light.
 *
 * @return Status codes from this function. (Return codes may originate from other underlying modules/drivers as well which are not listed below).
 * @retval DRV_EXT_LIGHT_STATUS_CODE_SUCCESS
 */
ret_code_t drv_ext_light_reset(void);

/**@brief Turn a given light on.
 *
 * @param[in] id            Valid light ID.
 *
 * @note In case the given LED ID corresponds to an RGB light, all colors will be set to max intensity.
 *
 * @return Status codes from this function. (Return codes may originate from other underlying modules/drivers as well which are not listed below).
 * @retval DRV_EXT_LIGHT_STATUS_CODE_SUCCESS
 * @retval DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM
 */
ret_code_t drv_ext_light_on(uint32_t id);

/**@brief Turn a given light off.
 *
 * @param[in] id            Valid light ID (Has to be in the range [0, num_lights-1]).
 *
 * @note In case the given LED ID corresponds to a RGB light, all colors will be set to 0 intensity.
 *
 * @return Status codes from this function. (Return codes may originate from other underlying modules/drivers as well which are not listed below).
 * @retval DRV_EXT_LIGHT_STATUS_CODE_SUCCESS
 * @retval DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM
 */
ret_code_t drv_ext_light_off(uint32_t id);

/**@brief Function for setting intensity of monochrome light.
 *
 * @param[in] id            Specifies the ID of the given light. (Has to be in the range [0, num_lights-1]).
 * @param[in] intensity     (0-255).
 *
 * @return Status codes from this function. (Return codes may originate from other underlying modules/drivers as well which are not listed below).
 * @retval DRV_EXT_LIGHT_STATUS_CODE_SUCCESS
 * @retval DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM
 */
ret_code_t drv_ext_light_intensity_set(uint32_t id, uint8_t intensity);

/**@brief Function for setting color for RGB LEDs.
 *
 * @param[in] id            Specifies the ID of the given light. (Has to be in the range [0, num_lights-1]).
 * @param[in] intensity     (0-255).
 *
 * @return Status codes from this function. (Return codes may originate from other underlying modules/drivers as well which are not listed below).
 * @retval DRV_EXT_LIGHT_STATUS_CODE_SUCCESS
 * @retval DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM
 */
ret_code_t drv_ext_light_rgb_intensity_set(uint32_t id, drv_ext_light_rgb_intensity_t const * const p_intensity);

/**
 * @brief Set color for a given light through the GPIO extender.
 *
 * @param[in] id                Specifies the ID of the given light. (Has to be in the range [0, num_lights-1]).
 * @param[in,out] p_sequence    Sequence struct.
 *
 * @note    The actual timings in milliseconds will not necessarily correspond to the supplied times.
 *          The underlying functionality will choose register values to obtain timings as close as possible.
 *          Adjusting DRV_EXT_LIGHT_CLKX_DIV will influence the accuracy of this function.
 *          Also note that for the remaining intenstiry and fade parameters, values will be set as close as possible to the desired values.
 *          The user may read back p_sequence, which will be populated with the actual values.
 *
 * @return Status codes from this function. (Return codes may originate from other underlying modules/drivers as well which are not listed below).
 * @retval DRV_EXT_LIGHT_STATUS_CODE_SUCCESS
 * @retval DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM
 */
ret_code_t drv_ext_light_sequence(uint32_t id, drv_ext_light_sequence_t * const p_sequence);

/**
 * @brief Set color for a given light through the GPIO extender.
 *
 * @param[in] id                Specifies the ID of the given light. (Has to be in the range [0, num_lights-1]).
 * @param[in,out] p_sequence    RGB sequence struct.
 *s
 * @note    The actual timings in milliseconds will not necessarily correspond to the supplied times.
 *          The underlying functionality will choose register values to obtain timings as close as possible.
 *          Adjusting drv_ext_light_clkx_div_t::clkx_div supplied to drv_ext_light_init will influence the accuracy of this function.
 *          Also note that for the remaining intenstiry and fade parameters, values will be set as close as possible to the desired values.
 *          The user may read back p_sequence, which will be populated with the actual values.
 *
 * @return Status codes from this function. (Return codes may originate from other underlying modules/drivers as well which are not listed below).
 * @retval DRV_EXT_LIGHT_STATUS_CODE_SUCCESS
 * @retval DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM
 */
ret_code_t drv_ext_light_rgb_sequence(uint32_t id, drv_ext_light_rgb_sequence_t * const p_sequence);

/**@brief Function for initializing the drv_ext_light driver.
 *
 * @param[in] p_init            Contains the full configuration struct sent to drv_ext_light_init @ref drv_ext_light_init_t.
 * @param[in] on_init_reset     If true, the io extender will be reset on init. This will put the
 *                              io extender in a known state, but will delete any previous configurations, even from other drivers.
 *
 * @note Make sure that @ref APP_SCHED_INIT and @ref APP_TIMER_APPSH_INIT have been run prior to this function call.
 * Also note that this driver will take full control of the IO extender oscillator. This may influence other HW connected to the extender on other pins.
 * p_init->p_light_conf->p_data must reside in RAM.
 *
 * @return Status codes from this function. (Return codes may originate from other underlying modules/drivers as well which are not listed below).
 * @retval DRV_EXT_LIGHT_STATUS_CODE_SUCCESS
 * @retval DRV_EXT_LIGHT_STATUS_CODE_INVALID_PARAM
 */
ret_code_t drv_ext_light_init(drv_ext_light_init_t const * p_init, bool on_init_reset);

#endif

/** @} */
