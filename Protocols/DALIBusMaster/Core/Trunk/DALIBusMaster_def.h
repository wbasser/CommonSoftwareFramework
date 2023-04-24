/******************************************************************************
 * @file DALIBusMaster_def.h
 *
 * @brief DALI Bus manager definitions 
 *
 * This file provides the definitions for the DALI bus master
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 * \addtogroup DALIBusMaster
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DALIBUSMASTER_DEF_H
#define _DALIBUSMASTER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _DALIBUSMASTERSTS
{
  DALIBUSMASTER_STS_NOERROR = 0,      ///< 0x00 - OK transmit, no receive expected
  DALIBUSMASTER_STS_NOERRRCV,         ///< 0x01 - OK transmit, OK receive expected
  DALIBUSMASTER_STS_NOERRNORCV,       ///< 0x02 - OK transmit, RCV Expected, no receive
  DALIBUSMASTER_STS_TMOXMT,           ///< 0x03 - timeout on transmit
  DALIBUSMASTER_STS_BUSERR,           ///< 0x04 - bus error - no receive echo
  DALIBUSMASTER_STS_CMPERR,           ///< 0x05 - echo received, compare error
  DALIBUSMASTER_STS_TMORCV,           ///< 0x06 - timeout on receive
  DALIBUSMASTER_STS_COLLISION,        ///< 0x07 - bus activity - collision
  DALIBUSMASTER_STS_ILLOPERATION,     ///< 0x08 - illegal operation, (i.e. a broadcast query request)
  DALIBUSMASTER_STS_BUSMONITORBAD,    ///< 0x09 - bus monitor indicates bad bus
  DALIBUSMASTER_STS_MAX,
} DALIBUSMASTERSTS;

/// enumerate the bus events
typedef enum _DALIBUSMASTERCTLEVENT
{
  DALIBUSMASTER_CTLEVENT_MSGINQUEUE = 0xE300,
  DALIBUSMASTER_CTLEVENT_DONE,
  DALIBUSMASTER_CTLEVENT_MAX
} DALIBUSMASTERCTLEVENT;

/// enumerate the xmit events
typedef enum _DALIBUSMASTERXMTEVENT
{
  DALIBUSMASTER_XMTEVENT_START = 0xE400,
  DALIBUSMASTER_XMTEVENT_TIMEOUT,
  DALIBUSMASTER_XMTEVENT_MANXMTDONE,
  DALIBUSMASTER_XMTEVENT_MANRCVDONE,
  DALIBUSMASTER_XMTEVENT_MANRCVEROR,
  DALIBUSMASTER_XMTEVENT_MANRCVEDGE,
  DALIBUSMASTER_XMTEVENT_MAX
} DALIBUSMASTERXMTEVENT;

// structures -----------------------------------------------------------------
/// define the callback function
typedef  void ( *PVDALIBUSMSTRCB )( DALIBUSMASTERSTS eStatus, U8 nData );

/// define the DALI message structure
typedef union _DALIXMTMSG
{
  struct
  {
    struct
    {
      BOOL     bLvlCmd : 1;       ///< 0-level, 1-command
      U8       nAddr   : 6;       ///< address
      BOOL     bDirGrp : 1;       ///< 0-direct address, 1-group
    } tAddr;
    U8  nDataCmd;                 ///< data or command
  } tFields;
  U8    anBuffer[ 2 ];
} DALIXMTMSG, *PDALIXMTMSG;
#define DALIXMTMSG_SIZE    sizeof( DALIXMTMSG )

/// define the return message
typedef struct _DALIXMTRCV
{
  DALIBUSMASTERSTS  eStatus;
  DALIXMTMSG        tXmtMsg;
  U8                nRcvMsg;
} DALIXMTRCV, *PDALIXMTRCV;
#define DALIXMTRCV_SIZE     sizeof( DALIXMTRCV )

/// define the message structure
typedef struct _DALIBUSMASTERMSG
{
  PVDALIBUSMSTRCB pvCallbackFunc;
  U32             uOption;
  DALIXMTRCV      tDaliXmtRcvMsg;
} DALIBUSMASTERMSG, *PDALIBUSMASTERMSG;
#define DALIBUSMASTERMSG_SIZE               sizeof ( DALIBUSMASTERMSG )

/**@} EOF DALIBusMaster_def.h */

#endif  // _DALIBUSMASTER_DEF_H