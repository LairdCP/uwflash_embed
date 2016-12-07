/******************************************************************************
** Module: Frm
**
*******************************************************************************/

#if !defined(_TARGETFRM_H)     /* prevent multiple inclusions */
#define _TARGETFRM_H

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
        ** This is the scratchpad size for processing inbound data on the fly
        **...................................................................*/
#define FRM_DOWNLOAD_VIA_SCRATCHPAD_MIN_BUFFER_SIZE     (2048)


#endif /* prevent multiple inclusions */
