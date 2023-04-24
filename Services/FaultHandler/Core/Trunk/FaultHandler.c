/******************************************************************************
 * @file FaultHandler.c
 *
 * @brief fault handler implementation 
 *
 * This file provides the fault handler implementation
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
 * \addtogroup FaultHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "FaultHandler/FaultHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the on/off times
#define FLT_ON_TIME_MSECS                       ( 100 )
#define FLT_OFF_TIME_MSECS                      ( 600 )

// enumerations ---------------------------------------------------------------
/// define the top level state
typedef enum _TOPLVLSTATE
{
  TOPLVL_STATE_IDLE = 0,
  TOPLVL_STATE_ON,
  TOPLVL_STATE_OFF,
} TOPLVLSTATE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function FaultHandler_Initialize
 *
 * @brief fault initialization
 *
 * This function will perform any neede initialization
 *
 * @return      TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL FaultHandler_Initialize( void )
{
  BOOL bStatus = FALSE;
  
  // perform any needed initialization
  bStatus |= FaultHandler_LocalInitialize( );
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function FaultHandler_Display
 *
 * @brief display the error
 *
 * This function will display the current error
 *
 * @param[in]   uFault    fault error
 *
 *****************************************************************************/
void FaultHandler_Display( U32 uFault )
{
  TOPLVLSTATE eTopLvlState = TOPLVL_STATE_OFF;
  U16         wDelayMsecs;
  BOOL        bGpioState;

  // configure the fault GPIO
  FaultHandler_GpioConfigure( );

  FOREVER
  {
    // process the state
    switch( eTopLvlState )
    {
      case TOPLVL_STATE_OFF :
        // tun on the GPIO
        bGpioState = ON;

        // set the delay
        wDelayMsecs = FLT_ON_TIME_MSECS;

        // goto on
        eTopLvlState = TOPLVL_STATE_ON;
        break;

      case TOPLVL_STATE_ON :
        // tun on the GPIO
        bGpioState = OFF;

        // set the delay
        wDelayMsecs = FLT_OFF_TIME_MSECS;

        // goto on
        eTopLvlState = TOPLVL_STATE_OFF;
        break;

      default :
        break;
    }

    // set the GPIO/delay
    FaultHandler_GpioControl( bGpioState );
    FaultHandler_DelayMsec( wDelayMsecs );
  }
}

/**@} EOF FaultHandler.c */
