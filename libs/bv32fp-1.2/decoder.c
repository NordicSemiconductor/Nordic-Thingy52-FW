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
  decoder.c : BV32 Floating-Point Decoder Main Subroutines

  $Log$
******************************************************************************/

#include <math.h>
#include "typedef.h"
#include "bv32cnst.h"
#include "bvcommon.h"
#include "bv32strct.h"
#include "bv32externs.h"

#include "utility.h"

void Reset_BV32_Decoder(struct BV32_Decoder_State *c)
{
   int i;

   for (i=0;i<LPCO;i++)
      c->lsplast[i] = (Float)(i+1)/(Float)(LPCO+1);
   Fzero(c->stsym,LPCO);
   Fzero(c->ltsym,LTMOFF);
   Fzero(c->lgpm,LGPORDER);
   Fzero(c->lsppm,LPCO*LSPPORDER);
   Fzero(c->dezfm,PFO);
   Fzero(c->depfm,PFO);
   c->cfecount = 0;
   c->idum=0;
   c->scplcg=1.0;
   c->per=0;
   c->E = 0.0;
   for(i=0; i<LPCO; i++)
      c->atplc[i+1] = 0.0;
   c->pp_last = 100;
   c->prevlg[0] = MinE;
   c->prevlg[1] = MinE;
   c->lgq_last = MinE;
   c->lmax = -100.;
   c->lmin = 100.;
   c->lmean = 8.0;
   c->x1 = 13.5;
   c->level = 13.5;
   c->nclglim=0;
   c->lctimer=0;
}

void BV32_Decode(
                        struct BV32_Bit_Stream     *bs,
                        struct BV32_Decoder_State  *ds,
                        short   *out)
{

   Float	xq[FRSZ];
   Float	ltsym[LTMOFF+FRSZ];
   Float a[LPCO+1];
   Float	lspq[LPCO];
   Float	bq[3];
   Float	gainq[NSF];
   Float	lgq[NSF];
   Float E;
   short pp, i;
   Float bss;

   ds->cfecount = 0; /* reset frame erasure counter */

   /* decode spectral information */
   lspdec(lspq,bs->lspidx,ds->lsppm,ds->lsplast);
   lsp2a(lspq,	a);

   /* decode pitch period & 3 pitch predictor taps */
   pp = (bs->ppidx + MINPP);
   pp3dec(bs->bqidx, bq);

   /* decode excitation gain */
   for (i=0;i<NSF;i++){
      gainq[i] = gaindec(lgq+i,bs->gidx[i],ds->lgpm,
				     ds->prevlg,ds->level,&ds->nclglim,ds->lctimer);

      if(ds->lctimer > 0)
         ds->lctimer = ds->lctimer-1;
      if(ds->nclglim == NCLGLIM_TRAPPED)
         ds->lctimer = LEVEL_CONVERGENCE_TIME;

      /* Level Estimation */
      estlevel(ds->prevlg[0],&ds->level,&ds->lmax,&ds->lmin,
         &ds->lmean,&ds->x1);
   }

   /* copy state memory ltsym[] to local buffer */
   Fcopy(ltsym, ds->ltsym, LTMOFF);

   /* decode the excitation signal */
   excdec_w_LT_synth(ltsym,bs->qvidx,gainq,bq,pp,&E);

   ds->E = E;

   /* lpc synthesis filtering of excitation */
   apfilter(a, LPCO, ltsym+LTMOFF, xq, FRSZ, ds->stsym, 1);

   /* update pitch period of last frame */
   ds->pp_last = pp;

   /* update signal memory */
   Fcopy(ds->ltsym, ltsym+FRSZ, LTMOFF);
   Fcopy(ds->bq_last, bq, 3);

   /* update average quantized log-gain */
   ds->lgq_last = (0.5f)*(lgq[0]+lgq[1]);

   /* de-emphasis filtering */
   azfilter(a_pre, PFO, xq, xq, FRSZ, ds->dezfm, 1);
   apfilter(b_pre, PFO, xq, xq, FRSZ, ds->depfm, 1);

   F2s(out, xq, FRSZ);
   Fcopy(ds->lsplast,lspq,LPCO);

   Fcopy(ds->atplc, a , LPCO+1);
   bss = bq[0]+bq[1]+bq[2];
   if(bss > 1.0f)
      bss = 1.0f;
   else if(bss < 0.0f)
      bss = 0.0f;
   ds->per = (0.5f)*ds->per+(0.5f)*bss;

}
