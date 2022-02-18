/******************************************************************************
 * @file Adc_prm.h
 *
 * @brief ADC parameters 
 *
 * This file provides the declarations for the parameters for the AtoD
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
 * \addtogroup Adc
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADC_PRM_H
#define _ADC_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------
/// define the clock generator for the A2D converter
#define ADC_CLKGEN_SELECT                       ( CLOCK_GENID_3 )

/// define the macro to enable 1V bandgap
#define ADC_BANDGAP1V_ENABLE                    ( OFF )

/// define the macro to enable external VREF
#define ADC_EXTREF_ENABLE                       ( ON )

#if ( ADC_EXTREF_ENABLE == TRUE )
  /// define the pin for external vref
  #define ADC_EXTREF_PORT_DEF                   ( GPIO_PORT_A )
  #define ADC_EXTREF_PIN_DEF                    ( 4 )
#endif // ADC_EXTREF_ENABLE

/**@} EOF Adc_prm.h */

#endif  // _ADC_PRM_H