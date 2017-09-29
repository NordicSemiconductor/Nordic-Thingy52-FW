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

#ifndef CRYS_CHACHA_POLY_ERROR_H
#define CRYS_CHACHA_POLY_ERROR_H


#include "crys_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!
@file
@brief This module contains the definitions of the CRYS CHACHA POLY errors.
*/



/************************ Defines ******************************/

/* The CRYS CHACHA POLY module errors base address - 0x00F02400 */
#define CRYS_CHACHA_POLY_ADATA_INVALID_ERROR                  	(CRYS_CHACHA_POLY_MODULE_ERROR_BASE + 0x01UL)
#define CRYS_CHACHA_POLY_DATA_INVALID_ERROR                 	(CRYS_CHACHA_POLY_MODULE_ERROR_BASE + 0x02UL)
#define CRYS_CHACHA_POLY_ENC_MODE_INVALID_ERROR              	(CRYS_CHACHA_POLY_MODULE_ERROR_BASE + 0x03UL)
#define CRYS_CHACHA_POLY_DATA_SIZE_INVALID_ERROR             	(CRYS_CHACHA_POLY_MODULE_ERROR_BASE + 0x04UL)
#define CRYS_CHACHA_POLY_GEN_KEY_ERROR          		(CRYS_CHACHA_POLY_MODULE_ERROR_BASE + 0x05UL)
#define CRYS_CHACHA_POLY_ENCRYPTION_ERROR         		(CRYS_CHACHA_POLY_MODULE_ERROR_BASE + 0x06UL)
#define CRYS_CHACHA_POLY_AUTH_ERROR          			(CRYS_CHACHA_POLY_MODULE_ERROR_BASE + 0x07UL)
#define CRYS_CHACHA_POLY_MAC_ERROR                             	(CRYS_CHACHA_POLY_MODULE_ERROR_BASE + 0x08UL)

/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  *****************************/

/************************ Public Variables *********************/

/************************ Public Functions *********************/

#ifdef __cplusplus
}
#endif

#endif


