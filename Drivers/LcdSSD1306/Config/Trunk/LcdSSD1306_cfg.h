/******************************************************************************
 * @file LcdSSD1306_cfg.h
 *
 * @brief LcdSSD1306 configuration declarations
 *
 * This file provides the declrationsfor the SSD1306
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
 * \addtogroup LcdSSD1306
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LCDSSD1306_CFG_H
#define _LCDSSD1306_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the display types
#define LCDSSD1306_TYPE_128_64                  ( 0 )
#define LCDSSD1306_TYPE_128_32                  ( 1 )
#define LCDSSD1306_TYPE_96_16                   ( 2 )

/// define the display type selection
#define LCDSSD1306_TYPE_SELECT                  ( LCDSSD1306_TYPE_128_32 )

/// determine the display width/height
#if ( LCDSSD1306_TYPE_SELECT == LCDSSD1306_TYPE_128_64 )
  #define LCDSSD1306_DISPLAY_WIDTH              ( 128 )
  #define LCDSSD1306_DISPLAY_HEIGHT             ( 64 )
#elif ( LCDSSD1306_TYPE_SELECT == LCDSSD1306_TYPE_128_32 )
  #define LCDSSD1306_DISPLAY_WIDTH              ( 128 )
  #define LCDSSD1306_DISPLAY_HEIGHT             ( 32 )
#elif ( LCDSDD1306_TYPE_SELECT == LCDSSD1306_TYPE_96_16 )
  #define LCDSSD1306_DISPLAY_WIDTH              ( 96 )
  #define LCDSSD1306_DISPLAY_HEIGHT             ( 16 )
#else
` #error "LCDSSD1306_TYPE_SELECT incorrectly set" 
#endif // LCDSSD1306_TYPE_SELECT

/// define the device address for I2C 
#define SSD1306_DEVICE_ADDRESS                  ( 0x3C )

/// define the VCC TYPES
#define LCDSSD1306_VCC_EXTRNAL                  ( 0 )
#define LCDSSD1306_VCC_SWITCHCAP                ( 1 )

/// define the VCC type selection
#define LCDSSD1306_VCC_SELECT                   ( LCDSSD1306_VCC_EXTERNAL )

/// define the interface type
#define LDCSSD1306_INTERFACE_SPI                ( 0 )
#define LCDSSD1306_INTERFACE_I2C                ( 1 )

/// define the interface selection
#define LCDSSD1306_INTERFACE_SELECTED           ( LCDSSD1306_INTERFACE_SPI )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  LcdSSD1306_LocalInitialize( void );
extern  void  LcdSSD1306_LocalReset( void );
extern  void  LcdSSD1306_LocalWriteBuffer( PU8 pNData, U16 wLength );
extern  void  LcdSSD1306_LocalWriteCommand( U8 nValue );
extern  void  LcdSSD1306_LocalWriteData( U8 nValue );

/**@} EOF LcdSSD1306_cfg.h */

#endif  // _LCDSSD1306_CFG_H