/******************************************************************************
 * @file LedHT16K33Handler.h
 *
 * @brief HT16K33 LEd driver definitions      
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
 * \addtogroup LedHT16K33Handler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LEDHT16K33HANDLER_H
#define _LEDHT16K33HANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedHT16K33Handler/LedHT16K33Handler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the blink rates
typedef enum _LEDHT16K33BLINKRATE
{
  LEDHT16K33_BLINKRATE_OFF = 0,
  LEDHT16K33_BLINKRATE_2HZ,
  LEDHT16K33_BLINKRATE_1HZ,
  LEDHT16K33_BLINKRATE_0HZ5,
  LEDHT16K33_BLINKRATE_MAX
} LEDHT16K33BLINKRATE;

/// enumerate the brightness
typedef enum _LEDHT16K33BRIGHTNESS
{
  LEDHT16K33_BRIGHTNESS_1 = 0,
  LEDHT16K33_BRIGHTNESS_2,
  LEDHT16K33_BRIGHTNESS_3,
  LEDHT16K33_BRIGHTNESS_4,
  LEDHT16K33_BRIGHTNESS_5,
  LEDHT16K33_BRIGHTNESS_6,
  LEDHT16K33_BRIGHTNESS_7,
  LEDHT16K33_BRIGHTNESS_8,
  LEDHT16K33_BRIGHTNESS_9,
  LEDHT16K33_BRIGHTNESS_10,
  LEDHT16K33_BRIGHTNESS_11,
  LEDHT16K33_BRIGHTNESS_12,
  LEDHT16K33_BRIGHTNESS_13,
  LEDHT16K33_BRIGHTNESS_14,
  LEDHT16K33_BRIGHTNESS_15,
  LEDHT16K33_BRIGHTNESS_16,
  LEDHT16K33_BRIGHTNESS_MAX
} LEDHT16K33BRIGHTNESS;

/// enumerate the special characters
typedef enum _LEDHT16K33SPCCHAR
{
  LEDHT16K33_SPCCHAR_L = 0x80,
  LEDHT16K33_SPCCHAR_H,
  LEDHT16K33_SPCCHAR_P,
  LEDHT16K33_SPCCHAR_Lo,
  LEDHT16K33_SPCCHAR_Uo,
  LEDHT16K33_SPCCHAR_z1,
  LEDHT16K33_SPCCHAR_z2,
  LEDHT16K33_SPCCHAR_Ln,
  LEDHT16K33_SPCCHAR_Un,
  LEDHT16K33_SPCCHAR_Lu,
  LEDHT16K33_SPCCHAR_Uu,
  LEDHT16K33_SPCCHAR_Lc,
  LEDHT16K33_SPCCHAR_Uc,
  LEDHT16K33_SPCCHAR_Lbc,
  LEDHT16K33_SPCCHAR_Ubc,
  LEDHT16K33_SPCCHAR_MINUS,
  LEDHT16K33_SPCCHAR_MAX
} LEDHT16K33SPCCHAR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  LedHT16K33Handler_Initialize( );
extern  void  LedHT16K33Handler_Clear( U8 nDeviceID );
extern  void  LedHT16K33Handler_SetBlink( U8 nDeviceID, LEDHT16K33BLINKRATE eBlinkRate );
extern  void  LedHT16K33Handler_SetBrightness( U8 nDeviceID, LEDHT16K33BRIGHTNESS eBrightness );
extern  void  LedHT16K33Handler_DisplayColon( U8 nDeviceID, BOOL bDisplayColon );
extern  void  LedHT16K33Handler_DisplayChar( U8 nDeviceID, U8 nDigit, U8 nCharacter, BOOL bDecimal );
extern  void  LedHT16K33Handler_DisplayNumber( U8 nDeviceID, U8 nBase, U32 uValue, U8 nDecimalPointLoc, BOOL bDisplayColon );
extern  void  LedHT16K33Handler_UpdateDisplay( U8 nDeviceID );

/**@} EOF LedHT16K33Handler.h */

#endif  // _LEDHT16K33HANDLER_H