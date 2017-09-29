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

#ifndef _SSI_PAL_LOG_PLAT_H_
#define _SSI_PAL_LOG_PLAT_H_

#include "dx_log_mask.h"
#include <stdio.h>

/************** PRINTF rules ******************/
#if defined(DEBUG)

#define PRINTF printf

#else /* Disable all prints */

#define PRINTF(...)  do {} while (0)

#endif

#define __SASI_PAL_LOG_PLAT(level, format, ...) PRINTF(format, ##__VA_ARGS__)

#endif /*_SSI_PAL_LOG_PLAT_H_*/
