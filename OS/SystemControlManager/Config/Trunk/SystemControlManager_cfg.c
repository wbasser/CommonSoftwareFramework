/******************************************************************************
 * @file SystemControlManager_cfg.c
 *
 * @brief system control manager configuration implementation
 *
 * This file provides the implementation for the system control manager configuration
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
 * $Rev: $
 *
 * \addtogroup SystemControlManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SystemControlManager/SystemControlManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  /// define the message queue entry size and number of entries
  #define SYSCTRL_TASK_PRIORITY                 ( tskIDLE_PRIORITY + 4 )
  #define SYSCTRL_QUEUE_SIZE                    ( 4 )
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  /// define the message queue entry size and number of entries
  #define SYSCTRL_QUEUE_ENTRY_SIZE              ( STATEEXECENG_ARG_SIZE_BYTES )
  #define SYSCTRL_QUEUE_MAX_ENTRIES             ( 4 )
  #define SYSCTRL_QUEUE_ALIGNMENT               ( 4 )
#endif // SYSTEMDEFINE_OS_SELECTION

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  QueueHandle_t g_xSystemControlManagerQueue;
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  K_MSGQ_DEFINE(  g_tSysCtrlEventQueue, SYSCTRL_QUEUE_ENTRY_SIZE, SYSCTRL_QUEUE_MAX_ENTRIES, SYSCTRL_QUEUE_ALIGNMENT );
#endif // SYSTEMDEFINE_OS_SELECTION

// local parameter declarations -----------------------------------------------
static  BOOL  bForceManufacturing;

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  static  void  SystemControlTask( PVOID pvParameters );
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  static  void  SystemControlTask( PVOID pvArg0, PVOID pvArg1, PVOID pvArg2 );
#endif // SYSTEMDEFINE_OS_SELECTION
static  void  LclInitEntry( void );
static  void  LclConfigEntry( void );
static  void  LclPowRcvEntry( void );
static  void  LclStandbyEntry( void );

// constant parameter initializations -----------------------------------------
/// declare the control manager definitions
const CODE  SYSCTRLMNGRSCHDDEF g_atSysCtrlMngrSchdDefs[ ] = 
{
  // SYSCNTRLMNGRSCHD_DEF( task,                mode0, mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8, mode9, mode10, mode11, mode12, mode13, mode14, mode15, func   ) 
};

#if ( TASK_TICK_ENABLE == 1 )
const CODE  SYSCTRLMNGRTICKDEF g_atSysCtrlMngrTickDefs[ ] =
{
  // SYSCNTRLMNGRTICK_DEF( task,               mode0, mode1, mode2, mode3, mode4, mode5, mode6, mode7, mode8, mode9, mode10, mode11, mode12, mode13, mode14, mode15   )
};
#endif // TASK_TICK_ENABLE

/// declare the functions for each mode
const CODE  SYSCTRLMGRENTCHK g_atSysCtrlMngrEntChkFuncs[ SYSCTRLMNGR_MODE_MAX ] = 
{
  SYSCTRLMNGGR_ENTCHK( LclInitEntry,    NULL            ),   ///< initialization
  SYSCTRLMNGGR_ENTCHK( LclConfigEntry,  NULL            ),   ///< configuration
  SYSCTRLMNGGR_ENTCHK( LclPowRcvEntry,  NULL            ),   ///< power recovery
  SYSCTRLMNGGR_ENTCHK( LclStandbyEntry, NULL            ),   ///< standby
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< idle
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< run
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< power loss
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< error
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< undefined
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< undefined
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< undefined
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< undefined
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< undefined
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< undefined
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< undefined
  SYSCTRLMNGGR_ENTCHK( NULL,            NULL            ),   ///< undefined
};

/******************************************************************************
 * @function SystemControlManager_LocalInitialize
 *
 * @brief initialize the system control manager
 *
 * This function will perform the initialization of the system control manager
 * and enter the initialization state
 *
 * @return  TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL SystemControlManager_LocalInitialize( void )
{
  BOOL  bStatus = FALSE;
  
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    // create the task and queue for the system control manager
    bStatus |= (( xTaskCreate( SystemControlTask, "SystemControl", configMINIMAL_STACK_SIZE, NULL, SYSCTRL_TASK_PRIORITY, NULL )) == NULL ) ? TRUE : FALSE;
    bStatus |= (( g_xSystemControlManagerQueue = xQueueCreate( SYSCTRL_QUEUE_SIZE, sizeof( STATEEXECENGARG ))) == NULL ) ? TRUE : FALSE;
  #endif // SYSTEMDEFINE_OS_SELECTION

  // clear the force manufacturing
  bForceManufacturing = FALSE;
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function SystemControlManage_GetNumberSchdDefs
 *
 * @brief get then number of entries
 *
 * This function will return the number of entries in the def table
 *
 *****************************************************************************/
U8 SystemControlManager_GetNumberSchdDefs( void )
{
  // return the number of entrys
  return( sizeof( g_atSysCtrlMngrSchdDefs ) / SYSCTRLMNGRSCHDDEF_SIZE );
}

#if ( TASK_TICK_ENABLE == 1 )
  /******************************************************************************
  * @function SystemControlManage_GetNumberTickDefs
  *
  * @brief get then number of entries
  *
  * This function will return the number of entries in the def table
  *
  *****************************************************************************/
  U8 SystemControlManager_GetNumberTickDefs( void )
  {
    // return the number of entrys
    return( sizeof( g_atSysCtrlMngrTickDefs ) / SYSCTRLMNGRTICKDEF_SIZE );
  }
#endif // TASK_TICK_ENABLE

/******************************************************************************
 * @function LclInitEntry
 *
 * @brief 
 *
 * This function 
 *
 *****************************************************************************/
static void LclInitEntry( void )
{
  STATEEXECENGARG xStateArg;
  
  // set the event
  xStateArg = SYSCTRLMNGR_EVENT_INITDONE;
  
  // mark local initialization done
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    xQueueSendToBack( g_xSystemControlManagerQueue, ( PVOID )&xStateArg, portMAX_DELAY );
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
    // post the event
    k_msgq_put( &g_tSysCtrlEventQueue, &xStateArg, K_NO_WAIT );
  #else
    TaskManager_PostEvent( SYSTEMCONTROLMANAGER_TASK_ENUM, xStateArg );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function LclConfigEntry
 *
 * @brief LclConfigEntry
 *
 * This function 
 *
 *****************************************************************************/
static void LclConfigEntry( void )
{
  STATEEXECENGARG xStateArg;
 
  // set the event
  xStateArg = SYSCTRLMNGR_EVENT_CONFIGDONE;

  // post a config done
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    xQueueSendToBack( g_xSystemControlManagerQueue, ( PVOID )&xStateArg, portMAX_DELAY );
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
    // post the event
    k_msgq_put( &g_tSysCtrlEventQueue, &xStateArg, K_NO_WAIT );
  #else
    TaskManager_PostEvent( SYSTEMCONTROLMANAGER_TASK_ENUM, xStateArg );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function LclPowRcvEntry
 *
 * @brief 
 *
 * This function 
 *
 *****************************************************************************/
static void LclPowRcvEntry( void )
{
  STATEEXECENGARG xStateArg;

   // set the event
  xStateArg = ( bForceManufacturing ) ? SYSCTRLMNGR_EVENT_GOMANUFACTURING : SYSCTRLMNGR_EVENT_GOIDLE;
  
  // move to idle
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    xQueueSendToBack( xSystemControlManagerQueue, ( PVOID )&xStateArg, portMAX_DELAY );
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
    // post the event
    k_msgq_put( &g_tSysCtrlEventQueue, &xStateArg, K_NO_WAIT );
  #else
    TaskManager_PostEvent( SYSTEMCONTROLMANAGER_TASK_ENUM, xStateArg );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function LclStandbyEntry
 *
 * @brief 
 *
 * This function 
 *
 *****************************************************************************/
static void LclStandbyEntry( void )
{
}

/******************************************************************************
 * @function LclSpcExit
 *
 * @brief exit special modes
 *
 * This function will test for a valid mode and return true if ok
 *
 * @param[in]     eNewMode      newmode
 *
 * @return        TRUE if valid mode, FALSE if not
 *
 *****************************************************************************/
static BOOL LclSpcExit( SYSCTRLMGRMODE eNewMode )
{
  BOOL bStatus = FALSE;

  // test for valid mode
  if ( eNewMode == SYSCTRLMNGR_MODE_04_IDLE )
  {
    // set status OK
    bStatus = TRUE;
  }

  // return the status
  return( bStatus );
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  /******************************************************************************
  * @function SystemControlTask
  *
  * @brief system control task
  *
  * This function is the system control task
  *
  * @param[in]   pvParameters  not used
  *
  *****************************************************************************/
  static void SystemControlTask( PVOID pvParameters )
  {
    STATEEXECENGARG xStateArg;
    
    // main loop
    FOREVER
    {
      // wait for an queue post
      xQueueReceive( g_xSystemControlManagerQueue, ( PVOID )&xStateArg, portMAX_DELAY );
      
      // now call the event handler with the event
      SystemControlManager_ProcessEvent( xStateArg );
    }
  }
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  /******************************************************************************
  * @function SystemControlTask
  *
  * @brief system control task
  *
  * This function is the system control task
  *
  * @param[in]   pvArg0      argument 0
  * @param[in]   pvArg1      argument 1
  * @param[in]   pvArg2      argument 2
  *
  *****************************************************************************/
  static void SystemControlTask( PVOID pvArg0, PVOID pvArg1, PVOID pvArg2 )
  {
    STATEEXECENGARG xStateArg;
    
    // main loop
    FOREVER
    {
      // get event from fifo
      k_msgq_get( &g_tSysCtrlEventQueue, &xStateArg, K_FOREVER );  

      // now call the event handler with the event
      SystemControlManager_ProcessEvent( xStateArg );
    }
  }
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  /******************************************************************************
   * @function SystemControlManager_ProcessEvent
   *
   * @brief event handler
   *
   * This function will process the incoming event and change states if permitted
   *
   * @param[in]   xArg    event argument to proces
   *
   * @return TRUE to flush event
   *
   *****************************************************************************/
  BOOL SystemControlManager_ProcessTaskCb( TASKARG xArg )
  {
    // process the event
    SystemControlManager_ProcessEvent(( STATEEXECENGARG )xArg );

    // return true
    return( TRUE );
  }
#endif  // SYSTEMDEFINE_OS_SELECTION

/**@} EOF SystemControlManager_cfg.c */
