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


#ifndef CRYS_ECPKI_DOMAIN_H
#define CRYS_ECPKI_DOMAIN_H


/*!
@file
@brief Defines the ecpki build domain API.
*/


#include "crys_error.h"
#include "crys_ecpki_types.h"

#ifdef __cplusplus
extern "C"
{
#endif




/**********************************************************************************
 *      	      CRYS_ECPKI_GetEcDomain function 			  	  *
 **********************************************************************************/

/*!
 * @brief  The function returns a pointer to an ECDSA saved domain (one of the supported domains).
 *
 * @return Domain pointer on success.
 * @return NULL on failure.
 */

const CRYS_ECPKI_Domain_t *CRYS_ECPKI_GetEcDomain(CRYS_ECPKI_DomainID_t domainId /*!< [in] Index of one of the domain Id (must be one of the supported domains). */);

#ifdef __cplusplus
}
#endif

#endif
