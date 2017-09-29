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



#ifndef _SSI_PAL_MEMMAP_H
#define _SSI_PAL_MEMMAP_H


#ifdef __cplusplus
extern "C"
{
#endif


#include "ssi_pal_types.h"
/*!
* @file
* @brief This file contains functions for memory mapping
*        None of the described functions will check the input parameters so the behavior
*        of the APIs in illegal parameters case is dependent on the operating system behavior.
*
*/


/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/**
 * @brief This function purpose is to return the base virtual address that maps the
 *        base physical address
 *
 * @return Zero on success.
 * @return A non-zero value in case of failure.
 */
uint32_t SaSi_PalMemMap(uint32_t physicalAddress, /*!< [in] Start physical address of the I/O range to be mapped. */
	                uint32_t mapSize,	  /*!< [in] Number of bytes that were mapped. */
		        uint32_t **ppVirtBuffAddr /*!< [out] Pointer to the base virtual address to which the physical pages were mapped. */ );


/**
 * @brief This function purpose is to Unmap a specified address range previously mapped
 *        by SaSi_PalMemMap.
 *
 * @return Zero on success.
 * @return A non-zero value in case of failure.
 */
uint32_t SaSi_PalMemUnMap(uint32_t *pVirtBuffAddr, /*!< [in] Pointer to the base virtual address to which the physical pages were mapped. */
	                  uint32_t mapSize	   /*!< [in] Number of bytes that were mapped. */);

#ifdef __cplusplus
}
#endif

#endif


