/******************************************************************************
 * @file HysteresisControl_cfg.c
 *
 * @brief Hystereiss control configuration implementation 
 *
 * This file provides the configuration implementation for the Hysteresis
 * control
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup HysteresisControl
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HysteresisControl/HysteresisControl_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE HYSTCONTROLDEF g_atHystControlDefs[ HYST_CONTROL_ENUM_MAX ] =
{
  // define controls using the below helper macro
  // HYSCONTROLDEFM( execrate, sensor, control, poshyst, neghyst, minontime )
  
};

/******************************************************************************
 * @function HysteresisControl_LocalInitialize
 *
 * @brief initialization
 *
 * This function will initialize the hysteresis control
 *
 * @return  TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL HysteresisControl_LocalInitialize( void )
{
  // return false
  return( FALSE );
}

/******************************************************************************
 * @function HysteresisControl_GetProcessValue
 *
 * @brief get the process value
 *
 * This function will fetch and return the process value
 *
 * @param[in]   nSensorEnum   sensor enumerator
 *
 * @return      sensor value
 *
 *****************************************************************************/
HYSTCTRLARG HysteresisControl_GetProcessValue( U8 nSensorEnum )
{
  HYSTCTRLARG xProcessValue = 0;
  
  // return the process value
  return( xProcessValue );
}

/******************************************************************************
 * @function HysteresisControl_SetControlState
 *
 * @brief set the control state
 *
 * This function will set the control state
 *
 * @param[in]   nControlEnum    control enumerator
 * @param[in]   bControlState   state of the control
 *
 *****************************************************************************/
void HysteresisControl_SetControlState( U8 nControlEnum, BOOL bControlState )
{
}
 
/**@} EOF HysteresisControl_cfg.c */
