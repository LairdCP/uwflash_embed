/******************************************************************************
**              Copyright (C) 2005 Ezurio Ltd
**
** Project:     Universal Wireless
**
** Module:		FRMPUBLIC.C
**
** Notes:       Contains the public interface to the run time engine
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
#include "FrmPublic.h"
#include "FrmPrivate.h"

#include "BlrPublic.h"

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
STATIC SUwgHeapStats gsHeapStats={0,0};
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/******************************************************************************/
/* External Function Declarations*/
/******************************************************************************/

/******************************************************************************/
/* Local Forward Function Declarations*/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/* Memeory management routines - do NOT modify unless you know exactly what   */
/*                               you are doing                                */
/******************************************************************************/
/******************************************************************************/

/*=============================================================================*/
/* Allocate memory from heap. Returns NULL if no memory available              */
/*=============================================================================*/
#if !defined(UWG_SINGLE_HEAP_MANAGER)
VOID *FrmMalloc_( UI32 size )
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
#if !defined(UWG_SINGLE_HEAP_MANAGER)
VOID  FrmFree_( VOID **ppMemBlock )
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
VOID FrmPubGetHeapStats( SUwgHeapStats *psHeapStats )
{
    *psHeapStats = gsHeapStats;
}
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

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
/*
** You can ONLY call OSA functions
*/
/*=============================================================================*/
VOID FrmPubPreInitialise(VOID)
{
}

/*=============================================================================*/
/*=============================================================================*/
VOID FrmPubPostInitialise(VOID)
{
}

/*=============================================================================*/
/*
** Process a firmware file which will be in .uwf format. See elsewhere for
** more details on the .uwf format.
*/
/*=============================================================================*/
UWRESULTCODE
FrmPubProcessFile(
    FPullDataIntoCache FPullDataFunc,
    VOID *pFuncContext,
    UI8  *pCache,
    UI32  nCacheLen 
    )
{
    UWRESULTCODE resultCode;
    SFrmManager *pFrmContext;
    UI32 nBlocklen;
    UI32 nConsumed;
    UI32 nUnprocessedLen;
    BOOLEAN fEOF;


    /* Create the frm context */
    pFrmContext = FrmPrvOpenManager(&resultCode);
    if((pFrmContext==NULL)||UWFAIL(resultCode))
    {
        return resultCode; 
    }

    resultCode=UWRESULTCODE_SUCCESS;
    nUnprocessedLen=0;
    while( UWSUCCESS(resultCode) || UWCONTINUE(resultCode))
    {
        FrmASSERT3(nUnprocessedLen<0x10000);

        /* read more from the file */
        resultCode = FPullDataFunc(pFuncContext,nUnprocessedLen,&nBlocklen);
        nBlocklen += (UI16)nUnprocessedLen;
        fEOF = (resultCode==UWRESULTCODE_EOF) ? TRUE : FALSE;

        if( UWSUCCESS(resultCode) || (fEOF && nBlocklen) )
           
        {
            /* And process the data */
            resultCode=FrmPrvProcessData(pFrmContext,
                                         pCache,
                                         nBlocklen,
                                         &nConsumed);
            if( UWSUCCESS(resultCode) || UWCONTINUE(resultCode))
            {
                if(fEOF && (nConsumed==0) )
                {
                    /* did not consume any bytes */
                    resultCode = UWRESULTCODE_FRM_NO_CONSUME;
                }
                nUnprocessedLen = nBlocklen - nConsumed;
                if(nUnprocessedLen && nConsumed)
                {
                    StdMEMCPY(pCache,&pCache[nConsumed],nUnprocessedLen);  
                }
                if( nUnprocessedLen >= nCacheLen )
                {
                    /* there is no more room in the buffer and we are being */
                    /* asked to process further */
                    resultCode = UWRESULTCODE_FRM_PACKET_TOO_LARGE;
                }
            }
        }
        else
        {
            FrmASSERT3(resultCode!=UWRESULTCODE_CONTINUE); /* because it is unexpected */
        }
    }

    /* if data still to process and end of file was reached then it is an error */
    if(resultCode == UWRESULTCODE_EOF)
    {
        FrmASSERT3(nUnprocessedLen==0);

        /* no more data to process so check if the file */
        /* process is not expecting more data */
        if(FrmPrvCanClose(pFrmContext))
        {
            resultCode = UWRESULTCODE_SUCCESS;
        }
        else
        {
            resultCode = UWRESULTCODE_FRM_PACKET_INCOMPLETE; 
        }
    }

    /* Destroy the context before exiting */
    FrmPrvCloseManager(&pFrmContext);

    return resultCode;
}

/*=============================================================================*/
/*
** Returns the minimum cache size the caller should provide for a session
*/
/*=============================================================================*/
UI32
FrmPubGetMinCacheSize(
    VOID
    )
{
    return FRM_DOWNLOAD_VIA_SCRATCHPAD_MIN_BUFFER_SIZE;
}

/*=============================================================================*/
/*
** Open the firmware manager session
*/
/*=============================================================================*/
VOID *
FrmPubOpenManager(
    UWRESULTCODE *pResultCode
    )
{
    return FrmPrvOpenManager(pResultCode);
}

/*=============================================================================*/
/*
** Close the firmware manager session
*/
/*=============================================================================*/
VOID
FrmPubCloseManager(
    VOID **ppFrmContext
    )
{
    FrmPrvCloseManager((SFrmManager **)ppFrmContext);
}

/*=============================================================================*/
/*
** Ask the firmware file processor if it thinks that the processing can end
** without error.
*/
/*=============================================================================*/
BOOLEAN
FrmPubCanClose(
    VOID *pFrmContext
    )
{
    return FrmPrvCanClose((SFrmManager *)pFrmContext);
}

/*=============================================================================*/
/*
** See description for FrmPrvProcessData()
*/
/*=============================================================================*/
UWRESULTCODE
FrmPubProcessData(
    VOID *pContext,
    UI8 *pPacketData,
    UI32 nPacketLen,
    UI32 nSourceRemain,
    UI32 *pConsumed
    )
{
    /* nSourceRemain was added later - in this sink it is not used */
    return FrmPrvProcessData((SFrmManager *)pContext,pPacketData,nPacketLen,pConsumed);
}
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/


