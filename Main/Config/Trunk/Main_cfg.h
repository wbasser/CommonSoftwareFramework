/******************************************************************************
 * @file Main_cfg.h
 *
 * @brief configuration file declarations
 *
 * This file declares the initialization structures for main
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

// ensure only one instatiation
#ifndef _MAIN_CFG_H
#define _MAIN_CFG_H

// local includes -------------------------------------------------------------
#include "Main/Main_def.h"

// library includes -----------------------------------------------------------
#include "SystemDefines/SystemDefines_prm.h"

// macro declarations ---------------------------------------------------------
#define MAIN_ENABLE_SYSTIMECMDS                          ( 0 )

// global parameter declarations -----------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  /// declare the initialization -IRQ disabled structure
  extern  const CODE PVMAININITFUNC    g_apvInitIrqDsbFunctions[ ];

  /// declare the initialization -IRQ enabled structure
  extern  const CODE PVMAININITFUNC    g_apvInitIrqEnbFunctions[ ];

  /// declare the idle structure
  extern  const CODE PVMAINIDLEFUNC    g_apvIdleFunctions[ ];

  /// declare the shutdown structure
  extern  const CODE PVMAINSHUTDNFUNC  g_apvShutdownFunctions[ ];
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

// global function declarations ---------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  extern  BOOL  Main_CheckTasksPending( void );
  extern  void  Main_EnterSleepMode( void );
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
extern  void  Main_LocalInitialize( void );
extern  void  Main_LocalIrqInitialize( void );
extern  void  Main_LocalIdle(void);
extern  void  Main_DisplayInitFault( U32 uError );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  extern void vApplicationStackOverflowHook( xTaskHandle *pxTask,signed char *pcTaskName);
  extern void vApplicationIdleHook(void);
  extern void vApplicationTickHook(void);
  extern void vApplicationMallocFailedHook(void);
#endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS

/**@} EOF Main_cfg.c */

#endif // _MAIN_CFG_H
