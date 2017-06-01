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

 /** @file Sx150x LED driver register calculation
 *
 * @defgroup sx150x_led_drv_calc Sx150x LED driver register calculation
 * @{
 * @ingroup util
 * @brief Sx150x LED driver register calculation API.
 *
 */

#ifndef __SX150X_LED_DRV_CALC_H__
#define __SX150X_LED_DRV_CALC_H__

#include "sx150x_led_drv_calc.h"
#include "sx150x_led_drv_regs.h"
#include "sdk_errors.h"
#include <stdint.h>
#include <stdbool.h>

/**@brief The SX150x led driver calc status codes
 */
enum
{
    SX150x_LED_DRC_CALC_STATUS_CODE_SUCCESS,         ///< Successful.
    SX150x_LED_DRV_CALC_STATUS_CODE_INACCURATE,      ///< One or more of the set time parameters deviate by more than 20 %. Accept or try again with different values or osc divider.
    SX150x_LED_DRV_CALC_STATUS_CODE_INVALID_PARAM,   ///< Invalid parameters.
    SX150x_LED_DRV_CALC_STATUS_CODE_NULL,            ///< NULL parameter supplied.
    SX150x_LED_DRV_CALC_STATUS_CODE_NOT_INIT,        ///< The module has not been initalized. Has init been called?
};


/**@brief Checks if all the supplied pins in port_mask supports fade
 *
 * @param[in] port_mask     16-bit port mask.
 *
 * @note: If port_mask = 0, false is returned.
 *
 * @return true             All the given pins support fade.
 * @return false            One or more of the supplied pins does not support fade.
 */
bool sx150x_led_drv_calc_fade_supp(uint16_t port_mask);


/**@brief Converts from real values (intensity (0x00-0xFF and time in milliseconds) to register values
 *
 * @param[in] port_mask         16-bit port mask
 * @param[in,out] real_vals     The requested values. Will contain the actual set values after processing
 * @param[out] reg_vals         Register values set as close as possible to achieve the requested values
 *
 * @return SX150x_LED_DRC_CALC_STATUS_CODE_SUCCESS
 * @return SX150x_LED_DRV_CALC_STATUS_CODE_INACCURATE
 * @return SX150x_LED_DRV_CALC_STATUS_CODE_INVALID_PARAM
 * @return SX150x_LED_DRV_CALC_STATUS_CODE_NULL
 * @return SX150x_LED_DRV_CALC_STATUS_CODE_NOT_INIT
 */
ret_code_t sx150x_led_drv_calc_convert(uint16_t port_mask, drv_ext_light_sequence_t * const real_vals,
                                       sx150x_led_drv_regs_vals_t * const reg_vals);

/**@brief Initializes the SX150x led driver register calculation module
 *
 * @param[in] fade_supported_port_mask          16-bit port mask holding "1" in positions corresponding to pins which supports fading.
 * @param[in] clkx_tics_pr_sec                  Number of ticks / sec for the internal ioext oscillatior after division.
 *
 */
void sx150x_led_drv_calc_init(uint16_t fade_supported_port_mask, uint32_t clkx_tics_pr_sec);

#endif

/** @} */
