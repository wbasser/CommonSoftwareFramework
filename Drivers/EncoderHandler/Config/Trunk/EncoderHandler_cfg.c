/******************************************************************************
 * @file EncoderHandler_cfg.c
 *
 * @brief encoder handler implemenation
 *
 * This file provides the implementation for the encoder handler
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
 * \addtogroup EncoderHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function EncoderHandler_LocalInitiailize
 *
 * @brief initialization
 *
 * This function will perform any needed initializaton
 *
 * @return    TRUE if errors occured, FALSE otherwise
 *
 *****************************************************************************/
BOOL EncoderHandler_LocalInitialize( void )
{
  BOOL bStatus = FALSE;

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function EncoderHandler_IrqCallback
 *
 * @brief IRQ callback
 *
 * This function will handler the IRQ callback
 *
 * @param[in]   nPin      pin that caused the interrupt
 * @param[in]   nEvent    event
 * @param[in]   bState    state of the pin
 *
 *****************************************************************************/
void EncoderHandler_IrqCallback( U8 nPin, U8 nEvent, BOOL bState )
{
  BOOL    bPhaseB;
  TASKARG xEvent;

  // get the state of the encoder phase B
  Gpio_Get( GPIO_PIN_ENUM_ENCPHB, &bPhaseB );

  // now check clockwise/counter clockwise/post event
  xEvent = ( bPhaseB ) ? ENCHANDLER_CW_EVENT : ENCHANDLER_CCW_EVENT;
  TaskManager_PostEventIrq( TASK_SCHD_DSPHAND, xEvent );
}

/******************************************************************************
 * @function EncoderHandler_ProcessEvent
 *
 * @brief encoder handler event handler
 *
 * This function will process the events posted to this handler
 *
 * @param[in]   xArg    event
 *
 * @return      TRUE to flush event
 *
 *****************************************************************************/
BOOL EncoderHandler_ProcessEvent( TASKARG xArg )
{
  // return true to flush event
  return( TRUE );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/

/**@} EOF EncoderHandler.c */
