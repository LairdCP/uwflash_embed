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

#include "TBootLdrProtV3.h"
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
TBootLdrProtV3::TBootLdrProtV3()
{
    /* Alter the command id's */
    SetCommandId(ATMEL_MSGNDX_SYNC,ATMEL_MSGID_WITS_SYNC);
    SetCommandId(ATMEL_MSGNDX_ACK,ATMEL_MSGID_WITS_ACK);
    SetCommandId(ATMEL_MSGNDX_NACK,ATMEL_MSGID_WITS_NACK);
    SetCommandId(ATMEL_MSGNDX_ERASE,ATMEL_MSGID_WITS_ERASE);
    SetCommandId(ATMEL_MSGNDX_WRITE,ATMEL_MSGID_WITS_WRITE);
    SetCommandId(ATMEL_MSGNDX_READ,ATMEL_MSGID_WITS_READ);
    SetCommandId(ATMEL_MSGNDX_DATA,ATMEL_MSGID_WITS_DATA);
    SetCommandId(ATMEL_MSGNDX_ERROR,ATMEL_MSGID_WITS_ERROR);
    SetCommandId(ATMEL_MSGNDX_ATS,ATMEL_MSGID_WITS_ATS);
    SetCommandId(ATMEL_MSGNDX_VERIFY,ATMEL_MSGID_WITS_VERIFY);
    SetCommandId(ATMEL_MSGNDX_PLATFORM,ATMEL_MSGID_WITS_PLATFORM);
    SetCommandId(ATMEL_MSGNDX_RESET,ATMEL_MSGID_WITS_RESET);
    SetCommandId(ATMEL_MSGNDX_QUERY,ATMEL_MSGID_WITS_QUERY);
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
TBootLdrProtV3::~TBootLdrProtV3()
{
}



/******************************************************************************/
/******************************************************************************/
// Global Functions Non Class Members
/******************************************************************************/
/******************************************************************************/


/******************************************************************************/
// END OF FILE
/******************************************************************************/


