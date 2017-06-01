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

#include "ble_tms.h"
#include "ble_srv_common.h"
#include "sdk_common.h"

#define BLE_UUID_TMS_CONFIG_CHAR      0x0401                      /**< The UUID of the config Characteristic. */
#define BLE_UUID_TMS_TAP_CHAR         0x0402                      /**< The UUID of the tap Characteristic. */
#define BLE_UUID_TMS_ORIENTATION_CHAR 0x0403                      /**< The UUID of the orientation Characteristic. */
#define BLE_UUID_TMS_QUATERNION_CHAR  0x0404                      /**< The UUID of the quaternion Characteristic. */
#define BLE_UUID_TMS_PEDOMETER_CHAR   0x0405                      /**< The UUID of the pedometer Characteristic. */
#define BLE_UUID_TMS_RAW_CHAR         0x0406                      /**< The UUID of the raw data Characteristic. */
#define BLE_UUID_TMS_EULER_CHAR       0x0407                      /**< The UUID of the euler Characteristic. */
#define BLE_UUID_TMS_ROT_MAT_CHAR     0x0408                      /**< The UUID of the rotation matrix Characteristic. */
#define BLE_UUID_TMS_HEADING_CHAR     0x0409                      /**< The UUID of the compass heading Characteristic. */
#define BLE_UUID_TMS_GRAVITY_CHAR     0x040A                      /**< The UUID of the gravity vector Characteristic. */


// EF68xxxx-9B35-4933-9B10-52FFA9740042
#define TMS_BASE_UUID                  {{0x42, 0x00, 0x74, 0xA9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49, 0x35, 0x9B, 0x00, 0x00, 0x68, 0xEF}} /**< Used vendor specific UUID. */

/**@brief Function for handling the @ref BLE_GAP_EVT_CONNECTED event from the S132 SoftDevice.
 *
 * @param[in] p_tms     Motion Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_connect(ble_tms_t * p_tms, ble_evt_t * p_ble_evt)
{
    p_tms->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the @ref BLE_GAP_EVT_DISCONNECTED event from the S132 SoftDevice.
 *
 * @param[in] p_tms     Motion Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_disconnect(ble_tms_t * p_tms, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_tms->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief Function for handling the @ref BLE_GATTS_EVT_WRITE event from the S132 SoftDevice.
 *
 * @param[in] p_tms     Motion Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_write(ble_tms_t * p_tms, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;


    if ( (p_evt_write->handle == p_tms->tap_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tms->is_tap_notif_enabled != notif_enabled)
        {
            p_tms->is_tap_notif_enabled = notif_enabled;

            if (p_tms->evt_handler != NULL)
            {
                p_tms->evt_handler(p_tms, BLE_TMS_EVT_NOTIF_TAP, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tms->orientation_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tms->is_orientation_notif_enabled != notif_enabled)
        {
            p_tms->is_orientation_notif_enabled = notif_enabled;

            if (p_tms->evt_handler != NULL)
            {
                p_tms->evt_handler(p_tms, BLE_TMS_EVT_NOTIF_ORIENTATION, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tms->quat_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tms->is_quat_notif_enabled != notif_enabled)
        {
            p_tms->is_quat_notif_enabled = notif_enabled;

            if (p_tms->evt_handler != NULL)
            {
                p_tms->evt_handler(p_tms, BLE_TMS_EVT_NOTIF_QUAT, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tms->pedo_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tms->is_pedo_notif_enabled != notif_enabled)
        {
            p_tms->is_pedo_notif_enabled = notif_enabled;

            if (p_tms->evt_handler != NULL)
            {
                p_tms->evt_handler(p_tms, BLE_TMS_EVT_NOTIF_PEDOMETER, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tms->raw_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tms->is_raw_notif_enabled != notif_enabled)
        {
            p_tms->is_raw_notif_enabled = notif_enabled;

            if (p_tms->evt_handler != NULL)
            {
                p_tms->evt_handler(p_tms, BLE_TMS_EVT_NOTIF_RAW, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tms->euler_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tms->is_euler_notif_enabled != notif_enabled)
        {
            p_tms->is_euler_notif_enabled = notif_enabled;

            if (p_tms->evt_handler != NULL)
            {
                p_tms->evt_handler(p_tms, BLE_TMS_EVT_NOTIF_EULER, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tms->rot_mat_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tms->is_rot_mat_notif_enabled != notif_enabled)
        {
            p_tms->is_rot_mat_notif_enabled = notif_enabled;

            if (p_tms->evt_handler != NULL)
            {
                p_tms->evt_handler(p_tms, BLE_TMS_EVT_NOTIF_ROT_MAT, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tms->heading_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tms->is_heading_notif_enabled != notif_enabled)
        {
            p_tms->is_heading_notif_enabled = notif_enabled;

            if (p_tms->evt_handler != NULL)
            {
                p_tms->evt_handler(p_tms, BLE_TMS_EVT_NOTIF_HEADING, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else if ( (p_evt_write->handle == p_tms->gravity_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        bool notif_enabled;

        notif_enabled = ble_srv_is_notification_enabled(p_evt_write->data);

        if (p_tms->is_gravity_notif_enabled != notif_enabled)
        {
            p_tms->is_gravity_notif_enabled = notif_enabled;

            if (p_tms->evt_handler != NULL)
            {
                p_tms->evt_handler(p_tms, BLE_TMS_EVT_NOTIF_GRAVITY, p_evt_write->data, p_evt_write->len);
            }
        }
    }
    else
    {
        // Do Nothing. This event is not relevant for this service.
    }
}


static void on_authorize_req(ble_tms_t * p_tms, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_rw_authorize_request_t * p_evt_rw_authorize_request = &p_ble_evt->evt.gatts_evt.params.authorize_request;
    uint32_t err_code;

    if (p_evt_rw_authorize_request->type  == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
    {
        if (p_evt_rw_authorize_request->request.write.handle == p_tms->config_handles.value_handle)
        {
            ble_gatts_rw_authorize_reply_params_t rw_authorize_reply;
            bool                                  valid_data = true;

            // Check for valid data
            if(p_evt_rw_authorize_request->request.write.len != sizeof(ble_tms_config_t))
            {
                valid_data = false;
            }
            else
            {
                ble_tms_config_t * p_config = (ble_tms_config_t *)p_evt_rw_authorize_request->request.write.data;

                if ( (p_config->pedo_interval_ms < BLE_TMS_CONFIG_PEDO_INT_MIN)    ||
                     (p_config->pedo_interval_ms > BLE_TMS_CONFIG_PEDO_INT_MAX)    ||
                     (p_config->temp_interval_ms < BLE_TMS_CONFIG_TEMP_INT_MIN)    ||
                     (p_config->temp_interval_ms > BLE_TMS_CONFIG_TEMP_INT_MAX)    ||
                     (p_config->compass_interval_ms < BLE_TMS_CONFIG_COMP_INT_MIN) ||
                     (p_config->compass_interval_ms > BLE_TMS_CONFIG_COMP_INT_MAX) ||
                     (p_config->motion_freq_hz < BLE_TMS_CONFIG_MPUF_MIN)          ||
                     (p_config->motion_freq_hz > BLE_TMS_CONFIG_MPUF_MAX)          ||
                  /* (p_config->wake_on_motion < BLE_TMS_CONFIG_WOM_MIN)           || */
                     (p_config->wake_on_motion > BLE_TMS_CONFIG_WOM_MAX) )
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

            if ( valid_data && (p_tms->evt_handler != NULL))
            {
                p_tms->evt_handler(p_tms,
                                   BLE_TMS_EVT_CONFIG_RECEIVED,
                                   p_evt_rw_authorize_request->request.write.data,
                                   p_evt_rw_authorize_request->request.write.len);
            }
        }
    }
}


/**@brief Function for adding tap characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t tap_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    ble_tms_tap_t       tap_init = {0};

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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_TAP_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tms_tap_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&tap_init;
    attr_char_value.max_len   = sizeof(ble_tms_tap_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->tap_handles);
}


/**@brief Function for adding orientation characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t orientation_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ble_gatts_char_md_t   char_md;
    ble_gatts_attr_md_t   cccd_md;
    ble_gatts_attr_t      attr_char_value;
    ble_uuid_t            ble_uuid;
    ble_gatts_attr_md_t   attr_md;
    ble_tms_orientation_t orientation_init = {0};

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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_ORIENTATION_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tms_orientation_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&orientation_init;
    attr_char_value.max_len   = sizeof(ble_tms_orientation_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->orientation_handles);
}


/**@brief Function for adding quaternion characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t quat_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    ble_tms_quat_t      quat_init = {0};

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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_QUATERNION_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tms_quat_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&quat_init;
    attr_char_value.max_len   = sizeof(ble_tms_quat_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->quat_handles);
}


/**@brief Function for adding raw data characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t raw_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    ble_tms_raw_t       raw_init;

    memset(&raw_init, 0, sizeof(raw_init));
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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_RAW_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tms_raw_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&raw_init;
    attr_char_value.max_len   = sizeof(ble_tms_raw_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->raw_handles);
}


/**@brief Function for adding euler angle data characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t euler_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    ble_tms_euler_t     euler_init = {0};

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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_EULER_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tms_euler_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&euler_init;
    attr_char_value.max_len   = sizeof(ble_tms_euler_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->euler_handles);
}


/**@brief Function for adding rotation matrix data characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t rot_mat_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    ble_tms_rot_mat_t   rot_mat_init;

    memset(&rot_mat_init, 0, sizeof(rot_mat_init));
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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_ROT_MAT_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tms_rot_mat_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&rot_mat_init;
    attr_char_value.max_len   = sizeof(ble_tms_rot_mat_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->rot_mat_handles);
}


/**@brief Function for adding compass heading data characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t heading_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    ble_tms_heading_t   heading_init = {0};

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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_HEADING_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tms_heading_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&heading_init;
    attr_char_value.max_len   = sizeof(ble_tms_heading_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->heading_handles);
}


/**@brief Function for adding gravity data characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t gravity_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    ble_tms_gravity_t   gravity_init = {0};

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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_GRAVITY_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tms_gravity_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&gravity_init;
    attr_char_value.max_len   = sizeof(ble_tms_gravity_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->gravity_handles);
}


/**@brief Function for adding pedometer characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t pedo_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    ble_tms_pedo_t      pedo_init = {0};

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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_PEDOMETER_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 1;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(ble_tms_pedo_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)&pedo_init;
    attr_char_value.max_len   = sizeof(ble_tms_pedo_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->pedo_handles);
}


/**@brief Function for adding configuration characteristic.
 *
 * @param[in] p_tms       Motion Service structure.
 * @param[in] p_tms_init  Information needed to initialize the service.
 *
 * @return NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t config_char_add(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
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

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_CONFIG_CHAR;

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
    attr_char_value.init_len  = sizeof(ble_tms_config_t);
    attr_char_value.init_offs = 0;
    attr_char_value.p_value   = (uint8_t *)p_tms_init->p_init_config;
    attr_char_value.max_len   = sizeof(ble_tms_config_t);

    return sd_ble_gatts_characteristic_add(p_tms->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_tms->config_handles);
}


void ble_tms_on_ble_evt(ble_tms_t * p_tms, ble_evt_t * p_ble_evt)
{
    if ((p_tms == NULL) || (p_ble_evt == NULL))
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_tms, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_tms, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_tms, p_ble_evt);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            on_authorize_req(p_tms, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


uint32_t ble_tms_init(ble_tms_t * p_tms, const ble_tms_init_t * p_tms_init)
{
    uint32_t      err_code;
    ble_uuid_t    ble_uuid;
    ble_uuid128_t tms_base_uuid = TMS_BASE_UUID;

    VERIFY_PARAM_NOT_NULL(p_tms);
    VERIFY_PARAM_NOT_NULL(p_tms_init);

    // Initialize the service structure.
    p_tms->conn_handle                  = BLE_CONN_HANDLE_INVALID;
    p_tms->evt_handler                  = p_tms_init->evt_handler;
    p_tms->is_tap_notif_enabled         = false;
    p_tms->is_orientation_notif_enabled = false;
    p_tms->is_quat_notif_enabled        = false;
    p_tms->is_pedo_notif_enabled        = false;
    p_tms->is_raw_notif_enabled         = false;
    p_tms->is_euler_notif_enabled       = false;
    p_tms->is_rot_mat_notif_enabled     = false;
    p_tms->is_heading_notif_enabled     = false;
    p_tms->is_gravity_notif_enabled     = false;


    // Add a custom base UUID.
    err_code = sd_ble_uuid_vs_add(&tms_base_uuid, &p_tms->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_tms->uuid_type;
    ble_uuid.uuid = BLE_UUID_TMS_SERVICE;

    // Add the service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_tms->service_handle);
    VERIFY_SUCCESS(err_code);

    /* Add characteristics */
    // Add the configuration characteristic.
    err_code = config_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);
    // Add the tap characteristic.
    err_code = tap_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);
    // Add the orientation characteristic.
    err_code = orientation_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);
    // Add the quaternion characteristic.
    err_code = quat_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);
    // Add the pedometer characteristic.
    err_code = pedo_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);
    // Add the raw data characteristic.
    err_code = raw_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);
    // Add the euler angle characteristic.
    err_code = euler_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);
    // Add the rotation matrix characteristic.
    err_code = rot_mat_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);
    // Add the compass heading characteristic.
    err_code = heading_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);
    // Add the gravity vector characteristic.
    err_code = gravity_char_add(p_tms, p_tms_init);
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}


uint32_t ble_tms_tap_set(ble_tms_t * p_tms, ble_tms_tap_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tms_tap_t);

    VERIFY_PARAM_NOT_NULL(p_tms);

    if ((p_tms->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tms->is_tap_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TMS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tms->tap_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tms->conn_handle, &hvx_params);
}


uint32_t ble_tms_orientation_set(ble_tms_t * p_tms, ble_tms_orientation_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tms_orientation_t);

    VERIFY_PARAM_NOT_NULL(p_tms);

    if ((p_tms->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tms->is_orientation_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TMS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tms->orientation_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tms->conn_handle, &hvx_params);
}


uint32_t ble_tms_quat_set(ble_tms_t * p_tms, ble_tms_quat_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tms_quat_t);

    VERIFY_PARAM_NOT_NULL(p_tms);

    if ((p_tms->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tms->is_quat_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TMS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tms->quat_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tms->conn_handle, &hvx_params);
}


uint32_t ble_tms_pedo_set(ble_tms_t * p_tms, ble_tms_pedo_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tms_pedo_t);

    VERIFY_PARAM_NOT_NULL(p_tms);

    if ((p_tms->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tms->is_pedo_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TMS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tms->pedo_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tms->conn_handle, &hvx_params);
}


uint32_t ble_tms_raw_set(ble_tms_t * p_tms, ble_tms_raw_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tms_raw_t);

    VERIFY_PARAM_NOT_NULL(p_tms);

    if ((p_tms->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tms->is_raw_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TMS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tms->raw_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tms->conn_handle, &hvx_params);
}


uint32_t ble_tms_euler_set(ble_tms_t * p_tms, ble_tms_euler_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tms_euler_t);

    VERIFY_PARAM_NOT_NULL(p_tms);

    if ((p_tms->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tms->is_euler_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TMS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tms->euler_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tms->conn_handle, &hvx_params);
}


uint32_t ble_tms_rot_mat_set(ble_tms_t * p_tms, ble_tms_rot_mat_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tms_rot_mat_t);

    VERIFY_PARAM_NOT_NULL(p_tms);

    if ((p_tms->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tms->is_rot_mat_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TMS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tms->rot_mat_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tms->conn_handle, &hvx_params);
}


uint32_t ble_tms_heading_set(ble_tms_t * p_tms, ble_tms_heading_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tms_heading_t);

    VERIFY_PARAM_NOT_NULL(p_tms);

    if ((p_tms->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tms->is_heading_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TMS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tms->heading_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tms->conn_handle, &hvx_params);
}


uint32_t ble_tms_gravity_set(ble_tms_t * p_tms, ble_tms_gravity_t * p_data)
{
    ble_gatts_hvx_params_t hvx_params;
    uint16_t               length = sizeof(ble_tms_gravity_t);

    VERIFY_PARAM_NOT_NULL(p_tms);

    if ((p_tms->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_tms->is_gravity_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    if (length > BLE_TMS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_tms->gravity_handles.value_handle;
    hvx_params.p_data = (uint8_t *)p_data;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(p_tms->conn_handle, &hvx_params);
}
