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

/******************************************************************************/
/* Target Build Definition Header File */
/******************************************************************************/
#include "Target.h"

/******************************************************************************/
/* CONDITIONAL COMPILE DEFINES*/
/******************************************************************************/

/******************************************************************************/
/* Include Files*/
/******************************************************************************/
#include "UwgPublic.h"
#include "UwgPrivate.h"

#if defined(_TARGETOSA_H)
#include "OsaPublic.h"
#endif
#if !defined(UWG_SINGLE_HEAP_MANAGER)
#include "StrPublic.h"
#endif

/******************************************************************************/
/* Local Defines*/
/******************************************************************************/

/******************************************************************************/
/* Local Macros*/
/******************************************************************************/

/******************************************************************************/
/* Local Forward Class,Struct & Union Declarations*/
/******************************************************************************/

/******************************************************************************/
/* Local Class,Struct,Union Typedefs*/
/******************************************************************************/

/******************************************************************************/
/* External Variable Declarations*/
/******************************************************************************/

/******************************************************************************/
/* Global/Static Variable Declarations*/
/******************************************************************************/

#if defined(UWG_MONITOR_MALLOCS)
STATIC SUwgHeapStats gsHeapStats={0,0,0};
#endif /* #if !defined(UWG_MONITOR_MALLOCS) */

#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
UI32   gMaxMallocSize=0;
I32   gMaxOpenMalloc=0;
I32   gMaxOpenMallocNoStr=0;
I32   gOpenMallocs=0;
#endif

/******************************************************************************/
/* External Function Declarations*/
/******************************************************************************/

/******************************************************************************/
/* Local Forward Function Declarations*/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/* Local Functions or Private Members*/
/******************************************************************************/
/******************************************************************************/
/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
STATIC VOID
UwgCheckedWriteDWORD(
    UI8 *pDest,
    UI32 nVal
    )
{
    UI8 *pSrc=(UI8 *)&nVal;
    *pDest++ = *pSrc++;
    *pDest++ = *pSrc++;
    *pDest++ = *pSrc++;
    *pDest++ = *pSrc++;
}
#endif

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
STATIC UI32
UwgCheckedReadDWORD(
    UI8 *pSrc
    )
{
    UI32 nRetVal;
    UI8 *pDest = (UI8 *)&nRetVal;
    *pDest++ = *pSrc++;
    *pDest++ = *pSrc++;
    *pDest++ = *pSrc++;
    *pDest++ = *pSrc++;
    return nRetVal;
}
#endif

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
STATIC VOID *
UwgCheckedMallocMonitor(
    UI32 size,
    BOOLEAN fMonitor
    )
{
    /* alloc size + space for 2 ui32's and 1 ui32 */
    /* we then have   ui8,ui32, size_data, ui8    */
    /* first ui16 = 0xDEAD and last ui16 = 0xDEAD */
    /* The ui32 contains the size of the block    */

    UI8 *pBlock,*pRet;
    UI32 nBlock = size + (sizeof(UI32) * 2) + sizeof(UI32);
    pBlock  = (UI8 *)Std__MALLOC(nBlock);
    if(pBlock)
    {
        UwgCheckedWriteDWORD(pBlock,0xDEADC0DE);
        pBlock += sizeof(UI32);
        UwgCheckedWriteDWORD(pBlock,size);
        pBlock += sizeof(UI32);
        pRet = pBlock;
        pBlock += size;
        UwgCheckedWriteDWORD(pBlock,0xDEADC0DE);

        if(fMonitor)
        {
            if(size>gMaxMallocSize)
            {
                gMaxMallocSize=size;
            }
        }
    }
    else
    {
        pRet = NULL; /* $i */
    }
    return pRet;
}
#endif

/******************************************************************************/
/******************************************************************************/
/* Global Functions or Public Members*/
/******************************************************************************/
/******************************************************************************/

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
#if DEBUGLEVEL < 2
#error HEAP MANAGER CHECKS should not be enabled if DEBUGLEVEL < 2
#endif
#endif

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
VOID *
UwgCheckedMalloc(
    UI32 size
    )
{

    gOpenMallocs++;
    if(gOpenMallocs>gMaxOpenMalloc)
    {
        gMaxOpenMalloc=gOpenMallocs;
    }
#if !defined(UWG_SINGLE_HEAP_MANAGER)
    {
        I32 nTemp=gOpenMallocs-StrMaxMallocs();
        if(nTemp>gMaxOpenMallocNoStr)
        {
            gMaxOpenMallocNoStr=nTemp;
        }
    }
#endif
    return UwgCheckedMallocMonitor(size,TRUE);
}
#endif

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
VOID *
UwgCheckedMallocNoMaxCheck(
    UI32 size
    )
{
    return UwgCheckedMallocMonitor(size,FALSE);
}
#endif

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
VOID
UwgCheckedFree(
    VOID *pMemBlock
    )
{
    /* See the Malloc function for description for check block format */

    UI32 nSize;
    UI32 nVal;

    UI8 *pBlock = pMemBlock;
    pBlock -= sizeof(UI32) + sizeof(UI32);

    gOpenMallocs--;

    nVal=UwgCheckedReadDWORD(pBlock);
    if( nVal!=0xDEADC0DE )
    {
        /* if this assert happens - we have memory corruption */
        UwgASSERT3( nVal==0xDEADC0DE ); /* $i */
    }

    pBlock += sizeof(UI32);
    nSize=UwgCheckedReadDWORD(pBlock);
    pBlock += sizeof(UI32);

    nVal=UwgCheckedReadDWORD(&pBlock[nSize]);
    if( nVal!=0xDEADC0DE )
    {
        /* if this assert happens - we have memory corruption */
        UwgASSERT3( nVal==0xDEADC0DE ); /* $i */
    }

    pBlock -= (sizeof(UI32) + sizeof(UI32));
    Std__FREE(pBlock);
}
#endif


/*=============================================================================*/
/* Allocate memory from heap. Returns NULL if no memory available              */
/*=============================================================================*/
#if !defined(UWG_SINGLE_HEAP_MANAGER) && 0
VOID *UwgMalloc_( UI32 size )
{
#if defined(UWG_MONITOR_MALLOCS)
    gsHeapStats.mMallocCount++;
    if( (gsHeapStats.mMallocCount-gsHeapStats.mFreeCount) > gsHeapStats.mMaxCount )
    {
        gsHeapStats.mMaxCount = (gsHeapStats.mMallocCount-gsHeapStats.mFreeCount);
    }
#endif
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)

    return UwgCheckedMalloc(size);
#else

    return Std__MALLOC(size);
#endif
}
#else

VOID *UwgMalloc_( UI32 size )
{ /* $i */
#if defined(UWG_MONITOR_MALLOCS)
    gsHeapStats.mMallocCount++; /* $i */
    if( (gsHeapStats.mMallocCount-gsHeapStats.mFreeCount) > gsHeapStats.mMaxCount ) /* $i */
    { /* $i */
        gsHeapStats.mMaxCount = (gsHeapStats.mMallocCount-gsHeapStats.mFreeCount); /* $i */
    } /* $i */
#endif
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
    return UwgCheckedMalloc(size); /* $i */
#else
    return Std__MALLOC(size); /* $i */
#endif
} /* $i */

#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/* Free allocated memory back to the heap.                                     */
/* It takes a pointer to a pointer. The function is safe if a NULL argument is */
/* provided. It will set the pointer to NULL after the memory is freed         */
/*=============================================================================*/
#if !defined(UWG_SINGLE_HEAP_MANAGER) && 0
VOID  UwgFree_( VOID **ppMemBlock )
{
    DEVASSERT2(gsHeapStats.mMallocCount > gsHeapStats.mFreeCount);

    if(ppMemBlock)
    {
        if(*ppMemBlock)
        {
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
            UwgCheckedFree(*ppMemBlock);
#else
            Std__FREE(*ppMemBlock);
#endif
#if defined(UWG_MONITOR_MALLOCS)
            gsHeapStats.mFreeCount++;
#endif
            *ppMemBlock=NULL;
        }
    }
}
#else

VOID  UwgFree_( VOID **ppMemBlock )
{ /* $i */
    if(ppMemBlock) /* $i */
    { /* $i */
        if(*ppMemBlock) /* $i */
        { /* $i */
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
            UwgCheckedFree(*ppMemBlock); /* $i */
#else
            Std__FREE(*ppMemBlock); /* $i */
#endif
#if defined(UWG_MONITOR_MALLOCS)
            gsHeapStats.mFreeCount++; /* $i */
#endif
            *ppMemBlock=NULL; /* $i */
        } /* $i */
    } /* $i */
} /* $i */
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/* Return the heap usage statistics                                            */
/*=============================================================================*/
#if defined(UWG_MONITOR_MALLOCS)
VOID UwgPubGetHeapStats( SUwgHeapStats *psHeapStats )
{
    *psHeapStats = gsHeapStats;
}
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/*
** You can ONLY call OSA functions
*/
/*=============================================================================*/
VOID UwgPubPreInitialise(VOID)
{
}

/*=============================================================================*/
/*
** It is safe to call CFG and FSA functions
*/
/*=============================================================================*/
VOID UwgPubPostInitialise(VOID)
{
}

/*============================================================================*/
/*
** This is called when an assert occurs
*/
/*============================================================================*/
INT UwlPubOnAssert(INT nExitCode)
{ /* $i */
#if defined(_TARGETOSA_H)
    return OsaPubTaskGetCurrent(); /* $i */
#else
    return 0; /* $i */
#endif
} /* $i */

/*=============================================================================*/
/*
** Call this to count the total number of word items in the array of blocks
*/
/*=============================================================================*/
UI16 UwgPubCountItems(
    SUwgWordBlock *pBlocks,
    UI16 nNumOfBlocks
    )
{
    UI32 nCount=0;
    while(nNumOfBlocks--)
    {
        if(pBlocks)
        {
            nCount += pBlocks->mWords;
        }
        else
        {
            break;
        }
        pBlocks++;
    }
    UwgASSERT3(nCount<0x10000);

    return (nCount>0xFFFF) ? 0xFFFF : (UI16)nCount;
}


/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
