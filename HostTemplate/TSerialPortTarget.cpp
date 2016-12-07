/******************************************************************************
**              Copyright (C) 2016 Laird
**
** Project:     UwFlash Upgrade Utility - Target based helper functions
**
** Module:      TSerialPortTarget.CPP
**
*******************************************************************************/

/******************************************************************************/
// CONDITIONAL COMPILE DEFINES
/******************************************************************************/

/******************************************************************************/
// Include Files
/******************************************************************************/
#include "target.h"
#include "TSerialPortTarget.h"
#include "AtConfig.h"

/******************************************************************************/
// Local Defines
/******************************************************************************/

/******************************************************************************/
// Local Macros
/******************************************************************************/

/******************************************************************************/
// Local Forward Class,Struct & Union Declarations
/******************************************************************************/

/******************************************************************************/
// Local Class,Struct,Union Typedefs
/******************************************************************************/

/******************************************************************************/
// External Variable Declarations
/******************************************************************************/

/******************************************************************************/
// Global/Static Variable Declarations
/******************************************************************************/

/******************************************************************************/
// External Function Declarations
/******************************************************************************/

/******************************************************************************/
// Local Forward Function Declarations
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
// Local Functions or Private Members
/******************************************************************************/
/******************************************************************************/

//=============================================================================
//=============================================================================

/******************************************************************************/
/******************************************************************************/
// Global Functions or Public Members
/******************************************************************************/
/******************************************************************************/

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
TSerialPortTarget::TSerialPortTarget(TSerialOnRxData *pOwner)
  : TSerialPortBase( pOwner )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
 TSerialPortTarget::~TSerialPortTarget()
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}

//------------------------------------------------------------------------------
// This function is called to open the uart interface which provides serial
// access to the Laird module.
//------------------------------------------------------------------------------
bool /* true if the serial port was successfully opened */
TSerialPortTarget::ComOpen(
    char *pDevName, /* specified via #define SERIALPORT_DEVICE_NAME in target.h */
    int nBaudrate,
    int nParity,    /* 0=None, 1=Odd, 2=Even */
    int nStopBits,  /* always 1 */
    int nDataBits   /* always 8 */
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE

    bool fRetVal = false; //assume cannot open the port

    return fRetVal;
}

//=============================================================================
// This function returns true if the serial port is open via a call of ComOpen()
//=============================================================================
bool /* true if the serial port is already open */
TSerialPortTarget::IsOpen(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE

    bool fIsOpen = false;

    return fIsOpen;
}

//=============================================================================
// Laird modules are designed to go into deep sleep when a uart_break is
// detected. When that uart_break is removed, the module will resume operation
// by resetting.
// This means a break/unbreak is a convenient method of resetting the module.
// If your system's uart is not capable of asserting a break on it's uart_tx line
// then if the module's reset line is driven by your host, then it is acceptable
// to assert a RESET when this function is called.
//=============================================================================
void
TSerialPortTarget::SetBreak(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}

//=============================================================================
// Laird modules are designed to go into deep sleep when a uart_break is
// detected. When that uart_break is removed, the module will resume operation
// by resetting.
// This means a break/unbreak is a convenient method of resetting the module.
// If your system's uart is not capable of asserting a break on it's uart_tx line
// then if the module's reset line is driven by your host, then it is acceptable
// to deassert the RESET when this function is called.
//=============================================================================
void
TSerialPortTarget::ClearBreak(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}

//=============================================================================
// Laird modules with smartBASIC have an nAutorun pin which is deasserted (taken
// high) then an autorunning smartBASIC application is supressed on power up.
// Hence as part of the firmware upgrade the DTR line which on Laird Devkits is
// connected to the nAutorun will be deasserted at the start of the process.
//
// If in your system yuur host has a gpio line which is driving the nAutorun
// pin of the module, then calling this function should set the nAutorun pin
// to high
//=============================================================================
void
TSerialPortTarget::DeassertDTR(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}

//=============================================================================
// Laird modules with smartBASIC have an nAutorun pin which is deasserted (taken
// high) then an autorunning smartBASIC application is supressed on power up.
// Hence as part of the firmware upgrade the DTR line which on Laird Devkits is
// connected to the nAutorun will be deasserted at the start of the process.
//
// If in your system yuur host has a gpio line which is driving the nAutorun
// pin of the module, then calling this function should set the nAutorun pin
// to low
//=============================================================================
void
TSerialPortTarget::AssertDTR(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}

//=============================================================================
// This function returns the current state of the RTS output of the Laird module
// which will be connected to the CTS input of the host mcu
//=============================================================================
bool  /* true if asserted */ 
TSerialPortTarget::GetCtsState(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}

//=============================================================================
// This function will be called by the engine to flush the low level receive
// buffers
//=============================================================================
void
TSerialPortTarget::FlushRxBuffer(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}

//=============================================================================
// This function will be called by the engine to flush the low level transmit
// buffers
//=============================================================================
void
TSerialPortTarget::FlushTxBuffer(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}

//=============================================================================
// This function will be called to output the buffer specified to the Laird module
// and the parameter nLen specifies how many bytes in that buffer to send.
//=============================================================================
unsigned /* number of bytes actually sent */
TSerialPortTarget::PutBlock(
    unsigned char *pTxBlock,  /* pointer to data that needs to be sent */
    unsigned short nLen       /* number of bytes to send */
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
    return 0;
}

//=============================================================================
// This function is used to call a maximum of nCount bytes from the low level
// buffer and the function will return the actual number returned.
//=============================================================================
unsigned /* number of bytes actually copied to destination buffer */
TSerialPortTarget::GetBlock(
    char *pDest,  /* pointer to buffer where data will be copied */
    unsigned nCount /* maximum bytes to copy to buffer */
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
    return 0;
}

//=============================================================================
// This function will be called to close the serial interface and no further
// serial activity will be allowed apart from ComOpen() and IsOpen()
//=============================================================================
void
TSerialPortTarget::Close(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
}    

/******************************************************************************/
/******************************************************************************/
// Global Functions Non Class Members
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/* This function returns the number of receive bytes waiting in an            */
/* underlying buffer waiting to be read                                       */
/* If need be take care of any threading issues                               */
/*============================================================================*/
unsigned int  /* returns the number of bytes waiting in the low level rx buffer */
SerialRxDataCount(
    void
    )
{
    /* TODO */
    CODE_TO_BE_WRITTEN_HERE
    return 0;
}

/******************************************************************************/
// END OF FILE
/******************************************************************************/





