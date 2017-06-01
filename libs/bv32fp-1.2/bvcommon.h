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
  bvcommon.h : Common #defines and prototypes

  $Log$
******************************************************************************/

#ifndef  BVCOMMON_H
#define  BVCOMMON_H

/* Function Prototypes */


void pp3dec(
short	idx,
Float	*b);

void apfilter(
    Float   *a,     /* (i) a[m+1] prediction coefficients   (m=10)  */
    int     m,      /* (i) LPC order                                */
    Float   *x,     /* (i) input signal                             */
    Float   *y,     /* (o) output signal                            */
    int     lg,     /* (i) size of filtering                        */
    Float   *mem,   /* (i/o) input memory                           */
    short   update);/* (i) flag for memory update                   */

void azfilter(
    Float   *a,     /* (i) prediction coefficients                  */
    int     m,      /* (i) LPC order                                */
    Float   *x,     /* (i) input signal vector                      */
    Float   *y,     /* (o) output signal vector                     */
    int     lg,     /* (i) size of filtering                        */
    Float   *mem,   /* (i/o) filter memory before filtering         */
    short   update); /* (i) flag for memory update                   */

void Autocor(
Float   *r,      /* (o) : Autocorrelations     */
Float   *x,      /* (i) : Input signal         */
Float   *window, /* (i) : LPC Analysis window  */
int     l_window,/* (i) : window length        */
int     m);      /* (i) : LPC order            */

void Levinson(
Float	*r,	   /* (i): autocorrelation coefficients         */
Float	*a,	   /* (o): LPC coefficients                     */
Float	*old_a,	/* (i/o): LPC coefficients of previous frame */
int	m);      /* (i): LPC order                            */

void a2lsp(
Float pc[],       /* (i) input the np+1 predictor coeff.          */
Float lsp[],      /* (o) line spectral pairs                      */
Float old_lsp[]); /* (i/o) old lsp[] (in case not found 10 roots) */

void lsp2a(
Float *lsp,	/* (i) LSP vector       */
Float *a); 	/* (o) LPC coefficients */

void stblz_lsp(Float *lsp, int order);
int stblchck(Float *x, int vdim);

/* LPC to LSP Conversion */
extern	Float grid[];

/* LPC bandwidth expansion */
extern	Float bwel[];

/* LPC WEIGHTING FILTER */
extern	Float	STWAL[];

/* ----- Basic Codec Parameters ----- */
#define LPCO  8 /* LPC Order  */
#define Ngrd 60 /* LPC to LSP Conversion */

#define LSPMIN  0.00150f /* minimum lsp frequency,      6/12 Hz for BV16/BV32 */
#define LSPMAX  0.99775f /* maximum lsp frequency, 3991/7982 Hz for BV16/BV32 */
#define DLSPMIN 0.01250f /* minimum lsp spacing,      50/100 Hz for BV16/BV32 */
#define STBLDIM 3       /* dimension of stability enforcement                */

extern Float pp9cb[];

#endif /* BVCOMMON_H */
