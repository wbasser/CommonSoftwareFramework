/******************************************************************************
 * @file PotMCP401X_cfg.h
 *
 * @brief configuration file for the MCP401X potentiometer
 *
 * This file provides the configruation delcarations for the MCP401X
 * potentiometer driver
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
#ifndef _POTMCP401X_CFG_H
#define _POTMCP401X_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PotMCP401X/PotMCP401X_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the devices
typedef enum _POTMCP401XENUM
{
	// enumerate user pots here
	
	// do not remove
	POTMCP401X_ENUM_MAX
} POTMCP401XENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern	POTMCP401XDEF	g_atPotMCP401XDefs[ ];

// global function prototypes --------------------------------------------------
extern	BOOL	PotMCP401X_LocalInitialize( void );
extern	void	PotMCP401X_SetGpio( GPIOENUM	eGpio, BOOL bState );
extern	void	PotMCP401X_TglGpio( GPIOENUM	eGpio );

/**@} EOF PotMCP401X_cfg.h */

#endif  // _POTMCP401X_CFG_H