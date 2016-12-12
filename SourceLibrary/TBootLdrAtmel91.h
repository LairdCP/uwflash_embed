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

#if !defined(_TBOOTLDRATMEL91_H)     /* prevent multiple inclusions */
#define _TBOOTLDRATMEL91_H

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "Target.h"
#include "project.h"
#include "TSerialPortBase.h"
#include "TBootLdrBase.h"
#include "AtConfig.h"
#include "BlrPublic.h"
#include "UwfUpgrade.h"
#include "AtUpgradeUI.h"

/******************************************************************************/
// Defines
/******************************************************************************/

#define notVERIFY_USING_READ

#define WITS_VIRTUAL_PORT_WAIT_TIME_MS      (15000)

#define FLASH_UNERASED_BYTE_VALUE           (0xFF)

#define MAX_FLASH_DEVICES_IN_MODULE          (8)

#define TXCACHE_SIZE                         0x8000
#define RXCACHE_SIZE                         0x8000
#define MAX_DISLAY_MESSAGE_LINE              4096
#define DEFAULT_BOOT_LOADER_WRITE_BUFSIZE    (128)

#define MAX_SYNC_RETRIES                     (3)

#define ATMEL_MAX_WAIT_TIME_MS               (2000)
#define ATMEL_MAX_DATA_WAIT_TIME_MS          (10000)
#define ATMEL_MAX_QUERY_WAIT_TIME_MS         (5000)

#define ATMEL_MSGNDX_MAXINDEX                (255)
#define ATMEL_MSGNDX_SYNC                    (0)
#define ATMEL_MSGNDX_ACK                     (1)
#define ATMEL_MSGNDX_ERASE                   (2)
#define ATMEL_MSGNDX_WRITE                   (3)
#define ATMEL_MSGNDX_READ                    (4)
#define ATMEL_MSGNDX_DATA                    (5)
#define ATMEL_MSGNDX_ERROR                   (6)
#define ATMEL_MSGNDX_ATS                     (7)
#define ATMEL_MSGNDX_VERIFY                  (8)
#define ATMEL_MSGNDX_PLATFORM                (9)
#define ATMEL_MSGNDX_NACK                    (10)
#define ATMEL_MSGNDX_RESET                   (11)
#define ATMEL_MSGNDX_QUERY                   (12)

#define ATMEL_MSGID_DEF_PLATFORM             (ATMEL_MSGID_DEF_SYNC)

#define ATMEL_MSGLEN_ATS                     (14)
#define ATMEL_MSGLEN_ACK                     (1)
/*
#define ATMEL_MSGID_
#define ATMEL_MSGID_
#define ATMEL_MSGID_
#define ATMEL_MSGID_
#define ATMEL_MSGID_
#define ATMEL_MSGID_
#define ATMEL_MSGID_
*/
/******************************************************************************/
// Macros
/******************************************************************************/

/******************************************************************************/
// Enum Typedefs
/******************************************************************************/

/******************************************************************************/
// Simple (non struct/union) Typedefs
/******************************************************************************/

/******************************************************************************/
// Forward declaration of Class, Struct & Unions
/******************************************************************************/

/******************************************************************************/
// Struct definitions
/******************************************************************************/

typedef struct SFlashDeviceTag
{
    UI32                mBaseAddress;
    UI8                 mNumOfBanks;
    UI32                mBankSize;
    UI8                 mBankSelAlgoFuncId;
    SBlrSectorMapItem  *mpSectorMap;
    UI32                mSectorMapItems;
}
SFlashDevice;

typedef struct SWriteBlockTag
{
    UI32                mOffset;
    UI32                mBlockLenBytes;
    UI32                mFlags;

    UI32                mCurrentCount;
}
SWriteBlock;

/******************************************************************************/
// Class definitions
/******************************************************************************/

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
class TBootLdrAtmel91 : TBootLdrBase
{
//Data
private:

//Data
protected:

//Data
public:
    TSerialPortBase  *mApdComPort;
    //Transmit/Receive UART low level buffer
    UI8             mTxCache[TXCACHE_SIZE];
    UI8             mRxCache[RXCACHE_SIZE];
    UI8            *mpTxData;
    UI32            mRxCount;
    SFlashDevice    mFlashDevices[MAX_FLASH_DEVICES_IN_MODULE];
    CHAR            mbaMessage[MAX_DISLAY_MESSAGE_LINE];

    UI8             mCurrentDevice;
    UI8             mCurrentBank;

    SWriteBlock     msWriteBlock;
    UI32            mBootBufferSize;
    UI8             mCmdId[ATMEL_MSGNDX_MAXINDEX+1];
    bool            mfLateOpenComPort;
    UI8             mVersionMajor;
    UI8             mVersionMinor;

    TSerialOnRxData *mpSerialOnRxData;

//Functions
private:

//Functions
protected:

//Functions
public:
    TBootLdrAtmel91();
    virtual ~TBootLdrAtmel91();

    bool IsComOpen(void);
    void ComClose(void);
    bool ComOpen(char *pComName,int nBaudrate,int nParity, int nStopBits, int nDataBits);
    bool ComOpenPollWait(char *pComName,int nBaudrate,int nParity, int nStopBits, int nDataBits,UI32 nPollTimeMs);
    void Wait(UI32 nPollTimeMs);
    void  OnSerialDataRx(int nCount);
    void  SetBreak(void);
    void  ClearBreak(void);
    void  DeAssertDTR(void);
    void  AssertDTR(void);
    void  FlushRxBuffer(void);
    void  FlushTxBuffer(void);
    void  FlushRxTx(void);
    void  SendString(char *pCmd);
    void  SendBlock(unsigned char *pTxBlock, unsigned short nLen);
    void  InitFlashDevice(SFlashDevice *pFlashDevice);
    void  DeleteFlashDevice(SFlashDevice *pFlashDevice);

    UWRESULTCODE GetSectorStartAddress(SFlashDevice *pFlashDevice, UI32 nOffset, UI32 *pSectStart, UI32 *pSectorSize);
    UI32 TimeElapsed(UI32 nTickStart);
    UI32 GetRxBytes(void) { return  mRxCount; };
    UWRESULTCODE WaitForNbytes(UI32 nBytes, UI32 nWaitMs);
    void OnUnexpectedMessage(void);
    UWRESULTCODE SetCommandId(UI8 nIndex,UI8 nCmdId);
    UWRESULTCODE WaitForATS(UI32 nWaitMs);
    UWRESULTCODE WaitForACK(UI32 nWaitMs);
    UWRESULTCODE WaitForDATA(UI32 nBytes,UI32 nWaitMs);
    UWRESULTCODE SendREAD(SFlashDevice *pFlashDevice,UI32  nAddrOffset,UI8 nBytes);
    UWRESULTCODE SetBootLoaderBufferSize(UI32 nNewSize);
    UWRESULTCODE SendERASE(SFlashDevice *pFlashDevice,UI32  nAddrOffset);
    UWRESULTCODE SendWRITE(SFlashDevice *pFlashDevice,UI32  nAddrOffset,UI8  nDataLenBytes);
    UWRESULTCODE SendDATA(UI8  nDataLenBytes);
    virtual UWRESULTCODE SendPLATFORM(UI32  nPlatformId);
    UWRESULTCODE SendQUERY(SFlashDevice *pFlashDevice,UI32  nQueryId,UI32 *pQueryVals,UI16 nNumOfVals);
    UWRESULTCODE WaitForQueryResp(UI32 nWaitMs);

    void CopyUI32ToStream(UI8 *pStream, UI32 nVal);
    void CopyUI16ToStream(UI8 *pStream, UI16 nVal);

    UI32  CalcVerifyChecksum(UI8  *pData, UI32  nBlockLenBytes);

    virtual UWRESULTCODE WriteBlockFlash(
        SFlashDevice *pFlashDevice,
        UI8  *pData,
        UI32  nAddrOffset,
        UI32  nBlockLenBytes
        );

    virtual UWRESULTCODE VerifyBlockFlash(
        SFlashDevice *pFlashDevice,
        UI8  *pData,
        UI32  nAddrOffset,
        UI32  nBlockLenBytes
        );


    virtual UWRESULTCODE DoSynchronisation(void);

    virtual UWRESULTCODE DriveModuleResetLine(bool fAssert)=0;
    virtual UWRESULTCODE SwitchModuleToProgramMode(void)=0;
    virtual VOID OnFailSwitchModuleToProgramMode(void)=0;

    virtual UWRESULTCODE OnOpen(
        void
        );

    virtual void OnClose(
        void
        );

    virtual bool CheckPlatform(
        UI32  nPlatform
        )=0;

    virtual UWRESULTCODE RegisterDevice(
        UI8  nFlashDevice,
        UI32 nBaseAddress,
        UI8  nNumOfBanks,
        UI32 nBankSize,
        UI8  nBankSelAlgoFuncId
        );

    virtual UWRESULTCODE UnregisterDevice(
        UI8  nFlashDevice
        );

    virtual UWRESULTCODE SetBank(
        UI8  nFlashDevice,
        UI8  nBank
        );

    virtual UWRESULTCODE WriteBlockHeader(
        UI32  nOffset,
        UI32  nBlockLenBytes,
        UI32  nFlags
        );

    virtual UWRESULTCODE WriteBlock(
        UI8  *pData,
        UI32  nAddrOffset,
        UI32  nBlockLenBytes
        );

    virtual UWRESULTCODE WriteBlockEnd(
        void
        );

    virtual UWRESULTCODE EraseBlock(
        UI32  nAddrOffsetStart,
        UI32  nBlockSize
        );

    virtual UWRESULTCODE SectorMap(
        SBlrSectorMapItem *pSectorMapArray,
        UI32 nNumOfItems
        );

    virtual UWRESULTCODE AutoSelectBank(
        UI8  nFlashDevice,
        UI8  *pBank
        );

    virtual UWRESULTCODE Query(
        UI32  nQueryId,
        UI32  *pQueryVals,
        UI16  nNumOfVals
        );
};

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
class TBootLdrAtmelOnRxData : public TSerialOnRxData
{
//Data
private:
     TBootLdrAtmel91 *mpOwner;
//Data
protected:

//Data
public:

//Functions
private:

//Functions
protected:

//Functions
public:
    TBootLdrAtmelOnRxData(TBootLdrAtmel91 *pThis){mpOwner=pThis;};
    virtual ~TBootLdrAtmelOnRxData(){};
    virtual void OnRxData(unsigned nCount){ if(mpOwner)mpOwner->OnSerialDataRx(nCount);};
};





/******************************************************************************/
// Union definitions
/******************************************************************************/

/******************************************************************************/
// Global Functions (API etc) exported for other modules
/******************************************************************************/

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
// END OF FILE
/******************************************************************************/
