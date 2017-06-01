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
#include <stdbool.h>
#include <stdlib.h>

#include "nrf_assert.h"
#include "nrf_error.h"
#include "nrf_gpio.h"
#include "nrf_drv_pdm.h"
//#include "app_debug.h"

#include "drv_audio.h"

#if CONFIG_AUDIO_ENABLED

// Verify SDK configuration.
STATIC_ASSERT(PDM_ENABLED);

// Check pin configuration.
STATIC_ASSERT(IS_IO_VALID(CONFIG_IO_PDM_CLK));
STATIC_ASSERT(IS_IO_VALID(CONFIG_IO_PDM_DATA));
#if CONFIG_PDM_MIC_PWR_CTRL_ENABLED
STATIC_ASSERT(IS_IO_VALID(CONFIG_IO_PDM_MIC_PWR_CTRL));
#endif

// Sampling rate depends on PDM configuration.
STATIC_ASSERT(PDM_CONFIG_CLOCK_FREQ == NRF_PDM_FREQ_1032K);
#define SAMPLING_RATE   (1032 * 1000 / 64)

static int16_t                      m_pdm_buff[2][CONFIG_PDM_BUFFER_SIZE_SAMPLES];
static drv_audio_buffer_handler_t   m_buffer_handler;
static uint8_t                      m_skip_buffers;

ret_code_t drv_audio_enable(void)
{
#if CONFIG_PDM_MIC_PWR_CTRL_ENABLED
#if CONFIG_PDM_MIC_PWR_CTRL_ACT_LOW
    nrf_gpio_pin_clear(CONFIG_IO_PDM_MIC_PWR_CTRL);
#else /* !CONFIG_PDM_MIC_PWR_CTRL_ACT_LOW */
    nrf_gpio_pin_set(CONFIG_IO_PDM_MIC_PWR_CTRL);
#endif /* CONFIG_PDM_MIC_PWR_CTRL_ACT_LOW */
#endif /* CONFIG_PDM_MIC_PWR_CTRL_ENABLED */

    // Skip buffers with invalid data.
    m_skip_buffers = MAX(1, ROUNDED_DIV((CONFIG_PDM_TRANSIENT_STATE_LEN * SAMPLING_RATE),
                                        (1000 * CONFIG_AUDIO_FRAME_SIZE_SAMPLES)));

    return nrf_drv_pdm_start();
}

ret_code_t drv_audio_disable(void)
{
#if CONFIG_PDM_MIC_PWR_CTRL_ENABLED
#if CONFIG_PDM_MIC_PWR_CTRL_ACT_LOW
    nrf_gpio_pin_set(CONFIG_IO_PDM_MIC_PWR_CTRL);
#else /* !CONFIG_PDM_MIC_PWR_CTRL_ACT_LOW */
    nrf_gpio_pin_clear(CONFIG_IO_PDM_MIC_PWR_CTRL);
#endif /* CONFIG_PDM_MIC_PWR_CTRL_ACT_LOW */
#endif /* CONFIG_PDM_MIC_PWR_CTRL_ENABLED */

    return nrf_drv_pdm_stop();
}

static void drv_audio_pdm_event_handler(uint32_t *p_buffer, uint16_t length)
{
    ASSERT(length == CONFIG_PDM_BUFFER_SIZE_SAMPLES);

    if (m_skip_buffers)
    {
        m_skip_buffers -= 1;
    }
    else
    {
        m_buffer_handler((int16_t *)p_buffer, length);
    }
}

ret_code_t drv_audio_init(drv_audio_buffer_handler_t buffer_handler)
{
    nrf_drv_pdm_config_t pdm_cfg = NRF_DRV_PDM_DEFAULT_CONFIG(CONFIG_IO_PDM_CLK,
                                                              CONFIG_IO_PDM_DATA,
                                                              m_pdm_buff[0],
                                                              m_pdm_buff[1],
                                                              CONFIG_PDM_BUFFER_SIZE_SAMPLES);
    if (buffer_handler == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    m_buffer_handler    = buffer_handler;
    pdm_cfg.gain_l      = CONFIG_PDM_GAIN;
    pdm_cfg.gain_r      = CONFIG_PDM_GAIN;

    pdm_cfg.mode        = NRF_PDM_MODE_MONO;

#if   (CONFIG_PDM_MIC == CONFIG_PDM_MIC_LEFT)
    pdm_cfg.edge        = NRF_PDM_EDGE_LEFTFALLING;
#elif (CONFIG_PDM_MIC == CONFIG_PDM_MIC_RIGHT)
    pdm_cfg.edge        = NRF_PDM_EDGE_LEFTRISING;
#else
#error "Value of CONFIG_PDM_MIC is not valid!"
#endif /* (CONFIG_PDM_MIC == CONFIG_PDM_MIC_LEFT) */

#if CONFIG_PDM_MIC_PWR_CTRL_ENABLED
#if CONFIG_PDM_MIC_PWR_CTRL_ACT_LOW
    nrf_gpio_pin_set(CONFIG_IO_PDM_MIC_PWR_CTRL);
#else /* !CONFIG_PDM_MIC_PWR_CTRL_ACT_LOW */
    nrf_gpio_pin_clear(CONFIG_IO_PDM_MIC_PWR_CTRL);
#endif /* CONFIG_PDM_MIC_PWR_CTRL_ACT_LOW */
    nrf_gpio_cfg_output(CONFIG_IO_PDM_MIC_PWR_CTRL);
#endif /* CONFIG_PDM_MIC_PWR_CTRL_ENABLED */

    return nrf_drv_pdm_init(&pdm_cfg, drv_audio_pdm_event_handler);
}

#endif /* CONFIG_AUDIO_ENABLED */
