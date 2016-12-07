/******************************************************************************
**              Copyright (C) 2002 TDK Systems Europe Ltd
**
** Project:     Application for generating an .SMU upgrade file which
**              contains .psr and .dfu information
**
** Module:      MISC.H
**
*******************************************************************************/


#if !defined(_MISC_H)     /* prevent multiple inclusions */
#define _MISC_H

/******************************************************************************/
// Include Files
/******************************************************************************/


/******************************************************************************/
// Defines
/******************************************************************************/

/******************************************************************************/
// Macros
/******************************************************************************/

/******************************************************************************/
// Simple (non struct/union) Typedefs
/******************************************************************************/

/******************************************************************************/
// Enum Typedefs
/******************************************************************************/

/******************************************************************************/
// Forward declaration of Class, Struct & Unions
/******************************************************************************/

/******************************************************************************/
// Class definitions
/******************************************************************************/

/******************************************************************************/
// Struct definitions
/******************************************************************************/


/******************************************************************************/
// Union definitions
/******************************************************************************/

/******************************************************************************/
// Global Functions (API etc) exported for other modules
/******************************************************************************/

UI8 MiscConvertHexChr2Nibble(UI8 ch);
unsigned long MiscHexStr2Bin(char *pHexStr);
UI8 MiscConvert2HexChr2Binary(UI8 *pSrc);
UI8 MiscConvertNibbleToAscii(UI8 val);
bool MiscAllHexDigits(char *pSrc, int nLen);
bool MiscAllDecDigits(char *pSrc, int nLen);
unsigned char *MiscParseHexValue(char *pSrc, int nSrcLen, unsigned char *pDst, int nDstLen);
unsigned int MiscExtractDecValue(char *pSrc, int nSrcLen);
unsigned char *MiscParseDecValue(char *pSrc, int nSrcLen, unsigned char *pDest, int nDestLen, unsigned int *piVal);
UI16 MiscConvertHexStringToBinary( char *pSrc, UI8 *pDst );
unsigned char MiscConvertHexChr2Nibble(char ch);
bool MiscPollApplicationEvents(void); //Returns true if application is to be closed

unsigned int MiscElapsedTime(unsigned int nStartTick);

/* Following need to be defined in a Target specific file - eg WinMisc.cpp or LinuxMisc.cpp*/
void MiscSleep(unsigned int nTimeMs);
unsigned int MiscGetTickCount();
void MiscCloseApp( int exitVal);
bool MiscPollBackground(void);
bool MiscPollBackgroundTarget(void);



#endif /* Prevention of multiple inclusion */
/******************************************************************************/
// END OF FILE
/******************************************************************************/
