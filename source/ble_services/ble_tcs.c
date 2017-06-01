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

#include "ble_tcs.h"
#include "ble_srv_common.h"
#include "sdk_common.h"

#define BLE_UUID_TCS_DEVICE_NAME_CHAR   0x0101                      /**< The UUID of the device name Characteristic. */
#define BLE_UUID_TCS_ADV_PARAMS_CHAR    0x0102                      /**< The UUID of the advertising parameters Characteristic. */
#define BLE_UUID_TCS_APPEARANCE_CHAR    0x0103                      /**< The UUID of the appearance Characteristic. */
#define BLE_UUID_TCS_CONN_PARAM_CHAR    0x0104                      /**< The UUID of the connection parameters Characteristic. */
#define BLE_UUID_TCS_BEACON_PARAM_CHAR  0x0105                      /**< The UUID of the beacon Characteristic. */
#define BLE_UUID_TCS_CLOUD_PARAM_CHAR   0x0106                      /**< The UUID of the cloud token Characteristic. */
#define BLE_UUID_TCS_FW_VERSION_CHAR    0x0107                      /**< The UUID of the FW version Characteristic. */
#define BLE_UUID_TCS_MTU_CHAR           0x0108                      /**< The UUID of the MTU Characteristic. */

// EF68xxxx-9B35-4933-9B10-52FFA9740042
#define TCS_BASE_UUID                  {{0x42, 0x00, 0x74, 0xA9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49, 0x35, 0x9B, 0x00, 0x00, 0x68, 0xEF}} /**< Used vendor specific UUID. */

/**@brief Function for handling the @ref BLE_GAP_EVT_CONNECTED event from the S132 SoftDevice.
 *
 * @param[in] p_tcs     Thingy Configuration Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_connect(ble_tcs_t * p_tcs, ble_evt_t * p_ble_evt)
{
    p_tcs->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the @ref BLE_GAP_EVT_DISCONNECTED event from the S132 SoftDevice.
 *
 * @param[in] p_tcs     Thingy Configuration Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_disconnect(ble_tcs_t * p_tcs, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_tcs->conn_handle = BLE_CONN_HANDLE_INVALID;
}


static void on_authorize_req(ble_tcs_t * p_tcs, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_rw_authorize_request_t * p_evt_rw_authorize_request = &p_ble_evt->evt.gatts_evt.params.authorize_request;
    uint32_t err_code;

    if (p_evt_rw_authorize_request->type  == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
    {
        ble_gatts_rw_authorize_reply_params_t rw_authorize_reply;
        bool                                  valid_data = true;
        bool                                  reply = true;
        ble_tcs_evt_type_t                    evt_type = BLE_TCS_EVT_DEV_NAME;


        if (p_evt_rw_authorize_request->request.write.handle == p_tcs->dev_name_handles.value_handle)
        {
            evt_type = BLE_TCS_EVT_DEV_NAME;

            // Check for valid data
            if(p_evt_rw_authorize_request->request.write.len > BLE_TCS_DEVICE_NAME_LEN_MAX)
            {
                valid_data = false;
            }
        }
        else if (p_evt_rw_authorize_request->request.write.handle == p_tcs->adv_param_handles.value_handle)
        {
            // Check for valid data
            if(p_evt_rw_authorize_request->request.write.len != sizeof(ble_tcs_adv_params_t))
            {
                valid_data = false;
            }
            else
            {
                ble_tcs_adv_params_t * p_data = (ble_tcs_adv_params_t *)p_evt_rw_authorize_request->request.write.data;

                evt_type = BLE_TCS_EVT_ADV_PARAM;

                if ( (p_data->interval  < TCS_ADV_PARAMS_INTERVAL_MIN)    ||
                     (p_data->interval  > TCS_ADV_PARAMS_INTERVAL_MAX)    ||
                  /* (p_data->timeout  < TCS_ADV_PARAMS_TIMEOUT_MIN)      || */
                     (p_data->timeout  > TCS_ADV_PARAMS_TIMEOUT_MAX))
                {
                    valid_data = false;
                }
            }
        }
        else if (p_evt_rw_authorize_request->request.write.handle == p_tcs->conn_param_handles.value_handle)
        {
            // Check for valid data
            if(p_evt_rw_authorize_request->request.write.len != sizeof(ble_tcs_conn_params_t))
            {
                valid_data = false;
            }
            else
            {
                ble_tcs_conn_params_t * p_data = (ble_tcs_conn_params_t *)p_evt_rw_authorize_request->request.write.data;

                evt_type = BLE_TCS_EVT_CONN_PARAM;

                if ( (p_data->min_conn_int  < BLE_GAP_CP_MIN_CONN_INTVL_MIN)    ||
                     (p_data->min_conn_int  > BLE_GAP_CP_MIN_CONN_INTVL_MAX)    ||
                     (p_data->max_conn_int  < BLE_GAP_CP_MAX_CONN_INTVL_MIN)    ||
                     (p_data->max_conn_int  > BLE_GAP_CP_MAX_CONN_INTVL_MAX)    ||
                     (p_data->slave_latency > BLE_GAP_CP_SLAVE_LATENCY_MAX)     ||
                     (p_data->sup_timeout   < BLE_GAP_CP_CONN_SUP_TIMEOUT_MIN)  ||
                     (p_data->sup_timeout   > BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX))
                {
                    valid_data = false;
                }

              /* If both conn_sup_timeout and max_conn_interval are specified, then the following constraint applies:
                 conn_sup_timeout * 4 > (1 + slave_latency) * max_conn_interval
                 that corresponds to the following Bluetooth Spec requirement:
                 The Supervision_Timeout in milliseconds shall be larger than
                 (1 + Conn_Latency) * Conn_Interval_Max * 2, where Conn_Interval_Max is given in milliseconds.   */

                if ( (p_data->sup_timeout * 4) <= ((1 + p_data->slave_latency) * p_data->max_conn_int) )
                {
                    valid_data = false;
                }
            }
        }
        else if (p_evt_rw_authorize_request->request.write.handle == p_tcs->beacon_handles.value_handle)
        {
            evt_type = BLE_TCS_EVT_BEACON;

            if (p_evt_rw_authorize_request->request.write.len > 0)
            {
            // Check for valid data
                if( (p_evt_rw_authorize_request->request.write.len > BLE_TCS_BEACON_LEN_MAX) ||
                    (p_evt_rw_authorize_request->request.write.len < BLE_TCS_BEACON_LEN_MIN))
                {
                    valid_data = false;
                }
            }
        }
        else if (p_evt_rw_authorize_request->request.write.handle == p_tcs->cloud_handles.value_handle)
        {
            evt_type = BLE_TCS_EVT_CLOUD_TOKEN;

            // Check for valid data
            if(p_evt_rw_authorize_request->request.write.len > BLE_TCS_CLOUD_LEN_MAX)
            {
                valid_data = false;
            }
        }
        else if (p_evt_rw_authorize_request->request.write.handle == p_tcs->mtu_handles.value_handle)
        {
            // Check for valid data
            if(p_evt_rw_authorize_request->request.write.len != sizeof(ble_tcs_mtu_t))
            {
                valid_data = false;
            }
            else
            {
                ble_tcs_mtu_t * p_data = (ble_tcs_mtu_t *)p_evt_rw_authorize_request->request.write.data;

                evt_type = BLE_TCS_EVT_MTU;

                if ( /*(p_data->req   < TCS_MTU_REQ_MIN)      || */
                     (p_data->req   > TCS_MTU_REQ_MAX)      ||
                     (p_data->size  < TCS_MTU_SIZE_MIN)     ||
                     (p_data->size  > TCS_MTU_SIZE_MAX))
                {
                    valid_data = false;
                }
            }
        }

        else
        {
            valid_data = false;
            reply = false;
        }

        if (reply)
        {
            // Reply depending on valid data or not
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
        }

        // Call event handler
        if ( valid_data && (p_tcs->evt_handler != NULL))
        {
            p_tcs->evt_handler(p_tcs,
                               evt_type,
                               p_evt_rw_authorize_request->request.write.data,
                               p_evt_rw_authorize_request->request.write.len);
        }
    }
}


/**@brief Function for adding device name characteristic.
 *
 * @param[in] p_tcs       Thingy Configuration Service structure.
 * @param[in] p_tcs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t device_name_char_add(ble_tcs_t * p_tcs, const ble_tcs_init_t * p_tcs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write         = 1;
    char_md.char_props.write_wo_resp = 0;
    char_md.char_props.read          = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    ble_uuid.type = p_tcs->uuid_type;
    ble_uuid.uuid = BLE_UUID_TCS_DEVICE_NAME_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = p_tcs_init->p_init_vals->dev_name.len;
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = p_tcs_init->p_init_vals->dev_name.name;
    attr_char_value.max_len   = BLE_TCS_DEVICE_NAME_LEN_MAX;

    return sd_ble_gatts_characteristic_add(p_tcs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tcs->dev_name_handles);
}


/**@brief Function for adding advertising parameters characteristic.
 *
 * @param[in] p_tcs       Thingy Configuration Service structure.
 * @param[in] p_tcs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t adv_params_char_add(ble_tcs_t * p_tcs, const ble_tcs_init_t * p_tcs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write         = 1;
    char_md.char_props.write_wo_resp = 0;
    char_md.char_props.read          = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    ble_uuid.type = p_tcs->uuid_type;
    ble_uuid.uuid = BLE_UUID_TCS_ADV_PARAMS_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tcs_adv_params_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&p_tcs_init->p_init_vals->adv_params;
    attr_char_value.max_len   = sizeof(ble_tcs_adv_params_t);

    return sd_ble_gatts_characteristic_add(p_tcs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tcs->adv_param_handles);
}


/**@brief Function for adding connection parameters characteristic.
 *
 * @param[in] p_tcs       Thingy Configuration Service structure.
 * @param[in] p_tcs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t conn_params_char_add(ble_tcs_t * p_tcs, const ble_tcs_init_t * p_tcs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write         = 1;
    char_md.char_props.write_wo_resp = 0;
    char_md.char_props.read          = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    ble_uuid.type = p_tcs->uuid_type;
    ble_uuid.uuid = BLE_UUID_TCS_CONN_PARAM_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tcs_conn_params_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&p_tcs_init->p_init_vals->conn_params;
    attr_char_value.max_len   = sizeof(ble_tcs_conn_params_t);

    return sd_ble_gatts_characteristic_add(p_tcs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tcs->conn_param_handles);
}


/**@brief Function for adding beacon characteristic.
 *
 * @param[in] p_tcs       Thingy Configuration Service structure.
 * @param[in] p_tcs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t beacon_data_char_add(ble_tcs_t * p_tcs, const ble_tcs_init_t * p_tcs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write         = 1;
    char_md.char_props.write_wo_resp = 0;
    char_md.char_props.read          = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    ble_uuid.type = p_tcs->uuid_type;
    ble_uuid.uuid = BLE_UUID_TCS_BEACON_PARAM_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = p_tcs_init->p_init_vals->eddystone_url.len;
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = p_tcs_init->p_init_vals->eddystone_url.data;
    attr_char_value.max_len   = BLE_TCS_MAX_DATA_LEN;

    return sd_ble_gatts_characteristic_add(p_tcs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tcs->beacon_handles);
}


/**@brief Function for adding cloud token characteristic.
 *
 * @param[in] p_tcs       Thingy Configuration Service structure.
 * @param[in] p_tcs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t token_data_char_add(ble_tcs_t * p_tcs, const ble_tcs_init_t * p_tcs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.write         = 1;
    char_md.char_props.write_wo_resp = 0;
    char_md.char_props.read          = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    ble_uuid.type = p_tcs->uuid_type;
    ble_uuid.uuid = BLE_UUID_TCS_CLOUD_PARAM_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = p_tcs_init->p_init_vals->cloud_token.len;
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = p_tcs_init->p_init_vals->cloud_token.token;
    attr_char_value.max_len   = BLE_TCS_CLOUD_LEN_MAX;

    return sd_ble_gatts_characteristic_add(p_tcs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tcs->cloud_handles);
}


/**@brief Function for adding FW version characteristic.
 *
 * @param[in] p_tcs       Thingy Configuration Service structure.
 * @param[in] p_tcs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t fw_version_char_add(ble_tcs_t * p_tcs, const ble_tcs_init_t * p_tcs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read          = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    ble_uuid.type = p_tcs->uuid_type;
    ble_uuid.uuid = BLE_UUID_TCS_FW_VERSION_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tcs_fw_version_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&p_tcs_init->p_init_vals->fw_version;
    attr_char_value.max_len   = sizeof(ble_tcs_fw_version_t);

    return sd_ble_gatts_characteristic_add(p_tcs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tcs->fwv_handles);
}


/**@brief Function for adding MTU characteristic.
 *
 * @param[in] p_tcs       Thingy Configuration Service structure.
 * @param[in] p_tcs_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t mtu_char_add(ble_tcs_t * p_tcs, const ble_tcs_init_t * p_tcs_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read          = 1;
    char_md.char_props.write_wo_resp = 0;
    char_md.char_props.write         = 1;
    char_md.p_char_user_desc         = NULL;
    char_md.p_char_pf                = NULL;
    char_md.p_user_desc_md           = NULL;
    char_md.p_cccd_md                = NULL;
    char_md.p_sccd_md                = NULL;

    ble_uuid.type = p_tcs->uuid_type;
    ble_uuid.uuid = BLE_UUID_TCS_MTU_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tcs_mtu_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&p_tcs_init->p_init_vals->mtu;
    attr_char_value.max_len   = sizeof(ble_tcs_mtu_t);

    return sd_ble_gatts_characteristic_add(p_tcs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tcs->mtu_handles);
}


void ble_tcs_on_ble_evt(ble_tcs_t * p_tcs, ble_evt_t * p_ble_evt)
{
    if ((p_tcs == NULL) || (p_ble_evt == NULL))
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_tcs, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_tcs, p_ble_evt);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            on_authorize_req(p_tcs, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


uint32_t ble_tcs_init(ble_tcs_t * p_tcs, const ble_tcs_init_t * p_tcs_init)
{
    uint32_t      err_code;
    ble_uuid_t    ble_uuid;
    ble_uuid128_t tcs_base_uuid = TCS_BASE_UUID;

    VERIFY_PARAM_NOT_NULL(p_tcs);
    VERIFY_PARAM_NOT_NULL(p_tcs_init);

    // Initialize the service structure.
    p_tcs->conn_handle                  = BLE_CONN_HANDLE_INVALID;
    p_tcs->evt_handler                  = p_tcs_init->evt_handler;

    // Add a custom base UUID.
    err_code = sd_ble_uuid_vs_add(&tcs_base_uuid, &p_tcs->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_tcs->uuid_type;
    ble_uuid.uuid = BLE_UUID_TCS_SERVICE;

    // Add the service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_tcs->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add the device name Characteristic.
    err_code = device_name_char_add(p_tcs, p_tcs_init);
    VERIFY_SUCCESS(err_code);

    // Add the advertising parameters Characteristic.
    err_code = adv_params_char_add(p_tcs, p_tcs_init);
    VERIFY_SUCCESS(err_code);

    // Add the connection parameters Characteristic.
    err_code = conn_params_char_add(p_tcs, p_tcs_init);
    VERIFY_SUCCESS(err_code);

    // Add the beacon Characteristic.
    err_code = beacon_data_char_add(p_tcs, p_tcs_init);
    VERIFY_SUCCESS(err_code);

    // Add the cloud token Characteristic.
    err_code = token_data_char_add(p_tcs, p_tcs_init);
    VERIFY_SUCCESS(err_code);

    // Add the FW version Characteristic.
    err_code = fw_version_char_add(p_tcs, p_tcs_init);
    VERIFY_SUCCESS(err_code);

    // Add the MTU Characteristic.
    err_code = mtu_char_add(p_tcs, p_tcs_init);
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}


uint32_t ble_tcs_mtu_set(ble_tcs_t * p_tcs, ble_tcs_mtu_t * p_data)
{
    ble_gatts_value_t      gatts_value;
    uint16_t               length = sizeof(ble_tcs_mtu_t);

    VERIFY_PARAM_NOT_NULL(p_tcs);

    if (p_tcs->conn_handle == BLE_CONN_HANDLE_INVALID)
    {
        return NRF_ERROR_INVALID_STATE;
    }

    gatts_value.len     = length;
    gatts_value.offset  = 0;
    gatts_value.p_value = (uint8_t *)p_data;

    return sd_ble_gatts_value_set(p_tcs->conn_handle, p_tcs->mtu_handles.value_handle, &gatts_value);
}
