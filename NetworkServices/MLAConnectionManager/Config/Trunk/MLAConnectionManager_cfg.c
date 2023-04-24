/******************************************************************************
 * @file MLAConnectionManager_cfg.c
 *
 * @brief MLA connection manager configuration definitions 
 *
 * This file provides the implementation for the configuration
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
 * \addtogroup MLAConnectionManager_cfg
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MLAConnectionManager/MLAConnectionManager_cfg.h"

// library includes -----------------------------------------------------------

// OS includes
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  #include "FreeRtos.h"
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  #include <zephyr.h>
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  #define MLACONMNGR_TASK_PRIORITY              ( tskIDLE_PRIORITY + 4 )
  #define MLACONMNGR_QUEUE_SIZE                 ( 4 )
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  #define MLACONMNGR_STACK_SIZE                 ( 384 )
  #define MLACONMNGR_QUEUE_ENTRY_SIZE           ( 4 )
  #define MLACONMNGR_QUEUE_MAX_ENTRIES          ( 4 )
  #define MLACONMNGR_QUEUE_ALIGNMENT            ( 4 )
#endif // SYSTEMDEFINE_OS_SELECTION

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  QueueHandle_t g_MLAConMngrQueue;
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  // define the trhead stack
  K_THREAD_STACK_DEFINE( tConnectThreadStack, MLACONMNGR_STACK_SIZE );
  static struct k_thread tConnectThreadData;
  static k_tid_t tConnectThreadId;
  // define the queue
  K_MSGQ_DEFINE(  g_tMLAConMgrEventQueue, MLACONMNGR_QUEUE_ENTRY_SIZE, MLACONMNGR_QUEUE_MAX_ENTRIES, RMLACONMNGR__QUEUE_ALIGNMENT );
#endif // SYSTEMDEFINE_OS_SELECTION

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  static  void  ConnectTask( PVOID pvParameters );
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  static  void  ConnectThread( PVOID pvParam1, PVOID pvParam2, PVOID pvParam3 );
#endif // SYSTEMDEFINE_OS_SELECTION

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function MLAConnectionManager_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any custom initialization
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL MLAConnectionManager_LocalInitialize( void )
{
  BOOL  bStatus = FALSE;
  // create tasks
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    // create the animation task
    bStatus |= xTaskCreate( ConnectTask, "MLAConnect", configMINIMAL_STACK_SIZE, NULL, MLACONMNGR_TASK_PRIORITY, NULL );
    bStatus |= (( g_MLAConMngrQueue = xQueueCreate( SYSCTRL_QUEUE_SIZE, sizeof( 2 ))) == NULL ) ? TRUE : FALSE;
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
    // create the animation task
    tConnectThreadId = k_thread_create( &tConnectThreadData, 
                                        tConnectThreadStack, 
                                        K_THREAD_STACK_SIZEOF( tConnectThreadStack), 
                                        ConnectThread, 
                                        NULL, NULL, NULL, 
                                        MLACONMNGR_TASK_PRIORITY, 0, K_NO_WAIT
                                      );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function MLAConnectionManager_PostEvent
 *
 * @brief post event
 *
 * This function will post the event to the task manager
 *
 * @param[in]   eArg    event to post
 *
 *****************************************************************************/
void MLAConnectionManager_PostEvent( MLACONNECTIONMANAGEREVENT eArg )
{
  // post and event to the task
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    TaskManager( MLACONMNGR_TASK_ENUM, ( TASKARG )eArg );
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    xQueueSendToBack( g_MLAConMngrQueue, ( PVOID )&eArg, portMAX_DELAY );
  #else( SYSTEMDEFIINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
    k_msgq_put( &g_tMLAConMgrEventQueue, &eArg, K_NO_WAIT );
  #endif
}

/******************************************************************************
 * @function MLAConnectionManager_CallConnectEvents
 *
 * @brief call any connect functions
 *
 * This function will notify other processes of a connect event
 *
 *****************************************************************************/
void MLAConnectionManager_CallConnectEvents( void )
{
}

/******************************************************************************
 * @function MLAConnectionManager_CallDisconnectEvents
 *
 * @brief call any disconnect functions
 *
 * This function will notify other processes of a cisconnect event
 *
 *****************************************************************************/
void MLAConnectionManager_CallDisconnectEvents( void )
{
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  /******************************************************************************
   * @function ConnectTask
   *
   * @brief animation task
   *
   * This function is the animation task
   *
   * @param[in]   pvParameters  not used
   *
   *****************************************************************************/
  static void ConnectTask( PVOID pvParameters )
  {
    MLACONNECTIONMANAGEREVENT eArg;
    
    // main loop
    FOREVER
    {
      // wait for an queue post
      xQueueReceive( g_MLAConMngrQueue, ( PVOID )&erg, portMAX_DELAY );
      
      // now call the event handler with the event
      MLAConnectionManager_ProcessEvent( eArg );
    }
  }
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  /******************************************************************************
   * @function MLAConnectionManager_ProcessTask
   *
   * @brief connection task
   *
   * This function will call the connection process
   *
   * @param[in]   xArg      task argument
   *
   * @return      TRUE      flush event
   *
   *****************************************************************************/
  BOOL MLAConnectionManager_ProcessTask( TASKARG xArg )
  {
    // call the animation process
    return( MLAConnectionManager_ProcessEvent(( MLACONNECTIONMANAGEREVENT )xArg ));
  }
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  /******************************************************************************
   * @function ConnectThread
   *
   * @brief connection thread
   *
   * This function is the connection task
   *
   * @param[in]   pvParam1      pointer to parameter #1
   * @param[in]   pvParam2      pointer to parameter #2
   * @param[in]   pvParam3      pointer to parameter #3
   *
   *****************************************************************************/
  static void ConnectThread( PVOID pvParam1, PVOID pvParam2, PVOID pvParam3 )
  {
    MLACONNECTIONMANAGEREVENT eArg;

    // main loop
    FOREVER
    {
      // get event from fifo
      k_msgq_get( &g_tSysCtrlEventQueue, &eArg, K_FOREVER );  

      // now call the event handler with the event
      MLAConnectionManager_ProcessEvent( eArg );
    }
  }

#endif  // SYSTEMDEFINE_OS_SELECTION

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
 
/**@} EOF MLAConnectionManager_cfg.c */
