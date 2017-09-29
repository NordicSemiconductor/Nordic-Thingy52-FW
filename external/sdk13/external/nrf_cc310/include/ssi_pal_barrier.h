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


#ifndef _SSI_PAL_BARRIER_H
#define _SSI_PAL_BARRIER_H

/*!
@file
@brief This file contains the definitions and APIs for memory barrier implementation.
* This is a place holder for platform specific memory barrier implementation
* The secure core driver should include a memory barrier before and after the last word of the descriptor
* to allow correct order between the words and different descriptors.
*/

/*!
 * This MACRO is responsible to put the memory barrier after the write operation.
 *
 * @return None
 */

void SaSi_PalWmb(void);

/*!
 * This MACRO is responsible to put the memory barrier before the read operation.
 *
 * @return None
 */
void SaSi_PalRmb(void);


#endif

