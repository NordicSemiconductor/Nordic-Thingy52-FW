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

/**
 *
 * @defgroup DRV_AUDIO_DSP Audio DSP
 * @{
 * @ingroup  MOD_AUDIO
 * @brief Audio DSP functions.
 */
#ifndef __DRV_AUDIO_DSP_H__
#define __DRV_AUDIO_DSP_H__

#include "nrf.h"
#include "arm_math.h"

/**@brief Audio gain correction by multiplying samples with a constant value.
 *
 * @details Sample buffer is overwritten with the processed samples.
 * Constant gain value is defined by AUDIO_GAIN_CONTROL_Q3_13 in (3.13) format  - (-4.000...+3.999).
 *
 * @param[in,out] p_samples_q15  Pointer to audio_buffer samples in (1.15) format.
 * @param[in] buffer_size  Number of (1.15) samples in a buffer.
 */
void drv_audio_dsp_gain_control(q15_t *p_samples_q15, unsigned int buffer_size);

/**@brief Audio equalization by IIR filtering.
 *
 * @details Sample buffer is overwritten with the processed samples.
 *
 * @param[in,out] p_samples_q15  Pointer to audio_buffer samples in (1.15) format.
 * @param[in] buffer_size  Number of (1.15) samples in a buffer.
 */
void drv_audio_dsp_equalizer(q15_t *p_samples_q15, unsigned int buffer_size);

#endif /** __DRV_AUDIO_DSP__ */
/** @} */
