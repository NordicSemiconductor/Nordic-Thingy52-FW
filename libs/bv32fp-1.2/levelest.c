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
  levelest.c : Input level estimation

  $Log$
******************************************************************************/

#include "typedef.h"
#include "bv32cnst.h"

Float	estlevel(
               Float	lg,
               Float	*level,
               Float	*lmax,
               Float	*lmin,
               Float	*lmean,
               Float	*x1)
{
   Float	lth;
   /* UPDATE THE NEW MAXIMUM, MINIMUM, & MEAN OF LOG-GAIN */
   if (lg > *lmax) *lmax=lg;	/* use new log-gain as max if it is > max */
   else *lmax=*lmean+estl_alpha*(*lmax-*lmean); /* o.w. attenuate toward lmean */
   if (lg < *lmin) *lmin=lg;	/* use new log-gain as min if it is < min */
   else *lmin=*lmean+estl_alpha*(*lmin-*lmean); /* o.w. attenuate toward lmean */
   *lmean=estl_beta*(*lmean)+estl_beta1*((0.5f)*(*lmax+*lmin));

  	/* UPDATE ESTIMATED INPUT LEVEL, BY CALCULATING A RUNNING AVERAGE
   (USING AN EXPONENTIAL WINDOW) OF LOG-GAINS EXCEEDING lmean */
   lth=*lmean+estl_TH*(*lmax-*lmean);
   if (lg > lth) {
      *x1=estl_a*(*x1)+estl_a1*lg;
      *level=estl_a*(*level)+estl_a1*(*x1);
   }

   return	lth;

}
