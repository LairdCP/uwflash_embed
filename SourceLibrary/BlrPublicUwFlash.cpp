/******************************************************************************
**              Copyright (C) 2005 Ezurio Ltd
**
** Project:     Universal Wireless
**
** Module:		BLRPUBLICUWFLASH.C
**
** Notes:       Contains the public interface to the Boot Loader in the
**              Vehicle Unit
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
#include <stdlib.h>

#include "AtUpgradeUI.h"
#include "UwfUpgrade.h"
#include "UwgPublic.h"
#include "BlrPublic.h"
#include "UwFlash.h"

#include "TBootLdrPreProcess.h"
#if TARGET_PLATFORM ==  TARGET_BL600
#include "TBootLdrBL600.h"
#endif
#if TARGET_PLATFORM ==  TARGET_BL620
#include "TBootLdrBL620.h"
#endif
#if TARGET_PLATFORM ==  TARGET_BL6XX
#include "TBootLdrBL6XX.h"
#endif
#if TARGET_PLATFORM ==  TARGET_BT900
#include "TBootLdrBT900.h"
#endif
#if TARGET_PLATFORM ==  TARGET_RM1XX
#include "TBootLdrRM1XX.h"
#endif
#if TARGET_PLATFORM ==  TARGET_RM1XX_PE
#include "TBootLdrRM1XX_PE.h"
#endif
#if TARGET_PLATFORM ==  TARGET_BL65X
#include "TBootLdrBL65X.h"
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

TBootLdrBase *gpContext=(TBootLdrBase *)NULL;

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

STATIC void Close(void)
{
    if( gpContext )
    {
        gpContext->OnClose();
        delete(gpContext);
        gpContext = NULL;
    }
}

/******************************************************************************/
/******************************************************************************/
/* Global Functions or Public Members*/
/******************************************************************************/
/******************************************************************************/


/*=============================================================================*/
/*
** Open the boot loader manager session
*/
/*=============================================================================*/
extern "C" VOID *
BlrPubOpenCPP(
    UWRESULTCODE *pResultCode
    )
{
    /* Assume all will be ok */
    *pResultCode = UWRESULTCODE_SUCCESS;

    if( gpContext )
    {
        delete(gpContext);
    }

    /* Allocate memory for the context */
    if( gAtConfig.mTargetType & TARGET_PREPROCESS_MASK )
    {
        gpContext = (TBootLdrBase *) new TBootLdrPreProcess();
    }
    else
    {
        switch( gAtConfig.mTargetType )
        {
#if TARGET_PLATFORM ==  TARGET_BL600
        case TARGET_BL600:
            gpContext = (TBootLdrBase *) new TBootLdrBL600();
            break;
#endif
#if TARGET_PLATFORM ==  TARGET_BL620
        case TARGET_BL620:
            gpContext = (TBootLdrBase *) new TBootLdrBL620();
            break;
#endif
#if TARGET_PLATFORM ==  TARGET_BL6XX
        case TARGET_BL6XX:
            gpContext = (TBootLdrBase *) new TBootLdrBL6XX();
            break;
#endif
#if TARGET_PLATFORM ==  TARGET_BL65X
        case TARGET_BL65X:
            gpContext = (TBootLdrBase *) new TBootLdrBL65X();
            break;
#endif
#if TARGET_PLATFORM ==  TARGET_BT900
        case TARGET_BT900:
            gpContext = (TBootLdrBase *) new TBootLdrBT900();
            break;
#endif
#if TARGET_PLATFORM ==  TARGET_RM1XX
        case TARGET_RM1XX:
            gpContext = (TBootLdrBase *) new TBootLdrRM1XX();
            break;
#endif
#if TARGET_PLATFORM ==  TARGET_RM1XX_PE
        case TARGET_RM1XX_PE:
            gpContext = (TBootLdrBase *) new TBootLdrRM1XX_PE();
            break;
#endif            
        default:
            gpContext = NULL;
            break;
        }
    }
    if( gpContext )
    {
        *pResultCode = gpContext->OnOpen();
        if( UWFAIL(*pResultCode) )
        {
            Close();
            gpContext=NULL;
        }
    }
    else
    {
        *pResultCode =  UWRESULTCODE_UWF_UNKNOWN_PLATFORM;
    }

    return (VOID *)gpContext;
}

/*=============================================================================*/
/*
** Close the boot loader manager session
*/
/*=============================================================================*/
extern "C" VOID
BlrPubCloseCPP(
    VOID *pBlrContext
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    pContext=pContext;
    Close();
}


/*=============================================================================*/
/*
** Returns true if nPlatform matches the target platform this code is running
** on
*/
/*=============================================================================*/
extern "C" bool
BlrPubCheckPlatformCPP(
    VOID *pBlrContext,
    UI32  nPlatform
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);

    return pContext->CheckPlatform(nPlatform);
}

/*=============================================================================*/
/*
** Register a flash device for manipulation
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubRegisterDeviceCPP(
    VOID *pBlrContext,
    UI8  nFlashDevice,
    UI32 nBaseAddress,
    UI8  nNumOfBanks,
    UI32 nBankSize,
    UI8  nBankSelAlgoFuncId
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    return pContext->RegisterDevice(nFlashDevice,nBaseAddress,nNumOfBanks,nBankSize,nBankSelAlgoFuncId);
}

/*=============================================================================*/
/*
** Unregister a flash device for manipulation
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubUnregisterDeviceCPP(
    VOID *pBlrContext,
    UI8  nFlashDevice
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    return pContext->UnregisterDevice(nFlashDevice);
}

/*=============================================================================*/
/*
** Instructs the boot loader to make the bank available for the flash device
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubSetBankCPP(
    VOID *pBlrContext,
    UI8  nFlashDevice,
    UI8  nBank
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    return pContext->SetBank(nFlashDevice,nBank);
}

/*=============================================================================*/
/*
** Called to start the WRITE_BLOCK packet processing
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubWriteBlockHeaderCPP(
    VOID *pBlrContext,
    UI32  nOffset,
    UI32  nBlockLenBytes,
    UI32  nFlags
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    return pContext->WriteBlockHeader(nOffset,nBlockLenBytes,nFlags);
}

/*=============================================================================*/
/*
** Called to end the WRITE_BLOCK packet processing
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubWriteBlockEndCPP(
    VOID *pBlrContext
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    return pContext->WriteBlockEnd();
}

/*=============================================================================*/
/*
** Write the data at pData to the current bootloader activated flash
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubWriteBlockCPP(
    VOID *pBlrContext,
    UI8  *pData,
    UI32  nAddrOffset,
    UI32  nBlockLenBytes
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    return pContext->WriteBlock(pData,nAddrOffset,nBlockLenBytes);
}

/*=============================================================================*/
/*
** Erase from start to end specified
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubEraseBlockCPP(
    VOID *pBlrContext,
    UI32  nAddrOffsetStart,
    UI32  nBlockSize
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    return pContext->EraseBlock(nAddrOffsetStart,nBlockSize) ;
}

/*=============================================================================*/
/*
** Specify Sector Map
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubSectorMapCPP(
    VOID *pBlrContext,
    SBlrSectorMapItem *pSectorMapArray,
    UI32 nNumOfItems
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    return pContext->SectorMap(pSectorMapArray,nNumOfItems) ;
}

/*=============================================================================*/
/*
** Asks the boot loader to choose a bank from the flash device selected
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubAutoSelectBankCPP(
    VOID *pBlrContext,
    UI8  nFlashDevice,
    UI8  *pBank
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    return pContext->AutoSelectBank(nFlashDevice,pBank);
}

/*=============================================================================*/
/*
** Send a query to the boot loader
*/
/*=============================================================================*/
extern "C" UWRESULTCODE
BlrPubQueryCPP(
    VOID *pBlrContext,
    UI32  nQueryId,
    UI32  *pQueryVals,
    UI16  nNumOfVals
    )
{
    TBootLdrBase *pContext = (TBootLdrBase *)pBlrContext;
    UwfASSERT3(pContext != NULL);
    UwfASSERT3(pContext == gpContext);
    UwfASSERT3(pQueryVals != NULL);
    UwfASSERT3(nNumOfVals);
    return pContext->Query(nQueryId,pQueryVals,nNumOfVals) ;
}

/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/



