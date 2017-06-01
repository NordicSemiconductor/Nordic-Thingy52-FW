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
  fineptch.c : Fine pitch search functions.

  $Log$
******************************************************************************/

#include "typedef.h"
#include "bv32cnst.h"

#define  FS	(XOFF+1)	/* Frame Starting index */
#define	 FE	(XOFF+FRSZ)	/* Frame Ending index	*/
#define  DEV	6

int refinepitch(
                Float	*x,
                int		cpp,
                Float	*ppt)
{
   Float	cor, cor2, energy, cormax, cor2max, energymax;
   Float	*fp0, *fp1, *fp2, *fp3;
   int	lb, ub, pp, i, j;

   if (cpp >= MAXPP) cpp = MAXPP-1;
   if (cpp < MINPP) cpp = MINPP;
   lb=cpp-DEV;
   if (lb < MINPP) lb=MINPP; /* lower bound of pitch period search range */
   ub=cpp+DEV;
   /* to avoid selecting MAXPP as the refined pitch period */
   if (ub >= MAXPP) ub=MAXPP-1; /* lower bound of pitch period search range */

   i=lb;				/* start the search from lower bound	    */

   fp0 = x+FS-1;
   fp1 = x+FS-1-i;
   cor = energy = 0.;
   for (j=0;j<(FE-FS+1); j++) {
      energy += (*fp1) * (*fp1);
      cor += (*fp0++) * (*fp1++);
   }

   pp=i;
   cormax=cor;
   cor2max=cor*cor;
   energymax=energy;

   fp0 = x+FE-lb-1;
   fp1 = x+FS-lb-2;
   for (i=lb+1;i<=ub;i++) {
      fp2 = x+FS-1;
      fp3 = x+FS-i-1;
      cor = 0.;
      for (j=0;j<(FE-FS+1);j++) cor += (*fp2++)*(*fp3++);
      cor2 = cor*cor;
      energy += ((*fp1)*(*fp1) - (*fp0)*(*fp0));
      fp0--; fp1--;
      if ((cor2*energymax) > (cor2max*energy)) {
         pp = i;
         cormax = cor;
         cor2max = cor2;
         energymax = energy;
      }
   }

   *ppt = (energymax!=0) ? (cormax/energymax) : 0.0f;

   return pp;
}
