/******************************************************************************
 * @file Main_cfg.c
 *
 * @brief Main configuration structures
 *
 * This file conatins the initilization structures
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
 *
 * \addtogroup Main
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "MainConfig/Main_cfg.h"

// library includes -------------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "TaskManager/TaskManager.h"
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
  #include "TaskScheduler/TaskScheduler.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// include HAL files

// include driver files

// include OS files

// include protocol file

// include Services files

// include application files

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL ) && ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_TASKSCHEDULER )
  /// initialization function - interrupts disabled
  const CODE PVMAININITFUNC g_apvInitIrqDsbFunctions[ ] = 
  {
    /// base HAL initializations here

    /// add HAL initializlations here

    /// add OS initializations here
    
    /// add driver initializations here
    
    /// add protocol initialization here

    /// add services initializations here

    /// add application initializations here
    
    // do not remove this entry
    NULL
  };

  /// initialization function - interrupts enabled
  const CODE PVMAININITFUNC g_apvInitIrqEnbFunctions[ ] =
  {
    // add initialization funtions that required interrupts to be enable eher

    // do not remove this entry
    NULL
  };

  /// idle functions
  const CODE PVMAINIDLEFUNC g_apvIdleFunctions[ ] = 
  {
    // add idle functions here
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
      TaskManager_IdleProcess,
    #endif // SYSTEMDEFINE_OS_SELECTION

    // do not remove this entry
    NULL
  };

  /// shutdown functions
  const CODE PVMAINSHUTDNFUNC  g_apvShutdownFunctions[ ] =
  {
    // add shutdown functions here
  
    // do not remove this entry
    NULL
  };

  /******************************************************************************
  * @function MainCheckTasksPendingf
  *
  * @brief tests for tasks pending
  *
  * This function checks to see if tasks are pending
  *
  * @return  TRUE if tasks are pending
  *
  *****************************************************************************/
  BOOL Main_CheckTasksPending( void )
  {
    BOOL bResult = TRUE;
    
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    bResult = TaskManager_CheckTasksPending( TASK_SCHD_MAX );
    #endif // SYSTEMDEFINE_OS_SELECTION

    // return the result
    return( bResult );
  }

  /******************************************************************************
  * @function Main_EnterSleepMode
  *
  * @brief go to sleep
  *
  * This function will call he sleep manager in the HAL and put the Main processor
  * to sleep if low power mode is desired
  *
  *****************************************************************************/
  void Main_EnterSleepMode( void )
  {
  }
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL

/******************************************************************************
 * @function Main_LocalInitialize
 *
 * @brief local initializtion
 *
 * This function will perform any local initialization
 *
 *****************************************************************************/
void  Main_LocalInitialize( void )
{
}  

/******************************************************************************
 * @function Main_LocalIrqInitialize
 *
 * @brief local initializtion
 *
 * This function will perform any local initialization
 *
 *****************************************************************************/
void  Main_LocalIrqInitialize( void )
{
}  

/******************************************************************************
 * @function Main_LocalIdle
 *
 * @brief local initializtion
 *
 * This function will perform any local initialization
 *
 *****************************************************************************/
void Main_LocalIdle(void)
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
    TaskScheduler_IdleProcess( );
  #endif // SYSTEMDEFFIN_OS_SELECTON
}

/******************************************************************************
 * @function Main_DisplayInitFault
 *
 * @brief display initialization fault
 *
 * This function will display the initializaiton fault
 *
 * @param[in]   uError    error to display
 *
 *****************************************************************************/
void Main_DisplayInitFault( U32 uError )
{
  // call the fault handler
  FaultHandler_Display( uError );

  // now stay here forever
  FOREVER{ };
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  void vApplicationStackOverflowHook(xTaskHandle *pxTask,signed char *pcTaskName)
  {
    while(1);
  }
  
  void vApplicationIdleHook(void)
  {
    while(1);
  }
  
  void vApplicationTickHook(void)
  {
    while(1);
  }
  
  void vApplicationMallocFailedHook(void)
  {
    while(1);
  }
  
#endif

/**@} EOF Main_cfg.c */
