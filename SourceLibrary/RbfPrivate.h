/******************************************************************************
**              Copyright (C) 2005 Ezurio Ltd
**
** Project:     Universal Wireless
**
** Module:      RBFPRIVATE.H
**
** Notes:
**
*******************************************************************************/

#if !defined(_RBFPRIVATE_H)     /* prevent multiple inclusions */
#define _RBFPRIVATE_H

/******************************************************************************/
/* Include Files*/
/******************************************************************************/
#include "project.h"

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
#define RbfMALLOC(_x_)          UwgMalloc_((_x_))
#define RbfFREE(_x_)            UwgFree_((VOID **)(_x_))
#else
#define RbfMALLOC(_x_)          RbfMalloc_((_x_))
#define RbfFREE(_x_)            RbfFree_((VOID **)(_x_))
#endif


#define RbfASSERT(cond)         DEVASSERT(cond)
#define RbfMESSAGE(msg)         DEVMESSAGE(msg)
#define RbfCHECK(cond,msg)      DEVCHECK(cond,msg)
#define RbfFAIL(msg)            DEVFAIL(msg)
                                
#define RbfASSERT1(cond)        DEVASSERT1(cond)
#define RbfMESSAGE1(msg)        DEVMESSAGE1(msg)
#define RbfCHECK2(cond,msg)     DEVCHECK1(cond,msg)
#define RbfFAIL1(msg)           DEVFAIL1(msg)
                                
#define RbfASSERT2(cond)        DEVASSERT2(cond)
#define RbfMESSAGE2(msg)        DEVMESSAGE2(msg)
#define RbfCHECK2(cond,msg)     DEVCHECK1(cond,msg)
#define RbfFAIL2(msg)           DEVFAIL2(msg)
                                
#define RbfASSERT3(cond)        DEVASSERT3(cond)
#define RbfMESSAGE3(msg)        DEVMESSAGE3(msg)
#define RbfCHECK2(cond,msg)     DEVCHECK1(cond,msg)
#define RbfFAIL3(msg)           DEVFAIL3(msg)

#if DEBUGLEVEL>=4
    #define RbfASSERTPC(cond)       _DEVASSERT_ALWAYS_(cond)
    #define RbfMESSAGEPC(msg)       _DEVMESSAGE_ALWAYS_(msg)
    #define RbfCHECKPC(cond,msg)    _DEVCHECK_ALWAYS_(cond,msg)
    #define RbfFAILPC(msg)          _DEVFAIL_ALWAYS_(msg)

#else
    #define RbfASSERTPC(cond)
    #define RbfMESSAGEPC(msg)
    #define RbfCHECKPC(cond,msg)
    #define RbfFAILPC(msg)
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
VOID *RbfMalloc_( UI32 size );
VOID  RbfFree_( VOID **ppMemBlock );
#endif


/******************************************************************************/
/* RbfXXXX.c */
/******************************************************************************/



/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
