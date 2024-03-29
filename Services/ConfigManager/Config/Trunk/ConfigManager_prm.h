/******************************************************************************
 * @file ConfigManager_prm.h
 *
 * @brief Config Manager parameter declarations
 *
 * This file declares any customization for the configr manager
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
 * \addtogroup ConfigManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CONFIGMANAGER_PRM_H
#define _CONFIGMANAGER_PRM_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "EepromHandler/EepromHandler.h"

// Macros and Defines ---------------------------------------------------------
/// define the log event enable macro
#define CONFIGMANAGER_ENABLE_LOGEVENTS          ( ON )

/// define the macro to enable notifications
#define	CONFIGMANAGER_ENABLE_NOTIFICATIONS      ( OFF )

/// define the macro to enable callbacks
#define	CONFIGMANAGER_ENABLE_CALLBACKS          ( OFF )

/// define the callback or the task
#if ( CONFIGMANAGER_ENABLE_NOTIFICATIONS == 1 )
  #if ( CONFIGMANAGER_ENABLE_CALLBACKS == 1 )
    #define	CONFIGMANAGER_DONE_CALLBACK( )  		( NULL )
  #else
    #define	CONFIGMANAGER_NOTIFICATION_TASK	( 0 )
    #define	CONFIGMANAGER_DONE_EVENT        ( 0 )
    #define CONFIGMANAGER_ERROR_EVENT           ( 0 )
  #endif	// CONFIGMANAGER_ENABLE_CALLBACKS
#endif	// CONFIGMANAGER_ENABLE_NOTIFICATIONS

/// define the macro to enable config version reset
#define CONFIGMGR_ENABLE_CONFIGFVER             ( ON )

#if ( CONFIGMGR_ENABLE_CONFIGVER == ON )
  /// define the configuration version
  #define CONFIGMGR_CONFIG_VERMAJ               ( 0 )
  #define CONFIGMGR_CONFIG_VERMIN               ( 1 )
#endif

/// define the address of the CRC
#define CONFIGMNGR_CHCK_ADDR                    ( EEPROMHANDLER_CFGBLOCK_BASE_ADDR )

/// define the address of the version block
#define CONFIGMNGR_VERS_ADDR                    ( CONFIGMNGR_CHCK_ADDR + sizeof( U16 ))  

/// define the address of the config block
#define CONFIGMNGR_DATA_ADDR                    ( CONFIGMNGR_VERS_ADDR + sizeof( U16 ))

/**@} EOF ConfigManager_prm.h */

#endif  // _CONFIGMANAGER_PRM_H