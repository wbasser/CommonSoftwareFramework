/******************************************************************************
 * @file LedMAX7219Handler.c
 *
 * @brief MAX7219 led driver implementation
 *
 * This file provides the implementation for the MAX7219 display driver
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedMAX7219Handler/LedMAX7219Handler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
// define the data/address indices
#define DATA_IDX                  ( LE_U16_LSB_IDX )
#define ADDR_IDX                  ( LE_U16_MSB_IDX )

// enumerations ---------------------------------------------------------------
/// enumerate the registers
typedef enum _REGADDR
{
  REG_ADDR_NOP = 0,
  REG_ADDR_DIG0,
  REG_ADDR_DIG1,
  REG_ADDR_DIG2,
  REG_ADDR_DIG3,
  REG_ADDR_DIG4,
  REG_ADDR_DIG5,
  REG_ADDR_DIG6,
  REG_ADDR_DIG7,
  REG_ADDR_MODE,
  REG_ADDR_INTN,
  REG_ADDR_SCNL,
  REG_ADDR_SHDN,
  REG_ADDR_TEST = 0xF
} REGADDR;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function LedMAX7219Handler_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed iniyialization
 *
 *****************************************************************************/
 void LedMAX7219Handler_Initialize( void )
 {
  U16UN  tCommand;

  // set the decode mode
  tCommand.anValue[ DATA_IDX ] = BIT(( LEDMAX7219HANDLER_NUM_DIGITS - 1 )) - 1;
  tCommand.anValue[ ADDR_IDX ] = REG_ADDR_MODE;
  LedMAX72119Handler_WriteData( tCommand );

  // set the scan limit
  tCommand.anValue[ DATA_IDX ] = ( LEDMAX7219HANDLER_NUM_DIGITS - 1 );
  tCommand.anValue[ ADDR_IDX ] = REG_ADDR_SCNL;
  LedMAX72119Handler_WriteData( tCommand );

  // set the intensity to mid
  tCommand.anValue[ DATA_IDX ] = LEDMAX7219_INTENSITY_8;
  tCommand.anValue[ ADDR_IDX ] = REG_ADDR_INTN;
  LedMAX72119Handler_WriteData( tCommand );

  // turn it on
  tCommand.anValue[ DATA_IDX ] = ON;
  tCommand.anValue[ ADDR_IDX ] = REG_ADDR_SHDN;
  LedMAX72119Handler_WriteData( tCommand );
 }

/******************************************************************************
 * @function LedMAX7219Handler_DisplayDigit
 *
 * @brief set a digit to the given value
 *
 * This function will set a particular digit to a value
 *
 * @param[in]   nDigitSel         digit selection
 * @param[in]   nValue            value to display
 *
 *****************************************************************************/
 void LedMAX7219Handler_DisplayDigit( U8 nDigitSel, U8 nValue )
 {
  U16UN tCommand;

  // fill the data/address
  tCommand.anValue[ DATA_IDX ] = nValue;
  tCommand.anValue[ ADDR_IDX ] = nDigitSel + 1;
  LedMAX72119Handler_WriteData( tCommand );
 }

/******************************************************************************
 * @function LedMAX7219Handler_DisplayDigits
 *
 * @brief display some number of digits
 *
 * This function will display some number of digits
 *
 * @param[in]   pnDigitValue
 *
 *****************************************************************************/
void LedMAX7219Handler_DisplayDigits( PU8 pnDigitValue )
{
  U16UN tCommand;
  U8    nIdx;

  // for each digit
  for ( nIdx = 0; nIdx < LEDMAX7219HANDLER_NUM_DIGITS; nIdx++ )
  {
    // set the register/data
    tCommand.anValue[ DATA_IDX ] = *( pnDigitValue + nIdx );
    tCommand.anValue[ ADDR_IDX ] = REG_ADDR_DIG0 + nIdx;
    LedMAX72119Handler_WriteData( tCommand );
  }
}

/**@} EOF LedMAX7219Handler.c */
