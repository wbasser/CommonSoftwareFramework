/******************************************************************************
 * @file HysteresisControl_def.h
 *
 * @brief Hysteresis control configuration definitions 
 *
 * This file provides the defnitions for configuring the hysteresis control
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
 
// ensure only one instantiation
#ifndef _HYSTERESISCONTROL_DEF_H
#define _HYSTERESISCONTROL_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HysteresisControl/HysteresisControl_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the argument type
#if ( HYSTCONTROL_ARG_TYPE == HYSTCONTROL_ARGTYPE_FLOAT )
  typedef FLOAT HYSTCTRLARG;
#elif ( HYSTCONTROL_ARG_TYPE == HYSTCONTROL_ARGTYPE_C8 )
  typedef C8    HYSTCTRLARG;
#elif ( HYSTCONTROL_ARG_TYPE == HYSTCONTROL_ARGTYPE_S16 )
  typedef S16   HYSTCTRLARG;
#elif ( HYSTCONTROL_ARG_TYPE == HYSTCONTROL_ARGTYPE_S32 )
  typedef S32   HYSTCTRLARG;
#lse
  #error You must define a argument type!
#endif

/// define the helper macro for defining a hysteresis control
#define HYSCONTROLDEFM( execrate, sensor, control, poshyst, neghyst, minontime ) \
  { \
    .uExecutionRate = execrate, \
    .nSensorEnum = sensor, \
    .nControlEnum = control, \
    .xPosHysteresis = poshyst, \
    .xNegHysteresis = neghyst, \
    .uMinOnTimeMsecs = minontime \
  }
  
// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the definition structure
typedef struct _HYSTCONTROLDEF
{
  U32           uExecutionRate;
  U8            nSensorEnum;
  U8            nControlEnum;
  HYSTCTRLARG   xPosHysteresis;
  HYSTCTRLARG   xNegHysteresis;
  U32           uMinOnTimeMsecs;
} HYSTCONTROLDEF, *PHYSTCONTROLDEF;
#define HYSTCONTROLDEF_SIZE           sizeof( HYSTCONTROLDEF )

/**@} EOF HysteresisControl_def.h */

#endif  // _HYSTERESISCONTROL_DEF_H