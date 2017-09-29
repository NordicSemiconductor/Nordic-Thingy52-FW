//-----------------------------------------------------------------------------------
//    The confidential and proprietary information contained in this file may 
//    only be used by a person authorised under and to the extent permitted
//    by a subsisting licensing agreement from ARM Limited or its affiliates 
//    or between you and a party authorised by ARM
//
//           (C) COPYRIGHT [2016] ARM Limited or its affiliates
//               ALL RIGHT RESERVED
//
//    This entire notice must be reproduced on all copies of this file
//    and copies of this files may only be made by a person if such person is
//    permitted to do so under the terms of a subsisting license agreement
//    from ARM Limited or its affiliates or between you and a party authorised by ARM
//-----------------------------------------------------------------------------------

#ifndef CRYS_HKDF_H
#define CRYS_HKDF_H

#include "crys_hash.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!
@file
@brief This module defines the API that supports HMAC Key derivation function as defined by RFC5869.
*/

#define CRYS_HKDF_MAX_HASH_KEY_SIZE_IN_BYTES        512


#define CRYS_HKDF_MAX_HASH_DIGEST_SIZE_IN_BYTES     CRYS_HASH_SHA512_DIGEST_SIZE_IN_BYTES

/************************ Defines ******************************/

/************************ Enums ********************************/

typedef enum
{
    CRYS_HKDF_HASH_SHA1_mode      = 0,
    CRYS_HKDF_HASH_SHA224_mode  = 1,
    CRYS_HKDF_HASH_SHA256_mode  = 2,
    CRYS_HKDF_HASH_SHA384_mode  = 3,
    CRYS_HKDF_HASH_SHA512_mode  = 4,

    CRYS_HKDF_HASH_NumOfModes,

    CRYS_HKDF_HASH_OpModeLast    = 0x7FFFFFFF,

}CRYS_HKDF_HASH_OpMode_t;

/************************ Typedefs  ****************************/

/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/

/****************************************************************/


/*********************************************************************************************************/
/**
 * @brief CRYS_HKDF_KeyDerivFunc performs the HMAC-based key derivation, according to RFC5869

  @return CRYSError_t - On success the value CRYS_OK is returned,
            and on failure an ERROR as defined in CRYS_KDF_error.h:
            CRYS_HKDF_INVALID_ARGUMENT_POINTER_ERROR
            CRYS_HKDF_INVALID_ARGUMENT_SIZE_ERROR
            CRYS_HKDF_INVALID_ARGUMENT_HASH_MODE_ERROR
*/
CEXPORT_C CRYSError_t  CRYS_HKDF_KeyDerivFunc(
                        CRYS_HKDF_HASH_OpMode_t HKDFhashMode,   /*!< [in]   The HKDF identifier of hash function to be used. */
                        uint8_t*                Salt_ptr,       /*!< [in]   A pointer to a non secret random value. can be NULL. */
                        size_t	                SaltLen,        /*!< [in]   The size of the salt_ptr. */
                        uint8_t*                Ikm_ptr,        /*!< [in]    A pointer to a input key message. */
                        uint32_t                IkmLen,         /*!< [in]   The size of the input key message */
                        uint8_t*                Info,           /*!< [in]   A pointer to an optional context and application specific information. can be NULL */
                        uint32_t                InfoLen,        /*!< [in]   The size of the info. */
                        uint8_t*                Okm,            /*!< [in]   A pointer to a output key material. */
                        uint32_t                OkmLen,         /*!< [in]   The size of the output key material. */
                        SaSiBool                IsStrongKey    /*!< [in]    if TRUE , then no need to perform the extraction phase. */
                        );

#ifdef __cplusplus
}
#endif

#endif

