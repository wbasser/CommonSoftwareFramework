/******************************************************************************
 * @file EncoderHandler.c
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
#include "EncoderHandler/EncoderHandler.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U16 wCurCounts;
static  U16 wPrvCounts;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function EncoderHandler_Initiailize
 *
 * @brief initialization
 *
 * This function will perform any needed initializaton
 *
 * @return    TRUE if errors occured, FALSE otherwise
 *
 *****************************************************************************/
BOOL EncoderHandler_Initialize( void )
{
  BOOL bStatus = FALSE;

  // call the local initialization
  bStatus = EncoderHandler_LocalInitialize( );

  // clear the counts
  wCurCounts = wPrvCounts = 0;

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function EncoderHandler_EventProcess
 *
 * @brief encoder handler event handler
 *
 * This function will process the events posted to this handler
 *
 * @param[in]   nEvent    event
 *
 *****************************************************************************/
void EncoderHandler_EventProcess( U8 nEvent )
{
  // if we are not saturated
  if ( wCurCounts < 0xFFFF )
  {
    // increment the current counts
    wCurCounts++;
  }

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
