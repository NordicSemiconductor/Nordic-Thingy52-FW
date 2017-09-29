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

#ifndef _SSI_SRAM_MAP_H_
#define _SSI_SRAM_MAP_H_

/*!
@file
@brief This file contains internal SRAM mapping definitions.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#define SASI_SRAM_PKA_BASE_ADDRESS                                0x0
#define SASI_PKA_SRAM_SIZE_IN_KBYTES				  4


#define SASI_SRAM_RND_HW_DMA_ADDRESS                              0x0
#define SASI_SRAM_RND_MAX_SIZE                                    0x800    /*!< Addresses 0K-4KB in SRAM reserved for RND operations - same as the PKA. */

#define SASI_SRAM_MAX_SIZE                                       0x1000    /*!< 4KB in SRAM.  */

#ifdef __cplusplus
}
#endif

#endif /*_SSI_SRAM_MAP_H_*/
