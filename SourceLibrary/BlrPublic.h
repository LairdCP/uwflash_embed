// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Copyright (c) 2016, Laird                                                  ++
//                                                                            ++
// Permission to use, copy, modify, and/or distribute this software for any   ++
// purpose with or without fee is hereby granted, provided that the above     ++
// copyright notice and this permission notice appear in all copies.          ++
//                                                                            ++
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES   ++
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF           ++
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR    ++
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES     ++
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN      ++
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR ++
// IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.                ++
//                                                                            ++
// SPDX-License-Identifier:ISC                                                ++
//                                                                            ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                                                                            ++
// Source to embed firmware upgrader functionality into a host system which   ++
// interacts with specific Laird modules. The host can be a microcontroller   ++
// or full blown OS based PC like Windows/Linex/Mac or other.                 ++
//                                                                            ++
// The source requires a C++ compiler and has been used to create utilities   ++
// supplied by Laird.                                                         ++
//                                                                            ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if !defined(_BLRPUBLIC_H)     /* prevent multiple inclusions */
#define _BLRPUBLIC_H

/******************************************************************************/
/* Include Files*/
/******************************************************************************/
#include "UwgPublic.h"

/******************************************************************************/
#if !defined(_TARGETBLR_H)
#error TargetBLR.h not included in Target.h
#endif
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/* Checks */
/******************************************************************************/
#if !defined(BLR_FIRMWARE_BUILD_NUMBER_OFFSET)
#error BLR_FIRMWARE_BUILD_NUMBER_OFFSET has not been defined in Target.h
#endif
#if !defined(BLR_TARGET_PLATFORM_ID)
#error BLR_TARGET_PLATFORM_ID has not been defined in Target.h
#endif

/******************************************************************************/
/* Defines*/
/******************************************************************************/

    /* This value must be == to  flash erased value */
#define BLR_UNALLOCATED_FIRMWARE_NUMBER        (0xFFFF)

#define BLR_FLASHDEVICE_CODE                0x0000
#define BLR_FLASHDEVICE_DATA                0x0100
#define BLR_BANK_ACTIVE_FIRMWARE            -1

/******************************************************************************/
/* Macros*/
/******************************************************************************/

#if defined(UWG_SINGLE_HEAP_MANAGER)
#define BlrGET_HEAP_STATS(s)   UwgPubGetHeapStats( (s));
#else
#define BlrGET_HEAP_STATS(s)   BlrPubGetHeapStats( (s));
#endif

/******************************************************************************/
/* Simple (non struct/union) Typedefs*/
/******************************************************************************/

typedef UI16        TBlrFirmwareNumber;

/******************************************************************************/
/* Enum Typedefs*/
/******************************************************************************/

/*=============================================================================*/
/* This enum collects all module BLR specific result codes */
/*=============================================================================*/
enum
{
    UWRESULTCODE_BLR_SAME_BUILDS         = UWRESULTCODE_START_BLR_MODULE

   /*
   ,UWRESULTCODE_BLR_
   */
};

/******************************************************************************/
/* Forward declaration of Class, Struct & Unions*/
/******************************************************************************/

/******************************************************************************/
/* Class definitions*/
/******************************************************************************/

/******************************************************************************/
/* Struct definitions*/
/******************************************************************************/

typedef struct SBlrSectorMapItemTag
{
    UI32            mNumOfSectors;
    UI32            mSectorSize;
}
SBlrSectorMapItem;

/******************************************************************************/
/* Union definitions*/
/******************************************************************************/

/******************************************************************************/
/* Global Functions (API etc) exported for other modules*/
/******************************************************************************/

/*=============================================================================*/
/* Return the heap usage statistics                                            */
/*=============================================================================*/
#if !defined(UWG_SINGLE_HEAP_MANAGER)
VOID
BlrPubGetHeapStats(
    SUwgHeapStats *psHeapStats
    );
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/*=============================================================================*/
VOID
BlrPubPreInitialise(
    VOID
    );

/*=============================================================================*/
/*=============================================================================*/
VOID
BlrPubPostInitialise(
    VOID
    );

/*=============================================================================*/
/*=============================================================================*/
VOID *
BlrPubOpen(
    UWRESULTCODE *pResultCode
    );

/*=============================================================================*/
/*=============================================================================*/
VOID
BlrPubClose(
    VOID *pContext
    );

/*=============================================================================*/
/*=============================================================================*/
VOID
BlrPubChooseBankInit(
    TBlrFirmwareNumber *pFwOldest
    );

/*=============================================================================*/
/*=============================================================================*/
BOOLEAN
BlrPubChooseBankCompare(
    TBlrFirmwareNumber *pFwOldest,
    TBlrFirmwareNumber nThisNumber
    );

/*=============================================================================*/
/*=============================================================================*/
BOOLEAN
BlrPubCheckPlatform(
    VOID *pBlrContext,
    UI32  nPlatform
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubRegisterDevice(
    VOID *pBlrContext,
    UI8  nFlashDevice,
    UI32 nBaseAddress,
    UI8  nNumOfBanks,
    UI32 nBankSize,
    UI8  nBankSelAlgoFuncId
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubUnregisterDevice(
    VOID *pBlrContext,
    UI8  nFlashDevice
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubAutoSelectBank(
    VOID *pBlrContext,
    UI8  nFlashDevice,
    UI8  *pBank
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubSetBank(
    VOID *pBlrContext,
    UI8  nFlashDevice,
    UI8  nBank
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubWriteBlockHeader(
    VOID *pBlrContext,
    UI32  nOffset,
    UI32  nBlockLenBytes,
    UI32  nFlags
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubWriteBlock(
    VOID *pBlrContext,
    UI8  *pData,
    UI32  nAddrOffset,
    UI32  nBlockLenBytes
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubWriteBlockEnd(
    VOID *pBlrContext
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubEraseBlock(
    VOID *pBlrContext,
    UI32  nAddrOffsetStart,
    UI32  nBlockSize
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubQuery(
    VOID *pBlrContext,
    UI32  nQueryId,
    UI32  *pQueryVals,
    UI16  nNumOfVals
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubSectorMap(
    VOID *pBlrContext,
    SBlrSectorMapItem *pSectorMapArray,
    UI32 nNumOfItems
    );

/*=============================================================================*/
/*=============================================================================*/
UI16
BlrPubGetActiveFwBank(
    VOID
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubReadActiveFirmwareNumber(
    TBlrFirmwareNumber *pFwNum
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubReadFlash(
    UI16    nFlashDevice,
    I16     nBankNumber,    /* -1 == active firmware */
    UI32    nOffset,
    UI8    *pDestBuffer,
    UI16    nBytesToRead
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
BlrPubWriteProgFlash(
    I16     nBankNumber,    /* -1 == active firmware */
    UI32    nAddrOffset,
    UI8    *pSrcBuffer,
    UI16    nBytesToWrite
    );

/*=============================================================================*/
/*=============================================================================*/
UI16
BlrPubProgFlashGets(
    UI32 nOffset,
    UI8 *pLineBuf,
    UI16 nMaxLineLen
    );

/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
