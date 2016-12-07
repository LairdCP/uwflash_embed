/******************************************************************************
**              Copyright (C) 2007 TDK Systems Europe Ltd
**
** Project:     WIDM - Upgrade Class
**
** Module:      UWFUPGRADE.CPP
**
*******************************************************************************/

/******************************************************************************/
// CONDITIONAL COMPILE DEFINES
/******************************************************************************/

/******************************************************************************/
// Include Files
/******************************************************************************/
#include <stdlib.h>
#include "UwfUpgrade.h"
#include "UwFlash.h"
#include "TTestNone.h"

/******************************************************************************/
// Local Defines
/******************************************************************************/

/******************************************************************************/
// Local Macros
/******************************************************************************/

/******************************************************************************/
// Local Forward Class,Struct & Union Declarations
/******************************************************************************/

/******************************************************************************/
// Local Class,Struct,Union Typedefs
/******************************************************************************/

/******************************************************************************/
// External Variable Declarations
/******************************************************************************/

/******************************************************************************/
// Global/Static Variable Declarations
/******************************************************************************/
UwfUpgrade  gUwfUpgrade;
TTestBase  *gpTestContext=NULL;

/******************************************************************************/
// External Function Declarations
/******************************************************************************/

/******************************************************************************/
// Local Forward Function Declarations
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
// Local Functions or Private Members
/******************************************************************************/
/******************************************************************************/

//=============================================================================
//=============================================================================

/******************************************************************************/
/******************************************************************************/
// Global Functions or Public Members
/******************************************************************************/
/******************************************************************************/

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
UwfUpgrade::UwfUpgrade()
{
    mpFile=NULL;
    mpLineBuf=NULL;
    mPassMsgId=ENUM_ATUPG_MSGID(UPGRADE_PASS);
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
UwfUpgrade::~UwfUpgrade()
{
    if(mpFile)
    {
        StdFCLOSE(mpFile);
        mpFile=NULL;
    }
    if(mpLineBuf)
    {
        StdFREE(mpLineBuf);
        mpLineBuf=NULL;
    }
}


/*=============================================================================*/
/*
** Read a block of data, of maximum length into the buffer specified
** On exit *pReadLen contains actual number of bytes read.
*/
/*=============================================================================*/
STATIC UWRESULTCODE UfmPrvReadBlock(
    VOID *pContext,
    UI32 nOffsetIntoCache,
    UI32 *pReadLen
    )
{
    UwfUpgrade *pUwfUpgrade = (UwfUpgrade *)pContext;
    UI8 *pBuffer = &pUwfUpgrade->mpLineBuf[nOffsetIntoCache];
    UI32 nBufferLen = pUwfUpgrade->mLineBufLen - nOffsetIntoCache;

    *pReadLen=StdFREAD(pBuffer,1,nBufferLen,pUwfUpgrade->mpFile);
    return (*pReadLen)<=0 ? UWRESULTCODE_EOF : UWRESULTCODE_SUCCESS;
}


//=============================================================================
//=============================================================================
bool UwfUpgrade::PreProcess(CHAR *pFilename)
{
    UWRESULTCODE resultCode;

    /* Change target type so that the preprocess is invoked */
    gAtConfig.mTargetType = (ETargetType)(gAtConfig.mTargetType | TARGET_PREPROCESS_MASK);

    /* open the file */
    mpFile = StdFOPEN( pFilename , "rb" );
    if(mpFile==NULL)
    {
        return NULL;
    }
    /* Finally all the malloc stuff inside this context */
    mpLineBuf = (UI8 *)StdMALLOC(gAtConfig.mFirmwareFileChunkLen);
    if(mpLineBuf==NULL)
    {
        return false;
    }
    mLineBufLen = gAtConfig.mFirmwareFileChunkLen;
    /************/

    /* Parse the firmware file */
    resultCode=FrmPubProcessFile(UfmPrvReadBlock,(void *)this,mpLineBuf,mLineBufLen);

    /************/
    /* Finally freea all the malloc stuff inside this context */
    if(mpLineBuf)
    {
        StdFREE(mpLineBuf);
        mpLineBuf=NULL;
    }

    /* Close the file */
    if(mpFile)
    {
        StdFCLOSE(mpFile);
        mpFile=NULL;
    }

    /* Restore target type so that the preprocess is not invoked */
    gAtConfig.mTargetType = (ETargetType)(gAtConfig.mTargetType & (~TARGET_PREPROCESS_MASK));

    return UWSUCCESS(resultCode);
}

//=============================================================================
//=============================================================================
bool UwfUpgrade::Process(CHAR *pFilename)
{
    UWRESULTCODE resultCode;

    /* open the file */
    mpFile = StdFOPEN( pFilename , "rb" );
    if(mpFile==NULL)
    {
        return NULL;
    }
    /* Finally all the malloc stuff inside this context */
    mpLineBuf = (UI8 *)StdMALLOC(gAtConfig.mFirmwareFileChunkLen);
    if(mpLineBuf==NULL)
    {
        return false;
    }
    mLineBufLen = gAtConfig.mFirmwareFileChunkLen;
    /************/

    /* Parse the firmware file */
    resultCode=FrmPubProcessFile(UfmPrvReadBlock,(void *)this,mpLineBuf,mLineBufLen);

    /************/
    /* Finally freea all the malloc stuff inside this context */
    if(mpLineBuf)
    {
        StdFREE(mpLineBuf);
        mpLineBuf=NULL;
    }

    if(UWSUCCESS(resultCode))
    {
        gpAtUpgradeUI->Message(mPassMsgId);
    }
    else
    {
        UpgradeUI_Message_S(UPGRADE_FAIL,(char *)UwgResultCodeString(resultCode));
    }

    /* Close the file */
    if(mpFile)
    {
        StdFCLOSE(mpFile);
        mpFile=NULL;
    }

    return UWSUCCESS(resultCode);
}


/******************************************************************************/
/******************************************************************************/
// Global Functions Non Class Members
/******************************************************************************/
/******************************************************************************/

//=============================================================================
//=============================================================================
bool UwfUpgrade::Start(CHAR *pFilename)
{
    bool fRetVal = false;
    UI32 nTotalByteEq;
    if( PreProcess( pFilename ) )
    {
        nTotalByteEq = gAtConfig.mTotalWriteLen + gAtConfig.mTotalEraseByteEquiv;
        gpAtUpgradeUI->InitAndShowMainProgressBar(0,nTotalByteEq);
        mOverallProgBytes=0;
        gpAtUpgradeUI->UpdateMainProgressBar(mOverallProgBytes);
        fRetVal = Process( pFilename );
        gpAtUpgradeUI->HideMainProgressBar();
    }

    return fRetVal;
}

//=============================================================================
//=============================================================================
UWRESULTCODE UwfUpgrade::PostUpgradeTest(void)
{
    UWRESULTCODE resultCode=UWRESULTCODE_SUCCESS;
    /* get and construct the test object */
    switch( gAtConfig.mTargetType )
    {
    default:
        gpTestContext = (TTestBase *) new TTestNone();
        break;
    }

    /* Perform the test */
    resultCode=gpTestContext->Initialise(gAtConfig.mTestNum,gAtConfig.mbaTestData);
    if(UWSUCCESS(resultCode))
    {
        /* reset the module */
        if( gAtConfig.mfAutomaticMode)
        {
            resultCode = gpTestContext->ResetAuto(gAtConfig.mTestNum);
        }
        else
        {
            resultCode = gpTestContext->ResetManual(gAtConfig.mTestNum);
        }
        /* Perform the test */
        if(UWSUCCESS(resultCode))
        {
            resultCode=gpTestContext->DoTest(gAtConfig.mTestNum,gAtConfig.mbaTestData);
        }
    }
    /* UWRESULTCODE resultCodeTerm = */gpTestContext->Terminate();

    /* destruct the test object */
    delete gpTestContext;
    gpTestContext=NULL;

    return resultCode;
}

/******************************************************************************/
// END OF FILE
/******************************************************************************/

