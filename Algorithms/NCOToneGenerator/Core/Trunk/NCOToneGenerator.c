/******************************************************************************
 * @file NCOToneGenerator.c
 *
 * @brief tone generator implementation
 *
 * This file provides the implementation for the tone generator
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
 * \addtogroup NCOToneGenerator
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "NCOToneGenerator/NCOToneGenerator.h"
#include "NCOToneGenerator/NCOToneGenerator_prv.h"

// library includes -----------------------------------------------------------
#include "DAC/Dac.h"
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U16 wCurrentStep;
static  U16 wTableIndex;
static  U16 wCurrentFreq;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function NCOToneGenerator_Initialize 
 *
 * @brief initilization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void NCOToneGenerator_Initialize( void )
{
  // set the current freqeuency/step
  wCurrentFreq = 400;
  wCurrentStep = 0;
}

/******************************************************************************
 * @function NCOToneGenerator_Control
 *
 * @brief control the tone generator
 *
 * This function will enable/disable the tone generator
 *
 * @param[in]   bState      desired state
 *
 *****************************************************************************/
void NCOToneGenerator_Control( BOOL bState )
{
  // on/off
  Timers_Ioctl( TIMER_ENUM_TONEGENERATE, TIMER_IOCTL_STOPSTART, ( PVOID )&bState );
}

/******************************************************************************
 * @function NCOToneGenerator_SetFrequency
 *
 * @brief set the frequency
 *
 * This function will set the current output frequency
 *
 * @param[in]   wFrequency    desired frequency
 *
 *****************************************************************************/
void NCOToneGenerator_SetFrequency( U16 wFrequency  )
{
  // store the frequency
  wCurrentFreq = wFrequency;

  // calculate the new step
  Interrupt_Disable( );
  wCurrentStep = ( MAX_NUM_STEPS * wCurrentFreq ) / NCOTONEGENERATOR_SAMPLE_RATE;
  Interrupt_Enable( );
}
 
/******************************************************************************
 * @function NCOToneGenerator_TimerCallback
 *
 * @brief timer callback
 *
 * This function process the timer callback, output the new DAC value,
 * and increment the step
 *
 * @param[in]   eEvent        timer event
 * @param[in]   eChan         timer channel
 * @param[in]   wValue        value
 *
 *****************************************************************************/
void NCOToneGenerator_TimerCallback( TIMERCBEVENT eEvent, TIMERCMPCAP eChan, U16 wValue )
{
  // output the current value
  Dac_SetOutput( awSineTable[ wTableIndex ] );

  // adjust the index
  wTableIndex += wCurrentStep;
  wTableIndex %= MAX_NUM_STEPS;
}

/**@} EOF NCOToneGenerator.c */
