/******************************************************************************
 * @file AdcADS122C04.h
 *
 * @brief ADC ADS122C04 declarations 
 *
 * This file provides the declarations for the implementation of the ADC ADS122C04
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * \addtogroup AdcADS122C04
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADCADS122C04_H
#define _ADCADS122C04_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AdcADS122C04/AdcADS122C04_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the minimum/maximum counts
#define ADCADS122C04_MIN_COUNTS                 ( 0x00000000 )
#define ADCADS122C04_MAX_COUNTS                 ( 0x7FFFFFFF )

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _ADCADS122C04ERR
{
  ADCADS122C04_ERR_NONE = 0,
  ADCADS122C04_ERR_ILLCHAN = 0xD0,
  ADCADS122C04_ERR_DATANOTREADY,
  ADCADS122C04_ERR_BUSERROR,
} ADCADS122C04ERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL            AdcADS122C04_Initialize( void );
extern  ADCADS122C04ERR AdcADS122C04_StartConversion( ADCADS122C04CHANENUM eChan );
extern  ADCADS122C04ERR AdcADS122C04_GetResults( PU32 puValue );

/**@} EOF AdcADS122C04.h */

#endif  // _ADCADS122C04_H