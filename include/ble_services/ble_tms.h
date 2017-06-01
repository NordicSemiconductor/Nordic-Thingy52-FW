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
 * @defgroup ble_sdk_srv_wss Thingy Motion Service
 * @{
 * @ingroup  ble_sdk_srv
 * @brief    Thingy Motion Service implementation.
 *
 * @details The Motion Service is a simple GATT-based service with TX and RX characteristics.
 *          Data received from the peer is passed to the application, and the data received
 *          from the application of this service is sent to the peer as Handle Value
 *          Notifications. This module demonstrates how to implement a custom GATT-based
 *          service and characteristics using the S110 SoftDevice. The service
 *          is used by the application to send and receive ASCII text strings to and from the
 *          peer.
 *
 * @note The application must propagate S110 SoftDevice events to the Motion Service module
 *       by calling the ble_tms_on_ble_evt() function from the ble_stack_handler callback.
 */

#ifndef BLE_TMS_H__
#define BLE_TMS_H__

#include "ble.h"
#include "ble_srv_common.h"
#include "app_util_platform.h"
#include <stdint.h>
#include <stdbool.h>

#define BLE_UUID_TMS_SERVICE 0x0400                      /**< The UUID of the Motion Service. */
#define BLE_TMS_MAX_DATA_LEN (GATT_MTU_SIZE_DEFAULT - 3) /**< Maximum length of data (in bytes) that can be transmitted to the peer by the Motion service module. */

#ifdef __GNUC__
    #ifdef PACKED
        #undef PACKED
    #endif

    #define PACKED(TYPE) TYPE __attribute__ ((packed))
#endif

typedef PACKED( struct
{
    int16_t x;
    int16_t y;
    int16_t z;
}) ble_tms_raw_accel_t;

typedef PACKED( struct
{
    int16_t x;
    int16_t y;
    int16_t z;
}) ble_tms_raw_gyro_t;

typedef PACKED( struct
{
    int16_t x;
    int16_t y;
    int16_t z;
}) ble_tms_raw_compass_t;

typedef PACKED( struct
{
    ble_tms_raw_accel_t   accel;
    ble_tms_raw_gyro_t    gyro;
    ble_tms_raw_compass_t compass;
}) ble_tms_raw_t;

typedef PACKED( struct
{
    uint8_t dir;
    uint8_t cnt;
}) ble_tms_tap_t;

typedef uint8_t ble_tms_orientation_t;

typedef PACKED( struct
{
    int32_t w;
    int32_t x;
    int32_t y;
    int32_t z;
}) ble_tms_quat_t;

typedef PACKED( struct
{
    int32_t roll;
    int32_t pitch;
    int32_t yaw;
}) ble_tms_euler_t;

typedef PACKED( struct
{
    int16_t matrix[9];
}) ble_tms_rot_mat_t;

typedef int32_t ble_tms_heading_t;

typedef PACKED( struct
{
    float x;
    float y;
    float z;
}) ble_tms_gravity_t;

typedef PACKED( struct
{
    uint32_t steps;
    uint32_t time_ms;
}) ble_tms_pedo_t;

typedef PACKED( struct
{
    uint16_t                  pedo_interval_ms;
    uint16_t                  temp_interval_ms;
    uint16_t                  compass_interval_ms;
    uint16_t                  motion_freq_hz;
    uint8_t                   wake_on_motion;
}) ble_tms_config_t;

#define BLE_TMS_CONFIG_PEDO_INT_MIN   100
#define BLE_TMS_CONFIG_PEDO_INT_MAX  5000
#define BLE_TMS_CONFIG_TEMP_INT_MIN   100
#define BLE_TMS_CONFIG_TEMP_INT_MAX  5000
#define BLE_TMS_CONFIG_COMP_INT_MIN   100
#define BLE_TMS_CONFIG_COMP_INT_MAX  5000
#define BLE_TMS_CONFIG_MPUF_MIN         1
#define BLE_TMS_CONFIG_MPUF_MAX       200
#define BLE_TMS_CONFIG_WOM_MIN          0
#define BLE_TMS_CONFIG_WOM_MAX          1

typedef enum
{
    BLE_TMS_EVT_CONFIG_RECEIVED,
    BLE_TMS_EVT_NOTIF_TAP,
    BLE_TMS_EVT_NOTIF_ORIENTATION,
    BLE_TMS_EVT_NOTIF_QUAT,
    BLE_TMS_EVT_NOTIF_PEDOMETER,
    BLE_TMS_EVT_NOTIF_RAW,
    BLE_TMS_EVT_NOTIF_EULER,
    BLE_TMS_EVT_NOTIF_ROT_MAT,
    BLE_TMS_EVT_NOTIF_HEADING,
    BLE_TMS_EVT_NOTIF_GRAVITY,
}ble_tms_evt_type_t;

/* Forward declaration of the ble_tms_t type. */
typedef struct ble_tms_s ble_tms_t;

/**@brief Motion Service event handler type. */
typedef void (*ble_tms_evt_handler_t) (ble_tms_t        * p_tms,
                                       ble_tms_evt_type_t evt_type,
                                       uint8_t          * p_data,
                                       uint16_t           length);

/**@brief Motion Service initialization structure.
 *
 * @details This structure contains the initialization information for the service. The application
 * must fill this structure and pass it to the service using the @ref ble_tms_init function.
 */
typedef struct
{
    ble_tms_config_t      * p_init_config;
    ble_tms_evt_handler_t   evt_handler; /**< Event handler to be called for handling received data. */
} ble_tms_init_t;

/**@brief Motion Service structure.
 *
 * @details This structure contains status information related to the service.
 */
struct ble_tms_s
{
    uint8_t                  uuid_type;                    /**< UUID type for Motion Service Base UUID. */
    uint16_t                 service_handle;               /**< Handle of Motion Service (as provided by the S110 SoftDevice). */
    ble_gatts_char_handles_t config_handles;               /**< Handles related to the config characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t tap_handles;                  /**< Handles related to the tap characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t orientation_handles;          /**< Handles related to the orientation characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t quat_handles;                 /**< Handles related to the quaternion characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t pedo_handles;                 /**< Handles related to the pedometer characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t raw_handles;                  /**< Handles related to the raw data characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t euler_handles;                /**< Handles related to the euler angles characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t rot_mat_handles;              /**< Handles related to the rotation matrix characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t heading_handles;              /**< Handles related to the compass heading characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t gravity_handles;              /**< Handles related to the gravity vector characteristic (as provided by the S132 SoftDevice). */
    uint16_t                 conn_handle;                  /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */
    bool                     is_tap_notif_enabled;         /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_orientation_notif_enabled; /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_quat_notif_enabled;        /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_pedo_notif_enabled;        /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_raw_notif_enabled;         /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_euler_notif_enabled;       /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_rot_mat_notif_enabled;     /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_heading_notif_enabled;     /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_gravity_notif_enabled;     /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    ble_tms_evt_handler_t    evt_handler;                  /**< Event handler to be called for handling received data. */
};

/**@brief Function for initializing the Motion Service.
 *
 * @param[out] p_wss      Motion Service structure. This structure must be supplied
 *                        by the application. It is initialized by this function and will
 *                        later be used to identify this particular service instance.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was successfully initialized. Otherwise, an error code is returned.
 * @retval NRF_ERROR_NULL If either of the pointers p_wss or p_tms_init is NULL.
 */
uint32_t ble_tms_init(ble_tms_t * p_wss, const ble_tms_init_t * p_tms_init);

/**@brief Function for handling the Motion Service's BLE events.
 *
 * @details The Motion Service expects the application to call this function each time an
 * event is received from the S110 SoftDevice. This function processes the event if it
 * is relevant and calls the Motion Service event handler of the
 * application if necessary.
 *
 * @param[in] p_wss       Motion Service structure.
 * @param[in] p_ble_evt   Event received from the S110 SoftDevice.
 */
void ble_tms_on_ble_evt(ble_tms_t * p_wss, ble_evt_t * p_ble_evt);

/**@brief Function for sending tap data.
 *
 * @details This function sends the input tap as an tap characteristic notification to the peer.
 *
 * @param[in] p_tms       Pointer to the Motion Service structure.
 * @param[in] p_data      Pointer to the tap data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tms_tap_set(ble_tms_t * p_tms, ble_tms_tap_t * p_data);

/**@brief Function for sending orientation data.
 *
 * @details This function sends the input orientation as an orientation characteristic notification to the peer.
 *
 * @param[in] p_tms       Pointer to the Motion Service structure.
 * @param[in] p_data      Pointer to the orientation data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tms_orientation_set(ble_tms_t * p_tms, ble_tms_orientation_t * p_data);

/**@brief Function for sending quaternion data.
 *
 * @details This function sends the input quaternion as an quaternion characteristic notification to the peer.
 *
 * @param[in] p_tms       Pointer to the Motion Service structure.
 * @param[in] p_data      Pointer to the quaternion data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tms_quat_set(ble_tms_t * p_tms, ble_tms_quat_t * p_data);

/**@brief Function for sending pedometer data.
 *
 * @details This function sends the input steps and time as an pedometer characteristic notification to the peer.
 *
 * @param[in] p_tms       Pointer to the Motion Service structure.
 * @param[in] p_data      Pointer to the data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tms_pedo_set(ble_tms_t * p_tms, ble_tms_pedo_t * p_data);

/**@brief Function for sending raw data.
 *
 * @details This function sends the input as an raw characteristic notification to the peer.
 *
 * @param[in] p_tms       Pointer to the Motion Service structure.
 * @param[in] p_data      Pointer to the data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tms_raw_set(ble_tms_t * p_tms, ble_tms_raw_t * p_data);

/**@brief Function for sending euler angle data.
 *
 * @details This function sends the input pitch, roll and yaw as an euler characteristic notification to the peer.
 *
 * @param[in] p_tms       Pointer to the Motion Service structure.
 * @param[in] p_data      Pointer to the data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tms_euler_set(ble_tms_t * p_tms, ble_tms_euler_t * p_data);

/**@brief Function for sending rotation matrix data.
 *
 * @details This function sends the input as a rotation matrix characteristic notification to the peer.
 *
 * @param[in] p_tms       Pointer to the Motion Service structure.
 * @param[in] p_data      Pointer to the data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tms_rot_mat_set(ble_tms_t * p_tms, ble_tms_rot_mat_t * p_data);

/**@brief Function for sending compass heading data.
 *
 * @details This function sends the input as a compass heading characteristic notification to the peer.
 *
 * @param[in] p_tms       Pointer to the Motion Service structure.
 * @param[in] p_data      Pointer to the data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tms_heading_set(ble_tms_t * p_tms, ble_tms_heading_t * p_data);

/**@brief Function for sending gravity vector data.
 *
 * @details This function sends the input as a gravity vector characteristic notification to the peer.
 *
 * @param[in] p_tms       Pointer to the Motion Service structure.
 * @param[in] p_data      Pointer to the data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tms_gravity_set(ble_tms_t * p_tms, ble_tms_gravity_t * p_data);

#endif // BLE_TMS_H__

/** @} */
