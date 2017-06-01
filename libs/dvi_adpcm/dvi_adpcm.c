/***********************************************************
  Copyright 1992 by Stichting Mathematisch Centrum, Amsterdam, The
  Netherlands.
  All Rights Reserved

  Permission to use, copy, modify, and distribute this software and its
  documentation for any purpose and without fee is hereby granted,
  provided that the above copyright notice appear in all copies and that
  both that copyright notice and this permission notice appear in
  supporting documentation, and that the names of Stichting Mathematisch
  Centrum or CWI not be used in advertising or publicity pertaining to
  distribution of the software without specific, written prior permission.
  15 STICHTING MATHEMATISCH CENTRUM DISCLAIMS ALL WARRANTIES WITH REGARD TO
  THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH CENTRUM BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
  OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************/

#include <stdint.h>
#include "dvi_adpcm.h"

#define ADPCM_DEBUG

#ifdef ADPCM_DEBUG
    #include "SEGGER_RTT.h"
    #define DEBUG_PRINTF (void)SEGGER_RTT_printf
#else
    #define DEBUG_PRINTF(...)
#endif

#ifndef htons
#define htons(a)                    \
    ((((a) >> 8) & 0x00ff) | \
     (((a) << 8) & 0xff00))
#endif

#ifndef ntohs
#define ntohs(a) htons((a))
#endif

#ifndef __STDC__
#define signed
#endif

/** Intel ADPCM step variation table */
static const signed char indexTable[] = {
    -1, -1, -1, -1, 2, 4, 6, 8,
    -1, -1, -1, -1, 2, 4, 6, 8
};

static const int16_t stepsizeTable[] = {
    7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
    19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
    50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
    130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
    337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
    876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
    2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
    5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
    15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
};

#define stepsizeTableSize (int32_t)(sizeof(stepsizeTable) / sizeof(int16_t))

void dvi_adpcm_init_state(dvi_adpcm_state_t * state)
{
    state->valpred = 0;
    state->index   = 0;
}

int dvi_adpcm_encode(void *in_buf, int in_size, void *out_buf, int *out_size, void *state_, bool header_flag)
{
    int8_t *out_sbuf = out_buf;
    int32_t val;              /* Current input sample value */
    int32_t sign;             /* Current adpcm sign bit */
    int32_t delta;            /* Current adpcm output value */
    int32_t diff;             /* Difference between val and valpred */
    int32_t step;             /* Stepsize */
    int32_t valpred;          /* Predicted output value */
    int32_t vpdiff;           /* Current change to valpred */
    int32_t index;            /* Current step change index */
    int32_t outputbuffer = 0; /* place to keep previous 4-bit value */
    int32_t bufferstep;       /* toggle between outputbuffer/output */
    int16_t *s;               /* output buffer for linear encoding */
    dvi_adpcm_state_t *state = (dvi_adpcm_state_t *)state_;

    in_size /= 2;
    s = (int16_t *)in_buf;

    /* Insert state into output buffer. */
    *out_size = in_size / 2;
    valpred = state->valpred;

    if (header_flag)
    {
        ((dvi_adpcm_state_t *)out_buf)->valpred = htons(state->valpred);
        ((dvi_adpcm_state_t *)out_buf)->index = state->index;
        *out_size += sizeof(dvi_adpcm_state_t);
        out_sbuf  += sizeof(dvi_adpcm_state_t);
    }

    index = state->index;
    step  = stepsizeTable[index];
    bufferstep = 1;  /* in/out: encoder state */

    for (; in_size > 0; --in_size)
    {
        val = *s++;

        /* Step 1 - Compute difference with the previous value. */
        diff = val - valpred;
        sign = (diff < 0) ? 8 : 0;
        if (sign) diff = (-diff);

        /* Step 2 - Divide and clamp. */
        /* Note:
         ** This code *approximately* computes:
         **    delta = diff*4/step;
         **    vpdiff = (delta+0.5)*step/4;
         ** but in shift step bits are dropped. The net result of this is
         ** that even if you have fast mul/div hardware, you cannot put it
         ** to good use since the fix would be too expensive.
         */
        delta = 0;
        vpdiff = (step >> 3);

        if (diff >= step)
        {
            delta |= 4;
            diff -= step;
            vpdiff += step;
        }
        step >>= 1;
        if (diff >= step)
        {
            delta |= 2;
            diff -= step;
            vpdiff += step;
        }
        step >>= 1;
        if (diff >= step)
        {
            delta |= 1;
            vpdiff += step;
        }

        /* Step 3 - Update the previous value. */
        if (sign)
            valpred -= vpdiff;
        else
            valpred += vpdiff;

        /* Step 4 - Clamp the previous value to 16 bits. */
        if (valpred > INT16_MAX)
            valpred = INT16_MAX;
        else if (valpred < INT16_MIN)
            valpred = INT16_MIN;

        /* Step 5 - Assemble value, update index and step values. */
        delta |= sign;

        index += indexTable[delta];
        if (index < 0) index = 0;
        if (index >= stepsizeTableSize) index = stepsizeTableSize - 1;
        step = stepsizeTable[index];

        // This bit here makes a diff.
        /* Step 6 - Output value. */
        if (bufferstep)
        {
            outputbuffer = (delta << 4) & 0xf0;
        }
        else
        {
            *out_sbuf++ = (delta & 0x0f) | outputbuffer;
        }
        bufferstep = !bufferstep;
    }
    /* Output last step, if needed. */
    if (!bufferstep) *out_sbuf++ = outputbuffer;

    state->valpred   = (int16_t)valpred;
    state->index     = index;
    return 0;
}

