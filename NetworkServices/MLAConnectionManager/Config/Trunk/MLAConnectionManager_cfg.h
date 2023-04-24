/******************************************************************************
 * @file MLAConnectionManager_cfg.h
 *
 * @brief MLA connection manager configuration definitions 
 *
 * This file provides the definitions for the configuration manager
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
#ifndef _MLACONNECTIONMANAGER_CFG_H
#define _MLACONNECTIONMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "TaskManager/TaskManager.h"
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #define MLACONMNGR_NUM_EVENTS                 ( 4 )
  #define MLACONMNGR_TASK_ENUM                  ( TASK_SCHD_ENUM_ILLEGAL )
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  MLAConnectionManager_LocalInitialize( void );
extern  void  MLAConnectionManager_PostEvent( MLACONNECTIONMANAGEREVENT eArg );
extern  void  MLAConnectionManager_CallConnectEvents( void );
extern  void  MLAConnectionManager_CallDisconnectEvents( void );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  extern  BOOL  MLAConnectionManager_ProcessTask( TASKARG xArg );
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/**@} EOF MLAConnectionManager_cfg.h */

#endif  // _MLACONNECTIONMANAGER_CFG_H