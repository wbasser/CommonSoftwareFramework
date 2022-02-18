/******************************************************************************
 * @file SystemMonitor.h
 *
 * @brief System Monitor declarations
 *
 * This file provides the declarations for the system monitor
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
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup SystemMonitor
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _SYSTEMMONITOR_H
#define _SYSTEMMONITOR_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemMonitor/SystemMonitor_cfg.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMMONITOR_ENABLE_DBGASCCOMMANDS == ON )
  #include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif // SYSTEMMONITOR_ENABLE_DBGASCCOMMANDS

#if ( SYSTEMMONITOR_ENABLE_DBGBINCOMMANDS == ON )
  #include "BinaryCommandHandler/BinaryCommandHandler.h"
#endif // SYSTEMMONITOR_ENABLE_DBGBINCOMMANDS

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error
typedef enum _SYSMONERRS
{
  SYSMON_ERRS_NONE = 0,         ///< no errors
  SYSMON_ERRS_SENDRESP,         ///< send response
  SYSMON_ERRS_ILLCMD = 0xF0,    ///< illegal command
  SYSMON_ERRS_ILLENUM,          ///< illegal enuemration
  SYSMON_ERRS_ILLLENGTH,        ///< illegal length
  SYSMON_ERRS_ILLSETUP,         ///< illegal setup
  SYSMON_ERRS_FRAMEOVF,         ///< frame overflow
  SYSMON_ERRS_ILLVALUE,         ///< illegal value on put
  SYSMON_ERRS_ILLTIME,          ///< illegal time
} SYSMONERRS;

/// enumerate the commands
typedef enum _SYSMONCMDS
{
  SYSMON_CMDS_SETUP = 1,        ///< setup the system monitor
  SYSMON_CMDS_START,            ///< start the transmission
  SYSMON_CMDS_STOP,             ///< stop the transmission
  SYSMON_CMDS_GET,              ///< get a value
  SYSMON_CMDS_PUT,              ///< put a value
  SYSMON_CMDS_RESPONSE = 0x20,  ///< auto response
} SYSMONCMDS;

// global parameter declarations -----------------------------------------------
#if ( SYSTEMMONITOR_ENABLE_DBGASCCOMMANDS == ON )
  extern  const CODE ASCCMDENTRY g_atSystemMonitorAscCmdHandlerTable[ ];
#endif // SYSTEMMONITOR_ENABLE_DBGASCCOMMANDS
#if ( SYSTEMMONITOR_ENABLE_DBGBINCOMMANDS == ON )
  extern  const CODE BINCMDSLVENTRY g_atSystemMonitorBinCmdHandlerTable[ ];
#endif // SYSTEMMONITOR_ENABLE_DBGBINCOMMANDS

// global function prototypes --------------------------------------------------
extern  BOOL        SystemMonitor_Initialize( void );
extern  void        SystemMonitor_ProcessTransmit( void );

/**@} EOF SystemMonitor.h */

#endif  // _SYSTEMMONITOR_H