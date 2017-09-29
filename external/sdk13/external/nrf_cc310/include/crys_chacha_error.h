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

#ifndef CRYS_CHACHA_ERROR_H
#define CRYS_CHACHA_ERROR_H


#include "crys_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!
@file
@brief This module contains the definitions of the CRYS CHACHA errors.
*/



/************************ Defines ******************************/

/* The CRYS AES module errors base address - 0x00F02200 */
#define CRYS_CHACHA_INVALID_NONCE_ERROR                       	(CRYS_CHACHA_MODULE_ERROR_BASE + 0x01UL)
#define CRYS_CHACHA_ILLEGAL_KEY_SIZE_ERROR                 	(CRYS_CHACHA_MODULE_ERROR_BASE + 0x02UL)
#define CRYS_CHACHA_INVALID_KEY_POINTER_ERROR              	(CRYS_CHACHA_MODULE_ERROR_BASE + 0x03UL)
#define CRYS_CHACHA_INVALID_ENCRYPT_MODE_ERROR             	(CRYS_CHACHA_MODULE_ERROR_BASE + 0x04UL)
#define CRYS_CHACHA_DATA_IN_POINTER_INVALID_ERROR          	(CRYS_CHACHA_MODULE_ERROR_BASE + 0x05UL)
#define CRYS_CHACHA_DATA_OUT_POINTER_INVALID_ERROR         	(CRYS_CHACHA_MODULE_ERROR_BASE + 0x06UL)
#define CRYS_CHACHA_INVALID_USER_CONTEXT_POINTER_ERROR          (CRYS_CHACHA_MODULE_ERROR_BASE + 0x07UL)
#define CRYS_CHACHA_CTX_SIZES_ERROR                             (CRYS_CHACHA_MODULE_ERROR_BASE + 0x08UL)
#define CRYS_CHACHA_INVALID_NONCE_PTR_ERROR                        (CRYS_CHACHA_MODULE_ERROR_BASE + 0x09UL)
#define CRYS_CHACHA_DATA_IN_SIZE_ILLEGAL                        (CRYS_CHACHA_MODULE_ERROR_BASE + 0x0AUL)
#define CRYS_CHACHA_GENERAL_ERROR    		                (CRYS_CHACHA_MODULE_ERROR_BASE + 0x0BUL)
#define CRYS_CHACHA_IS_NOT_SUPPORTED                       	(CRYS_CHACHA_MODULE_ERROR_BASE + 0xFFUL)

/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  *****************************/

/************************ Public Variables *********************/

/************************ Public Functions *********************/

#ifdef __cplusplus
}
#endif

#endif


