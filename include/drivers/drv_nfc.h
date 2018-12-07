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
 
/** @file NFC driver
 *
 * @defgroup NFC_driver NFC driver
 * @{
 * @ingroup drivers
 * @brief Near Field Communnication (NFC) driver API.
 *
 */
 
#ifndef __DRV_NFC_H__
#define __DRV_NFC_H__

#include "nrf_error.h"
#include "nfc_uri_rec.h"
#include <stdint.h>


/**@brief Function for writing raw data to the NFC tag.
 *
 * @param[in] p_data                   Pointer to the raw data.
 * @param[in] data_len                 Length of the raw data.
 *
 * @retval NRF_SUCCESS                 If the raw data was added successfully.
 * @retval NRF_ERROR_INVALID_LENGTH    If the length of the input data is too large for the NFC data buffer.
 * @retval NRF_ERROR_NULL              If a null pointer is given as an input parameter.
 * @retval Other errors from the underlying drivers.
 */
ret_code_t drv_nfc_raw_data_set(uint8_t const * const p_data, uint8_t data_len);

/**@brief Function for reading the data stored on the NFC tag.
 *
 * @param[in]     p_data         Pointer to the data buffer which will store the NFC tag data.
 * @param[out]    p_data_len     Pointer to a length variable which will store the data buffer length.
 *
 * @retval NRF_SUCCESS           If the raw data was added successfully.
 * @retval NRF_ERROR_NO_MEM      If the input buffer is not large enough for the NFC tag data.
 * @retval NRF_ERROR_NULL        If a null pointer is given as an input parameter.
 */
ret_code_t drv_nfc_raw_data_get(uint8_t * p_data, uint8_t * p_data_len);

/**@brief Function for adding a string record to an NDEF NFC message.
 *
 * @param[in] p_string      Pointer to the string.
 * @param[in] str_len       Length of the string including NUL ('\0') termination.
 *
 * @retval NRF_SUCCESS              If the record was added successfully.
 * @retval NRF_ERROR_NULL           The string is NULL.
 * @retval NRF_ERROR_INVALID_STATE  Driver has not been initialized.
 * @retval NRF_ERROR_INVALID_LENGTH The length is 1 or 0. 
 * @retval Other errors from the underlying drivers.
 */
ret_code_t drv_nfc_string_record_add(char const * const p_string, uint8_t str_len);


/**@brief Function for adding a URI record to an NDEF NFC message.
 *
 * @param[in] uri_id        URI identifier codes according to "URI Record Type Definition".
 * @param[in] p_uri         Pointer to the string.
 * @param[in] uri_length    Length of the string including NUL ('\0') termination.
 *
 * @retval NRF_SUCCESS              If the record was added successfully.
 * @retval NRF_ERROR_NULL           The string is NULL.
 * @retval NRF_ERROR_INVALID_STATE  Driver has not been initialized.
 * @retval NRF_ERROR_INVALID_LENGTH The length is 1 or 0. 
 * @retval Other errors from the underlying drivers.
 */
ret_code_t drv_nfc_uri_record_add(nfc_uri_id_t uri_id, char const * const p_uri, uint32_t uri_length);


/**@brief Function for adding an Android Application Record (AAR) to an NDEF NFC message.
 *
 * @param[in] p_android_package_name    Android package (app) name.
 * @param[in] package_name_length       Length of Android package name.
 *
 * @retval NRF_SUCCESS              If the record was added successfully.
 * @retval NRF_ERROR_NULL           The string is NULL.
 * @retval NRF_ERROR_INVALID_STATE  Driver has not been initialized.
 * @retval NRF_ERROR_INVALID_LENGTH The length is 1 or 0. 
 * @retval Other errors from the underlying drivers.
 */
ret_code_t drv_nfc_app_launch_android_record_add(char const * const p_android_package_name, uint32_t package_name_length);


/**@brief Function for enabling NFC.
 *
 * @note Add the desired records before calling this function.
 *
 * @retval NRF_SUCCESS  If NFC was enabled successfully.
 * @retval Other errors from the underlying drivers.
 */
ret_code_t drv_nfc_enable(void);


/**@brief Function for disabling NFC.
 *
 * @note This function deletes all stored NDEF records.
 *
 * @retval NRF_SUCCESS              If NFC was disabled successfully.
 * @retval NRF_ERROR_INVALID_STATE  Driver has not been initialized or no records have been added.
 * @retval Other errors from the underlying drivers.
 */
ret_code_t drv_nfc_disable(void);


/**@brief Function for initializing NFC.
 *
 * @note Run init first, add the desired NDEF records, and finally enable NFC.
 *
 * @retval NRF_SUCCESS  If NFC was initialized successfully.
 * @retval Other errors from the underlying drivers.
 */
ret_code_t drv_nfc_init(void);

#endif

/** @} */
