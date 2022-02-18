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
/// define the number of actual digits
#define LEDHT16K33_NUM_DIGITS                   ( 4 )

/// define the maximum value for display
#define LEDHT16K33_MAX_NUMBER_BASE_TEN          ( 9999 )
#define LEDHT16K33_MAX_NUMBER_BASE_HEX          ( 0xFFFF )

/// define the base address
#define LEDHT16K33_BASE_ADDRESS                 ( 0x70 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  LedHT16K33Handler_LocalInitialize( void );
extern  BOOL  LedHT16K33Handler_WriteData( U8 nCmd, PU8 pnData, U8 nLength );

/**@} EOF LedHT16K33Handler_cfg.h */

#endif  // _LEDHT16K33HANDLER_CFG_H