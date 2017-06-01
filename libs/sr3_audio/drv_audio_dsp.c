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

#include "drv_audio_dsp.h"
#include "drv_audio_config.h"

#define AUDIO_GAIN_CONTROL_Q3_13    ((q15_t)(1.0f * 8192)) // Fixpoint_val*2^13 format [F3.13] - [-4.000..+3.999].
#define MAX_Q15                     ((q31_t) 0x00007FFF)
#define MIN_Q15                     ((q31_t) 0xFFFF8000)
#define USE_STATIC_INIT
#define Q15_SCALE  32567
#define Q14_SCALE  16384
#define N_BANDS_MAX  4
#define N_BANDS_EQ1  1
#define N_BANDS_EQ2  2

#if (CONFIG_AUDIO_EQUALIZER_ENABLED || CONFIG_AUDIO_GAIN_CONTROL_ENABLED)
/**@brief Saturation of 32-bit fractional format
 *
 * @note SSAT is a name compatible CMISI math library definition.
 *
 * @param[in] x - Sample in 32-bit fractional format.
 * @param[in] max_y - Max value in 32-bit fractional format.
 * @param[in] min_y - Min value in 32-bit fractional format.
 *
 * @return  Saturated sample in 32-bit fractional format.
 */
static inline q31_t SSAT( q31_t x, q31_t max_y, q31_t min_y)
{
    q31_t y;

    y=x;
    if(y > max_y)
    {
      y = max_y;
    }
    else if(y < min_y)
    {
      y = min_y;
    }

    return (y);
}
#endif /* (CONFIG_AUDIO_EQUALIZER_ENABLED || CONFIG_AUDIO_GAIN_CONTROL_ENABLED) */

#if CONFIG_AUDIO_EQUALIZER_ENABLED

typedef struct   /* y[n]=b0*x[n]+b1*x[n-1]+b2*x[n]+a1*y[n-1]+a2*y[n-1]  */
{
    q15_t b0;     // [Q2.14]
    q15_t b1;     // [Q2.14]
    q15_t b2;     // [Q2.14]
    q15_t a1;     // [Q2.14]
    q15_t a2;     // [Q2.14]
    q15_t x_1;    // x[n-1] [Q1.15]
    q15_t x_2;    // x[n-2] [Q1.15]
    q15_t y_1;    // y[n-1] [Q1.15]
    q15_t y_2;    // y[n-2] [Q1.15]
} biquad_t;

typedef struct
{
    uint32_t      nOfBands;
    q15_t         gain[N_BANDS_MAX];  //[Q1.15]
    biquad_t  *p_biquad[N_BANDS_MAX];
} equalizer_q15_t;

#if (AUDIO_EQ_DEF == 1)

static biquad_t biquad1 = {
    .b0 =  (q15_t)((0.2398928f)*Q14_SCALE),     // [Q2.14]
    .b1 =  (q15_t)((0.0f)*Q14_SCALE),           // [Q2.14]
    .b2 =  (q15_t)((- 0.2398928f)*Q14_SCALE),   // [Q2.14]
    .a1 =  (q15_t)((0.6352174f)*Q14_SCALE),     // [Q2.14]
    .a2 =  (q15_t)((- 0.5202144f)*Q14_SCALE),   // [Q2.14]
    .x_1 = (q15_t) 0,
    .x_2 = (q15_t) 0,
    .y_1 = (q15_t) 0,
    .y_2 = (q15_t) 0
};

static equalizer_q15_t eq1 = {
 .nOfBands = N_BANDS_EQ1,
 .gain = { (q15_t)((0.5f)*Q15_SCALE) },
 .p_biquad = {&biquad1}
};

#elif (AUDIO_EQ_DEF == 2)

static biquad_t biquad2a = {
    .b0 =  (q15_t)((0.0272437f)*Q14_SCALE),     // [Q2.14]
    .b1 =  (q15_t)((0.0f)*Q14_SCALE),           // [Q2.14]
    .b2 =  (q15_t)((-0.0272437f)*Q14_SCALE),    // [Q2.14]
    .a1 =  (q15_t)((1.9395213f)*Q14_SCALE),     // [Q2.14]
    .a2 =  (q15_t)((-0.9455125f)*Q14_SCALE),    // [Q2.14]
    .x_1 = (q15_t) 0,
    .x_2 = (q15_t) 0,
    .y_1 = (q15_t) 0,
    .y_2 = (q15_t) 0
};

static biquad_t biquad2b = {
    .b0 =  (q15_t)((0.1955092f)*Q14_SCALE),     // [Q2.14]
    .b1 =  (q15_t)((0.0f)*Q14_SCALE),           // [Q2.14]
    .b2 =  (q15_t)((-0.1955092f)*Q14_SCALE),    // [Q2.14]
    .a1 =  (q15_t)((1.1790345f)*Q14_SCALE),     // [Q2.14]
    .a2 =  (q15_t)((-0.6089816f)*Q14_SCALE),    // [Q2.14]
    .x_1 = (q15_t) 0,
    .x_2 = (q15_t) 0,
    .y_1 = (q15_t) 0,
    .y_2 = (q15_t) 0
};

static equalizer_q15_t eq2 = {
 .nOfBands = N_BANDS_EQ2,
 .gain = { (q15_t)((0.5f)*Q15_SCALE), (q15_t)((-0.5f)*Q15_SCALE) },
 .p_biquad = {&biquad2a, &biquad2b}
};

#elif (AUDIO_EQ_DEF == 3)

static biquad_t biquad2a = {
    .b0 =  (q15_t)((0.0203635f)*Q14_SCALE),     // [Q2.14]
    .b1 =  (q15_t)((0.0f)*Q14_SCALE),           // [Q2.14]
    .b2 =  (q15_t)((-0.0203635f)*Q14_SCALE),    // [Q2.14]
    .a1 =  (q15_t)((1.0885138f)*Q14_SCALE),     // [Q2.14]
    .a2 =  (q15_t)((-0.9592731f)*Q14_SCALE),    // [Q2.14]
    .x_1 = (q15_t) 0,
    .x_2 = (q15_t) 0,
    .y_1 = (q15_t) 0,
    .y_2 = (q15_t) 0
};

static biquad_t biquad2b = {
    .b0 =  (q15_t)((0.0225756f)*Q14_SCALE),     // [Q2.14]
    .b1 =  (q15_t)((0.0f)*Q14_SCALE),           // [Q2.14]
    .b2 =  (q15_t)((-0.0225756f)*Q14_SCALE),    // [Q2.14]
    .a1 =  (q15_t)((0.7480883f)*Q14_SCALE),     // [Q2.14]
    .a2 =  (q15_t)((-0.9548489f)*Q14_SCALE),    // [Q2.14]
    .x_1 = (q15_t) 0,
    .x_2 = (q15_t) 0,
    .y_1 = (q15_t) 0,
    .y_2 = (q15_t) 0
};

static equalizer_q15_t eq2 = {
 .nOfBands = N_BANDS_EQ2,
 .gain = { (q15_t)((-0.5f)*Q15_SCALE), (q15_t)((0.5f)*Q15_SCALE) },
 .p_biquad = {&biquad2a, &biquad2b}
};

#else
#error "No Equalizer configuration selected"
#endif

/**@brief Trans-coding of [1.15] to [2.48]
 *.
 * @param[in] x - Sample in [1.15] format.
 *
 * @return  Sample in [4.28] format.
 */
static inline q31_t q15_to_q28(q15_t x )
{
    return ((q31_t)x << 13) ;   // [4.28] coded as [1.31].
}

/**@brief Multiplication of [1.15] by [2.48] numbers and formatting the result as [4.28]
 *
 * @param[in] x - Sample in [1.15] format.
 * @param[in] y - Sample in [2.14] format.
 *
 * @return  Product in [4.28] format.
 */
static inline q31_t q28_q15_x_q14(q15_t x_q15, q15_t y_q15)
{
    return (q31_t) (((q31_t)(x_q15) * (y_q15))>>1); // [Q1.15]x[Q2.14]=Q[3.29] >> 1 => Q[4.28]
}

/**@brief Multiplication of [1.15] by [1.15] numbers and formatting the result as [4.28].
 *
 * @param[in] x - Sample in [1.15] format.
 * @param[in] y - Sample in [1.15] format.
 *
 * @return  Product in [4.28] format.
 */
static inline q31_t q28_q15_x_q15(q15_t x_q15, q15_t y_q15)
{
    return (q31_t) (((q31_t)(x_q15) * (y_q15))>>2); // [Q1.15]x[Q1.15]=Q[2.30] >> 2 = Q[4.28]
}

/**@brief Saturation of 4.28 number.
 *
 * @param[in] x - Sample in [4.28] format.
 * @return  Saturated sample in [1.15] format.
 *
 */
static inline q15_t q15_sat_q28(q31_t x)
{
    return (q15_t)(SSAT(x, 0x0FFFFFFF, 0xF0000000) >> 13); // [4.28] (- SAT +0.99999/-1.0 ) -> [1.15]
}

/**@brief Equalization by IIR filtering
 *
 * @param[in] x - Samples in [1.15] format.
 * @param[in] p_eq - Pointer to bi-quad filter definition.
 * @return Filter response as a sample in [1.15] format.
 *
 */
static q15_t m_bp_filter(q15_t x, biquad_t *p_biquad)
{
    q31_t z;
    q15_t y;

    //  [z] = 4.28
    // z =b0*x[n]+b1*x[n-1]+b2*x[n]+a1*y[n-1]+a2*y[n-1]  */
    z = q28_q15_x_q14( x, p_biquad->b0);
    z += q28_q15_x_q14( p_biquad->x_1, p_biquad->b1);
    z += q28_q15_x_q14( p_biquad->x_2, p_biquad->b2);
    z += q28_q15_x_q14( p_biquad->y_1, p_biquad->a1);
    z += q28_q15_x_q14( p_biquad->y_2, p_biquad->a2);
    y = q15_sat_q28(z); // Accumulated in 4.28, saturated at [-1.0..0.9999], in 1.15.

    p_biquad->x_2 = p_biquad->x_1;
    p_biquad->x_1 = x;
    p_biquad->y_2 = p_biquad->y_1;
    p_biquad->y_1 = y;

    return y;
}

/**@brief Equalization
 *
 * @param[in] x - Samples in [1.15] format.
 * @param[in] p_eq - Pointer to equalizer definition.
 * @return Filter response as a sample in [1.15] format.
 *
 */
static q15_t m_equalizer_filter(q15_t x, equalizer_q15_t *p_eq)
{
  q31_t z;
  q15_t y;
  int32_t  i;

    z = q15_to_q28(x); // [4.28] z = x
    for (i=0; i<p_eq->nOfBands; i++)
    {
      y = m_bp_filter(x, p_eq->p_biquad[i]);
      z += q28_q15_x_q15( p_eq->gain[i], y); // [4.28] z = z + gain(i)*Band_out(i)
    }
    y = q15_sat_q28(z); // Accumulated in 4.28, saturated at [-1.0..0.9999], in 1.15.

    return y;
}

void drv_audio_dsp_equalizer(q15_t *p_samples_q15, unsigned int buffer_size)
{
    unsigned int i;

    for (i=0; i<buffer_size; i++)
    {
      *p_samples_q15 = m_equalizer_filter((*p_samples_q15), &eq2) ;
      p_samples_q15++;
    }
}
#endif /* CONFIG_AUDIO_EQUALIZER_ENABLED */

#if CONFIG_AUDIO_GAIN_CONTROL_ENABLED
void drv_audio_dsp_gain_control(q15_t *p_samples_q15, unsigned int buffer_size)
{
    q15_t gain = AUDIO_GAIN_CONTROL_Q3_13; // [F3.13]
    unsigned int i;

    for (i=0; i<buffer_size; i++)
    {
      // The result is [F1.15], that is ([F3.15] = [F1.15]x[F3.13]) saturated to [F1.15]).
      *p_samples_q15 = (q15_t) SSAT((((q31_t) (*p_samples_q15) * (gain)) >> 13), MAX_Q15, MIN_Q15);
      p_samples_q15++;
    }
}
#endif /* CONFIG_AUDIO_GAIN_CONTROL_ENABLED */
