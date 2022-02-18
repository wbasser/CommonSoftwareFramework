/******************************************************************************
 * @file HysteresisControl.c
 *
 * @brief Hysteresis control implementation 
 *
 * This file provides the implementation for the hysteresis control
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
#include "HysteresisControl/HysteresisControl.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _LCLCTL
{
  U16         wExecTime;
  U16         wExecRate;
  HYSTCTRLARG xSetPoint;
  U16         wOnTime;
  BOOL        bPrvState;
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE             sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL  atLclCtl[ HYST_CONTROL_ENUM_MAX ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function HysteresisControl_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the hysteresis control
 *
 * @return  TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL HysteresisControl_Initialize( void )
{
  PHYSTCONTROLDEF ptDef;
  HYSTCONTROLENUM eCtrlEnum;
  PLCLCTL         ptCtl;
  BOOL            bStatus;
  
  // call the local initializaion
  bStatus = HysteresisControl_LocalInitialize( );

  // for each control
  for ( eCtrlEnum = 0; eCtrlEnum < HYST_CONTROL_ENUM_MAX; eCtrlEnum++ )
  {
    // get the pointers to the def/control structure
    ptDef = ( PHYSTCONTROLDEF )&atHystControlDefs[ eCtrlEnum ];
    ptCtl = &atLclCtl[ eCtrlEnum ];
    ptCtl->wExecRate = PGM_RDDWRD( ptDef->uExecutionRate ) / HYSTERESISCONTROL_EXEC_RATE;
    ptCtl->bPrvState = OFF;
  }
  
  // return false
  return( bStatus );
}

/******************************************************************************
 * @function HysteresisControl_ProcessControl
 *
 * @brief process the control task
 *
 * This function will perform the process of each hysteresis control
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush the event
 *
 *****************************************************************************/
BOOL HysteresisControl_ProcessControl( TASKARG xArg )
{
  PHYSTCONTROLDEF ptDef;
  HYSTCONTROLENUM eCtrlEnum;
  PLCLCTL         ptCtl;
  HYSTCTRLARG     xSensorValue;
  BOOL            bCurState = OFF;
  
  // for each control
  for ( eCtrlEnum = 0; eCtrlEnum < HYST_CONTROL_ENUM_MAX; eCtrlEnum++ )
  {
    // get the pointers to the def/control structure
    ptDef = ( PHYSTCONTROLDEF )&atHystControlDefs[ eCtrlEnum ];
    ptCtl = &atLclCtl[ eCtrlEnum ];
    
    // increment the count/test for done
    if ( ++ptCtl->wExecTime >= ptCtl->wExecRate )
    {
      // reset the count
      ptCtl->wExecTime = 0;
      
      // now get the process variable
      xSensorValue = HysteresisControl_GetProcessValue( PGM_RDBYTE( ptDef->eSensorEnum ));
      
      // check for on/off
      if ( xSensorValue <= ( ptCtl->xSetPoint - PGM_RDDWORD( ptDef->xPosHysteresis )))
      {
        // turn on relay
        bCurState = OFF;
      }
      else if ( xSensorValue >= ( ptCtl->xSetPoint - PGM_RDDWORD( ptDef->xNegHysteresis )))
      {
        // turn on relay
        bCurState = ON;
      }

      // set the relay state
      HysteresisControl_SetControlState( PGM_RDBYTE( ptDef->nControlEnum ), bCurState );
      
      // update the previous
      ptCtl->bPrvState = bCurState;
    }
  }

  // return TRUE - flush event
  return( TRUE );
}

/******************************************************************************
 * @function HysteresisControl_PutSetpont
 *
 * @brief put the new setpoint
 *
 * This function will set the new setpoint for a hysteresis control channel
 *
 * @param[in]   eHystCtlEnum      hysteresis control enumeration
 * @Param[in]   iSetpoint         new control setpoint
 *
 *****************************************************************************/
void HysteresisControl_PutSetpoint( HYSTCONTROLENUM eHystCtlEnum, S16 iSetpoint )
{
  // update the setpoint
  if ( eHystCtlEnum < HYST_CONTROL_ENUM_MAX )
  {
    // update the setpoint
    atLclCtl[ eHystCtlEnum ].iSetPoint = iSetpoint;
  }
}

/**@} EOF HysteresisControl.c */
