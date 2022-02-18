/******************************************************************************
 * @file Adc.h
 *
 * @brief ADC declarations
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup ADC
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADC_H
#define _ADC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _ADCERRS
{
  ADC_ERR_NONE = 0,           ///< no error
  ADC_ERR_BLOCKING,           ///< blocking operation in progress
  ADC_ERR_ILLPRM = 0x80,      ///< illegal parameter
} ADCERRS;

/// enumerate the  positive channels
typedef enum _ADCPOSCHAN
{
  ADC_POSCHAN_AIN00 = 0,      ///< ADC AIN0 pin
  ADC_POSCHAN_AIN01,          ///< ADC AIN1 pin
  ADC_POSCHAN_AIN02,          ///< ADC AIN2 pin
  ADC_POSCHAN_AIN03,          ///< ADC AIN3 pin
  ADC_POSCHAN_AIN04,          ///< ADC AIN4 pin
  ADC_POSCHAN_AIN05,          ///< ADC AIN5 pin
  ADC_POSCHAN_AIN06,          ///< ADC AIN6 pin
  ADC_POSCHAN_AIN07,          ///< ADC AIN7 pin
  ADC_POSCHAN_AIN08,          ///< ADC AIN8 pin
  ADC_POSCHAN_AIN09,          ///< ADC AIN9 pin
  ADC_POSCHAN_AIN10,          ///< ADC AIN10 pin
  ADC_POSCHAN_AIN11,          ///< ADC AIN11 pin
  ADC_POSCHAN_GND,            ///< ground
  ADC_POSCHAN_BANDGAP,        ///< Bandgap
  ADC_POSCHAN_TEMP,           ///< Temperate reference
  ADC_POSCHAN_MAX
} ADCPOSCHAN;

/// enumerate the references
typedef enum _ADCREF
{
  ADC_REF_AVCC = 0,           ///< analog supply pin
  ADC_REF_EXTVREF,            ///< external VREFA
  ADC_REF_INT1V1,             ///< internal 1.1V 
  ADC_REF_MAX
} ADCREF;

/// enumerate the prescale 
typedef enum _ADCPRESCALE
{
  ADC_PRESCALE_2 = 1,     ///< divide by 2
  ADC_PRESCALE_4,         ///< divide by 4
  ADC_PRESCALE_8,         ///< divide by 8
  ADC_PRESCALE_16,        ///< divide by 16
  ADC_PRESCALE_32,        ///< divide by 32
  ADC_PRESCALE_64,        ///< divide by 64
  ADC_PRESCALE_128,       ///< divide by 128
  ADC_PRESCALE_MAX
} ADCPRESCALE;

// structures -----------------------------------------------------------------
/// define the callback 
typedef void ( *PVADCCALLBACK )( U16 );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  ADCERRS Adc_ConvertChannel( ADCPOSCHAN ePosChan, ADCREF eAdcRef, ADCPRESCALE ePrescale, PVADCCALLBACK pvCallback, PU16 pwResult );

/**@} EOF Adc.h */

#endif  // _ADC_H