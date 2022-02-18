/******************************************************************************
 * @file SteinhartHart.h
 *
 * @brief Steinhart-Hart equation declarations
 *
 * This file provides the declarations for the Steinhart-Hart thermistor calculation
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
 * \addtogroup SteinhartHart
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _STEINHARTHART_H
#define _STEINHARTHART_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the configuration structure
typedef struct _STEINHARTDEF
{
  U16   wMaxCounts;
  FLOAT fR1Res;
  FLOAT fACoeef;
  FLOAT fBCoeef;
  FLOAT fCCoeef;
} STEINHARTDEF, *PSTEINHARTDEF;
#define STEINHARTDEF_SIZE                       sizeof( STEINHARTDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  FLOAT  SteinhartHart_CalculateTemp( U16 wMeasuredCounts, PSTEINHARTDEF ptDef );

/**@} EOF SteinhartHart.h */

#endif  // _STEINHARTHART_H