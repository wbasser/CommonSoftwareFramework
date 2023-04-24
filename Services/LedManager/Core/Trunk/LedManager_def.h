/******************************************************************************
 * @file LedManager_def.h
 *
 * @brief Led manager definitions
 *
 * This file provides the definitions for the LED manager
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
 
// ensure only one instantiation
#ifndef _LEDMANAGER_DEF_H
#define _LEDMANAGER_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedManager/LedManager_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating a direct drive led entry
#define LEDDEF_DIRECT( pin ) \
  { \
    .eDriveType   = LED_DRIVETYPE_DIRECT, \
    .tRowDrive    = \
    { \
      .eDrivePin  = pin, \
    }, \
  }
  
/// define the helper macro for creating a matrix drive led entry
#define LEDDEF_MATRIX( row, col ) \
  { \
    .eDriveType   = LED_DRIVETYPE_MATRIX, \
    .tRowDrive    = \
    { \
      .nRowIndex  = row, \
    }, \
    .nColIndex    = col \
  }

/// define the helper macro for creating a special drive led entry
#define LEDDEF_SPECIAL( func, col ) \
  { \
    .eDriveType   = LED_DRIVETYPE_SPECIAL, \
    .tRowDrive    = \
    { \
      .pvSpclFunc = func, \
    }, \
    .nColIndex    = col \
  }   

/// define the helper macro for creating a PWM drive led entry
#define LEDDEF_DIMMED( pwm ) \
  { \
    .eDriveType   = LED_DRIVETYPE_PWM, \
    .tRowDrive    = \
    { \
      .eDrivePin  = pwm, \
    }, \
  }
  
#if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
  /// define the helper macro for creating an animation step, single led
  #define LEDMNGRDEFSNGSTEP( action, led, duration, nextevent, option, intensity ) \
    { \
      .eAction = action, \
      .nLedEnum = led, \
      .bRgbFlag = FALSE, \
      .wDurationMsecs = duration, \
      .eEvent = nextevent, \
      .wOption = option, \
      .nIntRed = intensity, \
    },
  
  /// define the helper macro for creating an animation step, RGB led
  #define LEDMNGRDEFRGBSTEP( action, led, color, duration, nextevent, option, intred, intgrn, intblu ) \
  { \
    .eAction = action, \
    .nLedEnum = led, \
    .eRgbColor = color, \
    .bRgbFlag = TRUE, \
    .wDurationMsecs = duration, \
    .eEvent = nextevent, \
    .wOption = option, \
    .nIntRed = intred, \
    .nIntGrn = intgrn, \
    .nIntBlu = intblu, \
  },
#else
  /// define the helper macro for creating an animation step, single led
  #define LEDMNGRDEFSNGSTEP( action, led, duration, nextevent, option ) \
    { \
      .eAction = action, \
      .nLedEnum = led, \
      .bRgbFlag = FALSE, \
      .wDurationMsecs = duration, \
      .eEvent = nextevent, \
      .wOption = option, \
    },
  
  /// define the helper macro for creating an animation step, RGB led
  #define LEDMNGRDEFRGBSTEP( action, led, color, duration, nextevent, option ) \
  { \
    .eAction = action, \
    .nLedEnum = led, \
    .eRgbColor = color, \
    .bRgbFlag = TRUE, \
    .wDurationMsecs = duration, \
    .eEvent = nextevent, \
    .wOption = option, \
  },
#endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS

/// define the helper macro for starting an animation sequence
#define LEDMNGRDEFSTART( name ) \
  static  const CODE  LEDSEQENTRY at ## name ## Defs[ ] = { \

/// define the helper macro for ending an animation sequence
#define LEDMNGRDEFSTOP \
  };
  
/// define the helper macro for defining an animation sequence
#define LEDMNGRANIMATIONDEF( name ) \
    ( const CODE PLEDSEQENTRY )&at ## name ## Defs \
  
/// define the helper macro for creating an RGB led
#define LEDMNGRRGBDEF( red, grn, blu ) \
  { \
    .nRedEnum = red, \
    .nGrnEnum = grn, \
    .nBluEnum = blu, \
  }

#if ( LEDMANAGER_7SEG_LEDS_ENABLED == ON )
  #define LEDMNGR7SEGDEF( sega, segb, segc, segd, sege, segf, segg, segdp ) \
  { \
    .nSegAEnum = sega, \
    .nSegBEnum = segb, \
    .nSegCEnum = segc, \
    .nSegDEnum = segd, \
    .nSegEEnum = sege, \
    .nSegFEnum = segf, \
    .nSegGEnum = segg, \
    .nSegDpEnum = segdp,\
  }
#endif // LEDMANAGER_7SEG_LEDS_ENABLED

// enumerations ---------------------------------------------------------------
/// enumerate the LED drive types
typedef enum _LEDDRIVETYPE
{
  LED_DRIVETYPE_DIRECT = 0, ///< direct drive
  LED_DRIVETYPE_MATRIX,     ///< matrix drive
  LED_DRIVETYPE_SPECIAL,    ///< special type
  LED_DRIVETYPE_PWM,        ///< drive type
  LED_DRIVETYPE_MAX
} LEDDRIVETYPE;

/// enumerate the LED actions
typedef enum _LEDACTION
{
  LED_ACTION_OFF = 0,           ///< turn off the led
  LED_ACTION_ON,                ///< turn on the led
  LED_ACTION_BLINKSLOW,         ///< blink the led slow
  LED_ACTION_BLINKFAST,         ///< blink the led fast
  LED_ACTION_PULSE,             ///< pulse the led on
  LED_ACTION_BLINKSLOW_LOCK,    ///< blink the led slow - disable all off/on changes
  LED_ACTION_BLINKFAST_LOCK,    ///< blink the led fast - disable all off/on changes
  LED_ACTION_PULSE_LOCK,        ///< pulse the led on - disable all off/on changes 
  LED_ACTION_MAX,               ///< individual max
  LED_ACTION_ALLOFF,            ///< all off
  LED_ACTION_ALLON,             ///< all on
  LED_ACTION_ALLBLINKSLOW,      ///< all blink slow
  LED_ACTION_ALLBLINKFAST,      ///< all blink fast
  LED_ACTION_ALLMAX,            ///< all max
  LED_ACTION_NONE,
} LEDACTION;

/// enumerate the RGB colors
typedef enum _LEDRGBCOLOR
{
  LED_RGBCOLOR_BLK = 0,         ///< black
  LED_RGBCOLOR_BLU,             ///< blue
  LED_RGBCOLOR_GRN,             ///< green
  LED_RGBCOLOR_CYN,             ///< cyan
  LED_RGBCOLOR_RED,             ///< red
  LED_RGBCOLOR_VIO,             ///< violet
  LED_RGBCOLOR_ORN,             ///< orange
  LED_RGBCOLOR_WHT,             ///< white
  LED_RGBCOLOR_MAX
} LEDRGBCOLOR;

/// enumerate the LED sequence events
typedef enum _LEDSEQEVENT
{
  LED_SEQEVENT_DONE = 0,    ///< sequence done
  LED_SEQEVENT_NEXT,        ///< next sequence
  LED_SEQEVENT_JMPBEG,      ///< jump to the beginning
  LED_SEQEVENT_JMPSEQ,      ///< jump to a step within this sequence
  LED_SEQEVENT_WAIT,        ///< wait till count is zero
  #if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
    LED_SEQEVENT_CALL,      ///< call another sequence
  #endif
  LED_SEQEVENT_MAX    
} LEDSEQEVENT, *PLEDSEQEVENT;

#if ( LEDMANAGER_7SEG_LEDS_ENABLED == ON )
  /// enumerate the LED values
  typedef enum _LED7SEGVALENUM
  {
    LED_7SEGVAL_0 = 0,
    LED_7SEGVAL_1,
    LED_7SEGVAL_2,
    LED_7SEGVAL_3,
    LED_7SEGVAL_4,
    LED_7SEGVAL_5,
    LED_7SEGVAL_6,
    LED_7SEGVAL_7,
    LED_7SEGVAL_8,
    LED_7SEGVAL_9,
    LED_7SEGVAL_A,
    LED_7SEGVAL_B,
    LED_7SEGVAL_C,
    LED_7SEGVAL_D,
    LED_7SEGVAL_E,
    LED_7SEGVAL_F,
    LED_7SEGVAL_H,
    LED_7SEGVAL_L,
    LED_7SEGVAL_P,
    LED_7SEGVAL_o,
    LED_7SEGVAL_c,
    LED_7SEGVAL_h,
    LED_7SEGVAL_r,
    LED_7SEGVAL_DASH,
    LED_7SEGVAL_MAX,
    LED_7SEGVAL_SPC = 0x80,
  } LED7SEGVALENUM;
#endif // LEDMANAGER_7SEG_LEDS_ENABLED

// structures -----------------------------------------------------------------
/// define the function type for special LED drives
typedef void ( *PVLEDSPCLFUNC )( U8, BOOL );

/// define the definition structure for LEDS
typedef struct _LEDDEF
{
  LEDDRIVETYPE      eDriveType;   ///< drive type
  union
  {
    U8              eDrivePin;    ///< direct type drive pin
    U8              nRowIndex;    ///< row index for matrix
    PVLEDSPCLFUNC   pvSpclFunc;   ///< special drive funciton
  } tRowDrive;                    ///< row drive
  U8                nColIndex;    ///< column index
} LEDDEF, *PLEDDEF;
#define LEDDEF_SIZE                             sizeof( LEDDEF )

/// define the structure for a sequence entry
typedef struct _LEDSEQENTRY
{
  LEDACTION     eAction;        ///< LED action
  U8            nLedEnum;       ///< led enumeration
  LEDRGBCOLOR   eRgbColor;      ///< RGB color
  BOOL          bRgbFlag;       ///< RGB flag
  U16           wDurationMsecs; ///< duration in msecs
  LEDSEQEVENT   eEvent;         ///< action to execute when this step is done
  U16           wOption;        ///< option
  #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
    U8            nIntRed;        ///< intensity red
    U8            nIntGrn;        ///< intensity green
    U8            nIntBlu;        ///< intensity blue
  #else
  #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
} LEDSEQENTRY, *PLEDSEQENTRY;
#define LEDSEQENTRY_SIZE                        sizeof( LEDSEQENTRY )

#if ( LEDMANAGER_RGB_LEDS_ENABLED == ON )
  /// define the structure for defining a RGB led
  typedef struct _LEDRGBDEF
  {
    U8            nRedEnum;       ///< red LED enumeration
    U8            nGrnEnum;       ///< green led enumeration
    U8            nBluEnum;       ///< blue led enumeration
  } LEDRGBDEF, *PLEDRGBDEF;
  #define LEDRGBDEF_SIZE                        sizeof( LEDRGBDEF )
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

#if ( LEDMANAGER_7SEG_LEDS_ENABLED == ON )
  /// define the structure for definining a 7 segment LED
  typedef struct _LED7SEGDEF
  {
    U8            nSegAEnum;    ///< segment A enumeration
    U8            nSegBEnum;    ///< segment B enumeration
    U8            nSegCEnum;    ///< segment C enumeration
    U8            nSegDEnum;    ///< segment D enumeration
    U8            nSegEEnum;    ///< segment E enumeration
    U8            nSegFEnum;    ///< segment F enumeration
    U8            nSegGEnum;    ///< segment G enumeration
    U8            nSegDpEnum;   ///< segment DP enumeration
  } LED7SEGDEF, *PLED7SEGDEF;
  #define LED7SEGDEF_SIZE                       sizeof( LED7SEGDEF )
#endif // LEDMANAGER_7SEG_LEDS_ENABLED

/**@} EOF LedManager_def.h */

#endif  // _LEDMANAGER_DEF_H