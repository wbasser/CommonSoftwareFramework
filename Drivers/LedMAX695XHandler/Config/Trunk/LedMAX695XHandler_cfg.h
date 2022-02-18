/******************************************************************************
 * @file LedMAX695XHandler_cfg.h
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
 * \addtogroup LedMAX695XHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LEDMAX695XHANDLER_CFG_H
#define _LEDMAX695XHANDLER_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the base address
#define LEDMAX695X_BASE_ADDRESS                 ( 0x70 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  LedMAX695XHandler_LocalInitialize( void );
extern  BOOL  LedMAX695XHandler_WriteData( U8 nCmd, PU8 pnData, U8 nLength );
extern  BOOL  LedMAX695XHandler_ReadData( U8 nCmd, PU8 pnData, U8 nLength );

/**@} EOF LedMAX695XHandler_cfg.h */

#endif  // _LEDMAX695XHANDLER_CFG_H