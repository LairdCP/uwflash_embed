/******************************************************************************
**              Copyright (C) 2015 Laird Technologies
**
** Project:     Console mode UwFlash Upgrade Utility
**
** Module:      ATUPGRADEUITARGET.H
**
*******************************************************************************/

#if !defined(_ATUPGRADEUITARGET_H)     /* prevent multiple inclusions */
#define _ATUPGRADEUITARGET_H

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "Target.h"
#include "project.h"
#include "UwgPublic.h"

#include "AtUpgradeUI.h"

/******************************************************************************/
// Defines
/******************************************************************************/

#define UPGRADE_APP_VERSION         "(v1.00)"

/*

  v1.00
    (*) First release
*/

/******************************************************************************/
// Macros
/******************************************************************************/

/******************************************************************************/
// Enum Typedefs
/******************************************************************************/


/******************************************************************************/
// Simple (non struct/union) Typedefs
/******************************************************************************/

/******************************************************************************/
// Forward declaration of Class, Struct & Unions
/******************************************************************************/

/******************************************************************************/
// Struct definitions
/******************************************************************************/

/******************************************************************************/
// Class definitions
/******************************************************************************/


class AtUpgradeUITarget : public AtUpgradeUI
{
//Data
private:

//Data
protected:

//Data
public:

//Functions
private:

//Functions
protected:

//Functions
public:
    AtUpgradeUITarget();
    virtual ~AtUpgradeUITarget();

    virtual void InitialiseUI(void){};
    virtual void SetButton(int nMsgId,bool fEnabled){};
    virtual void SetButton(bool fVisible){};
    virtual void MemoClear(void){};
    virtual void WriteToMemo(char *pMsg);

    virtual void InitAndShowProgressBar(UI32 minVal, UI32 maxVal){};
    virtual void UpdateProgressBar(UI32 curVal){};
    virtual void HideProgressBar(void){};

    virtual void InitAndShowMainProgressBar(UI32 minVal, UI32 maxVal);
    virtual void UpdateMainProgressBar(UI32 curVal);
    virtual void HideMainProgressBar(void){};

    virtual void OnAppClose(){};

};

/******************************************************************************/
// Union definitions
/******************************************************************************/

/******************************************************************************/
// Global Functions (API etc) exported for other modules
/******************************************************************************/

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
// END OF FILE
/******************************************************************************/
