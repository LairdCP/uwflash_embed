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

#if !defined(_TTestBase_H)     /* prevent multiple inclusions */
#define _TTestBase_H

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "Target.h"
#include "project.h"
#include "UwgPublic.h"
#include "TSerialPortBase.h"
#include "TTestBase.h"
#include "AtConfig.h"
#include "AtUpgradeUI.h"

/******************************************************************************/
// Defines
/******************************************************************************/

#define TXCACHE_SIZE                         0x8000
#define RXCACHE_SIZE                         0x8000

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

/******************************************************************************/
// Class definitions
/******************************************************************************/

class TTestBase
{
//Data
private:

//Data
protected:

//Data
public:
    TSerialPortBase *mApdComPort;
    //Transmit/Receive UART low level buffer
    UI8             mTxCache[TXCACHE_SIZE];
    UI8             mRxCache[RXCACHE_SIZE];
    UI32            mRxCount;

    TSerialOnRxData *mpSerialOnRxData;

//Functions
private:

//Functions
protected:

//Functions
public:
    TTestBase();
    virtual ~TTestBase();

    bool IsComOpen(void);
    void ComClose(void);
    bool ComOpen(char *pComName,int nBaudrate,int nParity, int nStopBits, int nDataBits);
    void OnSerialDataRx(int nCount);
    void AssertDTR(void);
    void DeassertDTR(void);
    void AssertRTS(void);
    void DeassertRTS(void);
    bool IsCtsAsserted(void);
    bool IsDsrAsserted(void);
    bool IsDcdAsserted(void);
    bool IsRiAsserted(void);
    void FlushRx(void);
    void FlushTx(void);
    void SendString(char *pCmd);
    void SendBlock(unsigned char *pTxBlock, unsigned short nLen);
    bool IsInResponse(char *pString);
    UWRESULTCODE WaitForCts(UI32 nMaxTimeMs);
    UWRESULTCODE WaitForDsr(UI32 nMaxTimeMs);

    virtual UWRESULTCODE ResetManual(
        UI32  nTestType
        ) = 0;

    virtual UWRESULTCODE ResetAuto(
        UI32  nTestType
        ) = 0;

    virtual UWRESULTCODE Initialise(
        UI32  nTestType,
        VOID *pTestData
        ) = 0;

    virtual UWRESULTCODE Terminate(
        VOID
        ) = 0;

    virtual UWRESULTCODE DoAllTests(
        VOID
        ) = 0;

    virtual UWRESULTCODE DoTest(
        UI32  nTestType,
        VOID *pTestData
        ) = 0;

};

class TTestBaseOnRxData : public TSerialOnRxData
{
//Data
private:
     TTestBase *mpOwner;
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
    TTestBaseOnRxData(TTestBase *pThis){mpOwner=pThis;};
    virtual ~TTestBaseOnRxData(){};
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
