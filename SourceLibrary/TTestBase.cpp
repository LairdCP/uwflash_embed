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

#include "TTestBase.h"
#include "Misc.h"
#include "TSerialPortBase.h"
#include "UwFlash.h"

/******************************************************************************/
// Local Defines
/******************************************************************************/

#define POLLMS      (500)
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
TTestBase::TTestBase()
{
    mApdComPort=NULL;
    mRxCount = 0;
    mpSerialOnRxData = new TTestBaseOnRxData(this);
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
TTestBase::~TTestBase()
{
    delete mpSerialOnRxData;
}



/******************************************************************************/
/******************************************************************************/
// Global Functions Non Class Members
/******************************************************************************/
/******************************************************************************/

//=============================================================================
//=============================================================================
bool TTestBase::IsComOpen(void)
{
    bool fIsOpen = false;

    if(mApdComPort)
    {
        fIsOpen = mApdComPort->IsOpen();
    }

    return fIsOpen;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool TTestBase::IsCtsAsserted(void)
{
    if( mApdComPort )
    {
        return mApdComPort->GetCtsState();
    }
    return false;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool TTestBase::IsDsrAsserted(void)
{
    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            return mApdComPort->GetDsrState();
        }
    }
    return false;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool TTestBase::IsDcdAsserted(void)
{
    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            return mApdComPort->GetDcdState();
        }
    }
    return false;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool TTestBase::IsRiAsserted(void)
{
    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            return mApdComPort->GetRiState();
        }
    }
    return false;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TTestBase::AssertDTR(void)
{
    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            mApdComPort->AssertDTR();
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TTestBase::DeassertDTR(void)
{
    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            mApdComPort->DeassertDTR();
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TTestBase::AssertRTS(void)
{
    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            mApdComPort->AssertRTS();
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TTestBase::DeassertRTS(void)
{
    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            mApdComPort->DeassertRTS();
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TTestBase::FlushRx(void)
{
    mRxCount=0;
    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            mApdComPort->FlushRxBuffer();
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TTestBase::FlushTx(void)
{
    if( mApdComPort )
    {
        if(mApdComPort->IsOpen())
        {
            mApdComPort->FlushTxBuffer();
        }
    }
}

//=============================================================================
//=============================================================================
void TTestBase::SendString(char *pCmd)
{
    if(mApdComPort)
    {
        mApdComPort->PutString(pCmd);
    }
}

//=============================================================================
//=============================================================================
void TTestBase::SendBlock(unsigned char *pTxBlock, unsigned short nLen)
{
    if(mApdComPort)
    {
        mApdComPort->PutBlock(pTxBlock, nLen);
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TTestBase::ComClose()
{
    if( mApdComPort )
    {
        mApdComPort->Close();
        if( gAtConfig.mfVirtualPort)MiscSleep(PORT_OPEN_CLOSE_DELAY_MS);
        delete mApdComPort;
        mApdComPort = NULL;
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool TTestBase::ComOpen(char *pComName,int nBaudrate,int nParity, int nStopBits, int nDataBits)
{
    bool fRetVal = false; //assume cannot open the port

    if( IsComOpen() )
    {
        ComClose();
    }

    //Create and initialise the apd comport object
    if(mApdComPort==NULL)mApdComPort = GetSerialPort(mpSerialOnRxData);
    if(mApdComPort)
    {
        fRetVal = mApdComPort->ComOpen(pComName,nBaudrate,nParity,nStopBits,nDataBits);
#if defined(CHECKPORT)
        if( ! fRetVal )
        {
            delete mApdComPort;
            mApdComPort = NULL;
            fRetVal = false;
        }
#endif
    }

    return fRetVal;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TTestBase::OnSerialDataRx(int nCount)
{
    //On entry assume that the cache buffer is empty
    unsigned readlen = nCount;
    unsigned thisread;

    if(mApdComPort)
    {
        while(readlen)
        {
            thisread = (readlen > RXCACHE_SIZE) ? RXCACHE_SIZE : readlen;
            mApdComPort->GetBlock((char *)&mRxCache[mRxCount], thisread);
            mRxCount += thisread;
            readlen -= thisread;
        }
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool TTestBase::IsInResponse(char *pString)
{
    UI32 nLen = strlen(pString);

    if((nLen==0)||(nLen>mRxCount))
    {
        return false;
    }

    /* response is bigger than pString */
    mRxCache[mRxCount]=0;
    return strstr((char *)mRxCache,pString) ? true : false;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
UWRESULTCODE TTestBase::WaitForCts(UI32 nMaxTimeMs)
{
    UI32 nWaitCount = 0;
    UI32 nMaxCount  = nMaxTimeMs/POLLMS ;

    /* Wait for CTS to be asserted */
    UpgradeUI_Message(WAIT_FOR_CTS);
    while( (!IsCtsAsserted()) && (nWaitCount < nMaxCount ) )
    {
        gpAtUpgradeUI->Sleep(POLLMS,false);
        nWaitCount++;
    }
    if( IsCtsAsserted() )
    {
        UpgradeUI_Message(MODULE_RTS_ASSERTED);
    }
    else
    {
        UpgradeUI_Message(NO_CTS_FROM_MODULE);
        return UWRESULTCODE_UWF_MODULE_NOT_READY;
    }
    return UWRESULTCODE_SUCCESS;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
UWRESULTCODE TTestBase::WaitForDsr(UI32 nMaxTimeMs)
{
    UI32 nWaitCount = 0;
    UI32 nMaxCount  = nMaxTimeMs/POLLMS;

    /* Wait for CTS to be asserted */
    UpgradeUI_Message(WAIT_FOR_DSR);
    while( (!IsDsrAsserted()) && (nWaitCount < nMaxCount ) )
    {
        gpAtUpgradeUI->Sleep(POLLMS,false);
        nWaitCount++;
    }
    if( IsDsrAsserted() )
    {
        UpgradeUI_Message(MODULE_DTR_ASSERTED);
    }
    else
    {
        UpgradeUI_Message(NO_DSR_FROM_MODULE);
        return UWRESULTCODE_UWF_MODULE_NOT_READY;
    }
    return UWRESULTCODE_SUCCESS;
}


/******************************************************************************/
// END OF FILE
/******************************************************************************/

