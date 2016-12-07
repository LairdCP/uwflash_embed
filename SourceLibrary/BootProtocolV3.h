/******************************************************************************
**              Copyright (C) 2007 TDK Systems Europe Ltd
**
** Project:     WITS/WISM
**
** Module:      BootProtocolV3.h
**
**              This file is shared between UwFlash and the BootLoader
**
*******************************************************************************/

#if !defined(_BOOTPROTOCOLV3_H)     /* prevent multiple inclusions */
#define _BOOTPROTOCOLV3_H

/******************************************************************************/
// Defines
/******************************************************************************/


/* The following are for legacy devices */
#define ATMEL_MSGID_DEF_SYNC                 (0x80)
#define ATMEL_MSGID_DEF_ACK                  ('A')
#define ATMEL_MSGID_DEF_NACK                 ('N')
#define ATMEL_MSGID_DEF_ERASE                ('E')
#define ATMEL_MSGID_DEF_WRITE                ('W')
#define ATMEL_MSGID_DEF_READ                 ('R')
#define ATMEL_MSGID_DEF_DATA                 ('D')
#define ATMEL_MSGID_DEF_ERROR                ('F')
#define ATMEL_MSGID_DEF_ATS                  ('B')
#define ATMEL_MSGID_DEF_VERIFY               ('V')
#define ATMEL_MSGID_DEF_RESET                ('Z')
#define ATMEL_MSGID_DEF_QUERY                ('Q')


/* The following are for latest devices */
#define ATMEL_MSGID_WITS_SYNC                 (0x80)
#define ATMEL_MSGID_WITS_ACK                  ('a')
#define ATMEL_MSGID_WITS_NACK                 ('n')
#define ATMEL_MSGID_WITS_ERASE                ('e')
#define ATMEL_MSGID_WITS_WRITE                ('w')
#define ATMEL_MSGID_WITS_READ                 ('r')
#define ATMEL_MSGID_WITS_DATA                 ('d')
#define ATMEL_MSGID_WITS_ERROR                ('f')
#define ATMEL_MSGID_WITS_ATS                  ('b')
#define ATMEL_MSGID_WITS_VERIFY               ('v')
#define ATMEL_MSGID_WITS_PLATFORM             ('p')
#define ATMEL_MSGID_WITS_RESET                ('z')
#define ATMEL_MSGID_WITS_QUERY                ('q')


#define ATMEL_ERROR_WRITE                     (1)
#define ATMEL_ERROR_READ                      (2)
#define ATMEL_ERROR_ERASE                     (3)
#define ATMEL_ERROR_UNRECOGNISED              (4)
#define ATMEL_ERROR_PLATFORM                  (5)


#endif /* Prevention of multiple inclusion */
/******************************************************************************/
// END OF FILE
/******************************************************************************/
