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

#include "ble_tes.h"
#include "ble_srv_common.h"
#include "sdk_common.h"

#define BLE_UUID_TES_TEMPERATURE_CHAR 0x0201                      /**< The UUID of the temperature Characteristic. */
#define BLE_UUID_TES_PRESSURE_CHAR    0x0202                      /**< The UUID of the pressure Characteristic. */
#define BLE_UUID_TES_HUMIDITY_CHAR    0x0203                      /**< The UUID of the humidity Characteristic. */
#define BLE_UUID_TES_GAS_CHAR         0x0204                      /**< The UUID of the gas Characteristic. */
#define BLE_UUID_TES_COLOR_CHAR       0x0205                      /**< The UUID of the gas Characteristic. */
#define BLE_UUID_TES_CONFIG_CHAR      0x0206                      /**< The UUID of the config Characteristic. */


#define BLE_TES_MAX_RX_CHAR_LEN        BLE_TES_MAX_DATA_LEN        /**< Maximum length of the RX Characteristic (in bytes). */
#define BLE_TES_MAX_TX_CHAR_LEN        BLE_TES_MAX_DATA_LEN        /**< Maximum length of the TX Characteristic (in bytes). */

// EF68xxxx-9B35-4933-9B10-52FFA9740042
#define TES_BASE_UUID                  {{0x42, 0x00, 0x74, 0xA9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49, 0x35, 0x9B, 0x00, 0x00, 0x68, 0xEF}} /**< Used vendor specific UUID. */


/**@brief Function for handling the @ref BLE_GAP_EVT_CONNECTED event from the S132 SoftDevice.
 *
 * @param[in] p_tes     Thingy Environment Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_connect(ble_tes_t * p_tes, ble_evt_t * p_ble_evt)
{
    p_tes->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the @ref BLE_GAP_EVT_DISCONNECTED event from the S132 SoftDevice.
 *
 * @param[in] p_tes     Thingy Environment Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_disconnect(ble_tes_t * p_tes, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_tes->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief Function for handling the @ref BLE_GATTS_EVT_WRITE event from the S132 SoftDevice.
 *
 * @param[in] p_tes     Thingy Environment Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_write(ble_tes_t * p_tes, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if ( (p_evt_write->handle == p_tes->temperature_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tes->is_temperature_notif_enabled != notif_enabled)
        {
            p_tes->is_temperature_notif_enabled = notif_enabled;

            if (p_tes->evt_handler != NULL)
            {
                p_tes->evt_handler(p_tes, BLE_TES_EVT_NOTIF_TEMPERATURE, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tes->pressure_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tes->is_pressure_notif_enabled != notif_enabled)
        {
            p_tes->is_pressure_notif_enabled = notif_enabled;

            if (p_tes->evt_handler != NULL)
            {
                p_tes->evt_handler(p_tes, BLE_TES_EVT_NOTIF_PRESSURE, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tes->humidity_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (notif_enabled != p_tes->is_humidity_notif_enabled)
        {
            p_tes->is_humidity_notif_enabled = notif_enabled;

            if (p_tes->evt_handler != NULL)
            {
                p_tes->evt_handler(p_tes, BLE_TES_EVT_NOTIF_HUMIDITY, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tes->gas_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (notif_enabled != p_tes->is_gas_notif_enabled)
        {
            p_tes->is_gas_notif_enabled = notif_enabled;

            if (p_tes->evt_handler != NULL)
            {
                p_tes->evt_handler(p_tes, BLE_TES_EVT_NOTIF_GAS, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tes->color_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (notif_enabled != p_tes->is_color_notif_enabled)
        {
            p_tes->is_color_notif_enabled = notif_enabled;

            if (p_tes->evt_handler != NULL)
            {
                p_tes->evt_handler(p_tes, BLE_TES_EVT_NOTIF_COLOR, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else
    {
        // Do Nothing. This event is not relevant for this service.
    }
}


static void on_authorize_req(ble_tes_t * p_tes, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_rw_authorize_request_t * p_evt_rw_authorize_request = &p_ble_evt->evt.gatts_evt.params.authorize_request;
    uint32_t err_code;

    if (p_evt_rw_authorize_request->type  == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
    {
        if (p_evt_rw_authorize_request->request.write.handle == p_tes->config_handles.value_handle)
        {
            ble_gatts_rw_authorize_reply_params_t rw_authorize_reply;
            bool                                  valid_data = true;

            // Check for valid data
            if(p_evt_rw_authorize_request->request.write.len != sizeof(ble_tes_config_t))
            {
                valid_data = false;
            }
            else
            {
                ble_tes_config_t * p_config = (ble_tes_config_t *)p_evt_rw_authorize_request->request.write.data;

                if ( (p_config->temperature_interval_ms < BLE_TES_CONFIG_TEMPERATURE_INT_MIN)    ||
                     (p_config->temperature_interval_ms > BLE_TES_CONFIG_TEMPERATURE_INT_MAX)    ||
                     (p_config->pressure_interval_ms < BLE_TES_CONFIG_PRESSURE_INT_MIN)          ||
                     (p_config->pressure_interval_ms > BLE_TES_CONFIG_PRESSURE_INT_MAX)          ||
                     (p_config->humidity_interval_ms < BLE_TES_CONFIG_HUMIDITY_INT_MIN)          ||
                     (p_config->humidity_interval_ms > BLE_TES_CONFIG_HUMIDITY_INT_MAX)          ||
                     (p_config->color_interval_ms < BLE_TES_CONFIG_COLOR_INT_MIN)         ||
                     (p_config->color_interval_ms > BLE_TES_CONFIG_COLOR_INT_MAX)         ||
                     (p_config->gas_interval_mode < BLE_TES_CONFIG_GAS_MODE_MIN)                 ||
                     ((int)p_config->gas_interval_mode > (int)BLE_TES_CONFIG_GAS_MODE_MAX))
                {
                    valid_data = false;
                }
            }

            rw_authorize_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;

            if (valid_data)
            {
                rw_authorize_reply.params.write.update      = 1;
                rw_authorize_reply.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;
                rw_authorize_reply.params.write.p_data      = p_evt_rw_authorize_request->request.write.data;
                rw_authorize_reply.params.write.len         = p_evt_rw_authorize_request->request.write.len;
                rw_authorize_reply.params.write.offset      = p_evt_rw_authorize_request->request.write.offset;
            }
            else
            {
                rw_authorize_reply.params.write.update      = 0;
                rw_authorize_reply.params.write.gatt_status = BLE_GATT_STATUS_ATTERR_WRITE_NOT_PERMITTED;
            }

            err_code = sd_ble_gatts_rw_authorize_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                       &rw_authorize_reply);
            APP_ERROR_CHECK(err_code);

            if ( valid_data && (p_tes->evt_handler != NULL))
            {
                p_tes->evt_handler(p_tes,
                                   BLE_TES_EVT_CONFIG_RECEIVED,
                                   p_evt_rw_authorize_request->request.write.data,
                                   p_evt_rw_authorize_request->request.write.len);
            }
        }
    }
}


/**@brief Function for adding temperature characteristic.
 *
 * @param[in] p_tes       Thingy Environment Service structure.
 * @param[in] p_tes_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t temperature_char_add(ble_tes_t * p_tes, const ble_tes_init_t * p_tes_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_tes->uuid_type;
    ble_uuid.uuid = BLE_UUID_TES_TEMPERATURE_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tes_temperature_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)p_tes_init->p_init_temperature;
    attr_char_value.max_len   = sizeof(ble_tes_temperature_t);

    return sd_ble_gatts_characteristic_add(p_tes->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tes->temperature_handles);
}


/**@brief Function for adding pressure characteristic.
 *
 * @param[in] p_tes       Thingy Environment Service structure.
 * @param[in] p_tes_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t pressure_char_add(ble_tes_t * p_tes, const ble_tes_init_t * p_tes_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_tes->uuid_type;
    ble_uuid.uuid = BLE_UUID_TES_PRESSURE_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tes_pressure_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)p_tes_init->p_init_pressure;
    attr_char_value.max_len   = sizeof(ble_tes_pressure_t);;

    return sd_ble_gatts_characteristic_add(p_tes->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tes->pressure_handles);
}


/**@brief Function for adding humidity characteristic.
 *
 * @param[in] p_tes       Thingy Environment Service structure.
 * @param[in] p_tes_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t humidity_char_add(ble_tes_t * p_tes, const ble_tes_init_t * p_tes_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_tes->uuid_type;
    ble_uuid.uuid = BLE_UUID_TES_HUMIDITY_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tes_humidity_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)p_tes_init->p_init_humidity;
    attr_char_value.max_len   = sizeof(ble_tes_humidity_t);

    return sd_ble_gatts_characteristic_add(p_tes->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tes->humidity_handles);
}


/**@brief Function for adding gas characteristic.
 *
 * @param[in] p_tes       Thingy Environment Service structure.
 * @param[in] p_tes_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t gas_char_add(ble_tes_t * p_tes, const ble_tes_init_t * p_tes_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_tes->uuid_type;
    ble_uuid.uuid = BLE_UUID_TES_GAS_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tes_gas_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)p_tes_init->p_init_gas;
    attr_char_value.max_len   = sizeof(ble_tes_gas_t);

    return sd_ble_gatts_characteristic_add(p_tes->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tes->gas_handles);
}


/**@brief Function for adding color characteristic.
 *
 * @param[in] p_tes       Thingy Environment Service structure.
 * @param[in] p_tes_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t color_char_add(ble_tes_t * p_tes, const ble_tes_init_t * p_tes_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_tes->uuid_type;
    ble_uuid.uuid = BLE_UUID_TES_COLOR_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tes_color_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)p_tes_init->p_init_color;
    attr_char_value.max_len   = sizeof(ble_tes_color_t);

    return sd_ble_gatts_characteristic_add(p_tes->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tes->color_handles);
}


/**@brief Function for adding configuration characteristic.
 *
 * @param[in] p_tes       Thingy Environment Service structure.
 * @param[in] p_tes_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t config_char_add(ble_tes_t * p_tes, const ble_tes_init_t * p_tes_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read          = 1;
    char_md.char_props.write         = 1;
    char_md.char_props.write_wo_resp = 0;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    ble_uuid.type = p_tes->uuid_type;
    ble_uuid.uuid = BLE_UUID_TES_CONFIG_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tes_config_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)p_tes_init->p_init_config;
    attr_char_value.max_len   = sizeof(ble_tes_config_t);

    return sd_ble_gatts_characteristic_add(p_tes->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tes->config_handles);
}


void ble_tes_on_ble_evt(ble_tes_t * p_tes, ble_evt_t * p_ble_evt)
{
    if ((p_tes == NULL) || (p_ble_evt == NULL))
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_tes, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_tes, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_tes, p_ble_evt);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            on_authorize_req(p_tes, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


uint32_t ble_tes_init(ble_tes_t * p_tes, const ble_tes_init_t * p_tes_init)
{
    uint32_t      err_code;
    ble_uuid_t    ble_uuid;
    ble_uuid128_t tes_base_uuid = TES_BASE_UUID;

    VERIFY_PARAM_NOT_NULL(p_tes);
    VERIFY_PARAM_NOT_NULL(p_tes_init);

    // Initialize the service structure.
    p_tes->conn_handle                  = BLE_CONN_HANDLE_INVALID;
    p_tes->evt_handler                  = p_tes_init->evt_handler;
    p_tes->is_temperature_notif_enabled = false;
    p_tes->is_pressure_notif_enabled    = false;
    p_tes->is_humidity_notif_enabled    = false;

    // Add a custom base UUID.
    err_code = sd_ble_uuid_vs_add(&tes_base_uuid, &p_tes->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_tes->uuid_type;
    ble_uuid.uuid = BLE_UUID_TES_SERVICE;

    // Add the service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_tes->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add the temperature Characteristic.
    err_code = temperature_char_add(p_tes, p_tes_init);
    VERIFY_SUCCESS(err_code);

    // Add the pressure Characteristic.
    err_code = pressure_char_add(p_tes, p_tes_init);
    VERIFY_SUCCESS(err_code);

    // Add the humidity Characteristic.
    err_code = humidity_char_add(p_tes, p_tes_init);
    VERIFY_SUCCESS(err_code);

    // Add the gas Characteristic.
    err_code = gas_char_add(p_tes, p_tes_init);
    VERIFY_SUCCESS(err_code);

    // Add the color Characteristic.
    err_code = color_char_add(p_tes, p_tes_init);
    VERIFY_SUCCESS(err_code);

    // Add the config Characteristic.
    err_code = config_char_add(p_tes, p_tes_init);
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}


uint32_t ble_tes_temperature_set(ble_tes_t * p_tes, ble_tes_temperature_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tes_temperature_t);

    VERIFY_PARAM_NOT_NULL(p_tes);

    if ((p_tes->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tes->is_temperature_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TES_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tes->temperature_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tes->conn_handle, &hvx_params);
}


uint32_t ble_tes_pressure_set(ble_tes_t * p_tes, ble_tes_pressure_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tes_pressure_t);

    VERIFY_PARAM_NOT_NULL(p_tes);

    if ((p_tes->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tes->is_pressure_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TES_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tes->pressure_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tes->conn_handle, &hvx_params);
}


uint32_t ble_tes_humidity_set(ble_tes_t * p_tes, ble_tes_humidity_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tes_humidity_t);

    VERIFY_PARAM_NOT_NULL(p_tes);

    if ((p_tes->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tes->is_humidity_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TES_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tes->humidity_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tes->conn_handle, &hvx_params);
}


uint32_t ble_tes_gas_set(ble_tes_t * p_tes, ble_tes_gas_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tes_gas_t);

    VERIFY_PARAM_NOT_NULL(p_tes);

    if ((p_tes->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tes->is_gas_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TES_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tes->gas_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tes->conn_handle, &hvx_params);
}


uint32_t ble_tes_color_set(ble_tes_t * p_tes, ble_tes_color_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tes_color_t);

    VERIFY_PARAM_NOT_NULL(p_tes);

    if ((p_tes->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tes->is_color_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TES_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tes->color_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tes->conn_handle, &hvx_params);
}
