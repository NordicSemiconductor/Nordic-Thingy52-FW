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
 * @defgroup ble_srv_tss Thingy Sound Service
 * @{
 * @ingroup  ble_sdk_srv
 * @brief    Thingy Sound Service implementation.
 *
 * @details The Thingy Sound Service is a simple GATT-based service with multiple characteristics for reading sensor data and configuring the sensors.
 *
 * @note The application must propagate S110 SoftDevice events to the Thingy Sound Service module
 *       by calling the ble_tss_on_ble_evt() function from the ble_stack_handler callback.
 */

#ifndef BLE_TSS_H__
#define BLE_TSS_H__

#include "ble.h"
#include "ble_srv_common.h"
#include "app_util_platform.h"
#include "drv_audio_config.h"
#include <stdint.h>
#include <stdbool.h>

#define BLE_UUID_TSS_SERVICE 0x0500                      /**< The UUID of the Thingy Sound Service. */
#define BLE_TSS_MAX_DATA_LEN (GATT_MTU_SIZE_DEFAULT - 3) /**< Maximum length of data (in bytss) that can be transmitted to the peer by the Thingy Sound service module. */

#ifdef __GNUC__
    #ifdef PACKED
        #undef PACKED
    #endif

    #define PACKED(TYPE) TYPE __attribute__ ((packed))
#endif

#define BLE_TSS_SPKR_STAT_FINISHED           0x00
#define BLE_TSS_SPKR_STAT_BUFFER_WARNING     0x01
#define BLE_TSS_SPKR_STAT_BUFFER_READY       0x02
#define BLE_TSS_SPKR_STAT_PACKET_DISREGARDED 0x10
#define BLE_TSS_SPKR_STAT_INVALID_COMMAND    0x11

#define BLE_TSS_MIC_FRAME_SIZE               CONFIG_AUDIO_FRAME_SIZE_BYTES

typedef uint8_t ble_tss_spkr_stat_t;

typedef uint8_t ble_tss_spkr_sample_t;

typedef PACKED( struct
{
    uint16_t freq;
    uint16_t duration_ms;
    uint8_t  volume;
}) ble_tss_spkr_freq_t;

typedef PACKED( union
{
    ble_tss_spkr_freq_t   freq;
    uint8_t               pcm[273];
    ble_tss_spkr_sample_t sample_id;
}) ble_tss_spkr_t;

typedef PACKED( union
{
    uint8_t spl;
    uint8_t raw[BLE_TSS_MIC_FRAME_SIZE];
}) ble_tss_mic_t;

typedef enum
{
    BLE_TSS_SPKR_MODE_FIRST,
    BLE_TSS_SPKR_MODE_FREQ,
    BLE_TSS_SPKR_MODE_PCM,
    BLE_TSS_SPKR_MODE_SAMPLE,
    BLE_TSS_SPKR_MODE_LAST
} ble_tss_spkr_mode_t;

typedef enum
{
    BLE_TSS_MIC_MODE_FIRST,
    BLE_TSS_MIC_MODE_ADPCM,
    BLE_TSS_MIC_MODE_SPL,
    BLE_TSS_MIC_MODE_LAST
} ble_tss_mic_mode_t;

typedef PACKED( struct
{
    ble_tss_spkr_mode_t spkr_mode;
    ble_tss_mic_mode_t  mic_mode;
}) ble_tss_config_t;

#define BLE_TSS_CONFIG_SPKR_MODE_MIN      (BLE_TSS_SPKR_MODE_FIRST + 1)
#define BLE_TSS_CONFIG_SPKR_MODE_MAX      (BLE_TSS_SPKR_MODE_LAST - 1)
#define BLE_TSS_CONFIG_MIC_MODE_MIN       (BLE_TSS_MIC_MODE_FIRST + 1)
#define BLE_TSS_CONFIG_MIC_MODE_MAX       (BLE_TSS_MIC_MODE_LAST - 1)

typedef enum
{
    BLE_TSS_EVT_NOTIF_MIC,
    BLE_TSS_EVT_CONFIG_RECEIVED,
    BLE_TSS_EVT_SPKR_RECEIVED,
    BLE_TSS_EVT_NOTIF_SPKR_STAT
}ble_tss_evt_type_t;

/* Forward declaration of the ble_tss_t type. */
typedef struct ble_tss_s ble_tss_t;

/**@brief Thingy Sound Service event handler type. */
typedef void (*ble_tss_evt_handler_t) (ble_tss_t        * p_tss,
                                       ble_tss_evt_type_t evt_type,
                                       uint8_t          * p_data,
                                       uint16_t           length);

/**@brief Thingy Sound Service initialization structure.
 *
 * @details This structure contains the initialization information for the service. The application
 * must fill this structure and pass it to the service using the @ref ble_tss_init function.
 */
typedef struct
{
    ble_tss_config_t      * p_init_config;
    ble_tss_evt_handler_t   evt_handler; /**< Event handler to be called for handling received data. */
} ble_tss_init_t;

/**@brief Thingy Sound Service structure.
 *
 * @details This structure contains status information related to the service.
 */
struct ble_tss_s
{
    uint8_t                  uuid_type;                    /**< UUID type for Thingy Sound Service Base UUID. */
    uint16_t                 service_handle;               /**< Handle of Thingy Sound Service (as provided by the S110 SoftDevice). */
    ble_gatts_char_handles_t spkr_handles;                 /**< Handles related to the speaker characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t mic_handles;                  /**< Handles related to the microphone characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t spkr_stat_handles;            /**< Handles related to the speaker status characteristic (as provided by the S132 SoftDevice). */
    ble_gatts_char_handles_t config_handles;               /**< Handles related to the config characteristic (as provided by the S132 SoftDevice). */
    uint16_t                 conn_handle;                  /**< Handle of the current connection (as provided by the S110 SoftDevice). BLE_CONN_HANDLE_INVALID if not in a connection. */
    bool                     is_mic_notif_enabled;         /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    bool                     is_spkr_stat_notif_enabled;   /**< Variable to indicate if the peer has enabled notification of the characteristic.*/
    ble_tss_evt_handler_t    evt_handler;                  /**< Event handler to be called for handling received data. */
};

/**@brief Function for initializing the Thingy Sound Service.
 *
 * @param[out] p_tss      Thingy Sound Service structure. This structure must be supplied
 *                        by the application. It is initialized by this function and will
 *                        later be used to identify this particular service instance.
 * @param[in] p_tss_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was successfully initialized. Otherwise, an error code is returned.
 * @retval NRF_ERROR_NULL If either of the pointers p_tss or p_tss_init is NULL.
 */
uint32_t ble_tss_init(ble_tss_t * p_tss, const ble_tss_init_t * p_tss_init);

/**@brief Function for handling the Thingy Sound Service's BLE events.
 *
 * @details The Thingy Sound Service expects the application to call this function each time an
 * event is received from the S110 SoftDevice. This function processes the event if it
 * is relevant and calls the Thingy Sound Service event handler of the
 * application if necessary.
 *
 * @param[in] p_tss       Thingy Sound Service structure.
 * @param[in] p_ble_evt   Event received from the S110 SoftDevice.
 */
void ble_tss_on_ble_evt(ble_tss_t * p_tss, ble_evt_t * p_ble_evt);

/**@brief Function for sending microphone data.
 *
 * @details This function sends the microphone input as an microphone characteristic notification to the peer.
 *
 * @param[in] p_tss       Pointer to the Thingy Sound Service structure.
 * @param[in] p_data      Pointer to the mic data.
 * @param[in] size        Mic data size.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tss_mic_set(ble_tss_t * p_tss, uint8_t * p_data, uint16_t size);

/**@brief Function for sending speaker status.
 *
 * @details This function sends the speaker status as an speaker status characteristic notification to the peer.
 *
 * @param[in] p_tss       Pointer to the Thingy Sound Service structure.
 * @param[in] p_data      Pointer to the mic data.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_tss_spkr_stat_set(ble_tss_t * p_tss, ble_tss_spkr_stat_t status);

#endif // BLE_TSS_H__

/** @} */
