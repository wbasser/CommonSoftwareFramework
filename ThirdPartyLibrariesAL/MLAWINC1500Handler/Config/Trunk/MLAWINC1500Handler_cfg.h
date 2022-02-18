/******************************************************************************
 * @file MLAWINC1500Handler_cfg.h
 *
 * @brief MLA WINC1500 handler configuration declarations 
 *
 * This file provides the declarations for the MLAWINC1500 handler
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
 * \addtogroup MLAWINC1500Handler_cfg
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MLAWINC1500HANDLER_CFG_H
#define _MLAWINC1500HANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "ConfigManager/ConfigManager.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
// define the number of events
#define MLAWINC1500HANDLER_NUM_EVENTS           ( 2 )
#define MLAWINC1500HANDLER_EXEC_MSECS           ( 5 )
#define MLAWINC1500HANDLER_EXEC_RATE            ( TASK_TIME_MSECS( MLAWINC1500HANDLER_EXEC_MSECS ))

/// define the enable debug entries macro
#define MLAWWINC1500HANDLER_ENABLE_DEBUG        ( OFF )

/// define the macro for enabling debug commands
#define MLAWINC1500HANDLER_ENABLE_DBGCMDS       ( ON )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the configuration structure
typedef struct _MLAWINC1500WIFICONFIG
{
  C8  acSSID[ WLAN_SSID_MAX_LEN ];
  C8  acPassword[ WLAN_PASS_MAX_LEN ];
} MLAWINC1500WIFICONFIG, *PMLAWINC1500WIFICONFIG;
#define MLAWINC1500WIFICONFIG_SIZE                     sizeof( MLAWINC1500WIFICONFIG )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            MLAWINC1500Handler_LocalInitialize( void );
extern  BOOL            MLAWINC1500Handler_ProcessTask( TASKARG xArg );
extern  PVGETPOINTER    MLAWINC1500Handler_GetDefaultConfig( void );
extern  U16             MLAWINC1500Handler_GetConfigSize( void );

/**@} EOF MLAWINC1500Handler_cfg.h */

#endif  // _MLAWINC1500HANDLER_CFG_H