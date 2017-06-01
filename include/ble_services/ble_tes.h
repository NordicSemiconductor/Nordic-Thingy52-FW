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

/**@file
 *
 * @defgroup ble_sdk_srv_tes Thingy Environment Service
 * @{
 * @ingroup  ble_sdk_srv
 * @brief    Thingy Envronment Service implementation.
 *
 * @details The Thingy Envronment Service is a simple GATT-based service with multiple characteristics for reading sensor data and configuring the sensors.
 *
 * @note The application must propagate S110 SoftDevice events to the Thingy Envronment Service module
 *       by calling the ble_tes_on_ble_evt() function from the ble_stack_handler callback.
 */

#ifndef BLE_TES_H__
#define BLE_TES_H__

#include "ble.h"
#include "ble_srv_common.h"
#include "app_util_platform.h"
#include <stdint.h>
#include <stdbool.h>

#define BLE_UUID_TES_SERVICE 0x0200                      /**< The UUID of the Thingy Envronment Service. */
#define BLE_TES_MAX_DATA_LEN (GATT_MTU_SIZE_DEFAULT - 3) /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Thingy Envronment service module. */

#ifdef __GNUC__
    #ifdef PACKED
        #undef PACKED
    #endif

    #define PACKED(TYPE) TYPE __attribute__ ((packed))
#endif

typedef PACKED( struct
{
    int8_t  integer;
    uint8_t decimal;
}) ble_tes_temperature_t;

typedef PACKED( struct
{
    int32_t  integer;
    uint8_t  decimal;
}) ble_tes_pressure_t;

typedef uint8_t ble_tes_humidity_t;

typedef PACKED( struct
{
    uint16_t eco2_ppm; ///< The equivalent CO2 (eCO2) value in parts per million.
    uint16_t tvoc_ppb; ///< The Total Volatile Organic Compound (TVOC) value in parts per billion.
}) ble_tes_gas_t;

typedef PACKED( struct
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t clear;
}) ble_tes_color_t;

typedef enum
{
    GAS_MODE_250MS,
    GAS_MODE_1S,
    GAS_MODE_10S,
    GAS_MODE_60S,
} ble_tes_gas_mode_t;

typedef PACKED( struct
{
    uint8_t  led_red;
    uint8_t  led_green;
    uint8_t  led_blue;
}) ble_tes_color_config_t;

typedef PACKED( struct
{
    uint16_t                temperature_interval_ms;
    uint16_t                pressure_interval_ms;
    uint16_t                humidity_interval_ms;
    uint16_t                color_interval_ms;
    uint8_t                 gas_interval_mode;
    ble_tes_color_config_t  color_config;
}) ble_tes_config_t;

#define BLE_TES_CONFIG_TEMPERATURE_INT_MIN      100
#define BLE_TES_CONFIG_TEMPERATURE_INT_MAX     5000
#define BLE_TES_CONFIG_PRESSURE_INT_MIN          50
#define BLE_TES_CONFIG_PRESSURE_INT_MAX        5000
#define BLE_TES_CONFIG_HUMIDITY_INT_MIN         100
#define BLE_TES_CONFIG_HUMIDITY_INT_MAX        5000
#define BLE_TES_CONFIG_COLOR_INT_MIN            200
#define BLE_TES_CONFIG_COLOR_INT_MAX           5000
#define BLE_TES_CONFIG_GAS_MODE_MIN               1
#define BLE_TES_CONFIG_GAS_MODE_MAX               3

typedef enum
{
    BLE_TES_EVT_NOTIF_TEMPERATURE,
    BLE_TES_EVT_NOTIF_PRESSURE,
    BLE_TES_EVT_NOTIF_HUMIDITY,
    BLE_TES_EVT_NOTIF_GAS,
    BLE_TES_EVT_NOTIF_COLOR,
    BLE_TES_EVT_CONFIG_RECEIVED
}ble_tes_evt_type_t;

/* Forward declaration of the ble_tes_t type. */
typedef struct ble_tes_s ble_tes_t;

/**@brief Thingy Envronment Service event handler type. */
typedef void (*ble_tes_evt_handler_t) (ble_tes_t        * p_tes,
                                       ble_tes_evt_type_t evt_type,
                                       uint8_t          * p_data,
                                       uint16_t           length);

/**@brief Thingy Envronment Service initialization structure.
 *
 * @details This structure contains the initialization information for the service. The application
 * must fill this structure and pass it to the service using the @ref ble_tes_init function.
 */
typedef struct
{
    ble_tes_temperature_t * p_init_temperature;
    ble_tes_pressure_t    * p_init_pressure;
    ble_tes_humidity_t    * p_init_humidity;
    ble_tes_gas_t         * p_init_gas;
    ble_tes_color_t       * p_init_color;
    ble_tes_config_t      * p_init_config;
    ble_tes_evt_handler_t   evt_handler; /**< Event handler to be called for handling received data. */
} ble_tes_init_t;

/**@brief Thingy Envronment Service structure.
 *
 * @details This structure contains status information related to the service.
 */
struct ble_tes_s
{
    uint8_t                  uuid_type;                    /**< UUID type for Thingy Envronment Service Base UUID. */
    uint16_t                 service_handle;               /**< Handle of Thingy Envronment Service (as provided by the S110 SoftDevice). */
    ble_gatts_char_handles_t temperature_handles;          /**< Handles related to the temperature characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t pressure_handles;             /**< Handles related to the pressure characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t humidity_handles;             /**< Handles related to the humidity characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t gas_handles;                  /**< Handles related to the gas characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t color_handles;                /**< Handles related to the gas characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t config_handles;               /**< Handles related to the config characteristic (as provided by the S132 SoftDevice). */
    uint16_t                 conn_handle;                  /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */
    bool                     is_temperature_notif_enabled; /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_pressure_notif_enabled;    /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_humidity_notif_enabled;    /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_gas_notif_enabled;         /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_color_notif_enabled;       /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    ble_tes_evt_handler_t    evt_handler;                  /**< Event handler to be called for handling received data. */
};

/**@brief Function for initializing the Thingy Envronment Service.
 *
 * @param[out] p_tes      Thingy Envronment Service structure. This structure must be supplied
 *                        by the application. It is initialized by this function and will
 *                        later be used to identify this particular service instance.
 * @param[in] p_tes_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was successfully initialized. Otherwise, an error code is returned.
 * @retval NRF_ERROR_NULL If either of the pointers p_tes or p_tes_init is NULL.
 */
uint32_t ble_tes_init(ble_tes_t * p_tes, const ble_tes_init_t * p_tes_init);

/**@brief Function for handling the Thingy Envronment Service's BLE events.
 *
 * @details The Thingy Envronment Service expects the application to call this function each time an
 * event is received from the S110 SoftDevice. This function processes the event if it
 * is relevant and calls the Thingy Envronment Service event handler of the
 * application if necessary.
 *
 * @param[in] p_tes       Thingy Envronment Service structure.
 * @param[in] p_ble_evt   Event received from the S110 SoftDevice.
 */
void ble_tes_on_ble_evt(ble_tes_t * p_tes, ble_evt_t * p_ble_evt);

/**@brief Function for setting the temperature.
 *
 * @details This function sends the input temperature as an temperature characteristic notification to the
 *          peer.
 *
 * @param[in] p_tes       Pointer to the Thingy Envronment Service structure.
 * @param[in] p_data      Pointer to the temperature data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tes_temperature_set(ble_tes_t * p_tes, ble_tes_temperature_t * p_data);

/**@brief Function for setting the pressure.
 *
 * @details This function sends the input pressure as an pressure characteristic notification to the peer.
 *
 * @param[in] p_tes       Pointer to the Thingy Envronment Service structure.
 * @param[in] p_data      Pointer to the pressure data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tes_pressure_set(ble_tes_t * p_tes, ble_tes_pressure_t * p_data);

/**@brief Function for setting the humidity.
 *
 * @details This function sends the input humidity as an humidity characteristic notification to the peer.
 *
 * @param[in] p_tes       Pointer to the Thingy Envronment Service structure.
 * @param[in] p_data      Pointer to the humidity data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tes_humidity_set(ble_tes_t * p_tes, ble_tes_humidity_t * p_data);

/**@brief Function for setting the gas data.
 *
 * @details This function sends the input humidity as an humidity characteristic notification to the peer.
 *
 * @param[in] p_tes       Pointer to the Thingy Envronment Service structure.
 * @param[in] p_data      Pointer to the humidity data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tes_gas_set(ble_tes_t * p_tes, ble_tes_gas_t * p_data);

/**@brief Function for setting the color data.
 *
 * @details This function sends the input humidity as an humidity characteristic notification to the peer.
 *
 * @param[in] p_tes       Pointer to the Thingy Envronment Service structure.
 * @param[in] p_data      Pointer to the humidity data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tes_color_set(ble_tes_t * p_tes, ble_tes_color_t * p_data);

#endif // BLE_TES_H__

/** @} */
