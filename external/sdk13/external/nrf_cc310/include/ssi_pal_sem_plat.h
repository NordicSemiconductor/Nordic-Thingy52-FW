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



#ifndef _SSI_PAL_SEM_INT__H
#define _SSI_PAL_SEM_INT__H



#ifdef __cplusplus
extern "C"
{
#endif
#include "ssi_pal_types.h"
/**
* @brief File Description:
*        This file contains functions for resource management (semaphor operations).
*        The functions implementations are generally just wrappers to different operating system calls.
*        None of the described functions will check the input parameters so the behavior
*        of the APIs in illegal parameters case is dependent on the operating system behavior.
*
*/

typedef int DX_PAL_SEM;



/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/**
 * @brief This function purpose is to create a semaphore.
 *
 *
 * @param[out] aSemId - Pointer to created semaphor handle
 * @param[in] aInitialVal - Initial semaphore value
 *
 * @return The return values is according to operating system return values.
 */
SaSiError_t _SaSi_PalSemCreate( DX_PAL_SEM *aSemId, uint32_t aInitialVal );

#define _SaSi_PalSemCreate(aSemId,aInitialVal) SASI_SUCCESS
/**
 * @brief This function purpose is to delete a semaphore
 *
 *
 * @param[in] aSemId - Semaphore handle
 *
 * @return The return values is according to operating system return values.
 */
SaSiError_t _SaSi_PalSemDelete( DX_PAL_SEM *aSemId );

#define _SaSi_PalSemDelete( aSemId ) SASI_SUCCESS
/**
 * @brief This function purpose is to Wait for semaphore with aTimeOut. aTimeOut is
 *        specified in milliseconds.
 *
 *
 * @param[in] aSemId - Semaphore handle
 * @param[in] aTimeOut - timeout in mSec, or SASI_INFINITE
 *
 * @return The return values is according to operating system return values.
 */
SaSiError_t _SaSi_PalSemWait(DX_PAL_SEM aSemId, uint32_t aTimeOut);

#define _SaSi_PalSemWait(aSemId, aTimeOut) SASI_SUCCESS
/**
 * @brief This function purpose is to signal the semaphore.
 *
 *
 * @param[in] aSemId - Semaphore handle
 *
 * @return The return values is according to operating system return values.
 */
SaSiError_t _SaSi_PalSemGive(DX_PAL_SEM aSemId);

#define _SaSi_PalSemGive(aSemId) SASI_SUCCESS


#ifdef __cplusplus
}
#endif

#endif


