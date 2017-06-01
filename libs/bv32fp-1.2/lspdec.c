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
  lspdec.c :  LSP decoding function

  $Log$
******************************************************************************/

#include <stdio.h>
#include "typedef.h"
#include "bv32cnst.h"
#include "bvcommon.h"
#include "bv32externs.h"

void vqdec(Float *, short, Float *, int);

void lspdec(
            Float   *lspq,
            short   *lspidx,
            Float   *lsppm,
            Float	*lspq_last)
{
   Float elsp[LPCO], lspe[LPCO];
   Float lspeq1[LPCO], lspeq2[LPCO];
   Float a0, *fp1, *fp2;
   int i, k, stbl;

   /* calculate estimated (ma-predicted) lsp vector */
   fp1 = lspp;
   fp2 = lsppm;
   for (i = 0; i < LPCO; i++) {
      a0 = 0.0F;
      for (k = 0; k < LSPPORDER; k++) {
         a0 += *fp1++ * *fp2++;
      }
      elsp[i] = a0;
   }

   /* perform first-stage vq codebook decode */
   vqdec(lspeq1,lspidx[0],lspecb1,LPCO);

   /* perform second-stage vq codebook decode */
   vqdec(lspeq2,lspidx[1],lspecb21,SVD1);
   vqdec(lspeq2+SVD1,lspidx[2],lspecb22,SVD2);

   /* get overall quantizer output vector of the two-stage vq */
   for (i = 0; i < LPCO; i++) {
      lspe[i] = lspeq1[i] + lspeq2[i];
   }

   /* calculate quantized lsp for stability check */
   for (i = 0; i < SVD1; i++) {
      lspq[i] = lspe[i] + elsp[i] + lspmean[i];
   }

   /* detect bit-errors based on ordering property */
   stbl = stblchck(lspq, SVD1);

   /* replace LSP if bit-errors are detected */
   if(!stbl) {
      for(i=0; i<LPCO; i++){
         lspq[i] = lspq_last[i];
         lspe[i] = lspq[i] - elsp[i] - lspmean[i];
      }
   }
   /* calculate remaining quantized LSP for error free case */
   else{
      for (i=SVD1;i<LPCO;i++) {
         lspq[i] = lspe[i] + elsp[i] + lspmean[i];
      }
   }

   /* update lsp ma predictor memory */
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

   /* ensure correct ordering of lsp to guarantee lpc filter stability */
   stblz_lsp(lspq, LPCO);

}


void vqdec(
           Float   *xq,    /* VQ output vector (quantized version of input vector) */
           short   idx,    /* VQ codebook index for the nearest neighbor */
           Float   *cb,    /* VQ codebook */
           int     vdim    /* vector dimension */
           )
{
   int j, k;

   j = idx * vdim;
   for (k = 0; k < vdim; k++) {
      xq[k] = cb[j + k];
   }
}


void lspplc(
            Float   *lspq,
            Float   *lsppm)
{
   Float elsp[LPCO];
   Float a0, *fp1, *fp2;
   int i, k;

   /* calculate estimated (ma-predicted) lsp vector */
   fp1 = lspp;
   fp2 = lsppm;
   for (i = 0; i < LPCO; i++) {
      a0 = 0.0F;
      for (k = 0; k < LSPPORDER; k++) {
         a0 += *fp1++ * *fp2++;
      }
      elsp[i] = a0;
   }

   /* update lsp ma predictor memory */
   i = LPCO * LSPPORDER - 1;
   fp1 = &lsppm[i];
   fp2 = &lsppm[i - 1];
   for (i = LPCO - 1; i >= 0; i--) {
      for (k = LSPPORDER; k > 1; k--) { *fp1-- = *fp2--; }
      *fp1-- = lspq[i] - lspmean[i] - elsp[i];
      fp2--;
   }
}
