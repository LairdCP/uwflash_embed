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

#if !defined(_TARGET_H)     /* prevent multiple inclusions */
#define _TARGET_H

#include "..\WirelessModule\TargetEmbed.h"

#define DEBUGLEVEL                                  (2)
#define ASSERT_MSG_BUF_LEN                          (1024)

#undef  UWF_FILENAME

#if 0
#define TARGET_PLATFORM                             TARGET_BL6XX
#define TARGET_PLATFORM_STR                         "TARGET_BL600"
#define UART_BAUDRATE_NORMAL_OPERATION              (9600)
#define SERIALPORT_DEVICE_NAME                      "9"
#define UWF_FILENAME                                "BL600.uwf"

#elif 0
#define TARGET_PLATFORM                             TARGET_BT900
#define TARGET_PLATFORM_STR                         "TARGET_BT900"
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "14"
#define UWF_FILENAME                                "BT900.uwf"

#elif 0
#define TARGET_PLATFORM                             TARGET_RM1XX
#define TARGET_PLATFORM_STR                         "TARGET_RM1XX"
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "1"
#define UWF_FILENAME                                "RM1XX.uwf"

#elif 0
#define TARGET_PLATFORM                             TARGET_RM1XX_PE
#define TARGET_PLATFORM_STR                         "TARGET_RM1XX_PE"
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "16"
#define UWF_FILENAME                                "RM1XX_PE.uwf"

#else
#define TARGET_PLATFORM                             TARGET_BL65X
#define TARGET_PLATFORM_STR                         "TARGET_BL65X"
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "16"
#define UWF_FILENAME                                "BL652.uwf"

#endif


/*----------------------------------------------------------------------------*/
/* The following define is checked for in basetype.h                          */
/*----------------------------------------------------------------------------*/
#define TARGET_DEFINED
#endif /* prevent multiple inclusions */
