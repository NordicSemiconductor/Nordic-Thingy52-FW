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


#ifndef _SSI_PAL_INIT_H
#define _SSI_PAL_INIT_H

#include "ssi_pal_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*!
@file
@brief This file contains the PAL layer entry point, it includes the definitions and APIs for PAL initialization and termination.
*/

/**
 * @brief This function Performs all initializations that may be required by the customer's PAL implementation, specifically by the DMA-able buffer
 *	scheme. The existing implementation allocates a contiguous memory pool that is later used by the ARM TrustZone CryptoCell TEE implementation.
 * 	In case no initializations are needed in the customer's environment, the function can be minimized to return OK.
 *      It is called by ::SaSi_LibInit.
 *
 * @return A non-zero value in case of failure.
 */
int SaSi_PalInit(void);



/**
 * @brief This function is used to terminate the PAL implementation and free the resources that were taken by ::SaSi_PalInit.
 *
 * @return Void.
 */
void SaSi_PalTerminate(void);



#ifdef __cplusplus
}
#endif

#endif


