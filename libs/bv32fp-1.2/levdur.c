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
  levdur.c : Common Floating-Point Library: Levinson-Durbin
  $Log$
******************************************************************************/

#include "typedef.h"

/*  Levinson-Durbin recursion */
void Levinson(
              Float	*r,	   /* (i): autocorrelation coefficients         */
              Float	*a,	   /* (o): LPC coefficients                     */
              Float	*old_a,	/* (i/o): LPC coefficients of previous frame */
              int	m)	      /* (i): LPC order                            */
{
   Float alpha;
   Float a0, a1;
   Float rc, *aip, *aib, *alp;
   int mh, minc, ip;

   *a = 1.;
   if (*r <= 0.0f) goto illcond;

   /* start durbin's recursion */
   rc = - *(r+1) / *r;
   *(a+1) = rc;
   alpha = *r + *(r+1) * rc;
   if(alpha <= 0.0f) goto illcond;
   for (minc = 2 ; minc <= m ; minc++ ) {
      a0 = 0.;
      aip = a;
      aib = r + minc;
      for ( ip = 0 ; ip <= minc - 1 ; ip++ )
         a0 = a0 + *aib-- * *aip++;
      rc = -a0 / alpha;
      mh = minc / 2;
      aip = a + 1;
      aib = a + minc - 1;
      for ( ip = 1 ; ip <= mh ; ip++ ) {
         a1 = *aip + rc * *aib;
         *aib = *aib + rc * *aip;
         aib--;
         *aip++ = a1;
      }
      *(a+minc) = rc;
      alpha = alpha + rc * a0;
      if (alpha <= 0.0f) goto illcond;
   }

   aip = a;
   alp = old_a;
   for(ip=0; ip<=m; ip++)
      *alp++ = *aip++;

   return;
illcond:
   aip = a;
   alp = old_a;
   for(ip=0; ip<=m; ip++)
      *aip++ = *alp++;

}
