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
  bv32externs.h : BV32 Floating-Point externs

  $Log$
******************************************************************************/

/* POINTERS */
extern Float winl[];
extern Float sstwin[];
extern int idxord[];
extern Float hpfa[];
extern Float hpfb[];
extern Float adf[];
extern Float bdf[];
extern Float x[];
extern Float x2[];
extern Float invk[];
extern Float MPTH[];


/* LSP Quantization */
extern Float  lspecb1[LSPECBSZ1*LPCO];
extern Float  lspecb21[LSPECBSZ21*SVD1];
extern Float  lspecb22[LSPECBSZ22*SVD2];
extern Float  lspmean[LPCO];
extern Float  lspp[LSPPORDER*LPCO];

/* Pitch Predictor Codebook */
extern Float pp9cb[];

/* Log-Gain Quantization */
extern Float  lgpecb[LGPECBSZ];
extern Float  lgp[LGPORDER];
extern Float  lgmean;

/* Log-Gain Limitation */
extern Float   lgclimit[];

/* Excitation Codebook */
extern Float  cccb[CBSZ*VDIM];

extern Float lgpecb_nh[];
extern Float a_pre[];
extern Float b_pre[];

/* Function Prototypes */

extern Float   estlevel(
Float   lg,
Float   *level,
Float   *lmax,
Float   *lmin,
Float   *lmean,
Float   *x1);

extern void excdec_w_LT_synth(
	Float   *ltsymd, /* long-term synthesis filter memory at decoder*/
	short   *idx,   /* excitation codebook index array for current subframe */
	Float   *gainq, /* quantized linear gains for sub-subframes */
	Float   *b,     /* coefficient of 3-tap pitch predictor */
	short   pp,     /* pitch period (# of 8 kHz samples) */
   Float   *EE);

extern Float gaindec(
    Float   *lgq,
    short   gidx,
    Float   *lgpm,
    Float   *prevlg,
    Float   level,
    short   *nclglim,
    short   lctimer);

void gainplc(Float E,
	     Float *lgeqm,
	     Float *lgqm);

extern void lspdec(
    Float   *lspq,
    short   *lspidx,
    Float   *lsppm,
	 Float	*lspq_last);

extern	void lspplc(
	Float   *lspq,
	Float   *lsppm);

extern int coarsepitch(
Float	*xw,
Float	*xwd,
Float	*dfm,
int	cpplast);


extern int refinepitch(
Float	*x,
int		cpp,
Float	*ppt);

extern int pitchtapquan(
Float	*x,
int	pp,
Float	*b);

extern void excquan(
    Float   *qv,    /* output quantized excitation signal vector */
    short   *idx,   /* quantizer codebook index for uq[] vector */
    Float   *d,     /* input prediction residual signal vector */
    Float   *h,     /* noise feedback filter coefficient array */
    Float   *b,     /* coefficient of 3-tap pitch predictor */
    Float   beta,   /* coefficient of weighted 3-tap pitch predictor */
    Float   *ltsym, /* long-term synthesis filter memory */
    Float   *ltnfm, /* long-term noise feedback filter memory */
    Float   *stnfm, /* short-term noise feedback filter memory */
    Float   *cb,    /* scalar quantizer codebook */
    int     pp);    /* pitch period (# of 8 kHz samples) */

extern int gainquan(
    Float   *gainq,
    Float   lg,
    Float   *lgpm,
    Float   *prevlg,
    Float   level);

extern void lspquan(
    Float   *lspq,
    short   *lspidx,
    Float   *lsp,
    Float   *lsppm);
