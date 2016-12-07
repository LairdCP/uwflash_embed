/******************************************************************************
** Project:     PC Based harness for the interpreter
**              Specific Build Header File
*******************************************************************************/

#if !defined(_TARGET_H)     /* prevent multiple inclusions */
#define _TARGET_H

#include "..\WirelessModule\TargetEmbed.h"

/*
   Set the DEBUGLEVEL to 1..4 to assist with debugging your port
*/
#define DEBUGLEVEL                                  (0)
/*
   If DEBUGLEVEL is non-zero then set the following to a value 
   as high as say 1024 so that large messages can be formatted
   and presented over the UI interface that you can code for
   in the fil AtUpgradeUITarget.cpp
*/
#define ASSERT_MSG_BUF_LEN                          (0)


/*
   Make the following one of these which hard codes the
   module that is attached to your microcontroller
        TARGET_BT900
        TARGET_BL6XX
        TARGET_RM1XX
        TARGET_BL65X
        TARGET_RM1XX_PE

   based on which one you set, you will need to add the 
   apppropriate TBootLdr?????.cpp file in your makefile or IDE
   for example, given TARGET_BL65X is the value below
   then the file to include is WirelessModule/TBootLdrBL65X.cpp
*/
#define TARGET_PLATFORM                             TARGET_BL65X
/*
   Set the baudrate as follows :-
        TARGET_BL65X            115200
        TARGET_BT900            115200
        TARGET_BL6XX            9600
        TARGET_RM1XX            9600
        TARGET_RM1XX_PE         9600
*/
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "8"

/*----------------------------------------------------------------------------*/
/* The following define is checked for in basetype.h                          */
/*----------------------------------------------------------------------------*/
#define TARGET_DEFINED
#endif /* prevent multiple inclusions */
