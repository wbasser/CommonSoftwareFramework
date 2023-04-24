/******************************************************************************
 * @file LcdSSD1306.c
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LcdSSD1306/LcdSSD1306.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// determine the display type/set various options
#if ( LCDSSD1306_TYPE_SELECT == LCDSSD1306_TYPE_128_64 )
  #define DISPLAY_END_PAGE                          ( 7 )
  #define SSD1306_MULTIPLEX_VALUE                   ( 0x3F )
  #define SSD1306_COMPINS_VALUE                     ( 0x12 )
  #if ( LCDSSD1306_VCC_SELECT == LCDSSD1306_VCC_EXTRNAL )
    #define SSD1306_CONTRAST_VALUE                  ( 0x9F )
    #define SSD1306_VCC_VALUE                       ( 0x10 )
    #define SSD1306_PRECHARGE_VALUE                 ( 0x22 )
  #else
    #define SSD1306_CONTRAST_VALUE                  ( 0xCF )
    #define SSD1306_VCC_VALUE                       ( 0x14 )
    #define SSD1306_PRECHARE_VALUE                  ( 0xF1 )
  #endif // LCDSSD1306_VCC_SELECT
#elif ( LCDSSD1306_TYPE_SELECT == LCDSSD1306_TYPE_128_32 )
  #define DISPLAY_END_PAGE                          ( 3 )
  #define SSD1306_MULTIPLEX_VALUE                   ( 0x3F )
  #define SSD1306_COMPINS_VALUE                     ( 0x02 )
  #if ( LCDSSD1306_VCC_SELECT == LCDSSD1306_VCC_EXTRNAL )
    #define SSD1306_CONTRAST_VALUE                  ( 0x8F )
    #define SSD1306_VCC_VALUE                       ( 0x10 )
    #define SSD1306_PRECHARGE_VALUE                 ( 0x22 )
  #else
    #define SSD1306_CONTRAST_VALUE                  ( 0xCF )
    #define SSD1306_VCC_VALUE                       ( 0x14 )
    #define SSD1306_PRECHARE_VALUE                  ( 0xF1 )
  #endif // LCDSSD1306_VCC_SELECT
#elif ( LCDSSD1306_TYPE_SELECT == LCDSSD1306_TYPE_96_16 )
  #define DISPLAY_END_PAGE                          ( 1 )
  #define SSD1306_MULTIPLEX_VALUE                   ( 0x0F )
  #define SSD1306_COMPINS_VALUE                     ( 0x02 )
  #if ( LCDSSD1306_VCC_SELECT == LCDSSD1306_VCC_EXTRNAL )
    #define SSD1306_CONTRAST_VALUE                  ( 0x10 )
    #define SSD1306_VCC_VALUE                       ( 0x10 )
    #define SSD1306_PRECHARGE_VALUE                 ( 0x22 )
  #else
    #define SSD1306_CONTRAST_VALUE                  ( 0xAF )
    #define SSD1306_VCC_VALUE                       ( 0x14 )
    #define SSD1306_PRECHARE_VALUE                  ( 0xF1 )
  #endif // LCDSSD1306_VCC_SELECT
#else
` #error "LCDSSD1306_TYPE_SELECT incorrectly set" 
#endif // LCDSSD1306_TYPE_SELECT

/// define the command
#define SSD1306_SETCONTRAST                       ( 0x81 )
#define SSD1306_LCDSSD1306_DISPLAY_ALLON_RESUME   ( 0xA4 )
#define SSD1306_LCDSSD1306_DISPLAY_ALLON          ( 0xA5 )
#define SSD1306_NORMALLCDSSD1306_DISPLAY_         ( 0xA6 )
#define SSD1306_INVERTLCDSSD1306_DISPLAY_         ( 0xA7 )
#define SSD1306_LCDSSD1306_DISPLAY_OFF            ( 0xAE )
#define SSD1306_LCDSSD1306_DISPLAY_ON             ( 0xAF )
#define SSD1306_SETLCDSSD1306_DISPLAY_OFFSET      ( 0xD3 )
#define SSD1306_SETCOMPINS                        ( 0xDA )
#define SSD1306_SETVCOMDETECT                     ( 0xDB )
#define SSD1306_SETLCDSSD1306_DISPLAY_CLOCKDIV    ( 0xD5 )
#define SSD1306_SETPRECHARGE                      ( 0xD9 )
#define SSD1306_SETMULTIPLEX                      ( 0xA8 )
#define SSD1306_SETLOWCOLUMN                      ( 0x00 )
#define SSD1306_SETHIGHCOLUMN                     ( 0x10 )
#define SSD1306_SETSTARTLINE                      ( 0x40 )
#define SSD1306_MEMORYMODE                        ( 0x20 )
#define SSD1306_COLUMNADDR                        ( 0x21 )
#define SSD1306_PAGEADDR                          ( 0x22 )
#define SSD1306_COMSCANINC                        ( 0xC0 )
#define SSD1306_COMSCANDEC                        ( 0xC8 )
#define SSD1306_SEGREMAP                          ( 0xA0 )
#define SSD1306_CHARGEPUMP                        ( 0x8D )
#define SSD1306_EXTERNALVCC                       ( 0x01 )
#define SSD1306_SWITCHCAPVCC                      ( 0x02 )
#define SSD1306_ACTIVATE_SCROLL                   ( 0x2F )
#define SSD1306_DEACTIVATE_SCROLL                 ( 0x2E )
#define SSD1306_SET_VERTICAL_SCROLL_AREA          ( 0xA3 )
#define SSD1306_RIGHT_HORIZONTAL_SCROLL           ( 0x26 )
#define SSD1306_LEFT_HORIZONTAL_SCROLL            ( 0x27 )
#define SSD1306_VERTICAL_RIGHT_HORIZONTAL_SCROLL  ( 0x29 )
#define SSD1306_VERTICAL_LEFT_HORIZONTAL_SCROLL   ( 0x2A )

/// define the clock divide ratio
#define SSD1306_CLKDIV_RATIO                      ( 0x80 )

/// define the buffer size
#define BUFFER_SIZE                               (( LCDSSD1306_DISPLAY_HEIGHT * LCDSSD1306_DISPLAY_WIDTH ) / 8 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8  anBuffer[ BUFFER_SIZE ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
static  const U8    anInitializationSequence[ ] =
{
  SSD1306_LCDSSD1306_DISPLAY_OFF,
  SSD1306_SETLCDSSD1306_DISPLAY_CLOCKDIV,
  SSD1306_CLKDIV_RATIO,
  SSD1306_SETMULTIPLEX,
  SSD1306_MULTIPLEX_VALUE,
  SSD1306_SETLCDSSD1306_DISPLAY_OFFSET,
  0,
  SSD1306_SETSTARTLINE,
  SSD1306_CHARGEPUMP,
  SSD1306_VCC_VALUE,
  SSD1306_MEMORYMODE,
  0x00,
  SSD1306_SEGREMAP | 0x1,
  SSD1306_COMSCANDEC,
  SSD1306_SETCOMPINS,
  SSD1306_COMPINS_VALUE,
  SSD1306_SETCONTRAST,
  SSD1306_CONTRAST_VALUE,
  SSD1306_SETPRECHARGE,
  SSD1306_PRECHARGE_VALUE,
  SSD1306_SETVCOMDETECT,
  0x40,
  SSD1306_LCDSSD1306_DISPLAY_ALLON_RESUME,
  SSD1306_NORMALLCDSSD1306_DISPLAY_,
  SSD1306_LCDSSD1306_DISPLAY_ON
};

/******************************************************************************
 * @function LcdSSD1306_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the display
 *
 * @return      TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL LcdSSD1306_Initialize( void )
{
  BOOL  bStatus;
  U8    nIdx;
  
  // call the local initialization
  bStatus = LcdSSD1306_LocalInitialize( );

  // reset the chip
  LcdSSD1306_LocalReset( );
  
  // for each byte in the initialization sequence
  for ( nIdx = 0; nIdx < sizeof( anInitializationSequence ); nIdx++ )
  {
    // write the command 
    bStatus |= LcdSSD1306_LocalWriteCommand( anInitializationSequence[ nIdx ] );
  }

  // return status
  return( bStatus );
}

/******************************************************************************
 * @function LcdSSD1306_SetDim
 *
 * @brief dim or set the brightness
 *
 * This function will set the brightness of the display
 *
 * @param[in]   bBright   
 *
 *****************************************************************************/
void LcdSSD1306_SetDim( BOOL bState )
{
  U8  nContrastValue;
  
  // check dim/bright
  nContrastValue = ( bState ) ? SSD1306_CONTRAST_VALUE : 0;
  
  // now output the value
  LcdSSD1306_LocalWriteCommand( SSD1306_SETCONTRAST );
  LcdSSD1306_LocalWriteCommand( nContrastValue );
}

/******************************************************************************
 * @function LcdSSD1306_ClearScreen
 *
 * @brief clear the screen
 *
 * This function will erase the screen
 *
 *****************************************************************************/
void LcdSSD1306_ClearScreen( void )
{
  // clear the display
  memset( anBuffer, 0, BUFFER_SIZE );
  
  // now write the buffer
  LcdSSD1306_WriteBuffer(  );
}

/******************************************************************************
 * @function LcdSSD1306_SetPixel
 *
 * @brief set a pixel in the display
 *
 * This function will in the local buffer
 *
 * @param[in]   nX        x location
 * @param[in]   nY        y location
 * @param[in]   eAction   pixel action, clear, set, toggle
 *
 *****************************************************************************/
void LcdSSD1306_SetPixel( U8 nX, U8 nY, LCDSSD1306DISPLAYPIXACT eAction )
{
  U16 wOffset;
  U8  nMask;
  
  // check for valid x/y locations
  if (( nX < LCDSSD1306_DISPLAY_WIDTH ) && ( nY < LCDSSD1306_DISPLAY_HEIGHT ))
  {
    // determine if we are rotated
    
    // calculate the offset/mask
    wOffset = nX + ( nY / 8 ) * LCDSSD1306_DISPLAY_WIDTH;
    nMask = BIT( nY & 0x07 );
    
    // determine the action
    switch( eAction )
    {
      case LCDSSD1306_DISPLAY_PIXACT_CLR :
        anBuffer[ wOffset ]  &= ~( nMask );
        break;
        
      case LCDSSD1306_DISPLAY_PIXACT_SET :
        anBuffer[ wOffset ] |= nMask;
        break;
        
      case LCDSSD1306_DISPLAY_PIXACT_TGL :
        anBuffer[ wOffset ] ^= nMask;
        break;
        
      default :
        break;
    }
  }
}

/******************************************************************************
 * @function LcdSSD1306_SetColData
 *
 * @brief sets a columns data
 *
 * This function will write the column data at a given page, column
 *
 * @param[in]   nPage     page
 * @param[in]   nCol      column
 * @param[in]   nData     data
 * @param[in]   eAction   pixel action, clear, set, toggle
 *
 *****************************************************************************/
void LcdSSD1306_SetColData( U8 nPage, U8 nCol, U8 nData, LCDSSD1306DISPLAYPIXACT eAction )
{
  U16 wOffset;

  // test for valid page/column
  if (( nPage < DISPLAY_END_PAGE ) && ( nCol < LCDSSD1306_DISPLAY_WIDTH )) 
  {
    // compute buffer index
    wOffset = ( nPage * LCDSSD1306_DISPLAY_WIDTH ) + nCol;
    
    // determine the action
    switch( eAction )
    {
      case LCDSSD1306_DISPLAY_PIXACT_CLR :
        anBuffer[ wOffset ] &= ~( nData );
        break;
        
      case LCDSSD1306_DISPLAY_PIXACT_SET :
        anBuffer[ wOffset ] |= nData;
        break;
        
      case LCDSSD1306_DISPLAY_PIXACT_TGL :
        anBuffer[ wOffset ] ^= nData;
        break;
        
      default :
        break;
    }
  }
}

/******************************************************************************
 * @function LcdSSD1306_WriteBuffer
 *
 * @brief outputs the buffer to the dislay
 *
 * This function will update the display with the modified buffer
 *
 *****************************************************************************/
void LcdSSD1306_WriteBuffer( void )
{
  // set the column start address/end addresses/page start/end addresses
  LcdSSD1306_LocalWriteCommand( SSD1306_COLUMNADDR );
  LcdSSD1306_LocalWriteCommand( 0 );
  LcdSSD1306_LocalWriteCommand( LCDSSD1306_DISPLAY_WIDTH - 1 );
  LcdSSD1306_LocalWriteCommand( SSD1306_PAGEADDR );
  LcdSSD1306_LocalWriteCommand( 0 );
  LcdSSD1306_LocalWriteCommand( DISPLAY_END_PAGE );

  // output the buffer
  LcdSSD1306_LocalWriteBuffer( anBuffer, BUFFER_SIZE );
}

/******************************************************************************
 * @function LcdSSD1306_SetScroll
 *
 * @brief scroll the display
 *
 * This function will scroll the display
 *
 * @param[in]   eScrollType     type of scroll
 * @param[in]   nStart          start location
 * @param[in]   nStop           Stop Location
 *
 *****************************************************************************/
void LcdSSD1306_SetScroll( LCDSSD1306DISPLAYSCROLL eScrollType, U8 nStart, U8 nStop )
{
  U8  anTempBuffer[ 10 ];
  U8  nBufferIdx = 0;
  U8  nIdx;

  // determine the scroll
  switch( eScrollType )
  {
    case LCDSSD1306_DISPLAY_SCROLL_STOP :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_DEACTIVATE_SCROLL;
      break;
      
    case LCDSSD1306_DISPLAY_SCROLL_RIGHT :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_RIGHT_HORIZONTAL_SCROLL;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStart;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStop;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = 0xFF;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_ACTIVATE_SCROLL;
      break;
      
    case LCDSSD1306_DISPLAY_SCROLL_LEFT :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_LEFT_HORIZONTAL_SCROLL;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStart;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStop;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = 0xFF;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_ACTIVATE_SCROLL;
      break;
      
    case LCDSSD1306_DISPLAY_SCROLL_DIAGRIGHT :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_SET_VERTICAL_SCROLL_AREA;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = LCDSSD1306_DISPLAY_HEIGHT;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_VERTICAL_RIGHT_HORIZONTAL_SCROLL;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStart;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStop;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = 1;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_ACTIVATE_SCROLL;
      break;
      
    case LCDSSD1306_DISPLAY_SCROLL_DIAGLIFT :
      anTempBuffer[ nBufferIdx++ ] = SSD1306_SET_VERTICAL_SCROLL_AREA;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = LCDSSD1306_DISPLAY_HEIGHT;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_VERTICAL_LEFT_HORIZONTAL_SCROLL;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStart;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = nStop;
      anTempBuffer[ nBufferIdx++ ] = 0;
      anTempBuffer[ nBufferIdx++ ] = 1;
      anTempBuffer[ nBufferIdx++ ] = SSD1306_ACTIVATE_SCROLL;
      break;
      
    default :
      break;
  }
  
  // now output the buffer
  for ( nIdx = 0; nIdx < nBufferIdx; nIdx++ )
  {
    // write the buffer
    LcdSSD1306_LocalWriteCommand( anTempBuffer[ nIdx ] );
  }
}

/**@} EOF LcdSSD1306.c */
 