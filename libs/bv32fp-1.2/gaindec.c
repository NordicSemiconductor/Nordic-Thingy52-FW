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
  gaindec.c : gain decoding functions

  $Log$
******************************************************************************/

#include <math.h>
#include "typedef.h"
#include "bv32cnst.h"
#include "bvcommon.h"
#include "bv32externs.h"

Float gaindec(
              Float   *lgq,
              short   gidx,
              Float   *lgpm,
              Float   *prevlg,        /* previous log gains (last two frames) */
              Float   level,           /* input level estimate */
              short   *nclglim,
              short   lctimer
              )
{
   Float gainq, elg, lgc, lgq_nh;
   int i, n, k;

   /* CALCULATE ESTIMATED LOG-GAIN (WITH MEAN VALUE OF LOG GAIN RESTORED) */
   elg = lgmean;
   for (i = 0; i < LGPORDER; i++) {
      elg += lgp[i] * lgpm[i];
   }

   /* CALCULATE DECODED LOG-GAIN */
   *lgq = lgpecb[gidx] + elg;

   /* next higher gain */
   if(gidx < LGPECBSZ-1){
      lgq_nh = lgpecb_nh[gidx] + elg;
      if(*lgq < MinE && fabsf(lgq_nh-MinE) < fabsf(*lgq-MinE)){
         /* To avoid thresholding when the enc Q makes it below the threshold */
         *lgq = MinE;
      }
   }

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

   /* UPDATE LOG-GAIN PREDICTOR MEMORY, CHECK WHETHER DECODED LOG-GAIN EXCEEDS LGCLIMIT */
   for (k = LGPORDER - 1; k > 0; k--) {
      lgpm[k] = lgpm[k-1];
   }
   lgc = *lgq - prevlg[0];
   if (lgc > lgclimit[i] && gidx > 0 && lctimer == 0) { /* if decoded log-gain exceeds limit */
      *lgq = prevlg[0];   /* use the log-gain of previous frame */
      lgpm[0] = *lgq - elg;
      *nclglim = *nclglim+1;
      if(*nclglim > NCLGLIM_TRAPPED)
         *nclglim = NCLGLIM_TRAPPED;
   }
   else{
      lgpm[0] = lgpecb[gidx];
      *nclglim = 0;
   }

   /* UPDATE PREVIOUS LOG-GAINS */
   prevlg[1] = prevlg[0];
   prevlg[0] = *lgq;

   /* CONVERT QUANTIZED LOG-GAIN TO LINEAR DOMAIN */
   gainq = powf(2.0F, 0.5F * *lgq);

   return gainq;
}

void gainplc(Float E,
             Float *lgeqm,
             Float *lgqm)
{
   int k;
   Float pe, lg, mrlg, elg, lge;

   pe = INVSFRSZ * E;

   if(pe - TMinlg > 0.0f)
      lg = logf(pe)/log(2.0f);
   else
      lg = Minlg;

   mrlg = lg - lgmean;

   elg = 0.0;
   for(k=0; k<GPO; k++)
      elg += lgp[k] * lgeqm[k];

   /* predicted log-gain error */
   lge = mrlg - elg;

   /* update quantizer memory */
   for(k=GPO-1; k>0; k--)
      lgeqm[k] = lgeqm[k-1];
   lgeqm[0] = lge;

   /* update quantized log-gain memory */
   lgqm[1] = lgqm[0];
   lgqm[0] = lg;

   return;
}
