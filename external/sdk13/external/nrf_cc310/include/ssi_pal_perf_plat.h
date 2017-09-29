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

#ifndef _SSI_PAL_PERF_PLAT_H__
#define _SSI_PAL_PERF_PLAT_H__




typedef unsigned int SaSi_PalPerfData_t;

/**
 * @brief   DSM environment bug - sometimes very long write operation.
 * 	   to overcome this bug we added while to make sure write opeartion is completed
 *
 * @param[in]
 * *
 * @return None
 */
void SaSi_PalDsmWorkarround();


#endif /*_SSI_PAL_PERF_PLAT_H__*/
