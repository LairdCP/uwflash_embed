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

#if !defined(_ATCONFIG_H)     /* prevent multiple inclusions */
#define _ATCONFIG_H

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "Target.h"
#include "project.h"

/******************************************************************************/
// Defines
/******************************************************************************/

#define MAX_SERIAL_PORT_NAMELEN             24
#define PORT_OPEN_CLOSE_DELAY_MS            1000
#define MAX_TESTDATA_LEN                    64

/******************************************************************************/
// Macros
/******************************************************************************/

/******************************************************************************/
// Enum Typedefs
/******************************************************************************/

typedef enum
{
    //The values in this enum MUST match the item index values
    //in the radiogroup in the main form.

    UPGFILE_UWF                     = 0
}
UPGRADE_FILETYPE;

typedef enum
{
    TARGET_BT900                    = 0
   ,TARGET_BL6XX                    /* Any BL6xx based hardware with new bootloader */
   ,TARGET_RM1XX                    /* LoRa - Central BLE Role */
   ,TARGET_BL65X                    /* Any BL65x based hardware*/
   ,TARGET_BL654X                   /* Any BL654x based hardware*/
   ,TARGET_RM1XX_PE                 /* LoRa - Peripheral BLE Role */

   ,TARGET_BL600
   ,TARGET_BL620

   ,TARGET_PREPROCESS_MASK          = 0x8000
   ,TARGET_UNKNOWN                  = 0xFFFF
}
ETargetType;

/******************************************************************************/
// Simple (non struct/union) Typedefs
/******************************************************************************/

/******************************************************************************/
// Forward declaration of Class, Struct & Unions
/******************************************************************************/

/******************************************************************************/
// Struct definitions
/******************************************************************************/

typedef struct
{
    int                  mnBaudrate;
    int                  mnDataBits;
    int                  mnStopBits;
    int                  mnParity;
}
COMPARMS;

/******************************************************************************/
// Class definitions
/******************************************************************************/

class AtConfig
{
//Data
private:
    CHAR                 mbaSerialPortName[MAX_SERIAL_PORT_NAMELEN+1];

//Data
protected:

//Data
public:
    COMPARMS             mUserCommParms;
    bool                 mfAutomaticMode;
    bool                 mToggleDTR;
    ETargetType          mTargetType;
    bool                 mfVirtualPort;
    UI32                 mTotalEraseByteEquiv;
    UI32                 mTotalWriteLen;
    UI32                 mEraseDefEq;
    UI32                 mEraseDefSize;
    UI16                 mFirmwareFileChunkLen;
    INT                  mTestNum;
    CHAR                 mbaTestData[MAX_TESTDATA_LEN+1];

//Functions
private:

//Functions
protected:

//Functions
public:
    AtConfig();
    virtual ~AtConfig();

    void SerialPortNameGet(int *pNum);
    void SerialPortNameGet(char **ppName);
    void SerialPortNameSet(int nComNum);
    void SerialPortNameSet(char *pName);

};


/******************************************************************************/
// Union definitions
/******************************************************************************/

/******************************************************************************/
// Global Functions (API etc) exported for other modules
/******************************************************************************/
extern AtConfig  gAtConfig;

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
// END OF FILE
/******************************************************************************/
