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

/** @file Microphone driver
 *
 * @defgroup mic_driver Microphone driver
 * @{
 * @ingroup drivers
 * @brief Microphone driver API.
 *
 */

#ifndef __DRV_MIC_H__
#define __DRV_MIC_H__

#include <stdint.h>
#include "drv_audio_config.h"

/**@brief Compressed audio frame representation.
 */
typedef struct
{
    uint8_t     data[CONFIG_AUDIO_FRAME_SIZE_BYTES];
    uint16_t    data_size;
} m_audio_frame_t;

/**@brief Microphone driver data handler callback type.
 */
typedef uint32_t (*drv_mic_data_handler_t)(m_audio_frame_t * p_frame);

/**@brief Function for starting the microphone driver.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 */
uint32_t drv_mic_start(void);

/**@brief Function for stopping the microphone driver.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 */
uint32_t drv_mic_stop(void);

/**@brief Function for initializing the microphone driver.
 *
 * @param[in] data_handler      Pointer data handler callback.
 *
 * @retval NRF_SUCCESS             If initialization was successful.
 */
uint32_t drv_mic_init(drv_mic_data_handler_t data_handler);

#endif

/** @} */
