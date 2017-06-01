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
  lspquan.c : Lsp quantization based on inter-frame moving-average
              prediction and two-stage VQ.

  $Log$
******************************************************************************/

#include <stdio.h>
#include "macros_common.h"
#include "typedef.h"
#include "bv32cnst.h"
#include "bvcommon.h"
#include "bv32externs.h"

void vqmse(Float *xq, short *idx, Float *x, Float *cb, int vdim, int cbsz);
void vqwmse_stbl(Float *xq, short *idx, Float *x, Float *w, Float *xa,
                 Float *cb , int vdim, int cbsz);
void vqwmse(Float *xq, short *idx, Float *x, Float *w, Float *cb, int vdim,
            int cbsz);

void lspquan(
             Float   *lspq,
             short   *lspidx,
             Float   *lsp,
             Float   *lsppm
             )
{
   Float d[LPCO], w[LPCO];
   Float elsp[LPCO], lspe[LPCO];
   Float lspeq1[LPCO], lspeq2[LPCO];
   Float lspa[LPCO];
   Float a0, *fp1, *fp2;
   int i, k;

   /* CALCULATE THE WEIGHTS FOR WEIGHTED MEAN-SQUARE ERROR DISTORTION */
   for (i = 0; i < LPCO - 1 ; i++) {
      d[i] = lsp[i+1] - lsp[i];       /* LSP difference vector */
   }
   w[0] = 1.0F / d[0];
   for (i = 1; i < LPCO - 1 ; i++) {
      if (d[i] < d[i-1])
         w[i] = 1.0F / d[i];
      else
         w[i] = 1.0F / d[i-1];
   }
   w[LPCO-1] = 1.0F / d[LPCO-2];

   /* CALCULATE ESTIMATED (MA-PREDICTED) LSP VECTOR */
   fp1 = lspp;
   fp2 = lsppm;
   for (i = 0; i < LPCO; i++) {
      a0 = 0.0F;
      for (k = 0; k < LSPPORDER; k++) {
         a0 += *fp1++ * *fp2++;
      }
      elsp[i] = a0;
   }

   /* SUBTRACT LSP MEAN VALUE & ESTIMATED LSP TO GET PREDICTION ERROR */
   for (i = 0; i < LPCO; i++) {
      lspe[i] = lsp[i] - lspmean[i] - elsp[i];
   }

   /* PERFORM FIRST-STAGE VQ CODEBOOK SEARCH, MSE VQ */
   vqmse(lspeq1,&lspidx[0],lspe,lspecb1,LPCO,LSPECBSZ1);

   /* CALCULATE QUANTIZATION ERROR VECTOR OF FIRST-STAGE VQ */
   for (i = 0; i < LPCO; i++) {
      d[i] = lspe[i] - lspeq1[i];
   }

   /* PERFORM SECOND-STAGE VQ CODEBOOK SEARCH */
   for (i = 0; i < SVD1; i++)
      lspa[i] = lspmean[i] + elsp[i] + lspeq1[i];
   vqwmse_stbl(lspeq2,&lspidx[1],d,w,lspa,lspecb21,SVD1,LSPECBSZ21);
   vqwmse(&lspeq2[SVD1],&lspidx[2],&d[SVD1],&w[SVD1],lspecb22,
      SVD2,LSPECBSZ22);

   /* GET OVERALL QUANTIZER OUTPUT VECTOR OF THE TWO-STAGE VQ */
   for (i = 0; i < LPCO; i++) {
      lspe[i] = lspeq1[i] + lspeq2[i];
   }

   /* UPDATE LSP MA PREDICTOR MEMORY */
   i = LPCO * LSPPORDER - 1;
   fp1 = &lsppm[i];
   fp2 = &lsppm[i - 1];
   for (i = LPCO - 1; i >= 0; i--) {
      for (k = LSPPORDER; k > 1; k--) {
         *fp1-- = *fp2--;
      }
      *fp1-- = lspe[i];
      fp2--;
   }

   /* CALCULATE QUANTIZED LSP */
   for (i = 0; i < LPCO; i++) {
      lspq[i] = lspe[i] + elsp[i] + lspmean[i];
   }

   /* ENSURE CORRECT ORDERING & MINIMUM SPACING TO GUARANTEE STABILITY */
   stblz_lsp(lspq, LPCO);
}

/*==========================================================================*/

/* MSE VQ */
void vqmse(
           Float   *xq,    /* VQ output vector (quantized version of input vector) */
           short   *idx,   /* VQ codebook index for the nearest neighbor */
           Float   *x,     /* input vector */
           Float   *cb,    /* VQ codebook */
           int     vdim,   /* vector dimension */
           int     cbsz    /* codebook size (number of codevectors) */
           )
{
   Float *fp1, dmin, d;
   int j, k;

   Float e;

   fp1 = cb;
   dmin = 1e30;
   for (j = 0; j < cbsz; j++) {
      d = 0.0F;
      for (k = 0; k < vdim; k++) {
         e = x[k] - (*fp1++);
         d += e * e;
      }
      if (d < dmin) {
         dmin = d;
         *idx = j;
      }
   }

   j = *idx * vdim;
   for (k = 0; k < vdim; k++) {
      xq[k] = cb[j + k];
   }
}

/*==========================================================================*/

/* WMSE VQ with enforcement of ordering property */
void vqwmse_stbl(
                 Float   *xq,    /* VQ output vector (quantized version of input vector) */
                 short   *idx,   /* VQ codebook index for the nearest neighbor */
                 Float   *x,     /* input vector */
                 Float   *w,     /* weights for weighted Mean-Square Error */
                 Float   *xa,    /* lsp approximation */
                 Float   *cb,    /* VQ codebook */
                 int     vdim,   /* vector dimension */
                 int     cbsz    /* codebook size (number of codevectors) */
                 )
{
   Float a0, *fp1, *fp2, dmin, d, xqc[LPCO];
   int j, k, stbl;

   fp1 = cb;
   dmin     = 1e30;
   *idx     = -1;
   for (j = 0; j < cbsz; j++) {

      /* check stability */
      fp2  = fp1;
      xqc[0]  = xa[0] + *fp2++;
      if(xqc[0] < 0.0f)
         stbl = 0;
      else
         stbl = 1;
      for(k=1; k<vdim; k++){
         xqc[k]  = xa[k] + *fp2++;
         if(xqc[k] - xqc[k-1] < 0.0f)
            stbl = 0;
      }

      /* calculate distortion */
      d = 0.0F;
      for (k = 0; k < vdim; k++) {
         a0 = x[k] - *fp1++;
         d += w[k] * a0 * a0;
      }

      if(stbl > 0){
         if (d < dmin) {
            dmin = d;
            *idx = j;
         }
      }
   }

   if(*idx == -1){
      DEBUG_PRINTF(0, "BV32 WARNING: Encoder-decoder synchronization lost for clean channel!!!");
      *idx = 1;
   }

   fp1 = cb + (*idx)*vdim;
   for (k = 0; k < vdim; k++) {
      xq[k] = *fp1++;
   }

   return;
}

/*==========================================================================*/

/* MSE VQ */
void vqwmse(
            Float   *xq,    /* VQ output vector (quantized version of input vector) */
            short   *idx,   /* VQ codebook index for the nearest neighbor */
            Float   *x,     /* input vector */
            Float   *w,     /* weights for weighted Mean-Square Error */
            Float   *cb,    /* VQ codebook */
            int     vdim,   /* vector dimension */
            int     cbsz    /* codebook size (number of codevectors) */
            )
{
   Float a0, *fp1, dmin, d;
   int j, k;

   fp1 = cb;
   dmin = 1e30;
   for (j = 0; j < cbsz; j++) {
      d = 0.0F;
      for (k = 0; k < vdim; k++) {
         a0 = x[k] - *fp1++;
         d += w[k] * a0 * a0;
      }
      if (d < dmin) {
         dmin = d;
         *idx = j;
      }
   }

   j = *idx * vdim;
   for (k = 0; k < vdim; k++) {
      xq[k] = cb[j + k];
   }
}
