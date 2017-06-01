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
  allzero.c : Common Floating-Point Library: window an input array and
              compute autocorrelation coefficients

  $Log$
******************************************************************************/

#include "typedef.h"

#define WINSZ 320    /* maximum analysis window size */

void Autocor(
             Float   *r,      /* (o) : Autocorrelations     */
             Float   *x,      /* (i) : Input signal         */
             Float   *window, /* (i) : LPC Analysis window  */
             int     l_window,/* (i) : window length        */
             int     m)       /* (i) : LPC order            */
{
   Float buf[WINSZ];
   Float a0;
   int i, n;

   /* apply analysis window */
   for (n = 0; n < l_window; n++) {
      buf[n] = x[n] * window[n];
   }

   /* compute autocorrealtion coefficients up to lag order */
   for (i = 0; i <= m; i++) {
      a0 = 0.0F;
      for (n = i; n < l_window; n++) {
         a0 += buf[n] * buf [n - i];
      }
      r[i] = a0;
   }
}
