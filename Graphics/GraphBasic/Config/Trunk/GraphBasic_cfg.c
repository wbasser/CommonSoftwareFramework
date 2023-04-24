/******************************************************************************
 * @file GraphBasic_cfg.c
 *
 * @brief graph basic configuration implementaion 
 *
 * This file provides the implementation for the graph basic
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
 * \addtogroup GraphBasic_cfg
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GraphBasic/GraphBasic_cfg.h"

// library includes -----------------------------------------------------------
#include "GraphFont5x7/GraphFont5x7.h"
#include "LcdSSD1306/LcdSSD1306.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
static  const LCDSSD1306DISPLAYPIXACT aeActions[ GRAPHBASIC_PIXACT_MAX ] =
{
  LCDSSD1306_DISPLAY_PIXACT_CLR,
  LCDSSD1306_DISPLAY_PIXACT_SET,
  LCDSSD1306_DISPLAY_PIXACT_TGL
};

/******************************************************************************
 * @function GraphBasic_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any needed initilaization, primarily that of the 
 * basic display device driver
 *
 * @ return TRUE if errors, FALSE if not
 * 
 *****************************************************************************/
BOOL GraphBasic_LocalInitialize( void )
{
  BOOL bStatus;

  // initialize the display
  bStatus = LcdSSD1306_Initialize( );

  return( bStatus );
}

/******************************************************************************
 * @function GraphBasic_RefreshScreen
 *
 * @brief refresh screen
 *
 * This function will perform a screen refresh
 *
 *****************************************************************************/
void GraphBasic_RefreshScreen( void )
{
  // refresh the screen
  LcdSSD1306_WriteBuffer( );
}

/******************************************************************************
 * @function GraphBasic_GetMaxX
 *
 * @brief get the maximum X value
 *
 * This function will return the maximum X value
 *
 * @return      the maximum X value
 *
 *****************************************************************************/
U16 GraphBasic_GetMaxX( void )
{
  // return the width
  return( LCDSSD1306_DISPLAY_WIDTH );
}

/******************************************************************************
 * @function GraphBasic_GetMaxX
 *
 * @brief get the maximum X value
 *
 * This function will return the maximum X value
 *
 * @return      the maximum X value
 *
 *****************************************************************************/
U16 GraphBasic_GetMaxY( void )
{
  // return the height
  return( LCDSSD1306_DISPLAY_HEIGHT );
}

/******************************************************************************
 * @function GraphBasic_GetFontX
 *
 * @brief get the font x size
 *
 * This function will return the font x size
 *
 * @return      the font x size
 *
 *****************************************************************************/
U16 GraphBasic_GetFontX( void )
{
  // return the font width
  return( GRAPHICFONT_X_SIZE );
}


/******************************************************************************
 * @function GraphBasic_PixelDraw
 *
 * @brief draw a pixel
 *
 * This function will draw pixel
 *
 * @param[in]   wX        X location
 * @param[in]   wY        Y location
 * @param[in]   eAction   pixel action
 * @Param[in]   eColor    pixel color
 *
 *****************************************************************************/
void GraphBasic_PixelDraw( U16 wX, U16 wY, GRAPHBASICPIXACT eAction, GRAPHBASICCLR eColor )
{
  // call the pixel draw for the display
  LcdSSD1306_SetPixel(( U8 )wX, ( U8 )wY, aeActions[ eAction ] );
}


/**@} EOF GraphBasic_cfg.c */
