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



#ifndef CRYS_AESCCM_ERROR_H
#define CRYS_AESCCM_ERROR_H


#include "crys_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!
@file
@brief This module contains the definitions of the CRYS AESCCM errors.
*/

/************************ Defines ******************************/

/* The CRYS AESCCM module errors.
   CRYS_AESCCM_MODULE_ERROR_BASE = 0x00F01500 */
#define CRYS_AESCCM_INVALID_USER_CONTEXT_POINTER_ERROR     (CRYS_AESCCM_MODULE_ERROR_BASE + 0x00UL)
#define CRYS_AESCCM_ILLEGAL_KEY_SIZE_ERROR                 (CRYS_AESCCM_MODULE_ERROR_BASE + 0x01UL)
#define CRYS_AESCCM_INVALID_KEY_POINTER_ERROR              (CRYS_AESCCM_MODULE_ERROR_BASE + 0x02UL)
#define CRYS_AESCCM_INVALID_ENCRYPT_MODE_ERROR             (CRYS_AESCCM_MODULE_ERROR_BASE + 0x03UL)
#define CRYS_AESCCM_USER_CONTEXT_CORRUPTED_ERROR           (CRYS_AESCCM_MODULE_ERROR_BASE + 0x04UL)
#define CRYS_AESCCM_DATA_IN_POINTER_INVALID_ERROR          (CRYS_AESCCM_MODULE_ERROR_BASE + 0x05UL)
#define CRYS_AESCCM_DATA_OUT_POINTER_INVALID_ERROR         (CRYS_AESCCM_MODULE_ERROR_BASE + 0x06UL)
#define CRYS_AESCCM_DATA_IN_SIZE_ILLEGAL                   (CRYS_AESCCM_MODULE_ERROR_BASE + 0x07UL)
#define CRYS_AESCCM_DATA_OUT_DATA_IN_OVERLAP_ERROR         (CRYS_AESCCM_MODULE_ERROR_BASE + 0x08UL)
#define CRYS_AESCCM_DATA_OUT_SIZE_INVALID_ERROR            (CRYS_AESCCM_MODULE_ERROR_BASE + 0x09UL)
#define CRYS_AESCCM_ADDITIONAL_BLOCK_NOT_PERMITTED_ERROR   (CRYS_AESCCM_MODULE_ERROR_BASE + 0x0AUL)
#define CRYS_AESCCM_ILLEGAL_DMA_BUFF_TYPE_ERROR        	   (CRYS_AESCCM_MODULE_ERROR_BASE + 0x0BUL)
#define CRYS_AESCCM_ILLEGAL_PARAMETER_SIZE_ERROR           (CRYS_AESCCM_MODULE_ERROR_BASE + 0x0CUL)
#define CRYS_AESCCM_ILLEGAL_PARAMETER_PTR_ERROR            (CRYS_AESCCM_MODULE_ERROR_BASE + 0x0DUL)
#define CRYS_AESCCM_ILLEGAL_DATA_TYPE_ERROR                (CRYS_AESCCM_MODULE_ERROR_BASE + 0x0EUL)
#define CRYS_AESCCM_CCM_MAC_INVALID_ERROR                  (CRYS_AESCCM_MODULE_ERROR_BASE + 0x0FUL)
#define CRYS_AESCCM_LAST_BLOCK_NOT_PERMITTED_ERROR         (CRYS_AESCCM_MODULE_ERROR_BASE + 0x10UL)
#define CRYS_AESCCM_ILLEGAL_PARAMETER_ERROR                (CRYS_AESCCM_MODULE_ERROR_BASE + 0x11UL)
#define CRYS_AESCCM_NOT_ALL_ADATA_WAS_PROCESSED_ERROR      (CRYS_AESCCM_MODULE_ERROR_BASE + 0x13UL)
#define CRYS_AESCCM_NOT_ALL_DATA_WAS_PROCESSED_ERROR       (CRYS_AESCCM_MODULE_ERROR_BASE + 0x14UL)
#define CRYS_AESCCM_ADATA_WAS_PROCESSED_ERROR      	   (CRYS_AESCCM_MODULE_ERROR_BASE + 0x15UL)
#define CRYS_AESCCM_ILLEGAL_NONCE_SIZE_ERROR		   (CRYS_AESCCM_MODULE_ERROR_BASE + 0x16UL)
#define CRYS_AESCCM_ILLEGAL_TAG_SIZE_ERROR		   (CRYS_AESCCM_MODULE_ERROR_BASE + 0x17UL)

#define CRYS_AESCCM_CTX_SIZES_ERROR		   	   (CRYS_AESCCM_MODULE_ERROR_BASE + 0x28UL)
#define CRYS_AESCCM_ILLEGAL_PARAMS_ERROR		   (CRYS_AESCCM_MODULE_ERROR_BASE + 0x29UL)
#define CRYS_AESCCM_IS_NOT_SUPPORTED                       (CRYS_AESCCM_MODULE_ERROR_BASE + 0xFFUL)

/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  *****************************/

/************************ Public Variables *********************/

/************************ Public Functions *********************/

#ifdef __cplusplus
}
#endif

#endif


