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

#if !defined(_TARGETEMBED_H)     /* prevent multiple inclusions */
#define _TARGETEMBED_H


/*---------------------------------------------------------------------------*/
/* The following #defines need to exist for all builds                        */
/*---------------------------------------------------------------------------*/
#define ON_TARGET_PC
#define ON_SUBTARGET_UWFMAKER
#define ON_TARGET_WINDOWS


#define notDISABLE_INLINE

#define EXCLUDE_VERSION_CORE_HEADER

/*---------------------------------------------------------------------------*/
/* Include Module specific target defines */
/*---------------------------------------------------------------------------*/
#include "TargetUwg.h"
#include "TargetRbf.h"
#include "TargetStd.h"
#include "TargetFrm.h"
#include "TargetBlr.h"

#define notDISPLAY_WRITE_BLOCK_INFO

#define UWF_FIRMWAREFILE_MAX_LINE_LEN               (1024)

#define BLR_FIRMWARE_BUILD_NUMBER_OFFSET            (0xdeadc0de not used)
#define BLR_TARGET_PLATFORM_ID                      (0xdeadcode not used)

#define BT900_FLASHUPLOAD_MODE_BAUDRATE             (115200)
#define RM1XX_FLASHUPLOAD_MODE_BAUDRATE             (115200)
#define RM1XX_PE_FLASHUPLOAD_MODE_BAUDRATE          (115200)

#define UWF_FILENAME                                "*"

#define UART_BAUDRATE_BOOTLOADER                    (115200)

#define COMPORT_POLL_WAIT_TIME_MS                   (1000)


/*---------------------------------------------------------------------------*/
/* The following define is checked for in basetype.h                         */
/*---------------------------------------------------------------------------*/
#define TARGET_DEFINED
#endif /* prevent multiple inclusions */
