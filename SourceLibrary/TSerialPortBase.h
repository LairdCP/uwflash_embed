/******************************************************************************
**              Copyright (C) 2007 TDK Systems Europe Ltd
**
** Project:
**
** Module:      TSerialPortBase.h
**
*******************************************************************************/

#if !defined(_TSERIALPORTBASE_H)     /* prevent multiple inclusions */
#define _TSERIALPORTBASE_H

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "Target.h"
#include "project.h"

#include "TBootLdrBase.h"

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

class TSerialOnRxData
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
    TSerialOnRxData(){};
    virtual ~TSerialOnRxData(){};
    virtual void OnRxData(unsigned nCount)=0;
};


class TSerialPortBase
{
//Data
private:

//Data
protected:

//Data
public:
    TSerialOnRxData *mpSerialOnRxData;

//Functions
private:

//Functions
protected:

//Functions
public:
    TSerialPortBase(TSerialOnRxData *pOwner);
    virtual ~TSerialPortBase();

    virtual BOOLEAN IsOpen()=0;
    virtual VOID Close()=0;
    virtual bool ComOpen(char *pDevName,int nBaudrate,int nParity, int nStopBits, int nDataBits)=0;
    virtual void SetBreak(void)=0;
    virtual void ClearBreak(void)=0;
    virtual void FlushRxBuffer(void)=0;
    virtual void FlushTxBuffer(void)=0;
    virtual void FlushRxTx(void){FlushRxBuffer();FlushTxBuffer();};
    virtual bool GetCtsState(void)=0;
    virtual bool GetDsrState(void);
    virtual bool GetDcdState(void);
    virtual bool GetRiState(void);
    virtual void AssertDTR(void)=0;
    virtual void DeassertDTR(void)=0;
    virtual void AssertRTS(void);
    virtual void DeassertRTS(void);

    /* Functions to read and write from underlying buffers */
    virtual void PutString(char *pTxStr);
    virtual unsigned PutBlock(unsigned char *pTxBlock, unsigned short nLen)=0;
    virtual unsigned GetBlock(char *pDest, unsigned nCount)=0;

    /*
    Derived class calls this when there is data with the amount of data
    that arrived. It is assumed that on exit ALL data will have been read.

    The member function GetBlock() is used to read the actual data that
    arrived
    */
    virtual void OnSerialReceive(unsigned int nCount);
};


/******************************************************************************/
// Union definitions
/******************************************************************************/

/******************************************************************************/
// Global Functions (API etc) exported for other modules
/******************************************************************************/
TSerialPortBase *GetSerialPort(TSerialOnRxData *pOnRxData);

/*
  In the following routine, do a ovelapped read of the serial port and
  store the data, if received, in a global buffer.
  Then call the OnRxData() member function of the TSerialPortBase class.
  This will pump the rest of the application to process it
*/
void PollSerialRxData(void);

#endif /* Prevention of multiple inclusion */
/******************************************************************************/
// END OF FILE
/******************************************************************************/
