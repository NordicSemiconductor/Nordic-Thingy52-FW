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
#include "macros_common.h"

#include "drv_audio.h"
#include "drv_audio_coder.h"

#if (CONFIG_AUDIO_CODEC == CONFIG_AUDIO_CODEC_BV32FP)
#include "typedef.h"
#include "bvcommon.h"
#include "bv32cnst.h"
#include "bv32strct.h"
#include "bv32.h"
#include "bitpack.h"

#if !defined(__FPU_USED) || (!__FPU_USED)
#error "BV32FP codec requires FPU!".
#endif

static struct BV32_Encoder_State    m_enc_state;

void drv_audio_coder_init(void)
{
    Reset_BV32_Coder(&m_enc_state);

    DEBUG_PRINTF(0, "drv_audio: BV32FP Codec selected");
}

void drv_audio_coder_encode(int16_t *input_samples, m_audio_frame_t *p_frame)
{
    struct BV32_Bit_Stream bs;

    BV32_Encode(&bs, &m_enc_state, input_samples);
    BV32_BitPack(p_frame->data, &bs);

    p_frame->data_size = CONFIG_AUDIO_FRAME_SIZE_BYTES;
}

#endif /* (CONFIG_AUDIO_CODEC == CONFIG_AUDIO_CODEC_BV32FP) */
