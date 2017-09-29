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

/*! @file
@brief This file contains basic type definitions that are platform dependent.
*/
#ifndef SSI_PAL_TYPES_PLAT_H
#define SSI_PAL_TYPES_PLAT_H
/* Host specific types for standard (ISO-C99) compilant platforms */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uintptr_t	    SaSiVirtAddr_t;
typedef uint32_t            SaSiBool_t;
typedef uint32_t            SaSiStatus;

#define SaSiError_t         SaSiStatus
#define SASI_INFINITE       0xFFFFFFFF

#define CEXPORT_C
#define CIMPORT_C

#endif /*SSI_PAL_TYPES_PLAT_H*/
