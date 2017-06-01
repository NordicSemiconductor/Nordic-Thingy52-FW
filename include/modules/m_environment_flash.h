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

 /** @file Environment module flash configuration
 *
 * @defgroup m_env_flash_config Environment flash configuration
 * @{
 * @ingroup m_environment
 * @brief Environment module flash configuration.
 *
 */

#ifndef __M_ENVIRONMENT_FLASH_H__
#define __M_ENVIRONMENT_FLASH_H__

#include "ble_tes.h"

/**@brief Gas sensor baseline values for different modes of operation.
 */
typedef PACKED( struct
{
    uint16_t mode_250ms;
    uint16_t mode_1s;
    uint16_t mode_10s;
    uint16_t mode_60s;
}) m_gas_baseline_t;

/**@brief Function for storing the configuration.
 *
 * @param[in] p_config  Pointer to configuration to be stored.
 *
 * @retval NRF_SUCCESS      If initialization was successful.
 * @retval NRF_ERROR_NULL   If a NULL pointer was supplied.
 * @retval Other codes from the underlying drivers.
 */
uint32_t m_env_flash_config_store(ble_tes_config_t * p_config);

/**@brief Function for loading the configuration.
 *
 * @param[out] p_config  Pointer to loaded configuration.
 *
 * @retval NRF_SUCCESS      If initialization was successful.
 * @retval Other codes from the underlying drivers.
 */
uint32_t m_env_flash_config_load(ble_tes_config_t ** p_config);

/**@brief Function for storing the gas sensor baseline.
 *
 * @param[in] p_baseline  Pointer to baseline to be stored.
 *
 * @retval NRF_SUCCESS      If initialization was successful.
 * @retval NRF_ERROR_NULL   If a NULL pointer was supplied.
 * @retval Other codes from the underlying drivers.
 */
uint32_t m_env_flash_baseline_store(m_gas_baseline_t * p_baseline);

/**@brief Function for loading the gas sensor baseline.
 *
 * @param[out] p_baseline  Pointer to loaded baseline.
 *
 * @retval NRF_SUCCESS      If initialization was successful.
 * @retval Other codes from the underlying drivers.
 */
uint32_t m_env_flash_baseline_load(m_gas_baseline_t ** p_baseline);

/**@brief Function for initializing weather station flash handling.
 *
 * @param[in]  p_default_config     Pointer to default configuration.
 * @param[out] p_config             Pointer to current configuration.
 * @param[in]  p_default_baseline   Pointer to default gas sensor baseline.
 * @param[out] p_baseline           Pointer to current gas sensor baseline.
 *
 * @retval NRF_SUCCESS      If initialization was successful.
 * @retval NRF_ERROR_NULL   If a NULL pointer was supplied.
 * @retval Other codes from the underlying drivers.
 */
uint32_t m_env_flash_init(const ble_tes_config_t * p_default_config,
                          ble_tes_config_t ** p_config,
                          const m_gas_baseline_t * p_default_baseline,
                          m_gas_baseline_t      ** p_baseline);

#endif

/** @} */
