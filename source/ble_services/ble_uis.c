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

#include "ble_uis.h"
#include "ble_srv_common.h"
#include "sdk_common.h"

/**@brief Function for handling the Connect event.
 *
 * @param[in] p_uis      UI Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_connect(ble_uis_t * p_uis, ble_evt_t * p_ble_evt)
{
    p_uis->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in] p_uis      UI Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_disconnect(ble_uis_t * p_uis, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_uis->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief Function for handling the Write event.
 *
 * @param[in] p_uis      UI Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_write(ble_uis_t * p_uis, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if ( (p_evt_write->handle == p_uis->button_char_handles.cccd_handle) &&
         (p_evt_write->len == 2) )
    {
        if (ble_srv_is_notification_enabled(p_evt_write->data))
        {
            p_uis->is_button_notif_enabled = true;
        }
        else
        {
            p_uis->is_button_notif_enabled = false;
        }
    }
    else
    {
        // Do Nothing. This event is not relevant for this service.
    }
}


/**@brief Checks data validity for the UI service
 *
 * @param[in] data_length  Length of recieved LED characteristic data
 * @param[in] p_led        Pointer to the data structure
 *
 * @return    true if all checks pass, else: false
 */
static bool valid_data_check(uint16_t data_length, ble_uis_led_t const * const p_led)
{
    //lint -save -e{685}
    if(p_led->mode > BLE_UIS_LED_MODE_MAX)
    {
        return false;
    }
    //lint -restore

    switch(p_led->mode)
    {
        case BLE_UIS_LED_MODE_OFF:
            if (data_length != BLE_UIS_LED_MODE_OFF_LENGTH)
            {
                return false;
            }
            // Further packet content need not be checked
            break;

        case BLE_UIS_LED_MODE_CONST:
            if (data_length != BLE_UIS_LED_MODE_CONST_LENGTH)
            {
                return false;
            }
            // Further packet content need not be checked
            break;

        case BLE_UIS_LED_MODE_BREATHE:
            if ( (data_length != BLE_UIS_LED_MODE_BREATHE_LENGTH)                ||
               (p_led->data.mode_breathe.color_mix < BLE_UIS_LED_COLOR_MIX_MIN)  ||
               (p_led->data.mode_breathe.color_mix > BLE_UIS_LED_COLOR_MIX_MAX)  ||
               (p_led->data.mode_breathe.intensity < BLE_UIS_LED_INTENSITY_MIN)  ||
               (p_led->data.mode_breathe.intensity > BLE_UIS_LED_INTENSITY_MAX)  ||
               (p_led->data.mode_breathe.delay     < BLE_UIS_LED_DELAY_MIN)      ||
               (p_led->data.mode_breathe.delay     > BLE_UIS_LED_DELAY_MAX) )
            {
                return false;
            }

            break;

        case BLE_UIS_LED_MODE_BREATHE_ONE_SHOT:
            if ( (data_length != BLE_UIS_LED_MODE_BREATHE_ONE_SHOT_LENGTH)                ||
               (p_led->data.mode_breathe_one_shot.color_mix < BLE_UIS_LED_COLOR_MIX_MIN)  ||
               (p_led->data.mode_breathe_one_shot.color_mix > BLE_UIS_LED_COLOR_MIX_MAX)  ||
               (p_led->data.mode_breathe_one_shot.intensity < BLE_UIS_LED_INTENSITY_MIN)  ||
               (p_led->data.mode_breathe_one_shot.intensity > BLE_UIS_LED_INTENSITY_MAX) )
            {
                return false;
            }

            break;

        default:
            return false;
    }

    return true;
}


static void on_authorize_req(ble_uis_t * p_uis, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_rw_authorize_request_t * p_evt_rw_authorize_request = &p_ble_evt->evt.gatts_evt.params.authorize_request;
    uint32_t err_code;

    if (p_evt_rw_authorize_request->type  == BLE_GATTS_AUTHORIZE_TYPE_WRITE)
    {
        if (p_evt_rw_authorize_request->request.write.handle == p_uis->led_char_handles.value_handle)
        {
            ble_gatts_rw_authorize_reply_params_t rw_authorize_reply;
            bool                                  valid_data = true;
            uint16_t                              data_length = p_evt_rw_authorize_request->request.write.len;

            // Check for valid data
            if (data_length > sizeof(ble_uis_led_t))
            {
                valid_data = false;
            }
            else
            {
                ble_uis_led_t * p_led = (ble_uis_led_t *)p_evt_rw_authorize_request->request.write.data;
                valid_data = valid_data_check(data_length, p_led);
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

            if ( valid_data && (p_uis->led_write_handler != NULL))
            {
                p_uis->led_write_handler(p_uis, (ble_uis_led_t *)p_evt_rw_authorize_request->request.write.data);
            }
        }
        else
        if (p_evt_rw_authorize_request->request.write.handle == p_uis->pin_char_handles.value_handle)
        {
            ble_gatts_rw_authorize_reply_params_t rw_authorize_reply;
            bool                                  valid_data = true;
            uint16_t                              data_length = p_evt_rw_authorize_request->request.write.len;

            // Check for valid data
            if (data_length != sizeof(ble_uis_pin_t))
            {
                valid_data = false;
            }
            else
            {
                ble_uis_pin_t * p_pin = (ble_uis_pin_t *)p_evt_rw_authorize_request->request.write.data;

                if ( ((p_pin->mos_1 > 0) && (p_pin->mos_1 < 255)) ||
                     ((p_pin->mos_2 > 0) && (p_pin->mos_2 < 255)) ||
                     ((p_pin->mos_3 > 0) && (p_pin->mos_3 < 255)) ||
                     ((p_pin->mos_4 > 0) && (p_pin->mos_4 < 255)) )
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

            if ( valid_data && (p_uis->led_write_handler != NULL))
            {
                p_uis->pin_write_handler(p_uis, (ble_uis_pin_t *)p_evt_rw_authorize_request->request.write.data);
            }
        }
    }
}


void ble_uis_on_ble_evt(ble_uis_t * p_uis, ble_evt_t * p_ble_evt)
{
    if ((p_uis == NULL) || (p_ble_evt == NULL))
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_uis, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_uis, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_uis, p_ble_evt);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            on_authorize_req(p_uis, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for adding the LED Characteristic.
 *
 * @param[in] p_uis      UI Service structure.
 * @param[in] p_uis_init UI Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t led_char_add(ble_uis_t * p_uis, const ble_uis_init_t * p_uis_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;

    ble_uuid.type = p_uis->uuid_type;
    ble_uuid.uuid = BLE_UIS_UUID_LED_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 1;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(ble_uis_led_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(ble_uis_led_t);;
    attr_char_value.p_value      = ((uint8_t *)(p_uis_init->p_init_led));

    return sd_ble_gatts_characteristic_add(p_uis->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_uis->led_char_handles);
}


/**@brief Function for adding the Button Characteristic.
 *
 * @param[in] p_uis      UI Service structure.
 * @param[in] p_uis_init UI Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t button_char_add(ble_uis_t * p_uis, const ble_uis_init_t * p_uis_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t            init_value = 0;

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

    ble_uuid.type = p_uis->uuid_type;
    ble_uuid.uuid = BLE_UIS_UUID_BUTTON_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);

    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = ((uint8_t *)(&init_value));

    return sd_ble_gatts_characteristic_add(p_uis->service_handle,
                                          &char_md,
                                          &attr_char_value,
                                          &p_uis->button_char_handles);
}


/**@brief Function for adding the PIN Characteristic.
 *
 * @param[in] p_uis      UI Service structure.
 * @param[in] p_uis_init UI Service initialization structure.
 *
 * @retval NRF_SUCCESS on success, else an error value from the SoftDevice
 */
static uint32_t pin_char_add(ble_uis_t * p_uis, const ble_uis_init_t * p_uis_init)
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

    ble_uuid.type = p_uis->uuid_type;
    ble_uuid.uuid = BLE_UIS_UUID_PIN_CHAR;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 1;
    attr_md.vlen       = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(ble_uis_pin_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(ble_uis_pin_t);;
    attr_char_value.p_value      = ((uint8_t *)(&p_uis_init->init_pin));

    return sd_ble_gatts_characteristic_add(p_uis->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_uis->pin_char_handles);
}


uint32_t ble_uis_init(ble_uis_t * p_uis, const ble_uis_init_t * p_uis_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure.
    p_uis->conn_handle             = BLE_CONN_HANDLE_INVALID;
    p_uis->led_write_handler       = p_uis_init->led_write_handler;
    p_uis->pin_write_handler       = p_uis_init->pin_write_handler;
    p_uis->is_button_notif_enabled = false;
    // Add service.
    ble_uuid128_t base_uuid = BLE_UIS_UUID_BASE;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_uis->uuid_type);
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_uis->uuid_type;
    ble_uuid.uuid = BLE_UIS_UUID_SERVICE;

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_uis->service_handle);
    VERIFY_SUCCESS(err_code);

    // Add characteristics.
    err_code = button_char_add(p_uis, p_uis_init);
    VERIFY_SUCCESS(err_code);

    err_code = led_char_add(p_uis, p_uis_init);
    VERIFY_SUCCESS(err_code);

    err_code = pin_char_add(p_uis, p_uis_init);
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}


uint32_t ble_uis_on_button_change(ble_uis_t * p_uis, uint8_t buttons_state)
{
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(buttons_state);

    VERIFY_PARAM_NOT_NULL(p_uis);

    if ((p_uis->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_uis->is_button_notif_enabled))
    {
        return NRF_ERROR_INVALID_STATE;
    }

    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_uis->button_char_handles.value_handle;
    params.p_data = (uint8_t *)(&buttons_state);
    params.p_len = &len;

    return sd_ble_gatts_hvx(p_uis->conn_handle, &params);
}
