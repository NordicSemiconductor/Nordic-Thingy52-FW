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
  ptquan.c : Quantization of the 3 pitch predictor taps

  $Log$
******************************************************************************/

#include "typedef.h"
#include "bv32cnst.h"
#include "bvcommon.h"

int pitchtapquan(
                 Float	*x,
                 int	pp,
                 Float	*b)
{

   Float	p[9], t, s0, s1, s2, cormax, cor;
   Float	t0, t1, t2;
   Float	*xt, *fp0, *fp1, *fp2;
   int	ppm2, qidx=0, i, j;

   ppm2 = pp-2;
   xt=x + XOFF;

   for (i=0;i<3;i++) {
      fp0 = xt; fp1 = x+XOFF-ppm2-i-1;
      t = 0;
      for (j=0;j<FRSZ;j++) t += (*fp0++) * (*fp1++);
      p[i] = t;
   }

   fp0 = x+XOFF-ppm2-3;
   s0 = *fp0++; s1 = *fp0++; s2 = *fp0--;
   t0 = p[8] = s0*s0; t1 = p[4] = s0*s1; p[5] = s0*s2;
   s0 = *fp0++; s1 = *fp0++; s2 = *fp0--;
   t2 = s0*s0; p[8] += t2; p[4] += s0*s1; p[5] += s0*s2;
   for (i=0;i<(FRSZ-2);i++) {
      s0 = *fp0++; s1 = *fp0++; s2 = *fp0--;
      p[8] += s0*s0;	p[4] += s0*s1; p[5] += s0*s2;
   }
   s0 = *fp0++; s1 = *fp0++; s2 = *fp0--;
   p[7] = p[8] + (s0*s0) - t0;
   p[3] = p[4] + (s0*s1) - t1;
   p[6] = p[7] + (s1*s1) - t2;

   cormax=-1.e30;
   fp0 = pp9cb;
   for (i=0;i<PPCBSZ;i++) {
      cor = 0.;
      fp1 = p;
      for (j=0;j<9;j++) cor += (*fp0++)*(*fp1++);
      if (cor > cormax) {
         cormax = cor; qidx = i;
      }
   }

   fp2 = pp9cb + qidx*9;
   for (i=0;i<3;i++) b[i] = fp2[i]*(0.5f);

   return qidx;
}
