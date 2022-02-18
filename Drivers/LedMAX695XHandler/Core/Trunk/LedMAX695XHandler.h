/******************************************************************************
 * @file LedMAX695XHandler.h
 *
 * @brief MAX695X LEd driver definitions      
 *
 * This file provides the definitiona for the led driver
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
 * \addtogroup LedMAX695XHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LEDMAX695XHANDLER_H
#define _LEDMAX695XHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedMAX695XHandler/LedMAX695XHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the special characters
typedef enum _LEDMAX695SPCCHAR
{
  LEDMAX695_SPCCHAR_L = 0x80,
  LEDMAX695_SPCCHAR_H,
  LEDMAX695_SPCCHAR_P,
  LEDMAX695_SPCCHAR_Lo,
  LEDMAX695_SPCCHAR_Uo,
  LEDMAX695_SPCCHAR_z1,
  LEDMAX695_SPCCHAR_z2,
  LEDMAX695_SPCCHAR_Ln,
  LEDMAX695_SPCCHAR_Un,
  LEDMAX695_SPCCHAR_Lu,
  LEDMAX695_SPCCHAR_Uu,
  LEDMAX695_SPCCHAR_Lc,
  LEDMAX695_SPCCHAR_Uc,
  LEDMAX695_SPCCHAR_Lbc,
  LEDMAX695_SPCCHAR_Ubc,
  LEDMAX695_SPCCHAR_MINUS,
  LEDMAX695_SPCCHAR_MAX
} LEDMAX695SPCCHAR;

// structures -----------------------------------------------------------------
// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  LedMAX695XHandler_Initialize( void );
extern  void  LedMAX695XHandler_Clear( void );
extern  void  LedMAX695XHandler_SetBrightness( U8 nBrightness );
extern  void  LedMAX695XHandler_DisplayChar( U8 nDigit, U8 nCharacter, BOOL bDecimal );
extern  void  LedMAX695XHandler_DisplayNumber( U8 nBase, U32 uValue, U8 nDecimalPointLoc );

/**@} EOF LedMAX695XHandler.h */

#endif  // _LEDMAX695XHANDLER_H