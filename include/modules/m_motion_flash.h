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

 /** @file Motion module flash configuration
 *
 * @defgroup m_motion_flash_config Motion flash configuration
 * @{
 * @ingroup m_motion
 * @brief Motion module flash configuration.
 *
 */

#ifndef __M_MOTION_FLASH_H__
#define __M_MOTION_FLASH_H__

#include "ble_tms.h"

/**@brief Function for storing the configuration.
 *
 * @param[in] p_config  Pointer to configuration to be stored.
 *
 * @retval NRF_SUCCESS      If initialization was successful.
 * @retval NRF_ERROR_NULL   If a NULL pointer was supplied.
 * @retval Other codes from the underlying drivers.
 */
uint32_t m_motion_flash_config_store(ble_tms_config_t * p_config);

/**@brief Function for storing the configuration.
 *
 * @param[out] p_config  Pointer to loaded configuration.
 *
 * @retval NRF_SUCCESS      If initialization was successful.
 * @retval Other codes from the underlying drivers.
 */
uint32_t m_motion_flash_config_load(ble_tms_config_t ** p_config);

/**@brief Function for initializing weather station flash handling.
 *
 * @param[in] p_default_config  Pointer to default configuration.
 * @param[out] p_config         Pointer to current configuration.
 *
 * @retval NRF_SUCCESS      If initialization was successful.
 * @retval NRF_ERROR_NULL   If a NULL pointer was supplied.
 * @retval Other codes from the underlying drivers.
 */
uint32_t m_motion_flash_init(const ble_tms_config_t * p_default_config, ble_tms_config_t ** p_config);

#endif

/** @} */
