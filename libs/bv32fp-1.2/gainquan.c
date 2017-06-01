/*****************************************************************************/
/* BroadVoice(R)32 (BV32) Floating-Point ANSI-C Source Code                  */
/* Revision Date: October 5, 2012                                            */
/* Version 1.2                                                               */
/*****************************************************************************/

/*****************************************************************************/
/* Copyright 2000-2012 Broadcom Corporation                                  */
/*                                                                           */
/* This software is provided under the GNU Lesser General Public License,    */
/* version 2.1, as published by the Free Software Foundation ("LGPL").       */
/* This program is distributed in the hope that it will be useful, but       */
/* WITHOUT ANY SUPPORT OR WARRANTY; without even the implied warranty of     */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the LGPL for     */
/* more details.  A copy of the LGPL is available at                         */
/* http://www.broadcom.com/licenses/LGPLv2.1.php,                            */
/* or by writing to the Free Software Foundation, Inc.,                      */
/* 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 */
/*****************************************************************************/


/*****************************************************************************
  gainquan.c : gain quantization based on inter-subframe
           moving-average prediction of logarithmic gain.

  $Log$
******************************************************************************/

#include <math.h>
#include "typedef.h"
#include "bv32cnst.h"
#include "bvcommon.h"
#include "bv32externs.h"

int gainquan(
             Float   *gainq,
             Float   lg,
             Float   *lgpm,
             Float   *prevlg,        /* previous log gains (last two frames) */
             Float   level           /* input level estimate */
             )
{
   Float elg, lgpe, limit, dmin, d;
   int i, n, gidx=0, *p_gidx;

   /* CALCULATE ESTIMATED LOG-GAIN */
   elg = lgmean;
   for (i = 0; i < LGPORDER; i++) {
      elg += lgp[i] * lgpm[i];
   }

   /* SUBTRACT LOG-GAIN MEAN & ESTIMATED LOG-GAIN TO GET PREDICTION ERROR */
   lgpe = lg -  elg;

   /* SCALAR QUANTIZATION OF LOG-GAIN PREDICTION ERROR */
   dmin = 1e30;
   p_gidx = idxord;
   for (i = 0; i < LGPECBSZ; i++) {
      d = lgpe - lgpecb[*p_gidx++];
      if (d < 0.0F) {
         d = -d;
      }
      if (d < dmin) {
         dmin = d;
         /* index into ordered codebook */
         gidx=i;
      }
   }

   /* CALCULATE QUANTIZED LOG-GAIN */
   *gainq = lgpecb[idxord[gidx]] + elg;

   /* LOOK UP FROM lgclimit() TABLE THE MAXIMUM LOG GAIN CHANGE ALLOWED */
   i = (int) ((prevlg[0] - level - LGLB) * 0.5F); /* get column index */
   if (i >= NGB) {
      i = NGB - 1;
   } else if (i < 0) {
      i = 0;
   }
   n = (int) ((prevlg[0] - prevlg[1] - GCLB) * 0.5F);  /* get row index */
   if (n >= NGCB) {
      n = NGCB - 1;
   } else if (n < 0) {
      n = 0;
   }
   i = i * NGCB + n;

   /* CHECK WHETHER QUANTIZED LOG-GAIN CAUSE A GAIN CHANGE > LGCLIMIT */
   limit = prevlg[0] + lgclimit[i];/* limit that log-gain shouldn't exceed */
   while (*gainq > limit && gidx > 0) { /* if quantized gain exceeds limit */
      gidx -= 1;     /* decrement gain quantizer index by 1 */
      *gainq = lgpecb[idxord[gidx]] + elg; /* use next quantizer output*/
   }
   /* get true codebook index */
   gidx = idxord[gidx];

   /* UPDATE LOG-GAIN PREDICTOR MEMORY */
   prevlg[1] = prevlg[0];
   prevlg[0] = *gainq;
   for (i = LGPORDER - 1; i > 0; i--) {
      lgpm[i] = lgpm[i-1];
   }
   lgpm[0] = lgpecb[gidx];

   /* CONVERT QUANTIZED LOG-GAIN TO LINEAR DOMAIN */
   *gainq = powf(2.0F, 0.5F * *gainq);

   return gidx;
}
