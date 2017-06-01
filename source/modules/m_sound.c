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

#include <string.h>
#include "app_util_platform.h"
#include "m_sound.h"
#include "drv_speaker.h"
#include "drv_mic.h"
#include "ble_tss.h"

#ifdef SOUND_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

static ble_tss_t              m_tss;  /**< Structure to identify the Thingy Sound Service. */

static void drv_speaker_evt_handler(drv_speaker_evt_t evt)
{
    switch(evt)
    {
        case DRV_SPEAKER_EVT_FINISHED:
        {
            DEBUG_PRINTF(0, "drv_speaker_evt_handler: DRV_SPEAKER_EVT_FINISHED\r\n");
            (void)ble_tss_spkr_stat_set(&m_tss, BLE_TSS_SPKR_STAT_FINISHED);
        }
        break;
        //
        case DRV_SPEAKER_EVT_BUFFER_WARNING:
        {
            DEBUG_PRINTF(0, "drv_speaker_evt_handler: DRV_SPEAKER_EVT_BUFFER_WARNING\r\n");
            (void)ble_tss_spkr_stat_set(&m_tss, BLE_TSS_SPKR_STAT_BUFFER_WARNING);
        }
        break;
        //
        case DRV_SPEAKER_EVT_BUFFER_READY:
        {
            DEBUG_PRINTF(0, "drv_speaker_evt_handler: DRV_SPEAKER_EVT_BUFFER_READY\r\n");
            (void)ble_tss_spkr_stat_set(&m_tss, BLE_TSS_SPKR_STAT_BUFFER_READY);
        }
        break;
        //
        default:
            APP_ERROR_CHECK_BOOL(false);
            break;
    }
}


static uint32_t drv_mic_data_handler(m_audio_frame_t * p_frame)
{
    uint32_t err_code;

    if (p_frame->data_size != CONFIG_AUDIO_FRAME_SIZE_BYTES)
    {
        DEBUG_PRINTF(0, "drv_mic_data_handler: size = %d", p_frame->data_size);
    }

    err_code = ble_tss_mic_set(&m_tss, p_frame->data, p_frame->data_size);
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "drv_mic_data_handler: Failed to send\r\n");
    }

    return NRF_SUCCESS;
}

/**@brief Function for handling event from the Thingy Sound Service.
 *
 * @details This function will process the data received from the Thingy Sound BLE Service.
 *
 * @param[in] p_tes    Thingy Sound Service structure.
 * @param[in] evt_type Thingy Sound Service event type.
 * @param[in] p_data   Event data.
 * @param[in] length   Length of the data.
 */
static void ble_tss_evt_handler( ble_tss_t        * p_tes,
                                 ble_tss_evt_type_t evt_type,
                                 uint8_t          * p_data,
                                 uint16_t           length)
{
    uint32_t err_code;
    static ble_tss_spkr_mode_t mode = BLE_TSS_SPKR_MODE_FREQ;

    switch (evt_type)
    {
        case BLE_TSS_EVT_SPKR_RECEIVED:
            if (mode == BLE_TSS_SPKR_MODE_PCM)
            {
                err_code = drv_speaker_ble_pcm_play(p_data, length);
                if (err_code == NRF_ERROR_NO_MEM)
                {
                    (void)ble_tss_spkr_stat_set(&m_tss, BLE_TSS_SPKR_STAT_PACKET_DISREGARDED);
                }
                else
                {
                    APP_ERROR_CHECK(err_code);
                }
            }
            else if (mode == BLE_TSS_SPKR_MODE_FREQ)
            {
                ble_tss_spkr_freq_t * p_spkr_freq = (ble_tss_spkr_freq_t *)p_data;

                if (length == sizeof(ble_tss_spkr_freq_t))
                {
                    if (drv_speaker_tone_start(p_spkr_freq->freq, p_spkr_freq->duration_ms, p_spkr_freq->volume) != NRF_SUCCESS)
                    {
                        (void)ble_tss_spkr_stat_set(&m_tss, BLE_TSS_SPKR_STAT_INVALID_COMMAND);
                    }
                }
                else
                {
                    (void)ble_tss_spkr_stat_set(&m_tss, BLE_TSS_SPKR_STAT_INVALID_COMMAND);
                }
            }
            else if (mode == BLE_TSS_SPKR_MODE_SAMPLE)
            {
                ble_tss_spkr_t * p_spkr = (ble_tss_spkr_t *)p_data;

                if (length == sizeof(ble_tss_spkr_sample_t))
                {
                    err_code = drv_speaker_sample_play(p_spkr->sample_id);
                    if (err_code != NRF_SUCCESS)
                    {
                        (void)ble_tss_spkr_stat_set(&m_tss, BLE_TSS_SPKR_STAT_INVALID_COMMAND);
                    }
                }
                else
                {
                    (void)ble_tss_spkr_stat_set(&m_tss, BLE_TSS_SPKR_STAT_INVALID_COMMAND);
                }
            }
            else
            {
                (void)ble_tss_spkr_stat_set(&m_tss, BLE_TSS_SPKR_STAT_INVALID_COMMAND);
            }
            break;

        case BLE_TSS_EVT_NOTIF_MIC:
            DEBUG_PRINTF(0, "tss_evt_handler: BLE_TES_EVT_NOTIF_MIC - %d\r\n", p_tes->is_mic_notif_enabled);
            if (p_tes->is_mic_notif_enabled)
            {
                err_code = drv_mic_start();
                APP_ERROR_CHECK(err_code);
            }
            else
            {
                err_code = drv_mic_stop();
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BLE_TSS_EVT_CONFIG_RECEIVED:
        {
            APP_ERROR_CHECK_BOOL(length == sizeof(ble_tss_config_t));

            ble_tss_config_t * p_config = (ble_tss_config_t *)p_data;
            DEBUG_PRINTF(0, "tes_evt_handler: BLE_TES_EVT_CONFIG_RECEIVED - %d\r\n", p_config->spkr_mode);
            mode = p_config->spkr_mode;
        }
        break;

        default:
            break;

    }
}


/**@brief Function for initializing the Thingy Sound Service.
 *
 * @details This callback function will be called from the ble handling module to initialize the Thingy Sound service.
 *
 * @retval NRF_SUCCESS If initialization was successful.
 */
static uint32_t sound_service_init(void)
{
    uint32_t              err_code;
    ble_tss_init_t        tss_init;
    ble_tss_config_t      init_cfg;

    memset(&tss_init, 0, sizeof(tss_init));

    init_cfg.mic_mode  = BLE_TSS_MIC_MODE_ADPCM;
    init_cfg.spkr_mode = BLE_TSS_SPKR_MODE_FREQ;
    tss_init.p_init_config = &init_cfg;
    tss_init.evt_handler   = ble_tss_evt_handler;

    DEBUG_PRINTF(0, "sound_service_init: ble_tss_init ");
    err_code = ble_tss_init(&m_tss, &tss_init);
    if (err_code != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "FAILED - %d\r\n", err_code);
        return err_code;
    }

    DEBUG_PRINTF(0, "\r\n");

    return NRF_SUCCESS;
}


/**@brief Function for passing the BLE event to the Thingy Sound service.
 *
 * @details This callback function will be called from the BLE handling module.
 *
 * @param[in] p_ble_evt    Pointer to the BLE event.
 */
static void sound_on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code;
    ble_tss_on_ble_evt(&m_tss, p_ble_evt);

    if (p_ble_evt->header.evt_id == BLE_GAP_EVT_DISCONNECTED)
    {
        err_code = drv_mic_stop();
        APP_ERROR_CHECK(err_code);
    }
}


uint32_t m_sound_init(m_ble_service_handle_t * p_handle)
{
    uint32_t           err_code;
    drv_speaker_init_t speaker_init;

    NULL_PARAM_CHECK(p_handle);

    DEBUG_PRINTF(0, "sound_init: \r\n");

    p_handle->ble_evt_cb = sound_on_ble_evt;
    p_handle->init_cb    = sound_service_init;

    speaker_init.evt_handler = drv_speaker_evt_handler;
    err_code = drv_speaker_init(&speaker_init);
    APP_ERROR_CHECK(err_code);

    return drv_mic_init(drv_mic_data_handler);
}
