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

#if !defined(_STDPUBLIC_H)     /* prevent multiple inclusions */
#define _STDPUBLIC_H

/******************************************************************************/
/* Include Files*/
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#if !defined(_TARGETSTD_H)
#error TargetStd.h not included in Target.h
#endif
/******************************************************************************/
/* Defines*/
/******************************************************************************/

/******************************************************************************/
/* Macros*/
/******************************************************************************/

#define Std__MALLOC(s)                  malloc((s))
#define Std__FREE(p)                    free((p))

#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
#define StdMALLOC(s)                    UwgCheckedMalloc((s))
#define StdFREE(p)                      UwgCheckedFree((p))
#else
#define StdMALLOC(s)                    malloc((s))
#define StdFREE(p)                      free((p))
#endif

#define StdMEMSET(a,b,c)                memset((a),(b),(c))
#define StdMEMCPY(a,b,c)                memcpy((a),(b),(c))
#define StdMEMCMP(a,b,c)                memcmp((a),(b),(c))
#define StdTOUPPER(c)                   toupper((c))
#define StdSPRINTF(x)                   sprintf x
#define StdVSNPRINTF(buf,len,fmt,ap)    vsnprintf((buf),(len),(fmt),(ap))
#define StdSTRCPY(dst, src)             strcpy((dst), (src))
#define StdSTRNCPY(dst, src, max)       strncpy((dst), (src), (max))
#define StdSTRLEN(s)                    strlen((s))
#define StdSTRCAT(s1, s2)               strcat((s1), (s2))
#define StdSTRCMP(s1, s2)               strcmp((s1), (s2))
#define StdSTRCHR(s1, c)                strchr((s1), (c))
#define StdISDIGIT(c)                   isdigit((c))
#define StdISXDIGIT(c)                  isxdigit((c))
#define StdISALPHA(c)                   isalpha((c))

#define StdOFFSETOF(s,e)                offsetof(s,e)
        /* Retrieves the offset of a member from the beginning of its parent structure */
        /* Requires include stddef.h */

/******************************************************************************/
/* Simple (non struct/union) Typedefs*/
/******************************************************************************/

/******************************************************************************/
/* Enum Typedefs*/
/******************************************************************************/

/******************************************************************************/
/* Forward declaration of Class, Struct & Unions*/
/******************************************************************************/

/******************************************************************************/
/* Class definitions*/
/******************************************************************************/

/******************************************************************************/
/* Struct definitions*/
/******************************************************************************/

/******************************************************************************/
/* Union definitions*/
/******************************************************************************/

/******************************************************************************/
/* Global Functions (API etc) exported for other modules*/
/******************************************************************************/
void * StdFOPEN ( const char * filename, const char * mode );
size_t StdFREAD ( void * ptr, size_t size, size_t count, void * stream );
int    StdFCLOSE( void * stream );



#ifdef __cplusplus
}
#endif

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
