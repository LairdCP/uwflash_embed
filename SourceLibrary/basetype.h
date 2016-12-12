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

#if !defined(BASETYPE_H)     /* prevent multiple inclusions */
#define BASETYPE_H


/******************************************************************************/
/* Include Files*/
/******************************************************************************/
/*
WARNING:

  Do NOT inclue TARGET.H in this file.

  TARGET.H must be explicitly included by every .c file.
  See template files XxxPublic.c and XxxPrivate.c

*/

#if !defined(TARGET_DEFINED)
#error The header file TARGET.H must exist in local folder and must contain the define TARGET_DEFINED
#endif

/******************************************************************************/
/* Validation checks*/
/******************************************************************************/

#if !defined(DEBUGLEVEL)
#error Define DEBUGLEVEL to be one of 0,1,2,3,4 in the command line
#endif

/******************************************************************************/
/* Defines*/
/******************************************************************************/

#define CONST                   const
#define STATIC                  static
#define EXTERN                  extern
#define VOLATILE                volatile

#if defined(__cplusplus)
#   if !defined(__BCPLUSPLUS__)
#       define TRUE                 true
#       define FALSE                false
#   endif
#else
#   define TRUE                 1
#   define FALSE                0
#endif

#if !defined(NULL)
#define NULL                    ((void*) 0)
#endif

/******************************************************************************/
/* Macros*/
/******************************************************************************/
#define sizeofarray(x)              ( sizeof(x) / sizeof(x[0]) )

#define ELEMENTOFFSETOF(_t_,_e_)    ((UI16)(&(( _t_ *)0) -> _e_ ))

/******************************************************************************/
/* Simple (non struct/union) Typedefs*/
/******************************************************************************/

/*====================================================*/
/* The following allow for compatibility with third*/
/* party libraries*/
/*====================================================*/
typedef int                         INT;
typedef char                        CHAR;
typedef unsigned char               UCHAR;
typedef unsigned int                UINT;
typedef float                       FLOAT;
typedef double                      DOUBLE;
#if defined(__cplusplus)
#   if !defined(__BCPLUSPLUS__)
    typedef bool                    BOOL;
#   endif
#else
#   define BOOL                    unsigned char
#   define bool                    unsigned char
#endif

#if !defined(VOID)
#   define VOID                    void
#endif

/*====================================================*/
/* Flag variables*/
/*====================================================*/
#if defined(__cplusplus)
#   if !defined(BOOLEAN)
#   define BOOLEAN                 bool
#   endif
#else
#   define BOOLEAN                 unsigned char
#endif

/*====================================================*/
/* bit field variables */
/*====================================================*/

#if defined(INT_BITFIELDS)
#define BITFIELD                    INT
#else
#define BITFIELD                    UI16
#endif

/*====================================================*/
/* 8 bit variables*/
/*====================================================*/
typedef char                        I8;
typedef unsigned char               UI8;
typedef volatile char               VOLATILE_I8;
typedef volatile unsigned char      VOLATILE_UI8;

#if defined(DEF_BYTE)
    typedef unsigned char           BYTE;
#endif
/*====================================================*/
/* 16 bit variables*/
/*====================================================*/
typedef short                       I16;
typedef unsigned short              UI16;
typedef volatile short              VOLATILE_I16;
typedef volatile unsigned short     VOLATILE_UI16;

#if defined(DEF_WORD)
    typedef unsigned short          WORD;
#endif

/*====================================================*/
/* 32 bit variables*/
/*====================================================*/
#ifdef __linux__
#include <inttypes.h>
typedef int32_t                     I32;
typedef uint32_t                    UI32;
typedef unsigned int                UI32;
typedef volatile int32_t            VOLATILE_I32;
typedef volatile uint32_t           VOLATILE_UI32;
#else
typedef long                        I32;
typedef unsigned long               UI32;
typedef volatile long               VOLATILE_I32;
typedef volatile unsigned long      VOLATILE_UI32;
#endif // __linux__
typedef float                       F32;
typedef volatile float              VOLATILE_F32;

#if defined(DEF_DWORD)
    typedef unsigned long           DWORD;
#endif

/*====================================================*/
/* 64 bit variables*/
/*====================================================*/

typedef double                      F64;
typedef volatile double             VOLATILE_F64;

/* Comment the following four lines if the compiler*/
/* does not provide 64bit integers*/
#if defined(TGT_INT64_SUPPORT)
typedef __int64                     I64;
typedef unsigned __int64            UI64;
typedef volatile __int64            VOLATILE_I64;
typedef volatile unsigned __int64   VOLATILE_UI64;
#endif

/*====================================================*/
/* Pointer variables*/
/*====================================================*/
typedef void*                       PVOID;
typedef void**                      PPVOID;

/*The following are to be used to access memory mapped io ports*/
typedef volatile unsigned char      PORT8;
typedef volatile unsigned short     PORT16;
typedef volatile unsigned long      PORT32;

#if defined(LOWERCASE_BASETYPES)
typedef unsigned long               uint32;
typedef unsigned short              uint16;
typedef long                        int32;
typedef short                       int16;
typedef char                        int8;
typedef unsigned char               uint8;
#endif


/*====================================================*/
/* InLine stuff */
/*====================================================*/
#if defined(DISABLE_INLINE)
#   define UWINLINE
#else

#if defined(_WIN32)

#   if defined(__BORLANDC__)
        /* Borland */
#       define UWINLINE        __inline
#   else
        /* On Visual Studio */
#       define UWINLINE        _inline
#   endif

#elif defined(__ghs__)
    /* On GHS */
#   define UWINLINE         inline

#elif defined(__linux__)
#define UWLINLINE           inline

#else
    /* -----------------------------------------------------------------*/
    /* THIS HEADER FILE IS COMMON FOR ALL PROJECTS                      */
    /* -----------------------------------------------------------------*/
    /* Do NOT place a #define in this #else segment, but create         */
    /* a #elif defined( YOUR_ENVIROMENT ) above and place the UWINLINE  */
    /* define there                                                     */
    /* -----------------------------------------------------------------*/
#error Need to define UWINLINE for this compiler
#endif

#endif


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

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
