/******************************************************************************
**              Copyright (C) 2005 Ezurio Ltd
**
** Project:     Universal Wireless
**
** Module:		RBFPUBLIC.C
**
** Notes:
**
*******************************************************************************/

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
#include "RbfPublic.h"
#include "RbfPrivate.h"

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

#if !defined(UWG_SINGLE_HEAP_MANAGER)
STATIC SUwgHeapStats gsHeapStats={0,0,0};
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

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


/******************************************************************************/
/******************************************************************************/
/* Global Functions or Public Members*/
/******************************************************************************/
/******************************************************************************/

/*=============================================================================*/
/* Allocate memory from heap. Returns NULL if no memory available              */
/*=============================================================================*/
#if !defined(UWG_SINGLE_HEAP_MANAGER) && 0
VOID *RbfMalloc_( UI32 size )
{
    gsHeapStats.mMallocCount++;
    if( (gsHeapStats.mMallocCount-gsHeapStats.mFreeCount) > gsHeapStats.mMaxCount )
    {
        gsHeapStats.mMaxCount = (gsHeapStats.mMallocCount-gsHeapStats.mFreeCount);
    }
    return StdMALLOC(size);
}
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/* Free allocated memory back to the heap.                                     */
/* It takes a pointer to a pointer. The function is safe if a NULL argument is */
/* provided. It will set the pointer to NULL after the memory is freed         */
/*=============================================================================*/
#if !defined(UWG_SINGLE_HEAP_MANAGER) && 0
VOID  RbfFree_( VOID **ppMemBlock )
{
    DEVASSERT2(gsHeapStats.mMallocCount > gsHeapStats.mFreeCount);

    if(ppMemBlock)
    {
        if(*ppMemBlock)
        {
            StdFREE(*ppMemBlock);
            gsHeapStats.mFreeCount++;
            *ppMemBlock=NULL;
        }
    }
}
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/* Return the heap usage statistics                                            */
/*=============================================================================*/
#if !defined(UWG_SINGLE_HEAP_MANAGER)
VOID RbfPubGetHeapStats( SUwgHeapStats *psHeapStats )
{
    *psHeapStats = gsHeapStats;
}
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/*
** You can ONLY call OSA functions
*/
/*=============================================================================*/
VOID RbfPubPreInitialise(VOID)
{
}

/*=============================================================================*/
/*
** It is safe to call CFG and FSA functions
*/
/*=============================================================================*/
VOID RbfPubPostInitialise(VOID)
{
}

/*=============================================================================*/
/* Initialise a SRbfBuffer object */
/*=============================================================================*/
SRbfBuffer *
RbfPubTextSet(
    SRbfBuffer *pText,    /* Pointer to object */
    CHAR       *pData,    /* Points to the data */
    UI16        nLen      /* String is NOT null terminated, but length specified */
    )
{
    RbfASSERTPC(pText != NULL);

    if(pText)
    {
        pText->u.mpcData = pData;
        pText->mnLen     = nLen;

#if DEBUGLEVEL>3
        if(pData==NULL)
        {
            RbfASSERT3(nLen==0);
        }
#endif
    }

    return pText;
}

/*=============================================================================*/
/* clears a SRbfBuffer object */
/*=============================================================================*/
SRbfBuffer *
RbfPubTextClear(
    SRbfBuffer *pText    /* Pointer to object */
    )
{
    RbfASSERTPC(pText != NULL);

    if(pText)
    {
        pText->u.mpcData = NULL;
        pText->mnLen     = 0;
    }

    return pText;
}

/*=============================================================================*/
/* sets both buffers to empty */
/*=============================================================================*/
VOID
RbfPubClearRingBuffer(
    SRbfRingBuffer *pRbfRingBuffer    /* Pointer to object */
    )
{
    RbfASSERTPC(pRbfRingBuffer != NULL);

    if(pRbfRingBuffer)
    {
        pRbfRingBuffer->mBuffer[0].u.mpcData = NULL;
        pRbfRingBuffer->mBuffer[0].mnLen   = 0;
        pRbfRingBuffer->mBuffer[1].u.mpcData = NULL;
        pRbfRingBuffer->mBuffer[1].mnLen   = 0;
    }
}

/*=============================================================================*/
/* Initialise a ring buffer with a len terminated char string */
/*=============================================================================*/
VOID
RbfPubSetRingBuffer(
    SRbfRingBuffer *pRbfRingBuffer,    /* Pointer to object */
    CHAR *pString,
    UI16 nLen
    )
{
    RbfASSERTPC(pRbfRingBuffer != NULL);

    if(pRbfRingBuffer)
    {
        if(pString)
        {
            pRbfRingBuffer->mBuffer[0].u.mpcData = pString;
            pRbfRingBuffer->mBuffer[0].mnLen   = nLen;
        }
        else
        {
            RbfASSERT3(nLen==0);
            pRbfRingBuffer->mBuffer[0].u.mpcData = NULL;
            pRbfRingBuffer->mBuffer[0].mnLen   = 0;
        }

        pRbfRingBuffer->mBuffer[1].u.mpcData = NULL;
        pRbfRingBuffer->mBuffer[1].mnLen   = 0;
    }
}

/*=============================================================================*/
/* Initialise a ring buffer with a null terminated char string */
/*=============================================================================*/
VOID
RbfPubSetRingBufferWithChar(
    SRbfRingBuffer *pRbfRingBuffer,    /* Pointer to object */
    CHAR *pString
    )
{
    RbfASSERTPC(pRbfRingBuffer != NULL);

    if(pRbfRingBuffer)
    {
        if(pString)
        {
            pRbfRingBuffer->mBuffer[0].u.mpcData = pString;
            pRbfRingBuffer->mBuffer[0].mnLen   = strlen(pString);;
        }
        else
        {
            pRbfRingBuffer->mBuffer[0].u.mpcData = NULL;
            pRbfRingBuffer->mBuffer[0].mnLen   = 0;
        }

        pRbfRingBuffer->mBuffer[1].u.mpcData = NULL;
        pRbfRingBuffer->mBuffer[1].mnLen   = 0;
    }
}

/*=============================================================================*/
/* returns the total length of the data in two buffers                         */
/*=============================================================================*/
UI16
RbfPubGetDataLen(
    SRbfRingBuffer *pRingBuf
    )
{
    UI16 nRetVal = 0;

    if(pRingBuf)
    {
        RbfVALID_RING_BUFFER(pRingBuf);
        nRetVal = pRingBuf->mBuffer[0].mnLen + pRingBuf->mBuffer[1].mnLen;
    }

    return nRetVal;
}

/*=============================================================================*/
/* Uses DEVTRACE to display the content of the ring buffer                     */
/*=============================================================================*/
#if 0
VOID
RbfPubRingBufferPrintf(
    SRbfRingBuffer *pRingBuf
    )
{
    UI16 nLen=pRingBuf->mBuffer[0].mnLen;
    UI8 *p=pRingBuf->mBuffer[0].u.mpuData;
    while(nLen--)
    {
        DEVTRACE(("%c",*p++));
    }
    nLen=pRingBuf->mBuffer[1].mnLen;
    p=pRingBuf->mBuffer[1].u.mpuData;
    while(nLen--)
    {
        DEVTRACE(("%c",*p++));
    }
}
#endif

/*=============================================================================*/
/* Returns true if the data in pRingBuf matches that in pSrc                   */
/* Case Sensitive                                                            */
/*=============================================================================*/
#if defined(ON_SUBTARGET_PCSCRIPT) || defined(PPR_ENABLE_PREPARSER)
BOOLEAN 
RbfPubRingBufCmpWithChar(
    SRbfRingBuffer *pRingBuf,
    CONST CHAR *pSrc
    )
{
    UI8  *pRing = pRingBuf->mBuffer[0].u.mpuData;
    UI16 nLen   = pRingBuf->mBuffer[0].mnLen;

    if( (nLen==0) && (*pSrc) ) return FALSE; 
    while(nLen--)
    {
        if( (*pSrc++) != (*pRing++) )
        {
            return FALSE;
        }
    }

    pRing  = pRingBuf->mBuffer[1].u.mpuData;
    nLen   = pRingBuf->mBuffer[1].mnLen;
    if( (nLen==0) && (*pSrc) ) return FALSE; 
    while(nLen--)
    {
        if( (*pSrc++) != (*pRing++) )
        {
            return FALSE;
        }
    }

    /* reaching here means the data compare */
    return TRUE;
}
#endif

/*=============================================================================*/
/* Returns true if the data in pRingBuf matches that in pSrc                   */
/* Case Insensitive                                                            */
/*=============================================================================*/
BOOLEAN 
RbfPubRingBufCmpiWithChar(
    SRbfRingBuffer *pRingBuf,
    CONST CHAR *pSrc
    )
{
    UI8  *pRing = pRingBuf->mBuffer[0].u.mpuData;
    UI16 nLen   = pRingBuf->mBuffer[0].mnLen;

    if( (nLen==0) && (*pSrc) ) return FALSE; 
    while(nLen--)
    {
        if( StdTOUPPER(*pSrc++) != StdTOUPPER(*pRing++) )
        {
            return FALSE;
        }
    }

    pRing  = pRingBuf->mBuffer[1].u.mpuData;
    nLen   = pRingBuf->mBuffer[1].mnLen;
    if( (nLen==0) && (*pSrc) ) return FALSE; 
    while(nLen--)
    {
        if( StdTOUPPER(*pSrc++) != StdTOUPPER(*pRing++) )
        {
            return FALSE;
        }
    }

    /* reaching here means the data compare */
    return TRUE;
}

/*=============================================================================*/
/* Returns true if the data in pRingBuf matches that in pSrc                   */
/* The char string is length terminated                                        */
/* Case Insensitive                                                            */
/*=============================================================================*/
#if 0
BOOLEAN 
RbfPubRingBufCmpiWithCharLen(
    SRbfRingBuffer *pRingBuf,
    CONST CHAR *pSrc,
    UI16 nSrcLen
    )
{
    UI8  *pRing = pRingBuf->mBuffer[0].u.mpuData;
    UI16 nLen   = pRingBuf->mBuffer[0].mnLen;

    /* if the lengths are not the same then they do not compare */
    if( (nLen+pRingBuf->mBuffer[1].mnLen) != nSrcLen ) return FALSE; 

    /* if both empty then they compare */
    if( nSrcLen == 0 ) return TRUE; 

    while(nSrcLen)
    {
        if( StdTOUPPER(*pSrc++) != StdTOUPPER(*pRing++) )
        {
            return FALSE;
        }
        nSrcLen--;
    }

    /* If source string len is 0 then they compare */
    if(nSrcLen==0)return TRUE;
    
    /* otherwise continue and compre with second buffer */
    pRing  = pRingBuf->mBuffer[1].u.mpuData;
    RbfASSERT3(pRing);

    nLen   = pRingBuf->mBuffer[1].mnLen;
    RbfASSERT3(nSrcLen==nLen);

    while(nSrcLen)
    {
        if( StdTOUPPER(*pSrc++) != StdTOUPPER(*pRing++) )
        {
            return FALSE;
        }
        nSrcLen--;
    }

    /* reaching here means the data compare */
    return TRUE;
}
#endif

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
    )
{
    /* if nStartPos is beyond the end of the data in the ring buffer return false */
    if( nPos >= (pRingBuf->mBuffer[0].mnLen + pRingBuf->mBuffer[1].mnLen) )
    {
        return FALSE;
    }

    /* update sNewBuf so that it specifies a ring buf starting from nStartPos */
    /* and the same length as the */
    if( nPos < pRingBuf->mBuffer[0].mnLen )
    {
        /* the character to match is in the first buffer */
        if(fCaseSensitive)
        {
            if( pRingBuf->mBuffer[0].u.mpuData[nPos] == nByte )
            {
                return TRUE;
            }
        }
        else
        {
            if( StdTOUPPER(pRingBuf->mBuffer[0].u.mpuData[nPos]) == StdTOUPPER(nByte))
            {
                return TRUE;
            }
        }
    }
    else /* $i */
    {
        /* the character to match is in the second buffer */
        RbfASSERT3(pRingBuf->mBuffer[1].u.mpuData);
        RbfASSERT3(pRingBuf->mBuffer[1].mnLen);

        nPos -= pRingBuf->mBuffer[0].mnLen;
        if(fCaseSensitive)
        {
            if( pRingBuf->mBuffer[1].u.mpuData[nPos] == nByte )
            {
                return TRUE;
            }
        }
        else
        {
            if( StdTOUPPER(pRingBuf->mBuffer[1].u.mpuData[nPos]) == StdTOUPPER(nByte) )
            {
                return TRUE;
            }
        }
    }

    /* reaching here means does not match */
    return FALSE;
}

/*=============================================================================*/
/* Returns true pSrc string is contained in the ring buffer at position        */
/* Case Insensitive                                                            */
/*=============================================================================*/
#if 0 /* Because it is not used anywhere */
BOOLEAN 
RbfPubRingBufMatchSubString(
    SRbfRingBuffer *pRingBuf,
    UI16 nStartPos, /* in ring buffer */
    CONST CHAR *pPattern,
    UI16 nSrcLen
    )
{
    SRbfRingBuffer sNewBuf;
    UI16 nLen = pRingBuf->mBuffer[0].mnLen + pRingBuf->mBuffer[1].mnLen;

    /* if nStartPos is beyond the end of the data in the ring buffer return false */
    if( nStartPos >= nLen )
    {
        return FALSE;
    }

    /* if nStartPos+nSrcLen is beyond the end of the data in the ring buffer return false */
    if( (nStartPos+nSrcLen) > nLen )
    {
        return FALSE;
    }

    /* update sNewBuf so that it specifies a ring buf starting from nStartPos */
    /* and the same length as the */
    if( nStartPos < pRingBuf->mBuffer[0].mnLen )
    {
        /* the first character is in the first buffer */
        sNewBuf = *pRingBuf;
        sNewBuf.mBuffer[0].u.mpuData += nStartPos;
        sNewBuf.mBuffer[0].mnLen   -= nStartPos;
        if( sNewBuf.mBuffer[0].mnLen >= nSrcLen )
        {
            sNewBuf.mBuffer[0].mnLen    = nSrcLen;
            sNewBuf.mBuffer[1].mnLen    = 0;
        }
        else
        {
            sNewBuf.mBuffer[1].mnLen    = nSrcLen - sNewBuf.mBuffer[0].mnLen;
        }
    }
    else
    {
        sNewBuf.mBuffer[0].u.mpuData  = pRingBuf->mBuffer[1].u.mpuData;
        sNewBuf.mBuffer[1].u.mpuData  = NULL;

        sNewBuf.mBuffer[0].u.mpuData += nStartPos-pRingBuf->mBuffer[0].mnLen;
        sNewBuf.mBuffer[0].mnLen    = nSrcLen;
        sNewBuf.mBuffer[1].mnLen    = 0;
    }

    RbfASSERT3((sNewBuf.mBuffer[0].mnLen+sNewBuf.mBuffer[1].mnLen)==nSrcLen);

    return RbfPubRingBufCmpiWithCharLen(&sNewBuf,pPattern,nSrcLen);
}
#endif

/*=============================================================================*/
/* returns true if the ring buffer is empty */
/*=============================================================================*/
#if 0
BOOLEAN 
RbfPubRingBufIsEmpty(
    SRbfRingBuffer *pRingBuf
    )
{
    return (pRingBuf->mBuffer[0].mnLen + pRingBuf->mBuffer[1].mnLen) ? FALSE : TRUE;
}
#endif

/*=============================================================================*/
/*
** Takes a ring buffer and copies to a linear buffer. A null will be appended
** Only nLinearBufLen-1 bytes are copied from the source.
** Returns the number of bytes copied to the linear buffer
*/
/*=============================================================================*/
UI16
RbfPubLineariseRingBufferAppendNull(
    SRbfRingBuffer  *pRingBuf,
    CHAR *pLinearBuf,
    UI16 nLinearBufLen
    )
{
    UI16 nSrcLen;
    UI16 nRetVal;
    UI16 nBlockLen;

    RbfASSERT3(pRingBuf);
    RbfASSERT3(pLinearBuf);
    RbfASSERT3(nLinearBufLen>1);

    nLinearBufLen--; /*guarantee room for the null terminator */

    nSrcLen = RbfPubGetDataLen(pRingBuf);
    if( nSrcLen >= nLinearBufLen )
    {
        nSrcLen = nLinearBufLen;
    }
    nRetVal = nSrcLen;

    /* Copy from the first buffer */
    if(nSrcLen)
    {
        nBlockLen = pRingBuf->mBuffer[0].mnLen;
        if(nBlockLen > nSrcLen)
        {
            nBlockLen = nSrcLen;
        }
        StdMEMCPY(pLinearBuf,pRingBuf->mBuffer[0].u.mpcData,nBlockLen);
        nSrcLen    -= nBlockLen;
        pLinearBuf += nBlockLen;
    }

    if(nSrcLen)
    {
        nBlockLen = pRingBuf->mBuffer[1].mnLen;
        if(nBlockLen > nSrcLen)
        {
            nBlockLen = nSrcLen;
        }
        StdMEMCPY(pLinearBuf,pRingBuf->mBuffer[1].u.mpcData,nBlockLen);
        nSrcLen    -= nBlockLen;
        pLinearBuf += nBlockLen;
    }

    RbfASSERT3(nSrcLen==0);

    *pLinearBuf=0;

    return nRetVal;
}

/*=============================================================================*/
/* If the ring buf points to a string which is delimited by " then it strips   */
/*=============================================================================*/
VOID
RbfPubStripStringDelimiters(
    SRbfRingBuffer *pRingBuf
    )
{
    /* If quotes exist, strip them*/
    if( pRingBuf->mBuffer[0].u.mpcData[0] == '"' )
    {
        /* it is so strip them */
        pRingBuf->mBuffer[0].u.mpcData++;
        pRingBuf->mBuffer[0].mnLen--;

        if(pRingBuf->mBuffer[1].mnLen)
        {
            /* Something in the second buf */
            pRingBuf->mBuffer[1].mnLen--;
            if(pRingBuf->mBuffer[1].u.mpcData[pRingBuf->mBuffer[1].mnLen] != '"')
            {
                pRingBuf->mBuffer[1].mnLen++;
            }
            if(pRingBuf->mBuffer[0].mnLen==0)
            {
                /* Something in second buffer, but not first so promote */
                pRingBuf->mBuffer[0] = pRingBuf->mBuffer[1];
                pRingBuf->mBuffer[1].mnLen = 0;
                pRingBuf->mBuffer[1].u.mpuData = NULL;
            }
        }
        else
        {
            /* the second buf is empty so the end delimiter is in the first */
            pRingBuf->mBuffer[0].mnLen--;
            if(pRingBuf->mBuffer[0].u.mpcData[pRingBuf->mBuffer[0].mnLen] != '"')
            {
                pRingBuf->mBuffer[0].mnLen++;
            }
        }
    }
}

/*=============================================================================*/
/*
** Compares the two ring buffers and will return true if the strings are the
** same
*/
/*=============================================================================*/
BOOLEAN
RbfPubIsSame(
    SRbfRingBuffer *pBuf1,
    SRbfRingBuffer *pBuf2,
    BOOLEAN fCaseSensitive
    )
{
    UI16 nLen;
    UI16 nLen1 = RbfPubGetDataLen(pBuf1);
    UI16 nLen2 = RbfPubGetDataLen(pBuf2);
    UI8 *pSrc1;
    UI8 *pSrc2;

    if( nLen1 != nLen2 )
    {
        /* different lengths so need to check individual characters */
        return FALSE;
    }
    nLen = nLen1;

    /* Both strings are the same size so check for equality */
    pSrc1=pBuf1->mBuffer[0].u.mpuData;
    nLen1=pBuf1->mBuffer[0].mnLen;
    pSrc2=pBuf2->mBuffer[0].u.mpuData;
    nLen2=pBuf2->mBuffer[0].mnLen;
    while(nLen--)
    {
        if( *pSrc1 != *pSrc2 )
        {
            if( ! fCaseSensitive )
            {
                if( StdTOUPPER(*pSrc1) != StdTOUPPER(*pSrc2) )
                {
                    return FALSE;
                }
            }
        }
        /* getting here means we have a match */
        nLen1--;
        if( nLen1 )
        {
            pSrc1++;
        }
        else
        {
            pSrc1=pBuf1->mBuffer[1].u.mpuData;
            nLen1=pBuf1->mBuffer[1].mnLen;
        }

        nLen2--;
        if( nLen2 )
        {
            pSrc2++;
        }
        else
        {
            pSrc2=pBuf2->mBuffer[1].u.mpuData;
            nLen2=pBuf2->mBuffer[1].mnLen;
        }
    }
    /* reaching here means the strings match */
    return TRUE;
}

/*=============================================================================*/
/*
** Drops the number of bytes from the start. Returns the number of characters
** left in the ring buffer
*/
/*=============================================================================*/
UI16
RbfPubDropLeadingBytes(
    SRbfRingBuffer *pRingBuf,
    UI16 nBytes
    )
{
    UI16 nLen = pRingBuf->mBuffer[0].mnLen + pRingBuf->mBuffer[1].mnLen;
    if( nLen <= nBytes )
    {
        /* just empty the ring buffer */
        pRingBuf->mBuffer[0].mnLen = 0;
        pRingBuf->mBuffer[1].mnLen = 0;
        return 0;
    }
    /* still something left in the buffer */
    nLen -= nBytes;
    if( pRingBuf->mBuffer[0].mnLen > nBytes )
    {
        pRingBuf->mBuffer[0].mnLen     -= nBytes;
        pRingBuf->mBuffer[0].u.mpuData += nBytes;
    }
    else
    {
        nBytes -= pRingBuf->mBuffer[0].mnLen;
        pRingBuf->mBuffer[0].mnLen      = pRingBuf->mBuffer[1].mnLen - nBytes;
        pRingBuf->mBuffer[0].u.mpuData  = pRingBuf->mBuffer[1].u.mpuData + nBytes;
        pRingBuf->mBuffer[1].mnLen      = 0;
    }

    return nLen;
}


/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
