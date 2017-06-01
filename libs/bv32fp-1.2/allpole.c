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
  allpole.c : Common Floating-Point Library: all-pole filter

  $Log$
******************************************************************************/

#include "typedef.h"
#include "bvcommon.h"

#define MAXDIM	160     /* maximum vector dimension */
#define MAXORDER LPCO     /* maximum filter order */

void apfilter(
              Float   *a,     /* (i) a[m+1] prediction coefficients   (m=10)  */
              int     m,      /* (i) LPC order                                */
              Float   *x,     /* (i) input signal                             */
              Float   *y,     /* (o) output signal                            */
              int     lg,     /* (i) size of filtering                        */
              Float   *mem,   /* (i/o) input memory                           */
              short   update) /* (i) flag for memory update                   */
{
   Float buf[MAXORDER+MAXDIM]; /* buffer for filter memory & signal */
   Float a0, *fp1;
   int i, n;

   /* copy filter memory to beginning part of temporary buffer */
   fp1 = &mem[m-1];
   for (i = 0; i < m; i++) {
      buf[i] = *fp1--;    /* this buffer is used to avoid memory shifts */
   }

   /* loop through every element of the current vector */
   for (n = 0; n < lg; n++) {

      /* perform multiply-adds along the delay line of filter */
      fp1 = &buf[n];
      a0 = x[n];
      for (i = m; i > 0; i--) {
         a0 -= *fp1++ * a[i];
      }

      /* update the output & temporary buffer for filter memory */
      y[n] = a0;
      *fp1 = a0;
   }

   /* get the filter memory after filtering the current vector */
   if(update){
      for (i = 0; i < m; i++) {
         mem[i] = *fp1--;
      }
   }
}
