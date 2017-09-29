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

 /** @file Common macros
 *
 * @defgroup macros_common Common macros
 * @{
 * @ingroup util
 * @brief A set of useful macros.
 */

#ifndef __MACROS_COMMON_H__
#define __MACROS_COMMON_H__

#include "nrf_log.h"

/**@brief Check if the error code is equal to NRF_SUCCESS. If not, return the error code.
 */
#define RETURN_IF_ERROR(err_code)                                                    \
if ((err_code) != NRF_SUCCESS)                                                       \
{                                                                                    \
    NRF_LOG_WARNING("Err code returned in file: %s, line: %d, code %d \r\n",         \
    nrf_log_push(__FILE__), __LINE__, err_code);                                     \
    return (err_code);                                                               \
}

/**@brief Check if the input pointer is NULL. If so, return NRF_ERROR_NULL.
 */
#define NULL_PARAM_CHECK(param)         \
        if ((param) == NULL)            \
        {                               \
            return NRF_ERROR_NULL;      \
        }

#endif /*__MACROS_COMMON_H__*/

/** @} */
