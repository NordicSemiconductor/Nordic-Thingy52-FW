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


#ifndef CRYS_ECPKI_DH_H
#define CRYS_ECPKI_DH_H

/*! @file
@brief Defines the API that supports EC Diffie-Hellman shared secret value derivation primitives.
*/


#include "crys_ecpki_types.h"
#include "crys_ecpki_error.h"

#ifdef __cplusplus
extern "C"
{
#endif

/***********************************************************************
 *		         CRYS_ECDH_SVDP_DH function                    *
 ***********************************************************************/
/*!
@brief Creates the shared secret value according to [IEEE1363, ANS X9.63]:

<ol><li> Checks input-parameter pointers and EC Domain in public and private
keys.</li>
<li> Derives the partner public key and calls the EcWrstDhDeriveSharedSecret
function, which performs EC SVDP operations.</li></ol>
\note The term "User"
refers to any party that calculates a shared secret value using this primitive.
The term "Partner" refers to any other party of shared secret value calculation.
Partner's public key shall be validated before using in this primitive.

@return CRYS_OK on success.
@return A non-zero value on failure as defined crys_ecpki_error.h.
*/
CIMPORT_C CRYSError_t CRYS_ECDH_SVDP_DH(
                        CRYS_ECPKI_UserPublKey_t *PartnerPublKey_ptr,       /*!< [in]  Pointer to a partner public key. */
                        CRYS_ECPKI_UserPrivKey_t *UserPrivKey_ptr,          /*!< [in]  Pointer to a user private key. */
                        uint8_t		         *SharedSecretValue_ptr,    /*!< [out] Pointer to an output buffer that will contain the shared
										       secret value. */
                        uint32_t                 *SharedSecrValSize_ptr,    /*!< [in/out] Pointer to the size of user-passed buffer (in) and
                                                                                          actual size of output of calculated shared secret value
											  (out). */
                        CRYS_ECDH_TempData_t     *TempBuff_ptr              /*!< [in]  Pointer to a temporary buffer. */);

#ifdef __cplusplus
}
#endif

#endif
