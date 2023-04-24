/******************************************************************************
 * @file LcdSSD1306.h
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
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
#ifndef _LCDSSD1306_H
#define _LCDSSD1306_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LcdSSD1306/LcdSSD1306_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the actions
typedef enum _LCD_SSD1306DISPLAYPIXACT
{
  LCDSSD1306_DISPLAY_PIXACT_CLR = 0,
  LCDSSD1306_DISPLAY_PIXACT_SET,
  LCDSSD1306_DISPLAY_PIXACT_TGL
} LCDSSD1306DISPLAYPIXACT;

/// enumerate the scroll operations
typedef enum _LCDSSD1306DDISPLAYSCROLL
{
  LCDSSD1306_DISPLAY_SCROLL_STOP = 0,
  LCDSSD1306_DISPLAY_SCROLL_RIGHT,
  LCDSSD1306_DISPLAY_SCROLL_LEFT,
  LCDSSD1306_DISPLAY_SCROLL_DIAGRIGHT,
  LCDSSD1306_DISPLAY_SCROLL_DIAGLIFT,
  LCDSSD1306_DISPLAY_SCROLL_MAX
} LCDSSD1306DISPLAYSCROLL;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  LcdSSD1306_Initialize( void );
extern  void  LcdSSD1306_SetDim( BOOL bState );
extern  void  LcdSSD1306_ClearScreen( void );
extern  void  LcdSSD1306_SetPixel( U8 nX, U8 nY, LCDSSD1306DISPLAYPIXACT eAction );
extern  void  LcdSSD1306_WriteBuffer( void );
extern  void  LcdSSD1306_SetScroll( LCDSSD1306DISPLAYSCROLL eScrollType, U8 nStart, U8 nStop );

/**@} EOF LcdSSD1306.h */

#endif  // _LCDSSD1306_H