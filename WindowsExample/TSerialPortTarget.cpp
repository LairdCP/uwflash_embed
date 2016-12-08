/******************************************************************************
**              Copyright (C) 2007 TDK Systems Europe Ltd
**
** Project:
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

#define SERIAL_BUFFER_LEN                       (1024*8)

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

extern int gArgc;
extern char **gArgv;

/******************************************************************************/
// Global/Static Variable Declarations
/******************************************************************************/

char gbaRxBuffer[ SERIAL_BUFFER_LEN ];
unsigned  gRxCount=0;
extern TSerialPortTarget *gpSerialPort;


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
    mApdComPort=NULL;
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
 TSerialPortTarget::~TSerialPortTarget()
{
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
TParity TSerialPortTarget::GetParityEnum(int nParity)
{
    TParity retVal = pNone;

    switch(nParity)
    {
    case 0: retVal = pNone; break;
    case 1: retVal = pOdd; break;
    case 2: retVal = pEven; break;
    default:retVal = pNone; break;
    }

    return retVal;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TSerialPortTarget::OnRxDataAvail(TObject *CP, WORD Count)
{
    /* just read the data into static buffer like linux driver will do */
    /* only store amount that can be stored */
    int nSpace = SERIAL_BUFFER_LEN - gRxCount;
    int nReadLen;
    if(mApdComPort)
    {
        nReadLen = ( Count > nSpace ) ? nSpace : Count;
        mApdComPort->GetBlock(&gbaRxBuffer[gRxCount], nReadLen);
        gRxCount += nReadLen;
    }
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
    bool fRetVal = false; //assume cannot open the port
    int nComport=0;

    //if second paramter existed in command line that it has to be the comport name
    if( gArgc >= 2 )
    {
        nComport = atoi(gArgv[1]);
    }
    if((nComport <=0)||(nComport >255))
    {
        nComport = atoi(pDevName);
    }

    try
    {
        //Create and initialise the apd comport object
        if(mApdComPort==NULL)mApdComPort = new TApdComPort(NULL);
        if(mApdComPort)
        {
            {
                try
                {
                    mApdComPort->Open                    = false;
                    if( gAtConfig.mfVirtualPort)
                    {
                        Sleep(PORT_OPEN_CLOSE_DELAY_MS);
                    }
                    mApdComPort->AutoOpen                = false;
                    mApdComPort->ComNumber               = nComport;
                    mApdComPort->Baud                    = nBaudrate;
                    mApdComPort->Parity                  = pNone; /* GetParityEnum(nParity); */
                    mApdComPort->DataBits                = nDataBits;
                    mApdComPort->StopBits                = nStopBits;
                    mApdComPort->OnTriggerAvail          = OnRxDataAvail;

                    //Enable rts/cts hardware handshaking
                    mApdComPort->HWFlowOptions.Clear();
                    /*
                    mApdComPort->HWFlowOptions << hwfUseRTS << hwfRequireCTS;
                    mApdComPort->BufferFull = mApdComPort->InSize * 0.9;
                    mApdComPort->BufferResume = mApdComPort->InSize * 0.1;
                    */

                    //finally open it
                    mApdComPort->Open                    = true;
                    if( gAtConfig.mfVirtualPort)
                    {
                        Sleep(PORT_OPEN_CLOSE_DELAY_MS);
                    }
                    mApdComPort->DTR = true;

                    fRetVal = true;

                }
                catch (EBadId*)
                {
                    delete mApdComPort;
                    mApdComPort = NULL;
                    fRetVal = false;
                }
                catch(...)
                {
                    delete mApdComPort;
                    mApdComPort = NULL;
                    fRetVal = false;
                }
            }
#if defined(CHECKPORT)
            else
            {
                delete mApdComPort;
                mApdComPort = NULL;
                fRetVal = false;
            }
#endif
        }
    }
    catch(...)
    {
        mApdComPort = NULL;
        fRetVal = false;
    }

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
    bool fIsOpen = false;

    if(mApdComPort)
    {
        fIsOpen = mApdComPort->Open;
    }

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
    if(mApdComPort)
    {
        HANDLE hFile = (HANDLE)mApdComPort->Dispatcher->ComHandle;
        SetCommBreak(hFile);
    }
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
    if(mApdComPort)
    {
        HANDLE hFile = (HANDLE)mApdComPort->Dispatcher->ComHandle;
        ClearCommBreak(hFile);
    }
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
    if(mApdComPort)
    {
        mApdComPort->DTR = false;
    }
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
    if(mApdComPort)
    {
        mApdComPort->DTR = true;
    }
}

//=============================================================================
// This function returns the current state of the RTS output of the Laird module
// which will be connected to the CTS input of the host mcu
//=============================================================================
bool TSerialPortTarget::GetCtsState(void)
{
    if( mApdComPort )
    {
        if(mApdComPort->Open)
        {
            return mApdComPort->CTS;
        }
    }
    return false;
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
    if(mApdComPort)
    {
        mApdComPort->FlushInBuffer();
    }
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
    if(mApdComPort)
    {
        mApdComPort->FlushOutBuffer();
    }
}

//=============================================================================
// This function will be called to output the buffer specified to the Laird module
// and the parameter nLen specifies how many bytes in that buffer to send.
//=============================================================================
unsigned
TSerialPortTarget::PutBlock(
    unsigned char *pTxBlock,
    unsigned short nLen
    )
{
    if(mApdComPort)
    {
        mApdComPort->PutBlock(pTxBlock, nLen);
    }
    return nLen;
}

//=============================================================================
// This function is used to call a maximum of nCount bytes from the low level
// buffer and the function will return the actual number returned.
//=============================================================================
unsigned
TSerialPortTarget::GetBlock(
    char *pDest,
    unsigned nCount
    )
{
    unsigned nReadLen = ( nCount >= gRxCount) ? gRxCount : nCount;
    memcpy(pDest,gbaRxBuffer,nReadLen);
    unsigned nRemain = gRxCount - nReadLen;
    if(nRemain)
    {
        memcpy( gbaRxBuffer, &gbaRxBuffer[nReadLen],nRemain);
    }
    gRxCount=nRemain;

    return nReadLen;
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
    if(mApdComPort)
    {
        mApdComPort->Open  = false;
    }
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
    unsigned int nCount=0;
	if (gpSerialPort)
    {
        nCount = gRxCount;
    }
    return nCount;
}

/******************************************************************************/
// END OF FILE
/******************************************************************************/





