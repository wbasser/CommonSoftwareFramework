/******************************************************************************
 * @file Micro_cfg.c
 *
 * @brief Micro configuration structures
 *
 * This file conatins the initilization structures
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
 * \addtogroup Micro
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "MicroConfig/Micro_cfg.h"

// library includes -------------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "TaskManager/TaskManager.h"
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
  #include "TaskScheduler/TaskScheduler.h"
#endif // SYSTEMDEFINE_OS_SELECTION

// include HAL files
#include "Clock/Clock.h"
#include "GPIO/Gpio.h"
#include "I2C/I2c.h"
#include "Interrupt/Interrupt.h"
#include "PowerManager/PowerManager.h"
#include "SPI/Spi.h"
#include "SystemTick/SystemTick.h"
#include "USB/Usb.h"

// include driver files
#include "EepromHandler/EepromHandler.h"
#include "IoePCA9555/IoePCA9555.h"
#include "UsbGenHandler/UsbGenHandler.h"

// include OS files
#include "SystemControlManager/SystemControlManager.h"
#include "TaskManager/TaskManager.h"

// include protocol file

// include Services files
#include "AsciiCommandHandler/AsciiCommandHandler.h"
#include "BinaryCommandHandler/BinaryCommandHandler.h"
#include "ButtonManager/ButtonManager.h"
#include "ConfigManager/ConfigManager.h"
#include "DebugManager/DebugManager.h"
#include "FaultHandler/FaultHandler.h"
#include "LedManager/LedManager.h"

// include application files
#include "CommunicationsManager/CommunicationsManager.h"
#include "DbgAscCommandHandler/DbgAscCommandHandler.h"
#include "NetBinCommandHandler/NetBinCommandHandler.h"
#include "NetworkCommandHandler/NetworkCommandHandler.h"
#include "StatusManager/StatusManager.h"

// include third party libraries files
#include "MLAWINC1500Handler/MLAWINC1500Handler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL ) && ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_TASKSCHEDULER )
/// initialization function - interrupts disabled
const CODE PVMICROINITFUNC g_apvInitIrqDsbFunctions[ ] = 
{
  /// base HAL initializations here
  PowerManager_Initialize,
  Clock_Initialize,
  Interrupt_Initialize,
  SystemTick_Initialize,
  
  /// add HAL initializlations here
  Gpio_Initialize,
  I2c_Initialize,
  Spi_Initialize,
  
  /// add OS initializations here
  TaskManager_Initialize,
    
  /// add driver initializations here
  EepromHandler_Initialize,

  /// add protocol initialization here
  
  /// add services initializations here
  AsciiCommandHandler_Initialize,
  BinaryCommandHandler_Initialize,
  ButtonManager_Initialize,
  LedManager_Initialize,

  /// add third party libraries calls
  MLAWINC1500Handler_Initialize,

  /// add application initializations here
  CommunicationsManager_Initialize,
  DbgAscCommandHandler_Initialize,
  NetBinCommandHandler_Initialize,
  StatusManager_Initialize,
  
  // do not remove this entry
  NULL
};

/// initialization function - interrupts enabled
const CODE PVMICROINITFUNC g_apvInitIrqEnbFunctions[ ] =
{
  // add initialization funtions that required interrupts to be enable eher
  SystemControlManager_Initialize,
  IoePCA9555_Initialize,
  UsbGenHandler_Initialize,
  NetworkCommandHandler_Initialize,

  // do not remove this entry
  NULL
};

/// idle functions
const CODE PVMICROIDLEFUNC g_apvIdleFunctions[ ] = 
{
  // add idle functions here
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    TaskManager_IdleProcess,
  #endif // SYSTEMDEFINE_OS_SELECTION

  // do not remove this entry
  NULL
};

/// shutdown functions
const CODE PVMICROSHUTDNFUNC  g_apvShutdownFunctions[ ] =
{
  // add shutdown functions here
 
  // do not remove this entry
  NULL
};

/******************************************************************************
 * @function MicroCheckTasksPendingf
 *
 * @brief tests for tasks pending
 *
 * This function checks to see if tasks are pending
 *
 * @return  TRUE if tasks are pending
 *
 *****************************************************************************/
BOOL Micro_CheckTasksPending( void )
{
  BOOL bResult = TRUE;
  
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
    bResult = TaskManager_CheckTasksPending( TASK_SCHD_MAX );
  #endif // SYSTEMDEFINE_OS_SELECTION

  // return the result
  return( bResult );
}

/******************************************************************************
 * @function Micro_EnterSleepMode
 *
 * @brief go to sleep
 *
 * This function will call he sleep manager in the HAL and put the micro processor
 * to sleep if low power mode is desired
 *
 *****************************************************************************/
void Micro_EnterSleepMode( void )
{
}
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL

/******************************************************************************
 * @function Micro_LocalInitialize
 *
 * @brief local initializtion
 *
 * This function will perform any local initialization
 *
 *****************************************************************************/
void  Micro_LocalInitialize( void )
{
}  

/******************************************************************************
 * @function Micro_LocalIrqInitialize
 *
 * @brief local IEQ initializtion
 *
 * This function will perform any local initialization
 *
 *****************************************************************************/
void  Micro_LocalIrqInitialize( void )
{
}  

/******************************************************************************
 * @function Micro_LocalIdle
 *
 * @brief local dile handler
 *
 * This function will perform handle the local idle tasks
 *
 *****************************************************************************/
void Micro_LocalIdle(void)
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
    TaskScheduler_IdleProcess( );
  #endif // SYSTEMDEFFIN_OS_SELECTON
}

/******************************************************************************
 * @function Micro_DisplayInitFault
 *
 * @brief display initialization fault
 *
 * This function will display the initializaiton fault
 *
 * @param[in]   uError    error to display
 *
 *****************************************************************************/
void Micro_DisplayInitFault( U32 uError )
{
  // disable interrupts
  Interrupt_Disable( );

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

/**@} EOF Micro_cfg.c */
