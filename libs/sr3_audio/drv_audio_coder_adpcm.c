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

#define DRV_ACA_DEBUG

#ifdef DRV_ACA_DEBUG
    #include "SEGGER_RTT.h"
    #define DEBUG_PRINTF (void)SEGGER_RTT_printf
#else
    #define DEBUG_PRINTF(...)
#endif

#if (CONFIG_AUDIO_CODEC == CONFIG_AUDIO_CODEC_ADPCM)
#include "dvi_adpcm.h"

static dvi_adpcm_state_t    m_adpcm_state;

void drv_audio_coder_init(void)
{
    if (sizeof(dvi_adpcm_state_t) != 3)
    {
        DEBUG_PRINTF(0, "drv_audio: dvi_adpcm_state_t not packed correctly! size is %d", sizeof(dvi_adpcm_state_t));
    }

    dvi_adpcm_init_state(&m_adpcm_state);

    DEBUG_PRINTF(0, "drv_audio: ADPCM Codec selected (frame: %u ms)\r\n",
              1000 * CONFIG_AUDIO_FRAME_SIZE_SAMPLES / 16000);
}

void drv_audio_coder_encode(int16_t *input_samples, m_audio_frame_t *p_frame)
{
    int frame_size;

    (void)dvi_adpcm_encode((void *)input_samples,
                           (CONFIG_AUDIO_FRAME_SIZE_SAMPLES * sizeof(int16_t)),
                           p_frame->data,
                           &frame_size,
                           &m_adpcm_state, true);

    p_frame->data_size = frame_size;
}

#endif /* (CONFIG_AUDIO_CODEC == CONFIG_AUDIO_CODEC_ADPCM) */
