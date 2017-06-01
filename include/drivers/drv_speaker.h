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

 /** @file Speaker driver
 *
 * @defgroup speaker_driver Speaker driver
 * @{
 * @ingroup drivers
 * @brief Speaker driver API.
 *
 */

#ifndef __DRV_SPEAKER_H__
#define __DRV_SPEAKER_H__

#include <stdint.h>

/**@brief Speaker driver event types.
 */
typedef enum
{
    DRV_SPEAKER_EVT_FINISHED,
    DRV_SPEAKER_EVT_BUFFER_WARNING,
    DRV_SPEAKER_EVT_BUFFER_READY
}drv_speaker_evt_t;

/**@brief Speaker driver event handler callback type.
 */
typedef void (*drv_speaker_evt_handler_t)(drv_speaker_evt_t evt);

/**@brief Speaker driver initialization type.
 */
typedef struct
{
    drv_speaker_evt_handler_t evt_handler;
}drv_speaker_init_t;

/**@brief Initializes the speaker driver.
 *
 * @param[in] p_params   Initialization parameters.
 *
 * @retval NRF_SUCCESS      Completed successfully.
 * @retval NRF_ERROR_NULL   Null pointer supplied.
 * @retval Other codes from underlying drivers.
 */
uint32_t drv_speaker_init(drv_speaker_init_t * p_params);

/**@brief Plays a single tone through the speaker.
 *
 * @param[in] freq_hz       Tone frequency (>100 Hz)
 * @param[in] duration_ms   Tone length [ms]
 * @param[in] volume        Speaker volume (0 to 100). 0 Volume will cause the function to return
 * immediately with NRF_SUCCESS.
 *
 * @retval NRF_SUCCESS                  Completed successfully.
 * @retval NRF_ERROR_NOT_SUPPORTED      Invalid parameters supplied.
 */
uint32_t drv_speaker_tone_start(uint16_t freq_hz, uint32_t duration_ms, uint8_t volume);

/**@brief Play an 8-bit 8kHz PCM encoded audio sample.
 *
 * @param[in] p_sound   Sound sample. PCM encoded, 8-bit.
 * @param[in] size      Size of the sample.
 *
 * @retval NRF_SUCCESS      Completed successfully.
 * @retval Other codes from underlying drivers.
 */
uint32_t drv_speaker_flash_pcm_play(uint8_t const * const p_sound, uint32_t size);

/**@brief Play an 8-bit 8kHz PCM encoded audio split into packages.
 *
 * @param[in] p_sound   Sound sample. PCM encoded, 8-bit.
 * @param[in] length    Length of the package.
 *
 * @retval NRF_SUCCESS      Completed successfully.
 * @retval Other codes from underlying drivers.
 */
uint32_t drv_speaker_ble_pcm_play(uint8_t * p_sound, uint32_t length);

/**@brief Play one of the prestored samples. @ref sounds.h.
 *
 * @param[in] sample_id   ID of the sample to play.
 *
 * @retval NRF_ERROR_INVALID_PARAM  Invalid sample requested.
 * @retval Other codes from underlying drivers.
 */
uint32_t drv_speaker_sample_play(uint8_t sample_id);

#endif

/** @} */
