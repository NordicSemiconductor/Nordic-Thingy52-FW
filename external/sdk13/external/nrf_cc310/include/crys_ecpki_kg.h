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


#ifndef CRYS_ECPKI_KG_H
#define CRYS_ECPKI_KG_H

/*! @file
@brief Defines the API for generation of ECC private and public keys.
*/


#include "crys_error.h"
#include "crys_rnd.h"
#include "crys_ecpki_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*****************  CRYS_ECPKI_GenKeyPair function   **********************/
/*!
@brief Generates a pair of private and public keys in internal representation according to [ANS X9.62].

@return CRYS_OK on success.
@return A non-zero value on failure as defined crys_ecpki_error.h.
*/
CIMPORT_C CRYSError_t CRYS_ECPKI_GenKeyPair(
                        CRYS_RND_Context_t         *pRndContext,      /*!< [in/out] Pointer to the RND context buffer. */
                        const CRYS_ECPKI_Domain_t  *pDomain,          /*!< [in]  Pointer to EC domain (curve). */
                        CRYS_ECPKI_UserPrivKey_t   *pUserPrivKey,     /*!< [out] Pointer to the private key structure. This structure is used as input to the
										 ECPKI cryptographic primitives. */
                        CRYS_ECPKI_UserPublKey_t   *pUserPublKey,     /*!< [out] Pointer to the public key structure. This structure is used as input to the
										 ECPKI cryptographic primitives. */
			CRYS_ECPKI_KG_TempData_t   *pTempData,        /*!< [in] Temporary buffers for internal use, defined in ::CRYS_ECPKI_KG_TempData_t. */
                        CRYS_ECPKI_KG_FipsContext_t   *pFipsCtx       /*!< [in] Pointer to temporary buffer used in case FIPS certification if required. */
);



#ifdef __cplusplus
}
#endif

#endif




