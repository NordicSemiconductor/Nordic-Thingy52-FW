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
  bitpack.c: BV32 bit packing routines

  $Log$
******************************************************************************/

#include "typedef.h"
#include "bv32cnst.h"
#include "bvcommon.h"
#include "bv32strct.h"
#include "bv32.h"

/***************************************************************************/
/**
*  BV32_BitPack - BroadVoice32 Encoded Bit Pack Function
*
*  This function take the encoded bit structure (27 words) and packed it
*  into a bit stream (10 words) for sending it across the network.
*
*  @param   PackedStream  => (out) pointer to the outgoing encoded stream
*  @param   BitStruct     => (in)  pointer to the encoded bit structure
*
*  @return  Nothing
*
*  @remarks
*
*     The following is the bit table within the bit structure for
*     BroadVoice32
*
*     Word16 bit_table[] = {
*        7, 5, 5,                      // LSP
*        8,                            // Pitch Lag
*        5,                            // Pitch Gain
*        5, 5,                         // Excitation Vector Log-Gain
*        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, // Excitation Vector 1st subframe
*        6, 6, 6, 6, 6, 6, 6, 6, 6, 6  // Excitation Vector 2nd subframe
*      };
*
*
*/

void BV32_BitPack(UWord8 *PackedStream, struct BV32_Bit_Stream *BitStruct)
{
   UWord32 temppack;
   int i;

   /* fill the first 16 bits */
   temppack = ( ((UWord32)BitStruct->lspidx[0]) << 25 );
   temppack |= ( ((UWord32)BitStruct->lspidx[1]) << 20 );
   temppack |= ( ((UWord32)BitStruct->lspidx[2]) << 15 );

   /* store 1st byte in the payload */
   *PackedStream++ = (UWord8)(temppack >> 24);

   /* store 2nd byte in the payload */
   *PackedStream++ = (UWord8)((temppack << 8) >> 24);

   /* clear the upper 16 bits */
   temppack = temppack << 16;

   temppack |= ( ((UWord32)BitStruct->ppidx) << 23 );
   temppack |= ( ((UWord32)BitStruct->bqidx) << 18 );
   temppack |= ( ((UWord32)BitStruct->gidx[0]) << 13 );

   /* store 3rd byte in the payload */
   *PackedStream++ = (UWord8)(temppack >> 24);

   /* store 4th byte in the payload */
   *PackedStream++ = (UWord8)((temppack << 8) >> 24);

   /* clear the upper 16 bits */
   temppack = temppack << 16;

   temppack |= ( ((UWord32)BitStruct->gidx[1]) << 24 );
   temppack |= ( ((UWord32)BitStruct->qvidx[0]) << 18 );
   temppack |= ( ((UWord32)BitStruct->qvidx[1]) << 12 );

   /* store 5th byte in the payload */
   *PackedStream++ = (UWord8)(temppack >> 24);

   /* store 6th byte in the payload */
   *PackedStream++ = (UWord8)((temppack << 8) >> 24);

   /* clear the upper 16 bits */
   temppack = temppack << 16;

   temppack |= ( ((UWord32)BitStruct->qvidx[2]) << 22 );
   temppack |= ( ((UWord32)BitStruct->qvidx[3]) << 16 );

   /* store 7th byte in the payload */
   *PackedStream++ = (UWord8)(temppack >> 24);

   /* store 8th byte in the payload */
   *PackedStream++ = (UWord8)((temppack << 8) >> 24);

   for ( i=0; (i+11) < NVPSF; i += 8 )
   {
      temppack = ( ((UWord32)BitStruct->qvidx[i+4]) << 26 );
      temppack |= ( ((UWord32)BitStruct->qvidx[i+5]) << 20 );
      temppack |= ( ((UWord32)BitStruct->qvidx[i+6]) << 14 );

      /* store MS byte in the payload */
      *PackedStream++ = (UWord8)(temppack >> 24);

      /* store LS byte in the payload */
      *PackedStream++ = (UWord8)((temppack << 8) >> 24);

      /* clear the upper 16 bits */
      temppack = temppack << 16;

      temppack |= ( ((UWord32)BitStruct->qvidx[i+7]) << 24 );
      temppack |= ( ((UWord32)BitStruct->qvidx[i+8]) << 18 );
      temppack |= ( ((UWord32)BitStruct->qvidx[i+9]) << 12 );

      /* store MS byte in the payload */
      *PackedStream++ = (UWord8)(temppack >> 24);

      /* store LS byte in the payload */
      *PackedStream++ = (UWord8)((temppack << 8) >> 24);

      /* clear the upper 16 bits */
      temppack = temppack << 16;

      temppack |= ( ((UWord32)BitStruct->qvidx[i+10]) << 22 );
      temppack |= ( ((UWord32)BitStruct->qvidx[i+11]) << 16 );

      /* store MS byte in the payload */
      *PackedStream++ = (UWord8)(temppack >> 24);

      /* store LS byte in the payload */
      *PackedStream++ = (UWord8)((temppack << 8) >> 24);
   }

   return;
} /* BV32_BitPack */

/***************************************************************************/
/**
*  BV32_BITUNPACK - BroadVoice32 Encoded Bit Unpack Function
*
*  This function take the encoded bit stream (10 words) and unpack it
*  into a bit structure (27 words) for the BroadVoice32 decoder.
*
*  @param   PackedStream  => (in) pointer to the incoming encoded stream
*  @param   BitStruct     => (out)  pointer to the bit structure for decoder
*
*  @return  Nothing
*
*  @remarks
*
*     The following is the bit table within the bit structure for
*     BroadVoice32
*
*     Word16 bit_table[] = {
*        7, 5, 5,                      // LSP
*        8,                            // Pitch Lag
*        5,                            // Pitch Gain
*        5, 5,                         // Excitation Vector Log-Gain
*        6, 6, 6, 6, 6, 6, 6, 6, 6, 6, // Excitation Vector 1st subframe
*        6, 6, 6, 6, 6, 6, 6, 6, 6, 6  // Excitation Vector 2nd subframe
*      };
*/

void BV32_BitUnPack (UWord8 *PackedStream, struct BV32_Bit_Stream * BitStruct )
{
   UWord32 bitword32;
   int i;

   /* unpack bytes 1 and 2 of bit stream */
   bitword32 = (UWord32)*PackedStream++;
   bitword32 = (bitword32 << 8) | (UWord32)*PackedStream++;
   BitStruct->lspidx[0] = (short)( bitword32 >> 9 );
   BitStruct->lspidx[1] = (short)( ( bitword32 & 0x01FF )>> 4 );

   /* unpack bytes 3 and 4 of bit stream */
   bitword32 = ((bitword32 & 0x000F ) << 8) | (UWord32) *PackedStream++;
   bitword32 = (bitword32 << 8) | (UWord32)*PackedStream++;
   BitStruct->lspidx[2] = (short)( bitword32 >> 15 );
   BitStruct->ppidx = (short)( ( bitword32 & 0x7FFF ) >> 7 );
   BitStruct->bqidx = (short)( ( bitword32 & 0x007F ) >> 2 );

   /* unpack bytes 5 and 6 of bit stream */
   bitword32 = ((bitword32 & 0x0003 ) << 8) | (UWord32) *PackedStream++;
   bitword32 = (bitword32 << 8) | (UWord32)*PackedStream++;
   BitStruct->gidx[0] = (short)( bitword32 >> 13 );
   BitStruct->gidx[1] = (short)( ( bitword32 & 0x1FFF ) >> 8 );
   BitStruct->qvidx[0] = (short)( ( bitword32 & 0x00FF ) >> 2 );

   /* unpack bytes 7 and 8 of bit stream */
   bitword32 = ((bitword32 & 0x0003 ) << 8) | (UWord32) *PackedStream++;
   bitword32 = (bitword32 << 8) | (UWord32)*PackedStream++;
   BitStruct->qvidx[1] = (short)( bitword32 >> 12 );
   BitStruct->qvidx[2] = (short)( ( bitword32 & 0x0FFF ) >> 6 );
   BitStruct->qvidx[3] = (short)( bitword32 & 0x003F );

   for ( i=0; (i+11) < NVPSF; i+= 8 )
   {
      /* unpack next MS and LS bytes */
      bitword32 = (UWord32)*PackedStream++;
      bitword32 = (bitword32 << 8) | (UWord32)*PackedStream++;
      BitStruct->qvidx[i+4] = (short)( bitword32 >> 10 );
      BitStruct->qvidx[i+5] = (short)( ( bitword32 & 0x03FF ) >> 4 );

      /* unpack next MS and LS bytes */
      bitword32 = ((bitword32 & 0x000F ) << 8) | (UWord32) *PackedStream++;
      bitword32 = (bitword32 << 8) | (UWord32)*PackedStream++;
      BitStruct->qvidx[i+6] = (short)( bitword32 >> 14 );
      BitStruct->qvidx[i+7] = (short)( ( bitword32 & 0x3FFF ) >> 8 );
      BitStruct->qvidx[i+8] = (short)( ( bitword32 & 0x00FF ) >> 2 );

      /* unpack next MS and LS bytes */
      bitword32 = ((bitword32 & 0x0003 ) << 8) | (UWord32) *PackedStream++;
      bitword32 = (bitword32 << 8) | (UWord32)*PackedStream++;
      BitStruct->qvidx[i+9] = (short)( bitword32 >> 12 );
      BitStruct->qvidx[i+10] = (short)( ( bitword32 & 0x0FFF ) >> 6 );
      BitStruct->qvidx[i+11] = (short)( bitword32 & 0x003F );
   }

   return;
}
