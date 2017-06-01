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
 * @defgroup ble_sdk_srv_tcs Thingy Configuration Service
 * @{
 * @ingroup  ble_sdk_srv
 * @brief    Thingy Configuration Service implementation.
 *
 * @details The Thingy Configuration Service is a simple GATT-based service with TX and RX characteristics.
 *          Data received from the peer is passed to the application, and the data received
 *          from the application of this service is sent to the peer as Handle Value
 *          Notifications. This module demonstrates how to implement a custom GATT-based
 *          service and characteristics using the S110 SoftDevice. The service
 *          is used by the application to send and receive ASCII text strings to and from the
 *          peer.
 *
 * @note The application must propagate S110 SoftDevice events to the Thingy Configuration Service module
 *       by calling the ble_tcs_on_ble_evt() function from the ble_stack_handler callback.
 */

#ifndef BLE_TCS_H__
#define BLE_TCS_H__

#include "ble.h"
#include "ble_srv_common.h"
#include "app_util_platform.h"
#include <stdint.h>
#include <stdbool.h>

#define BLE_UUID_TCS_SERVICE 0x0100                      /**< The UUID of the Thingy Configuration Service. */
#define BLE_TCS_MAX_DATA_LEN (GATT_MTU_SIZE_DEFAULT - 3) /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Thingy Configuration service module. */

#define BLE_TCS_DEVICE_NAME_LEN_MAX 10
#define BLE_TCS_BEACON_LEN_MAX      17
#define BLE_TCS_BEACON_LEN_MIN       3
#define BLE_TCS_CLOUD_LEN_MAX      250
#define BLE_TCS_CLOUD_LEN_MIN        0

#ifdef __GNUC__
    #ifdef PACKED
        #undef PACKED
    #endif

    #define PACKED(TYPE) TYPE __attribute__ ((packed))
#endif

typedef PACKED( struct
{
    uint8_t name[BLE_TCS_DEVICE_NAME_LEN_MAX+1];
    uint8_t len;
}) ble_tcs_dev_name_t;

typedef PACKED( struct
{
    uint16_t interval;
    uint8_t  timeout;
}) ble_tcs_adv_params_t;

#define TCS_ADV_PARAMS_INTERVAL_MIN 32UL
#define TCS_ADV_PARAMS_INTERVAL_MAX 8000UL
#define TCS_ADV_PARAMS_TIMEOUT_MIN  0UL
#define TCS_ADV_PARAMS_TIMEOUT_MAX  180UL

typedef uint16_t ble_tcs_appear_t;

typedef PACKED( struct
{
    uint16_t min_conn_int;
    uint16_t max_conn_int;
    uint16_t slave_latency;
    uint16_t sup_timeout;
}) ble_tcs_conn_params_t;

typedef PACKED( struct
{
    uint8_t data[BLE_TCS_BEACON_LEN_MAX];
    uint8_t len;
}) ble_tcs_eddystone_url_t;

typedef PACKED( struct
{
    uint8_t token[BLE_TCS_CLOUD_LEN_MAX];
    uint8_t len;
}) ble_tcs_cloud_token_t;

typedef PACKED( struct
{
    uint8_t major;
    uint8_t minor;
    uint8_t patch;
}) ble_tcs_fw_version_t;

typedef PACKED( struct
{
    uint8_t  req;
    uint16_t size;
}) ble_tcs_mtu_t;

#define TCS_MTU_REQ_EXCHANGE    0x01
#define TCS_MTU_REQ_MIN         0UL
#define TCS_MTU_REQ_MAX         1UL
#define TCS_MTU_SIZE_MIN        23UL
#define TCS_MTU_SIZE_MAX        276UL

typedef struct
{
    ble_tcs_dev_name_t      dev_name;
    ble_tcs_adv_params_t    adv_params;
    ble_tcs_conn_params_t   conn_params;
    ble_tcs_eddystone_url_t eddystone_url;
    ble_tcs_cloud_token_t   cloud_token;
    ble_tcs_fw_version_t    fw_version;
    ble_tcs_mtu_t           mtu;
}ble_tcs_params_t;

typedef enum
{
    BLE_TCS_EVT_DEV_NAME,
    BLE_TCS_EVT_ADV_PARAM,
    BLE_TCS_EVT_CONN_PARAM,
    BLE_TCS_EVT_BEACON,
    BLE_TCS_EVT_CLOUD_TOKEN,
    BLE_TCS_EVT_MTU
}ble_tcs_evt_type_t;

/* Forward declaration of the ble_tcs_t type. */
typedef struct ble_tcs_s ble_tcs_t;

/**@brief Thingy Configuration Service event handler type. */
typedef void (*ble_tcs_evt_handler_t) (ble_tcs_t        * p_tcs,
                                       ble_tcs_evt_type_t evt_type,
                                       uint8_t          * p_data,
                                       uint16_t           length);

/**@brief Thingy Configuration Service initialization structure.
 *
 * @details This structure contains the initialization information for the service. The application
 * must fill this structure and pass it to the service using the @ref ble_tcs_init function.
 */
typedef struct
{
    ble_tcs_params_t      * p_init_vals;
    ble_tcs_evt_handler_t   evt_handler; /**< Event handler to be called for handling received data. */
} ble_tcs_init_t;

/**@brief Thingy Configuration Service structure.
 *
 * @details This structure contains status information related to the service.
 */
struct ble_tcs_s
{
    uint8_t                  uuid_type;                    /**< UUID type for Thingy Configuration Service Base UUID. */
    uint16_t                 service_handle;               /**< Handle of Thingy Configuration Service (as provided by the S110 SoftDevice). */
    ble_gatts_char_handles_t dev_name_handles;             /**< Handles related to the temperature characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t adv_param_handles;            /**< Handles related to the pressure characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t conn_param_handles;           /**< Handles related to the config characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t beacon_handles;
    ble_gatts_char_handles_t cloud_handles;
    ble_gatts_char_handles_t fwv_handles;
    ble_gatts_char_handles_t mtu_handles;
    uint16_t                 conn_handle;                  /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */
    ble_tcs_evt_handler_t    evt_handler;                  /**< Event handler to be called for handling received data. */
};

/**@brief Function for initializing the Thingy Configuration Service.
 *
 * @param[out] p_tcs      Thingy Configuration Service structure. This structure must be supplied
 *                        by the application. It is initialized by this function and will
 *                        later be used to identify this particular service instance.
 * @param[in] p_tcs_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was successfully initialized. Otherwise, an error code is returned.
 * @retval NRF_ERROR_NULL If either of the pointers p_tcs or p_tcs_init is NULL.
 */
uint32_t ble_tcs_init(ble_tcs_t * p_tcs, const ble_tcs_init_t * p_tcs_init);

/**@brief Function for handling the Thingy Configuration Service's BLE events.
 *
 * @details The Thingy Configuration Service expects the application to call this function each time an
 * event is received from the S110 SoftDevice. This function processes the event if it
 * is relevant and calls the Thingy Configuration Service event handler of the
 * application if necessary.
 *
 * @param[in] p_tcs       Thingy Configuration Service structure.
 * @param[in] p_ble_evt   Event received from the S110 SoftDevice.
 */
void ble_tcs_on_ble_evt(ble_tcs_t * p_tcs, ble_evt_t * p_ble_evt);

/**@brief Function for setting the MTU char data.
 *
 * @details This function updates the MTU characteristic.
 *
 * @param[in] p_tcs       Pointer to the Thingy Configuration Service structure.
 * @param[in] p_data      Pointer to the MTU data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tcs_mtu_set(ble_tcs_t * p_tcs, ble_tcs_mtu_t * p_data);

#endif // BLE_TCS_H__

/** @} */
