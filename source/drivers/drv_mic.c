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

#include "drv_audio.h"
#include "drv_audio_anr.h"
#include "drv_audio_dsp.h"
#include "drv_audio_coder.h"
#include "app_scheduler.h"
#include "app_error.h"
#include "drv_ext_gpio.h"
#include "drv_mic.h"
#include "app_util_platform.h"

STATIC_ASSERT(CONFIG_PDM_BUFFER_SIZE_SAMPLES == (1 * CONFIG_AUDIO_FRAME_SIZE_SAMPLES));

#ifdef DRV_MIC_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

typedef struct
{
    int16_t  buf[CONFIG_PDM_BUFFER_SIZE_SAMPLES];
    uint16_t samples;
    bool     free;
}pdm_buf_t;

#define PDM_BUF_NUM 6

static bool                   m_audio_enabled;          ///< Audio enabled flag.
static drv_mic_data_handler_t m_data_handler;
static pdm_buf_t              m_pdm_buf[PDM_BUF_NUM];

static void mic_power_on(void)
{
    uint32_t err_code;

    #if defined(THINGY_HW_v0_7_0)
        err_code = drv_ext_gpio_pin_clear(SX_MIC_PWR_CTRL);
    #elif defined(THINGY_HW_v0_8_0)
        err_code = drv_ext_gpio_pin_clear(SX_MIC_PWR_CTRL);
    #elif defined(THINGY_HW_v0_9_0)
        err_code = drv_ext_gpio_pin_clear(SX_MIC_PWR_CTRL);
    #else
        err_code = drv_ext_gpio_pin_set(SX_MIC_PWR_CTRL);
    #endif
    APP_ERROR_CHECK(err_code);
}


static void mic_power_off(void)
{
    uint32_t err_code;

    #if defined(THINGY_HW_v0_7_0)
        err_code = drv_ext_gpio_pin_set(SX_MIC_PWR_CTRL);
    #elif defined(THINGY_HW_v0_8_0)
        err_code = drv_ext_gpio_pin_set(SX_MIC_PWR_CTRL);
    #elif defined(THINGY_HW_v0_9_0)
        err_code = drv_ext_gpio_pin_set(SX_MIC_PWR_CTRL);
    #else
        err_code = drv_ext_gpio_pin_clear(SX_MIC_PWR_CTRL);
    #endif
    APP_ERROR_CHECK(err_code);
}


static void m_audio_process(void * p_event_data, uint16_t event_size)
{
    int16_t         * p_buffer;
    ret_code_t        status;
    m_audio_frame_t   frame_buf;
    pdm_buf_t       * p_pdm_buf = (pdm_buf_t *)(*(uint32_t *)p_event_data);

    APP_ERROR_CHECK_BOOL(p_event_data != NULL);
    APP_ERROR_CHECK_BOOL(event_size > 0);
    p_buffer = p_pdm_buf->buf;

#if CONFIG_AUDIO_EQUALIZER_ENABLED
    drv_audio_dsp_equalizer((q15_t *)p_buffer, CONFIG_AUDIO_FRAME_SIZE_SAMPLES);
#endif /* CONFIG_AUDIO_EQUALIZER_ENABLED */

#if CONFIG_AUDIO_GAIN_CONTROL_ENABLED
    drv_audio_dsp_gain_control((q15_t *)p_buffer, CONFIG_AUDIO_FRAME_SIZE_SAMPLES);
#endif /* CONFIG_AUDIO_GAIN_CONTROL_ENABLED */

    uint8_t nested;
    app_util_critical_region_enter(&nested);
    drv_audio_coder_encode(p_buffer, &frame_buf);
    p_pdm_buf->free = true;
    app_util_critical_region_exit(nested);

    // Schedule audio transmission. It cannot be done from this context.
    status = m_data_handler(&frame_buf);
    if (status != NRF_SUCCESS)
    {
        DEBUG_PRINTF(0, "%s(): WARNING: Cannot schedule audio frame transmission!\r\n", __func__);
        /*
         * Do not clear CONFIG_IO_DBG_PCM. This will make debugging pulse wider
         * than expected and easier to spot on the logic analyzer.
         */
    }
}


static void m_audio_buffer_handler(int16_t *p_buffer, uint16_t samples)
{
    uint32_t     err_code;
    pdm_buf_t  * p_pdm_buf = NULL;
    uint32_t     pdm_buf_addr;

    for(uint32_t i = 0; i < PDM_BUF_NUM; i++)
    {
        if ( m_pdm_buf[i].free == true )
        {
            m_pdm_buf[i].free    = false;
            m_pdm_buf[i].samples = samples;

            for (uint32_t j = 0; j < samples; j++)
            {
                m_pdm_buf[i].buf[j] = p_buffer[j];
            }

            p_pdm_buf = &m_pdm_buf[i];
            pdm_buf_addr = (uint32_t)&m_pdm_buf[i];

            break;
        }
    }

    if (p_pdm_buf != NULL)
    {
        err_code = app_sched_event_put(&pdm_buf_addr, sizeof(pdm_buf_t *), m_audio_process);
        APP_ERROR_CHECK(err_code);
    }
    else
    {
        DEBUG_PRINTF(0, "m_audio_buffer_handler: BUFFER FULL!!\r\n");
    }
}


uint32_t drv_mic_start(void)
{
    ret_code_t status;

    DEBUG_PRINTF(0, "m_audio: Enabled\r\n");

    if(m_audio_enabled == true)
    {
        return NRF_SUCCESS;
    }

    mic_power_on();

    status = drv_audio_enable();
    if (status == NRF_SUCCESS)
    {
        m_audio_enabled = true;
    }

    return status;
}


uint32_t drv_mic_stop(void)
{
    ret_code_t status;

    DEBUG_PRINTF(0, "m_audio: Disabled\r\n");

    if(m_audio_enabled == false)
    {
        return NRF_SUCCESS;
    }

    status = drv_audio_disable();
    if (status == NRF_SUCCESS)
    {
        m_audio_enabled = false;
    }

    mic_power_off();

    return status;
}


uint32_t drv_mic_init(drv_mic_data_handler_t data_handler)
{
    uint32_t err_code;

    m_audio_enabled = false;
    m_data_handler  = data_handler;

    for(uint32_t i = 0; i < PDM_BUF_NUM; i++)
    {
        m_pdm_buf[i].free = true;
    }

    err_code = drv_ext_gpio_cfg_output(SX_MIC_PWR_CTRL);
    RETURN_IF_ERROR(err_code);

    mic_power_off();

    return drv_audio_init(m_audio_buffer_handler);
}
