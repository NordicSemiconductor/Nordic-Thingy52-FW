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

 /** @file UI module
 *
 * @defgroup m_ui User interface
 * @{
 * @ingroup modules
 * @brief User interface module API.
 *
 */

#ifndef __M_UI_H__
#define __M_UI_H__

#include "app_error.h"
#include "m_ble.h"
#include "nrf_drv_twi.h"

#define DEFAULT_LED_INTENSITY_PERCENT     20    ///< Default LED intensity [percent].
#define DEFAULT_LED_ON_TIME_MS            35    ///< LED on duration [ms].
#define DEFAULT_LED_OFF_TIME_MS         3500    ///< Time LED is off between cycles [ms].
#define DEFAULT_LED_FADE_IN_TIME        2000    ///< LED fade in time [ms].
#define DEFAULT_LED_FADE_OUT_TIME        500    ///< LED fade out time [ms].

/** @brief m_ui status return codes.
 */
enum
{
    M_IU_STATUS_CODE_SUCCESS = NRF_SUCCESS,                 ///< Successful.
    M_IU_STATUS_CODE_INVALID_PARAM,                         ///< Invalid parameters supplied.
};

/** @brief Default LED configuration upon BLE connection.
 */
#define UI_CONFIG_DEFAULT_CONNECTED                             \
{                                                               \
    .mode = BLE_UIS_LED_MODE_BREATHE,                           \
    .data =                                                     \
    {                                                           \
        .mode_breathe =                                         \
        {                                                       \
            .color_mix  = (uint8_t)DRV_EXT_LIGHT_COLOR_CYAN,    \
            .intensity  = DEFAULT_LED_INTENSITY_PERCENT,        \
            .delay      = DEFAULT_LED_OFF_TIME_MS               \
        }                                                       \
    }                                                           \
}

/** @brief Default LED configuration upon BLE disconnect.
 */
#define UI_CONFIG_DEFAULT_DISCONNECTED                          \
{                                                               \
    .mode = BLE_UIS_LED_MODE_BREATHE,                           \
    .data =                                                     \
    {                                                           \
        .mode_breathe =                                         \
        {                                                       \
            .color_mix  = (uint8_t)DRV_EXT_LIGHT_COLOR_BLUE,    \
            .intensity  = DEFAULT_LED_INTENSITY_PERCENT,        \
            .delay      = DEFAULT_LED_OFF_TIME_MS               \
        }                                                       \
    }                                                           \
}

/** @brief Default LED configuration when an error occurs.
 */
#define UI_CONFIG_DEFAULT_ERROR                                 \
{                                                               \
    .mode = BLE_UIS_LED_MODE_BREATHE,                           \
    .data =                                                     \
    {                                                           \
        .mode_breathe =                                         \
        {                                                       \
            .color_mix  = (uint8_t)DRV_EXT_LIGHT_COLOR_RED,     \
            .intensity  = DEFAULT_LED_INTENSITY_PERCENT,        \
            .delay      = DEFAULT_LED_OFF_TIME_MS / 4           \
        }                                                       \
    }                                                           \
}

/** @brief Default LED sequence values
 */
#define SEQUENCE_DEFAULT_VALUES                                     \
{                                                                   \
    .sequence_vals.on_time_ms       = DEFAULT_LED_ON_TIME_MS,       \
    .sequence_vals.on_intensity     = 0xFF,                         \
    .sequence_vals.off_intensity    = 0,                            \
    .sequence_vals.fade_in_time_ms  = DEFAULT_LED_FADE_IN_TIME,     \
    .sequence_vals.fade_out_time_ms = DEFAULT_LED_FADE_OUT_TIME,    \
}

/** @brief Predefined events linked to corresponding LED configurations.
 */
typedef enum
{
    M_UI_BLE_CONNECTED,
    M_UI_BLE_DISCONNECTED,
    M_UI_ERROR
}ui_led_events;

/** @brief TWI configuraion.
 */
typedef struct
{
    nrf_drv_twi_t         const * p_twi_instance;
}m_ui_init_t;

/**@brief Function for initializing all UI components (Buttons and LEDs).
 *
 * @retval NRF_SUCCESS      Operation was successful.
 * @retval NRF_ERROR_NULL   NULL pointer supplied.
 * @retval Other codes from the underlying drivers.
 */
ret_code_t m_ui_init(m_ble_service_handle_t * p_handle, m_ui_init_t * p_params);

/**@brief Function for setting the RGB value of a led.
 *
 * @param[in]   r   Red intensity (0 to 255).
 * @param[in]   g   Green intensity (0 to 255).
 * @param[in]   b   Blue intensity (0 to 255).
 *
 * @note in Breathe or One-shot mode the intensity will be set via a separate intensity variable.
 * The values entered in these two modes will be treated as binary (boolean) for each color.
 *
 * @retval NRF_SUCCESS      Operation was successful.
 * @retval Other codes from the underlying drivers.
 */
ret_code_t m_ui_led_set(uint8_t r, uint8_t g, uint8_t b);

/**
 * @brief Function for setting LED color according to predefined events
 *
 * @param[in]   event_code  Predefined event code
 *
 * @retval NRF_SUCCESS      If initialization was successful.
 * @retval Other codes from the underlying drivers.
 */
ret_code_t m_ui_led_set_event(ui_led_events event_code);
#endif /*__THINGY_UI_H__*/

/** @} */
