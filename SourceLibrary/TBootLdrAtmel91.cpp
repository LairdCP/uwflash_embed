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

#include "TBootLdrAtmel91.h"
#include "AtConfig.h"
#include "AtUpgradeUI.h"
#include "Misc.h"
#include "UwFlash.h"
#include "TSerialPortBase.h"
#include "BootProtocolV3.h"

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
TBootLdrAtmel91::TBootLdrAtmel91()
{
    int nIndex;

    mfLateOpenComPort = false;

    mApdComPort = NULL;
    mRxCount=0;
    mCurrentDevice=0;
    mCurrentBank=0;

    mpSerialOnRxData = new TBootLdrAtmelOnRxData(this);

    /* Set default atmel protocol command id */
    for(nIndex=0;nIndex<=ATMEL_MSGNDX_MAXINDEX;nIndex++)
    {
        mCmdId[nIndex]=ATMEL_MSGID_DEF_SYNC;
    }
    mCmdId[ATMEL_MSGNDX_SYNC]  =ATMEL_MSGID_DEF_SYNC;
    mCmdId[ATMEL_MSGNDX_ACK]   =ATMEL_MSGID_DEF_ACK;
    mCmdId[ATMEL_MSGNDX_ERASE] =ATMEL_MSGID_DEF_ERASE;
    mCmdId[ATMEL_MSGNDX_WRITE] =ATMEL_MSGID_DEF_WRITE;
    mCmdId[ATMEL_MSGNDX_READ]  =ATMEL_MSGID_DEF_READ;
    mCmdId[ATMEL_MSGNDX_DATA]  =ATMEL_MSGID_DEF_DATA;
    mCmdId[ATMEL_MSGNDX_ERROR] =ATMEL_MSGID_DEF_ERROR;
    mCmdId[ATMEL_MSGNDX_ATS]   =ATMEL_MSGID_DEF_ATS;
    mCmdId[ATMEL_MSGNDX_VERIFY]=ATMEL_MSGID_DEF_VERIFY;
    mCmdId[ATMEL_MSGNDX_NACK]  =ATMEL_MSGID_DEF_NACK;
    mCmdId[ATMEL_MSGNDX_RESET] =ATMEL_MSGID_DEF_RESET;
    mCmdId[ATMEL_MSGNDX_QUERY] =ATMEL_MSGID_DEF_QUERY;

    for(nIndex=0;nIndex<MAX_FLASH_DEVICES_IN_MODULE;nIndex++)
    {
        InitFlashDevice(&mFlashDevices[nIndex]);
    }

    mpTxData=NULL;
    SetBootLoaderBufferSize(DEFAULT_BOOT_LOADER_WRITE_BUFSIZE);
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
TBootLdrAtmel91::~TBootLdrAtmel91()
{
    int nIndex;

    if( mpTxData )
    {
        free(mpTxData);
        mpTxData=NULL;
    }

    for(nIndex=0;nIndex<MAX_FLASH_DEVICES_IN_MODULE;nIndex++)
    {
        DeleteFlashDevice(&mFlashDevices[nIndex]);
    }

    delete mpSerialOnRxData;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SetCommandId(UI8 nIndex,UI8 nCmdId)
{
    mCmdId[nIndex] = nCmdId;
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SetBootLoaderBufferSize(UI32 nNewSize)
{
    if( mpTxData )
    {
        free(mpTxData);
        mpTxData=NULL;
    }

    nNewSize = nNewSize & (~1); //round down to even

    mpTxData = (UI8 *)malloc(nNewSize+32);
    if(mpTxData==NULL)
    {
        mBootBufferSize =  0;
        return UWRESULTCODE_MALLOC_FAIL;
    }
    mBootBufferSize =  nNewSize;
    return  UWRESULTCODE_SUCCESS;
}


/******************************************************************************/
/******************************************************************************/
// Global Functions Non Class Members
/******************************************************************************/
/******************************************************************************/


//=============================================================================
//=============================================================================
void TBootLdrAtmel91::CopyUI32ToStream(UI8 *pStream, UI32 nVal)
{
    pStream[0]=(UI8)((nVal >> 0)  & 0xff);
    pStream[1]=(UI8)((nVal >> 8)  & 0xff);
    pStream[2]=(UI8)((nVal >> 16)  & 0xff);
    pStream[3]=(UI8)((nVal >> 24)  & 0xff);
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::CopyUI16ToStream(UI8 *pStream, UI16 nVal)
{
    pStream[0]=(UI8)((nVal >> 0)  & 0xff);
    pStream[1]=(UI8)((nVal >> 8)  & 0xff);
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::InitFlashDevice(SFlashDevice *pFlashDevice)
{
    pFlashDevice->mBaseAddress  = 0;
    pFlashDevice->mNumOfBanks = 0;
    pFlashDevice->mBankSize = 0;
    pFlashDevice->mBankSelAlgoFuncId = 0;
    pFlashDevice->mpSectorMap = NULL;
    pFlashDevice->mSectorMapItems = 0;
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::DeleteFlashDevice(SFlashDevice *pFlashDevice)
{
    pFlashDevice->mBaseAddress  = 0;
    pFlashDevice->mNumOfBanks = 0;
    pFlashDevice->mBankSize = 0;
    pFlashDevice->mBankSelAlgoFuncId = 0;
    pFlashDevice->mSectorMapItems = 0;

    if( pFlashDevice->mpSectorMap )
    {
        free(pFlashDevice->mpSectorMap);
    }
    pFlashDevice->mpSectorMap = NULL;
}

//=============================================================================
//=============================================================================
bool TBootLdrAtmel91::IsComOpen(void)
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
void TBootLdrAtmel91::ComClose()
{
    if( mApdComPort )
    {
        mApdComPort->Close();
        if( gAtConfig.mfVirtualPort)MiscSleep(PORT_OPEN_CLOSE_DELAY_MS);
        delete mApdComPort;
        mApdComPort = NULL;
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void TBootLdrAtmel91::OnSerialDataRx(int nCount)
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
bool TBootLdrAtmel91::ComOpen(char *pComName,
                              int nBaudrate,
                              int nParity,
                              int nStopBits,
                              int nDataBits)
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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
UI32 TBootLdrAtmel91::TimeElapsed(UI32 nTickStart)
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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void TBootLdrAtmel91::Wait(UI32 nDelayMs)
{
    UI32 nTickStart;

    nTickStart=MiscGetTickCount();
    while( TimeElapsed(nTickStart) < nDelayMs )
    {
        MiscPollApplicationEvents();
        MiscSleep(25);
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
bool TBootLdrAtmel91::ComOpenPollWait(char *pComName,int nBaudrate,int nParity, int nStopBits, int nDataBits,UI32 nPollTimeMs)
{
    bool fRetVal = false; //assume cannot open the port
    UI32 nTickStart;
    UI32 nElapsed;

    if( IsComOpen() )
    {
        ComClose();
    }

    nTickStart=MiscGetTickCount();
    gpAtUpgradeUI->InitAndShowProgressBar(0,nPollTimeMs);
    while( (TimeElapsed(nTickStart) < nPollTimeMs ) && (fRetVal == false) )
    {
        gpAtUpgradeUI->UpdateProgressBar(TimeElapsed(nTickStart));
        MiscPollApplicationEvents();

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

        if(! fRetVal )
        {
            MiscSleep(1000);
        }
    }
    gpAtUpgradeUI->HideProgressBar();

    return fRetVal;
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::SetBreak(void)
{
    if(mApdComPort)
    {
        mApdComPort->SetBreak();
    }
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::ClearBreak(void)
{
    if(mApdComPort)
    {
        mApdComPort->ClearBreak();
    }
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::DeAssertDTR(void)
{
    if(mApdComPort)
    {
        mApdComPort->DeassertDTR();
    }
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::AssertDTR(void)
{
    if(mApdComPort)
    {
        mApdComPort->AssertDTR();
    }
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::FlushRxBuffer(void)
{
    if(mApdComPort)
    {
        mApdComPort->FlushRxBuffer();
    }
    mRxCount=0;
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::FlushTxBuffer(void)
{
    if(mApdComPort)
    {
        mApdComPort->FlushTxBuffer();
    }
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::FlushRxTx(void)
{
    FlushRxBuffer();
    FlushTxBuffer();
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::SendString(char *pCmd)
{
    if(mApdComPort)
    {
        mApdComPort->PutString(pCmd);
#if DEBUGLEVEL >= 4
        {
          UI32 i;
          CHAR *pDst = mbaMessage;
          unsigned char *pSrc = pCmd;
          UI32 nLen = strlen(pCmd);
          for(i=0;i<nLen;i++)
          {
            sprintf(pDst,"%02X",*pSrc++);
            pDst+=2;
          }
          UpgradeUI_Message_S(TX_DATA, mbaMessage);
        }
#endif
    }
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::SendBlock(unsigned char *pTxBlock, unsigned short nLen)
{
    if(mApdComPort)
    {
        mApdComPort->PutBlock(pTxBlock, nLen);
#if DEBUGLEVEL >= 4
        {
          UI32 i;
          CHAR *pDst = mbaMessage;
          unsigned char *pSrc = pTxBlock;
          for(i=0;i<nLen;i++)
          {
            sprintf(pDst,"%02X",*pSrc++);
            pDst+=2;
          }
          UpgradeUI_Message_S(TX_DATA, mbaMessage);
        }
#endif
    }
}


//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::OnOpen(
    void
    )
{
    UWRESULTCODE resultCode;
    INT nTryCount;
    char baLine[32];

    //Open the comport at specified parameters
    if( ! mfLateOpenComPort )
    {
        char *pDevName;
        gAtConfig.SerialPortNameGet(&pDevName);
        if( ! ComOpen(pDevName,
                   gAtConfig.mUserCommParms.mnBaudrate,
                   gAtConfig.mUserCommParms.mnParity,
                   gAtConfig.mUserCommParms.mnStopBits,
                   gAtConfig.mUserCommParms.mnDataBits) )
        {
            return UWRESULTCODE_UWF_COMPORT_OPENFAIL;
        }
    }

    //Now check if the module is responding ...
    nTryCount=0;
    while(nTryCount++<MAX_SYNC_RETRIES)
    {
        //Now arrange for the module to go into flash program mode
        /* now switch to program mode */
        resultCode = SwitchModuleToProgramMode();

        if(UWFAIL(resultCode))
        {
            if( resultCode == UWRESULTCODE_UWF_USER_ABORT )
            {
                break;
            }
            else
            {
                UpgradeUI_Message(FAIL_TO_ENTER_PROGMODE);
            }
        }
        else
        {
            if( mfLateOpenComPort )
            {
                char *pDevName;
                gAtConfig.SerialPortNameGet(&pDevName);

                if( gAtConfig. mfVirtualPort )
                {
                    if( ! ComOpenPollWait(pDevName,
                               gAtConfig.mUserCommParms.mnBaudrate,
                               gAtConfig.mUserCommParms.mnParity,
                               gAtConfig.mUserCommParms.mnStopBits,
                               gAtConfig.mUserCommParms.mnDataBits,
                               WITS_VIRTUAL_PORT_WAIT_TIME_MS) )
                    {
                        return UWRESULTCODE_UWF_COMPORT_OPENFAIL;
                    }
                }
                else
                {
                    if( ! ComOpen(pDevName,
                               gAtConfig.mUserCommParms.mnBaudrate,
                               gAtConfig.mUserCommParms.mnParity,
                               gAtConfig.mUserCommParms.mnStopBits,
                               gAtConfig.mUserCommParms.mnDataBits) )
                    {
                        return UWRESULTCODE_UWF_COMPORT_OPENFAIL;
                    }
                }
            }

            resultCode = DoSynchronisation();
            if(UWFAIL(resultCode))
            {
                UpgradeUI_Message(FAIL_SYNCHONISE_RETRY);
            }
            else
            {
                break;
            }
        }
    }
    if(UWFAIL(resultCode))
    {
        if( resultCode != UWRESULTCODE_UWF_USER_ABORT )
        {
            UpgradeUI_Message(FAIL_BOOTMODE);
            OnFailSwitchModuleToProgramMode();
        }
        return resultCode;
    }

    sprintf(baLine,"%d.%d",mVersionMajor,mVersionMinor);

    UpgradeUI_Message_S(SYNCHONISED,baLine);
    //UpgradeUI_Message(SYNCHONISED);
    return  UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::OnClose(
    void
    )
{
    /* Send BREAK so that it ends up in normal AT mode */
    Wait(500);
    SetBreak();
    Wait(1000);
    ClearBreak();

    if(IsComOpen())
    {
        ComClose();
    }
}

//=============================================================================
//=============================================================================
bool TBootLdrAtmel91::CheckPlatform(
    UI32  nPlatform
    )
{
    return false;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::RegisterDevice(
    UI8  nFlashDevice,
    UI32 nBaseAddress,
    UI8  nNumOfBanks,
    UI32 nBankSize,
    UI8  nBankSelAlgoFuncId
    )
{
    SFlashDevice *pFlashDevice=&mFlashDevices[nFlashDevice] ;

#if DEBUGLEVEL >= 3
    sprintf(mbaMessage,"Device=%u Addr=0x%08X Banks=%u BankSize=0x%x (%u)",nFlashDevice,nBaseAddress,nNumOfBanks,nBankSize,nBankSize);
    UpgradeUI_Message_S(REGISTER_DEVICE, mbaMessage);
#endif

    if(nFlashDevice >= MAX_FLASH_DEVICES_IN_MODULE)
    {
        return UWRESULTCODE_UWF_INVALID_FLASH;
    }

    pFlashDevice->mBaseAddress = nBaseAddress;
    pFlashDevice->mNumOfBanks = nNumOfBanks;
    pFlashDevice->mBankSize = nBankSize;
    pFlashDevice->mBankSelAlgoFuncId = nBankSelAlgoFuncId;

    mCurrentDevice = nFlashDevice;
    mCurrentBank = 0;

    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::UnregisterDevice(
    UI8  nFlashDevice
    )
{
    SFlashDevice *pFlashDevice=&mFlashDevices[nFlashDevice] ;

#if DEBUGLEVEL >= 3
    sprintf(mbaMessage,"Device=%u",nFlashDevice);
    UpgradeUI_Message_S(UNREGISTER_DEVICE, mbaMessage);
#endif

    if(nFlashDevice >= MAX_FLASH_DEVICES_IN_MODULE)
    {
        return UWRESULTCODE_UWF_INVALID_FLASH;
    }

    //Nothing to do here

    pFlashDevice=pFlashDevice;
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SetBank(
    UI8  nFlashDevice,
    UI8  nBank
    )
{
    SFlashDevice *pFlashDevice=&mFlashDevices[nFlashDevice] ;
#if DEBUGLEVEL >= 3
    sprintf(mbaMessage,"Device=%u Bank=%u",nFlashDevice,nBank);
    UpgradeUI_Message_S(SETBANK, mbaMessage);
#endif

    if(nFlashDevice >= MAX_FLASH_DEVICES_IN_MODULE)
    {
        return UWRESULTCODE_UWF_INVALID_FLASH;
    }
    if(nBank>=pFlashDevice->mNumOfBanks)
    {
        return UWRESULTCODE_UWF_INVALID_FLASH;
    }
    mCurrentBank = nBank;
    mCurrentDevice = nFlashDevice;
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SendERASE(
    SFlashDevice *pFlashDevice,
    UI32  nAddrOffset
    )
{
    UWRESULTCODE resultCode;
    UI8 *pWrite = &mTxCache[0];
    UI32 nPhysAddr;

    //Send the ERASE message
    FlushRxTx();
    *pWrite++ =mCmdId[ATMEL_MSGNDX_ERASE];
    nPhysAddr = pFlashDevice->mBaseAddress + nAddrOffset;
#if (DEBUGLEVEL >= 3)
    sprintf(mbaMessage,"PhysAddr=0x%08X (%u)",nPhysAddr,nPhysAddr);
    UpgradeUI_Message_S(ERASE_PACKET, mbaMessage);
#endif
    CopyUI32ToStream(pWrite, nPhysAddr);
    pWrite+=4;
    SendBlock(mTxCache,(pWrite-mTxCache));
    //Wait for an ACK
    resultCode=WaitForACK(ATMEL_MAX_WAIT_TIME_MS);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }

    return  resultCode;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SendQUERY(
    SFlashDevice *pFlashDevice,
    UI32  nQueryId,
    UI32 *pQueryVals,
    UI16 nNumOfVals
    )
{
    UWRESULTCODE resultCode;
    UI8 *pWrite = &mTxCache[0];
    UI16 i;

    /* Limit num of values so that it will fit in a 255 byte block */
    if(nNumOfVals>((255-6)/4))
    {
        nNumOfVals = ((255-6)/4);
    }
    UwfASSERT3(nNumOfVals<=255);

    //Send the QUERY message
    FlushRxTx();
    *pWrite++ =mCmdId[ATMEL_MSGNDX_QUERY];
    CopyUI32ToStream(pWrite, nQueryId);
    pWrite+=4;
    *pWrite++ =(UI8)nNumOfVals;
    for(i=0;i<nNumOfVals;i++)
    {
        CopyUI32ToStream(pWrite, pQueryVals[i]);
        pWrite+=4;
    }
    SendBlock(mTxCache,(pWrite-mTxCache));
    //Wait for an query response
    resultCode=WaitForQueryResp(ATMEL_MAX_QUERY_WAIT_TIME_MS);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }

    return  resultCode;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SendWRITE(
    SFlashDevice *pFlashDevice,
    UI32  nAddrOffset,
    UI8  nDataLenBytes
    )
{
    UWRESULTCODE resultCode;
    UI8 *pWrite = &mTxCache[0];
    UI32 nPhysAddr;

    //Send the WRITE message
    FlushRxTx();
    *pWrite++ =mCmdId[ATMEL_MSGNDX_WRITE];
    nPhysAddr = pFlashDevice->mBaseAddress + nAddrOffset+(mCurrentBank * pFlashDevice->mBankSize);
#if (DEBUGLEVEL >= 3)
    sprintf(mbaMessage,"PhysAddr=0x%08X (%u) ByteLen=%u",nPhysAddr,nPhysAddr,nDataLenBytes);
    UpgradeUI_Message_S(WRITE_PACKET, mbaMessage);
#endif
    CopyUI32ToStream(pWrite, nPhysAddr);
    pWrite+=4;
    *pWrite++= nDataLenBytes;
    SendBlock(mTxCache,(pWrite-mTxCache));
    //Wait for an ACK
#if (DEBUGLEVEL >= 3)
    UI32 nTickStart=MiscGetTickCount();
#endif
    resultCode=WaitForACK(ATMEL_MAX_WAIT_TIME_MS);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }
#if (DEBUGLEVEL >= 3)
    UI32 nTickElapsed=TimeElapsed( nTickStart );
    sprintf(mbaMessage,"AckWaitTime(Write)=%u",nTickElapsed);
    UpgradeUI_Message_S(GENERIC_MSG, mbaMessage);
#endif

    return  resultCode;
}

//=============================================================================
// Reads nBytes and if successful the data will be at &mRxCache[1]
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SendREAD(
    SFlashDevice *pFlashDevice,
    UI32  nAddrOffset,
    UI8  nBytes
    )
{
    UI32 nAbsPhysAddr;
    //Send the READ message
    FlushRxTx();
    mTxCache[0]=mCmdId[ATMEL_MSGNDX_READ];
    nAbsPhysAddr =  pFlashDevice->mBaseAddress + nAddrOffset + (mCurrentBank * pFlashDevice->mBankSize);
    CopyUI32ToStream(&mTxCache[1], nAbsPhysAddr);
    mTxCache[5]= (UI8)nBytes;
    SendBlock(mTxCache,6);
    //Wait for DATA
    return WaitForDATA(nBytes,ATMEL_MAX_DATA_WAIT_TIME_MS);
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SendDATA(
    UI8  nDataLenBytes
    )
{
    UWRESULTCODE resultCode;

    //Send the WRITE message
    FlushRxTx();
    mpTxData[0] =mCmdId[ATMEL_MSGNDX_DATA];
    mpTxData[nDataLenBytes+1] = CalcVerifyChecksum(&mpTxData[1],nDataLenBytes);
    SendBlock(mpTxData,(nDataLenBytes+2));
    //Wait for an ACK
#if (DEBUGLEVEL >= 3)
    UI32 nTickStart=MiscGetTickCount();
#endif
    resultCode=WaitForACK(ATMEL_MAX_WAIT_TIME_MS);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }
#if (DEBUGLEVEL >= 3)
    UI32 nTickElapsed=TimeElapsed( nTickStart );
    sprintf(mbaMessage,"AckWaitTime(DATA)=%u",nTickElapsed);
    UpgradeUI_Message_S(GENERIC_MSG, mbaMessage);
#endif

    return  resultCode;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::WriteBlockFlash(
    SFlashDevice *pFlashDevice,
    UI8  *pSrcData,
    UI32  nOffset,
    UI32  nLenBytes
    )
{
    UWRESULTCODE resultCode;
    UI8 *pData = pSrcData;
    UI32 nAddrOffset=nOffset;
    UI32  nBlockLenBytes=nLenBytes;

    UI8 *pWrite;
    UI32 nDataLen;
    UI32 nConsumeLen;
    UI32 nLastWriteDataLen=0xFFFFFFFF;

    UwfASSERT3(nBlockLenBytes<=TXCACHE_SIZE);

    nConsumeLen = (nBlockLenBytes > mBootBufferSize) ? mBootBufferSize :  nBlockLenBytes;

    pWrite = &mpTxData[1];
    if(  nAddrOffset & 0x1 )
    {
        //Offset is odd, so need to deal with it so that it becomes even
        if( nConsumeLen >= mBootBufferSize)
        {
            nDataLen=mBootBufferSize;
            nConsumeLen--;
        }
        else
        {
            nDataLen=nConsumeLen+1;
        }
        nDataLen = ROUND_DOWN_TO_EVEN(nDataLen);
        nAddrOffset = ROUND_DOWN_TO_EVEN(nAddrOffset);
        resultCode=SendREAD(pFlashDevice,nAddrOffset,1);
        if(UWFAIL(resultCode))
        {
            return resultCode;
        }
        *pWrite++ = mRxCache[1];
        StdMEMCPY(pWrite,pData,nConsumeLen);
        pWrite += nConsumeLen;

        pData  += nConsumeLen;
        nBlockLenBytes -= nConsumeLen;
    }
    else
    {
        //offset is even
        nConsumeLen = ROUND_DOWN_TO_EVEN( nConsumeLen );
        nDataLen = nConsumeLen;
        StdMEMCPY(pWrite,pData,nConsumeLen);
        pWrite += nConsumeLen;

        pData  += nConsumeLen;
        nBlockLenBytes -= nConsumeLen;

    }

    if(  nDataLen )
    {
        resultCode=SendWRITE(pFlashDevice,nAddrOffset, nDataLen);
        if(UWFAIL(resultCode))
        {
            return resultCode;
        }

#if (DEBUGLEVEL >= 3)
        sprintf(mbaMessage,"AddrOffset=0x%08X (%u) ByteLen=%u",nAddrOffset,nAddrOffset,nDataLen);
        UpgradeUI_Message_S(SEND_DATA, mbaMessage);
#endif
        resultCode=SendDATA(nDataLen);
        if(UWFAIL(resultCode))
        {
            return resultCode;
        }
        nLastWriteDataLen = nDataLen;
        nAddrOffset += nDataLen;

    }
    else
    {
        nLastWriteDataLen = 0;
    }

    while(nBlockLenBytes>1)
    {
        nConsumeLen = (nBlockLenBytes > mBootBufferSize) ? mBootBufferSize :  nBlockLenBytes;
        nConsumeLen = ROUND_DOWN_TO_EVEN( nConsumeLen );
        nDataLen = nConsumeLen;

        if(nDataLen != nLastWriteDataLen)
        {
            resultCode=SendWRITE(pFlashDevice,nAddrOffset, nDataLen);
            if(UWFAIL(resultCode))
            {
                return resultCode;
            }
            nLastWriteDataLen = nDataLen;
        }

        pWrite = &mpTxData[1];
        StdMEMCPY(pWrite,pData,nConsumeLen);
        pWrite += nConsumeLen;

#if (DEBUGLEVEL >= 3)
        sprintf(mbaMessage,"AddrOffset=0x%08X (%u) ByteLen=%u",nAddrOffset,nAddrOffset,nDataLen);
        UpgradeUI_Message_S(SEND_DATA, mbaMessage);
#endif
        resultCode=SendDATA(nDataLen);
        if(UWFAIL(resultCode))
        {
            return resultCode;
        }

        pData  += nConsumeLen;
        nBlockLenBytes -= nConsumeLen;
        nAddrOffset += nDataLen;
    }

    if( nBlockLenBytes )
    {
        UwfASSERT3(nBlockLenBytes==1);
        nConsumeLen = 1;
        nDataLen = 2;
        if(nDataLen != nLastWriteDataLen)
        {
            resultCode=SendWRITE(pFlashDevice,nAddrOffset, nDataLen);
            if(UWFAIL(resultCode))
            {
                return resultCode;
            }
            nLastWriteDataLen = nDataLen;
        }

        resultCode=SendREAD(pFlashDevice,nAddrOffset,nDataLen);
        if(UWFAIL(resultCode))
        {
            return resultCode;
        }
        UwfASSERT3((mRxCache[1]==0xFF)||(mRxCache[1]==*pData));
        pWrite = &mpTxData[1];
        *pWrite++ = *pData++;
        *pWrite++ = mRxCache[2];
        nBlockLenBytes -= nConsumeLen;
        nAddrOffset+=nDataLen;
#if (DEBUGLEVEL >= 3)
        sprintf(mbaMessage,"AddrOffset=0x%08X (%u) ByteLen=%u",nAddrOffset,nAddrOffset,nDataLen);
        UpgradeUI_Message_S(SEND_DATA, mbaMessage);
#endif
        resultCode=SendDATA(nDataLen);
        if(UWFAIL(resultCode))
        {
            return resultCode;
        }
    }

    msWriteBlock.mCurrentCount += nLenBytes;
    gpAtUpgradeUI->UpdateProgressBar(msWriteBlock.mCurrentCount);

    return  resultCode;
}

//=============================================================================
//=============================================================================
UI32  TBootLdrAtmel91::CalcVerifyChecksum(
    UI8  *pData,
    UI32  nBlockLenBytes
    )
{
    UI32 nCheckSum=0;
    while(nBlockLenBytes--)
    {
        nCheckSum += *pData++;
    }
    return nCheckSum;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::VerifyBlockFlash(
    SFlashDevice *pFlashDevice,
    UI8  *pData,
    UI32  nAddrOffset,
    UI32  nBlockLenBytes
    )
{
    UWRESULTCODE resultCode;
    UI32 nPhysAddr;

    UwfASSERT3(nBlockLenBytes<=TXCACHE_SIZE);

#if !defined(VERIFY_USING_READ)
    //Send the VERIFY message
    FlushRxTx();
    mTxCache[0]=mCmdId[ATMEL_MSGNDX_VERIFY];
    nPhysAddr = pFlashDevice->mBaseAddress+nAddrOffset + (mCurrentBank * pFlashDevice->mBankSize );
    CopyUI32ToStream(&mTxCache[1], nPhysAddr);
    CopyUI32ToStream(&mTxCache[5], nBlockLenBytes);
    CopyUI32ToStream(&mTxCache[9], CalcVerifyChecksum(pData,nBlockLenBytes));
    SendBlock(mTxCache,13);
    //Wait for an ACK
    resultCode=WaitForACK(ATMEL_MAX_WAIT_TIME_MS);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }

#else

    ---------------------
    This CODE is untested
    ---------------------

    if( nBlockLenBytes > 255 )
    {
        return UWRESULTCODE_UWF_WRITE_TOO_LARGE;
    }

    //Send the READ message
    FlushRxTx();
    mTxCache[0]=mCmdId[ATMEL_MSGNDX_READ];
    nPhysAddr = pFlashDevice->mBaseAddress+nAddrOffset + (mCurrentBank * pFlashDevice->mBankSize );
    CopyUI32ToStream(&mTxCache[1], nPhysAddr);
    mTxCache[5]= (UI8)nBlockLenBytes;
    SendBlock(mTxCache,6);
    //Wait for an DATA
    resultCode=WaitForDATA(nBlockLenBytes,ATMEL_MAX_DATA_WAIT_TIME_MS);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }
    if( StdMEMCMP( pData, &mRxCache[1], nBlockLenBytes ) != 0 )
    {
        return UWRESULTCODE_UWF_VERIFY_FAIL;
    }

    //Update the progress bar
    msWriteBlock.mCurrentCount += nBlockLenBytes;
    gpAtUpgradeUI->UpdateProgressBar(msWriteBlock.mCurrentCount);
#endif

    return  UWRESULTCODE_SUCCESS;
}

//=============================================================================
// This gets called once for each WRITE_BLOCK at the end
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::WriteBlockHeader(
    UI32  nOffset,
    UI32  nBlockLenBytes,
    UI32  nFlags
    )
{
    //SFlashDevice *pFlashDevice = &mFlashDevices[mCurrentDevice];

#if DEBUGLEVEL >= 3
    sprintf(mbaMessage,"Offset=0x%x (%u) Bytes=0x%x (%u) Flags=0x%x",nOffset,nOffset,nBlockLenBytes,nBlockLenBytes,nFlags);
    UpgradeUI_Message_S(WRITEBLOCKHEADER, mbaMessage);
#else
    UpgradeUI_Message(WRITING);
#endif

    msWriteBlock.mOffset = nOffset;
    msWriteBlock.mBlockLenBytes = nBlockLenBytes;
    msWriteBlock.mFlags = nFlags;
    msWriteBlock.mCurrentCount = 0;

    //Display the progress bar
#if defined(VERIFY_USING_READ)
    gpAtUpgradeUI->InitAndShowProgressBar(0,(msWriteBlock.mBlockLenBytes<<1));
#else
    gpAtUpgradeUI->InitAndShowProgressBar(0,(msWriteBlock.mBlockLenBytes));
#endif
    gpAtUpgradeUI->UpdateProgressBar(msWriteBlock.mCurrentCount);

    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
// This gets called multiple times for each WRITE_BLOCK
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::WriteBlock(
    UI8  *pData,
    UI32  nAddrOffset,
    UI32  nBlockLenBytes
    )
{
    UWRESULTCODE resultCode;
    SFlashDevice *pFlashDevice = &mFlashDevices[mCurrentDevice];

#if (DEBUGLEVEL >= 3) && defined(DISPLAY_WRITE_BLOCK_INFO)
    sprintf(mbaMessage,"Offset=0x%x (%u) Bytes=0x%x (%u)",nAddrOffset,nAddrOffset,nBlockLenBytes,nBlockLenBytes);
    UpgradeUI_Message_S(WRITEBLOCK, mbaMessage);
#endif

    resultCode = WriteBlockFlash(pFlashDevice, pData, nAddrOffset , nBlockLenBytes);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }

    resultCode = VerifyBlockFlash(pFlashDevice, pData, nAddrOffset , nBlockLenBytes);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }

    gUwfUpgrade.mOverallProgBytes +=  nBlockLenBytes;
    gpAtUpgradeUI->UpdateMainProgressBar(gUwfUpgrade.mOverallProgBytes);

    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
// This gets called once for each WRITE_BLOCK at the end
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::WriteBlockEnd(
    VOID
    )
{
    //Hide the progress bar
    gpAtUpgradeUI->HideProgressBar();

    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE  TBootLdrAtmel91::GetSectorStartAddress(SFlashDevice *pFlashDevice, UI32 nOffset, UI32 *pSectStart, UI32 *pSectorSize)
{
    SBlrSectorMapItem  *pSectorMap = pFlashDevice->mpSectorMap;
    UI32                nSectorMapItems = pFlashDevice->mSectorMapItems;
    UI32 nRunningOffset=0;
    UI32 nSectorNext;
    UI32 nSectors;

    while( nSectorMapItems-- )
    {
        *pSectorSize = pSectorMap->mSectorSize;
        nSectors=pSectorMap->mNumOfSectors;
        while(nSectors--)
        {
            nSectorNext = nRunningOffset + pSectorMap->mSectorSize;
            if( nSectorNext > nOffset )
            {
                *pSectStart = nRunningOffset;
                return UWRESULTCODE_SUCCESS;
            }
            nRunningOffset +=  pSectorMap->mSectorSize;
        }
        pSectorMap++;
    }
    //We should never reach here
    return  UWRESULTCODE_UWF_ADDR_OUTOF_RANGE;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::EraseBlock(
    UI32  nAddrOffsetStart,
    UI32  nBlockSize
    )
{
    UWRESULTCODE resultCode=UWRESULTCODE_SUCCESS;
    SFlashDevice *pFlashDevice = &mFlashDevices[mCurrentDevice];
    UI32 nSectorAddr;
    UI32 nSectorSize;
    UI32 nAddrOffsetEnd;
    UI32 nErasedCount;

#if DEBUGLEVEL >= 3
    sprintf(mbaMessage,"Offset=0x%x (%u) Bytes=0x%x (%u)",nAddrOffsetStart,nAddrOffsetStart,nBlockSize,nBlockSize);
    UpgradeUI_Message_S(ERASEBLOCK, mbaMessage);
#else
    UpgradeUI_Message(ERASING);
#endif

    if( (pFlashDevice->mpSectorMap == NULL) || (pFlashDevice->mSectorMapItems==0) )
    {
        return UWRESULTCODE_UWF_NO_SECTOR_MAP;
    }

    //Display the progress bar
    gpAtUpgradeUI->InitAndShowProgressBar(0,nBlockSize);

    nAddrOffsetStart += (mCurrentBank * pFlashDevice->mBankSize);

    resultCode = GetSectorStartAddress(pFlashDevice, nAddrOffsetStart, &nSectorAddr, &nSectorSize);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }
    nAddrOffsetEnd = nAddrOffsetStart + nBlockSize;
    nErasedCount=0;

    while( nSectorAddr < nAddrOffsetEnd )
    {
        //Send the ERASE message
        resultCode=SendERASE(pFlashDevice,nSectorAddr);
        if(UWFAIL(resultCode))
        {
            break;
        }
        nErasedCount += nSectorSize;
        nSectorAddr +=nSectorSize;

        gUwfUpgrade.mOverallProgBytes +=  ((nSectorSize * gAtConfig.mEraseDefEq)/gAtConfig.mEraseDefSize);
        gpAtUpgradeUI->UpdateMainProgressBar(gUwfUpgrade.mOverallProgBytes);

        if( nSectorAddr < nAddrOffsetEnd )
        {
            resultCode = GetSectorStartAddress(pFlashDevice, nSectorAddr, &nSectorAddr, &nSectorSize);
            if(UWFAIL(resultCode))
            {
                break;
            }
        }

        //Update the progress bar
        gpAtUpgradeUI->UpdateProgressBar(nErasedCount);
    }

    //Hide the progress bar
    gpAtUpgradeUI->HideProgressBar();

    return resultCode;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::Query(
    UI32  nQueryId,
    UI32  *pQueryVals,
    UI16  nNumOfVals
    )
{
    UWRESULTCODE resultCode;
    CHAR *pDst =  mbaMessage;
    INT i;
    SFlashDevice *pFlashDevice = &mFlashDevices[mCurrentDevice];


    sprintf(pDst,"id=0x%x ",nQueryId);
    pDst = mbaMessage + strlen(mbaMessage);
    for( i =0; i<nNumOfVals; i++)
    {
        sprintf(pDst,"[%u]=0x%x ",i,pQueryVals[i]);
        pDst = mbaMessage + strlen(mbaMessage);
    }
    UpgradeUI_Message_S(QUERY, mbaMessage);

    //Send the ERASE message
    resultCode=SendQUERY(pFlashDevice,nQueryId,pQueryVals,2);
    if(UWSUCCESS(resultCode))
    {
        /* Display the response in mRxCache */
        memcpy(mbaMessage,&mRxCache[2],mRxCache[1]);
        mbaMessage[mRxCache[1]]=0;
        UpgradeUI_Message_S(QUERYRESP, mbaMessage);
    }

    //A query will always return success so as not to stop the process
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SectorMap(
    SBlrSectorMapItem *pSectorMapArray,
    UI32 nNumOfItems
    )
{
    SFlashDevice *pFlashDevice = &mFlashDevices[mCurrentDevice];
    SBlrSectorMapItem *pMap;
    UI32 nDeviceSize=0;
    UI32 nItems;

#if DEBUGLEVEL >= 3
    sprintf(mbaMessage,"Items=0x%x (%u) ",nNumOfItems,nNumOfItems);
    UpgradeUI_Message_S(SECTORMAP, mbaMessage);
#endif

    if( (pFlashDevice->mpSectorMap != NULL) || (pFlashDevice->mSectorMapItems>0) )
    {
        return UWRESULTCODE_UWF_SECTOR_MAP_EXISTS;
    }

    //Validate the map
    nItems = nNumOfItems;
    pMap   = pSectorMapArray;
    while(nItems--)
    {
        nDeviceSize +=  pMap->mNumOfSectors * pMap->mSectorSize;
        pMap++;
    }
    if( nDeviceSize !=  (pFlashDevice->mNumOfBanks * pFlashDevice->mBankSize) )
    {
        return UWRESULTCODE_UWF_SECTOR_MAP_MISMATCH;
    }

    //It is consistant so copy the map over to the device object
    pFlashDevice->mpSectorMap = (SBlrSectorMapItem *)StdMALLOC(sizeof(SBlrSectorMapItem)*nNumOfItems);
    if(pFlashDevice->mpSectorMap==NULL)
    {
        return  UWRESULTCODE_MALLOC_FAIL;
    }
    StdMEMCPY( pFlashDevice->mpSectorMap,pSectorMapArray,(sizeof(SBlrSectorMapItem)*nNumOfItems));
    pFlashDevice->mSectorMapItems = nNumOfItems;

    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::AutoSelectBank(
    UI8  nFlashDevice,
    UI8  *pBank
    )
{
    SFlashDevice *pFlashDevice=&mFlashDevices[nFlashDevice] ;
#if DEBUGLEVEL >= 3
    sprintf(mbaMessage,"%u",nFlashDevice);
    UpgradeUI_Message_S(AUTO_SELECT_BANK_FROM, mbaMessage);
#endif

    if(nFlashDevice >= MAX_FLASH_DEVICES_IN_MODULE)
    {
        return UWRESULTCODE_UWF_INVALID_FLASH;
    }

    *pBank = 0xFF;   //AT THIS POINT ASK DEVICE FOR AUTO SELECTION

#if DEBUGLEVEL >= 3
    sprintf(mbaMessage,"%u",*pBank);
    UpgradeUI_Message_S(AUTO_SELECT_BANK, mbaMessage);
#endif

    pFlashDevice=pFlashDevice;
    return UWRESULTCODE_CODE_TOBE_WRITTEN;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::WaitForNbytes(UI32 nBytes, UI32 nWaitMs)
{
    UWRESULTCODE resultCode;
    UI32 nTickStart=MiscGetTickCount();

    nTickStart=MiscGetTickCount();
    while( GetRxBytes() < nBytes)
    {
        if( MiscPollApplicationEvents() )
        {
            return UWRESULTCODE_UWF_USER_CLOSE;
        }
        if(  TimeElapsed( nTickStart ) >  nWaitMs )
        {
            return UWRESULTCODE_UWF_RESP_TIMEOUT;
        }
    }
#if DEBUGLEVEL >= 4
    {
      UI32 i;
      CHAR *pDst = mbaMessage;
      CHAR *pSrc = mRxCache;
      for(i=0;i<nBytes;i++)
      {
        sprintf(pDst,"%02X",*pSrc++);
        pDst+=2;
      }
      UpgradeUI_Message_S(RX_DATA, mbaMessage);
    }
#endif
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
void TBootLdrAtmel91::OnUnexpectedMessage(void)
{
    if(mRxCache[0] == mCmdId[ATMEL_MSGNDX_ERROR])
    {
        switch( mRxCache[1] )
        {
        case ATMEL_ERROR_WRITE: StdSTRCPY(mbaMessage,"Flash Write Error");break;
        case ATMEL_ERROR_READ: StdSTRCPY(mbaMessage,"Flash Read Error");break;
        case ATMEL_ERROR_ERASE: StdSTRCPY(mbaMessage,"Flash Erase Sector Error");break;
        case ATMEL_ERROR_UNRECOGNISED: StdSTRCPY(mbaMessage,"Unrecognised command");break;
        case ATMEL_ERROR_PLATFORM: StdSTRCPY(mbaMessage,"Platform ID mismatch");break;
        default: StdSTRCPY(mbaMessage,"Unknown error");break;
        }
        UpgradeUI_Message_S(ATMELERROR, mbaMessage);
    }
    else if(mRxCache[0] == mCmdId[ATMEL_MSGNDX_ACK])
    {
        UpgradeUI_Message(UNEXPECTED_ACK);
    }
    else if(mRxCache[0] == mCmdId[ATMEL_MSGNDX_NACK])
    {
        UpgradeUI_Message(UNEXPECTED_NACK);
    }
    else if(mRxCache[0] == mCmdId[ATMEL_MSGNDX_ATS])
    {
        UpgradeUI_Message(UNEXPECTED_ATS);
    }
    else if(mRxCache[0] == mCmdId[ATMEL_MSGNDX_DATA])
    {
        UpgradeUI_Message(UNEXPECTED_DATA);
    }
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::WaitForATS(UI32 nWaitMs)
{
    UWRESULTCODE resultCode;

    resultCode = WaitForNbytes(ATMEL_MSGLEN_ATS,nWaitMs);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }
    //got a correctly sized message so check if it is correct type
    if( mRxCache[0] != mCmdId[ATMEL_MSGNDX_ATS] )
    {
        OnUnexpectedMessage();
        return UWRESULTCODE_UWF_NOT_ATS;
    }

    mVersionMajor =  mRxCache[6];
    mVersionMinor =  mRxCache[7];

    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::WaitForACK(UI32 nWaitMs)
{
    UWRESULTCODE resultCode;

    resultCode = WaitForNbytes(ATMEL_MSGLEN_ACK,nWaitMs);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }
    //got a correctly sized message so check if it is correct type
    if( mRxCache[0] != mCmdId[ATMEL_MSGNDX_ACK] )
    {
        OnUnexpectedMessage();
        return UWRESULTCODE_UWF_NOT_ACK;
    }
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
// nBytes does NOT include the header
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::WaitForDATA(UI32 nBytes,UI32 nWaitMs)
{
    UWRESULTCODE resultCode;
    UI8 nChecksum;
    UI8 *pReadData;

    nBytes+=2; //The 'D' and the checksum at the end

    resultCode = WaitForNbytes(nBytes,nWaitMs);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }
    //got a correctly sized message so check if it is correct type
    if( mRxCache[0] != mCmdId[ATMEL_MSGNDX_DATA] )
    {
        OnUnexpectedMessage();
        return UWRESULTCODE_UWF_NOT_DATA;
    }

    //Check that the checksum is valid
    nChecksum=0;
    nBytes -= 2;
    pReadData=&mRxCache[1];
    while(nBytes--)
    {
        nChecksum += *pReadData++;
    }
    if(*pReadData != nChecksum)
    {
        return UWRESULTCODE_UWF_CHECKSUM_ERROR;
    }
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::SendPLATFORM(
    UI32  nPlatformId
    )
{
    UWRESULTCODE resultCode;
    UI8 *pWrite = &mTxCache[0];

    //Send the WRITE message
    FlushRxTx();
    *pWrite++ =mCmdId[ATMEL_MSGNDX_PLATFORM];
    CopyUI32ToStream(pWrite, nPlatformId);
    pWrite+=4;
    SendBlock(mTxCache,(pWrite-mTxCache));
    //Wait for an ACK
    resultCode=WaitForACK(ATMEL_MAX_WAIT_TIME_MS);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }

    return  resultCode;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::DoSynchronisation(void)
{
    UWRESULTCODE resultCode;

    //Send the SYNC message
    FlushRxTx();
    mTxCache[0]=mCmdId[ATMEL_MSGNDX_SYNC];
    SendBlock(mTxCache,1);
    resultCode=WaitForATS(ATMEL_MAX_WAIT_TIME_MS);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }

    //Send an ACK
    FlushRxTx();
    mTxCache[0]=mCmdId[ATMEL_MSGNDX_ACK];
    SendBlock(mTxCache,1);
    resultCode=WaitForACK(ATMEL_MAX_WAIT_TIME_MS);

    return resultCode;
}

//=============================================================================
// Wait for the response to a query
//=============================================================================
UWRESULTCODE TBootLdrAtmel91::WaitForQueryResp(UI32 nWaitMs)
{
    UWRESULTCODE resultCode;
    UI8 nBytes;
    UI16 nTotalBytes;

    /* wait for the resp id and the databytes len */
    resultCode = WaitForNbytes(2,nWaitMs);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }
    //got 2 bytes
    if( mRxCache[0] != mCmdId[ATMEL_MSGNDX_QUERY] )
    {
#if DEBUGLEVEL >= 4
        OnUnexpectedMessage();
#endif
        return UWRESULTCODE_UWF_NOT_QUERY_RESPONSE;
    }

    /* now we know how many bytes are going to come */
    resultCode = WaitForNbytes((2+mRxCache[1]),nWaitMs);
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }

    return UWRESULTCODE_SUCCESS;
}

/******************************************************************************/
// END OF FILE
/******************************************************************************/
