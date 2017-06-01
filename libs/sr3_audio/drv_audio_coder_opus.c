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

#include "nrf.h"
#include "nrf_assert.h"
//#include "app_debug.h"

#include "drv_audio.h"
#include "drv_audio_coder.h"

#define DRV_ACO_DEBUG

#ifdef DRV_ACO_DEBUG
    #include "SEGGER_RTT.h"
    #define DEBUG_PRINTF (void)SEGGER_RTT_printf
#else
    #define DEBUG_PRINTF(...)
#endif

#if (CONFIG_AUDIO_CODEC == CONFIG_AUDIO_CODEC_OPUS)
#include "opus.h"

#define OPUS_MAX_FRAME_SIZE 3840

#if   (CONFIG_OPUS_MODE == CONFIG_OPUS_MODE_CELT)
#define OPUS_APPLICATION    OPUS_APPLICATION_RESTRICTED_LOWDELAY
#define OPUS_ENCODER_SIZE   7078
#define OPUS_MODE           "CELT"

#if (CONFIG_AUDIO_FRAME_SIZE_SAMPLES != 80) && \
    (CONFIG_AUDIO_FRAME_SIZE_SAMPLES != 160) && \
    (CONFIG_AUDIO_FRAME_SIZE_SAMPLES != 320)
#error "Selected audio frame size is not supported by CELT codec."
#endif

#elif (CONFIG_OPUS_MODE == CONFIG_OPUS_MODE_SILK)
#define OPUS_APPLICATION    OPUS_APPLICATION_VOIP
#define OPUS_ENCODER_SIZE   12040
#define OPUS_MODE           "SILK"

#if (CONFIG_AUDIO_FRAME_SIZE_SAMPLES != 160) && \
    (CONFIG_AUDIO_FRAME_SIZE_SAMPLES != 320) && \
    (CONFIG_AUDIO_FRAME_SIZE_SAMPLES != 640) && \
    (CONFIG_AUDIO_FRAME_SIZE_SAMPLES != 960)
#error "Selected audio frame size is not supported by SILK codec"
#endif

#else
#error "Unsupported OPUS Mode"
#endif

__ALIGN(4) static uint8_t           m_opus_encoder[OPUS_ENCODER_SIZE];
static OpusEncoder * const          m_opus_state = (OpusEncoder *)m_opus_encoder;

void drv_audio_coder_init(void)
{
    ASSERT(opus_encoder_get_size(1) == sizeof(m_opus_encoder));

    APP_ERROR_CHECK_BOOL(opus_encoder_init(m_opus_state, 16000, 1, OPUS_APPLICATION)                    == OPUS_OK);

#if (CONFIG_OPUS_BITRATE != 0)
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_BITRATE(CONFIG_OPUS_BITRATE))          == OPUS_OK);
#else
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_BITRATE(OPUS_AUTO))                    == OPUS_OK);
#endif

#if CONFIG_OPUS_VBR_ENABLED
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_VBR(1))                                == OPUS_OK);
#else
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_VBR(0))                                == OPUS_OK);
#endif

#if (CONFIG_OPUS_VBR_ENABLED && (CONFIG_OPUS_BITRATE != 0))
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_VBR_CONSTRAINT(1))                     == OPUS_OK);
#else
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_VBR_CONSTRAINT(0))                     == OPUS_OK);
#endif

    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_COMPLEXITY(CONFIG_OPUS_COMPLEXITY))    == OPUS_OK);
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_SIGNAL(OPUS_SIGNAL_VOICE))             == OPUS_OK);
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_LSB_DEPTH(16))                         == OPUS_OK);
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_DTX(0))                                == OPUS_OK);
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_INBAND_FEC(0))                         == OPUS_OK);
    APP_ERROR_CHECK_BOOL(opus_encoder_ctl(m_opus_state, OPUS_SET_PACKET_LOSS_PERC(0))                   == OPUS_OK);

#if (CONFIG_OPUS_VBR_ENABLED && (CONFIG_OPUS_BITRATE == 0))
    DEBUG_PRINTF(0, "drv_audio: OPUS/" OPUS_MODE " Codec selected (mode: VBR, complexity: %u, frame: %u ms)\r\n",
              CONFIG_OPUS_COMPLEXITY,
              1000u * CONFIG_AUDIO_FRAME_SIZE_SAMPLES / 16000);
#else
    DEBUG_PRINTF(0, "drv_audio: OPUS/" OPUS_MODE " Codec selected (mode: %s %u kbit/s, complexity: %u, frame: %u ms)\r\n",
              (CONFIG_OPUS_VBR_ENABLED) ? "CVBR" : "CBR",
              CONFIG_OPUS_BITRATE / 1000,
              CONFIG_OPUS_COMPLEXITY,
              1000u * CONFIG_AUDIO_FRAME_SIZE_SAMPLES / 16000);
#endif
}

void drv_audio_coder_encode(int16_t *input_samples, m_audio_frame_t *p_frame)
{
    int frame_size;

    frame_size = opus_encode(m_opus_state,
                             input_samples,
                             CONFIG_AUDIO_FRAME_SIZE_SAMPLES,
                             p_frame->data + 2,
                             CONFIG_AUDIO_FRAME_SIZE_BYTES - 2);

    APP_ERROR_CHECK_BOOL((frame_size >= 0) && (frame_size <= OPUS_MAX_FRAME_SIZE));

    p_frame->data[0] = frame_size >> 8;
    p_frame->data[1] = frame_size >> 0;

    p_frame->data_size = frame_size + 2;
}

#endif /* (CONFIG_AUDIO_CODEC == CONFIG_AUDIO_CODEC_OPUS) */
