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

#include "advertiser_beacon.h"
#include <stdio.h>
#include <string.h>

#include "nrf_soc.h"
#include "app_error.h"
#include "app_util.h"


#ifdef ABT_DEBUG_PRINT
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

#define ADV_PACK_LENGTH_IDX     1
#define ADV_DATA_LENGTH_IDX    16
#define ADV_HEADER_LEN          3
#define ADV_TYPE_LEN            2
#define APP_DEVICE_TYPE      0x02       /**< 0x02 refers to Beacon. */
#define ADV_CHANNEL_37         37
#define ADV_CHANNEL_38         38
#define ADV_CHANNEL_39         39
#define FREQ_ADV_CHANNEL_37     2
#define FREQ_ADV_CHANNEL_38    26
#define FREQ_ADV_CHANNEL_39    80
#define BEACON_SLOT_LENGTH     5500

static struct
{
    uint32_t                adv_interval;                       /** Advertising interval in milliseconds to be used for 'beacon' advertisements. */
    bool                    keep_running;                       /** */
    bool                    is_running;                         /** is the 'beacon' running*/
    uint32_t                slot_length;                        /** */
    nrf_radio_request_t     timeslot_request;                   /** */
    ble_gap_addr_t          beacon_addr;                        /** ble address to be used by the beacon. */
    ble_srv_error_handler_t error_handler;                      /** Function to be called in case of an error. */
    uint8_t               * p_data;
    uint16_t                data_size;
} m_beacon;

enum mode_t
{
  ADV_INIT,                                                 /** Initialisation. */
  ADV_RX_CH37,                                              /** Advertising on Rx channel 37. */
  ADV_RX_CH38,                                              /** Advertising on Rx channel 38. */
  ADV_RX_CH39,                                              /** Advertising on Rx channel 39. */
  ADV_DONE                                                  /** Done advertising. */
};


nrf_radio_request_t * m_configure_next_event(void)
{
    m_beacon.timeslot_request.request_type              = NRF_RADIO_REQ_TYPE_NORMAL;
    m_beacon.timeslot_request.params.normal.hfclk       = NRF_RADIO_HFCLK_CFG_XTAL_GUARANTEED;
    m_beacon.timeslot_request.params.normal.priority    = NRF_RADIO_PRIORITY_HIGH;
    m_beacon.timeslot_request.params.normal.distance_us = m_beacon.adv_interval * 1000;
    m_beacon.timeslot_request.params.normal.length_us   = m_beacon.slot_length;
    return &m_beacon.timeslot_request;
}


uint32_t m_request_earliest(enum NRF_RADIO_PRIORITY priority)
{
    m_beacon.timeslot_request.request_type                = NRF_RADIO_REQ_TYPE_EARLIEST;
    m_beacon.timeslot_request.params.earliest.hfclk       = NRF_RADIO_HFCLK_CFG_XTAL_GUARANTEED;
    m_beacon.timeslot_request.params.earliest.priority    = priority;
    m_beacon.timeslot_request.params.earliest.length_us   = m_beacon.slot_length;
    m_beacon.timeslot_request.params.earliest.timeout_us  = 1000000;
    return sd_radio_request(&m_beacon.timeslot_request);
}


static uint8_t * m_get_adv_packet(void)
{
    static uint8_t adv_pdu[40];
    uint8_t packet_len_start_idx, service_data_len_idx;
    uint8_t offset    = 0;

    // Constructing header
    adv_pdu[offset]    = BLE_GAP_ADV_TYPE_ADV_SCAN_IND;    // Advertisement type ADV_NONCONN_IND.
    adv_pdu[offset++] |= 1 << 6;                           // TxAdd 1 (random address).
    adv_pdu[offset++]  = 0;                                // Packet length field (will be filled later).
    packet_len_start_idx = offset;
    adv_pdu[offset++]  = 0x00;                             // Extra byte used to map into the radio register.

    // Constructing base advertising packet.
    memcpy(&adv_pdu[offset], m_beacon.beacon_addr.addr, BLE_GAP_ADDR_LEN);
    offset += BLE_GAP_ADDR_LEN;

    // Adding advertising data: Flags.
    adv_pdu[offset++] =  ADV_TYPE_LEN;
    adv_pdu[offset++] =  BLE_GAP_AD_TYPE_FLAGS;
    adv_pdu[offset++] =  BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    // Adding eddystone url UUID.
    adv_pdu[offset++] = 0x03;
    adv_pdu[offset++] = BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE;
    adv_pdu[offset++] = 0xAA;
    adv_pdu[offset++] = 0xFE;

    // Adding eddystone service data.
    adv_pdu[offset++] = 0; // Service data length field(filled in later).
    service_data_len_idx = offset;
    adv_pdu[offset++] = BLE_GAP_AD_TYPE_SERVICE_DATA;
    adv_pdu[offset++] = 0xAA;
    adv_pdu[offset++] = 0xFE;
    adv_pdu[offset++] = 0x10;
    adv_pdu[offset++] = 0xEE;

    // Adding URL.
    memcpy(&adv_pdu[offset], m_beacon.p_data, m_beacon.data_size);
    offset += m_beacon.data_size;

    // Filling in length fields.
    adv_pdu[ADV_PACK_LENGTH_IDX]         = offset - packet_len_start_idx;
    adv_pdu[ADV_DATA_LENGTH_IDX]         = offset - service_data_len_idx;

    return &adv_pdu[0];
}


static void m_set_adv_ch(uint32_t channel)
{
    if (channel == ADV_CHANNEL_37)
    {
        NRF_RADIO->FREQUENCY    = FREQ_ADV_CHANNEL_37;
        NRF_RADIO->DATAWHITEIV  = ADV_CHANNEL_37;
    }
    if (channel == ADV_CHANNEL_38)
    {
        NRF_RADIO->FREQUENCY    = FREQ_ADV_CHANNEL_38;
        NRF_RADIO->DATAWHITEIV  = ADV_CHANNEL_38;
    }
    if (channel == ADV_CHANNEL_39)
    {
        NRF_RADIO->FREQUENCY    = FREQ_ADV_CHANNEL_39;
        NRF_RADIO->DATAWHITEIV  = ADV_CHANNEL_39;
    }
}


static void m_configure_radio()
{
    uint8_t * p_adv_pdu = m_get_adv_packet();

    NRF_RADIO->POWER        = 1;
    NRF_RADIO->PCNF0        =   (((1UL) << RADIO_PCNF0_S0LEN_Pos                               ) & RADIO_PCNF0_S0LEN_Msk)
                              | (((2UL) << RADIO_PCNF0_S1LEN_Pos                               ) & RADIO_PCNF0_S1LEN_Msk)
                              | (((6UL) << RADIO_PCNF0_LFLEN_Pos                               ) & RADIO_PCNF0_LFLEN_Msk);
    NRF_RADIO->PCNF1        =   (((RADIO_PCNF1_ENDIAN_Little)     << RADIO_PCNF1_ENDIAN_Pos    ) & RADIO_PCNF1_ENDIAN_Msk)
                              | (((3UL)                           << RADIO_PCNF1_BALEN_Pos     ) & RADIO_PCNF1_BALEN_Msk)
                              | (((0UL)                           << RADIO_PCNF1_STATLEN_Pos   ) & RADIO_PCNF1_STATLEN_Msk)
                              | ((((uint32_t) 37)                 << RADIO_PCNF1_MAXLEN_Pos    ) & RADIO_PCNF1_MAXLEN_Msk)
                              | ((RADIO_PCNF1_WHITEEN_Enabled     << RADIO_PCNF1_WHITEEN_Pos   ) & RADIO_PCNF1_WHITEEN_Msk);
    NRF_RADIO->CRCCNF       =   (((RADIO_CRCCNF_SKIPADDR_Skip)    << RADIO_CRCCNF_SKIPADDR_Pos ) & RADIO_CRCCNF_SKIPADDR_Msk)
                              | (((RADIO_CRCCNF_LEN_Three)        << RADIO_CRCCNF_LEN_Pos      ) & RADIO_CRCCNF_LEN_Msk);
    NRF_RADIO->CRCPOLY      = 0x0000065b;
    NRF_RADIO->RXADDRESSES  = ((RADIO_RXADDRESSES_ADDR0_Enabled)  << RADIO_RXADDRESSES_ADDR0_Pos);
    NRF_RADIO->SHORTS       = ((1 << RADIO_SHORTS_READY_START_Pos) | (1 << RADIO_SHORTS_END_DISABLE_Pos));
    NRF_RADIO->MODE         = ((RADIO_MODE_MODE_Ble_1Mbit)        << RADIO_MODE_MODE_Pos       ) & RADIO_MODE_MODE_Msk;
    NRF_RADIO->TIFS         = 150;
    NRF_RADIO->INTENSET     = (1 << RADIO_INTENSET_DISABLED_Pos);
    NRF_RADIO->PREFIX0      = 0x0000008e; //access_addr[3].
    NRF_RADIO->BASE0        = 0x89bed600; //access_addr[0:3].
    NRF_RADIO->CRCINIT      = 0x00555555;
    NRF_RADIO->PACKETPTR    = (uint32_t) p_adv_pdu;

    NVIC_EnableIRQ(RADIO_IRQn);
}


void m_handle_start(void)
{
    // Configure TX_EN on TIMER EVENT_0.
    NRF_PPI->CH[8].TEP    = (uint32_t)(&NRF_RADIO->TASKS_TXEN);
    NRF_PPI->CH[8].EEP    = (uint32_t)(&NRF_TIMER0->EVENTS_COMPARE[0]);
    NRF_PPI->CHENSET      = (1 << 8);

    // Configure and initiate radio.
    m_configure_radio();
    NRF_RADIO->TASKS_DISABLE = 1;
}


void m_handle_radio_disabled(enum mode_t mode)
{
    switch (mode)
    {
        case ADV_RX_CH37:
            m_set_adv_ch(ADV_CHANNEL_37);
            NRF_RADIO->TASKS_TXEN = 1;
            break;
        case ADV_RX_CH38:
            m_set_adv_ch(ADV_CHANNEL_38);
            NRF_TIMER0->TASKS_CLEAR = 1;
            NRF_TIMER0->CC[0]       = 400;
            break;
        case ADV_RX_CH39:
            m_set_adv_ch(ADV_CHANNEL_39);
            NRF_TIMER0->TASKS_CLEAR = 1;
            NRF_TIMER0->CC[0]       = 400;
            break;
        default:
            break;
    }
}


static nrf_radio_signal_callback_return_param_t * m_timeslot_callback(uint8_t signal_type)
{
  static nrf_radio_signal_callback_return_param_t signal_callback_return_param;
  static enum mode_t mode;

  signal_callback_return_param.params.request.p_next  = NULL;
  signal_callback_return_param.callback_action        = NRF_RADIO_SIGNAL_CALLBACK_ACTION_NONE;

  switch (signal_type)
  {
    case NRF_RADIO_CALLBACK_SIGNAL_TYPE_START:

      m_handle_start();

      mode = ADV_INIT;
      mode++;
      break;
    case NRF_RADIO_CALLBACK_SIGNAL_TYPE_RADIO:
      if (NRF_RADIO->EVENTS_DISABLED == 1)
      {
        NRF_RADIO->EVENTS_DISABLED = 0;

        m_handle_radio_disabled(mode);

        if (mode == ADV_DONE)
        {
            DEBUG_PRINTF(0, "app_beacon_adv_done:\r\n");
            NRF_PPI->CHENCLR = (1 << 8);
            if (m_beacon.keep_running)
            {
                signal_callback_return_param.params.request.p_next = m_configure_next_event();
                signal_callback_return_param.callback_action       = NRF_RADIO_SIGNAL_CALLBACK_ACTION_REQUEST_AND_END;
            }
            else
            {
                signal_callback_return_param.callback_action       = NRF_RADIO_SIGNAL_CALLBACK_ACTION_END;
            }
            break;
        }
        mode++;
      }
      break;
    default:
        if (m_beacon.error_handler != NULL)
        {
            m_beacon.error_handler(NRF_ERROR_INVALID_STATE);
        }
      break;
  }

  return ( &signal_callback_return_param );
}


void app_beacon_on_sys_evt(uint32_t event)
{
    uint32_t err_code;

    DEBUG_PRINTF(0, "app_beacon_on_sys_evt: %d\r\n", event);

    switch (event)
    {
        case NRF_EVT_RADIO_SESSION_IDLE:
            if (m_beacon.is_running)
            {
                m_beacon.is_running = false;
                err_code = sd_radio_session_close();
                if ((err_code != NRF_SUCCESS) && (m_beacon.error_handler != NULL))
                {
                    m_beacon.error_handler(err_code);
                }
            }
            break;
        case NRF_EVT_RADIO_SESSION_CLOSED:
            break;
        case NRF_EVT_RADIO_BLOCKED:
        case NRF_EVT_RADIO_CANCELED: // Fall through.
            if (m_beacon.keep_running)
            {
                // A proper solution should try again in <block_count> * m_beacon.adv_interval
                err_code = m_request_earliest(NRF_RADIO_PRIORITY_HIGH);
                if ((err_code != NRF_SUCCESS) && (m_beacon.error_handler != NULL))
                {
                    m_beacon.error_handler(err_code);
                }
            }
            break;
        default:
            break;
    }
}


void app_beacon_init(ble_beacon_init_t * p_init)
{
    DEBUG_PRINTF(0, "app_beacon_init:\r\n");
    m_beacon.adv_interval  = p_init->adv_interval;
    m_beacon.slot_length   = BEACON_SLOT_LENGTH;
    m_beacon.beacon_addr   = p_init->beacon_addr;
    m_beacon.error_handler = p_init->error_handler;
    m_beacon.data_size     = p_init->data_size;
    m_beacon.p_data        = p_init->p_data;
}


void app_beacon_start(void)
{
    if (m_beacon.is_running || m_beacon.keep_running)
    {
        return;
    }

    DEBUG_PRINTF(0, "app_beacon_start:\r\n");
    m_beacon.keep_running = true;
    m_beacon.is_running   = true;

    uint32_t err_code = sd_radio_session_open(m_timeslot_callback);
    if ((err_code != NRF_SUCCESS) && (m_beacon.error_handler != NULL))
    {
        m_beacon.error_handler(err_code);
    }

    err_code = m_request_earliest(NRF_RADIO_PRIORITY_NORMAL);
    if ((err_code != NRF_SUCCESS) && (m_beacon.error_handler != NULL))
    {
        m_beacon.error_handler(err_code);
    }
}


void app_beacon_stop(void)
{
    DEBUG_PRINTF(0, "app_beacon_stop:\r\n");
    m_beacon.keep_running = false;
}
