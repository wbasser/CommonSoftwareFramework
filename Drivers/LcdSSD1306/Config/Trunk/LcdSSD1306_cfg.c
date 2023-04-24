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
#if ( LCDSSD1306_INTERFACE_SELECTED == LCDSSD1306_INTERFACE_SPI )
  #include "GPIO/Gpio.h"
  #include "SPI/Spi.h"
#elif ( LCDSSD1306_INTERFACE_SELECTED == LCDSSD1306_INTERFACE_I2C )
  #include "I2C/I2c.h"
#endif

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the command data destination
typedef enum _CMDDATSEL
{
  CMD_DAT_CMD = 0,
  CMD_DAT_DAT,
} CMDDATSEL;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  void  WriteValue( CMDDATSEL eDst, U8 nValue );

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
void LcdSSD1306_LocalWriteBuffer( PU8 pnData, U16 wLength )
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
void LcdSSD1306_LocalWriteCommand( U8 nValue )
{
  // write the value
  WriteValue( CMD_DAT_CMD, nValue );
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
void LcdSSD1306_LocalWriteData( U8 nValue )
{
  // write the value
  WriteValue( CMD_DAT_DAT, nValue );
}

/******************************************************************************
 * @function WriteValue
 *
 * @brief write value
 *
 * This function willw rite a value to the display
 *
 * @param[in]   eDst      destination
 * @param[in]   nValue    value to write to the command
 *
 *****************************************************************************/
static void WriteValue( CMDDATSEL eDst, U8 nValue )
{
  #if ( LCDSSD1306_INTERFACE_SELECTED == LCDSSD1306_INTERFACE_SPI )
    // enable the chip select pin
    Gpio_Set( GPIO_PIN_ENUM_LCDCEN, ON );
    
    // select the destination pin
    Gpio_Set( GPIO_PIN_ENUM_LCDCDT, eDst );
    
    // write the data
    Spi_Write( SPI_DEV_ENUM_LCLBUS, nValue );
    
    // deselect it
    Gpio_Set( GPIO_PIN_ENUM_LCDCEN, OFF );
  #elif ( LCDSSD1306_INTERFACE_SELECTED == LCDSSD1306_INTERFACE_I2C )
  #endif
}

/**@} EOF LcdSSD1306_cfg.c */
