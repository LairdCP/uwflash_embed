// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Copyright (c) 2016, Laird                                                  ++
//                                                                            ++
// Permission to use, copy, modify, and/or distribute this software for any   ++
// purpose with or without fee is hereby granted, provided that the above     ++
// copyright notice and this permission notice appear in all copies.          ++
//                                                                            ++
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES   ++
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF           ++
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR    ++
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES     ++
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN      ++
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR ++
// IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.                ++
//                                                                            ++
// SPDX-License-Identifier:ISC                                                ++
//                                                                            ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                                                                            ++
// Source to embed firmware upgrader functionality into a host system which   ++
// interacts with specific Laird modules. The host can be a microcontroller   ++
// or full blown OS based PC like Windows/Linex/Mac or other.                 ++
//                                                                            ++
// The source requires a C++ compiler and has been used to create utilities   ++
// supplied by Laird.                                                         ++
//                                                                            ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/******************************************************************************/
// Include Files
/******************************************************************************/
#include <stdlib.h>
#include <ctype.h>

#include "Target.h"
#include "basetype.h"
#include "Misc.h"
#include "TSerialPortBase.h"


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


/******************************************************************************/
/******************************************************************************/
// Global Functions or Public Members
/******************************************************************************/
/******************************************************************************/

//=============================================================================
//=============================================================================
UI8 MiscConvertHexChr2Nibble(UI8 ch)
{
    if( (ch>='0') && (ch<='9') )
    {
        return (ch - '0');
    }
    else if( (ch>='A') && (ch<='F') )
    {
        return (ch - 'A' + 10);
    }
    else if( (ch>='a') && (ch<='f') )
    {
        return (ch - 'a' + 10);
    }
    return 0;
}

//=============================================================================
//=============================================================================
unsigned long MiscHexStr2Bin(char *pHexStr)
{
    unsigned long nRetVal=0;

    while(*pHexStr)
    {
        nRetVal <<= 4;
        nRetVal += MiscConvertHexChr2Nibble(*pHexStr);
        pHexStr++;
    }

    return nRetVal;
}

//=============================================================================
//=============================================================================
UI8 MiscConvert2HexChr2Binary(UI8 *pSrc)
{
    return (MiscConvertHexChr2Nibble(pSrc[0])<<4) + MiscConvertHexChr2Nibble(pSrc[1]);
}


//=============================================================================
// Converts the byte into a two character string - UNTERMINATED
//=============================================================================
UI8 MiscConvertNibbleToAscii(UI8 val)
{
    val &= 0x0F;

    if( val <= 9 )
    {
        val += '0';
    }
    else
    {
        val += ('A' - 10);
    }

    return val;
}

//=============================================================================
//=============================================================================
bool MiscAllHexDigits(char *pSrc, int nLen)
{
    if( pSrc == NULL) return false;
    if( nLen == 0   ) return false;

    while( nLen-- )
    {
        if( ! isxdigit(*pSrc++) )
        {
            return false;
        }
    }
    return true;
}

//=============================================================================
//=============================================================================
bool MiscAllDecDigits(char *pSrc, int nLen)
{
    if( pSrc == NULL) return false;
    if( nLen == 0   ) return false;

    while( nLen-- )
    {
        if( ! isdigit(*pSrc++) )
        {
            return false;
        }
    }
    return true;
}

//=============================================================================
//=============================================================================
unsigned char *MiscParseHexValue(char *pSrc, int nSrcLen, unsigned char *pDst, int nDstLen)
{
    unsigned char *pRetVal=NULL;
    if( nSrcLen == (nDstLen*2) )
    {
        if( MiscAllHexDigits(pSrc,nSrcLen) )
        {
            pRetVal=pDst;
            for(int i=0;i<nDstLen;i++)
            {
                *pDst++=MiscConvert2HexChr2Binary((UI8 *)pSrc);
                pSrc+=2;
            }
        }
    }
    return pRetVal;
}

//=============================================================================
//=============================================================================
unsigned int MiscExtractDecValue(char *pSrc, int nSrcLen)
{
    unsigned int nRetVal=0;
    if( nSrcLen > 0 )
    {
        while(nSrcLen--)
        {
            nRetVal *= 10;
            if( isdigit(*pSrc))
            {
                nRetVal += (*pSrc) - '0';
            }
            pSrc++;
        }
    }
    return nRetVal;
}

//=============================================================================
//=============================================================================
unsigned char *MiscParseDecValue(char *pSrc, int nSrcLen, unsigned char *pDest, int nDestLen, unsigned int *piVal)
{
    if( MiscAllDecDigits(pSrc,nSrcLen) )
    {
        unsigned int nValue = MiscExtractDecValue(pSrc, nSrcLen);
        *piVal = nValue;
        char *pDst = (char *)&pDest[nDestLen-1];
        while( nDestLen--)
        {
            *pDst-- = nValue & 0xFF;
            nValue >>= 8;
        }
        return pDest;
    }

    return NULL;
}

//=============================================================================
//=============================================================================
UI16 MiscConvertHexStringToBinary( char *pSrc, UI8 *pDst )
{
    UI16 nCount=0;
    UI8 nLow=1;
    UI8 nVal;

    while( *pSrc )
    {
        if( ! isxdigit(*pSrc) )
        {
            break;
        }

        nVal = MiscConvertHexChr2Nibble(*pSrc++);
        if(nLow&0x01)
        {
            *pDst = nVal;
            nCount++;
        }
        else
        {
            *pDst    <<= 4;
            *pDst++  += nVal;
        }
        nLow++;
    }
    return nCount;
}

//=============================================================================
//=============================================================================
unsigned char MiscConvertHexChr2Nibble(char ch)
{
    if( (ch>='0') && (ch<='9') )
    {
        return (ch - '0');
    }
    else if( (ch>='A') && (ch<='F') )
    {
        return (ch - 'A' + 10);
    }
    else if( (ch>='a') && (ch<='f') )
    {
        return (ch - 'a' + 10);
    }
    return 0;
}

//=============================================================================
//=============================================================================
bool MiscPollApplicationEvents(void)
{
    return MiscPollBackground();
}

//=============================================================================
//=============================================================================
void MiscSleep(unsigned int nTimeMs)
{
    unsigned int nStart = MiscGetTickCount();
    unsigned int nElapsed=0;
    unsigned int nNow;

    while( nElapsed < nTimeMs )
    {
        MiscPollBackground();

        /* calculate time elapsed */
        nNow = MiscGetTickCount();
        nElapsed = nNow-nStart;
        if( nNow < nStart ){ nElapsed = nNow-nStart+0xFFFFFFFF; }
    }
}

//=============================================================================
//=============================================================================
unsigned int MiscElapsedTime(unsigned int nStartTick)
{
    unsigned int nElapsed = MiscGetTickCount();

    //Calculate time elapsed
    if( nElapsed <  nStartTick )
    {
        nElapsed += (0xFFFFFFFF -  nStartTick);
    }
    else
    {
        nElapsed -=  nStartTick;
    }
    return nElapsed;
}

//=============================================================================
//=============================================================================
void MiscCloseApp( int exitVal)
{
    exit(exitVal);
}

//------------------------------------------------------------------------------
//Returns true if application is to be closed
//------------------------------------------------------------------------------
bool MiscPollBackground(void)
{
    bool fRetVal=MiscPollBackgroundTarget();
    if(fRetVal==false)
    {
        PollSerialRxData();
    }
    return fRetVal;
}

/******************************************************************************/
// END OF FILE
/******************************************************************************/
