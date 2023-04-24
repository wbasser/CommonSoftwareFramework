/******************************************************************************
 * @file Adc.c
 *
 * @brief ADC implementation 
 *
 * This file provides the implementation for the ADC
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ADC/Adc.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  VBOOL         bConvertInProgress;
static  PVADCCALLBACK pvCurCallback;
static  U16UN         tLastResult;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Adc_ConvertChannel
 *
 * @brief perform a conversion on a given AD channel
 *
 * This function will setup and perform a converstion on a A2D channel
 *
 * @param[in]   ptAdcDef    pointer to the definition
 * @param[io]   pwResult    result of the conversion if non-blocking
 *
 * @return      the current result of the operation
 *
 *****************************************************************************/
ADCERRS Adc_ConvertChannel( ADCPOSCHAN ePosChan, ADCREF eAdcRef, ADCPRESCALE ePrescale, PVADCCALLBACK pvCallback, PU16 pwResult )
{
  ADCERRS eError = ADC_ERR_NONE;
  U8      nCsrAValue;

  // validate the parameters
  if (( ePosChan < ADC_POSCHAN_MAX ) && ( eAdcRef < ADC_REF_MAX ))
  {
    // set the callback
    pvCurCallback = pvCallback;

    // set the conversion flag true
    bConvertInProgress = TRUE;

    // set up the mu and referense select
    ADMUX = BITS( eAdcRef, REFS0 ) | ePosChan;

    // start the conversion
    nCsrAValue = ePrescale;
    nCsrAValue |= BIT( ADEN ) | BIT( ADSC ) | BIT( ADIF );
  
    // write the value
    ADCSRA = nCsrAValue;

    //  now determine blocking/non-blocking
    if ( pvCurCallback == NULL )
    {
      // wait till done
      while( bConvertInProgress );
    
      // get result
      *( pwResult ) = tLastResult.wValue;
    }
    else
    {
      // return blocking status
      eError = ADC_ERR_BLOCKING;
    }
  }
  else
  {
    // error in parameters
    eError = ADC_ERR_ILLPRM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function ADC_vect
 *
 * @brief ADC interrupt handler
 *
 * This function will process the ADC handler
 *
 *****************************************************************************/
ISR( ADC_vect )
{
  // get the last result
  tLastResult.anValue[ LE_U16_MSB_IDX ] = ADCH;
  tLastResult.anValue[ LE_U16_LSB_IDX ] = ADCL;

  // if callback do it
  if ( pvCurCallback != NULL )
  {
    // call it
    pvCurCallback( tLastResult.wValue );
  }

  // in all cases clear the convert in progress
  bConvertInProgress = FALSE;
}

/**@} EOF Adc.c */
