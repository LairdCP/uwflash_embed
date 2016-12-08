/******************************************************************************
** Project:     PC Based harness for the interpreter
**              Specific Build Header File
*******************************************************************************/

#if !defined(_TARGET_H)     /* prevent multiple inclusions */
#define _TARGET_H

#include "..\WirelessModule\TargetEmbed.h"

#define DEBUGLEVEL                                  (2)
#define ASSERT_MSG_BUF_LEN                          (1024)

#undef  UWF_FILENAME

#if 1
#define TARGET_PLATFORM                             TARGET_BL65X
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "8"
#define UWF_FILENAME                                "BL652.uwf"
#endif

#if 0
#define TARGET_PLATFORM                             TARGET_BL6XX
#define UART_BAUDRATE_NORMAL_OPERATION              (9600)
#define SERIALPORT_DEVICE_NAME                      "9"
#define UWF_FILENAME                                "BL600.uwf"
#endif

#if 0
#define TARGET_PLATFORM                             TARGET_BT900
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "14"
#define UWF_FILENAME                                "BT900.uwf"
#endif

#if 0
#define TARGET_PLATFORM                             TARGET_RM1XX
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "1"
#define UWF_FILENAME                                "RM1XX.uwf"
#endif

#if 0
#define TARGET_PLATFORM                             TARGET_RM1XX_PE
#define UART_BAUDRATE_NORMAL_OPERATION              (115200)
#define SERIALPORT_DEVICE_NAME                      "1"
#define UWF_FILENAME                                "RM1XX_PE.uwf"
#endif


/*----------------------------------------------------------------------------*/
/* The following define is checked for in basetype.h                          */
/*----------------------------------------------------------------------------*/
#define TARGET_DEFINED
#endif /* prevent multiple inclusions */
