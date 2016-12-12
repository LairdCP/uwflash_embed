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

#if !defined(_BLRPRIVATE_H)     /* prevent multiple inclusions */
#define _BLRPRIVATE_H

/******************************************************************************/
/* Include Files*/
/******************************************************************************/

/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/* Defines*/
/******************************************************************************/

/******************************************************************************/
/* Macros*/
/******************************************************************************/

#if defined(UWG_SINGLE_HEAP_MANAGER)
#define BlrMALLOC(_x_)          UwgMalloc_((_x_))
#define BlrFREE(_x_)            UwgFree_((VOID **)(_x_))
#else
#define BlrMALLOC(_x_)          BlrMalloc_((_x_))
#define BlrFREE(_x_)            BlrFree_((VOID **)(_x_))
#endif

#define BlrASSERT(cond)         DEVASSERT(cond)
#define BlrMESSAGE(msg)         DEVMESSAGE(msg)
#define BlrFAIL(msg)            DEVFAIL(msg)
#define BlrTRACE(x)             DEVTRACE1(x)

#define BlrASSERT1(cond)        DEVASSERT1(cond)
#define BlrMESSAGE1(msg)        DEVMESSAGE1(msg)
#define BlrFAIL1(msg)           DEVFAIL1(msg)
#define BlrTRACE1(x)            DEVTRACE1(x)

#define BlrASSERT2(cond)        DEVASSERT2(cond)
#define BlrMESSAGE2(msg)        DEVMESSAGE2(msg)
#define BlrFAIL2(msg)           DEVFAIL2(msg)
#define BlrTRACE2(x)            DEVTRACE2(x)

#define BlrASSERT3(cond)        DEVASSERT3(cond)
#define BlrMESSAGE3(msg)        DEVMESSAGE3(msg)
#define BlrFAIL3(msg)           DEVFAIL3(msg)
#define BlrTRACE3(x)            DEVTRACE3(x)

#if DEBUGLEVEL>=4
    #define BlrASSERTPC(cond)   _DEVASSERT_ALWAYS_(cond)
    #define BlrMESSAGEPC(msg)   _DEVMESSAGE_ALWAYS_(msg)
    #define BlrFAILPC(msg)      _DEVFAIL_ALWAYS_(msg)
    #define BlrTRACEPC(x)       DEVTRACE3(x)

#else
    #define BlrASSERTPC(cond)   /* */
    #define BlrMESSAGEPC(msg)   /* */
    #define BlrFAILPC(msg)      /* */
    #define BlrTRACEPC(x)       /* */
#endif


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

#if !defined(UWG_SINGLE_HEAP_MANAGER)
VOID *BlrMalloc_( UI32 size );
VOID  BlrFree_( VOID **ppMemBlock );
#endif


/******************************************************************************/
/* BlrPrivate.c */
/******************************************************************************/

/*=============================================================================*/
/*=============================================================================*/
VOID
BlrPrvLock(
    VOID
    );

/*=============================================================================*/
/*=============================================================================*/
VOID
BlrPrvUnLock(
    VOID
    );



/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
