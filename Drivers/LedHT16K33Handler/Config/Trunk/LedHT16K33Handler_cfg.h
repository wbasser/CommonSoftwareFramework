/******************************************************************************
 * @file LedHT16K33Handler_cfg.h
 *
 * @brief HT16K33 LED driver configuration declarations
 *
 * This file file provides the declarations for the configuration of the
 * HT16K33 led driver
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
 
// ensure only one instantiation
#ifndef _LEDHT16K33HANDLER_CFG_H
#define _LEDHT16K33HANDLER_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the maximum value for display
#define LEDHT16K33_MAX_NUMBER_BASE_TEN          ( 9999 )
#define LEDHT16K33_MAX_NUMBER_BASE_HEX          ( 0xFFFF )

/// define the base addresses
#define LEDHT16K33_BASE_ADDRESS                 ( 0x70 )

// enumerations ---------------------------------------------------------------
/// define the enumerationt
typedef enum _LEDHT16K33ENUM
{
  // enumerate the displays here
  LEDHT16K33_ENUM_VOLT = 0,

  // do not remove below
  LEDHT16K33_ENUM_MAX
} LEDHT16K33ENUM;

/// enumerate the character position offsets
typedef enum _LEDHT16K33OFFSET
{
  LEDHT16K33_OFFSET_0 = 0,
  LEDHT16K33_OFFSET_1,
  LEDHT16K33_OFFSET_2,
  LEDHT16K33_OFFSET_3,
  LEDHT16K33_OFFSET_COLON,
  LEDHT16K33_OFFSET_MAX,
} LEDHT16K33OFFSET;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  U8  g_anLedHT16K33CharOffset[ ];

// global function prototypes --------------------------------------------------
extern  BOOL  LedHT16K33Handler_LocalInitialize( void );
extern  BOOL  LedHT16K33Handler_WriteData( U8 nDevice, U8 nCmd, PU8 pnData, U8 nLength );

/**@} EOF LedHT16K33Handler_cfg.h */

#endif  // _LEDHT16K33HANDLER_CFG_H