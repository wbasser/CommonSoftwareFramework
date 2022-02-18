/******************************************************************************
 * @file LedMAX695XHandler.c
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
 * \addtogroup LedMAX695XHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedMAX695XHandler/LedMAX695XHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the Decimal point masks 

// define the special character mask
#define SPECIAL_CHAR_MASK                       ( 0x80 )

/// define the number of actual digits
#define MAX_NUM_DIGITS                          ( 4 )

/// define the maximum value for display
#define MAX_NUMBER_BASE_TEN                     ( 9999 )
#define MAX_NUMBER_BASE_HEX                     ( 0xFFFF )

// enumerations ---------------------------------------------------------------
/// enumerate the register addresses
typedef enum _REGADDR
{
  REG_ADDR_NOOP     = 0,        ///< no-operation
  REG_ADDR_DECMODE,             ///< decode mode
  REG_ADDR_INTEN,               ///< intensity
  REG_ADDR_SCANLIM,             ///< scan limit
  REG_ADDR_CONFIG,              ///< configuration
  REG_ADDR_DISPTST  = 0x07,     ///< display test
  REG_ADDR_DIGIT0   = 0x20,     ///< digit 0
  REG_ADDR_DIGIT1,              ///< digit 1
  REG_ADDR_DIGIT2,              ///< digit 2
  REG_ADDR_DIGIT3,              ///< digit 3
  REG_ADDR_SEGMENT,             ///< segments
} REGADDR;

// structures -----------------------------------------------------------------
//// define the decode mode register structure
typedef union REGDECMODE
{
  struct
  {
    U8 bDig0DecDEnb   : 1;
    U8 bDig1DecDEnb   : 1;
    U8 bDig2DecDEnb   : 1;
    U8 bDig3DecDEnb   : 1;
  } tFields;
  U8  nByte;
} REGDECMODE, *PREGDECMODE;
#define REGDECMODE_SIZE                         sizeof( REGDECMODE )

/// define the config register strcuture
typedef union REGCONFIG
{
  struct
  {
    U8 bShutdownDsb    : 1;
    U8 bSel6959Enb     : 1;
    U8 bReserved1      : 3;
    U8 bClearDisplay   : 1;
  } tFields;
  U8  nByte;
} REGCONFIG, *PREGCONFIG;
#define REGCONFIG_SIZE                          sizeof( REGCONFIG )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8  anLclBuffer[ MAX_NUM_DIGITS ];

// local function prototypes --------------------------------------------------
  
// constant parameter initializations -----------------------------------------
/// instantiate the special characters
static  const CODE U8 anSpecial7Segment[ LEDMAX695_SPCCHAR_MAX ] =
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
 * @function LedMAX695XHandler_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void LedMAX695XHandler_Initialize( void )
{
  U8  nTemp;

  // call the clear
  LedMAX695XHandler_Clear( );

  // set the sacn limit/intensity
  nTemp = 0x03;
  LedMAX695XHandler_WriteData( REG_ADDR_SCANLIM, &nTemp, 1 );
  nTemp = 0x40;
  LedMAX695XHandler_WriteData( REG_ADDR_INTEN, &nTemp, 1 );
}

/******************************************************************************
 * @function LedMAX695XHandler_Clear
 *
 * @brief clear the display
 *
 * This function will clear the display
 *
 *****************************************************************************/
void LedMAX695XHandler_Clear( void )
{
  REGCONFIG tConfig;
  
  // fill the command
  tConfig.nByte = 0;
  tConfig.tFields.bShutdownDsb  = ON;
  tConfig.tFields.bClearDisplay = ON;
  
  // write it
  LedMAX695XHandler_WriteData( REG_ADDR_CONFIG, &tConfig.nByte, REGCONFIG_SIZE );
}

/******************************************************************************
 * @function LedMAX695XHandler_SetBrightness 
 *
 * @brief set the brightness
 *
 * This function will set the display brightness
 *
 * @param[in]   eBrightness   selected brightness
 *
 *****************************************************************************/
void LedMAX695XHandler_SetBrightness( U8 nBrightness )
{
  // write it
  LedMAX695XHandler_WriteData( REG_ADDR_INTEN, &nBrightness, 1 );
}

/******************************************************************************
 * @function LedMAX695XHandler_DisplayChar
 *
 * @brief display a character
 *
 * This function will display a character
 *
 * @param[in]   nDigit      digit location
 * @param[in]   nCharacter  nCharacter
 * @param[in]   bDecimal    enable decimal
 *
 *****************************************************************************/
void LedMAX695XHandler_DisplayChar( U8 nDigit, U8 nCharacter, BOOL bDecimal )
{
  U8  nBitmap, nRegAddr, nDecodeMask, nDecimalMask;

  // read the current decode/decimal mask
  LedMAX695XHandler_ReadData( REG_ADDR_DECMODE, &nDecodeMask, 1 );
  LedMAX695XHandler_ReadData( REG_ADDR_SEGMENT, &nDecimalMask, 1 );

  // set the register address
  nRegAddr = REG_ADDR_DIGIT0 + nDigit;

  // check for a special character
  if ( nCharacter & SPECIAL_CHAR_MASK )
  {
    // get the special character
    nBitmap = PGM_RDBYTE( anSpecial7Segment[ nCharacter & 0x7F ] );

    // clear the decode mask
    nDecodeMask &= ~( BIT( nDigit ));
  }
  else
  {
    // set the bit map
    nBitmap = nCharacter & 0x7F;
    nDecodeMask |= BIT( nDigit );
  }
  
  // check for decimal
  if ( bDecimal )
  {
    // set the decimal
    nDecimalMask |= BIT( nDigit );
  }
  else
  {
    // clear the decimal
    nDecimalMask &= ~( BIT( nDigit ));
  }

  // write the decode mask/character
  LedMAX695XHandler_WriteData( REG_ADDR_DECMODE, &nDecodeMask, 1 );
  LedMAX695XHandler_WriteData( REG_ADDR_SEGMENT, &nDecimalMask, 1 );
  LedMAX695XHandler_WriteData( nRegAddr, &nBitmap, 1 );
}

/******************************************************************************
 * @function LedMAX695XHandler_DisplayNumber
 *
 * @brief display a number
 *
 * This function will display a number
 *
 * @param[in]   nBase             base
 * @param[in]   uValue            value to display
 * @param[in]   nDecimalPointLoc  decimal point location
 *
 *****************************************************************************/
void LedMAX695XHandler_DisplayNumber( U8 nBase, U32 uValue, U8 nDecimalPointLoc )
{
  U8  nCurValue, nCurDigit, nDecMask;

  // check for valid number
  if ((( 10 == nBase ) && ( uValue <= MAX_NUMBER_BASE_TEN )) ||
      (( 16 == nBase ) && ( uValue <= MAX_NUMBER_BASE_HEX )))
  {
    // set the digit to lsb
    nCurDigit = MAX_NUM_DIGITS;
    nDecMask = 0;

    // loop while 
    while ( uValue > 0 )
    {
      // get the current character value
      nCurValue = uValue % nBase;

      // display the digit/decode
      anLclBuffer[ nCurDigit - 1 ] = nCurValue;
      nDecMask |= BIT(( nCurDigit = 1 )); 

      // adjust value
      uValue /= nBase;

      // update the digit
      nCurDigit--;
    }

    // clear rest
    while( nCurDigit > 0 )
    {
      // set 0
      anLclBuffer[ nCurDigit ] = 0;

      // decrement the digit
      nCurDigit--;
    }
  }
  else
  {
    // for each character 
    for ( nCurDigit = 0; nCurDigit < MAX_NUM_DIGITS; nCurDigit++ )
    {
      // turn on segment g
      anLclBuffer[ nCurDigit ] = 1;
    }
  }

  // output the decode mask
  LedMAX695XHandler_WriteData( REG_ADDR_DECMODE, &nDecMask, 1 );

  // now output the buffr
  LedMAX695XHandler_WriteData( REG_ADDR_DIGIT0, anLclBuffer, MAX_NUM_DIGITS );
}

/**@} EOF LedMAX695XHandler.c */
