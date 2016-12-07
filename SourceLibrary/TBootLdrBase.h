/******************************************************************************
**              Copyright (C) 2007 TDK Systems Europe Ltd
**
** Project:     WISM - Upgrade Class
**
** Module:      TBootLdrBase.h
**
*******************************************************************************/

#if !defined(_TBOOTLDRBASE_H)     /* prevent multiple inclusions */
#define _TBOOTLDRBASE_H

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "Target.h"
#include "project.h"
#include "UwgPublic.h"
#include "BlrPublic.h"
#include "BlrPlatforms.h"

/******************************************************************************/
// Defines
/******************************************************************************/

/******************************************************************************/
// Macros
/******************************************************************************/

#define ROUND_DOWN_TO_EVEN(_x_)         ((_x_) & (~1))
#define ROUND_UP_TO_EVEN(_x_)           (((_x_)+1) & (~1))

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

class TBootLdrBase
{
//Data
private:

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
    TBootLdrBase();
    virtual ~TBootLdrBase();

    virtual UWRESULTCODE DriveModuleResetLine(bool fAssert)=0;
    virtual UWRESULTCODE SwitchModuleToProgramMode(void)=0;

    virtual UWRESULTCODE OnOpen(
        void
        ) = 0;

    virtual void OnClose(
        void
        ) = 0;

    virtual bool CheckPlatform(
        UI32  nPlatform
        ) = 0;

    virtual UWRESULTCODE RegisterDevice(
        UI8  nFlashDevice,
        UI32 nBaseAddress,
        UI8  nNumOfBanks,
        UI32 nBankSize,
        UI8  nBankSelAlgoFuncId
        ) = 0;

    virtual UWRESULTCODE UnregisterDevice(
        UI8  nFlashDevice
        ) = 0;

    virtual UWRESULTCODE SetBank(
        UI8  nFlashDevice,
        UI8  nBank
        ) = 0;

    virtual UWRESULTCODE WriteBlockHeader(
        UI32  nOffset,
        UI32  nBlockLenBytes,
        UI32  nFlags
        ) = 0;

    virtual UWRESULTCODE WriteBlockEnd(
        void
        ) = 0;

    virtual UWRESULTCODE WriteBlock(
        UI8  *pData,
        UI32  nAddrOffset,
        UI32  nBlockLenBytes
        ) = 0;

    virtual UWRESULTCODE EraseBlock(
        UI32  nAddrOffsetStart,
        UI32  nBlockSize
        ) = 0;

    virtual UWRESULTCODE SectorMap(
        SBlrSectorMapItem *pSectorMapArray,
        UI32 nNumOfItems
        ) = 0;

    virtual UWRESULTCODE AutoSelectBank(
        UI8  nFlashDevice,
        UI8  *pBank
        ) = 0;

    virtual UWRESULTCODE Query(
        UI32  nQueryId,
        UI32  *pQueryVals,
        UI16  nNumOfVals
        ) = 0;

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
