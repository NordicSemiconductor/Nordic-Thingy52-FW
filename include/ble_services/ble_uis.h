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

/** @file
 *
 * @defgroup ble_sdk_srv_uis Thingy User Interface Service
 * @{
 * @ingroup ble_sdk_srv
 *
 * @brief Thingy User Interface Service module.
 *
 * @details This module implements a custom LED Button Service with an LED and Button Characteristics.
 *          During initialization, the module adds the LED Button Service and Characteristics
 *          to the BLE stack database.
 *
 *          The application must supply an event handler for receiving LED Button Service
 *          events. Using this handler, the service notifies the application when the
 *          LED value changes.
 *
 *          The service also provides a function for letting the application notify
 *          the state of the Button Characteristic to connected peers.
 *
 * @note The application must propagate BLE stack events to the LED Button Service
 *       module by calling ble_uis_on_ble_evt() from the @ref softdevice_handler callback.
*/

#ifndef BLE_UIS_H__
#define BLE_UIS_H__

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "app_util_platform.h"

#define BLE_UIS_UUID_BASE        {{0x42, 0x00, 0x74, 0xA9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49, 0x35, 0x9B, 0x00, 0x00, 0x68, 0xEF}}

#define BLE_UIS_UUID_SERVICE     0x0300     /**< UUID for UI service */
#define BLE_UIS_UUID_LED_CHAR    0x0301     /**< UUID for LED characteristic. */
#define BLE_UIS_UUID_BUTTON_CHAR 0x0302     /**< UUID for button characteristic. */
#define BLE_UIS_UUID_PIN_CHAR    0x0303     /**< UUID for pin characteristic. */

#ifdef __GNUC__
    #ifdef PACKED
        #undef PACKED
    #endif

    #define PACKED(TYPE) TYPE __attribute__ ((packed))
#endif

/**@brief The available LED operation modes.
 */
typedef enum
{
    BLE_UIS_LED_MODE_OFF,               /**< LED off. */
    BLE_UIS_LED_MODE_CONST,             /**< LED on with a given R, G and B color mix. */
    BLE_UIS_LED_MODE_BREATHE,           /**< LED breathe cycle (fade in, on, fade out, off). */
    BLE_UIS_LED_MODE_BREATHE_ONE_SHOT   /**< Similar to BREATHE, but only a single cycle is performed. */
}ble_uis_led_mode_t;

#define BLE_UIS_LED_MODE_MIN BLE_UIS_LED_MODE_OFF               /**< Minimum value for mode. */
#define BLE_UIS_LED_MODE_MAX BLE_UIS_LED_MODE_BREATHE_ONE_SHOT  /**< Maximum value for mode. */

#define BLE_UIS_LED_MODE_OFF_LENGTH                     1       /**< Off mode command length in bytes. */
#define BLE_UIS_LED_MODE_CONST_LENGTH                   4       /**< Const mode command length in bytes. */
#define BLE_UIS_LED_MODE_BREATHE_LENGTH                 5       /**< Breahte mode command length in bytes. */
#define BLE_UIS_LED_MODE_BREATHE_ONE_SHOT_LENGTH        3       /**< One shot mode command length in bytes. */

#define BLE_UIS_LED_COLOR_MIX_MIN                       1       /**< Minimum value for color mix. */
#define BLE_UIS_LED_COLOR_MIX_MAX                       7       /**< Maximum value for color mix. */
#define BLE_UIS_LED_INTENSITY_MIN                       1       /**< Minimum LED intensity. */
#define BLE_UIS_LED_INTENSITY_MAX                       100     /**< Maximum LED intensity. */
#define BLE_UIS_LED_DELAY_MIN                           1       /**< Minimum delay for breathe sequence.*/
#define BLE_UIS_LED_DELAY_MAX                           10000   /**< Maximum delay for breathe sequence.*/

/**@brief union representing LED BLE characteristics.
 */
typedef PACKED( union
{
    PACKED( struct
    {
        uint8_t r;          /**< Red intensity .*/
        uint8_t g;          /**< Green intensity .*/
        uint8_t b;          /**< Blue intensity .*/
    })mode_const;
    PACKED( struct
    {
        uint8_t  color_mix; /**< Color mix. (on/off for each of the primary colors, R, G and B). */
        uint8_t  intensity; /**< LED intensity. */
        uint16_t delay;     /**< Delay between breathe sequences. */
    })mode_breathe;
    PACKED( struct
    {
        uint8_t color_mix;  /**< Color mix. (on/off for each of the primary colors, R, G and B). */
        uint8_t intensity;  /**< LED intensity. */
    })mode_breathe_one_shot;
}) ble_uis_led_data_t;

/**@brief The LED mode and data, editable over BLE.
 */
typedef PACKED( struct
{
    ble_uis_led_mode_t mode;
    ble_uis_led_data_t data;
}) ble_uis_led_t;

/**@brief Data structure for the PIN characteristic. Controls the Metal Oxide Semiconductors (transistors).
 */
typedef PACKED( struct
{
    uint8_t mos_1;  /**< Transisior 1 */
    uint8_t mos_2;  /**< Transisior 2 */
    uint8_t mos_3;  /**< Transisior 3 */
    uint8_t mos_4;  /**< Transisior 4 */
}) ble_uis_pin_t;

/**@brief  Forward declaration of the ble_uis_t type.
 */
typedef struct ble_uis_s ble_uis_t;

/**@brief  Declaring the LED write handler.
 */
typedef void (*ble_uis_led_write_handler_t) (ble_uis_t * p_uis, ble_uis_led_t * rgb);

/**@brief  Declaring the pin write handler.
 */
typedef void (*ble_uis_pin_write_handler_t) (ble_uis_t * p_uis, ble_uis_pin_t * pin);

/** @brief LED Button Service init structure. This structure contains all options and data needed for
 *         initialization of the service.
 */
typedef struct
{
    ble_uis_pin_t               init_pin;
    ble_uis_led_t             * p_init_led;
    ble_uis_led_write_handler_t led_write_handler; /**< Event handler to be called when the LED Characteristic is written. */
    ble_uis_pin_write_handler_t pin_write_handler; /**< Event handler to be called when the PIN Characteristic is written. */
} ble_uis_init_t;

/**@brief LED Button Service structure. This structure contains various status information for the service.
 */
struct ble_uis_s
{
    uint16_t                    service_handle;            /**< Handle of LED Button Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    led_char_handles;          /**< Handles related to the LED Characteristic. */
    ble_gatts_char_handles_t    pin_char_handles;          /**< Handles related to the PIN Characteristic. */
    ble_gatts_char_handles_t    button_char_handles;       /**< Handles related to the Button Characteristic. */
    uint8_t                     uuid_type;                 /**< UUID type for the LED Button Service. */
    uint16_t                    conn_handle;               /**< Handle of the current connection (as provided by the BLE stack). BLE_CONN_HANDLE_INVALID if not in a connection. */
    bool                        is_button_notif_enabled;
    ble_uis_led_write_handler_t led_write_handler;         /**< Event handler to be called when the LED Characteristic is written. */
    ble_uis_pin_write_handler_t pin_write_handler;         /**< Event handler to be called when the PIN Characteristic is written. */
};

/**@brief Function for initializing the LED Button Service.
 *
 * @param[out] p_uis      LED Button Service structure. This structure must be supplied by
 *                        the application. It is initialized by this function and will later
 *                        be used to identify this particular service instance.
 * @param[in] p_uis_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was initialized successfully. Otherwise, an error code is returned.
 */
uint32_t ble_uis_init(ble_uis_t * p_uis, const ble_uis_init_t * p_uis_init);

/**@brief Function for handling the application's BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of interest to the LED Button Service.
 *
 * @param[in] p_uis      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
void ble_uis_on_ble_evt(ble_uis_t * p_uis, ble_evt_t * p_ble_evt);

/**@brief Function for sending a button state notification.
 *
 * @param[in] p_uis         LED Button Service structure.
 * @param[in] button_state  New button state.
 *
 * @retval NRF_SUCCESS If the notification was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_uis_on_button_change(ble_uis_t * p_uis, uint8_t buttons_state);

#endif // BLE_UIS_H__

/** @} */
