/******************************************************************************
 * @file AscCommonCommands.c
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup AscCommonCommands
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AscCommonCommands/AscCommonCommands.h"

// library includes -----------------------------------------------------------
#include "ConfigManager/ConfigManager.h"
#include "DebugManager/DebugManager.h"
#include "EepromHandler/EepromHandler.h"
#include "HardwareDebug/HardwareDebug.h"
#include "LedManager/LedManager.h"
#include "LogHandler/LogHandler.h"
#include "ManufInfo/ManufInfo.h"
#include "Main/Main.h"
#include "RTCManager/RTCManager.h"
#include "SystemControlManager/SystemControlManager.h"
#include "SystemMonitor/SystemMonitor.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

/// command handlers

// constant parameter initializations -----------------------------------------
/// declare the command strings

/// initialize the command table
const CODE ASCCMDENTRY g_atAscCommonCommandsTable[ ] =
{
  // add the external tables here
  #if ( MANUFINFO_ENABLE_DEBUGCOMMANDS == ON )
    ASCCMD_EXTTBL( g_atManufInfoCmdHandlerTable ),
  #endif // MANUFINFO_ENABLE_DEBUGCOMMANDS

  #if ( EEPROMHANDLER_ENABLE_DEBUGCOMMANDS == ON )
    ASCCMD_EXTTBL( g_atEepromDbgHandlerTable ),
  #endif // EEPROMHANDLER_ENABLE_DEBUGCOMMANDS

  #if ( SYSTEMCONTROLMANAGER_ENABLE_DEBUGCOMMANDS == ON )
    ASCCMD_EXTTBL( g_atSysCtrlMngrCmdHandlerTable ),
  #endif // SYSTEMCONTROLMANAGER_ENABLE_DEBUGOMMANDS

  #if ( DEBUGMANAGER_ENABLE_DEBUG_COMMANDS == ON )
    ASCCMD_EXTTBL( g_atDebugManagerCmdHandlerTable ),
  #endif  // DEBUGMANAGER_ENABLE_DEBUG_COMMANDS

  #if ( LEDMANAGER_ENABLE_DEBUG_COMMANDS == ON )
    ASCCMD_EXTTBL( g_atLedManagerCmdHandlerTable ),
  #endif

  #if ( LOGHANDLER_ENABLE_DBGASCCOMMANDS == ON )
    ASCCMD_EXTTBL( g_atLogHandlerAscCmdHandlerTable ),
  #endif // LOGHANDLER_ENABLE_DEBUGCOMMANDS
  
  #if( RTCMANAGER_ENABLE_DEBUG_COMMANDS == ON )
   ASCCMD_EXTTBL( g_atRtcManagerAscCmdHandlerTable ),
  #endif // RTCMANAGER_ENABLE_DEBUG_COMMANDS
  
  #if ( MICRO_ENABLE_SYSTIMECMDS == ON )
   ASCCMD_EXTTBL( g_atMainCmdHandlerTable ),
  #endif // MICRO_ENABLE_SYSTIMECMDS
  
  #if ( PARAM_ENABLE_DEBUG_COMMANDS == ON )
   ASCCMD_EXTTBL( g_atParamManagerCmdHandlerTable ),
  #endif // PARAM_ENABLE_DEBUG_COMMANDS
  
  #if ( CONFIGMANAGER_ENABLE_DEBUG_COMMANDS == ON )
    ASCCMD_EXTTBL( g_atConfigManagerCmdHandlerTable ),
  #endif // CONFIGMANAGER_ENABLE_DEBUG_COMMANDS

  #if ( REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS == ON )
    ASCCMD_EXTTBL( g_atRedirectionManagerDbgCmdTable ),
  #endif // REDIRECTIONMANAGER_ENABLE_DEBUG_COMMANDS

  #if ( SYSTEMMONITOR_ENABLE_DBGASCCOMMANDS == ON )
    ASCCMD_EXTTBL( g_atSystemMonitorAscCmdHandlerTable ),
  #endif // SYSTEMMONITOR_ENABLE_DBGASCCOMMANDS

  // include the hardware debug
  ASCCMD_EXTTBL( g_atHardwareDebugCommandsTable ),

  // the entry below must be here
  ASCCMD_ENDTBL( )
};

/**@} EOF AscCommonCommands.c */