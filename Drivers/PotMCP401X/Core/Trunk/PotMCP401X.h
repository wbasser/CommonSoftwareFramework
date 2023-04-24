/******************************************************************************
 * @file PotMCP401X.h
 *
 * @brief Potentiometer MCP401X declaraltions
 *
 * This file provides the declarations for the MCP401X potentiometer
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
 * \addtogroup PotMCP401X
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _POTMCP401X_H
#define _POTMCP401X_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PotMCP401X/PotMCP401X_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _POTMCP401XERR
{
	POTMCP401X_ERR_NONE = 0,
	POTMCP401X_ERR_ILLENUM,
	POTMCP401X_ERR_ILLVAL,
} POTMCP401XERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	BOOL					PotMCP401X_Initialize( void );
extern	POTMCP401XERR	PotMCP401X_SetPosition( POTMCP401XENUM ePotEnum, U8 nPosition );
extern	POTMCP401XERR	PotMCP401X_IncrementDecrement( POTMCP401XENUM ePotEnum, C8 cDelta );

/**@} EOF PotMCP401X.h */

#endif  // _POTMCP401X_H