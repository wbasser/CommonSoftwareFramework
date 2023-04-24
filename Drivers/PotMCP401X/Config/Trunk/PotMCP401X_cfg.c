/******************************************************************************
 * @file PotMCP401X_cfg.c
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PotMCP401X/PotMCP401X.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
{
	extern	POTMCP401XDEF	g_atPotMCP401XDefs[ POTMCP401X_ENUM_MAX ] =
};
#else
	extern	const CODE	POTMCP401XDEF	g_atPotMCP401XDefs[ POTMCP401X_ENUM_MAX ] =
	{
		// instantiate pots using the below helper macro
		// POTMCP401XDEMF( cspin, udpin )
	};
#endif

/******************************************************************************
 * @function PotMCP401X_LocalInitialize
 *
 * @brief local initiailization
 *
 * This function will perform any needed initialization
 *
 * @return      TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL PotMCP401X_LocalInitialize( void )
{
	BOOL bStatus = FALSE;
	
	// return the status
	return( bStatus );
}

/******************************************************************************
 * @function PotMCP401X_SetGpio
 *
 * @brief GPIo control
 *
 * This function will call the GPIO handler to set the state of a pin
 *
 * @param[in]   eGpio		GPIO eneumeration
 * @param[in]		bState	state of the pin
 *
 *****************************************************************************/
void PotMCP401X_SetGpio( GPIOENUM	eGpio, BOOL bState )
{
}

/******************************************************************************
 * @function PotMCP401X_SetGpio
 *
 * @brief GPIo control
 *
 * This function will call the GPIO handler to togglethe state of a pin
 *
 * @param[in]   eGpio		GPIO eneumeration
 *
 *****************************************************************************/
void PotMCP401X_TglGpio( GPIOENUM	eGpio )
{
}

/**@} EOF PotMCP401X_cfg.c */
