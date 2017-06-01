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
 * @defgroup DRV_AUDIO Audio driver
 * @{
 * @ingroup MOD_AUDIO
 * @brief Audio top-level driver.
 */
#ifndef __DRV_AUDIO_H__
#define __DRV_AUDIO_H__

#include <stdbool.h>
#include <stdint.h>

#include "sdk_errors.h"
#include "drv_audio_config.h"

/**@brief Audio buffer handler. */
typedef void (*drv_audio_buffer_handler_t)(int16_t *p_buffer, uint16_t samples);

/**@brief Enable audio source.
 *
 * @retval NRF_SUCCESS
 * @retval NRF_ERROR_INTERNAL
 */
ret_code_t drv_audio_enable(void);

/**@brief Disable audio source.
 *
 * @retval NRF_SUCCESS
 * @retval NRF_ERROR_INTERNAL
 */
ret_code_t drv_audio_disable(void);

/**@brief Initialization.
 *
 * @param[in] buffer_handler    Handler which will be called when buffer with RAW audio is ready.
 * @retval NRF_SUCCESS
 * @retval NRF_ERROR_INVALID_PARAM
 * @retval NRF_ERROR_INTERNAL
 */
ret_code_t drv_audio_init(drv_audio_buffer_handler_t buffer_handler);

#endif /* __DRV_AUDIO_H__ */
/** @} */
