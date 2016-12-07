/******************************************************************************
**              Copyright (C) 2002 TDK Systems Europe Ltd
**
** Project:     TDK Intelligent Serial Dongle - Upgrade Class
**
** Module:      ATUPGRADEUI.H
**
*******************************************************************************/

#if !defined(_ATUPGRADEUI_H)     /* prevent multiple inclusions */
#define _ATUPGRADEUI_H

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "Target.h"
#include "project.h"
#include "UwgPublic.h"

/******************************************************************************/
// Defines
/******************************************************************************/

/******************************************************************************/
// Macros
/******************************************************************************/

#define ENUM_ATUPG_MSGID(_x_)           ATUPG_MSGID_ ## _x_

#define UpgradeUI_Clear()               gpAtUpgradeUI->MemoClear();

#define UpgradeUI_Message(_x_)          gpAtUpgradeUI->Message( ATUPG_MSGID_ ## _x_ )
#define UpgradeUI_Message_N(_x_,_n_)    gpAtUpgradeUI->Message( ATUPG_MSGID_ ## _x_ , _n_)
#define UpgradeUI_Message_S(_x_,_s_)    gpAtUpgradeUI->Message( ATUPG_MSGID_ ## _x_ , _s_)
#define UpgradeUI_Message_NNNN(_x_,_n1_,_n2_,_n3_,_n4_)    gpAtUpgradeUI->Message( ATUPG_MSGID_ ## _x_ ,_n1_,_n2_,_n3_,_n4_)
#define UpgradeUI_Message_NNNNN(_x_,_n1_,_n2_,_n3_,_n4_,_n5_)    gpAtUpgradeUI->Message( ATUPG_MSGID_ ## _x_ ,_n1_,_n2_,_n3_,_n4_,_n5_)

#define UpgradeUI_SetButton(_x_, _t_)   gpAtUpgradeUI->SetButton( ATUPG_MSGID_ ## _x_, _t_ )
#define UpgradeUI_SetButtonVis(_t_)     gpAtUpgradeUI->SetButton( _t_ )

#define UpgradeUI_GetText(_x_)          gpAtUpgradeUI->GetText( ATUPG_MSGID_ ## _x_ )


/******************************************************************************/
// Enum Typedefs
/******************************************************************************/

typedef enum
{
    ECOLOR_RED                                      = 0
   ,ECOLOR_BLUE
   ,ECOLOR_BLACK
   ,ECOLOR_MAROON
}
EColor;

typedef enum
{
    ENUM_ATUPG_MSGID(CONVERTING_TO_FLASH_MODE)      = 10
   ,ENUM_ATUPG_MSGID(FAIL_TO_ENTER_PROGMODE)        = 11
   ,ENUM_ATUPG_MSGID(FAIL_ENTER_PROGMODE)           = 12
   ,ENUM_ATUPG_MSGID(FAIL_BOOTMODE)                 = 13
   ,ENUM_ATUPG_MSGID(SYNCHONISED)                   = 20
   ,ENUM_ATUPG_MSGID(FAIL_SYNCHONISE_RETRY)         = 21
   ,ENUM_ATUPG_MSGID(ERASING)                       = 30
   ,ENUM_ATUPG_MSGID(ERASEBLOCK)                    = 31
   ,ENUM_ATUPG_MSGID(WRITING)                       = 40
   ,ENUM_ATUPG_MSGID(UPGRADE_PASS)                  = 50
   ,ENUM_ATUPG_MSGID(UPGRADE_FAIL)                  = 51
   ,ENUM_ATUPG_MSGID(GENERIC_MSG)                   = 99


   ,ENUM_ATUPG_MSGID(QUERY)                         = 60
   ,ENUM_ATUPG_MSGID(QUERYRESP)                     = 61
   ,ENUM_ATUPG_MSGID(ATMELERROR)                    = 62
   ,ENUM_ATUPG_MSGID(UNEXPECTED_ACK)                = 63
   ,ENUM_ATUPG_MSGID(UNEXPECTED_NACK)               = 64
   ,ENUM_ATUPG_MSGID(UNEXPECTED_ATS)                = 65
   ,ENUM_ATUPG_MSGID(UNEXPECTED_DATA)               = 66
   ,ENUM_ATUPG_MSGID(WAIT_FOR_CTS)                  = 67
   ,ENUM_ATUPG_MSGID(NO_CTS_FROM_MODULE)            = 68
   ,ENUM_ATUPG_MSGID(WAIT_FOR_DSR)                  = 69
   ,ENUM_ATUPG_MSGID(NO_DSR_FROM_MODULE)            = 70
   ,ENUM_ATUPG_MSGID(MODULE_DTR_ASSERTED)           = 71
   ,ENUM_ATUPG_MSGID(MODULE_RTS_ASSERTED)           = 72
   ,ENUM_ATUPG_MSGID(RX_DATA)                       = 73
   ,ENUM_ATUPG_MSGID(TX_DATA)                       = 74
   ,ENUM_ATUPG_MSGID(UNREGISTER_DEVICE)             = 75
   ,ENUM_ATUPG_MSGID(REGISTER_DEVICE)               = 76
   ,ENUM_ATUPG_MSGID(SETBANK)                       = 77
   ,ENUM_ATUPG_MSGID(WRITE_PACKET)                  = 78
   ,ENUM_ATUPG_MSGID(SEND_DATA)                     = 79
   ,ENUM_ATUPG_MSGID(ERASE_PACKET)                  = 80
   ,ENUM_ATUPG_MSGID(SECTORMAP)                     = 81
   ,ENUM_ATUPG_MSGID(WRITEBLOCKHEADER)              = 82
   ,ENUM_ATUPG_MSGID(AUTO_SELECT_BANK_FROM)         = 83
   ,ENUM_ATUPG_MSGID(AUTO_SELECT_BANK)              = 84

/*
    UpgradeUI_Message(_x_)
    UpgradeUI_Message_N(_x_,_n_)
    UpgradeUI_Message_S(_x_,_s_)

   ,ENUM_ATUPG_MSGID()
*/
   ,ENUM_ATUPG_MSGID( LAST_ID                             )
}
ATUPG_MSGID;

typedef enum
{
    ATUPG_LANGID_ENGLISH                            = 0

/*
   ,ATUPG_LANGID_
*/
   ,ATUPG_LANGID_LAST
}
ATUPG_LANGID;


/******************************************************************************/
// Simple (non struct/union) Typedefs
/******************************************************************************/

/******************************************************************************/
// Forward declaration of Class, Struct & Unions
/******************************************************************************/

/******************************************************************************/
// Struct definitions
/******************************************************************************/

typedef struct
{
    EColor color;
    char *pLang[ATUPG_LANGID_LAST];
}
ATUPG_MSG_STR;

typedef struct
{
    int  nRegNo;
    char *pLang[ATUPG_LANGID_LAST];
}
ATUPG_SREG_STR;

/******************************************************************************/
// Class definitions
/******************************************************************************/


class AtUpgradeUI
{
//Data
private:

//Data
protected:
    int     mLangId;
    char    baMsgBuf[512];

//Data
public:

//Functions
private:

//Functions
protected:

//Functions
public:
    AtUpgradeUI();
    virtual ~AtUpgradeUI();

    virtual void InitialiseUI(void)=0;
    virtual void SetButton(int nMsgId,bool fEnabled)=0;
    virtual void SetButton(bool fVisible)=0;
    virtual void Message(int nMsgId);
    virtual void Message(int nMsgId, int nParam);
    virtual void Message(int nMsgId, int nParam1,int nParam2,int nParam3,int nParam4,int nParam5 );
    virtual void Message(int nMsgId, char *pStr);
    virtual void MemoClear(void)=0;
    virtual void WriteToMemo(char *pMsg)=0;

    virtual char *GetText(int nMsgId);

    virtual UI32 TimeElapsed(UI32 nTickStart);
    virtual UWRESULTCODE Sleep(UI32 nSleepMs, bool fDisplayProgress);

    virtual void InitAndShowProgressBar(UI32 minVal, UI32 maxVal)=0;
    virtual void UpdateProgressBar(UI32 curVal)=0;
    virtual void HideProgressBar(void)=0;

    virtual void InitAndShowMainProgressBar(UI32 minVal, UI32 maxVal)=0;
    virtual void UpdateMainProgressBar(UI32 curVal)=0;
    virtual void HideMainProgressBar(void)=0;

    virtual void OnAppClose()=0;
};

/******************************************************************************/
// Union definitions
/******************************************************************************/

/******************************************************************************/
// Global Functions (API etc) exported for other modules
/******************************************************************************/
extern AtUpgradeUI *gpAtUpgradeUI;

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
// END OF FILE
/******************************************************************************/
