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
// CONDITIONAL COMPILE DEFINES
/******************************************************************************/

/******************************************************************************/
// Include Files
/******************************************************************************/
#include <stdlib.h>

#include "TBootLdrBL6XX.h"
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
TBootLdrBL6XX::TBootLdrBL6XX()
{
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
TBootLdrBL6XX::~TBootLdrBL6XX()
{
}



/******************************************************************************/
/******************************************************************************/
// Global Functions Non Class Members
/******************************************************************************/
/******************************************************************************/


//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrBL6XX::DriveModuleResetLine(bool fAssert)
{
    return  UWRESULTCODE_SUCCESS;
}

//=============================================================================
// It assumes that the comport is open on entry
//=============================================================================
UWRESULTCODE TBootLdrBL6XX::SwitchModuleToProgramMode(void)
{
    UpgradeUI_Message(CONVERTING_TO_FLASH_MODE);

    if( gAtConfig.mToggleDTR )
    {
        DeAssertDTR();
    }
    Wait(500);
    /* Send BREAK so that it ends up in normal AT mode */
    SetBreak();
    Wait(1000);
    ClearBreak();
    /* module is now in 9600 baudmode normal mode */
    ComClose();

    char *pDevName;
    gAtConfig.SerialPortNameGet(&pDevName);

    if( ComOpenPollWait(pDevName,
                        BL600_NORMAL_MODE_BAUDRATE,
                        gAtConfig.mUserCommParms.mnParity,
                        gAtConfig.mUserCommParms.mnStopBits,
                        gAtConfig.mUserCommParms.mnDataBits,
                        COMPORT_POLL_WAIT_TIME_MS) )
    {
        /* can send the command to put it into flash mode */
        FlushRxBuffer();
        FlushTxBuffer();
        SendString("AT+FUP\r");
        Wait(250);
        ComClose();
        Wait(250);
        /* reopen at standard baudrate */
        if( ! ComOpenPollWait(pDevName,
                              BL600_FLASHUPLOAD_MODE_BAUDRATE,
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
VOID TBootLdrBL6XX::OnFailSwitchModuleToProgramMode(void)
{
    UpgradeUI_Message(FAIL_ENTER_PROGMODE);
}

//=============================================================================
//=============================================================================
bool TBootLdrBL6XX::CheckPlatform(
    UI32  nPlatform
    )
{
    if( nPlatform!= BLR_PLATFORM_ID_BL6XX )
    {
        return false;
    }
    /* first line of defence has passed, so now ask the unit if it is ok */
    return UWSUCCESS(SendPLATFORM(nPlatform));
}


/******************************************************************************/
// END OF FILE
/******************************************************************************/


