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

/*
   Set the DEBUGLEVEL to 1..4 to assist with debugging your port
*/
#define DEBUGLEVEL                                  (0)
/*
   If DEBUGLEVEL is non-zero then set the following to a value
   as high as say 1024 so that large messages can be formatted
   and presented over the UI interface that you can code for
   in the fil AtUpgradeUITarget.cpp
*/
#define ASSERT_MSG_BUF_LEN                          (0)


/*
   Make the following one of these which hard codes the
   module that is attached to your microcontroller
        TARGET_BT900
        TARGET_BL6XX
        TARGET_RM1XX
        TARGET_BL65X
        TARGET_RM1XX_PE

   based on which one you set, you will need to add the
   apppropriate TBootLdr?????.cpp file in your makefile or IDE
   for example, given TARGET_BL65X is the value below
   then the file to include is WirelessModule/TBootLdrBL65X.cpp
*/
#define TARGET_PLATFORM                             TARGET_BL65X
/*
   Set the baudrate as follows :-
        TARGET_BL65X            115200
        TARGET_BT900            115200
        TARGET_BL6XX            9600
        TARGET_RM1XX            9600
        TARGET_RM1XX_PE         9600
*/
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "8"

/*----------------------------------------------------------------------------*/
/* The following define is checked for in basetype.h                          */
/*----------------------------------------------------------------------------*/
#define TARGET_DEFINED
#endif /* prevent multiple inclusions */
