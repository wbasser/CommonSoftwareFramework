/******************************************************************************
 * @file NetworkLocator_def.h
 *
 * @brief Network Locator definitions
 *
 * This file provides the definitions for the Network Locator service
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
 * \addtogroup NetworkLocator
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _NETWORKLOCATOR_DEF_H
#define _NETWORKLOCATOR_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the name/key max lengths
#define NETWORKLOCATOR_NAME_LEN                 ( 32 )
#define NETWORKLOCATOR_KEY_LEN                  ( 32 )
#define NETWORKLOCATOR_APP_LEN                  ( 32 )

/// define the locator command/response
#define NETWORKLOCATOR_COMMAND                  ( 0xC53CA55A )
#define NETWORKLOCATOR_RESPONSE                 ( 0x5CC35AA5 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the network locator configuration structure
typedef struct PACKED _NETWORKLOCATORCFG
{
  C8  acName[ NETWORKLOCATOR_NAME_LEN ];
  C8  acKey[ NETWORKLOCATOR_KEY_LEN ];
  U16 wPort;
} NETWORKLOCATORCFG, *PNETWORKLOCATORCFG;
#define NETWORKLOCATORCFG_SIZE                  sizeof( NETWORKLOCATORCFG )

/// define the receive packet structre
typedef struct PACKED _NETWORKLOCATORRCVPKT
{
  U32 uCommand;
  U32 uKey;
} NETWORKLOCATORRCVPKT, *PNETWORKLOCATORRCVPKT;
#define NETWORKLOCATORRCVPKT_SIZE               sizeof( NETWORKLOCATORRCVPKT )

/// define the transmit packet structure
typedef struct PACKED _NETWORKLOCATORXMTPKT
{
  U32 uRepsonse;
  U32 uIpAddr;
  U8  anMacAddr[ MAC_ADDR_SIZE ];
  U8  nHdwMajor;
  U8  nHdwMinor;
  U8  nSfwMajor;
  U8  nSfwMinor;
  U8  nSfwTest;
  C8  acName[ NETWORKLOCATOR_NAME_LEN ];
  C8  acAppl[ NETWORKLOCATOR_APP_LEN ];
} NETWORKLOCATORXMTPKT, *PNETWORKLOCATORXMTPKT;
#define NETWORKLOCATORXMTPKT_SIZE               sizeof( NETWORKLOCATORXMTPKT )

/**@} EOF NetworkLocator_def.h */

#endif  // _NETWORKLOCATOR_DEF_H