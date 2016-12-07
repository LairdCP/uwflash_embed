/******************************************************************************
**              Copyright (C) 2016 Laird
**
** Project:     UwFlash Upgrade Utility - Target based helper functions
**
** Module:      TSerialPortTarget.H
**
*******************************************************************************/

#if !defined(_TSERIALPORTTARGET_H)     /* prevent multiple inclusions */
#define _TSERIALPORTTARGET_H

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "Target.h"
#include "project.h"

#include "TSerialPortBase.h"

/******************************************************************************/
// Defines
/******************************************************************************/

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

class TSerialPortTarget : public TSerialPortBase
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
    TSerialPortTarget(TSerialOnRxData *pOwner);
    virtual ~TSerialPortTarget();

    virtual BOOLEAN IsOpen();
    virtual void Close();
    virtual bool ComOpen(char *pDevName,int nBaudrate,int nParity, int nStopBits, int nDataBits);
    virtual unsigned GetBlock(char *pDest, unsigned nCount);
    virtual void SetBreak(void);
    virtual void ClearBreak(void);
    virtual void FlushRxBuffer(void);
    virtual void FlushTxBuffer(void);
    virtual unsigned PutBlock(unsigned char *pTxBlock, unsigned short nLen);
    virtual void AssertDTR(void);
    virtual void DeassertDTR(void);
    virtual bool GetCtsState(void);

};


/******************************************************************************/
// Union definitions
/******************************************************************************/

/******************************************************************************/
// Global Functions (API etc) exported for other modules
/******************************************************************************/

/*============================================================================*/
/* This function returns the number of receive bytes waiting in an            */
/* underlying buffer waiting to be read                                       */
/* If need be take care of any threading issues                               */
/*============================================================================*/
unsigned int SerialRxDataCount(void);

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
// END OF FILE
/******************************************************************************/
