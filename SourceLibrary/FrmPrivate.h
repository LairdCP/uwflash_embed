/******************************************************************************
**              Copyright (C) 2005 Ezurio Ltd
**
** Project:     Universal Wireless
**
** Module:      FRMPRIVATE.H
**
** Notes:
**
*******************************************************************************/

#if !defined(_FRMPRIVATE_H)     /* prevent multiple inclusions */
#define _FRMPRIVATE_H

/******************************************************************************/
/* Include Files*/
/******************************************************************************/

#include "FrmPrivateTarget.h"

/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/* Defines*/
/******************************************************************************/

#define FRM_PACKET_HEADER_LENGTH                (6)
#define FRM_PACKET_HEADER_OFFSET_CMDID          (0)
#define FRM_PACKET_HEADER_OFFSET_LENGTH         (2)

/******************************************************************************/
/* Macros*/
/******************************************************************************/

#if defined(UWG_SINGLE_HEAP_MANAGER)
#define FrmMALLOC(_x_)          UwgMalloc_((_x_))
#define FrmFREE(_x_)            UwgFree_((VOID **)(_x_))
#else
#define FrmMALLOC(_x_)          FrmMalloc_((_x_))
#define FrmFREE(_x_)            FrmFree_((VOID **)(_x_))
#endif

#define FrmASSERT(cond)         DEVASSERT(cond)
#define FrmMESSAGE(msg)         DEVMESSAGE(msg)
#define FrmFAIL(msg)            DEVFAIL(msg)
#define FrmTRACE(x)             DEVTRACE1(x)

#define FrmASSERT1(cond)        DEVASSERT1(cond)
#define FrmMESSAGE1(msg)        DEVMESSAGE1(msg)
#define FrmFAIL1(msg)           DEVFAIL1(msg)
#define FrmTRACE1(x)            DEVTRACE1(x)

#define FrmASSERT2(cond)        DEVASSERT2(cond)
#define FrmMESSAGE2(msg)        DEVMESSAGE2(msg)
#define FrmFAIL2(msg)           DEVFAIL2(msg)
#define FrmTRACE2(x)            DEVTRACE2(x)

#define FrmASSERT3(cond)        DEVASSERT3(cond)
#define FrmMESSAGE3(msg)        DEVMESSAGE3(msg)
#define FrmFAIL3(msg)           DEVFAIL3(msg)
#define FrmTRACE3(x)            DEVTRACE3(x)

#if DEBUGLEVEL>=4
    #define FrmASSERTPC(cond)   _DEVASSERT_ALWAYS_(cond)
    #define FrmMESSAGEPC(msg)   _DEVMESSAGE_ALWAYS_(msg)
    #define FrmFAILPC(msg)      _DEVFAIL_ALWAYS_(msg)
    #define FrmTRACEPC(x)       DEVTRACE3(x)

#else
    #define FrmASSERTPC(cond)   /* */
    #define FrmMESSAGEPC(msg)   /* */
    #define FrmFAILPC(msg)      /* */
    #define FrmTRACEPC(x)       /* */
#endif


/******************************************************************************/
/* Simple (non struct/union) Typedefs*/
/******************************************************************************/

/******************************************************************************/
/* Enum Typedefs*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
typedef enum EFrmStateTag
{
    FRMSTATE_WAIT_1ST_PACKET_HEADER     =   0
   ,FRMSTATE_PROCESS_1ST_PACKET
   ,FRMSTATE_WAIT_PACKET_HEADER
   ,FRMSTATE_PROCESS_PACKET

/*
   ,FRMSTATE_
*/
}
EFrmState;

/*----------------------------------------------------------------------------*/
typedef enum EFrmSubStateTag
{
    FRM_SUBSTATE_FIRSTPASS                   =   0

    /* Sub States associated with Write Data to flash command */
   ,FRM_SUBSTATE_WRITEDATA

/*
   ,FRM_SUBSTATE_
*/
}
EFrmSubState;


/******************************************************************************/
/* Forward declaration of Class, Struct & Unions*/
/******************************************************************************/

/******************************************************************************/
/* Class definitions*/
/******************************************************************************/

/******************************************************************************/
/* Struct definitions*/
/******************************************************************************/

/*----------------------------------------------------------------------------*/
typedef struct SFrmManagerTag
{
    EFrmState           mState;
    EFrmSubState        mSubState;
    UI8                 mCurrentDevice;
    UI8                 mCurrentBank;
    UI8                 mCommandId;
    UI32                mPacketLength;

    /* Fields related to managing writes to flash */
    UI32                mWriteOffset;
    UI32                mWriteBlockLen;
    UI32                mWriteFlags;

    /* This is managed by target specific code */
    VOID               *mpBootLoader;
}
SFrmManager;


/******************************************************************************/
/* Union definitions*/
/******************************************************************************/

/******************************************************************************/
/* Global Functions (API etc) exported for other modules*/
/******************************************************************************/

#if !defined(UWG_SINGLE_HEAP_MANAGER)
VOID *FrmMalloc_( UI32 size );
VOID  FrmFree_( VOID **ppMemBlock );
#endif


/******************************************************************************/
/* FrmPrivate.c */
/******************************************************************************/

/*=============================================================================*/
/*=============================================================================*/
EFrmState
FrmPrvChangeMainState(
    SFrmManager *pContext,
    EFrmState newState
    );

/*=============================================================================*/
/*=============================================================================*/
UI16
FrmPrvRead16(
    UI8 *pStream
    );

/*=============================================================================*/
/*=============================================================================*/
UI32
FrmPrvRead32(
    UI8 *pStream
    );

/*=============================================================================*/
/*=============================================================================*/
BOOLEAN
FrmPubOpenManagerTarget(
    SFrmManager *pContext
    );

/*=============================================================================*/
/*=============================================================================*/
VOID
FrmPubCloseManagerTarget(
    SFrmManager *pContext
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceTargetPlatform(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceSelectDevice(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceWriteBlock(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceEraseBlock(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceSectorMap(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceRegisterDevice(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceUnregisterDevice(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    );

/*=============================================================================*/
/*=============================================================================*/
SFrmManager *
FrmPrvOpenManager(
    UWRESULTCODE *pResultCode
    );

/*=============================================================================*/
/*=============================================================================*/
VOID
FrmPrvCloseManager(
    SFrmManager **ppContext
    );

/*=============================================================================*/
/*=============================================================================*/
BOOLEAN
FrmPrvCanClose(
    SFrmManager *pContext
    );

/*=============================================================================*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvProcessData(
    SFrmManager *pContext,
    UI8 *pPacketData,
    UI32 nPacketLen,
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
