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

#if !defined(_BOOTPROTOCOLV3_H)     /* prevent multiple inclusions */
#define _BOOTPROTOCOLV3_H

/******************************************************************************/
// Defines
/******************************************************************************/


/* The following are for legacy devices */
#define ATMEL_MSGID_DEF_SYNC                 (0x80)
#define ATMEL_MSGID_DEF_ACK                  ('A')
#define ATMEL_MSGID_DEF_NACK                 ('N')
#define ATMEL_MSGID_DEF_ERASE                ('E')
#define ATMEL_MSGID_DEF_WRITE                ('W')
#define ATMEL_MSGID_DEF_READ                 ('R')
#define ATMEL_MSGID_DEF_DATA                 ('D')
#define ATMEL_MSGID_DEF_ERROR                ('F')
#define ATMEL_MSGID_DEF_ATS                  ('B')
#define ATMEL_MSGID_DEF_VERIFY               ('V')
#define ATMEL_MSGID_DEF_RESET                ('Z')
#define ATMEL_MSGID_DEF_QUERY                ('Q')


/* The following are for latest devices */
#define ATMEL_MSGID_WITS_SYNC                 (0x80)
#define ATMEL_MSGID_WITS_ACK                  ('a')
#define ATMEL_MSGID_WITS_NACK                 ('n')
#define ATMEL_MSGID_WITS_ERASE                ('e')
#define ATMEL_MSGID_WITS_WRITE                ('w')
#define ATMEL_MSGID_WITS_READ                 ('r')
#define ATMEL_MSGID_WITS_DATA                 ('d')
#define ATMEL_MSGID_WITS_ERROR                ('f')
#define ATMEL_MSGID_WITS_ATS                  ('b')
#define ATMEL_MSGID_WITS_VERIFY               ('v')
#define ATMEL_MSGID_WITS_PLATFORM             ('p')
#define ATMEL_MSGID_WITS_RESET                ('z')
#define ATMEL_MSGID_WITS_QUERY                ('q')


#define ATMEL_ERROR_WRITE                     (1)
#define ATMEL_ERROR_READ                      (2)
#define ATMEL_ERROR_ERASE                     (3)
#define ATMEL_ERROR_UNRECOGNISED              (4)
#define ATMEL_ERROR_PLATFORM                  (5)


#endif /* Prevention of multiple inclusion */
/******************************************************************************/
// END OF FILE
/******************************************************************************/
