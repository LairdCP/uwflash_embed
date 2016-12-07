/******************************************************************************
**              Copyright (C) 2007 TDK Systems Europe Ltd
**
** Project:     WISM - Upgrade Class
**
** Module:      TBootLdrBT900.CPP
**
*******************************************************************************/

/******************************************************************************/
// CONDITIONAL COMPILE DEFINES
/******************************************************************************/

/******************************************************************************/
// Include Files
/******************************************************************************/
#include <stdlib.h>

#include "TBootLdrBT900.h"
#include "AtConfig.h"
#include "AtUpgradeUI.h"
#include "UwFlash.h"

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
TBootLdrBT900::TBootLdrBT900()
{
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
TBootLdrBT900::~TBootLdrBT900()
{
}



/******************************************************************************/
/******************************************************************************/
// Global Functions Non Class Members
/******************************************************************************/
/******************************************************************************/


//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrBT900::DriveModuleResetLine(bool fAssert)
{
    return  UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
bool TBootLdrBT900::IsCtsAsserted(void)
{
    if( mApdComPort )
    {
        return mApdComPort->GetCtsState();
    }
    return false;
}

//=============================================================================
#define CTS_POLL_DELAY_TIME_MS      (100)
//=============================================================================
void TBootLdrBT900::WaitForCtsOrTimeout(unsigned int nDelayMs, bool fTargetCtsState )
{

    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            unsigned int nTimeElapsed=0;
            bool fCurCtsState;

            while(  nTimeElapsed < nDelayMs )
            {
                fCurCtsState =  mApdComPort->GetCtsState();
                if( fCurCtsState == fTargetCtsState )
                {
                    return;
                }
                Wait(  CTS_POLL_DELAY_TIME_MS );
                nTimeElapsed += CTS_POLL_DELAY_TIME_MS;
            }
        }
    }
    else
    {
        Wait(nDelayMs);
    }
}

//=============================================================================
// It assumes that the comport is open on entry
//=============================================================================
UWRESULTCODE TBootLdrBT900::SwitchModuleToProgramMode(void)
{
    UpgradeUI_Message(CONVERTING_TO_FLASH_MODE);

    if( gAtConfig.mToggleDTR )
    {
        DeAssertDTR();
    }
    Wait(500);
    /* Send BREAK so that it ends up in normal AT mode */
    SetBreak();
    WaitForCtsOrTimeout(1000,false);
    ClearBreak();
    /* module is now in 9600 baudmode normal mode */
    WaitForCtsOrTimeout(5000,true);
    ComClose();

    char *pDevName;
    gAtConfig.SerialPortNameGet(&pDevName);

    if( ComOpenPollWait(pDevName,
                        BT900_NORMAL_MODE_BAUDRATE,
                        gAtConfig.mUserCommParms.mnParity,
                        gAtConfig.mUserCommParms.mnStopBits,
                        gAtConfig.mUserCommParms.mnDataBits,
                        COMPORT_POLL_WAIT_TIME_MS) )
    {
        /* can send the command to put it into flash mode */
        FlushRxBuffer();
        FlushTxBuffer();
        SendString("AT+FUP\r");
        WaitForCtsOrTimeout(1000,false);

        WaitForCtsOrTimeout(5000,true);
        ComClose();
        Wait(250);

        char *pDevName;
        gAtConfig.SerialPortNameGet(&pDevName);

        /* reopen at standard baudrate */
        if( ! ComOpenPollWait(pDevName,
                              BT900_FLASHUPLOAD_MODE_BAUDRATE,
                              gAtConfig.mUserCommParms.mnParity,
                              gAtConfig.mUserCommParms.mnStopBits,
                              gAtConfig.mUserCommParms.mnDataBits,
                              COMPORT_POLL_WAIT_TIME_MS) )
        {
            return UWRESULTCODE_UWF_COMPORT_OPENFAIL;
        }
        Wait(100);
        FlushRxBuffer();
        FlushTxBuffer();
    }
    else
    {
        return UWRESULTCODE_UWF_COMPORT_OPENFAIL;
    }
    return  UWRESULTCODE_SUCCESS;
}


//=============================================================================
// Called when fail to enter program mode
//=============================================================================
VOID TBootLdrBT900::OnFailSwitchModuleToProgramMode(void)
{
    UpgradeUI_Message(FAIL_ENTER_PROGMODE);
}

//=============================================================================
//=============================================================================
bool TBootLdrBT900::CheckPlatform(
    UI32  nPlatform
    )
{
    if( nPlatform!= BLR_PLATFORM_ID_BT900 )
    {
        return false;
    }
    /* first line of defence has passed, so now ask the unit if it is ok */
    return UWSUCCESS(SendPLATFORM(nPlatform));
}


/******************************************************************************/
// END OF FILE
/******************************************************************************/


