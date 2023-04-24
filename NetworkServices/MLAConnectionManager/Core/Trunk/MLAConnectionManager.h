/******************************************************************************
 * @file MLAConnectionManager
 *
 * @brief network command hnadler
 *
 * This file providces the declarations for the netowrk command handler
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
 * \addtogroup MLAConnectionManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MLACONNECTIONMANAGER_H
#define _MLACONNECTIONMANAGER_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "MLAConnectionManager/MLAConnectionManager_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the events
typedef enum _MLACONNECTIONMANAGEREVENT
{
  MLACONNECTIONMANAGER_EVENT_DISCONNECT = 0x5335,
  MLACONNECTIONMANAGER_EVENT_CONNECT,
} MLACONNECTIONMANAGEREVENT;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  MLAConnectionManager_Initialize( void );
extern  void  MLAConnectionManager_Control( BOOL bState );
extern  BOOL  MLAConnectionManager_ProcessEvent( MLACONNECTIONMANAGEREVENT eArg );
extern  BOOL  MLAConnectionManager_GetConnectedStatus( void );

/**@} EOF MLAConnectionManager.h */

#endif  // _MLACONNECTIONMANAGER_H