/******************************************************************************
 * @file PotMCP401X_def.h
 *
 * @brief definitions for the PotMCP401X 
 *
 * This file provides the definitions for the PotMCP401x
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
#ifndef _POTMCP401X_DEF_H
#define _POTMCP401X_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro for defining a pot entry
#define	POTMCP401XDEFM( cspin, udpin ) \
  { \
    .eCsPin = cspin, \
    .eUdPin = udpin \
  }
	
// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the structure for an instantiation of a pot
typedef struct _POTMCP401XDEF
{
	GPIOPINENUM	eCsPin;
	GPIOPINENUM	eUdPin;
} POTMCP401XDEF, *PPOTMCP401XDEF;
#define	POTMCP401XDEF_SIZE						sizeof( POTMCP401XDEF )
				
/**@} EOF PotMCP401X_def.h */

#endif  // _POTMCP401X_DEF_H