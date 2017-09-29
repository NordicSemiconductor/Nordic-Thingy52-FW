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



#ifndef _SSI_PAL_MEM_PLAT_H
#define _SSI_PAL_MEM_PLAT_H


#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>
#include <string.h>
/**
* @brief File Description:
*        This file contains the implementation for memory operations APIs.
*        The functions implementations are generally just wrappers to different operating system calls.
*/


/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/**
 * @brief A wrapper over memcmp functionality. The function compares two given buffers
 *        according to size.
 */
#define _SaSi_PalMemCmp        memcmp

/**
 * @brief A wrapper over memmove functionality, the function copies from one
 *        buffer to another according to given size
 *
 */
#define _SaSi_PalMemCopy       memmove

#define	_SaSi_PalMemMove	      memmove

/**
 * @brief A wrapper over memset functionality, the function sets a buffer with given value
 *        according to size
 *
 */
#define _SaSi_PalMemSet(aTarget, aChar, aSize)        memset(aTarget, aChar, aSize)

/**
 * @brief A wrapper over memset functionality, the function sets a buffer with zeroes
 *        according to size
 *
 */
#define _SaSi_PalMemSetZero(aTarget, aSize)    _SaSi_PalMemSet(aTarget,0x00, aSize)



#ifdef __cplusplus
}
#endif

#endif


