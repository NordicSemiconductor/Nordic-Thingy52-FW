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

#ifndef _SSI_PAL_ABORT_PLAT_H
#define _SSI_PAL_ABORT_PLAT_H

#include "ssi_pal_log.h"
#include <stdlib.h>

#define _SaSi_PalAbort(exp)	do {					\
	SASI_PAL_LOG_ERR("ASSERT:%s:%d: %s", __FILE__, __LINE__, (#exp));	\
	abort();							\
	} while (0)

#endif
