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
  utility.c : Common Floating-Point Library: utilities

  $Log$
******************************************************************************/

#include "typedef.h"

void Fcopy(Float *y, Float *x, int size)
{
   while ((size--)>0) *y++ = *x++;
}

void Fzero(Float *x, int size)
{
   while ((size--)>0) *x++ = 0.;
}

void F2s(short *s, Float *f, int size)
{
   Float	t;
   short v;
   int   i;
   for (i=0;i<size;i++)  {
      t = *f++;

      /* rounding */
      if (t >= 0) t += 0.5f;
      else t -= 0.5f;

      if (t>32767) v = 32767;
      else if (t<-32768.) v = -32768;
      else v = (short) t;
      *s++ = v;
   }
}
