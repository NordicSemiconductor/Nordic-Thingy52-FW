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

/*!
@file
@brief This file contains definitions that are used for the ARM CryptoCell 3xx version of the CryptoCell AES APIs.
*/

#ifndef SSI_AES_DEFS_H
#define SSI_AES_DEFS_H

#include "ssi_pal_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/************************ Defines ******************************/

/*! The size of the user's context prototype (see SaSiAesUserContext_t) in words. */
#define SASI_AES_USER_CTX_SIZE_IN_WORDS (4+4+7+4)

/*! The AES block size in words. */
#define SASI_AES_BLOCK_SIZE_IN_WORDS 4
/*! The AES block size in bytes. */
#define SASI_AES_BLOCK_SIZE_IN_BYTES  (SASI_AES_BLOCK_SIZE_IN_WORDS * sizeof(uint32_t))

/*! The size of the IV buffer in words. */
#define SASI_AES_IV_SIZE_IN_WORDS   SASI_AES_BLOCK_SIZE_IN_WORDS
/*! The size of the IV buffer in bytes. */
#define SASI_AES_IV_SIZE_IN_BYTES  (SASI_AES_IV_SIZE_IN_WORDS * sizeof(uint32_t))

/*! The maximum size of the AES KEY in words. */
#define SASI_AES_KEY_MAX_SIZE_IN_WORDS 4
/*! The maximum size of the AES KEY in bytes. */
#define SASI_AES_KEY_MAX_SIZE_IN_BYTES (SASI_AES_KEY_MAX_SIZE_IN_WORDS * sizeof(uint32_t))


#ifdef __cplusplus
}
#endif

#endif /* #ifndef SSI_AES_DEFS_H */
