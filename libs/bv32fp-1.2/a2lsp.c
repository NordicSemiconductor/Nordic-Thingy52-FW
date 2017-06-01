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

/*-------------- Telecommunications & Signal Processing Lab -------------------
*                             McGill University
* Copyright (c) 1994, P. Kabal
* Portions of this code are provided by P. Kabal and contributors "as
* is" and any express or implied warranties, including, but not limited to,
* the implied warranties of merchantability and fitness for a particular
* purpose are disclaimed. In no event shall the copyright holder or
* contributors be liable for any direct, indirect, incidental, special,
* exemplary, or consequential damages (including, but not limited to,
* procurement of substitute goods or services; loss of use, data, or
* profits; or business interruption) however caused and on any theory of
* liability, whether in contract, strict liability, or tort (including
* negligence or otherwise) arising in any way out of the use of this
* software, even if advised of the possibility of such damage.
*
* Author / revision:
*  P. Kabal
*  $Revision: 1.4 $  $Date: 2002/03/06 17:50:35 $
*  Revised for speechlib library 8/15/94
-----------------------------------------------------------------------------*/

/*****************************************************************************
  a2sp.c : Common Floating-Point Library: conversion from a's to lsp's

  $Log$
******************************************************************************/

#include <stdio.h>
#include <math.h>
#include "macros_common.h"
#include "typedef.h"
#include "bvcommon.h"

#define	PI	(3.14159265358979f)

#define NAB	((LPCO >> 1) + 1)

static Float FNevChebP (Float x, Float *c, int nd2);

#define NBIS            4               /* number of bisections */

/*----------------------------------------------------------------------------
* a2lsp -  Convert predictor coefficients to line spectral pairs
*
* Description:
*  The transfer function of the prediction error filter is formed from the
*  predictor coefficients.  This polynomial is transformed into two reciprocal
*  polynomials having roots on the unit circle. The roots of these polynomials
*  interlace.  It is these roots that determine the line spectral pairs.
*  The two reciprocal polynomials are expressed as series expansions in
*  Chebyshev polynomials with roots in the range -1 to +1.  The inverse cosine
*  of the roots of the Chebyshev polynomial expansion gives the line spectral
*  pairs.  If np line spectral pairs are not found, this routine
*  stops with an error message.  This error occurs if the input coefficients
*  do not give a prediction error filter with minimum phase.
*
*  Line spectral pairs and predictor coefficients are usually expressed
*  algebraically as vectors.
*    lsp[0]     first (lowest frequency) line spectral pair
*    lsp[i]     1 <= i < np
*    pc[0]=1.0  predictor coefficient corresponding to lag 0
*    pc[i]      1 <= 1 <= np
*
* Parameters:
*  ->   Float pc[]
*       Vector of predictor coefficients (Np+1 values).  These are the
*       coefficients of the predictor filter, with pc[0] being the predictor
*       coefficient corresponding to lag 0, and pc[Np] corresponding to lag Np.
*       The predictor coeffs. must correspond to a minimum phase prediction
*       error filter.
*  <-   Float lsp[]
*       Array of Np line spectral pairss (in ascending order).  Each line
*       spectral pair lies in the range 0 to pi.
*  <->  old_lsp[] previous line spectral pairs should it not be able to
*       find all
*----------------------------------------------------------------------------*/

void a2lsp(
           Float pc[],       /* (i) input the np+1 predictor coeff.          */
           Float lsp[],      /* (o) line spectral pairs                      */
           Float old_lsp[]   /* (i/o) old lsp[] (in case not found 10 roots) */
           )
{
   Float fa[NAB], fb[NAB];
   Float ta[NAB], tb[NAB];
   Float *t;
   Float xlow, xmid, xhigh;
   Float ylow, ymid, yhigh;
   Float xroot;
   Float dx;
   int i, j, nf, nd2, nab = ((LPCO>>1) + 1), ngrd;

   fb[0] = fa[0] = 1.0;
   for (i = 1, j = LPCO; i <= (LPCO/2); i++, j--) {
      fa[i] = pc[i] + pc[j] - fa[i-1];
      fb[i] = pc[i] - pc[j] + fb[i-1];
   }

   nd2 = LPCO/2;

   /*
   *   To look for roots on the unit circle, Ga(D) and Gb(D) are evaluated for
   *   D=exp(jw).  Since Gz(D) and Gb(D) are symmetric, they can be expressed in
   *   terms of a series in cos(nw) for D on the unit circle.  Since M is odd and
   *   D=exp(jw)
   *
   *           M-1        n
   *   Ga(D) = SUM fa(n) D             (symmetric, fa(n) = fa(M-1-n))
   *           n=0
   *                                    Mh-1
   *         = exp(j Mh w) [ f1(Mh) + 2 SUM fa(n) cos((Mh-n)w) ]
   *                                    n=0
   *                       Mh
   *         = exp(j Mh w) SUM ta(n) cos(nw) ,
   *                       n=0
   *
   *   where Mh=(M-1)/2=Nc-1.  The Nc=Mh+1 coefficients ta(n) are defined as
   *
   *   ta(n) =   fa(Nc-1) ,    n=0,
   *         = 2 fa(Nc-1-n) ,  n=1,...,Nc-1.
   *   The next step is to identify cos(nw) with the Chebyshev polynomial T(n,x).
   *   The Chebyshev polynomials satisfy the relationship T(n,cos(w)) = cos(nw).
   *   Omitting the exponential delay term, the series expansion in terms of
   *   Chebyshev polynomials is
   *
   *           Nc-1
   *   Ta(x) = SUM ta(n) T(n,x)
   *           n=0
   *
   *   The domain of Ta(x) is -1 < x < +1.  For a given root of Ta(x), say x0,
   *   the corresponding position of the root of Fa(D) on the unit circle is
   *   exp(j arccos(x0)).
   */
   ta[0] = fa[nab-1];
   tb[0] = fb[nab-1];
   for (i = 1, j = nab - 2; i < nab; ++i, --j) {
      ta[i] = (2.0f) * fa[j];
      tb[i] = (2.0f) * fb[j];
   }

   /*
   *   To find the roots, we sample the polynomials Ta(x) and Tb(x) looking for
   *   sign changes.  An interval containing a root is successively bisected to
   *   narrow the interval and then linear interpolation is used to estimate the
   *   root.  For a given root at x0, the line spectral pair is w0=acos(x0).
   *
   *   Since the roots of the two polynomials interlace, the search for roots
   *   alternates between the polynomials Ta(x) and Tb(x).  The sampling interval
   *   must be small enough to avoid having two cancelling sign changes in the
   *   same interval.  The sampling (grid) points were trained from a large amount
   *   of LSP vectors derived with high accuracy and stored in a table.
   */

   nf = 0;
   t = ta;
   xroot = 2.0f;
   ngrd = 0;
   xlow = grid[0];
   ylow = FNevChebP(xlow, t, nd2);


   /* Root search loop */
   while (ngrd<(Ngrd-1) && nf < LPCO) {

      /* New trial point */
      ngrd++;
      xhigh = xlow;
      yhigh = ylow;
      xlow = grid[ngrd];
      ylow = FNevChebP(xlow, t, nd2);

      if (ylow * yhigh <= 0.0f) {

         /* Bisections of the interval containing a sign change */
         dx = xhigh - xlow;
         for (i = 1; i <= NBIS; ++i) {
            dx = (0.5f) * dx;
            xmid = xlow + dx;
            ymid = FNevChebP(xmid, t, nd2);
            if (ylow * ymid <= 0.0f) {
               yhigh = ymid;
               xhigh = xmid;
            } else {
               ylow = ymid;
               xlow = xmid;
            }
         }

         /*
         * Linear interpolation in the subinterval with a sign change
         * (take care if yhigh=ylow=0)
         */
         if (yhigh != ylow)
            xmid = xlow + dx * ylow / (ylow - yhigh);
         else
            xmid = xlow + dx;

         /* New root position */
         lsp[nf] = acosf(xmid)/PI;
         ++nf;

         /* Start the search for the roots of the next polynomial at the estimated
         * location of the root just found.  We have to catch the case that the
         * two polynomials have roots at the same place to avoid getting stuck at
         * that root.
         */
         if (xmid >= xroot) {
            xmid = xlow - dx;
         }
         xroot = xmid;
         if (t == ta)
            t = tb;
         else
            t = ta;
         xlow = xmid;
         ylow = FNevChebP(xlow, t, nd2);
      }
   }

   /* if LPCO roots have not been found */
   if (nf != LPCO) {
      DEBUG_PRINTF(0, "BV32: WARNING: a2lsp failed to find all lsp nf=%d LPCO=%d", nf, LPCO);
      for(i=0; i<LPCO; i++)
         lsp[i] = old_lsp[i];
   }
   /* else update LSP of previous frame with the new LSP */
   else{
      for(i=0; i<LPCO; i++)
         old_lsp[i] = lsp[i];
   }

   return;
}

/*-------------------------------------------------------------------------
*
* Module:
*  Float FNevChebP(Float x,  Float c[], int N)
*
* Purpose:
*  Evaluate a series expansion in Chebyshev polynomials
*
* Description:
*  The series expansion in Chebyshev polynomials is defined as
*
*              N-1
*       Y(x) = SUM c(i) T(i,x) ,
*              i=0
*
*  where N is the order of the expansion, c(i) is the coefficient for the i'th
*  Chebyshev polynomial, and T(i,x) is the i'th order Chebyshev polynomial
*  evaluated at x.
*
*  The Chebyshev polynomials satisfy the recursion
*    T(i,x) = 2x T(i-1,x) - T(i-2,x),
*  with the initial conditions T(0,x)=1 and T(1,x)=x.  This routine evaluates
*  the expansion using a backward recursion.
*
* Parameters:
*  <-  Float
*  FNevChebP -  Resulting value
*  ->  Float
*  x -          Input value
*  ->  Float []
*  c -          Array of coefficient values.  c[i] is the coefficient of the
*               i'th order Chebyshev polynomial.
*  ->  int
*  N -          Number of coefficients
*
-------------------------------------------------------------------------*/

/*****************
*   Consider the backward recursion
*     b(i,x) = 2xb(i+1,x) - b(i+2,x) + c(i),
*   with initial conditions b(n,x)=0 and b(n+1,x)=0.  Then dropping the
*   dependence on x,
*     c(i) = b(i) - 2xb(i+1) + b(i+2).
*
*          n-1
*   Y(x) = SUM c(i) T(i)
*          i=0
*
*          n-1
*        = SUM [b(i)-2xb(i+1)+b(i+2)] T(i)
*          i=0
*                                             n-1
*        = b(0)T(0) + b(1)T(1) - 2xb(1)T(0) + SUM b(i) [T(i)-2xT(i-1)+T(i-2)]
*                                             i=2
* The term inside the sum is zero because of the recursive relationship
* satisfied by the Chebyshev polynomials.  Then substituting the values T(0)=1
* and T(1)=x, Y(x) is expressed in terms of the diff. between b(0) and b(2)
* (errors in b(0) and b(2) tend to cancel),
*   Y(x) = b(0)-xb(1) = [b(0)-b(2)+c(0)] / 2
*******************/

Float	FNevChebP(	/* (o) result 	*/
                Float	x,			/* (i) value 	*/
                Float	*c,			/* (i) coefficient array */
                int		nd2)
{
   Float	t;
   Float	b[NAB];
   int	i;

   t = x*2;
   b[0] = c[nd2];
   b[1] = c[nd2-1] + t * b[0];
   for (i=2;i<nd2;i++)
      b[i] = c[nd2-i] - b[i-2] + t * b[i-1];
   return (c[0] - b[nd2-2] + x * b[nd2-1]);
}
