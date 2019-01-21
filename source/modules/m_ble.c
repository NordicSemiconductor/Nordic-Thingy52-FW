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
#include "drv_nfc.h"
#include "thingy_config.h"
#include "advertiser_beacon.h"
#include "pca20020.h"
#include "nrf_delay.h"
#include "nrf_drv_rng.h"
#include "support_func.h"

#define  NRF_LOG_MODULE_NAME "m_ble         "
#include "nrf_log.h"
#include "macros_common.h"

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

#define RANDOM_VECTOR_DEVICE_ID_SIZE         4                                          /** Length of random ID vector. Must be <= 32. */

static uint16_t                   m_conn_handle = BLE_CONN_HANDLE_INVALID;              /**< Handle of the current connection. */
static m_ble_evt_handler_t        m_evt_handler = 0;
static m_ble_service_handle_t   * m_service_handles = 0;
static uint32_t                   m_service_num = 0;
static ble_tcs_t                  m_tcs;
static ble_tcs_params_t         * m_ble_config;
static const ble_tcs_params_t     m_ble_default_config = THINGY_CONFIG_DEFAULT;
static ble_tcs_mtu_t              m_mtu;
static bool                       m_flash_disconnect = false;
static bool                       m_major_minor_fw_ver_changed = false;
static char                       m_mac_addr[SUPPORT_FUNC_MAC_ADDR_STR_LEN];            /**< The device MAC address. */
static uint8_t                    m_random_vector_device_id[RANDOM_VECTOR_DEVICE_ID_SIZE];        /**< Device random ID. Used for NFC BLE pairng on iOS. */

#define NRF_BLE_MAX_MTU_SIZE            BLE_GATT_ATT_MTU_DEFAULT*12         /**< MTU size used in the softdevice enabling and to reply to a BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST event. */

#ifdef BLE_DFU_APP_SUPPORT
    static ble_dfu_t                  m_dfus;                                   /**< Structure used to identify the DFU service. */

    static void ble_dfu_evt_handler(ble_dfu_t * p_dfu, ble_dfu_evt_t * p_evt)
    {
        switch (p_evt->type)
        {
            case BLE_DFU_EVT_INDICATION_DISABLED:
                NRF_LOG_INFO("Indication for BLE_DFU is disabled\r\n");
                break;

            case BLE_DFU_EVT_INDICATION_ENABLED:
                NRF_LOG_INFO("Indication for BLE_DFU is enabled\r\n");
                break;

            case BLE_DFU_EVT_ENTERING_BOOTLOADER:
                NRF_LOG_INFO("Device is entering bootloader mode!\r\n");
                break;
            default:
                NRF_LOG_WARNING("Unknown event from ble_dfu\r\n");
                break;
        }
    }

#endif // BLE_DFU_APP_SUPPORT
#define CONN_CFG_TAG_THINGY 1


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

/**@brief Generate random number.
 */
static uint32_t random_vector_generate(uint8_t * p_buff, uint8_t size)
{
    uint32_t err_code;
    uint8_t  bytes_available = 0;

    nrf_drv_rng_bytes_available(&bytes_available);
    uint8_t retries = 0;
    
    while (bytes_available < size)
    {
        retries++;
        NRF_LOG_WARNING("Too few random bytes available. Trying again \r\n");
        nrf_drv_rng_bytes_available(&bytes_available);
        nrf_delay_ms(5);
        
        if (retries > 5)    // Return after n attempts.
        {
            return NRF_ERROR_TIMEOUT;
        }
    }
    
    NRF_LOG_INFO("Available random bytes: %d \r\n", bytes_available);

    err_code = nrf_drv_rng_rand(p_buff, size);
    RETURN_IF_ERROR(err_code);
    
    NRF_LOG_INFO("Random value (hex): ");
    
    for (uint8_t i = 0; i < size; i++)
    {
        NRF_LOG_RAW_INFO("%02x", p_buff[i]);
    }
    
    NRF_LOG_RAW_INFO("\r\n");

    return NRF_SUCCESS;
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

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&sec_mode);

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
        NRF_LOG_ERROR("on_conn_params_evt: BLE_CONN_PARAMS_EVT_FAILED\r\n");
        
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
    else if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_SUCCEEDED)
    {
        NRF_LOG_INFO("on_conn_params_evt: BLE_CONN_PARAMS_EVT_SUCCEEDED\r\n");
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
    NRF_LOG_ERROR("conn_params_error_handler: %d\r\n", nrf_error);
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
            NRF_LOG_INFO("on_adv_evt: BLE_ADV_EVT_FAST\r\n");
            break;
        case BLE_ADV_EVT_IDLE:
            NRF_LOG_INFO("on_adv_evt: BLE_ADV_EVT_IDLE\r\n");
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
            NRF_LOG_INFO("on_ble_evt: BLE_GAP_EVT_CONNECTED\r\n");
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

            evt.evt_type = thingy_ble_evt_connected;
            m_evt_handler(&evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("on_ble_evt: BLE_GAP_EVT_DISCONNECTED. Reason: 0x%x \r\n", p_ble_evt->evt.gap_evt.params.disconnected.reason);
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

        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
        {
            NRF_LOG_INFO("on_ble_evt: BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST - %d\r\n", p_ble_evt->evt.gatts_evt.params.exchange_mtu_request.client_rx_mtu);
            err_code = sd_ble_gatts_exchange_mtu_reply(p_ble_evt->evt.gatts_evt.conn_handle,
                                                       NRF_BLE_MAX_MTU_SIZE);
            APP_ERROR_CHECK(err_code);

            m_mtu.size = p_ble_evt->evt.gatts_evt.params.exchange_mtu_request.client_rx_mtu;
            err_code = ble_tcs_mtu_set(&m_tcs, &m_mtu);
            APP_ERROR_CHECK(err_code);
        }
        break; // BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST

        case BLE_GATTC_EVT_EXCHANGE_MTU_RSP:
            NRF_LOG_INFO("on_ble_evt: BLE_GATTC_EVT_EXCHANGE_MTU_RSP - %d\r\n", p_ble_evt->evt.gattc_evt.params.exchange_mtu_rsp.server_rx_mtu);
            break;

        case BLE_GAP_EVT_DATA_LENGTH_UPDATE_REQUEST:
            /* Allow SoftDevice to choose Data Length Update Procedure parameters
            automatically. */
            NRF_LOG_INFO("on_ble_evt: BLE_GAP_EVT_DATA_LENGTH_UPDATE_REQUEST\r\n");
            err_code = sd_ble_gap_data_length_update(p_ble_evt->evt.gap_evt.conn_handle, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;
        
        case BLE_GAP_EVT_DATA_LENGTH_UPDATE:
            NRF_LOG_INFO("on_ble_evt: BLE_GAP_EVT_DATA_LENGTH_UPDATE\r\n");
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
    uint32_t app_ram_start = 0;
    err_code = softdevice_app_ram_start_get(&app_ram_start);
    APP_ERROR_CHECK(err_code);
    
    ble_cfg_t ble_cfg;

    // Configure long MTU.
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.conn_cfg.conn_cfg_tag = CONN_CFG_TAG_THINGY;
    ble_cfg.conn_cfg.params.gatt_conn_cfg.att_mtu = 276;
    err_code = sd_ble_cfg_set(BLE_CONN_CFG_GATT, &ble_cfg, app_ram_start);
    APP_ERROR_CHECK(err_code);
    
    // Configure event length (7.5ms)
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.conn_cfg.conn_cfg_tag = CONN_CFG_TAG_THINGY;
    ble_cfg.conn_cfg.params.gap_conn_cfg.conn_count = 1;
    ble_cfg.conn_cfg.params.gap_conn_cfg.event_length = 6;
    err_code = sd_ble_cfg_set(BLE_CONN_CFG_GAP, &ble_cfg, app_ram_start);
    APP_ERROR_CHECK(err_code);
    
    // Configure HVN queue size
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.conn_cfg.conn_cfg_tag = CONN_CFG_TAG_THINGY;
    ble_cfg.conn_cfg.params.gatts_conn_cfg.hvn_tx_queue_size = 16;
    err_code = sd_ble_cfg_set(BLE_CONN_CFG_GATTS, &ble_cfg, app_ram_start);
    APP_ERROR_CHECK(err_code);
    
    // Configure WRITE_CMD queue size
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.conn_cfg.conn_cfg_tag = CONN_CFG_TAG_THINGY;
    ble_cfg.conn_cfg.params.gattc_conn_cfg.write_cmd_tx_queue_size = 2;
    err_code = sd_ble_cfg_set(BLE_CONN_CFG_GATTC, &ble_cfg, app_ram_start);
    APP_ERROR_CHECK(err_code);
    
    memset(&ble_cfg, 0, sizeof(ble_cfg));    
    ble_cfg.common_cfg.vs_uuid_cfg.vs_uuid_count = 2;
    err_code = sd_ble_cfg_set(BLE_COMMON_CFG_VS_UUID, &ble_cfg, app_ram_start);
    APP_ERROR_CHECK(err_code);

    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.gap_cfg.role_count_cfg.periph_role_count  = 1;
    err_code = sd_ble_cfg_set(BLE_GAP_CFG_ROLE_COUNT, &ble_cfg, app_ram_start);
    APP_ERROR_CHECK(err_code);
    
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.gatts_cfg.attr_tab_size.attr_tab_size = 3300; // GATT attribute table size. Default value 3300. 
    err_code = sd_ble_cfg_set(BLE_GATTS_CFG_ATTR_TAB_SIZE, &ble_cfg, app_ram_start);
    APP_ERROR_CHECK(err_code);
    
    memset(&ble_cfg, 0, sizeof(ble_cfg));
    ble_cfg.gatts_cfg.service_changed.service_changed = 1;
    err_code = sd_ble_cfg_set(BLE_GATTS_CFG_SERVICE_CHANGED, &ble_cfg, app_ram_start);
    APP_ERROR_CHECK(err_code);
    
    err_code = softdevice_enable(&app_ram_start);
    APP_ERROR_CHECK(err_code);
    
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
    ble_uuid_t    adv_uuids = {BLE_UUID_TCS_SERVICE, m_tcs.uuid_type};
    
    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));
    
    advdata.name_type                       = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance              = false;
    advdata.flags                           = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.uuids_more_available.uuid_cnt   = 1;
    advdata.uuids_more_available.p_uuids    = &adv_uuids;

    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = true;
    options.ble_adv_fast_interval = m_ble_config->adv_params.interval;
    options.ble_adv_fast_timeout  = m_ble_config->adv_params.timeout;
    
    // Build scan response data struct to pass into @ref ble_advertising_init.
    ble_advdata_t scan_response_data;
    memset(&scan_response_data, 0, sizeof(scan_response_data));

    // Manufacturer specific data in advertising packet.
    ble_advdata_manuf_data_t adv_manuf_data;
    
    uint8_t random_vector_device_id_reversed[RANDOM_VECTOR_DEVICE_ID_SIZE];
    
    for (uint8_t i = 0; i < RANDOM_VECTOR_DEVICE_ID_SIZE; i++)
    {
        random_vector_device_id_reversed[i] = m_random_vector_device_id[RANDOM_VECTOR_DEVICE_ID_SIZE - i - 1];
    }
    
    adv_manuf_data.company_identifier   = NORDIC_COMPANY_ID;
    adv_manuf_data.data.p_data          = random_vector_device_id_reversed;
    adv_manuf_data.data.size            = RANDOM_VECTOR_DEVICE_ID_SIZE;

    scan_response_data.name_type             = BLE_ADVDATA_NO_NAME; 
    scan_response_data.p_manuf_specific_data = &adv_manuf_data;
    
    // Set both advertisement data and scan response data.
    err_code = ble_advertising_init(&advdata, &scan_response_data, &options, on_adv_evt, NULL);
    RETURN_IF_ERROR(err_code);
    
    ble_advertising_conn_cfg_tag_set(CONN_CFG_TAG_THINGY);
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

    err_code = sd_ble_gap_addr_get(&beacon_init.beacon_addr);

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
                    NRF_LOG_INFO("tcs_evt_handler: TCS_MTU_REQ_EXCHANGE - %d\r\n", p_mtu->size);
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

        case BLE_TCS_EVT_NFC:
            if(length <= BLE_TCS_NFC_LEN_MAX)
            {
                memcpy(m_ble_config->nfc.data, p_data, length);
                m_ble_config->nfc.len = length;

                APP_ERROR_CHECK(drv_nfc_disable());
                APP_ERROR_CHECK(drv_nfc_raw_data_set(m_ble_config->nfc.data, length));

                update_flash = true;
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


/**@brief Checks the current version of the FW against the previous version stored in flash.
 * If a major or minor FW change is detected, modules must reinitialize their flash storage.
 *
 * @note: If the FW version is changed while erasing all flash, a FW change cannot be detected.
 */
static uint32_t thingy_config_verify(void)
{
    bool update_flash = false;
    uint32_t err_code;
    
    bool fw_version_major_changed = ( m_ble_config->fw_version.major != m_ble_default_config.fw_version.major );
    bool fw_version_minor_changed = ( m_ble_config->fw_version.minor != m_ble_default_config.fw_version.minor );
    bool fw_version_patch_changed = ( m_ble_config->fw_version.patch != m_ble_default_config.fw_version.patch );
    
    ble_tcs_fw_version_t prev_fw_version = m_ble_config->fw_version;

    if ( fw_version_major_changed || fw_version_minor_changed || fw_version_patch_changed)
    {
        m_ble_config->fw_version.major = m_ble_default_config.fw_version.major;
        m_ble_config->fw_version.minor = m_ble_default_config.fw_version.minor;
        m_ble_config->fw_version.patch = m_ble_default_config.fw_version.patch;
        
        update_flash = true;
        
        if(fw_version_major_changed || fw_version_minor_changed)
        {       
            update_flash = false;
            m_major_minor_fw_ver_changed = true;
            
            err_code = m_ble_flash_config_store(&m_ble_default_config);
            APP_ERROR_CHECK(err_code);
        }
    }
    
    NRF_LOG_INFO(NRF_LOG_COLOR_CODE_GREEN"m_ble: Current FW: v%d.%d.%d \r\n",
    m_ble_default_config.fw_version.major, m_ble_default_config.fw_version.minor, m_ble_default_config.fw_version.patch);
    
    if(m_major_minor_fw_ver_changed)
    {
        NRF_LOG_INFO(NRF_LOG_COLOR_CODE_YELLOW"m_ble: Major or minor FW version changed. Prev. FW (from flash): v%d.%d.%d \r\n",
        prev_fw_version.major, prev_fw_version.minor, prev_fw_version.patch);
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

    // Check NFC tag length.
    if (m_ble_config->nfc.len > BLE_TCS_NFC_LEN_MAX || m_ble_config->nfc.len == 0)
    {
        memcpy(m_ble_config->nfc.data, m_ble_default_config.nfc.data, m_ble_default_config.nfc.len);
        m_ble_config->nfc.len = m_ble_default_config.nfc.len;
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
            err_code = p_service_handles[i].init_cb(m_major_minor_fw_ver_changed);
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


uint32_t nfc_init(void)
{
    uint32_t err_code;

    err_code = drv_nfc_init();
    RETURN_IF_ERROR(err_code);

    if(memcmp(m_ble_config->nfc.data, m_ble_default_config.nfc.data, m_ble_default_config.nfc.len) == 0)
    {
        err_code = drv_nfc_app_launch_android_record_add(THINGY_NFC_APP_ANDROID_NAME_DEFAULT, THINGY_NFC_APP_ANDROID_NAME_LEN);
        RETURN_IF_ERROR(err_code);

        ble_tcs_nfc_t s_nfc_buf;
        uint8_t NFC_STR_LEN = SUPPORT_FUNC_MAC_ADDR_STR_LEN + (RANDOM_VECTOR_DEVICE_ID_SIZE * 2) + 1; // MAC (with '\0') + space + random vector.
        char nfc_string[NFC_STR_LEN];
        nfc_string[0] = '\0';

        strcat(nfc_string, m_mac_addr);
        strcat(nfc_string, " ");

        for (uint8_t i = 0; i < RANDOM_VECTOR_DEVICE_ID_SIZE; i++)
        {
            char tmp[3] = {0};
            sprintf(tmp, "%02x", m_random_vector_device_id[i]);
            strcat(nfc_string, tmp);
        }

        NRF_LOG_INFO("nfc string: %s \r\n", nrf_log_push(nfc_string));

        err_code = drv_nfc_string_record_add(nfc_string, NFC_STR_LEN);
        RETURN_IF_ERROR(err_code);

        err_code = drv_nfc_uri_record_add(NFC_URI_HTTP_WWW, THINGY_NFC_URI_DEFAULT, THINGY_NFC_URI_LEN);
        RETURN_IF_ERROR(err_code);

        err_code = drv_nfc_enable();
        RETURN_IF_ERROR(err_code);

        uint8_t len = BLE_TCS_NFC_LEN_MAX;
        err_code = drv_nfc_raw_data_get(s_nfc_buf.data, &len);
        RETURN_IF_ERROR(err_code);

        s_nfc_buf.len = len;
        err_code = ble_tcs_nfc_set(&m_tcs, &s_nfc_buf);
        RETURN_IF_ERROR(err_code);
    }
    else
    {
        err_code = drv_nfc_raw_data_set(m_ble_config->nfc.data, m_ble_config->nfc.len);
        RETURN_IF_ERROR(err_code);
    }

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
        NRF_LOG_ERROR("ble_stack_init failed - %d\r\n", err_code);
        return err_code;
    }

        /* Enable FPU again due to SD issue */
    #if (__FPU_USED == 1)
        SCB->CPACR |= (3UL << 20) | (3UL << 22);
        __DSB();
        __ISB();
    #endif
    
    err_code = random_vector_generate(m_random_vector_device_id, RANDOM_VECTOR_DEVICE_ID_SIZE);

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("random_vector_generate failed - %d\r\n", err_code);
        return err_code;
    }
    
    /**@brief Load configuration from flash. */
    err_code = m_ble_flash_init(&m_ble_default_config, &m_ble_config);

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR(" m_ble_flash_init failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = thingy_config_verify();

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Thingy_config_verify failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = gap_params_init();

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("gap_params_init failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = services_init(m_service_handles, m_service_num);

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Thingy_ble_init: services_init failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = advertising_init();

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Advertising_init failed - %d\r\n", err_code);
        return err_code;
    }

    err_code = conn_params_init();

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Conn_params_init failed - %d\r\n", err_code);
        return err_code;
    }
    
    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("ble_advertising_start failed - %d\r\n", err_code);
        return err_code;
    }
    
    err_code = support_func_ble_mac_address_get(m_mac_addr);

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("mac address get failed - %d\r\n", err_code);
        return err_code;
    }
    
    NRF_LOG_RAW_INFO("MAC addr-> %s \r\n", nrf_log_push(m_mac_addr));
    
    err_code = nfc_init();

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("nfc init failed - %d\r\n", err_code);
        return err_code;
    }
    
    nrf_delay_ms (10);

    err_code = timeslot_init();

    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("timeslot_init failed - %d\r\n", err_code);
        return err_code;
    }

    return NRF_SUCCESS;
}
