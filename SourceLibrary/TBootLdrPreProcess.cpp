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

#include "TBootLdrPreProcess.h"
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
TBootLdrPreProcess::TBootLdrPreProcess()
{
    gAtConfig.mTotalEraseByteEquiv = 0;
    gAtConfig.mTotalWriteLen = 0;
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
TBootLdrPreProcess::~TBootLdrPreProcess()
{
}

/******************************************************************************/
/******************************************************************************/
// Global Functions Non Class Members
/******************************************************************************/
/******************************************************************************/


//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::DriveModuleResetLine(bool fAssert)
{
    return  UWRESULTCODE_SUCCESS;
}

//=============================================================================
// It assumes that the comport is open on entry
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::SwitchModuleToProgramMode(void)
{
    return  UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::OnOpen(
    void
    )
{
    return  UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
void TBootLdrPreProcess::OnClose(
    void
    )
{
}

//=============================================================================
//=============================================================================
bool TBootLdrPreProcess::CheckPlatform(
    UI32  nPlatform
    )
{
    return true;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::RegisterDevice(
    UI8  nFlashDevice,
    UI32 nBaseAddress,
    UI8  nNumOfBanks,
    UI32 nBankSize,
    UI8  nBankSelAlgoFuncId
    )
{
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::UnregisterDevice(
    UI8  nFlashDevice
    )
{
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::SetBank(
    UI8  nFlashDevice,
    UI8  nBank
    )
{
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
// This gets called once for each WRITE_BLOCK at the end
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::WriteBlockHeader(
    UI32  nOffset,
    UI32  nBlockLenBytes,
    UI32  nFlags
    )
{
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
// This gets called multiple times for each WRITE_BLOCK
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::WriteBlock(
    UI8  *pData,
    UI32  nAddrOffset,
    UI32  nBlockLenBytes
    )
{
    gAtConfig.mTotalWriteLen +=  nBlockLenBytes;
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
// This gets called once for each WRITE_BLOCK at the end
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::WriteBlockEnd(
    VOID
    )
{
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::EraseBlock(
    UI32  nAddrOffsetStart,
    UI32  nBlockSize
    )
{
    UI32 nTemp;
    nBlockSize += PREPROCESS_DEFAULT_SECTOR_SIZE;
    gAtConfig.mEraseDefEq   = PREPROCESS_DEFAULT_SECTOR_TIME_EQUIV;
    gAtConfig.mEraseDefSize = PREPROCESS_DEFAULT_SECTOR_SIZE;
    if(gAtConfig.mEraseDefSize)
    {
        //nTemp = ((nBlockSize*gAtConfig.mEraseDefEq)/gAtConfig.mEraseDefSize);
        nTemp = (nBlockSize/gAtConfig.mEraseDefSize);
        nTemp *= gAtConfig.mEraseDefEq;
        gAtConfig.mTotalEraseByteEquiv += nTemp;
    }
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::SectorMap(
    SBlrSectorMapItem *pSectorMapArray,
    UI32 nNumOfItems
    )
{
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::AutoSelectBank(
    UI8  nFlashDevice,
    UI8  *pBank
    )
{
    return UWRESULTCODE_SUCCESS;
}

//=============================================================================
//=============================================================================
UWRESULTCODE TBootLdrPreProcess::Query(
    UI32  nQueryId,
    UI32  *pQueryVals,
    UI16  nNumOfVals
    )
{
    return UWRESULTCODE_SUCCESS;
}


/******************************************************************************/
// END OF FILE
/******************************************************************************/


