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

#ifndef CRYS_SRP_ERROR_H
#define CRYS_SRP_ERROR_H


#include "crys_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!
@file
@brief This module contains the definitions of the CRYS SRP errors.
*/



/************************ Defines ******************************/

/* The CRYS SRP module errors base address - 0x00F02600 */
#define CRYS_SRP_PARAM_INVALID_ERROR               (CRYS_SRP_MODULE_ERROR_BASE + 0x01UL)
#define CRYS_SRP_MOD_SIZE_INVALID_ERROR            (CRYS_SRP_MODULE_ERROR_BASE + 0x02UL)
#define CRYS_SRP_STATE_UNINITIALIZED_ERROR         (CRYS_SRP_MODULE_ERROR_BASE + 0x03UL)
#define CRYS_SRP_RESULT_ERROR         		   (CRYS_SRP_MODULE_ERROR_BASE + 0x04UL)
#define CRYS_SRP_PARAM_ERROR         		   (CRYS_SRP_MODULE_ERROR_BASE + 0x05UL)

/************************ Enums ********************************/

/************************ Typedefs  ****************************/

/************************ Structs  *****************************/

/************************ Public Variables *********************/

/************************ Public Functions *********************/

#ifdef __cplusplus
}
#endif

#endif


