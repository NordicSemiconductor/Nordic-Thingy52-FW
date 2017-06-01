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
  plc.c : Packet Loss Concealment

  $Log$
******************************************************************************/

#include <math.h>
#include "typedef.h"
#include "bv32cnst.h"
#include "bvcommon.h"
#include "bv32strct.h"
#include "bv32externs.h"

#include "utility.h"

extern void BV32_PLC(
                     struct  BV32_Decoder_State   *ds,
                     short   *out)
{
   int n, i_sf;
   Float r[SFRSZ];        /* random excitation                       */
   Float E, gain;
   Float tmp;
   Float xq[SFRSZ];
   Float	d[LTMOFF+FRSZ];	/* long-term synthesis filter memory */

   /************************************************************/
   /*                 Copy decoder state memory                */
   /************************************************************/
   Fcopy(d, ds->ltsym, LTMOFF); /* excitation */

   /************************************************************/
   /*        Update counter of consecutive list frames         */
   /************************************************************/
   if(ds->cfecount < HoldPLCG+AttnPLCG-1)
      ds->cfecount++;

   /* loop over subframes */
   for(i_sf=0; i_sf<FECNSF; i_sf++){

      /************************************************************/
      /*                Generate Unscaled Excitation              */
      /************************************************************/
      E = 0.0;
      for(n=0; n<SFRSZ; n++){
         ds->idum = 1664525L*ds->idum + 1013904223L;
         r[n] = (Float)(ds->idum >> 16) - 32767.0f;
         E += r[n] * r[n];
      }


      /************************************************************/
      /*                      Calculate Scaling                   */
      /************************************************************/
      ds->scplcg = ScPLCG_a + ScPLCG_b * ds->per;
      if( ds->scplcg > ScPLCGmax)
         ds->scplcg = ScPLCGmax;
      else if(ds->scplcg < ScPLCGmin)
         ds->scplcg = ScPLCGmin;
      gain = ds->scplcg * sqrtf(ds->E / E);

      /************************************************************/
      /*                  Long-term synthesis filter              */
      /************************************************************/
      for(n=0; n<SFRSZ; n++){
         d[LTMOFF+i_sf*SFRSZ+n] = gain * r[n];
         d[LTMOFF+i_sf*SFRSZ+n] += ds->bq_last[0] * d[LTMOFF+i_sf*SFRSZ+n-ds->pp_last+1];
         d[LTMOFF+i_sf*SFRSZ+n] += ds->bq_last[1] * d[LTMOFF+i_sf*SFRSZ+n-ds->pp_last];
         d[LTMOFF+i_sf*SFRSZ+n] += ds->bq_last[2] * d[LTMOFF+i_sf*SFRSZ+n-ds->pp_last-1];
      }


      /************************************************************/
      /*                Short-term synthesis filter               */
      /************************************************************/
      apfilter(ds->atplc, LPCO, d+i_sf*SFRSZ+LTMOFF, xq, SFRSZ, ds->stsym, 1);

      /**********************************************************/
      /*                    De-emphasis filter                  */
      /**********************************************************/
      for(n=0; n<SFRSZ; n++){
         tmp = xq[n] + PEAPFC * ds->dezfm[0] -PEAZFC * ds->depfm[0];
         ds->dezfm[0] = xq[n];
         ds->depfm[0] = tmp;
         if (tmp>=0) tmp += 0.5f;
         else tmp -= 0.5f;

         if (tmp>32767.0f) tmp = 32767.0f;
         else if (tmp<-32768.0f) tmp = -32768.0f;
         out[i_sf*SFRSZ+n] = (short)tmp;
      }

      /************************************************************/
      /*        Update memory of predictive gain quantizer        */
      /************************************************************/
      gainplc(ds->E, ds->lgpm, ds->prevlg);

      /************************************************************/
      /*                  Signal level estimation                 */
      /************************************************************/
      estlevel(ds->prevlg[0],&ds->level,&ds->lmax,&ds->lmin,
         &ds->lmean,&ds->x1);
   }

   /************************************************************/
   /*                 Save decoder state memory                */
   /************************************************************/
   Fcopy(ds->ltsym, d+FRSZ, LTMOFF);

   /************************************************************/
   /*        Update memory of predictive LSP quantizer         */
   /************************************************************/
   lspplc(ds->lsplast,ds->lsppm);

   /************************************************************/
   /*          Attenuation during long packet losses           */
   /************************************************************/
   if(ds->cfecount >= HoldPLCG){
      gain = 1.0f -AttnFacPLCG * (Float)(ds->cfecount - (HoldPLCG-1));
      ds->bq_last[0] = gain * ds->bq_last[0];
      ds->bq_last[1] = gain * ds->bq_last[1];
      ds->bq_last[2] = gain * ds->bq_last[2];
      ds->E = gain * gain * ds->E;
   }

   return;
}
