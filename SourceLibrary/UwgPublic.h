/******************************************************************************
**              Copyright (C) 2005 Ezurio Ltd
**
** Project:     Universal Wireless
**
** Module:      UWGPUBLIC.H
**
** Notes:
**
*******************************************************************************/

#if !defined(_UWGPUBLIC_H)     /* prevent multiple inclusions */
#define _UWGPUBLIC_H

/******************************************************************************/
/* Include Files*/
/******************************************************************************/
#include "project.h"

#include "StdPublic.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#if !defined(_TARGETUWG_H)
#error TargetUwg.h not included in Target.h
#endif
/******************************************************************************/
/* Defines*/
/******************************************************************************/

#define UWRESULTCODE_SUCCESS                0x0000
#define UWRESULTCODE_LOWEST_FAIL_VALUE      0x0100

/* SUCCESS resultcode start value - where MSB == 0 */

/* Add more above as TOK needs 32 for the moment so positioned at the */
/* upper end */
#define UWRESULTCODE_OKSTART_TOK_MODULE     0x00E0  /* reserving 32 for TOK */

/* ERROR resultcode start value - where MSB != 0 */
#define UWRESULTCODE_START_XXX_MODULE_MASK  0xFF00
#define UWRESULTCODE_START_UWG_MODULE       0x0200
#define UWRESULTCODE_START_TOK_MODULE       0x0400
#define UWRESULTCODE_START_RUN_MODULE       0x0600
#define UWRESULTCODE_START_RUN_MODULE_FATAL 0x0700
#define UWRESULTCODE_START_TAR_MODULE       0x0800
#define UWRESULTCODE_START_WLA_MODULE       0x0A00
#define UWRESULTCODE_START_OSA_MODULE       0x0C00
#define UWRESULTCODE_START_SKT_MODULE       0x0E00
#define UWRESULTCODE_START_CFG_MODULE       0x1000
#define UWRESULTCODE_START_SRM_MODULE       0x1100
#define UWRESULTCODE_START_STR_MODULE       0x1200
#define UWRESULTCODE_START_HYP_MODULE       0x1400
#define UWRESULTCODE_START_TKN_MODULE       0x1500
#define UWRESULTCODE_START_UWL_MODULE       0x1600
#define UWRESULTCODE_START_IMM_MODULE       0x1700
#define UWRESULTCODE_START_FSA_MODULE       0x1800
#define UWRESULTCODE_START_NVO_MODULE       0x1900
#define UWRESULTCODE_START_FDV_MODULE       0x1A00
#define UWRESULTCODE_START_INI_MODULE       0x1B00
#define UWRESULTCODE_START_TSK_MODULE       0x1C00
#define UWRESULTCODE_START_APP_MODULE       0x2000
#define UWRESULTCODE_START_PIO_MODULE       0x2200
#define UWRESULTCODE_START_MSC_MODULE       0x2400
#define UWRESULTCODE_START_TCP_MODULE       0x2500  /* up to 1024 error codes */
#define UWRESULTCODE_START_NVH_MODULE       0x2900
#define UWRESULTCODE_START_NVW_MODULE       0x2A00
#define UWRESULTCODE_START_FSQ_MODULE       0x2B00
#define UWRESULTCODE_START_RTC_MODULE       0x2C00
#define UWRESULTCODE_START_PPR_MODULE       0x2D00
#define UWRESULTCODE_START_CMM_MODULE       0x2E00
#define UWRESULTCODE_START_ENC_MODULE       0x2F00
#define UWRESULTCODE_START_ACC_MODULE       0x3000
#define UWRESULTCODE_START_GPS_MODULE       0x3100
#define UWRESULTCODE_START_LED_MODULE       0x3200
#define UWRESULTCODE_START_CFM_MODULE       0x3300
#define UWRESULTCODE_START_LGM_MODULE       0x3400
#define UWRESULTCODE_START_CRC_MODULE       0x3500
#define UWRESULTCODE_START_VSF_MODULE       0x3600
#define UWRESULTCODE_START_FRM_MODULE       0x3700
#define UWRESULTCODE_START_BLR_MODULE       0x3800
#define UWRESULTCODE_START_USB_MODULE       0x3900
#define UWRESULTCODE_START_BAT_MODULE       0x3a00
#define UWRESULTCODE_START_BLU_MODULE       0x3b00 
#define UWRESULTCODE_START_SYS_MODULE       0x3c00
#define UWRESULTCODE_START_OBD_MODULE       0x3d00    
#define UWRESULTCODE_START_BTH_MODULE       0x3e00
#define UWRESULTCODE_START_NET_MODULE       0x4000
#define UWRESULTCODE_START_COM_MODULE       0x4800
#define UWRESULTCODE_START_FTP_MODULE       0x4900

#define UWRESULTCODE_START_APP_RESPCODES    0xE000
/* 
** Add more UWRESULTCODE_START_mmm_MODULE above
**
** DO NOT USE 0xFF00 as a start value
** 
** ---------------------------------------------------------
** WARNING - DANGER  || WARNING - DANGER || WARNING - DANGER 
** ---------------------------------------------------------
**
** Do NOT assign a start value in the range FF00 to FFFF inclusive
** as that range is used as a special value
**
*/

#define UWG_UUID_SIZE_UI8                   (16)
#define UWG_UUID_SIZE_UI16                  (8)
#define UWG_UUID_SIZE_UI32                  (4)

/******************************************************************************/
/* Macros*/
/******************************************************************************/

#define UWSUCCESS(r)            ( (r)==UWRESULTCODE_SUCCESS)
#define UWCONTINUE(r)           ( (r)==UWRESULTCODE_CONTINUE)
#define UWFUZZYSUCCESS(r)       ( (r) <UWRESULTCODE_LOWEST_FAIL_VALUE)
#define UWFUZZYSUCCESSONLY(r)   (((r) >UWRESULTCODE_SUCCESS) && ((r)<UWRESULTCODE_LOWEST_FAIL_VALUE))
#define UWFAIL(r)               ( (r)>=UWRESULTCODE_LOWEST_FAIL_VALUE)

#if defined(UWG_SINGLE_HEAP_MANAGER)
#define UwgGET_HEAP_STATS(s)   UwgPubGetHeapStats( (s));
#else
#define UwgGET_HEAP_STATS(s)   UwgPubGetHeapStats( (s));
#endif

#define UWG_VERSION_NUMBER(mj,mn)   (((mj)<<8)+((mn)))

/******************************************************************************/
/* Enum Typedefs*/
/******************************************************************************/

/*=============================================================================*/
/* This enum collects all generic success result codes */
/*=============================================================================*/
enum
{
    UWRESULTCODE_OK                     = UWRESULTCODE_SUCCESS
   ,UWRESULTCODE_CONTINUE

   /*
   ,UWRESULTCODE_
   */
};

/*=============================================================================*/
/* This enum collects all generic error result codes */
/*=============================================================================*/
enum
{
    UWRESULTCODE_GENERIC_FAIL           = UWRESULTCODE_START_UWG_MODULE
   ,UWRESULTCODE_MALLOC_FAIL            /* 0201 */
   ,UWRESULTCODE_NULL_POINTER           /* 0202 */
   ,UWRESULTCODE_TIMEOUT                /* 0203 */
   ,UWRESULTCODE_CODE_TOBE_WRITTEN      /* 0204 */
   ,UWRESULTCODE_RESOURCE_FULL          /* 0205 */
#define UWRESULTCODE_FULL   UWRESULTCODE_RESOURCE_FULL  /* backward compat. */
   ,UWRESULTCODE_RESOURCE_EMPTY         /* 0206 */
#define UWRESULTCODE_EMPTY  UWRESULTCODE_RESOURCE_EMPTY /* backward compat. */
   ,UWRESULTCODE_RESOURCE_DELETED       /* 0207 */
#define UWRESULTCODE_DELETED    UWRESULTCODE_RESOURCE_DELETED /* bckw. compat.*/
   ,UWRESULTCODE_ACCESS_DENIED          /* 0208 */
   ,UWRESULTCODE_INCORRECT_MODE         /* 0209 */
   ,UWRESULTCODE_EOF                    /* 020A */
   ,UWRESULTCODE_OPEN_FAIL              /* 020B */

   /*
   ,UWRESULTCODE_
   */
};

/******************************************************************************/
/* Simple (non struct/union) Typedefs*/
/******************************************************************************/

typedef UI16    UWRESULTCODE;

/******************************************************************************/
/* Forward declaration of Class, Struct & Unions*/
/******************************************************************************/

/******************************************************************************/
/* Class definitions*/
/******************************************************************************/

/******************************************************************************/
/* Struct definitions*/
/******************************************************************************/

/*=============================================================================*/
typedef struct SUwgHeapStatsTag
{
    I32     mMallocCount;
    I32     mFreeCount;
    I32     mMaxCount;
}
SUwgHeapStats;

/*=============================================================================*/
typedef struct SUwgWordBlockTag
{
    UI16    mWords;
    UI16   *mpData;
}
SUwgWordBlock;

/*=============================================================================*/
typedef union UUwgUuidTag
{
    UI8                   mui8 [UWG_UUID_SIZE_UI8 ];
    UI16                  mui16[UWG_UUID_SIZE_UI16];
    UI32                  mui32[UWG_UUID_SIZE_UI32];
}
UUwgUuid;


/******************************************************************************/
/* Union definitions*/
/******************************************************************************/

/******************************************************************************/
/* Global Functions (API etc) exported for other modules*/
/******************************************************************************/

/*=============================================================================*/
/* Return the heap usage statistics                                            */
/*=============================================================================*/
#if defined(UWG_MONITOR_MALLOCS)
VOID
UwgPubGetHeapStats(
    SUwgHeapStats *psHeapStats
    );
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_SINGLE_HEAP_MANAGER)
VOID *UwgMalloc_( UI32 size );
VOID  UwgFree_( VOID **ppMemBlock );
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/*=============================================================================*/
VOID UwgPubPreInitialise(VOID);

/*=============================================================================*/
/*=============================================================================*/
VOID UwgPubPostInitialise(VOID);

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
VOID * 
UwgCheckedMalloc(
    UI32 size
    );
#endif

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
VOID * 
UwgCheckedMallocNoMaxCheck(
    UI32 size
    );
#endif

/*=============================================================================*/
/*=============================================================================*/
#if defined(UWG_HEAP_DATA_ALLOC_FREE_CHECK)
VOID 
UwgCheckedFree(
    VOID *pMemBlock
    );
#endif

/*=============================================================================*/
/* Returns a const string which describes the result code                      */
/*=============================================================================*/
CONST CHAR *
UwgResultCodeString(
    UWRESULTCODE resultCode
    );

/*=============================================================================*/
/*=============================================================================*/
INT UwlPubOnAssert(
    INT nExitCode
    );

/*=============================================================================*/
/*=============================================================================*/
UI16 UwgPubCountItems(
    SUwgWordBlock *pBlocks,
    UI16 nNumOfBlocks
    );

/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
