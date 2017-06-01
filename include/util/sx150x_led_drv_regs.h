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

#ifndef __SX150X_LED_DRV_REGS_H__
#define __SX150X_LED_DRV_REGS_H__

#include <stdint.h>

 /** @file Settings for Sx150x LED driver registers
 *
 * @defgroup sx150x_led_drv_regs Settings for Sx150x LED driver registers
 * @{
 * @ingroup util
 * @brief Settings for all LED driver registers on the Sx150x.
 * @note Actual register values will be calculated based on the DRV_EXT_FOSC_DIVIDER.
 * Timings are not accurate, and accuracy will depend on the divider value and requested time.
 * ext_global_osc_divider influences all devices connected to the SX150x
 */

typedef struct
{
    uint32_t    on_time_ms;
    uint8_t     on_intensity;
    uint32_t    off_time_ms;
    uint8_t     off_intensity;
    uint32_t    fade_in_time_ms;
    uint32_t    fade_out_time_ms;
}drv_ext_light_sequence_t;

/**@brief Struct for holding values to be placed in io extender led driver registers.
* Consult the SX150x manual for further details */
typedef struct
{
    uint8_t     on_time;
    uint8_t     on_intensity;
    uint8_t     off_time;
    uint8_t     off_intensity;
    uint8_t     fade_in_time;
    uint8_t     fade_out_time;
}sx150x_led_drv_regs_vals_t;

#define SX150X_LED_DRIVER_SEQUENCE_ON_TIME_DEFAULT         (0)
#define SX150X_LED_DRIVER_SEQUENCE_ON_INTENSITY_DEFAULT    (0xFF)
#define SX150X_LED_DRIVER_SEQUENCE_OFF_TIME_DEFAULT        (0)
#define SX150X_LED_DRIVER_SEQUENCE_OFF_INTENSITY_DEFAULT   (0)
#define SX150X_LED_DRIVER_SEQUENCE_FADE_IN_TIME_DEFAULT    (0)
#define SX150X_LED_DRIVER_SEQUENCE_FADE_OUT_TIME_DEFAULT   (0)

#define SEQUENCE_DEFAULT_VAL (drv_ext_light_sequence_t)                                      \
{   .on_time_ms         =  SX150X_LED_DRIVER_SEQUENCE_ON_TIME_DEFAULT,                       \
    .on_intensity       =  SX150X_LED_DRIVER_SEQUENCE_ON_INTENSITY_DEFAULT,                  \
    .off_time_ms        =  SX150X_LED_DRIVER_SEQUENCE_OFF_TIME_DEFAULT,                      \
    .off_intensity      =  SX150X_LED_DRIVER_SEQUENCE_OFF_INTENSITY_DEFAULT,                 \
    .fade_in_time_ms    =  SX150X_LED_DRIVER_SEQUENCE_FADE_IN_TIME_DEFAULT,                  \
    .fade_out_time_ms   =  SX150X_LED_DRIVER_SEQUENCE_FADE_OUT_TIME_DEFAULT,                 \
}

#define SEQUENCE_REG_DEFAULT_VAL (sx150x_led_drv_regs_vals_t)                             \
{   .on_time         =  SX150X_LED_DRIVER_SEQUENCE_ON_TIME_DEFAULT,                       \
    .on_intensity    =  SX150X_LED_DRIVER_SEQUENCE_ON_INTENSITY_DEFAULT,                  \
    .off_time        =  SX150X_LED_DRIVER_SEQUENCE_OFF_TIME_DEFAULT,                      \
    .off_intensity   =  SX150X_LED_DRIVER_SEQUENCE_OFF_INTENSITY_DEFAULT,                 \
    .fade_in_time    =  SX150X_LED_DRIVER_SEQUENCE_FADE_IN_TIME_DEFAULT,                  \
    .fade_out_time   =  SX150X_LED_DRIVER_SEQUENCE_FADE_OUT_TIME_DEFAULT,                 \
}


#endif

/** @} */
