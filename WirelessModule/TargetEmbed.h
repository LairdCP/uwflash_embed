/******************************************************************************
** Project:
*******************************************************************************/

#if !defined(_TARGETEMBED_H)     /* prevent multiple inclusions */
#define _TARGETEMBED_H


/*---------------------------------------------------------------------------*/
/* The following #defines need to exist for all builds                        */
/*---------------------------------------------------------------------------*/
#define ON_TARGET_PC
#define ON_SUBTARGET_UWFMAKER
#define ON_TARGET_WINDOWS


#define notDISABLE_INLINE

#define EXCLUDE_VERSION_CORE_HEADER

/*---------------------------------------------------------------------------*/
/* Include Module specific target defines */
/*---------------------------------------------------------------------------*/
#include "TargetUwg.h"
#include "TargetRbf.h"
#include "TargetStd.h"
#include "TargetFrm.h"
#include "TargetBlr.h"

#define notDISPLAY_WRITE_BLOCK_INFO

#define UWF_FIRMWAREFILE_MAX_LINE_LEN               (1024)

#define BLR_FIRMWARE_BUILD_NUMBER_OFFSET            (0xdeadc0de not used)
#define BLR_TARGET_PLATFORM_ID                      (0xdeadcode not used)

#define BT900_FLASHUPLOAD_MODE_BAUDRATE             (115200)
#define RM1XX_FLASHUPLOAD_MODE_BAUDRATE             (115200)
#define RM1XX_PE_FLASHUPLOAD_MODE_BAUDRATE          (115200)

#define UWF_FILENAME                                "*"

#define UART_BAUDRATE_BOOTLOADER                    (115200)

#define COMPORT_POLL_WAIT_TIME_MS                   (1000)


/*---------------------------------------------------------------------------*/
/* The following define is checked for in basetype.h                         */
/*---------------------------------------------------------------------------*/
#define TARGET_DEFINED
#endif /* prevent multiple inclusions */
