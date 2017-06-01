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
  lsp2a.c : Common Floating-Point Library: conversion from lsp's to a's

  $Log$
******************************************************************************/

#include <math.h>
#include "typedef.h"
#include "bvcommon.h"

#define OR (LPCO+1)	/* Maximum LPC order */
#define PI  ((float)3.14159265358979)

void lsp2a(
           Float *lsp,	/* (i) LSP vector       */
           Float *a) 	/* (o) LPC coefficients */
{
   Float c1, c2, p[OR], q[OR];
   int orderd2, n, i, nor;

   orderd2=LPCO/2;
   for(i = 1; i <= LPCO ; i++)
      p[i] = q[i]= 0.;
   /* Get Q & P polyn. less the (1 +- z-1) ( or (1 +- z-2) ) factor */
   p[0] = q[0] = 1.;
   for(n = 1; n <= orderd2; n++) {
      nor= 2 * n;
      c1 = ((Float)2.) * cosf((Float)PI*lsp[nor-1]);
      c2 = ((Float)2.) * cosf((Float)PI*lsp[nor-2]);
      for(i = nor; i >= 2; i--) {
         q[i] += q[i-2] - c1*q[i-1];
         p[i] += p[i-2] - c2*p[i-1];
      }
      q[1] -= c1;
      p[1] -= c2;
   }
   /* Get the the predictor coeff. */
   a[0] = 1.;
   a[1] = (Float)0.5 * (p[1] + q[1]);
   for(i=1, n=2; i < LPCO ; i++, n++)
      a[n] = (Float)0.5 * (p[i] + p[n] + q[n] - q[i]);
}
