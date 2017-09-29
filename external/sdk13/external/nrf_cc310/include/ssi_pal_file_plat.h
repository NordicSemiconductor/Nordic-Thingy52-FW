//-----------------------------------------------------------------------------------
//    The confidential and proprietary information contained in this file may 
//    only be used by a person authorised under and to the extent permitted
//    by a subsisting licensing agreement from ARM Limited or its affiliates 
//    or between you and a party authorised by ARM
//
//           (C) COPYRIGHT [2016] ARM Limited or its affiliates
//               ALL RIGHT RESERVED
//
//    This entire notice must be reproduced on all copies of this file
//    and copies of this files may only be made by a person if such person is
//    permitted to do so under the terms of a subsisting license agreement
//    from ARM Limited or its affiliates or between you and a party authorised by ARM
//-----------------------------------------------------------------------------------



#ifndef _SSI_PAL_FILE_INT_H
#define _SSI_PAL_FILE_INT_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "stdio.h"

/**
* @brief File Description:
*        This file contains wrapper functions for file related operations.
*/

/**** ----- Files General Definitions ----- ****/

/* Definitions for file modes */
#define SASI_PAL_MAX_SIZE_MODE            4
#define SASI_PAL_NUM_OF_SUPPORT_MODES     12

typedef char SaSiPalFileModeStr_t[SASI_PAL_MAX_SIZE_MODE];

typedef SaSiPalFileModeStr_t SaSiPalFileModesTable_t[SASI_PAL_NUM_OF_SUPPORT_MODES];

extern const SaSiPalFileModeStr_t SaSiPalFileModeTable[];
/**** ------------------------------------- ****/

//#define   _SaSiFile_t   FILE

/*----------------------------
      PUBLIC FUNCTIONS
-----------------------------------*/
/**
* @brief A wrapper for fopen functionality (to create a new file, the file is opened for read and
*        write).
*
*/
#define _SaSi_PalFileCreate(aFileName)   SaSi_PalFOpen(aFileName, SASI_PAL_WriteAndRead)

/**
* @brief A wrapper for fopen functionality. SaSiPalFileModeTable contains all possible modes
*        for fopen
*
*/
#define _SaSi_PalFOpen(aFileName, aFileMode)  ((SaSiFile_t)fopen(aFileName, SaSiPalFileModeTable[aFileMode]))

/**
 * @brief A wrapper for fclose functionality.
 *
 */
#define _SaSi_PalFClose(aFileHandle)    fclose((FILE*)aFileHandle)

/**
 * @brief A wrapper for fseek functionality
 *
 */
#define _SaSi_PalFSeek(aFileHandle, aOffset, aSeekOrigin)     fseek((FILE*)aFileHandle, aOffset, aSeekOrigin)

/**
 * @brief A wrapper for ftell functionality
 *
 */
#define _SaSi_PalFTell(aFileHandle)  ftell((FILE*)aFileHandle)

/**
* @brief A wrapper for fread functionality
*
*/
#define _SaSi_PalFRead(aFileHandle, aBuffer, aSize)  fread(aBuffer, 1, aSize, (FILE*)aFileHandle)

/**
* @brief A wrapper for fwrite functionality
*
*/
#define _SaSi_PalFWrite(aFileHandle, aBuffer, aSize)  fwrite(aBuffer, 1, aSize, (FILE*)aFileHandle)
/**
 * @brief A wrapper for fflush functionality
 *
 */
#define _SaSi_PalFFlush(aFileHandle)    fflush((FILE*)aFileHandle)


#ifdef __cplusplus
}
#endif

#endif


