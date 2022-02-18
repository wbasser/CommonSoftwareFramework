/******************************************************************************
 * @file LedManager.h
 *
 * @brief sound manager declarations
 *
 * This file provides the declarations for the sound manager
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
 *
 * \addtogroup LedManager
 * @{
 *****************************************************************************/
 
// ensure only one instatiation
#ifndef _LEDMANAGER_H
#define _LEDMANAGER_H

// local includes -------------------------------------------------------------
#include "LedManager/LedManager_cfg.h"

// library includes -----------------------------------------------------------
#if ( LEDMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  #include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif  // LEDMANAGER_ENABLE_DEBUG_COMMANDS

// Macros and Defines ---------------------------------------------------------

// Enumerations ---------------------------------------------------------------
/// enumeratre the errors
typedef enum _LEDMANAGERERR
{
  LEDMANAGER_ERR_NONE = 0,      ///< no error
  LEDMANAGER_ERR_ILLLED,        ///< illegal LED enumeration
  LEDMANAGER_ERR_ILLACT,        ///< illegal LED action
  LEDMANAGER_ERR_ILLANI,        ///< illegal animation
  LEDMANAGER_ERR_ILLCLR,        ///< illegal color
} LEDMANAGERERR;

// global parameter declarations -----------------------------------------------
#if ( LEDMANAGER_ENABLE_DEBUG_COMMANDS == ON )
  extern  const CODE ASCCMDENTRY g_atLedManagerCmdHandlerTable[ ];
#endif  // LEDMANAGER_ENABLE_DEBUG_COMMANDS

// global function prototypes --------------------------------------------------
extern 	BOOL          LedManager_Initialize( void );
#if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == ON )
  extern  LEDMANAGERERR LedManager_Control( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption, U8 nIntensity );
#else
  extern  LEDMANAGERERR LedManager_Control( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption );
#endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  extern	LEDMANAGERERR LedManager_PlayAnimation( LEDMNGRANIMENUM eAnimation, U16 wOption );  
#endif // ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
extern  void          LedManager_ProcessAnimation( void );
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
  extern  void          LedManager_ProcessScan( void );
#endif  // MATRIX DEFS
#if ( LEDMANAGER_RGB_LEDS_ENABLED == ON )
  #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == ON )
    extern  LEDMANAGERERR LedManager_RgbControl( LEDMANAGERRGBENUM eLedSel, LEDACTION eAction, LEDRGBCOLOR eColor, U16 wOption, U8 nIntRed, U8 nIntGrn, U8 nIntBlu );
  #else
    extern  LEDMANAGERERR LedManager_RgbControl( LEDMANAGERRGBENUM eLedSel, LEDACTION eAction, LEDRGBCOLOR eColor, U16 wOption );
  #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

#if ( LEDMANAGER_7SEG_LEDS_ENABLED == ON )
  extern  LEDMANAGERERR LedManager_7SegControl( LEDMANAGER7SEGENUM eLedSel, LEDACTION eAction, LED7SEGVALENUM eValue, BOOL bDecimal, U16 wOption );
#endif // LEDMANAGER_7SEG_LEDS_ENABLED 

/**@} EOF LedManager.h */

#endif  // _LEDMANAGER_H