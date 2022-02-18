/******************************************************************************
 * @file SteinhartHart.c
 *
 * @brief STeinhart-Hart implementation
 *
 * This file provides the implementation for the STeinhart-Hart implementation
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

// system includes ------------------------------------------------------------
#include <math.h>

// local includes -------------------------------------------------------------
#include "SteinhartHart/SteinhartHart.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the kelvin base
#define KELVIN_BASE_TEMP                        ( 273.15 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SteinhartHart_CalculateTemp
 *
 * @brief calculate the temperature of a thermistor
 *
 * This function will calculate the temperature of a thermistor
 *
 * @param[in]   wMeasuredCounts AtoD value in counts
 * @param[in]   ptDef           pointer to the definition file
 *
 * @return      temperature in C
 *
 *****************************************************************************/
FLOAT SteinhartHart_CalculateTemp( U16 wMeasuredCounts, PSTEINHARTDEF ptDef )
{
  FLOAT fTemp, fThermRes;
  
  // calculate the resistance of the thermistor
  fThermRes= log( ptDef->fR1Res * (( FLOAT )ptDef->wMaxCounts / ( FLOAT )wMeasuredCounts - 1.0 ));
  fTemp = ( 1.0 / ( ptDef->fACoeef + ( ptDef->fBCoeef * fThermRes ) + ( ptDef->fCCoeef * fThermRes * fThermRes * fThermRes )));
  fTemp -= KELVIN_BASE_TEMP;
  
  // return the temp
  return( fTemp );
}
 
/**@} EOF SteinhartHart.c */
