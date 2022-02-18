/******************************************************************************
 * @file LcdSSD1306_cfg.c
 *
 * @brief Display SSD1306 configuration implementation
 *
 * This file provides the implementation for the confiuratioation
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LcdSSD1306/LcdSSD1306.h"

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  WriteValue( U8 nAddress, U8 nCmd, U8 nValue );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function LcdSSD1306_LocalInitialize
 *
 * @brief initialization
 *
 * This function will perform any needed local initialization
 *
 * @ return TRUE if errors, FALSE if not
 * 
*****************************************************************************/
BOOL LcdSSD1306_LocalInitialize( void )
{
  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function LcdSSD1306_LocalReset
 *
 * @brief reset
 *
 * This function will perform a chip reset if enabled
 *
 *****************************************************************************/
void LcdSSD1306_LocalReset( void )
{
}

/******************************************************************************
 * @function LcdSSD1306_LocalWriteBuffer
 *
 * @brief write a buffer of data
 *
 * This function will write the buffer of data
 *
 * @param[in]   pnData    pointer to the data
 * @param[in]   wLength   length of the data
 *
 * @return      
 *
 *****************************************************************************/
void LcdSSD1306_LocalWriteBuffer( U8 nAddress, PU8 pnData, U16 wLength )
{
}

/******************************************************************************
 * @function LcdSSD1306_LocalWriteCommand
 *
 * @brief write a command
 *
 * This function will write a value to the command register
 *
 * @param[in]   nAddress  address of the device
 * @param[in]   nValue    value to write to the command
 *
 *****************************************************************************/
void LcdSSD1306_LocalWriteCommand( U8 nAddress, U8 nValue )
{
  U8  nCommand = 0;
  
  // write the value
  WriteValue( nAddress, nCommand, nValue );
}

/******************************************************************************
 * @function LcdSSD1306_LocalWriteData
 *
 * @brief wrtie data to the display
 *
 * This function will write the data value
 *
 * @param[in]   nAddress  address of the device
 * @param[in]   nValue    value to write to the command
 *
 *****************************************************************************/
void LcdSSD1306_LocalWriteData( U8 nAddress, U8 nValue )
{
  U8  nCommand = 0x40;

  // write the value
  WriteValue( nAddress, nCommand, nValue );
}

/******************************************************************************
 * @function WriteValue
 *
 * @brief write value
 *
 * This function willw rite a value to the display
 *
 * @param[in]   nAddress  address of the device
 * @param[in]   nValue    value to write to the command
 *
 *****************************************************************************/
static void WriteValue( U8 nAddress, U8 nCommand, U8 nValue )
{
  I2CERROR  eI2cErr;

  // fill the transfer control
  I2CXFRCTL tXfrCtl =
  {
    .nDevAddr     = nAddress,
    .nAddrLen     = 1,
    .tAddress     = 
    {
      .anValue[ LE_U16_LSB_IDX ] = nCommand,
    },
    .pnData       = &nValue,
    .wDataLen     = 1,
    .uTimeout     = 5
  };

  // write it
  eI2cErr = I2c_Write( I2C_DEV_ENUM_LCLBUS, &tXfrCtl );
}

/**@} EOF LcdSSD1306_cfg.c */
