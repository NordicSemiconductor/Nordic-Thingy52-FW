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



#ifndef _SSI_PAL_STRING_INT_H
#define _SSI_PAL_STRING_INT_H


#ifdef __cplusplus
extern "C"
{
#endif

#include <string.h>
/**
* @brief File Description:
*        This file contains wrapper functions for string manipulation.
*/


/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/**
 * @brief A wrapper function for strlen functionality. The function returns the size of a given string
 *
 */
#define  _SaSi_PalStrLen     strlen

/**
 * @brief A wrapper function for strstr functionality. The functions find a string in a string and
 *        return a pointer to it.
 *
 */
#define _SaSi_PalFindStr     strstr

/**
 * @brief A wrapper function for strchr functionality. The function finds a char in a given string.
 *
 */
#define _SaSi_PalStrChr      strchr

/**
 * @brief A wrapper function for strrchr functionality. The function finds a char inside a string
 *        (from the end) and returns a pointer to it
 *
 */
#define _SaSi_PalStrRChr     strrchr

/**
 * @brief A wrapper for strncpy functionality. The function copies a string.
 *
 */
#define _SaSi_PalStrNCopy    strncpy


#ifdef __cplusplus
}
#endif

#endif


