/******************************************************************************
**              Copyright (C) 2005 Ezurio Ltd
**
** Project:     Universal Wireless
**
** Module:      FRMPUBLIC.H
**
** Notes:       It is intended that this module shall be used both in the
**              UwWism related builds AND the UwLoad Windows Host application
**              which is used to perform firmware downloads.
**
*******************************************************************************/

#if !defined(_FRMPUBLIC_H)     /* prevent multiple inclusions */
#define _FRMPUBLIC_H

/******************************************************************************/
/* Include Files*/
/******************************************************************************/
#include "UwgPublic.h"

/******************************************************************************/
#if !defined(_TARGETFRM_H)
#error TargetFRM.h not included in Target.h
#endif
/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/* Defines*/
/******************************************************************************/

/* These are the commands that can be processed */
#define FRM_COMMAND_TARGET_PLATFORM             ('T')
#define FRM_COMMAND_SELECT_DEVICE               ('S')
#define FRM_COMMAND_WRITE_BLOCK                 ('W')
#define FRM_COMMAND_REGISTER_DEVICE             ('G')
#define FRM_COMMAND_UNREGISTER_DEVICE           ('U')
#define FRM_COMMAND_ERASE_BLOCK                 ('E')
#define FRM_COMMAND_SECTOR_MAP                  ('M')
#define FRM_COMMAND_QUERY                       ('Q')
#define FRM_COMMAND_EMBED                       ('B') /* used to reserve a packet in the stream */
                                                      /* Frm module should not cater for it */
#define FRM_COMMAND_OBJECTFILE                  ('O') /* used to reserve a packet in the stream */
                                                      /* Frm module should not cater for it */

/* These are sizes for commands */
#define FRM_COMMAND_TARGET_PLATFORM_LEN         (4)
#define FRM_COMMAND_SELECT_DEVICE_LEN           (2)
#define FRM_COMMAND_REGISTER_DEVICE_LEN         (11)
#define FRM_COMMAND_UNREGISTER_DEVICE_LEN       (1)
#define FRM_COMMAND_WRITE_BLOCK_SUBHDR_LEN      (8)
#define FRM_COMMAND_ERASE_BLOCK_LEN             (8)
#define FRM_COMMAND_QUERY_BLOCK_LEN             (12)

#define FRM_SECTORMAP_ITEM_LEN                  (8)

#define FRM_AUTO_SELECT_BANK                    (0xFF)
#define FRM_DEVICE_ID_INVALID                   (0xFF)
#define FRM_BANK_ID_INVALID                     (0xFF)


/******************************************************************************/
/* Macros*/
/******************************************************************************/

#if defined(UWG_SINGLE_HEAP_MANAGER)
#define FrmGET_HEAP_STATS(s)   UwgPubGetHeapStats( (s));
#else
#define FrmGET_HEAP_STATS(s)   FrmPubGetHeapStats( (s));
#endif

/******************************************************************************/
/* Simple (non struct/union) Typedefs*/
/******************************************************************************/

typedef UWRESULTCODE (*FPullDataIntoCache)(VOID *pContext, UI32 nOffsetIntoCache, UI32 *pNewReadLen);

/******************************************************************************/
/* Enum Typedefs*/
/******************************************************************************/

/*=============================================================================*/
/* This enum collects all module FRM specific result codes */
/*=============================================================================*/
enum
{
    UWRESULTCODE_FRM_PACKET_INCOMPLETE          = UWRESULTCODE_START_FRM_MODULE
   ,UWRESULTCODE_FRM_UNKNOWN_COMMAND            /* 3701 */
   ,UWRESULTCODE_FRM_WRITE_INCOMPLETE           /* 3702 */
   ,UWRESULTCODE_FRM_PLATFORM_MISMATCH          /* 3703 */
   ,UWRESULTCODE_FRM_PLATFORM_REDEFINE          /* 3704 */
   ,UWRESULTCODE_FRM_PLATFORM_UNDEFINED         /* 3705 */
   ,UWRESULTCODE_FRM_PACKET_SIZE_ERROR          /* 3706 */
   ,UWRESULTCODE_FRM_INVALID_PARAMETER          /* 3707 */
   ,UWRESULTCODE_FRM_SECTORMAP_SIZEERR          /* 3708 */
   ,UWRESULTCODE_FRM_SECTORMAP_EMPTY            /* 3709 */
   ,UWRESULTCODE_FRM_SECTORMAP_ITEMERR          /* 370A */
   ,UWRESULTCODE_FRM_SECTORMAP_INCOMPLETE       /* 370B */
   ,UWRESULTCODE_FRM_PACKET_TOO_LARGE           /* 370C */
   ,UWRESULTCODE_FRM_NO_CONSUME                 /* 370D */

   /*
   ,UWRESULTCODE_FRM_
   */
};

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

/*=============================================================================*/
/* Return the heap usage statistics                                            */
/*=============================================================================*/
#if !defined(UWG_SINGLE_HEAP_MANAGER)
VOID
FrmPubGetHeapStats(
    SUwgHeapStats *psHeapStats
    );
#endif /* #if !defined(UWG_SINGLE_HEAP_MANAGER) */

/*=============================================================================*/
/*=============================================================================*/
VOID 
FrmPubPreInitialise(
    VOID
    );

/*=============================================================================*/
/*=============================================================================*/
VOID 
FrmPubPostInitialise(
    VOID
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPubProcessFile(
    FPullDataIntoCache FPullDataFunc,
    VOID *pFuncContext,
    UI8  *pCache,
    UI32  nCacheLen 
    );

/*=============================================================================*/
/*=============================================================================*/
UI32
FrmPubGetMinCacheSize(
    VOID
    );

/*=============================================================================*/
/*=============================================================================*/
VOID *
FrmPubOpenManager(
    UWRESULTCODE *pResultCode
    );

/*=============================================================================*/
/*=============================================================================*/
VOID
FrmPubCloseManager(
    VOID **ppFrmContext
    );

/*=============================================================================*/
/*=============================================================================*/
BOOLEAN
FrmPubCanClose(
    VOID *pFrmContext
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPubProcessData(
    VOID *pContext,
    UI8 *pPacketData,
    UI32 nPacketLen,
    UI32 nSourceRemain,
    UI32 *pConsumed
    );

/******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/
