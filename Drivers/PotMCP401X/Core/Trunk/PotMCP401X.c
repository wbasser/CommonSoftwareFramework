/******************************************************************************
 * @file PotMCP401X
 *
 * @brief	PotMCP401X implementation 
 *
 * This file provides the implementation for the MCP401X potentiometer
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PotMCP401X/PotMCP401X.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the maximum position
#define	POT_MAX_POS											( 64 )
#define	POT_POR_POS											( 31 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	U8	anCurPosition[ POTMCP401X_ENUM_MAX ];

// local function prototypes --------------------------------------------------
static	void	SetPosition( POTMCP401XENUM ePotEnum, U8 nPosition );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function PotMCP401X_Initialize
 *
 * @brief local initiailization
 *
 * This function will perform any needed initialization
 *
 * @return      TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL PotMCP401X_Initialize( void )
{
	BOOL						bStatus = FALSE;
	POTMCP401XENUM	eEnum;
	
	// call the local initializaiton
	bStatus = PotMCP401X_LocalInitialize( );
	
	// for each control - set to power on value
	for( eEnum = 0; eEnum < POTMCP401X_ENUM_MAX; eEnum++ )
	{
		// set the power on reset value 
		anCurPosition[ eEnum ] = POT_POR_POS;
	}
	
	// return the status
	return( bStatus );
}

/******************************************************************************
 * @function PotMCP401X_SetPosition
 *
 * @brief set the position of a pot
 *
 * This function will set the position of the pot
 *
 * @param[in]   ePotEnum		pot enumeration
 * @param[in]		nPosition		desired position
 *
 * @return      appropriate error
 *
 *****************************************************************************/
POTMCP401XERR	PotMCP401X_SetPosition( POTMCP401XENUM ePotEnum, U8 nPosition )
{
	POTMCP401XERR eError = POTMCP401X_ERR_NONE;
	
	// check for a valid pot enumeration
	if ( ePotEnum < POTMCP401X_ENUM_MAX )
	{
		// check for valid value
		if ( nPosition < POT_MAX_POS )
		{
			// do it
			SetPosition( ePotEnum, nPosition );
		}
		else
		{
			// set the error
			eError = POTMCP401X_ERR_ILLVAL;
		}
	}
	else
	{
		// set the error
		eError = POTMCP401X_ERR_ILLENUM;
	}
	
	// return the error
	return( eError );
}

/******************************************************************************
 * @function PotMCP401X_IncrementDecrement
 *
 * @brief increment/decrement the postion
 *
 * This function will set the position of the pot
 *
 * @param[in]   ePotEnum		pot enumeration
 * @param[in]		cDelta			delta, 1 for up, -1 for down
 *
 * @return      appropriate error
 *
 *****************************************************************************/
POTMCP401XERR	PotMCP401X_IncrementDecrement( POTMCP401XENUM ePotEnum, C8 cDelta )
{
	POTMCP401XERR eError = POTMCP401X_ERR_NONE;
	U8						nNewPosition;
	
	// check for a valid pot enumeration
	if ( ePotEnum < POTMCP401X_ENUM_MAX )
	{
		// set the new position
		nNewPosition += cDelta;
		
		// check for valid
		if ( nNewPosition < POT_MAX_POS )
		{
			// do it
			SetPosition( ePotEnum, nNewPosition );
		}
		else
		{
			// set the error
			eError = POTMCP401X_ERR_ILLVAL;
		}
	}
	else
	{
		// set the error
		eError = POTMCP401X_ERR_ILLENUM;
	}
	
	// return the error
	return( eError );
}

/******************************************************************************
 * @function PotMCP401X_IncrementDecrement
 *
 * @brief increment/decrement the postion
 *
 * This function will set the position of the pot
 *
 * @param[in]   ePotEnum		pot enumeration
 * @param[in]		nNewPosition		new position
 *
 *****************************************************************************/
static void	SetPosition( POTMCP401XENUM ePotEnum, U8 nNewPosition )
{
	PPOTMCP401XDEF	ptDef;
	U8							nCount, nCurPosition;
	
	// get a poniter to the definition 
	ptDef = ( PPOTMCP401XDEF )&g_atPotMCP401XDefs[ ePotEnum ];
	
	// get the current position
	nCurPosition = anCurPosition [ ePotEnum ];
	
	// check for direction
	if ( nNewPosition > nCurPosition )
	{
		// up 
		PotMCP401X_SetGpio( ptDef->eUdPin, OFF );
		nCount = nNewPosition - nCurPosition;
	}
	else
	{
		// down
		PotMCP401X_SetGpio( ptDef->eUdPin, ON );
		nCount = nCurPosition - nNewPosition;
	}
	
	// now set the chip select
	PotMCP401X_SetGpio( ptDef->eCsPin, ON );

	// for each count
	do
	{
		// toggle the up/down pin
		PotMCP401X_TglGpio( ptDef->eUdPin );
	} 
	while ( --nCount != 0 );
	
	// deselect it
	PotMCP401X_SetGpio( ptDef->eCsPin, OFF );
	
	// update the current position
	anCurPosition[ ePotEnum ] = nNewPosition;
}

/**@} EOF PotMCP401X.c */
