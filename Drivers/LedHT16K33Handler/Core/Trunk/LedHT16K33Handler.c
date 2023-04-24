/******************************************************************************
 * @file LedHT16K33Handler.c
 *
 * @brief LED HT16K3 led handler implementation
 *
 * This file provides the implementation for the HT16K3 led driver
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedHT16K33Handler/LedHT16K33Handler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the commands
#define CMD_DSP_ADDR                            ( 0x00 )
#define CMD_SYS_SETP                            ( 0x02 )
#define CMD_KEY_ADDR                            ( 0x04 )
#define CMD_INT_FLAG                            ( 0x06 )
#define CMD_DSP_CTRL                            ( 0x08 )
#define CMD_ROW_IRQS                            ( 0x0A )
#define CMD_DIM_CTRL                            ( 0x0E )

// define the colon index
#define COLON_BUF_INDEX                         ( 2 )

// define the decimal point mask
#define DECIMAL_POINT_MASK                      ( 0x80 )

// define the special character mask
#define SPECIAL_CHAR_MASK                       ( 0x80 )

// define the buffer size
#define LEDBUFFER_SIZE                          ( LEDHT16K33_OFFSET_MAX << 1 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the structure for command
typedef union _LEDHT16K33CMDDEF
{
  struct
  {
    U8  nOpt    : 4;        ///< option
    U8  nCmd    : 4;        ///< command
  } tFields;
  U8  nByte;                ///< byte
} LEDHT16K33CMDDEF, *PLEDHT16K33CMDDEF;
#define LEDHT16K33CMDDEF_SIZE                   sizeof( LEDHT16K33CMDDEF )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static U8   anDisplayBuffer[ LEDHT16K33_ENUM_MAX ][ LEDBUFFER_SIZE ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// instantiate the numeric lookup table
static  const CODE  U8  anNumeric7Segment[ ] =
{
  0x3F,   ///< 0
  0x06,   ///< 1
  0x5B,   ///< 2
  0x4F,   ///< 3
  0x66,   ///< 4
  0x6D,   ///< 5
  0x7D,   ///< 6
  0x07,   ///< 7
  0x7F,   ///< 8
  0x6F,   ///< 9
  0x77,   ///< A
  0x7C,   ///< b
  0x39,   ///< C
  0x5E,   ///< d
  0x79,   ///< E
  0x71    ///< F
};

/// instantiate the special characters
static  const CODE U8 anSpecial7Segment[ LEDHT16K33_SPCCHAR_MAX ] =
{
  0x38,   ///< L
  0x76,   ///< H
  0x73,   ///< P
  0x00,   ///< Lo
  0x00,   ///< Uo
  0x00,   ///< Z1
  0x00,   ///< Z2
  0x00,   ///< Ln
  0x00,   ///< Un
  0x00,   ///< Lu
  0x00,   ///< Uu
  0x00,   ///< Lc
  0x00,   ///< Uc
  0x00,   ///< Lbc
  0x00,   ///< Ubc
  0x40,   ///< Minus
};

  
/******************************************************************************
 * @function LedHT16K33Handler_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 * @return    TRUE if errors, FALSE otherwise
 *
 *****************************************************************************/
BOOL LedHT16K33Handler_Initialize( void )
{
  BOOL  bStatus = FALSE;
  LEDHT16K33CMDDEF  tCmd;
  LEDHT16K33ENUM    eEnum;

  // call the local initializetion
  bStatus = LedHT16K33Handler_LocalInitialize( );

  // for each display
  for( eEnum = 0; eEnum < LEDHT16K33_ENUM_MAX; eEnum++ )
  {
    // turn on oscillator
    tCmd.tFields.nCmd = CMD_SYS_SETP;
    tCmd.tFields.nOpt = ON;
    LedHT16K33Handler_WriteData( eEnum, tCmd.nByte, NULL, 0 );

    // clear the display
    LedHT16K33Handler_Clear( eEnum);

    // turn on display
    tCmd.tFields.nCmd = CMD_DSP_CTRL;
    tCmd.tFields.nOpt = ON;
    LedHT16K33Handler_WriteData( eEnum, tCmd.nByte, NULL, 0 );
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function LedHT16K33Handler_Clear
 *
 * @brief clear the display
 *
 * This function will clear the display
 *
 * @param[in]   nDevice device ID
 *
 *****************************************************************************/
void LedHT16K33Handler_Clear( U8 nDeviceID )
{
  // clear the display
  memset( &anDisplayBuffer[ nDeviceID ][ 0 ], 0, LEDBUFFER_SIZE );
  LedHT16K33Handler_UpdateDisplay( nDeviceID );
}

/******************************************************************************
 * @function LedHT16K33Handler_SetBlink
 *
 * @brief set the blink state
 *
 * This function will cause the LED to blink
 *
 * @param[in]   nDevice device ID
 * @param[in]   eBlnkRate desired blink rate
 *
 *****************************************************************************/
void LedHT16K33Handler_SetBlink( U8 nDeviceID, LEDHT16K33BLINKRATE eBlinkRate )
{
  LEDHT16K33CMDDEF  tCmd;

  // set up the command
  tCmd.tFields.nCmd = CMD_DSP_CTRL;
  tCmd.tFields.nOpt = ( eBlinkRate << 1 ) | ON;
  LedHT16K33Handler_WriteData( nDeviceID, tCmd.nByte, NULL, 0 );
}

/******************************************************************************
 * @function LedHT16K33Handler_SetBrightness 
 *
 * @brief set the brightness
 *
 * This function will set the display brightness
 *
 * @param[in]   nDevice device ID
 * @param[in]   eBrightness   selected brightness
 *
 *****************************************************************************/
void LedHT16K33Handler_SetBrightness( U8 nDeviceID, LEDHT16K33BRIGHTNESS eBrightness )
{
  LEDHT16K33CMDDEF  tCmd;

  // set up the command
  tCmd.tFields.nCmd = CMD_DIM_CTRL;
  tCmd.tFields.nOpt = eBrightness;
  LedHT16K33Handler_WriteData( nDeviceID, tCmd.nByte, NULL, 0 );
}

/******************************************************************************
 * @function LedHT16K33Handler_DisplayNumber
 *
 * @brief display a number
 *
 * This function will display a number
 *
 * @param[in]   nDevice device ID
 * @param[in]   bDisplayColon     display the colon
 *
 *****************************************************************************/
void LedHT16K33Handler_DisplayColon( U8 nDeviceID, BOOL bDisplayColon )
{
  // check for colon
  if ( bDisplayColon == TRUE )
  {
    // turn on cursor
    anDisplayBuffer[nDeviceID ][ g_anLedHT16K33CharOffset[ LEDHT16K33_OFFSET_COLON ]  << 1 ] = 0x02;
  }
}

/******************************************************************************
 * @function LedHT16K33Handler_DisplayChar
 *
 * @brief display a character
 *
 * This function will display a character
 *
 * @param[in]   nDevice device ID
 * @param[in]   nDigit      digit location
 * @param[in]   nCharacter  nCharacter
 * @param[in]   bDecimal    enable decimal
 *
 *****************************************************************************/
void LedHT16K33Handler_DisplayChar( U8 nDeviceID, U8 nDigit, U8 nCharacter, BOOL bDecimal )
{
  U8  nBitmap, nBufIndex;

  // set the buffer index
  nBufIndex = g_anLedHT16K33CharOffset[ nDigit ] << 1;

  // check for a special character
  if ( nCharacter & SPECIAL_CHAR_MASK )
  {
    // get the special character
    nBitmap = PGM_RDBYTE( anSpecial7Segment[ nCharacter & 0x7F ] );
  }
  else
  {
    // get the bit map
    nBitmap = PGM_RDBYTE( anNumeric7Segment[ nCharacter & 0x0F ] );
  }

  // check for decimal
  if ( bDecimal )
  {
    // set the decimal point
    nBitmap |= DECIMAL_POINT_MASK;
  }

  // store the character
  anDisplayBuffer[ nDeviceID ][ nBufIndex ] =  nBitmap;
}

/******************************************************************************
 * @function LedHT16K33Handler_DisplayNumber
 *
 * @brief display a number
 *
 * This function will display a number
 *
 * @param[in]   nDevice device ID
 * @param[in]   nBase             base
 * @param[in]   uValue            value to display
 * @param[in]   nDecimalPointLoc  decimal point location
 * @param[in]   bDisplayColon     display the colon
 *
 *****************************************************************************/
void LedHT16K33Handler_DisplayNumber( U8 nDeviceID, U8 nBase, U32 uValue, U8 nDecimalPointLoc, BOOL bDisplayColon )
{
  U8  nCurValue, nCurDigit, nBufIndex;

  // check for valid number
  if ((( 10 == nBase ) && ( uValue <= LEDHT16K33_MAX_NUMBER_BASE_TEN )) ||
      (( 16 == nBase ) && ( uValue <= LEDHT16K33_MAX_NUMBER_BASE_HEX )))
  {
    // set the digit to lsb
    nCurDigit = LEDHT16K33_OFFSET_3;

    // loop while 
    while ( uValue > 0 )
    {
      // get the current character value
      nCurValue = uValue % nBase;

      // display the digit
      LedHT16K33Handler_DisplayChar( nDeviceID, nCurDigit, nCurValue, ( nDecimalPointLoc == nCurDigit ));

      // adjust value
      uValue /= nBase;

      // update the digit
      nCurDigit--;
    }

    // clear rest
    while( nCurDigit > 0 )
    {
      // decrement the digit
      nCurDigit--;

      // set the index
      nBufIndex = nCurDigit << 1;

      // clear rest of display
      anDisplayBuffer[ nDeviceID ][ nBufIndex ] = 0;
    }

    // check for colon
    if ( bDisplayColon == TRUE )
    {
      // turn on cursor
      anDisplayBuffer[ nDeviceID ][ COLON_BUF_INDEX << 1 ] = 0x02;
    }
  }
  else
  {
    // for each character 
    for ( nCurDigit = 0; nCurDigit < LEDHT16K33_OFFSET_MAX; nCurDigit++ )
    {
      // display a dash
      LedHT16K33Handler_DisplayChar( nDeviceID, nCurDigit, LEDHT16K33_SPCCHAR_MINUS, OFF );
    }
  }

  // auto update display
  LedHT16K33Handler_UpdateDisplay( nDeviceID );
}

/******************************************************************************
 * @function LedHT16K33Handler_UpdateDisplay
 *
 * @brief update the display
 *
 * @param[in]   nDevice device ID
 *
 * This function will update the display
 *
 *****************************************************************************/
void LedHT16K33Handler_UpdateDisplay( U8 nDeviceID )
{
  LEDHT16K33CMDDEF  tCmd;

  // set up the command
  tCmd.tFields.nCmd = CMD_DSP_ADDR;
  tCmd.tFields.nOpt = 0;
  LedHT16K33Handler_WriteData( nDeviceID, tCmd.nByte, &anDisplayBuffer[ nDeviceID ][ 0], LEDBUFFER_SIZE );
}

/**@} EOF LedHT16K33Handler.c */
