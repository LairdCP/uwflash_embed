/******************************************************************************
**              Copyright (C) 2005 Ezurio Ltd
**
** Project:     Universal Wireless
**
** Module:		FRMPRIVATE.C
**
** Notes:       Contains the private routines which should be visible to this
**              module only
**
*******************************************************************************/

/******************************************************************************/
/* Target Build Definition Header File */
/******************************************************************************/
#include "Target.h"

/******************************************************************************/
/* CONDITIONAL COMPILE DEFINES*/
/******************************************************************************/

/******************************************************************************/
/* Include Files*/
/******************************************************************************/
#include <stdlib.h>

#include "FrmPublic.h"
#include "FrmPrivate.h"
#include "FrmPrivateTarget.h"

#include "BlrPublic.h"

/******************************************************************************/
/* Local Defines*/
/******************************************************************************/

/******************************************************************************/
/* Local Macros*/
/******************************************************************************/

/******************************************************************************/
/* Local Forward Class,Struct & Union Declarations*/
/******************************************************************************/

/******************************************************************************/
/* Local Class,Struct,Union Typedefs*/
/******************************************************************************/

/******************************************************************************/
/* External Variable Declarations*/
/******************************************************************************/

/******************************************************************************/
/* Global/Static Variable Declarations*/
/******************************************************************************/

/******************************************************************************/
/* External Function Declarations*/
/******************************************************************************/

/******************************************************************************/
/* Local Forward Function Declarations*/
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/* Local Functions or Private Members*/
/******************************************************************************/
/******************************************************************************/

/*=============================================================================*/
/*
** Change the main state
*/
/*=============================================================================*/
STATIC EFrmSubState
FrmPrvChangeSubState(
    SFrmManager *pContext,
    EFrmSubState newState
    )
{
    pContext->mSubState = newState;
    return pContext->mSubState;
}

/******************************************************************************/
/******************************************************************************/
/* Global Functions or Public Members*/
/******************************************************************************/
/******************************************************************************/

/*=============================================================================*/
/*
** Change the main state
*/
/*=============================================================================*/
EFrmState
FrmPrvChangeMainState(
    SFrmManager *pContext,
    EFrmState newState
    )
{
    if( pContext->mState != newState )
    {
        pContext->mSubState = FRM_SUBSTATE_FIRSTPASS;
    }
    pContext->mState = newState;
    return pContext->mState;
}

/*=============================================================================*/
/*
** Read the little-endien 16 bit value starting at the address specified
*/
/*=============================================================================*/
UI16
FrmPrvRead16(
    UI8 *pStream
    )
{
    UI16 nVal=0;

    nVal = pStream[1];
    nVal <<= 8;
    nVal |= pStream[0];

    return nVal;
}

/*=============================================================================*/
/*
** Read the little-endien 32 bit value starting at the address specified
*/
/*=============================================================================*/
UI32
FrmPrvRead32(
    UI8 *pStream
    )
{
    UI32 nVal=0;

    nVal = pStream[3];
    nVal <<= 8;
    nVal |= pStream[2];
    nVal <<= 8;
    nVal |= pStream[1];
    nVal <<= 8;
    nVal |= pStream[0];

    return nVal;
}

/*=============================================================================*/
/*
** Process command FRM_COMMAND_TARGET_PLATFORM
**
** Read the 32 bit target id and ask boot loader for a match
*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceTargetPlatform(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    )
{
    UI32 nPlatform;

    /*-----------------------------------------------------**
    ** This functionality was tested and confirmed to work
    ** on 24 May 2007. Resultcode 3703 filters through
    ** to the tar function instigating the parsing
    **-----------------------------------------------------*/


    FrmASSERT3(pContext);
    FrmASSERT3(pPacket);
    FrmASSERT3(pConsumed);

    /* Check that there is enough data to for the type of command */
    if(nDataLen < FRM_COMMAND_TARGET_PLATFORM_LEN)
    {
        return UWRESULTCODE_CONTINUE;
    }

    /* Check if packet length is consistant */
    if(pContext->mPacketLength != FRM_COMMAND_TARGET_PLATFORM_LEN)
    {
        return UWRESULTCODE_FRM_PACKET_SIZE_ERROR;
    }

    /* read the platform id from data stream */
    nPlatform = FrmPrvRead32(pPacket);
    (*pConsumed) += FRM_COMMAND_TARGET_PLATFORM_LEN;

    /* After this we need to look for new packet */
    FrmPrvChangeMainState(pContext,FRMSTATE_WAIT_PACKET_HEADER);

    return BlrPubCheckPlatform(pContext->mpBootLoader,nPlatform) 
               ? UWRESULTCODE_SUCCESS 
               : UWRESULTCODE_FRM_PLATFORM_MISMATCH;
}

/*=============================================================================*/
/*
** Process command FRM_COMMAND_SELECT_DEVICE
**
** Read the 8 bit flash device value
** Read the 8 bit bank value. If 0xFF then leave decision to boot loader
*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceSelectDevice(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    )
{
    UWRESULTCODE resultCode;
    FrmASSERT3(pContext);
    FrmASSERT3(pPacket);
    FrmASSERT3(pConsumed);

    /* Check that there is enough data to for the type of command */
    if(nDataLen < FRM_COMMAND_SELECT_DEVICE_LEN)
    {
        return UWRESULTCODE_CONTINUE;
    }

    /* Check if packet length is consistant */
    if(pContext->mPacketLength != FRM_COMMAND_SELECT_DEVICE_LEN)
    {
        return UWRESULTCODE_FRM_PACKET_SIZE_ERROR;
    }

    pContext->mCurrentDevice = *pPacket++;
    pContext->mCurrentBank   = *pPacket++;
    (*pConsumed) += FRM_COMMAND_SELECT_DEVICE_LEN;
    if(pContext->mCurrentBank == FRM_AUTO_SELECT_BANK)
    {
        resultCode = BlrPubAutoSelectBank(pContext->mpBootLoader, 
                                          pContext->mCurrentDevice,
                                          &pContext->mCurrentBank);
        if( resultCode == UWRESULTCODE_BLR_SAME_BUILDS)
        {
            /* All banks seem to have the same information */
            /* hence just select the first bank */
            pContext->mCurrentBank = 0;
        }
        else if(UWFAIL(resultCode))
        {
            return resultCode;
        }
    }

    /* After this we need to look for new packet */
    FrmPrvChangeMainState(pContext,FRMSTATE_WAIT_PACKET_HEADER);

    return BlrPubSetBank(pContext->mpBootLoader,
                         pContext->mCurrentDevice,
                         pContext->mCurrentBank);
}

/*=============================================================================*/
/*
** Process command FRM_COMMAND_REGISTER_DEVICE
**
** Read 1 byte = flash handle
** Read 4 byte = base address of flash
** Read 1 byte = number of banks in flash
** Read 4 byte = size of each bank
** Read 1 byte = Bank Selection Algorithm Handle
*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceRegisterDevice(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    )
{
    UWRESULTCODE resultCode;
    UI32 nBaseAddress;
    UI32 nBankSize;

    FrmASSERT3(pContext);
    FrmASSERT3(pPacket);
    FrmASSERT3(pConsumed);


    /* Check that there is enough data to for the type of command */
    if(nDataLen < FRM_COMMAND_REGISTER_DEVICE_LEN)
    {
        return UWRESULTCODE_CONTINUE;
    }

    /* Check if packet length is consistant */
    if(pContext->mPacketLength != FRM_COMMAND_REGISTER_DEVICE_LEN)
    {
        return UWRESULTCODE_FRM_PACKET_SIZE_ERROR;
    }

    /* Read all data in preparation for calling BLR module */
    nBaseAddress = FrmPrvRead32(&pPacket[1]);
    nBankSize    = FrmPrvRead32(&pPacket[6]);
    /* Update consumed output variable */
    (*pConsumed) += FRM_COMMAND_REGISTER_DEVICE_LEN;

    /* After this we need to look for new packet */
    FrmPrvChangeMainState(pContext,FRMSTATE_WAIT_PACKET_HEADER);

    resultCode = BlrPubRegisterDevice(pContext->mpBootLoader,
                                      pPacket[0], /* flash handle */
                                      nBaseAddress,
                                      pPacket[5], /* number of banks */
                                      nBankSize,
                                      pPacket[10] /* Bank Selection Algorithm Handle */
                                      );
    if(UWFAIL(resultCode))
    {
        return resultCode;
    }

    /* Device was registered successfully so auto select this device and bank 0 */
    pContext->mCurrentDevice = pPacket[0];
    pContext->mCurrentBank   = 0;

    return BlrPubSetBank(pContext->mpBootLoader,
                         pContext->mCurrentDevice,
                         pContext->mCurrentBank);
}

/*=============================================================================*/
/*
** Process command FRM_COMMAND_UNREGISTER_DEVICE
**
** Read 1 byte = handle
*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceUnregisterDevice(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    )
{
    FrmASSERT3(pContext);
    FrmASSERT3(pPacket);
    FrmASSERT3(pConsumed);

    /* Check that there is enough data to for the type of command */
    if(nDataLen < FRM_COMMAND_UNREGISTER_DEVICE_LEN)
    {
        return UWRESULTCODE_CONTINUE;
    }

    /* Check if packet length is consistant */
    if(pContext->mPacketLength != FRM_COMMAND_UNREGISTER_DEVICE_LEN)
    {
        return UWRESULTCODE_FRM_PACKET_SIZE_ERROR;
    }

    /* Update consumed output variable */
    (*pConsumed) += FRM_COMMAND_UNREGISTER_DEVICE_LEN;

    /* After this we need to look for new packet */
    FrmPrvChangeMainState(pContext,FRMSTATE_WAIT_PACKET_HEADER);

    return BlrPubUnregisterDevice(pContext->mpBootLoader,pPacket[0]);
}

/*=============================================================================*/
/*
** Process command FRM_COMMAND_WRITE_BLOCK
**
** Read 4 bytes for address offset
** Read 4 bytes for data block length in bytes
** Then the rest of the data is the data to write
*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceWriteBlock(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    )
{
    UWRESULTCODE resultCode=UWRESULTCODE_SUCCESS;
    UI32 nBlockLen;

    FrmASSERT3(pContext);
    FrmASSERT3(pPacket);
    FrmASSERT3(pConsumed);

    switch(pContext->mSubState)
    {
    case FRM_SUBSTATE_FIRSTPASS:
        if(pContext->mPacketLength <= FRM_COMMAND_WRITE_BLOCK_SUBHDR_LEN)
        {
            /* packet length is not consistant */
            return UWRESULTCODE_FRM_PACKET_SIZE_ERROR;
        }
        else if(nDataLen < FRM_COMMAND_WRITE_BLOCK_SUBHDR_LEN)
        {
            resultCode = UWRESULTCODE_CONTINUE;
        }
        else if(*pConsumed != 0)
        {
            /* Force the data to be shifted to the begining of the buffer */
            /* so that the start data byte is at an even address and */
            /* the length is also even */
            resultCode = UWRESULTCODE_CONTINUE;
        }
        else
        {
            pContext->mWriteOffset   = FrmPrvRead32(&pPacket[0]);
            pContext->mWriteFlags    = FrmPrvRead32(&pPacket[4]);
            pContext->mWriteBlockLen = pContext->mPacketLength-FRM_COMMAND_WRITE_BLOCK_SUBHDR_LEN;
            if(pContext->mWriteBlockLen==0)
            {
                /* Writing 0 bytes does not make sense */
                return UWRESULTCODE_FRM_INVALID_PARAMETER;
            }
            else
            {
                /* At least one byte will be written */
                (*pConsumed) += FRM_COMMAND_WRITE_BLOCK_SUBHDR_LEN;
                FrmPrvChangeSubState(pContext,FRM_SUBSTATE_WRITEDATA);

                resultCode = BlrPubWriteBlockHeader(pContext->mpBootLoader,
                                                    pContext->mWriteOffset,
                                                    pContext->mWriteBlockLen,
                                                    pContext->mWriteFlags);
                if(UWFAIL(resultCode))
                {
                    return resultCode;
                }

            }
        }
        break;

    case FRM_SUBSTATE_WRITEDATA:
        FrmASSERT3(nDataLen);
        /* There is enough data to write */
        nBlockLen = nDataLen;
        if(nBlockLen>pContext->mWriteBlockLen)
        {
            nBlockLen=pContext->mWriteBlockLen;
        }
        resultCode = BlrPubWriteBlock(pContext->mpBootLoader,
                                      pPacket,
                                      pContext->mWriteOffset,
                                      nBlockLen);
        (*pConsumed) += nBlockLen;
        pContext->mWriteBlockLen -= nBlockLen;
        pContext->mWriteOffset   += nBlockLen;
        if(pContext->mWriteBlockLen == 0)
        {
            /* All data in this write block has been written */
            if(UWSUCCESS(resultCode))
            {
                resultCode = BlrPubWriteBlockEnd(pContext->mpBootLoader);
            }
            FrmPrvChangeMainState(pContext,FRMSTATE_WAIT_PACKET_HEADER);
        }
        break;

    default:
        FrmFAIL("Unexpected sub state");
        break;

    }
    return resultCode;
}

/*=============================================================================*/
/*
** Process command FRM_COMMAND_ERASE_BLOCK
**
** Read 4 byte = Start Offset 
** Read 4 byte = End Offset
*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceEraseBlock(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    )
{
    UI32 nStartAddress;
    UI32 nBlockSize;

    FrmASSERT3(pContext);
    FrmASSERT3(pPacket);
    FrmASSERT3(pConsumed);

    /* Check that there is enough data to for the type of command */
    if(nDataLen < FRM_COMMAND_ERASE_BLOCK_LEN)
    {
        return UWRESULTCODE_CONTINUE;
    }

    /* Check if packet length is consistant */
    if(pContext->mPacketLength != FRM_COMMAND_ERASE_BLOCK_LEN)
    {
        return UWRESULTCODE_FRM_PACKET_SIZE_ERROR;
    }

    /* Read all data in preparation for calling BLR module */
    nStartAddress = FrmPrvRead32(&pPacket[0]);
    nBlockSize    = FrmPrvRead32(&pPacket[4]);
    /* Update consumed output variable */
    (*pConsumed) += FRM_COMMAND_ERASE_BLOCK_LEN;

    if(nBlockSize==0)
    {
        return UWRESULTCODE_FRM_INVALID_PARAMETER;
    }

    /* After this we need to look for new packet */
    FrmPrvChangeMainState(pContext,FRMSTATE_WAIT_PACKET_HEADER);

    /*
    FrmTRACE3(("ERASE_BLOCK offset=0x%08X, bytes=%u\n",nStartAddress,nBlockSize));
    */
    return BlrPubEraseBlock(pContext->mpBootLoader,
                            nStartAddress,
                            nBlockSize);
}

/*=============================================================================*/
/*
** Process command FRM_COMMAND_QUERY
**
** Read 4 byte = Query Id 
** Read 4 byte = Query Value 1
** Read 4 byte = Query Value 2
*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceQuery(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    )
{
    UI32 nQueryId;
    UI32 nQueryVal[2];

    FrmASSERT3(pContext);
    FrmASSERT3(pPacket);
    FrmASSERT3(pConsumed);

    /* Check that there is enough data to for the type of command */
    if(nDataLen < FRM_COMMAND_QUERY_BLOCK_LEN)
    {
        return UWRESULTCODE_CONTINUE;
    }

    /* Check if packet length is consistant */
    if(pContext->mPacketLength != FRM_COMMAND_QUERY_BLOCK_LEN)
    {
        return UWRESULTCODE_FRM_PACKET_SIZE_ERROR;
    }

    /* Read all data in preparation for calling BLR module */
    nQueryId      = FrmPrvRead32(&pPacket[0]);
    nQueryVal[0]  = FrmPrvRead32(&pPacket[4]);
    nQueryVal[1]  = FrmPrvRead32(&pPacket[8]);
    /* Update consumed output variable */
    (*pConsumed) += FRM_COMMAND_QUERY_BLOCK_LEN;

    /* After this we need to look for new packet */
    FrmPrvChangeMainState(pContext,FRMSTATE_WAIT_PACKET_HEADER);

    /*
    FrmTRACE3(("QUERY id=%u val1=%u val2=%u\n",nQueryId,nQueryVal[0],nQueryVal[1]));
    */
    return BlrPubQuery(pContext->mpBootLoader,nQueryId,nQueryVal,2);
}

/*=============================================================================*/
/*
** Process command FRM_COMMAND_SECTOR_MAP
**
** An array of two 4 byte integers. First number of the pair is the number of
** sectors of size specified in the second number of the pair. Hence each
** pair consists of 8 bytes.
** The number of pairs is derived by dividing the packet length by 8. And it
** will FAIL with a resultcode if the packet length is not a multiple of 8
*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvServiceSectorMap(
    SFrmManager *pContext,
    UI8 *pPacket,
    UI32 nDataLen,
    UI32 *pConsumed
    )
{
    UWRESULTCODE resultCode;
    SBlrSectorMapItem *pSectorItem;
    I32 nNumOfItems;
    UI32 nOffset;
    SBlrSectorMapItem *pSectorMapArray=NULL;


    FrmASSERT3(pContext);
    FrmASSERT3(pPacket);
    FrmASSERT3(pConsumed);

    /* Check there is at least one item in the packet */
    if(pContext->mPacketLength==0)
    {
        return UWRESULTCODE_FRM_SECTORMAP_EMPTY;
    }

    /* Check there is at least one item in the packet */
    if(pContext->mPacketLength<FRM_SECTORMAP_ITEM_LEN)
    {
        return UWRESULTCODE_FRM_SECTORMAP_INCOMPLETE;
    }

    /* Check that the packet payload length is a multiple of FRM_SECTORMAP_ITEM_LEN */
    if( (pContext->mPacketLength%FRM_SECTORMAP_ITEM_LEN) != 0)
    {
        return UWRESULTCODE_FRM_SECTORMAP_SIZEERR;
    }

    /* Check that there is enough data for the type of command */
    if(nDataLen < pContext->mPacketLength)
    {
        return UWRESULTCODE_CONTINUE;
    }

    /* Calculate the number of items */
    nNumOfItems = pContext->mPacketLength/FRM_SECTORMAP_ITEM_LEN;
    FrmASSERT3((pContext->mPacketLength%FRM_SECTORMAP_ITEM_LEN) == 0);

    /* Allocate memory for it */
    pSectorMapArray = (SBlrSectorMapItem *)FrmMALLOC(nNumOfItems * sizeof(SBlrSectorMapItem));
    if(pSectorMapArray==NULL)
    {
        return UWRESULTCODE_MALLOC_FAIL;
    }

    /* Read all data in preparation for calling BLR module */
    nOffset=0;
    pSectorItem=pSectorMapArray;
    while(nOffset<pContext->mPacketLength)
    {
        /* If this assert fails then need to update this loop */
        FrmASSERT3(FRM_SECTORMAP_ITEM_LEN==8);

        /* so a sanity check on the number of items */
        if(nNumOfItems <= 0 )
        {
            return UWRESULTCODE_FRM_SECTORMAP_ITEMERR;
        }

        /* Finally read the data into the item struct */
        pSectorItem->mNumOfSectors = FrmPrvRead32(&pPacket[nOffset]);
        nOffset+=4;
        pSectorItem->mSectorSize   = FrmPrvRead32(&pPacket[nOffset]);
        nOffset+=4;
        pSectorItem++;
        nNumOfItems--;
    }
    
    
    /* Update consumed output variable */
    (*pConsumed) += pContext->mPacketLength;

    /* After this we need to look for new packet */
    FrmPrvChangeMainState(pContext,FRMSTATE_WAIT_PACKET_HEADER);

    resultCode = BlrPubSectorMap(pContext->mpBootLoader,
                                 pSectorMapArray,
                                 (pContext->mPacketLength/FRM_SECTORMAP_ITEM_LEN));
    /*
    FrmTRACE3(("SECTOR_MAP items=%u\n",(pContext->mPacketLength/FRM_SECTORMAP_ITEM_LEN)));
    */
    FrmFREE(&pSectorMapArray);
    return resultCode;
}

/*=============================================================================*/
/*
** Open the firmware manager session
*/
/*=============================================================================*/
SFrmManager *
FrmPrvOpenManager(
    UWRESULTCODE *pResultCode
    )
{
    /* Allocate memory for the context */
    SFrmManager *pContext = (SFrmManager*)FrmMALLOC(sizeof(SFrmManager));
    if(pContext)
    {
        /* Set the context block to 0 */
        StdMEMSET(pContext,0,sizeof(SFrmManager));

        /* Initialise values */
        pContext->mState         = FRMSTATE_WAIT_1ST_PACKET_HEADER;
        pContext->mCurrentDevice = FRM_DEVICE_ID_INVALID;
        pContext->mCurrentBank   = FRM_BANK_ID_INVALID;

        /* Initialise the boot loader */
        pContext->mpBootLoader = BlrPubOpen(pResultCode);
        if( pContext->mpBootLoader == NULL)
        {
            /* Target specific initialise failed */
            FrmFREE(&pContext);
            return NULL;
        }
    }

    return pContext;
}

/*=============================================================================*/
/*
** Close the firmware manager session
*/
/*=============================================================================*/
VOID
FrmPrvCloseManager(
    SFrmManager **ppContext
    )
{
    SFrmManager *pContext;

    if( ppContext  )
    {
        if( *ppContext )
        {
            pContext = *ppContext;
            BlrPubClose(pContext->mpBootLoader);
            FrmFREE(ppContext);
        }
        *ppContext = NULL;
    }
}

/*=============================================================================*/
/*
** Ask the firmware file processor if it thinks that the processing can end
** without error.
*/
/*=============================================================================*/
BOOLEAN
FrmPrvCanClose(
    SFrmManager *pContext
    )
{
    if( (pContext->mState == FRMSTATE_WAIT_1ST_PACKET_HEADER) ||
        (pContext->mState == FRMSTATE_WAIT_PACKET_HEADER)
      )
    {
        return TRUE;
    }
    return FALSE;
}

/*=============================================================================*/
/*
** Process a firmware packet starting at pPacket, given that nDataLen bytes 
** occur from that point.
** It will return the number of bytes consumed in pConsumed an the return value
** will consist of a resultcode.
**
** Each Packet format is as follows:-
**
**  ALL MULTIBYTE VALUE ARE LITTLE_ENDIEN. THAT IS, LOWEST SIGNIFICANT BYTE
**  WILL BE AT THE LOWEST ADDRESS
**
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** PACKET_HEADER
** ............................................................................
** Offset   Length  Description
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** 0        1       Command Id
** 1        1       Future Expansion
** 2        4       Number of bytes in this packet - excluding these 6
**
** The following commands are processed:-
**
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** TARGET_PLATFORM -- Command Id = 'T'
** Specifies the target this file is meant for.
** ............................................................................
** Offset   Length  Description
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** 0        4       Target Id :- 0x00000100    Wits Vehicle Unit
**
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** REGISTER_DEVICE -- Command Id = 'G'
** Registers details of a flash device and automatically selects it and makes
** bank 0 active. Basically SELECT_DEVICE(h,0) after this packet is redundant
** ............................................................................
** Offset   Length  Description
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** 0        1       Handle
** 1        4       Base Address of the Flash
** 5        1       Number of Banks in this flash
** 6        4       Size of each bank
** 10       1       Bank Selection Algorithm Handle
**
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** UNREGISTER_DEVICE -- Command Id = 'U'
** Unregisters details of a flash device
** ............................................................................
** Offset   Length  Description
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** 0        1       Handle
**
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** SELECT_DEVICE -- Command Id = 'S'
** Select a flash and also the bank to view.
** ............................................................................
** Offset   Length  Description
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** 0        1       Flash Device Number 0..255
** 1        1       Set the Bank number 0..255
**                    If bankNum = 255, then the bank is auto selected by
**                    the boot loader.
**
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** WRITE_BLOCK -- Command Id = 'W'
** For the currently selected flash and bank write data
** ............................................................................
** Offset   Length  Description
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** 0        4       Offset address
** 4        4       Flags for future use
**
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** ERASE_BLOCK -- Command Id = 'E'
** For the currently selected flash and bank erase from the offset supplied
** and the number of bytes requested
** The erase operation is most likely not granular to byte level, hence this
** operation will start from the sector in which the first offset resides and
** will erase up to and including the end offset.
** ............................................................................
** Offset   Length  Description
** ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
** 0        4       Start Offset address
** 4        4       Number of bytes to erase
**
**
*/
/*=============================================================================*/
UWRESULTCODE
FrmPrvProcessData(
    SFrmManager *pContext,
    UI8 *pPacketData,
    UI32 nPacketLen,
    UI32 *pConsumed
    )
{
    UWRESULTCODE resultCode;
    UI8 *pPacket;
    UI32 nDataLen;
    BOOLEAN fLoop;

    FrmASSERT3(pContext);
    FrmASSERT3(pPacketData);
    FrmASSERT3(pConsumed);

    /* Initialise consumed output */
    (*pConsumed) = 0;

    /* Now process the packet data as per the state we are in */
    nDataLen   = nPacketLen - (*pConsumed);
    pPacket    = pPacketData + (*pConsumed);
    fLoop = TRUE;
    while( fLoop )
    {
        /* Assume we will not loop again */
        fLoop = FALSE;

        /* process according to state */
        switch(pContext->mState)
        {
        case FRMSTATE_WAIT_1ST_PACKET_HEADER:
        case FRMSTATE_WAIT_PACKET_HEADER:
            /* Check if the packet main header has arrived */
            if( nDataLen < FRM_PACKET_HEADER_LENGTH )
            {
                resultCode = UWRESULTCODE_CONTINUE;
                break;
            }
            /* get the length of the packet */
            pContext->mPacketLength = FrmPrvRead32(&pPacket[FRM_PACKET_HEADER_OFFSET_LENGTH]);
            /* get the command id */
            pContext->mCommandId = pPacket[FRM_PACKET_HEADER_OFFSET_CMDID];
            /* adjust the packet */
            pPacket  += FRM_PACKET_HEADER_LENGTH;
            nDataLen -= FRM_PACKET_HEADER_LENGTH;
            (*pConsumed) += FRM_PACKET_HEADER_LENGTH;
            if( pContext->mState == FRMSTATE_WAIT_1ST_PACKET_HEADER)
            {
                /* This is the first packet */
                FrmPrvChangeMainState(pContext,FRMSTATE_PROCESS_1ST_PACKET);
            }
            else
            {
                /* This is at least the second packet */
                FrmPrvChangeMainState(pContext,FRMSTATE_PROCESS_PACKET);
            }
            fLoop = TRUE;
            break;

        case FRMSTATE_PROCESS_1ST_PACKET:
            if( pContext->mCommandId == FRM_COMMAND_TARGET_PLATFORM )
            {
                resultCode = FrmPrvServiceTargetPlatform(pContext,pPacket,nDataLen,pConsumed);
            }
            else
            {
                return UWRESULTCODE_FRM_PLATFORM_UNDEFINED;
            }
            break;

        case FRMSTATE_PROCESS_PACKET:
            switch(pContext->mCommandId)
            {
            case FRM_COMMAND_TARGET_PLATFORM:
                return UWRESULTCODE_FRM_PLATFORM_REDEFINE;

            case FRM_COMMAND_SELECT_DEVICE:
                resultCode = FrmPrvServiceSelectDevice(pContext,pPacket,nDataLen,pConsumed);
                break;

            case FRM_COMMAND_REGISTER_DEVICE:
                resultCode = FrmPrvServiceRegisterDevice(pContext,pPacket,nDataLen,pConsumed);
                break;

            case FRM_COMMAND_UNREGISTER_DEVICE:
                resultCode = FrmPrvServiceUnregisterDevice(pContext,pPacket,nDataLen,pConsumed);
                break;

            case FRM_COMMAND_WRITE_BLOCK:
                resultCode = FrmPrvServiceWriteBlock(pContext,pPacket,nDataLen,pConsumed);
                break;

            case FRM_COMMAND_ERASE_BLOCK:
                resultCode = FrmPrvServiceEraseBlock(pContext,pPacket,nDataLen,pConsumed);
                break;

            case FRM_COMMAND_SECTOR_MAP:
                resultCode = FrmPrvServiceSectorMap(pContext,pPacket,nDataLen,pConsumed);
                break;

            case FRM_COMMAND_QUERY:
                resultCode = FrmPrvServiceQuery(pContext,pPacket,nDataLen,pConsumed);
                break;

            default:
                resultCode = UWRESULTCODE_FRM_UNKNOWN_COMMAND;
                break;
            }
            break;

        default:
            FrmFAIL("Unexpected FRM state");
            break;
        }
        /* Update temp length variable containing the fragment still to be processed */
        FrmASSERT3(nPacketLen>=(*pConsumed));
        nDataLen = nPacketLen - (*pConsumed);
        /* make temp pointer to start of unprocessed data */
        pPacket    = pPacketData + (*pConsumed);

        /* should we loop again */
        fLoop = fLoop || ((nDataLen>0) && (resultCode==UWRESULTCODE_SUCCESS));
    }
    return resultCode;
}

/******************************************************************************/
/* END OF FILE*/
/******************************************************************************/


