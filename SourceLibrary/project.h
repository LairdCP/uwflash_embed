/******************************************************************************
**              Copyright (C) Laird
**
** Module:      project.h
**
** Description:
**
** Notes:
**
*******************************************************************************/

#if !defined(PROJECT_H)     /* prevent multiple inclusions */
#define PROJECT_H

/******************************************************************************/
/* Include Files*/
/******************************************************************************/
#include "basetype.h"

/******************************************************************************/
/* Defines*/
/******************************************************************************/

/******************************************************************************/
/* Macros*/
/******************************************************************************/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*The folowing globally defined functions are required to service*/
/*the debug assert macros.*/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
VOID RelMessage(CHAR *pPrintfFormat,CHAR *pCondition,CHAR *pFilename,INT lineNum,INT nExitCode);
VOID RelFail(INT nExitCode);
#if DEBUGLEVEL>=1
#ifdef __cplusplus
    extern "C"
    {
#endif
#if defined(DLL_BUILD)
    typedef VOID (* FuncDevMessage)(char *pPrintfFormat,char *pCondition,char *pFilename,int lineNum,int nExitCode);
    typedef VOID (* FuncDevFail)(int nExitCode);
    typedef VOID (* FuncDevTrace)(const char* pcPrintfFormat, ...);
    extern FuncDevMessage DevMessage;
    extern FuncDevFail    DevFail;
    extern FuncDevTrace   DevTrace;
#else
    VOID DevMessage(CHAR *pPrintfFormat,CHAR *pCondition,CHAR *pFilename,INT lineNum,INT nExitCode);
    VOID DevFail(INT nExitCode);
    VOID DevTrace(CONST CHAR* pcPrintfFormat, ...);
#endif
#ifdef __cplusplus
    }
#endif

#endif

#define FMTDEVASS                   "\nDEVASSERT>%s\nFile: %s, Line: %d\n"
#define FMTDEVMSG                   "\nDEVMSG>%s\nFile: %s, Line: %d\n"
#define FMTDEVFAIL                  "\nDEVFAIL>\"%s\"\nFile: %s, Line: %d\n"
#define FMTRELASS                   "\nRELASSERT>%s\nFile: %s, Line: %d\n"
#define FMTRELMSG                   "\nRELMSG>%s\nFile: %s, Line: %d\n"
#define FMTRELFAIL                  "\nRELFAIL>\"%s\"\nFile: %s, Line: %d\n"

/*The following macros are defined for ALL builds*/
#define RELASSERT(cond)             {if(!(cond)){RelMessage( FMTRELASS,#cond,__FILE__,__LINE__,1);}}
#define RELMESSAGE(msg)             {RelMessage( FMTRELMSG,(msg),__FILE__,__LINE__,0);}
#define RELFAIL(msg)                {RelMessage( FMTRELFAIL,msg,__FILE__,__LINE__,0);RelFail(0);}

#define _DEVASSERT_ALWAYS_(cond)    {if(!(cond)){DevMessage( FMTDEVASS,#cond,__FILE__,__LINE__,1);}}
#define _DEVMESSAGE_ALWAYS_(msg)    {DevMessage( FMTDEVMSG,(msg),__FILE__,__LINE__,0);}
#define _DEVFAIL_ALWAYS_(msg)       {DevMessage( FMTDEVFAIL,msg,__FILE__,__LINE__,1);DevFail(1);}
#define _DEVCHECK_ALWAYS_(cond,msg) {if(!(cond)){DevMessage( FMTDEVASS,#cond,__FILE__,__LINE__,0);}}

#define PRINT     printf

#define DEVPRINTF(x)            PRINT((CONST CHAR *) x)

#if defined(ENABLE_DBGPRINTF)   /* which needs to be defined in Target.h */
#define DBGPRINTF(x)            PRINT((CONST CHAR *) x)
#else
#define DBGPRINTF(x)            /* */
#endif

#if (DEBUGLEVEL >= 1)
#define DEVASSERT(cond)     _DEVASSERT_ALWAYS_(cond)
#define DEVMESSAGE(msg)     _DEVMESSAGE_ALWAYS_(msg)
#define DEVCHECK(cond,msg)  _DEVCHECK_ALWAYS_(cond,msg)
#define DEVFAIL(msg)        _DEVFAIL_ALWAYS_(msg)
#define DEVTRACE(x)         DevTrace x
#define DEVASSERT1(cond)    _DEVASSERT_ALWAYS_(cond)
#define DEVMESSAGE1(msg)    _DEVMESSAGE_ALWAYS_(msg)
#define DEVFAIL1(msg)       _DEVFAIL_ALWAYS_(msg)
#define DEVTRACE1(x)        DevTrace x
#define DEVPRINTF1(x)        PRINT((CONST CHAR *) x)
#define DEVDECLARE1(t,v)     t v
#else
#define DEVASSERT(cond)
#define DEVMESSAGE(msg)
#define DEVCHECK(cond,msg)
#define DEVFAIL(msg)
#define DEVTRACE(x)
#define DEVASSERT1(cond)
#define DEVMESSAGE1(msg)
#define DEVCHECK1(cond,msg)
#define DEVFAIL1(msg)
#define DEVTRACE1(x)
#define DEVPRINTF1(x)
#define DEVDECLARE1(t,v)
#endif

#if (DEBUGLEVEL >= 2)
#define DEVASSERT2(cond)    _DEVASSERT_ALWAYS_(cond)
#define DEVMESSAGE2(msg)    _DEVMESSAGE_ALWAYS_(msg)
#define DEVCHECK2(cond,msg) _DEVCHECK_ALWAYS_(cond,msg)
#define DEVFAIL2(msg)       _DEVFAIL_ALWAYS_(msg)
#define DEVTRACE2(x)        DevTrace x
#define DEVPRINTF2(x)        PRINT((CONST CHAR *) x)
#define DEVDECLARE2(t,v)     t v
#else
#define DEVASSERT2(cond)
#define DEVMESSAGE2(msg)
#define DEVCHECK2(cond,msg)
#define DEVFAIL2(msg)
#define DEVTRACE2(x)
#define DEVPRINTF2(x)
#define DEVDECLARE2(t,v)
#endif

#if (DEBUGLEVEL >= 3)
#define DEVASSERT3(cond)    _DEVASSERT_ALWAYS_(cond)
#define DEVMESSAGE3(msg)    _DEVMESSAGE_ALWAYS_(msg)
#define DEVCHECK3(cond,msg) _DEVCHECK_ALWAYS_(cond,msg)
#define DEVFAIL3(msg)       _DEVFAIL_ALWAYS_(msg)
#define DEVTRACE3(x)        DevTrace x
#define DEVPRINTF3(x)        PRINT((CONST CHAR *) x)
#define DEVDECLARE3(t,v)     t v
#else
#define DEVASSERT3(cond)
#define DEVMESSAGE3(msg)
#define DEVCHECK3(cond,msg)
#define DEVFAIL3(msg)
#define DEVTRACE3(x)
#define DEVPRINTF3(x)
#define DEVDECLARE3(t,v)
#endif

#if (DEBUGLEVEL >= 4)
#define DEVASSERT4(cond)    _DEVASSERT_ALWAYS_(cond)
#define DEVMESSAGE4(msg)    _DEVMESSAGE_ALWAYS_(msg)
#define DEVCHECK4(cond,msg) _DEVCHECK_ALWAYS_(cond,msg)
#define DEVFAIL4(msg)       _DEVFAIL_ALWAYS_(msg)
#define DEVTRACE4(x)        DevTrace x
#define DEVPRINTF4(x)        PRINT((CONST CHAR *) x)
#define DEVDECLARE4(t,v)     t v
#else
#define DEVASSERT4(cond)
#define DEVMESSAGE4(msg)
#define DEVCHECK4(cond,msg)
#define DEVFAIL4(msg)
#define DEVTRACE4(x)
#define DEVPRINTF4(x)
#define DEVDECLARE4(t,v)
#endif

#if !defined(SUPPRESS_USAGE_WARNING)
#define SUPPRESS_USAGE_WARNING(x)       x=x=0
#endif

#if !defined(SUPPRESS_USAGE_WARNING1)
#define SUPPRESS_USAGE_WARNING1(x)       x=x
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



#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
