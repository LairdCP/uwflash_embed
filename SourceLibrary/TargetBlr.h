/******************************************************************************
** Module: Blr
**
*******************************************************************************/

#if !defined(_TARGETBLR_H)     /* prevent multiple inclusions */
#define _TARGETBLR_H

/*
        IF ANY OF THESE VALUES ARE CHANGED DO SO VIA TARGET.H

              WARNING::WARNING::WARNING::WARNING::WARNING

        DO NOT CHANGE THIS FILE, OTHER THAN TO ADD NEW #defines

              WARNING::WARNING::WARNING::WARNING::WARNING

        Even if you want the #define to be undefined by default
        you MUST place the define in this file, but in that case
        prefix the #define name with the word "not"
*/

        /*....................................................................
        ** This is the offset into the code flash where the multibyte 
        ** firmware build is stored in little endien format, which means the
        ** lsb of the build number is at the lowest address.
        ** Specify this in Target.h
        **...................................................................*/
#define notBLR_FIRMWARE_BUILD_NUMBER_OFFSET            (0xSomeAddress)

        /*....................................................................
        ** This identifies the platform so that firmware for another
        ** platform is not mistakenly loaded into this module
        **...................................................................*/
#define notBLR_TARGET_PLATFORM_ID            (0xSome32bitValue)

        /*....................................................................
        ** This is the oldest firmware number which is guaranteed to force
        ** an update from the server
        **...................................................................*/
#define BLR_OLDEST_FIRMWARE_NUMBER           (0)

        /*....................................................................
        ** If this is defined, ask BSP to determine from which bank the
        ** current running app was copied from
        **...................................................................*/
#define notBLR_ASK_BSP_FOR_ACTIVE_BANK


        /*....................................................................
        ** When this is defined, new firmware downloaded from the server 
        ** can NEVER be overwritten
        **...................................................................*/
#define BLR_DO_NOT_OVERWRITE_ACTIVE_FIRMWARE

#endif /* prevent multiple inclusions */
