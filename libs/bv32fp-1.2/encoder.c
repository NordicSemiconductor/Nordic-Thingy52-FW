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
  encoder.c : BV32 Floating-Point Encoder Main Subroutines

  $Log$
******************************************************************************/

#include <math.h>
#include "typedef.h"
#include "bv32cnst.h"
#include "utility.h"
#include "bvcommon.h"
#include "bv32externs.h"
#include "bv32strct.h"

void Reset_BV32_Coder(struct BV32_Encoder_State *c)
{
   int k;

   Fzero(c->lgpm,LGPORDER);
   c->allast[0] = 1.0;
   Fzero(c->allast+1, LPCO);
   for(k=0; k<LPCO; k++)
      c->lsplast[k] = (Float)(k+1)/(Float)(LPCO+1);
   Fzero(c->lsppm,LPCO*LSPPORDER);
   Fzero(c->x,XOFF);
   Fzero(c->xwd, XDOFF);
   Fzero(c->dq, XOFF);
   Fzero(c->stpem, LPCO);
   Fzero(c->stwpm, LPCO);
   Fzero(c->dfm, DFO);
   Fzero(c->stnfm,LPCO);
   Fzero(c->stsym,LPCO);
   Fzero(c->ltsym,MAXPP1+FRSZ);
   Fzero(c->ltnfm,MAXPP1+FRSZ);
   c->cpplast = 12*cpp_scale;
   Fzero(c->hpfzm,HPO);
   Fzero(c->hpfpm,HPO);
   c->prevlg[0] = MinE;
   c->prevlg[1] = MinE;
   c->lmax = -100.;
   c->lmin = 100.;
   c->lmean = 8.0;
   c->x1 = 13.5;
   c->level = 13.5;
}

void BV32_Encode(
                 struct BV32_Bit_Stream *bs,
                 struct BV32_Encoder_State *cs,
                 short  *inx)
{
   Float	x[LX];
   Float dq[LX];
   Float	xw[FRSZ];
   Float	r[LPCO+1];
   Float a[LPCO+1];
   Float aw[LPCO+1];
   Float	lsp[LPCO];
   Float	lspq[LPCO];
   Float	cbs[VDIM*CBSZ];
   Float qv[SFRSZ];
   Float	bq[3], beta;
   Float	gainq[2], lg, e, ee, ppt;
   Float	lth;
   int	pp, cpp;
   int	i, issf;
   Float *fp0, *fp1;

   /* copy state memory to local memory buffers */
   Fcopy(x, cs->x, XOFF);
   for (i=0;i<FRSZ;i++) x[XOFF+i] = (Float) inx[i];

   /* highpass filtering & pre-emphasis filtering */
   azfilter(hpfb, HPO, x+XOFF, x+XOFF, FRSZ, cs->hpfzm, 1);
   apfilter(hpfa, HPO, x+XOFF, x+XOFF, FRSZ, cs->hpfpm, 1);

   /* copy to coder state */
   Fcopy(cs->x,x+FRSZ,XOFF);

   /* perform lpc analysis with asymmetrical window */
   Autocor(r,x+LX-WINSZ,winl,WINSZ,LPCO);	/* get autocorrelation lags */

   for (i=0;i<=LPCO;i++) r[i]*=sstwin[i];	/* apply spectral smoothing */
   Levinson(r, a,cs->allast,LPCO); 			/* Levinson-Durbin recursion */
   for (i=0;i<=LPCO;i++) a[i] *= bwel[i];

   a2lsp(a, lsp, cs->lsplast);

   lspquan(lspq,bs->lspidx,lsp,cs->lsppm);

   lsp2a(lspq,a);

   /* calculate lpc prediction residual */
   Fcopy(dq,cs->dq,XOFF); 			/* copy dq() state to buffer */
   azfilter(a, LPCO, x+XOFF, dq+XOFF, FRSZ, cs->stpem, 1);

   /* use weighted version of lpc filter as noise feedback filter */
   for (i=0;i<=LPCO; i++) aw[i] = STWAL[i]*a[i];

   /* get perceptually weighted version of speech */
   apfilter(aw, LPCO, dq+XOFF, xw, FRSZ, cs->stwpm, 1);

   /* get the coarse version of pitch period using 8:1 decimation */
   cpp = coarsepitch(xw, cs->xwd, cs->dfm, cs->cpplast);
   cs->cpplast=cpp;

   /* refine the pitch period in the neighborhood of coarse pitch period
   also calculate the pitch predictor tap for single-tap predictor */
   pp = refinepitch(dq, cpp, &ppt);
   bs->ppidx = (pp - MINPP);

   /* vq 3 pitch predictor taps with minimum residual energy */
   bs->bqidx = pitchtapquan(dq, pp, bq);

   /* get coefficients for long-term noise feedback filter */
   if (ppt > 1.0f) beta = LTWFL;
   else if (ppt < 0.0f) beta = 0.0f;
   else beta = LTWFL*ppt;

   /* Loop over excitation sub-frames */
   for(issf=0; issf<NSF; issf++){

      /* calculate pitch prediction residual */
      fp0 = dq + XOFF + issf*SFRSZ;
      fp1 = dq + XOFF + issf*SFRSZ - (pp-2) - 1;
      ee = 0.0;
      for (i=0;i<SFRSZ;i++) {
         e = *fp0++ - bq[0]*fp1[0] - bq[1] * fp1[-1] - bq[2] * fp1[-2];
         fp1++;
         ee += e*e;
      }

      /* log-gain quantization within each sub-frame */
      lg = (ee < TMinE) ? MinE : logf(ee/SFRSZ)/log(2.0);
      bs->gidx[issf] = gainquan(gainq+issf,lg,cs->lgpm,cs->prevlg,cs->level);

      /* Level Estimation */
      lth = estlevel(cs->prevlg[0],&cs->level,&cs->lmax,&cs->lmin,
         &cs->lmean,&cs->x1);
	  (void)lth;

      /* scale the excitation codebook */
      for (i=0;i<(VDIM*CBSZ);i++) cbs[i] = gainq[issf]*cccb[i];


      /* perform noise feedback coding of the excitation signal */
      excquan(qv,bs->qvidx+issf*NVPSSF,dq+XOFF+issf*SFRSZ,
         aw,bq,beta,cs->ltsym,cs->ltnfm,cs->stnfm,cbs,pp);

      /* update quantized short-term prediction residual buffer */
      Fcopy(dq+XOFF+issf*SFRSZ, qv, SFRSZ);
   }

   /* update state memory */
   Fcopy(cs->dq, dq+FRSZ, XOFF);
   Fcopy(cs->lsplast, lspq, LPCO);

}
