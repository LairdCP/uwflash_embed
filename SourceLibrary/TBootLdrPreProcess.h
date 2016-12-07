/******************************************************************************
**              Copyright (C) 2007 TDK Systems Europe Ltd
**
** Project:     WISM - Upgrade Class
**
** Module:      TBootLdrAPreProcess.h
**
*******************************************************************************/

#if !defined(_TBOOTLDRPREPROCESS_H)     /* prevent multiple inclusions */
#define _TBOOTLDRPREPROCESS_H

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "Target.h"
#include "project.h"
#include "TBootLdrBase.h"
#include "AtConfig.h"
#include "BlrPublic.h"
#include "UwfUpgrade.h"
#include "AtUpgradeUI.h"

/******************************************************************************/
// Defines
/******************************************************************************/

#define PREPROCESS_DEFAULT_SECTOR_SIZE          (0x10000)
#define PREPROCESS_DEFAULT_SECTOR_TIME_EQUIV    (300000/70)

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

class TBootLdrPreProcess : TBootLdrBase
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
    TBootLdrPreProcess();
    virtual ~TBootLdrPreProcess();

    virtual UWRESULTCODE DriveModuleResetLine(bool fAssert);
    virtual UWRESULTCODE SwitchModuleToProgramMode(void);

    virtual UWRESULTCODE OnOpen(
        void
        );

    virtual void OnClose(
        void
        );

    virtual bool CheckPlatform(
        UI32  nPlatform
        );

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
