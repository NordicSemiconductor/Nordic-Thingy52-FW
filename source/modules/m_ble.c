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

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_conn_state.h"
#include "softdevice_handler_appsh.h"
#include "app_timer.h"
#include "fstorage.h"
#include "m_ble.h"
#include "m_ble_flash.h"
#include "thingy_config.h"
#include "advertiser_beacon.h"
#include "pca20020.h"
#include "nrf_delay.h"

#ifdef BLE_DFU_APP_SUPPORT
    #include "ble_dfu.h"
#endif // BLE_DFU_APP_SUPPORT

#ifdef BLE_DFU_APP_SUPPORT
    #define DFU_REV_MAJOR                    0x00                                       /** DFU Major revision number to be exposed. */
    #define DFU_REV_MINOR                    0x01                                       /** DFU Minor revision number to be exposed. */
    #define DFU_REVISION                     ((DFU_REV_MAJOR << 8) | DFU_REV_MINOR)     /** DFU Revision number to be exposed. Combined of major and minor versions. */
    #define APP_SERVICE_HANDLE_START         0x000C                                     /**< Handle of first application specific service when when service changed characteristic is present. */
    #define BLE_HANDLE_MAX                   0xFFFF                                     /**< Max handle value in BLE. */

    STATIC_ASSERT(IS_SRVC_CHANGED_CHARACT_PRESENT);                                     /** When having DFU Service support in application the Service Changed Characteristic should always be present. */
#endif // BLE_DFU_APP_SUPPORT

#ifdef TB_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

static uint16_t                   m_conn_handle = BLE_CONN_HANDLE_INVALID;      /**< Handle of the current connection. */
static m_ble_evt_handler_t        m_evt_handler = 0;
static m_ble_service_handle_t   * m_service_handles = 0;
static uint32_t                   m_service_num = 0;

static ble_tcs_t                  m_tcs;
static ble_tcs_params_t         * m_ble_config;
static const ble_tcs_params_t     m_ble_default_config = THINGY_CONFIG_DEFAULT;
static ble_tcs_mtu_t              m_mtu;
static bool                       m_flash_disconnect = false;

#if (NRF_SD_BLE_API_VERSION == 3)
    #define NRF_BLE_MAX_MTU_SIZE            GATT_MTU_SIZE_DEFAULT*12            /**< MTU size used in the softdevice enabling and to reply to a BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST event. */
#endif

#ifdef BLE_DFU_APP_SUPPORT
    static ble_dfu_t                  m_dfus;                                   /**< Structure used to identify the DFU service. */

    static void ble_dfu_evt_handler(ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt)
    {
        switch (p_evt->type)
        {
            case BLE_DFU_EVT_INDICATION_DISABLED:
                DEBUG_PRINTF(0, "Indication for BLE_DFU is disabled\r\n");
                break;

            case BLE_DFU_EVT_INDICATION_ENABLED:
                DEBUG_PRINTF(0, "Indication for BLE_DFU is enabled\r\n");
                break;

            case BLE_DFU_EVT_ENTERING_BOOTLOADER:
                DEBUG_PRINTF(0, "Device is entering bootloader mode!\r\n");
                break;
            default:
                DEBUG_PRINTF(0, "Unknown event from ble_dfu\r\n");
                break;
        }
    }

#endif // BLE_DFU_APP_SUPPORT


/**@brief Check if flash is currently being accessed.
 */
static bool flash_access_ongoing(void)
{
    fs_ret_t err_code;
    uint32_t flash_op_cnt = 0;

    err_code = fs_queued_op_count_get(&flash_op_cnt);
    APP_ERROR_CHECK(err_code);

    return (flash_op_cnt == 0) ? false : true;
}

/**@brief Function for the GAP initialization.
 *
 * @details This function will set up all the necessary GAP (Generic Access Profile) parameters of
 *          the device. It also sets the permissions and appearance.
 */
static uint32_t gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          m_ble_config->dev_name.name,
                                          strlen((const char *)m_ble_config->dev_name.name));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = m_ble_config->conn_params.min_conn_int;
    gap_conn_params.max_conn_interval = m_ble_config->conn_params.max_conn_int;
    gap_conn_params.slave_latency     = m_ble_config->conn_params.slave_latency;
    gap_conn_params.conn_sup_timeout  = m_ble_config->conn_params.sup_timeout;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    if (err_code == NRF_ERROR_INVALID_PARAM)
    {
        // Use default config
        m_ble_config->conn_params.min_conn_int  = (uint16_t)MSEC_TO_UNITS(MIN_CONN_INTERVAL_MS, UNIT_1_25_MS);
        m_ble_config->conn_params.max_conn_int  = MSEC_TO_UNITS(MAX_CONN_INTERVAL_MS, UNIT_1_25_MS);
        m_ble_config->conn_params.slave_latency = SLAVE_LATENCY;
        m_ble_config->conn_params.sup_timeout   = MSEC_TO_UNITS(CONN_SUP_TIMEOUT_MS, UNIT_10_MS);

        err_code = m_ble_flash_config_store(m_ble_config);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }

        gap_conn_params.min_conn_interval = (uint16_t)MSEC_TO_UNITS(MIN_CONN_INTERVAL_MS, UNIT_1_25_MS);
        gap_conn_params.max_conn_interval = MSEC_TO_UNITS(MAX_CONN_INTERVAL_MS, UNIT_1_25_MS);
        gap_conn_params.slave_latency     = SLAVE_LATENCY;
        gap_conn_params.conn_sup_timeout  = MSEC_TO_UNITS(CONN_SUP_TIMEOUT_MS, UNIT_10_MS);

        err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }
    else if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}

/**@brief Function for handling an event from the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module
 *          which are passed to the application.
 *
 * @note All this function does is to disconnect. This could have been done by simply setting
 *       the disconnect_on_fail config parameter, but instead we use the event handler
 *       mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);

        DEBUG_PRINTF(0, "on_conn_params_evt: BLE_CONN_PARAMS_EVT_FAILED\r\n");
    }
    else if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_SUCCEEDED)
    {
        DEBUG_PRINTF(0, "on_conn_params_evt: BLE_CONN_PARAMS_EVT_SUCCEEDED\r\n");
    }
    else
    {

    }
}


/**@brief Function for handling errors from the Connection Parameters module.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    DEBUG_PRINTF(0, "conn_params_error_handler: %d\r\n", nrf_error);
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static uint32_t conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;
    ble_gap_conn_params_t  gap_conn_params;

    memset(&cp_init, 0, sizeof(cp_init));
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = m_ble_config->conn_params.min_conn_int;
    gap_conn_params.max_conn_interval = m_ble_config->conn_params.max_conn_int;
    gap_conn_params.slave_latency     = m_ble_config->conn_params.slave_latency;
    gap_conn_params.conn_sup_timeout  = m_ble_config->conn_params.sup_timeout;

    cp_init.p_conn_params                  = &gap_conn_params;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


 /**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    uint32_t      err_code;
    m_ble_evt_t evt;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            DEBUG_PRINTF(0, "on_adv_evt: BLE_ADV_EVT_FAST\r\n");
            break;
        case BLE_ADV_EVT_IDLE:
            DEBUG_PRINTF(0, "on_adv_evt: BLE_ADV_EVT_IDLE\r\n");
            evt.evt_type = thingy_ble_evt_timeout;
            m_evt_handler(&evt);

            err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
            APP_ERROR_CHECK(err_code);

            break;
        default:
            break;
    }
}


/**@brief Function for the application's SoftDevice event handler.
 *
 * @param[in] p_ble_evt SoftDevice event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t                       err_code;
    m_ble_evt_t                    evt;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            DEBUG_PRINTF(0, "on_ble_evt: BLE_GAP_EVT_CONNECTED\r\n");
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

            evt.evt_type = thingy_ble_evt_connected;
            m_evt_handler(&evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            DEBUG_PRINTF(0, "on_ble_evt: BLE_GAP_EVT_DISCONNECTED\r\n");
            m_conn_handle = BLE_CONN_HANDLE_INVALID;

            if (flash_access_ongoing())
            {
                m_flash_disconnect = true;
            }
            else
            {
                m_flash_disconnect = false;
                evt.evt_type = thingy_ble_evt_disconnected;
                m_evt_handler(&evt);
            }

            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported.
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle, BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_EVT_USER_MEM_REQUEST:
            err_code = sd_ble_user_mem_reply(m_conn_handle, NULL);
            APP_ERROR_CHECK(err_code);
            break; // BLE_EVT_USER_MEM_REQUEST

#if (NRF_SD_BLE_API_VERSION == 3)
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
        {
            DEBUG_PRINTF(0, "on_ble_evt: BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST - %d\r\n", p_ble_evt->evt.gatts_evt.params.exchange_mtu_request.client_rx_mtu);
            err_code = sd_ble_gatts_exchange_mtu_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                       NRF_BLE_MAX_MTU_SIZE);
            APP_ERROR_CHECK(err_code);

            m_mtu.size = p_ble_evt->evt.gatts_evt.params.exchange_mtu_request.client_rx_mtu;
            err_code = ble_tcs_mtu_set(&m_tcs, &m_mtu);
            APP_ERROR_CHECK(err_code);
        }
        break; // BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST
#endif

        case BLE_GATTC_EVT_EXCHANGE_MTU_RSP:
            DEBUG_PRINTF(0, "on_ble_evt: BLE_GATTC_EVT_EXCHANGE_MTU_RSP - %d\r\n", p_ble_evt->evt.gattc_evt.params.exchange_mtu_rsp.server_rx_mtu);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for dispatching a SoftDevice event to all modules with a SoftDevice
 *        event handler.
 *
 * @details This function is called from the SoftDevice event interrupt handler after a
 *          SoftDevice event has been received.
 *
 * @param[in] p_ble_evt  SoftDevice event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_state_on_ble_evt(p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);

    for (uint32_t i = 0; i < m_service_num; i++)
    {
        if (m_service_handles[i].ble_evt_cb != NULL)
        {
            m_service_handles[i].ble_evt_cb(p_ble_evt);
        }
    }

    ble_tcs_on_ble_evt(&m_tcs, p_ble_evt);
#ifdef BLE_DFU_APP_SUPPORT
    ble_dfu_on_ble_evt(&m_dfus, p_ble_evt);
#endif // BLE_DFU_APP_SUPPORT
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);
}


/**@brief Function for dispatching system events from the SoftDevice.
 *
 * @details This function is called from the SoftDevice event interrupt handler after a
 *          SoftDevice event has been received.
 *
 * @param[in] evt_id  System event id.
 */
static void sys_evt_dispatch(uint32_t evt_id)
{
    fs_sys_event_handler(evt_id);
    app_beacon_on_sys_evt(evt_id);
    ble_advertising_on_sys_evt(evt_id);

    if ( (evt_id == NRF_EVT_FLASH_OPERATION_SUCCESS) ||
         (evt_id == NRF_EVT_FLASH_OPERATION_ERROR) )
    {
        if (!flash_access_ongoing())
        {
            if (m_flash_disconnect)
            {
                m_ble_evt_t  evt;
                evt.evt_type = thingy_ble_evt_disconnected;

                m_evt_handler(&evt);
            }
        }
    }
}


/**@brief Function for the SoftDevice initialization.
 *
 * @details This function initializes the SoftDevice and the BLE event interrupt.
 */
static uint32_t ble_stack_init(void)
{
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

    // Initialize SoftDevice.
    SOFTDEVICE_HANDLER_APPSH_INIT(&clock_lf_cfg, true);
    //SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    ble_enable_params.common_enable_params.vs_uuid_count = 2 + m_service_num; // ble_tcs + ble_dfu + other thingy services
    ble_enable_params.gatts_enable_params.attr_tab_size = 3000;

#ifdef BLE_DFU_APP_SUPPORT
    ble_enable_params.gatts_enable_params.service_changed = 1;
#endif // BLE_DFU_APP_SUPPORT

    //Check the ram settings against the used number of links.
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
#if (NRF_SD_BLE_API_VERSION == 3)
    ble_enable_params.gatt_enable_params.att_mtu = NRF_BLE_MAX_MTU_SIZE;
#endif
    err_code = softdevice_enable(&ble_enable_params);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Subscribe for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = sd_power_mode_set(NRF_POWER_MODE_LOWPWR);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


/**@brief Function for initializing the Advertising functionality.
 */
static uint32_t advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_uuid_t    adv_uuids = {BLE_UUID_TCS_SERVICE,m_tcs.uuid_type};

    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = false;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;
    advdata.uuids_more_available.uuid_cnt = 1;
    advdata.uuids_more_available.p_uuids = &adv_uuids;

    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = true;
    options.ble_adv_fast_interval = m_ble_config->adv_params.interval;
    options.ble_adv_fast_timeout  = m_ble_config->adv_params.timeout;

    err_code = ble_advertising_init(&advdata, NULL, &options, on_adv_evt, NULL);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


/**@brief Function for handling a BeaconAdvertiser error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void beacon_advertiser_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the beacon timeslot functionality.
 */
static uint32_t timeslot_init(void)
{
    uint32_t err_code;
    static ble_beacon_init_t beacon_init;

    if (m_ble_config->eddystone_url.len < BLE_TCS_BEACON_LEN_MIN)
    {
        app_beacon_stop();
        return NRF_SUCCESS;
    }

    beacon_init.adv_interval  = THINGY_BEACON_ADV_INTERVAL;
    beacon_init.p_data         = m_ble_config->eddystone_url.data;
    beacon_init.data_size      = m_ble_config->eddystone_url.len;
    beacon_init.error_handler = beacon_advertiser_error_handler;

    #if (NRF_SD_BLE_API_VERSION == 3)
        err_code = sd_ble_gap_addr_get(&beacon_init.beacon_addr);
    #else
        err_code = sd_ble_gap_address_get(&beacon_init.beacon_addr);
    #endif
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Increment device address by 2 for beacon advertising.
    beacon_init.beacon_addr.addr[0] += 2;

    app_beacon_init(&beacon_init);
    app_beacon_start();

    return NRF_SUCCESS;
}


/**@brief Function for handling thingy configuration events.
 */
static void tcs_evt_handler (ble_tcs_t        * p_tcs,
                             ble_tcs_evt_type_t evt_type,
                             uint8_t          * p_data,
                             uint16_t           length)
{
    bool update_flash = false;

    switch (evt_type)
    {
        case BLE_TCS_EVT_DEV_NAME:
            if (length <= BLE_TCS_DEVICE_NAME_LEN_MAX)
            {
                memcpy(m_ble_config->dev_name.name, p_data, length);
                m_ble_config->dev_name.name[length] = 0;
                m_ble_config->dev_name.len = length;
                update_flash = true;
            }
            break;
        case BLE_TCS_EVT_ADV_PARAM:
            if (length == sizeof(ble_tcs_adv_params_t))
            {
                memcpy(&m_ble_config->adv_params, p_data, length);

                update_flash = true;
            }
            break;
        case BLE_TCS_EVT_CONN_PARAM:
            if (length == sizeof(ble_tcs_conn_params_t))
            {
                uint32_t              err_code;
                ble_gap_conn_params_t gap_conn_params;

                memcpy(&m_ble_config->conn_params, p_data, length);
                memset(&gap_conn_params, 0, sizeof(gap_conn_params));

                gap_conn_params.min_conn_interval = m_ble_config->conn_params.min_conn_int;
                gap_conn_params.max_conn_interval = m_ble_config->conn_params.max_conn_int;
                gap_conn_params.slave_latency     = m_ble_config->conn_params.slave_latency;
                gap_conn_params.conn_sup_timeout  = m_ble_config->conn_params.sup_timeout;

                err_code = ble_conn_params_change_conn_params(&gap_conn_params);
                APP_ERROR_CHECK(err_code);

                update_flash = true;
            }
            break;
        case BLE_TCS_EVT_BEACON:
            if (length <= BLE_TCS_BEACON_LEN_MAX)
            {
                uint32_t err_code;

                memcpy(m_ble_config->eddystone_url.data, p_data, length);
                m_ble_config->eddystone_url.len = length;
                update_flash = true;

                err_code = timeslot_init();
                APP_ERROR_CHECK(err_code);
            }
            break;
        case BLE_TCS_EVT_CLOUD_TOKEN:
            if (length <= BLE_TCS_CLOUD_LEN_MAX)
            {
                memcpy(m_ble_config->cloud_token.token, p_data, length);
                m_ble_config->cloud_token.len = length;
                update_flash = true;
            }
            break;

        case BLE_TCS_EVT_MTU:
            if (length == sizeof(ble_tcs_mtu_t))
            {
                uint32_t        err_code;
                ble_tcs_mtu_t * p_mtu = (ble_tcs_mtu_t *)p_data;

                if (p_mtu->req == TCS_MTU_REQ_EXCHANGE)
                {
                    DEBUG_PRINTF(0, "tcs_evt_handler: TCS_MTU_REQ_EXCHANGE - %d\r\n", p_mtu->size);
                    err_code = sd_ble_gattc_exchange_mtu_request(m_conn_handle, p_mtu->size);
                    if (err_code == NRF_SUCCESS)
                    {
                        memcpy(&m_mtu, p_data, length);
                    }
                    else
                    {
                        err_code = ble_tcs_mtu_set(&m_tcs, &m_mtu);
                        APP_ERROR_CHECK(err_code);
                    }
                }
                else
                {
                    err_code = ble_tcs_mtu_set(&m_tcs, &m_mtu);
                    APP_ERROR_CHECK(err_code);
                }
            }
            break;
    }

    if (update_flash)
    {
        uint32_t err_code;

        err_code = m_ble_flash_config_store(m_ble_config);
        APP_ERROR_CHECK(err_code);
    }
}


static uint32_t thingy_config_verify(void)
{
    bool update_flash = false;
    uint32_t err_code;

    if ( (m_ble_config->fw_version.major != m_ble_default_config.fw_version.major) ||
         (m_ble_config->fw_version.minor != m_ble_default_config.fw_version.minor) ||
         (m_ble_config->fw_version.patch != m_ble_default_config.fw_version.patch) )
    {
        err_code = m_ble_flash_config_store((ble_tcs_params_t *)&m_ble_default_config);
        APP_ERROR_CHECK(err_code);
    }

    // Check Eddystone URL length.
    if (m_ble_config->eddystone_url.len > 17)
    {
        memcpy(m_ble_config->eddystone_url.data, m_ble_default_config.eddystone_url.data, m_ble_default_config.eddystone_url.len);
        m_ble_config->eddystone_url.len = m_ble_default_config.eddystone_url.len;
        update_flash = true;
    }

    // Check Cloud Token length.
    if (m_ble_config->cloud_token.len > BLE_TCS_CLOUD_LEN_MAX)
    {
        memcpy(m_ble_config->cloud_token.token, m_ble_default_config.cloud_token.token, m_ble_default_config.cloud_token.len);
        m_ble_config->cloud_token.len = m_ble_default_config.cloud_token.len;
        update_flash = true;
    }

    if (update_flash)
    {
        err_code = m_ble_flash_config_store(m_ble_config);
        APP_ERROR_CHECK(err_code);
    }

    return NRF_SUCCESS;
}


static uint32_t thingy_config_init(void)
{
    ble_tcs_init_t params;
    uint32_t err_code;

    params.p_init_vals = m_ble_config;

    params.evt_handler = tcs_evt_handler;
    err_code = ble_tcs_init(&m_tcs, &params);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}


/**@brief Function for initializing the ble services.
 */
static uint32_t services_init(m_ble_service_handle_t * p_service_handles, uint32_t num_services)
{
    uint32_t err_code;

    err_code = thingy_config_init();
    APP_ERROR_CHECK(err_code);

    for (uint32_t i = 0; i < num_services; i++)
    {
        if (p_service_handles[i].init_cb != NULL)
        {
            err_code = p_service_handles[i].init_cb();
            if (err_code != NRF_SUCCESS)
            {
                return err_code;
            }
        }
    }


    #ifdef BLE_DFU_APP_SUPPORT
        ble_dfu_init_t   dfus_init;

        // Initialize the Device Firmware Update Service.
        memset(&dfus_init, 0, sizeof(dfus_init));

        dfus_init.evt_handler                               = ble_dfu_evt_handler;
        dfus_init.ctrl_point_security_req_write_perm        = SEC_OPEN;
        dfus_init.ctrl_point_security_req_cccd_write_perm   = SEC_OPEN;

        err_code = ble_dfu_init(&m_dfus, &dfus_init);
        APP_ERROR_CHECK(err_code);

    #endif // BLE_DFU_APP_SUPPORT

    return NRF_SUCCESS;
}


uint32_t m_ble_init(m_ble_init_t * p_params)
{
    uint32_t err_code;

    m_evt_handler     = p_params->evt_handler;
    m_service_handles = p_params->p_service_handles;
    m_service_num     = p_params->service_num;

    err_code = ble_stack_init();
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "thingy_ble_init: ble_stack_init failed - %d\r\n", err_code);
        return err_code;
    }

        /* Enable FPU again due to SD issue */
    #if (__FPU_USED == 1)
        SCB->CPACR |= (3UL << 20) | (3UL << 22);
        __DSB();
        __ISB();
    #endif

    /**@brief Load configuration from flash. */
    err_code = m_ble_flash_init(&m_ble_default_config, &m_ble_config);
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "thingy_ble_init: m_ble_flash_init failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = thingy_config_verify();
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "thingy_ble_init: thingy_config_verify failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = gap_params_init();
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "thingy_ble_init: gap_params_init failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = services_init(m_service_handles, m_service_num);
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "thingy_ble_init: services_init failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = advertising_init();
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "thingy_ble_init: advertising_init failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = conn_params_init();
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "thingy_ble_init: conn_params_init failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "thingy_ble_init: ble_advertising_start failed - %d\r\n", err_code);
        return err_code;
    }

    nrf_delay_ms (10);

    err_code = timeslot_init();
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "thingy_ble_init: timeslot_init failed - %d\r\n", err_code);
        return err_code;
    }

    return NRF_SUCCESS;
}
