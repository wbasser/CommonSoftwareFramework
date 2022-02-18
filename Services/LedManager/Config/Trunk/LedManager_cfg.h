/******************************************************************************
 * @file LedManager_cfg.h
 *
 * @brief LED manager configuration declarations
 *
 * This file provides the configuration declarations for the LED manager
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
 * $Log: $
 * 
 * \addtogroup LedManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LEDMANAGER_CFG_H
#define _LEDMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedManager/LedManager_prm.h"
#include "LedManager/LedManager_def.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "TaskManager/TaskManager.h"
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #define LEDMANAGER_ANIMATE_EXEC_RATE    TASK_TIME_MSECS(( LEDMANAGER_ANIMATE_RATE_MSECS ))
  #define LEDMANAGER_ANIMATE_NUM_EVENTS   ( 4 )
  #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
    #define LEDMANAGER_SCAN_EXEC_RATE       ( TASK_TIME_MSECS( LEDMANAGER_SCAN_RATE_MSECS ))
    #define LEDMANAGER_SCAN_NUM_EVENTS      ( 2 )
  #endif // SCAN
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )


// enumerations ---------------------------------------------------------------
/// enumerate each led
typedef enum _LEDMANAGERSELENUM
{
  // enumerate user LEDS here
  LEDMANAGER_ENUM_STSLED = 0,

  
  // do not remove the below entry
  LEDMANAGER_ENUM_MAX,
  LEDMANAGER_ENUM_NONE,
  LEDMANAGER_ENUM_ALL = 0xFF
} LEDMANAGERSELENUM;

#if ( LEDMANAGER_RGB_LEDS_ENABLED == ON )
  /// enumerate the RGB leds
  typedef enum _LEDMANAGERRGBENUM
  {
    // enumerate user RGBs here
  
    // do not remove the below entries
    LEDMANAGER_RGBENUM_MAX,
    LEDMANAGER_RGBENUM_NONE,
    LEDMANAGER_RGBENUM_ALL = 0xFF
  } LEDMANAGERRGBENUM;
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

#if ( LEDMANAGER_7SEG_LEDS_ENABLED == ON )
  /// enuemrate the 7 seg leds
  typedef enum _LEDMANAGER7SEGENUM
  {
    // enumerate user 7 segment here
    
    // do not remove these below
    LEDMNGR_7SEGENUM_MAX,
    LEDMNGR_7SEGENUM_NONE,
    LEDMNGR_7SEGENUM_ALL = 0xFF
  } LEDMANAGER7SEGENUM;
#endif // LEDMANAGER_7SEG_LEDS_ENABLED

/// enumerate each animation
typedef enum _LEDMNGRANIMENUM
{
  // enumerate user animations here

  // do not remove these below
  LEDMNGR_ANIMATION_MAX,
  LEDMNGR_ANIMATION_NONE,
  LEDMNGR_ANIMATION_STOP = 0xFF
} LEDMNGRANIMENUM;

// global parameter declarations -----------------------------------------------
/// declare the led definitions
extern  const CODE LEDDEF  g_atLedDefs[ ];
#if ( LEDMANAGER_RGB_LEDS_ENABLED == ON )
  extern  const CODE LEDRGBDEF  g_atLedRgbDefs[ ];
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

/// declare the animation enumeration
extern  const CODE PLEDSEQENTRY g_apLedAnimationsDef[ ];

/// declare the LED matrix rows/cols
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
  extern  const CODE GPIOPINENUM g_aeLedMatrixRows[ ];
  extern  const CODE GPIOPINENUM g_aeLedMatrixCols[ ];
#endif  // MATRIX DEFS
#if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
  extern  const CODE LEDRGBDEF  g_atLedsRgbDef[ ];
#endif // LEDMANAGER_RGB_LEDS_ENABLED

#if ( LEDMANAGER_7SEG_LEDS_ENABLED == ON )
  extern  const CODE LED7SEGDEF g_atLeds7SegDef[ ];
#endif // LEDMANAGER_7SEG_LEDS_ENABLED

// global function prototypes --------------------------------------------------
extern  BOOL  LedManager_LocalInitialize( void );
extern  void  LedManager_GpioSet( U8 nPinEnum, BOOL bState );
#if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
  extern  void  LedManager_PwmSet( U8 nPwmEnum, U8 nIntensity );
#endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
extern  void  LedManager_NotifyDone( void );
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  extern  BOOL  LedManager_AnimationTask( TASKARG xArg );
  #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
    extern  BOOL  LedManager_ScanTask( TASKARG xArg );
  #endif // SCCAN
#endif // ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/**@} EOF LedManager_cfg.h */

#endif  // _LEDMANAGER_CFG_H