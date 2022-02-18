/******************************************************************************
 * @file LedMAX7219Handler.h
 *
 * @brief MAX7219 LED display handler 
 *
 * This file provides the declarations for the MAX7219 led driver
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
 * \addtogroup LedMAX7219Handler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LEDMAX7219HANDLER_H
#define _LEDMAX7219HANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedMAX7219Handler/LedMAX7219Handler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the decimal point mask
#define   LEDMAX7219_DECIMAL_MASK                         ( 0x80 )

/// define the maximum number of digits
#define   LEDMAX7219_MAXNUM_DIGITS                        ( 8 )

// enumerations ---------------------------------------------------------------
/// enumerate the intensities
typedef enum _LEDMAX7219INTENSITY
{
  LEDMAX7219_INTENSITY_MIN = 0,
  LEDMAX7219_INTENSITY_1,
  LEDMAX7219_INTENSITY_2,
  LEDMAX7219_INTENSITY_3,
  LEDMAX7219_INTENSITY_4,
  LEDMAX7219_INTENSITY_5,
  LEDMAX7219_INTENSITY_6,
  LEDMAX7219_INTENSITY_7,
  LEDMAX7219_INTENSITY_8,
  LEDMAX7219_INTENSITY_9,
  LEDMAX7219_INTENSITY_10,
  LEDMAX7219_INTENSITY_11,
  LEDMAX7219_INTENSITY_12,
  LEDMAX7219_INTENSITY_13,
  LEDMAX7219_INTENSITY_14,
  LEDMAX7219_INTENSITY_MAX
} LEDMAX7219INTENSITY;

/// enumerate the digit
typedef enum LEDMAX7219DIGIT
{
  LEDMAX7219_DIGIT_0 = 0,
  LEDMAX7219_DIGIT_1,
  LEDMAX7219_DIGIT_2,
  LEDMAX7219_DIGIT_3,
  LEDMAX7219_DIGIT_4,
  LEDMAX7219_DIGIT_5,
  LEDMAX7219_DIGIT_6,
  LEDMAX7219_DIGIT_7,
  LEDMAX7219_DIGIT_8,
  LEDMAX7219_DIGIT_9,
  LEDMAX7219_DIGIT_DASH,
  LEDMAX7219_DIGIT_E,
  LEDMAX7219_DIGIT_H,
  LEDMAX7219_DIGIT_L,
  LEDMAX7219_DIGIT_P,
  LEDMAX7219_DIGIT_BLANK,
  LEDMAX7219_DIGIT_DP = 0x80,
} LEDMAX7219DIGIT;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  LedMAX7219Handler_Initialize( void );
extern  void  LedMAX7219Handler_DisplayDigit( U8 nDigitSel, U8 nValue );
extern  void  LedMAX7219Handler_DisplayDigits( PU8 pnDigitVal );

/**@} EOF LedMAX7219Handler.h */

#endif  // _LEDMAX7219HANDLER_H