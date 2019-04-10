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

#if !defined(_BLRPLATFORMS_H)     /* prevent multiple inclusions */
#define _BLRPLATFORMS_H

/******************************************************************************/
/* Defines*/
/******************************************************************************/

/*
This file is a list of all the different platforms that have a boot loader.
Each target boot loader is customised so that it responds with a ACK or ERROR
to a PLATFORM_ID command in the boot protocol.

The PLATFORM_IS command consists of a 4 byte field. Therefore all values
specified in the #defines below MUST expand to a 4 byte dword, which can
be specified as a decimal value, hex value or a 4 character string constant.

CONVENTIONS:-
(1) If the least significant byte of the ID is a 'X' (0x58) then it implies that
    the boot loader will not respond to READ commands
*/


#define BLR_PLATFORM_ID_LEGACY_WISM             0x3F3F3F3F  /* == '????' because it does not care */
#define BLR_PLATFORM_ID_VEHICLEUNIT             0x56553158  /* 'X1UV' */
#define BLR_PLATFORM_ID_WISM_2MB                0x20325357  /* ' 2SW' */
#define BLR_PLATFORM_ID_WISM_2MB_BANKED         0x42325357  /* 'B2SW' */
#define BLR_PLATFORM_ID_WISM_4MB_BANKED         0x42345357  /* 'B4SW' */
#define BLR_PLATFORM_ID_WISM_8MB                0x20385357  /* ' 8SW' */
#define BLR_PLATFORM_ID_BL600                   0x000B1600
#define BLR_PLATFORM_ID_BL620                   0x000B1620
#define BLR_PLATFORM_ID_BL6XX                   0x000B16FF  /* Any BL6xx platform with new MBR bootloader */
#define BLR_PLATFORM_ID_BT900                   0x000B7900
#define BLR_PLATFORM_ID_RM1XX                   0x58314D52  /* 'RM1X' */
#define BLR_PLATFORM_ID_RM1XX_PE                0x50314D52  /* 'RM1P' */
#define BLR_PLATFORM_ID_BL65X                   0x000B165F  /* Any BL65x platform with new laird bootloader */
#define BLR_PLATFORM_ID_BL654X                  0x000B654F  /* Any BL654 platform with new laird bootloader */

/*
#define BLR_PLATFORM_ID_
*/

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
