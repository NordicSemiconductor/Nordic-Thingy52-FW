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


#ifndef _SSI_PAL_MUTEX_H
#define _SSI_PAL_MUTEX_H

#include "ssi_pal_mutex_plat.h"
#include "ssi_pal_types_plat.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!
* @file
* @brief This file contains functions for resource management (mutex operations).
*        The functions implementations are generally just wrappers to different operating system calls.
*        None of the described functions will check the input parameters so the behavior
*        of the APIs in illegal parameters case is dependent on the operating system behavior.
*
*/




/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/

/**
 * @brief This function purpose is to create a mutex.
 *
 *
 * @return Zero on success.
 * @return A non-zero value on failure.
 */
SaSiError_t SaSi_PalMutexCreate(SaSi_PalMutex *pMutexId /*!< [out] Pointer to created mutex handle. */);


/**
 * @brief This function purpose is to destroy a mutex.
 *
 *
 * @return Zero on success.
 * @return A non-zero value on failure.
 */
SaSiError_t SaSi_PalMutexDestroy(SaSi_PalMutex *pMutexId /*!< [in] Pointer to mutex handle. */);


/**
 * @brief This function purpose is to Wait for Mutex with aTimeOut. aTimeOut is
 *        specified in milliseconds (SASI_INFINITE is blocking).
 *
 * @return Zero on success.
 * @return A non-zero value on failure.
 */
SaSiError_t SaSi_PalMutexLock (SaSi_PalMutex *pMutexId, /*!< [in] Pointer to Mutex handle. */
			       uint32_t aTimeOut	/*!< [in] Timeout in mSec, or SASI_INFINITE. */);


/**
 * @brief This function purpose is to release the mutex.
 *
 * @return Zero on success.
 * @return A non-zero value on failure.
 */
SaSiError_t SaSi_PalMutexUnlock (SaSi_PalMutex *pMutexId/*!< [in] Pointer to Mutex handle. */);





#ifdef __cplusplus
}
#endif

#endif


