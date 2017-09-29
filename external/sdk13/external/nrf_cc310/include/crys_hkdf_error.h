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

#ifndef CRYS_HKDF_ERROR_H
#define CRYS_HKDF_ERROR_H

#include "crys_error.h"


#ifdef __cplusplus
extern "C"
{
#endif

/*!
@file
@brief This module contains the definitions of the CRYS HKDF errors.
 */


/************************ Defines *******************************/

/* The CRYS KDF module errors / base address - 0x00F01100*/
#define CRYS_HKDF_INVALID_ARGUMENT_POINTER_ERROR				(CRYS_HKDF_MODULE_ERROR_BASE + 0x0UL)
#define CRYS_HKDF_INVALID_ARGUMENT_SIZE_ERROR					(CRYS_HKDF_MODULE_ERROR_BASE + 0x1UL)
#define CRYS_HKDF_INVALID_ARGUMENT_HASH_MODE_ERROR				(CRYS_HKDF_MODULE_ERROR_BASE + 0x3UL)
#define CRYS_HKDF_IS_NOT_SUPPORTED                              (CRYS_HKDF_MODULE_ERROR_BASE + 0xFFUL)

/************************ Enums *********************************/

/************************ Typedefs  *****************************/

/************************ Structs  ******************************/

/************************ Public Variables **********************/

/************************ Public Functions **********************/




#ifdef __cplusplus
}
#endif

#endif




