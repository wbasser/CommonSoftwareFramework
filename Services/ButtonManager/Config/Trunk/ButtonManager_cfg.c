/******************************************************************************
 * @file ButtonManager_cfg.c
 *
 * @brief button manager configuration implementation
 *
 * This file provides the configuration for the button manager
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
 * \addtogroup ButtonManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ButtonManager/ButtonManager.h"

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
  #include "TaskScheduler/TaskScheduler.h"
  #include "GPIO/Gpio.h"
#endif // SYSTEMDEFINE_OS_SELECTION
#include "LocalEventHandler/LocalEventHandler.h"

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  #define PROCESS_TASK_PRIORITY                   ( tskIDLE_PRIORITY + 4 )
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
  #define PROCESS_TASK_PRIORITY                   ( 4 )
  #define PROCESS_TASK_NUMNRMEVENTS               ( 2 )
  #define PROCESS_TASK_NUMPRIEVENTS               ( 0 )
  #define PROCESS_TASK_EXECUTION_RATE             ( TASKSCHEDULER_TIME_MSEC( BTNMANAGER_PROCESS_RATE_MSECS ))
#endif // SYSTEMDEFINE_OS_SELECTION

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
  static  PTASKSCHEDULERHANDLE  ptProcessTask;
  static  GPIOENUM              ePwrBtn;
#endif // SYSTEMDEFINE_OS_SELECTION 

// local function prototypes --------------------------------------------------
static  BOOL  GetKeyStatus( U32 uKeyEnum );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  static  void  ProcessTask( PVOID pvParameters );
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
  static  BOOL  ProcessTask( TASKSCHEDULEREVENT xEvent );
#endif // SYSTEMDEFINE_OS_SELECTION

// constant parameter initializations -----------------------------------------
/// fill out the defaults for the config
const CODE BTNMNGRCFG g_tBtnMgrCfg = 
{
  // BTNMNGR_CFG_ENTRY( debnc, repdly, reprate, sh_time, mh_time, lh_time, stuck_time )
  BTNMNGR_CFG_ENTRY( 50, 500, 100, 1000, 2000, 3000, 7000 )
};

/// fill out the button defs
const CODE BTNMNGRDEF g_atBtnMgrDefs[ BTNMNGR_ENUM_MAX ]  =
{
  // BTNMNGR_DEFCB_ENTRY( keyenum,             rel_enb, prs_enb, rep_enb, shh_enb, mdh_enb, lng_enb, tgl_enb, getstatus, callback )
  // BTNMNGR_DEFEVENT_ENTRY( keyenum,          rel_enb, prs_enb, rep_enb, shh_enb, mdh_enb, lng_enb, tgl_enb, getstatus, task )
};

/******************************************************************************
 * @function ButtonManager_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any custom initialization
 *
 *****************************************************************************/
void ButtonManager_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    // create the animation task
    xTaskCreate( ProcessTask, "ButtonProcess", configMINIMAL_STACK_SIZE, NULL, BTNMANAGER_PROCESS_TASK_PRIORITY, NULL );
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
    ptProcessTask = TaskScheduler_Create( TASKSCHEDULER_TYPE_TIMED_CONTINUOUS, 
                                          ProcessTask, 
                                          PROCESS_TASK_NUMNRMEVENTS, 
                                          PROCESS_TASK_NUMPRIEVENTS, 
                                          PROCESS_TASK_EXECUTION_RATE, 
                                          PROCESS_TASK_PRIORITY, 
                                          TRUE, 
                                          FALSE 
                                        );

    // create the buttons
    ePwrBtn = Gpio_Configure( SYSTEM_CONFIG_PWRBTN_PORT, SYSTEM_CONFIG_PWRBTN_PIN, GPIO_MODE_INPUT_PULLUP, ON, GPIO_FUNCMUX_A, OFF );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  /******************************************************************************
   * @function ProcessTask
   *
   * @brief process task
   *
   * This function is the process task
   *
   * @param[in]   pvParameters  not used
   *
   *****************************************************************************/
  static void ProcessTask( PVOID pvParameters )
  {
    // main loop
    FOREVER
    {
      // call the default handler
      ButtonManager_Process( );

      // sleep for the animate rate
      vTaskDelay( LEDMANAGER_ANIMATE_RATE_MSECS / portTICK_RATE_MS );
    }
  }
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKAMANAGER )
  /******************************************************************************
   * @function BUttonManager_ProcessTask
   *
   * @brief process task
   *
   * This function will call the animation process
   *
   * @param[in]   xArg      task argument
   *
   * @return      TRUE      flush event
   *
   *****************************************************************************/
  BOOL ButtonManager_ProcessTask( TASKARG xArg )
  {
    // call the animation process
    ButtonManager_Process( );
  
    // return true
    return( TRUE );
  }
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
  /******************************************************************************
   * @function BUttonManager_ProcessTask
   *
   * @brief process task
   *
   * This function will call the animation process
   *
   * @param[in]   xArg      task argument
   *
   * @return      TRUE      flush event
   *
   *****************************************************************************/
  static BOOL ProcessTask( TASKSCHEDULEREVENT xEvent )
  {
    // call the animation process
    ButtonManager_Process( );
  
    // return true
    return( TRUE );
  }
#endif  // SYSTEMDEFINE_OS_SELECTION

/******************************************************************************
 * @function GetKeyStatus
 *
 * @brief button manager get key status
 *
 * This function calls the appropriate user defined function to return the
 * status of a given key enumeration
 *
 * @param[in]   uKeyEnum      key enumeration
 *  
 * @return      returns TRUE for key pressed, FALSE otherwise
 *
 *****************************************************************************/
static BOOL GetKeyStatus( U32 uKeyEnum )
{
  BOOL bButtonStatus = FALSE;
  
  // get the status of the pin 
  // return the button status
  return( bButtonStatus );
}

/**@} EOF ButtonManager_cfg.c */
