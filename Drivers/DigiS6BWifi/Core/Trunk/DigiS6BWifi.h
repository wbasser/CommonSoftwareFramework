/******************************************************************************
 * @file DigiS6BWifi.h  
 *
 * @brief DIGI S6F Wifi module
 *
 * This file provides the declarations for the DIGI S6B WIFI module
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
 * \addtogroup DigiS6BWifi
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DIGIS6BWIFI_H
#define _DIGIS6BWIFI_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DigiNetAPIProtocolHandler/DigiNetAPIProtocolHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _DIGIS6BWIFIERR
{
  DIGIS6BWIFI_ERR_NONE = 0,
  DIGIS6BWIFI_ERR_ILLHANDLE,
  DIGIW6BWIFI_ERR_APNOTFOUND,
} DIGIS6BWIFIERR;

// structures -----------------------------------------------------------------
/// define the handle 
typedef PVDIGIWIFIHANDLE  PVOID;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void              DigiS6BWifi_Initialize( void );
extern  BOOL              DigiS6BWifi_ProcessChar( U8 nChar );
extern  void              DigiS6BWifi_Connect( PC8 pcSSID, PC8 pcPassword );
extern  BOOL              DigiS6BWifi_GetConnectionState( void );
extern  PVDIGIWIFIHANDLE  DigiS6BWifi_OpenSocket( IPADDR ptIpAddr, U16 wPort, DIGIPROTOCOLTYPE eProtoType );
extern  DIGIS6BWIFIERR    DigiS6BWifi_WriteSocket( PVDIGIWIFIHANDLE pvHandler, PU8 pnData, U16 wLength );
extern  DIGIS6BWIFIERR    DigiS6BWifi_ReadSocket( PVDIGIWIFIHANDLE pvHandler, PU8 pnData, U16 wLength );
extern  DIGIS6BWIFIERR    DigiS6BWifi_CloseSocket( PVDIGIWIFIHANDLE pvHandler );

/**@} EOF DigiS6BWifi.h */

#endif  // _DIGIS6BWIFI_H