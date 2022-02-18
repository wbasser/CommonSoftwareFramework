/******************************************************************************
 * @file BinaryProtocoDynamic.h
 *
 * @brief Cyber BInary protocol handler declarations 
 *
 * This file provides the declarations for the Cyber Binary protocol
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
 *
 * \addtogroup BinaryProtocoDynamic
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BINARYPROTOCOLDYNAMIC_H
#define _BINARYPROTOCOLDYNAMIC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
// special characters
#define CH_ACK  0x06
#define CH_NAK  0x15

/// define the helper macro for creating command table entries
#define BINPROTCMDTBLDEF( cmd, handler ) \
  { \
    .nCommand = cmd, \
    .pvCmdHandler = handler \
  }

/// define the helper macro for defining the command table end
#define BINPROTCMDTBLEND( ) \
  { \
    .nCommand = 0, \
    .pvCmdHandler = NULL \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the message decode status
typedef enum _BINPROTDYNSTS
{
  BINPROTDYN_STS_IDLE = 0,
  BINPROTDYN_STS_MSGINPROG,
  BINPROTDYN_STS_MSGCOMP,
  BINPROTDYN_STS_MSGERR
} BINPROTDYNSTS;

/// enumerate the command handler return status
typedef enum _BINPROTDYNRSP
{
  BINPROTDYN_RSP_ACK = 0x80,
  BINPROTDYN_RSP_ACKDAT,
  BINPROTDYN_RSP_NAK,
  BINPROTDYN_RSP_NOCMD,
} BINPROTDYNRSP;

// structures -----------------------------------------------------------------
/// define the write function
typedef void ( *PVWRITECHARFUNC )( U8 );

/// define the command handler function prototype
typedef BINPROTDYNRSP ( *PVCMDHANDLER )( void );

/// define the structure
typedef struct _BINPROTCMDTBL
{
  U8              nCommand;
  PVCMDHANDLER    pvCmdHandler;
} BINPROTCMDTBL, *PBINPROTCMDTBL;
#define BINPROTCMDTBL_SIZE                      sizeof( BINPROTCMDTBL )

/// define the receive message structure
typedef struct _BINPROTDYNMSG
{
  U8              nSrcAdr;
  U8              nDstAdr;
  U8              nCommand;
  U8              nOption1;
  U8              nOption2;
  U8              nRcvSequence;
  U16             wRxLength;
  U16             wTxLength;
} BINPROTDYNMSG, *PBINPROTDYNMSG;
#define BINPROTDYNMSG_SIZE           sizeof( BINPROTDYNMSG )

/// define the config structure
typedef struct _BINPROTDYNCFG
{
  BOOL            bMultiDropEnable;
  BOOL            bSequenceEnable;
  PU8             pnTxRxBuffer;
  U16             wBufferSize;
  PVWRITECHARFUNC pvWriteFunc;
  PBINPROTDYNMSG  ptMessage;
  PBINPROTCMDTBL  ptCommandTable;
} BINPROTDYNCFG, *PBINPROTDYNCFG;
#define BINPROTDYNCFG_SIZE             sizeof( BINPROTDYNCFG )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            BinaryProtocolDynamic_Initialize( void );
extern  void            BinaryProtocolDynamic_Configure( PBINPROTDYNCFG ptConfig );
extern  BINPROTDYNSTS   BinaryProtocolDynamic_ProcessChar( U8 nChar );

/**@} EOF BinaryProtocolDynamic.h */

#endif  // _BINARYPROTOCOLDYNAMIC_H