/******************************************************************************
 * @file FaultHandler_cfg.c
 *
 * @brief fault handler configuration implementation 
 *
 * This file provides the implementation for the fault handler
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
#include "FaultHandler/FaultHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function FaultHandler_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any needed local initialization
 *
 * @return      TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL FaultHandler_LocalInitialize( void )
{
}

/******************************************************************************
 * @function FaultHandler_GpioConfigure
 *
 * @brief fault handler GPIO configuraton
 *
 * This function will configure the GPIO used in fault display
 *
 *****************************************************************************/
void FaultHandler_GpioConfigure( void )
{
}

/******************************************************************************
 * @function FaultHandler_GpioControl
 *
 * @brief fault handler GPIO control
 *
 * This function will control the fault indicator GPIO
 *
 * @param[in]   bState      state of the pin
 *
 *****************************************************************************/
void FaultHandler_GpioControl( BOOL bState )
{
}

/******************************************************************************
 * @function FaultHandler_DelayMsec
 *
 * @brief delay
 *
 * This function will delay for the desired msecs
 *
 * @param[in]   wMsec     delay in milliseconds
 *
 *****************************************************************************/
void FaultHandler_DelayMsec( U16 wMsec )
{
}
 
/**@} EOF FaultHandler_cfg.c */
