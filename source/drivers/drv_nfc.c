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

#include "drv_nfc.h"
#include "nfc_t2t_lib.h"
#include "nfc_ndef_msg.h"
#include "nfc_text_rec.h"
#include "nfc_uri_msg.h"
#include "nfc_launchapp_msg.h"
#include "nfc_launchapp_rec.h"
#include "nrf_error.h"
#include <string.h>
#include <stdint.h>
#define NRF_LOG_MODULE_NAME "drv_nfc       "
#include "nrf_log.h"
#include "macros_common.h"

#define MAX_REC_COUNT         3                     /**< Maximum number of records. */
#define MAX_NDEF_BUF_LEN    256                     /**< Maximum string length. */
static uint8_t  m_ndef_msg_buf[MAX_NDEF_BUF_LEN];   /**< String to show on NFC. */
static bool     m_initialized   = false;            /**< Is the module initialized */
static bool     m_record_added  = false;            /**< Has a record been added. */

NFC_NDEF_MSG_DEF(m_ndef_msg, MAX_REC_COUNT);

/** @brief Callback function for handling NFC events.
 */
static void nfc_callback(void * p_context, nfc_t2t_event_t event,
                         const uint8_t * p_data, 
                         size_t data_length)
{
    (void)p_context;

    switch (event)
    {
        case NFC_T2T_EVENT_FIELD_ON:
            NRF_LOG_INFO("NFC field on \r\n");
            break;
        case NFC_T2T_EVENT_FIELD_OFF:
            NRF_LOG_INFO("NFC field off \r\n");
            break;
        default:
            break;
    }
}


ret_code_t drv_nfc_string_record_add(char const * const p_string,
                                     uint8_t str_len)
{
    ret_code_t err_code;
    
    if (str_len <= 1)
    {
        return NRF_ERROR_INVALID_LENGTH;
    }
    
    if (!m_initialized)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    
    str_len--; // Removes string termination.
    
    NULL_PARAM_CHECK(p_string);
    
    static const uint8_t en_code[] = {'e', 'n'};

    NFC_NDEF_TEXT_RECORD_DESC_DEF(en_text_rec,
                                  UTF_8,
                                  en_code,
                                  sizeof(en_code),
                                  (uint8_t*)p_string,
                                  str_len);

    err_code = nfc_ndef_msg_record_add(&NFC_NDEF_MSG(m_ndef_msg), &NFC_NDEF_TEXT_RECORD_DESC(en_text_rec));
    RETURN_IF_ERROR(err_code);
                                  
    m_record_added = true;
    
    return NRF_SUCCESS;
}


ret_code_t drv_nfc_uri_record_add(nfc_uri_id_t uri_id, 
                             char const * const p_uri,
                             uint32_t uri_length)
{
    ret_code_t err_code;
    
    if(uri_length <= 1)
    {
        return NRF_ERROR_INVALID_LENGTH;
    }
    
    if( !m_initialized )
    {
        return NRF_ERROR_INVALID_STATE;
    }
    
    uri_length--;   // Removes string termination.
    nfc_ndef_record_desc_t * p_uri_rec;
    
    NULL_PARAM_CHECK(p_uri);
    
    p_uri_rec = nfc_uri_rec_declare(NFC_URI_HTTP_WWW, (uint8_t*)p_uri, uri_length);
    
    err_code = nfc_ndef_msg_record_add(&NFC_NDEF_MSG(m_ndef_msg), p_uri_rec);

    RETURN_IF_ERROR(err_code); 
    
    m_record_added = true;
    
    return NRF_SUCCESS;
}


ret_code_t drv_nfc_app_launch_android_record_add(char const * const p_android_package_name,
                                                 uint32_t package_name_length)
{
    ret_code_t err_code;
    
    if (package_name_length <= 1)
    {
        return NRF_ERROR_INVALID_LENGTH;
    }
    
    if (!m_initialized)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    
    package_name_length--;  // Remove '\0' string termination.
    nfc_ndef_record_desc_t * p_android_rec;
    
    NULL_PARAM_CHECK(p_android_package_name);
    
    p_android_rec = nfc_android_application_rec_declare((uint8_t*)p_android_package_name, package_name_length);

    /* Add Android App Record as second record to message. */
    err_code = nfc_ndef_msg_record_add(&NFC_NDEF_MSG(m_ndef_msg), p_android_rec);
    RETURN_IF_ERROR(err_code);
    
    m_record_added = true;
    
    return NRF_SUCCESS;
}


ret_code_t drv_nfc_enable(void)
{
    ret_code_t err_code;
    
    if (!m_initialized || !m_record_added)
    {
        return NRF_ERROR_INVALID_STATE;
    }
    
    uint32_t ndef_msg_buf_len = MAX_NDEF_BUF_LEN;
    
    NRF_LOG_INFO("NFC total message size %d bytes \r\n", ndef_msg_buf_len);
    
    err_code = nfc_ndef_msg_encode(&NFC_NDEF_MSG(m_ndef_msg),
                                   m_ndef_msg_buf,
                                   &ndef_msg_buf_len);
    RETURN_IF_ERROR(err_code);
    
    NRF_LOG_INFO("NFC used message size %d bytes \r\n", ndef_msg_buf_len);
    
    /* Set created message as the NFC payload. */
    err_code = nfc_t2t_payload_set(m_ndef_msg_buf, ndef_msg_buf_len);
    RETURN_IF_ERROR(err_code);
    
    /* Start sensing NFC field. */
    err_code = nfc_t2t_emulation_start();
    RETURN_IF_ERROR(err_code);

    return NRF_SUCCESS;
}


ret_code_t drv_nfc_disable(void)
{
    ret_code_t err_code;
    
    err_code = nfc_t2t_emulation_stop();
    RETURN_IF_ERROR(err_code);
    
    nfc_ndef_msg_clear(&NFC_NDEF_MSG(m_ndef_msg));
    
    m_initialized  = false;
    m_record_added = false;
    
    return NRF_SUCCESS;
}


ret_code_t drv_nfc_init(void)
{
    ret_code_t err_code;
    
    nfc_ndef_msg_clear(&NFC_NDEF_MSG(m_ndef_msg));
    
    err_code = nfc_t2t_setup(nfc_callback, NULL);
    RETURN_IF_ERROR(err_code);
    
    m_initialized  = true;
    m_record_added = false;
    
    return NRF_SUCCESS;
}
