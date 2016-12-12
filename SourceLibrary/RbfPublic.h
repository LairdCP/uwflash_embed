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

#if !defined(_RBFPUBLIC_H)     /* prevent multiple inclusions */
#define _RBFPUBLIC_H

/******************************************************************************/
/* Include Files*/
/******************************************************************************/
#include "UwgPublic.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#if !defined(_TARGETRBF_H)
#error TargetRbf.h not included in Target.h
#endif
/******************************************************************************/
/* Defines*/
/******************************************************************************/

/******************************************************************************/
/* Macros*/
/******************************************************************************/

#if defined(UWG_SINGLE_HEAP_MANAGER)
#define RbfGET_HEAP_STATS(s)   UwgPubGetHeapStats( (s));
#else
#define RbfGET_HEAP_STATS(s)   RbfPubGetHeapStats( (s));
#endif

#define RbfVALID_RING_BUFFER(x) DEVASSERT3( !((x->mBuffer[1].mnLen!=0)&&(x->mBuffer[0].mnLen==0)) )

/******************************************************************************/
/* Simple (non struct/union) Typedefs*/
/******************************************************************************/

/******************************************************************************/
/* Enum Typedefs*/
/******************************************************************************/

/******************************************************************************/
/* Forward declaration of Class, Struct & Unions*/
/******************************************************************************/

/******************************************************************************/
/* Class definitions*/
/******************************************************************************/

/******************************************************************************/
/* Struct definitions*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
typedef struct SRbfBufferTag
{
    union
    {
        CHAR  *mpcData;         /* Points to the data - char*/
        UI8   *mpuData;         /* Points to the data - ui8 */
    } u ;
    UI16   mnLen;               /* number of octets in the object */
}
SRbfBuffer;

/*----------------------------------------------------------------------------*/
/*
    The following struct can totally define all the data available
    in a ring buffer
*/
typedef struct SRbfRingBufferTag
{
    SRbfBuffer mBuffer[2];
}
SRbfRingBuffer;

/******************************************************************************/
/* Union definitions*/
/******************************************************************************/

/******************************************************************************/
/* Global Functions (API etc) exported for other modules on PC Harness only   */
/******************************************************************************/

/*=============================================================================*/
/* Uses DEVTRACE to display the content of the ring buffer                     */
/*=============================================================================*/
#if defined(DEVTRACE)
VOID
RbfPubRingBufferPrintf(
    SRbfRingBuffer *pRingBuf
    );
#endif

/******************************************************************************/
/* Global Functions (API etc) exported for other modules*/
/******************************************************************************/

/*=============================================================================*/
/* Return the heap usage statistics                                            */
/*=============================================================================*/
#if !defined(UWG_SINGLE_HEAP_MANAGER)
VOID
RbfPubGetHeapStats(
    SUwgHeapStats *psHeapStats
    );
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/*=============================================================================*/
VOID RbfPubPreInitialise(VOID);

/*=============================================================================*/
/*=============================================================================*/
VOID RbfPubPostInitialise(VOID);

/*=============================================================================*/
/*=============================================================================*/
SRbfBuffer *
RbfPubTextSet(
    SRbfBuffer *pUwgText, /* Pointer to object */
    CHAR       *pData,    /* Points to the data */
    UI16       nLen      /* String is NOT null terminated, but length specified */
    );

/*=============================================================================*/
/*=============================================================================*/
SRbfBuffer *
RbfPubTextClear(
    SRbfBuffer *pText    /* Pointer to object */
    );

/*=============================================================================*/
/* sets both buffers to empty */
/*=============================================================================*/
VOID
RbfPubClearRingBuffer(
    SRbfRingBuffer *pRbfRingBuffer    /* Pointer to object */
    );

/*=============================================================================*/
/* Initialise a ring buffer with a len terminated char string */
/*=============================================================================*/
VOID
RbfPubSetRingBuffer(
    SRbfRingBuffer *pRbfRingBuffer,    /* Pointer to object */
    CHAR *pString,
    UI16 nLen
    );

/*=============================================================================*/
/* Initialise a ring buffer with a null terminated char string */
/*=============================================================================*/
VOID
RbfPubSetRingBufferWithChar(
    SRbfRingBuffer *pRbfRingBuffer,    /* Pointer to object */
    CHAR *pString
    );

/*=============================================================================*/
/* returns the total length of the data in two buffers                         */
/*=============================================================================*/
UI16
RbfPubGetDataLen(
    SRbfRingBuffer *pRingBuf
    );

/*=============================================================================*/
/* Returns true if the data in pRingBuf matches that in pSrc                   */
/* Case Sensitive                                                              */
/*=============================================================================*/
BOOLEAN
RbfPubRingBufCmpWithChar(
    SRbfRingBuffer *pRingBuf,
    CONST CHAR *pSrc
    );

/*=============================================================================*/
/* Returns true if the data in pRingBuf matches that in pSrc                   */
/* Case Insensitive                                                            */
/*=============================================================================*/
BOOLEAN
RbfPubRingBufCmpiWithChar(
    SRbfRingBuffer *pRingBuf,
    CONST CHAR *pSrc
    );

/*=============================================================================*/
/* Returns true if the data in pRingBuf matches that in pSrc                   */
/* Case Insensitive                                                            */
/*=============================================================================*/
BOOLEAN
RbfPubRingBufCmpiWithCharLen(
    SRbfRingBuffer *pRingBuf,
    CONST CHAR *pSrc,
    UI16 nSrcLen
    );

/*=============================================================================*/
/* Returns true pSrc string is contained in the ring buffer at position        */
/* Case Insensitive                                                            */
/*=============================================================================*/
BOOLEAN
RbfPubRingBufMatchSubString(
    SRbfRingBuffer *pRingBuf,
    UI16 nStartPos, /* in ring buffer */
    CONST CHAR *pPattern,
    UI16 nSrcLen
    );

/*=============================================================================*/
/* returns true if the character matches that in the ring buffer at the */
/* position specified */
/*=============================================================================*/
BOOLEAN
RbfPubRingBufMatchByte(
    SRbfRingBuffer *pRingBuf,
    UI16 nPos, /* in ring buffer */
    UI8  nByte,
    BOOLEAN fCaseSensitive
    );

/*=============================================================================*/
/* returns true if the ring buffer is empty */
/*=============================================================================*/
BOOLEAN
RbfPubRingBufIsEmpty(
    SRbfRingBuffer *pRingBuf
    );

/*=============================================================================*/
/* returns true if the ring buffer is empty */
/*=============================================================================*/
UI16
RbfPubLineariseRingBufferAppendNull(
    SRbfRingBuffer  *pRingBuf,
    CHAR *pLinearBuf,
    UI16 nLinearBufLen
    );

/*=============================================================================*/
/* If the ring buf points to a string which is delimited by " then it strips   */
/*=============================================================================*/
VOID
RbfPubStripStringDelimiters(
    SRbfRingBuffer *pRingBuf
    );

/*=============================================================================*/
/*=============================================================================*/
BOOLEAN
RbfPubIsSame(
    SRbfRingBuffer *pRingBuf1,
    SRbfRingBuffer *pRingBuf2,
    BOOLEAN fCaseSensitive
    );

/*=============================================================================*/
/*=============================================================================*/
UI16
RbfPubDropLeadingBytes(
    SRbfRingBuffer *pRingBuf,
    UI16 nBytes
    );

#ifdef __cplusplus
}
#endif

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
