/******************************************************************************
 * @file Main.c
 *
 * @brief main system initialization and idle loop
 *
 * This file contains the system initializations system tick
 * initializations 
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
// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Main.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "SystemTick/SystemTick.h"
  #include "TaskManager/TaskManager.h"
#endif

// local parameter declarations -----------------------------------------------
#if ( MAIN_ENABLE_SYSTIMECMDS  == ON )
  static  U32 uMaxExecTimeMsec;
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL

// local function prototypes --------------------------------------------------
#if ( MAIN_ENABLE_SYSTIMECMDS  == ON )
  /// command handlers
  static  ASCCMDSTS CmdQrySys( U8 nCmdEnum );

  // constant parameter initializations -----------------------------------------
  /// declare the command strings
  static  const CODE C8 szQrySys[ ]   = { "QRYSYS" };

  /// initialize the command table
  const CODE ASCCMDENTRY g_atMainCmdHandlerTable[ ] =
  {
    ASCCMD_ENTRY( szQrySys,  6, 0, ASCFLAG_COMPARE_NONE, 0, CmdQrySys ),

    // the entry below must be here
    ASCCMD_ENDTBL( )
  };

  /// define the response strings
  static  const CODE C8 szRspSys[ ]   = { "RSYS, %8lu\n\r" };
#endif // MAIN_ENABLE_SYSTIMECMDS

/******************************************************************************
 * @function main
 *
 * @brief this is the main function
 *
 * This function initializes the system clock, initialization functions listed
 * in the config structure, system tick and the task manager.
 *
 * @return  -1 to indicate an error as we should never exit
 *
 *****************************************************************************/
int main( void )
{
  BOOL bStatus = FALSE;
  
  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL ) && ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_TASKSCHEDULER )
    U8              nIdx;
    PVMAININITFUNC pvInitFunc;
    #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_ZEPHYR )
      PVMAINIDLEFUNC pvIdleFunc;
    #endif 
    #if ( MAIN_ENABLE_SYSTIMECMDS == ON )
      U32             uStartTime, uStopTime, uDiffTime;
    #endif
  #endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL

  // disable interrupts
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
    Interrupt_Disable( );
  #else
    __disable_irq( );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR
  
  // call the local initialization
  #if (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL ) || ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER ))
    Main_LocalInitialize( );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL

  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL ) && ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_TASKSCHEDULER )
    #if ( MAIN_ENABLE_SYSTIMECMDS == ON )
      // clear the execution time
      uMaxExecTimeMsec = 0;
    #endif
  
    // initialize IRQ disabled functions
    nIdx = 0;
    while (( pvInitFunc = ( PVMAININITFUNC )PGM_RDWORD( g_apvInitIrqDsbFunctions[ nIdx++ ])) != NULL )
    {
       // call the init function
       bStatus |= pvInitFunc( );

      // check for errors
      if ( bStatus == TRUE )
      {
        // display fault
        Main_DisplayInitFault(( U32 )pvInitFunc );
      }
    }
  #endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
  
  // enable interrupts
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
    Interrupt_Enable( );
  #else
    __enable_irq( );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR
  
    // call the local initialization
  #if (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL ) || ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER ))
      Main_LocalIrqInitialize( );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL

  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL ) && ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_TASKSCHEDULER )
    // initialize IRQ enabled functions
    nIdx = 0;
    while (( pvInitFunc = ( PVMAININITFUNC )PGM_RDWORD( g_apvInitIrqEnbFunctions[ nIdx++ ])) != NULL )
    {
      // call the init function
      bStatus |= pvInitFunc( );
    }

    // check for errors
    if ( bStatus == TRUE )
    {
      // diaplay the fault
      Main_DisplayInitFault(( U32 )pvInitFunc );
    }
  #endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
  
  // idle loop
  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_ZEPHYR )
    FOREVER
    {
      // local the local idle
      #if (( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL ) && ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_TASKSCHEDULER ))
        #if ( MAIN_ENABLE_SYSTIMECMDS  == ON )
          // get the current time
          uStartTime = SystemTick_GetTimeMsec();
        #endif
              
        // reset index
        nIdx = 0;
      
        // while not a null entry
        while (( pvIdleFunc = ( PVMAINIDLEFUNC )PGM_RDWORD( g_apvIdleFunctions[ nIdx++ ])) != NULL )
        {
          // call the init function
          pvIdleFunc( );
        }

        #if (( MAIN_ENABLE_SYSTIMECMDS  == ON ) && ( SYSTEM_DEFINE_OS_SELECTION != SYSTEMDEFINE_OS_ZEPHYR ))
          // get the stop time
          uStopTime = SystemTick_GetTimeMsec();
          
          // compute time
          if ( uStopTime < uStartTime )
          {
            uDiffTime = uStartTime - uStopTime;      
          }
          else
          {
            uDiffTime = uStopTime - uStartTime;
          }
          
          // is this the max
          if ( uDiffTime > uMaxExecTimeMsec )
          {
            uMaxExecTimeMsec = uDiffTime;
          }
        #endif
        
        // check for pending tasks
        if ( !Main_CheckTasksPending( /*TASK_SCHD_ENUM_MAX*/ ))
        {
          // go to sleep
          Main_EnterSleepMode( );
        }
      #elif (( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )  || ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER ))
        Main_LocalIdle( );
      #endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
    }
  #endif   
  // return a -1
  return( -1 );
}

#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL ) && ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_TASKSCHEDULER )
  /******************************************************************************
  * @function Main_Shutdown
  *
  * @brief shutdown all peripherals
  *
  * This function will shut down all peripherals
  *
  *****************************************************************************/
  void Main_Shutdown( void )
  {
    U8                nIdx;
    PVMAINSHUTDNFUNC pvShutdownFunc;

    // disable interrupts
    Interrupt_Disable( );

    // initialize IRQ disabled functions
    nIdx = 0;
    while (( pvShutdownFunc = ( PVMAINSHUTDNFUNC ) PGM_RDWORD( g_apvShutdownFunctions[ nIdx++ ] )) != NULL )
    {
      // call the shutdown function
      pvShutdownFunc( );
    }
  } 
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL

#if ( MAIN_ENABLE_SYSTIMECMDS == ON )
  /******************************************************************************
  * @function CmdQrySys
  *
  * @brief query system execution time command handler
  *
  * This function outputs the current board title and version
  *
  * @return  Aappropriate statue
  *****************************************************************************/
  static ASCCMDSTS CmdQrySys( U8 nCmdEnum )
  {
    PC8   pcBuffer;
  
    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // output the version
    SPRINTF_P( ( pcBuffer ), ( char const * )szRspSys, uMaxExecTimeMsec );

    // return the error
    return( ASCCMD_STS_OUTPUTBUFFER);
  }
#endif // MAIN_ENABLE_SYSTIMECMDS

/**@} EOF Main.c */
