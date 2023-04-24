/******************************************************************************
 * @file LogHandler_cfg.h
 *
 * @brief log handler configuration declarations
 *
 * This file provides the declarations for the log handler configuration
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 *
 * \addtogroup LogHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LOGHANDLER_CFG_H
#define _LOGHANDLER_CFG_H

// local includes -------------------------------------------------------------
#include "LogHandler/LogHandler_def.h"

// library includes -------------------------------------------------------------
#include "EepromHandler/EepromHandler.h"
#include "SystemControlManager/SystemControlManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the description length
#define LOGHANDLER_MAX_DESCR_LEN                ( 20 )

/// define the debug command enable macro
#define LOGHANDLER_ENABLE_DBGASCCOMMANDS        ( OFF )
#define LOGHANDLER_ENABLE_DBGBINCOMMANDS        ( OFF )

/// define the mode for debug commands
#define LOGHANDLER_ENABLE_SYSTEMMODE            ( SYSTEMCONTROLMANAGE_LCLMODE_DIAG )

/// define the EEPROM base address
#define LOGHANDLER_EEP_BASE_ADDR                ( EEPROMHANDLER_LOGBLOCK_BASE_ADDR )

// enumerations ---------------------------------------------------------------
/// enumerate the log entry types
typedef enum _LOGTYPE
{
  LOG_TYPE_INIT = 1,
  LOG_TYPE_RESET,
  LOG_TYPE_CFGRST,
  /// user will enter log entries after init and before max

  /// do not remove below
  LOG_TYPE_MAX,
} LOGTYPE, *PLOGTYPE;

// global parameters ------------------------------------------------------
extern  const CODE  LOGENTRY g_atLogEntries[ ];

// global function prototypes -------------------------------------------
extern  U32   LogHandler_GetSystemTime( void );
extern  BOOL  LogHandler_RdWord( U16 wAddress, PU16 pwValue );
extern  BOOL  LogHandler_WrWord( U16 wAddress, U16 wValue );
extern  BOOL  LogHandler_RdBlock( U16 wAddress, PU8 pnData, U16 wLength );
extern  BOOL  LogHandler_WrBlock( U16 wAddress, PU8 pnData, U16 wLength );

/**@} EOF LogHandler_cfg.h */

#endif  // _LOGHANDLER_CFG_H