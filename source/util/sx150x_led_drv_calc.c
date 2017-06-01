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

#include "sx150x_led_drv_calc.h"
#include "sx150x_led_drv_regs.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef DRV_EXT_LIGHT_DEBUG
    #define LOCAL_DEBUG
#endif
#include "macros_common.h"

#define REG_ONOFF_TIME_LOW_MULTIPLIER       64
#define REG_LOW_MAXVAL                      15
#define REG_HIGH_MINVAL                     16
#define REG_HIGH_MAXVAL                     31
#define REG_OFF_INTENSITY_MAXVAL            7
#define REG_ONOFF_TIME_HIGH_MULTIPLIER      512
#define REG_RISEFALL_TIME_LOW_MULTIPLIER    1
#define REG_RISEFALL_TIME_HIGH_MULTIPLIER   16
#define ACCURACY_LIMIT_PERCENT              20      /**< The maximum deviation in percent between requested time
                                                         and resulting time before a notification is returned. */

/**@brief Used for assignment of default values to sx150x_led_drv_regs_vals_t
 */
#define SX150X_LED_DRV_REG_VALS_DEFAULT   \
(sx150x_led_drv_regs_vals_t){             \
    .on_time         = 0,                 \
    .on_intensity    = 0xFF,              \
    .off_time        = 0,                 \
    .off_intensity   = 0,                 \
    .fade_in_time    = 0,                 \
    .fade_out_time   = 0                  \
};

static float    m_clkx_tics_pr_sec;         // SX150x clock ticks per sec.
static bool     m_initialized = false;      // Is this module initialized?
static uint16_t m_fade_supported_port_mask; // Port mask, indicates which pins that support the fade functionality.


bool sx150x_led_drv_calc_fade_supp(uint16_t port_mask)
{
    if (!m_initialized)
    {
        return SX150x_LED_DRV_CALC_STATUS_CODE_NOT_INIT;
    }

    if (port_mask == 0)
    {
        return false;
    }

    if ((port_mask & m_fade_supported_port_mask) == port_mask)
    {
        return true;
    }

    return false;
}


/**@brief Checks if the difference between the desired value and actual value is above a given threshold.
 */
static uint32_t diff_above_limit(uint32_t desired_val, uint32_t actual_val)
{
    if ((desired_val * ( 1 + (ACCURACY_LIMIT_PERCENT / (float)100))) < actual_val)
    {
        return 1;
    }
    else if ((desired_val * ( 1 - (ACCURACY_LIMIT_PERCENT / (float)100))) > actual_val)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**@brief Calculates the on and off register values to be set based on the desired real values requested by the user.
 *
 * @param[in,out] real_val      Will be populated with the acutal values used.
 * @param[out]    reg_val       The register values to be written to the IO extender.
*/
static uint32_t optimal_time_settings_onoff_calculate(drv_ext_light_sequence_t * const real_val,
                                                      sx150x_led_drv_regs_vals_t * const reg_val)
{
    uint32_t inaccurate_results_num = 0;

    uint32_t on_time_low_mult_reg  = (uint32_t) round(
                                     (real_val->on_time_ms /
                                     (REG_ONOFF_TIME_LOW_MULTIPLIER *
                                     255 / m_clkx_tics_pr_sec) ) / 1000);
    uint32_t on_time_high_mult_reg = (uint32_t) round(
                                     (real_val->on_time_ms /
                                     (REG_ONOFF_TIME_HIGH_MULTIPLIER *
                                     255 / m_clkx_tics_pr_sec) ) / 1000);

    if (on_time_low_mult_reg > REG_LOW_MAXVAL)
    {
        on_time_low_mult_reg = REG_LOW_MAXVAL;
    }

    if (on_time_high_mult_reg > REG_HIGH_MAXVAL)
    {
        on_time_high_mult_reg = REG_HIGH_MAXVAL;
    }

    if (on_time_high_mult_reg < REG_HIGH_MINVAL)
    {
        on_time_high_mult_reg = REG_HIGH_MINVAL;
    }

    /* Find setting with smallest difference in time. */
    uint32_t on_time_low_mult_ms  = (uint32_t) round(1000 * REG_ONOFF_TIME_LOW_MULTIPLIER *
                                    on_time_low_mult_reg  * 255 / m_clkx_tics_pr_sec);
    uint32_t on_time_high_mult_ms = (uint32_t) round(1000 * REG_ONOFF_TIME_HIGH_MULTIPLIER *
                                    on_time_high_mult_reg * 255 / m_clkx_tics_pr_sec);

    /* Calculate error */
    if (abs( (int32_t)real_val->on_time_ms - (int32_t)on_time_low_mult_ms) <=
        abs( (int32_t)real_val->on_time_ms - (int32_t)on_time_high_mult_ms))
    {
        reg_val->on_time  = (uint8_t)on_time_low_mult_reg;
        uint32_t desired_time = real_val->on_time_ms;
        real_val->on_time_ms = on_time_low_mult_ms;
        inaccurate_results_num += diff_above_limit(desired_time, real_val->on_time_ms);
    }
    else
    {
        reg_val->on_time  = (uint8_t)on_time_high_mult_reg;
        uint32_t desired_time = real_val->on_time_ms;
        real_val->on_time_ms = on_time_high_mult_ms;
        inaccurate_results_num += diff_above_limit(desired_time, real_val->on_time_ms);
    }

    uint32_t off_time_low_mult_reg  = (uint32_t) round(
                                      (real_val->off_time_ms /
                                      (REG_ONOFF_TIME_LOW_MULTIPLIER *
                                      255 / m_clkx_tics_pr_sec) ) / 1000);
    uint32_t off_time_high_mult_reg = (uint32_t) round(
                                      (real_val->off_time_ms /
                                      (REG_ONOFF_TIME_HIGH_MULTIPLIER *
                                      255 / m_clkx_tics_pr_sec) ) / 1000);

    if (off_time_low_mult_reg > REG_LOW_MAXVAL)
    {
        off_time_low_mult_reg = REG_LOW_MAXVAL;
    }

    if (off_time_high_mult_reg > REG_HIGH_MAXVAL)
    {
        off_time_high_mult_reg = REG_HIGH_MAXVAL;
    }

    if (off_time_high_mult_reg < REG_HIGH_MINVAL)
    {
        off_time_high_mult_reg = REG_HIGH_MINVAL;
    }

    /* Find setting with smallest difference in time. */
    uint32_t off_time_low_mult_ms  = (uint32_t) round(1000 * REG_ONOFF_TIME_LOW_MULTIPLIER *
                                     off_time_low_mult_reg * 255 / m_clkx_tics_pr_sec);
    uint32_t off_time_high_mult_ms = (uint32_t) round(1000 * REG_ONOFF_TIME_HIGH_MULTIPLIER *
                                     off_time_high_mult_reg * 255 / m_clkx_tics_pr_sec);


    if (abs( (int32_t)real_val->off_time_ms - (int32_t)off_time_low_mult_ms) <=
        abs( (int32_t)real_val->off_time_ms - (int32_t)off_time_high_mult_ms))
    {
        reg_val->off_time  = (uint8_t)off_time_low_mult_reg;
        uint32_t desired_time = real_val->off_time_ms;
        real_val->off_time_ms = off_time_low_mult_ms;
        inaccurate_results_num += diff_above_limit(desired_time, real_val->off_time_ms);

    }
    else
    {
        reg_val->off_time  = (uint8_t)off_time_high_mult_reg;
        uint32_t desired_time = real_val->off_time_ms;
        real_val->off_time_ms = off_time_high_mult_ms;
        inaccurate_results_num += diff_above_limit(desired_time, real_val->off_time_ms);
    }

    return inaccurate_results_num;
}

/**@brief Calculates the rise and fall register values to be set based on the desired real values requested by the user.
 *
 * @param[in,out] real_val      Will be populated with the acutal values used.
 * @param[out]    reg_val       The register values to be written to the IO extender.
 */
static uint32_t optimal_time_settings_risefall_calculate(drv_ext_light_sequence_t * const real_val,
                                                         sx150x_led_drv_regs_vals_t * const reg_val)
{
    uint32_t inaccurate_results_num = 0;
    /* Fade in */
    uint32_t fade_in_time_low_mult_reg  = (uint32_t) round(
                                          (real_val->fade_in_time_ms /
                                          (REG_RISEFALL_TIME_LOW_MULTIPLIER *
                                          (reg_val->on_intensity - (4 * reg_val->off_intensity)) *
                                          (255 / m_clkx_tics_pr_sec))) / 1000);

    uint32_t fade_in_time_high_mult_reg = (uint32_t) round(
                                          (real_val->fade_in_time_ms /
                                          (REG_RISEFALL_TIME_HIGH_MULTIPLIER *
                                          (reg_val->on_intensity - (4 * reg_val->off_intensity)) *
                                          (255 / m_clkx_tics_pr_sec))) / 1000);

    if (fade_in_time_low_mult_reg  > REG_LOW_MAXVAL)
    {
        fade_in_time_low_mult_reg  = REG_LOW_MAXVAL;
    }

    if (fade_in_time_high_mult_reg > REG_HIGH_MAXVAL)
    {
        fade_in_time_high_mult_reg = REG_HIGH_MAXVAL;
    }

    if (fade_in_time_high_mult_reg < REG_HIGH_MINVAL)
    {
        fade_in_time_high_mult_reg = REG_HIGH_MINVAL;
    }

    uint32_t fade_in_time_low_mult_ms = (uint32_t) round(1000 * ((reg_val->on_intensity -
                                        (4 * reg_val->off_intensity)) *
                                        REG_RISEFALL_TIME_LOW_MULTIPLIER *
                                        fade_in_time_low_mult_reg * 255 / m_clkx_tics_pr_sec));

    uint32_t fade_in_time_high_mult_ms = (uint32_t) round(1000 * ((reg_val->on_intensity -
                                         (4 * reg_val->off_intensity)) *
                                         REG_RISEFALL_TIME_HIGH_MULTIPLIER *
                                         fade_in_time_high_mult_reg * 255 / m_clkx_tics_pr_sec));

    /* Calculate error. */
    if (abs( (int32_t)real_val->fade_in_time_ms - (int32_t)fade_in_time_low_mult_ms) <=
        abs( (int32_t)real_val->fade_in_time_ms - (int32_t)fade_in_time_high_mult_ms))
    {
        reg_val->fade_in_time  = (uint8_t)fade_in_time_low_mult_reg;
        uint32_t desired_time = real_val->fade_in_time_ms;
        real_val->fade_in_time_ms = fade_in_time_low_mult_ms;
        inaccurate_results_num += diff_above_limit(desired_time, real_val->fade_in_time_ms);
    }

    else
    {
        reg_val->fade_in_time  = (uint8_t)fade_in_time_high_mult_reg;
        uint32_t desired_time = real_val->fade_in_time_ms;
        real_val->fade_in_time_ms = fade_in_time_high_mult_ms;
        inaccurate_results_num += diff_above_limit(desired_time, real_val->fade_in_time_ms);
    }

    /* Fade out. */

    uint32_t fade_out_time_low_mult_reg  = (uint32_t) round((real_val->fade_out_time_ms /
                                           (REG_RISEFALL_TIME_LOW_MULTIPLIER *
                                           (reg_val->on_intensity - (4 * reg_val->off_intensity)) *
                                           (255 / m_clkx_tics_pr_sec))) / 1000);

    uint32_t fade_out_time_high_mult_reg = (uint32_t) round((real_val->fade_out_time_ms /
                                           (REG_RISEFALL_TIME_HIGH_MULTIPLIER *
                                           (reg_val->on_intensity - (4 * reg_val->off_intensity)) *
                                           (255 / m_clkx_tics_pr_sec))) / 1000);

    if (fade_out_time_low_mult_reg  > REG_LOW_MAXVAL )
    {
        fade_out_time_low_mult_reg  = REG_LOW_MAXVAL;
    }

    if (fade_out_time_high_mult_reg > REG_HIGH_MAXVAL )
    {
        fade_out_time_high_mult_reg = REG_HIGH_MAXVAL;
    }

    if (fade_out_time_high_mult_reg < REG_HIGH_MINVAL )
    {
        fade_out_time_high_mult_reg = REG_HIGH_MINVAL;
    }

    uint32_t fade_out_time_low_mult_ms =  (uint32_t) round( 1000 * (
                                          (reg_val->on_intensity -
                                          (4 * reg_val->off_intensity)) *
                                          REG_RISEFALL_TIME_LOW_MULTIPLIER *
                                          fade_out_time_low_mult_reg * 255 / m_clkx_tics_pr_sec));

    uint32_t fade_out_time_high_mult_ms = (uint32_t) round( 1000 * (
                                          (reg_val->on_intensity -
                                          (4 * reg_val->off_intensity)) *
                                          REG_RISEFALL_TIME_HIGH_MULTIPLIER *
                                          fade_out_time_high_mult_reg * 255 / m_clkx_tics_pr_sec));


    if ( abs( (int32_t)real_val->fade_out_time_ms - (int32_t)fade_out_time_low_mult_ms) <=
         abs( (int32_t)real_val->fade_out_time_ms - (int32_t)fade_out_time_high_mult_ms))
    {
        reg_val->fade_out_time  = (uint8_t)fade_out_time_low_mult_reg;
        uint32_t desired_time = real_val->fade_out_time_ms;
        real_val->fade_out_time_ms = fade_out_time_low_mult_ms;
        inaccurate_results_num += diff_above_limit(desired_time, real_val->fade_out_time_ms);
    }

    else
    {
        reg_val->fade_out_time  = (uint8_t)fade_out_time_high_mult_reg;
        uint32_t desired_time = real_val->fade_out_time_ms;
        real_val->fade_out_time_ms = fade_out_time_high_mult_ms;
        inaccurate_results_num += diff_above_limit(desired_time, real_val->fade_out_time_ms);
    }

    return inaccurate_results_num;
}


ret_code_t sx150x_led_drv_calc_convert(uint16_t port_mask,
                                       drv_ext_light_sequence_t * const real_vals,
                                       sx150x_led_drv_regs_vals_t * const reg_vals)
{
    uint32_t inaccurate_results_num = 0;

    if (port_mask == 0)
    {
        return SX150x_LED_DRV_CALC_STATUS_CODE_INVALID_PARAM;
    }

    if (!m_initialized)
    {
        return SX150x_LED_DRV_CALC_STATUS_CODE_NOT_INIT;
    }

    NULL_PARAM_CHECK(real_vals);

    *reg_vals = SX150X_LED_DRV_REG_VALS_DEFAULT;


    reg_vals->on_intensity  = real_vals->on_intensity;

    // Rounding division
    reg_vals->off_intensity = (real_vals->off_intensity + (4 / 2)) / 4;

    if ( reg_vals->off_intensity > REG_OFF_INTENSITY_MAXVAL )
    {
        reg_vals->off_intensity = REG_OFF_INTENSITY_MAXVAL ;
    }

    real_vals->off_intensity = reg_vals->off_intensity * 4;

    inaccurate_results_num += optimal_time_settings_onoff_calculate(real_vals, reg_vals);

    if (sx150x_led_drv_calc_fade_supp(port_mask))
    {
        inaccurate_results_num += optimal_time_settings_risefall_calculate(real_vals, reg_vals);
    }
    else
    {
        if ((real_vals->fade_in_time_ms != 0) || (real_vals->fade_out_time_ms != 0))
        {
            DEBUG_PRINTF(0, RTT_CTRL_TEXT_BRIGHT_YELLOW"The given pin does not support rise/fall. These values have been ignored."RTT_CTRL_RESET"\n");
        }

        real_vals->fade_in_time_ms  = 0;
        real_vals->fade_out_time_ms = 0;
        reg_vals->fade_in_time      = 0;
        reg_vals->fade_out_time     = 0;
    }

    if (inaccurate_results_num != 0)
    {
        return SX150x_LED_DRV_CALC_STATUS_CODE_INACCURATE;
    }
    return SX150x_LED_DRC_CALC_STATUS_CODE_SUCCESS;
}


void sx150x_led_drv_calc_init(uint16_t fade_supported_port_mask, uint32_t clkx_tics_pr_sec)
{

    m_clkx_tics_pr_sec = (float)clkx_tics_pr_sec;

    m_fade_supported_port_mask = fade_supported_port_mask;
    m_initialized = true;
}
