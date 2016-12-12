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
#include <stdio.h>

#include "AtUpgradeUI.h"
#include "UwFlash.h"
#include "Misc.h"

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
AtUpgradeUI *gpAtUpgradeUI = NULL;

#if DEBUGLEVEL > 2
static char baMsg[2048];
#endif

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
AtUpgradeUI::AtUpgradeUI()
{
    mLangId  = ATUPG_LANGID_ENGLISH;
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
AtUpgradeUI::~AtUpgradeUI()
{
}

//=============================================================================
//=============================================================================
UI32 AtUpgradeUI::TimeElapsed(UI32 nTickStart)
{
    UI32 nTickNow;
    UI32 nElapsed;

    nTickNow=MiscGetTickCount();
    if(  nTickNow >=  nTickStart )
    {
        nElapsed =  nTickNow -  nTickStart;
    }
    else
    {
        nElapsed = 0xFFFFFFFF -  nTickStart;
        nElapsed += nTickNow;
    }
    return  nElapsed;
}

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE AtUpgradeUI::Sleep(UI32 nSleepMs, bool fDisplayProgress)
{
    UI32 nTickStart=MiscGetTickCount();
    UI32 nElapsed;

    if(fDisplayProgress)
    {
        InitAndShowProgressBar(0,nSleepMs);
    }
    nElapsed = TimeElapsed(nTickStart);
    while( nElapsed < nSleepMs )
    {
        UpdateProgressBar(nElapsed);
        if( MiscPollBackground() )
        {
            if(fDisplayProgress)
            {
                HideProgressBar();
            }
            return UWRESULTCODE_UWF_USER_CLOSE;
        }
        nElapsed = TimeElapsed(nTickStart);
    }
    if(fDisplayProgress)
    {
        HideProgressBar();
    }
    MiscPollBackground();
    return UWRESULTCODE_SUCCESS;
}

/*=============================================================================*/
/*=============================================================================*/
char * AtUpgradeUI::GetText(int nMsgId)
{
    static char baMsg[16];
    sprintf(baMsg,"%d",nMsgId);
    return baMsg;
}

/*=============================================================================*/
/*=============================================================================*/
void AtUpgradeUI::Message(int nMsgId)
{
    if(nMsgId < ENUM_ATUPG_MSGID(LAST_ID))
    {
#if DEBUGLEVEL <= 2
        char baMsg[16];
#endif
        sprintf(baMsg,"%d",nMsgId);
        WriteToMemo(baMsg);
    }
}

/*=============================================================================*/
/*=============================================================================*/
void AtUpgradeUI::Message(int nMsgId, int nParam)
{
    if(nMsgId < ENUM_ATUPG_MSGID(LAST_ID))
    {
#if DEBUGLEVEL <= 2
        char baMsg[32];
#endif
        sprintf(baMsg,"%d(%d)",nMsgId,nParam);
        WriteToMemo(baMsg);
    }
}

/*=============================================================================*/
/*=============================================================================*/
void AtUpgradeUI::Message(int nMsgId, int nParam1,int nParam2,int nParam3,int nParam4,int nParam5 )
{
    if(nMsgId < ENUM_ATUPG_MSGID(LAST_ID) )
    {
#if DEBUGLEVEL <= 2
        char baMsg[64];
#endif
        sprintf(baMsg,"%d(%d,%d,%d,%d,%d)",nMsgId,nParam1,nParam2,nParam3,nParam4,nParam5);
        WriteToMemo(baMsg);
    }
}

/*=============================================================================*/
/*=============================================================================*/
void AtUpgradeUI::Message(int nMsgId, char *pStr)
{
    if(nMsgId < ENUM_ATUPG_MSGID(LAST_ID) )
    {
#if DEBUGLEVEL <= 2
        char baMsg[256];
#endif
        sprintf(baMsg,"%d(%s)",nMsgId,pStr);
        WriteToMemo(baMsg);
    }
}

/*=============================================================================*/
/*=============================================================================*/
const char *UwgResultCodeString(UWRESULTCODE resCode)
{
    static char baMsg[16];
    sprintf(baMsg,"%d",resCode);
    return baMsg;
}
/******************************************************************************/
// END OF FILE
/******************************************************************************/


