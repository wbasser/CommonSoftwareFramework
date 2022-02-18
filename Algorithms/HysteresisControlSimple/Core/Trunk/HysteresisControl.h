/******************************************************************************
 * @file HysteresisControl.h
 *
 * @brief hystermesis control declarations 
 *
 * This file provides the declarations for the hysteresis control
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
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
#ifndef _HYSTERESISCONTROL_H
#define _HYSTERESISCONTROL_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

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

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the definition structure
typedef struct _HYSTCONTROLDEF
{
  HYSTCTRLARG   xProcSet;
  HYSTCTRLARG   xPosHysteresis;
  HYSTCTRLARG   xNegHysteresis;
} HYSTCONTROLDEF, *PHYSTCONTROLDEF;
#define HYSTCONTROLDEF_SIZE           sizeof( HYSTCONTROLDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  HysteresisControl_Initialize( PHYSTCONTROLDEF ptDef );
extern  BOOL  HysteresisControl_Execute( PHYSTCONTROLDEF ptDef,  HYSTCTRLARG xProcVar );

/**@} EOF HysteresisControl.h */

#endif  // _HYSTERESISCONTROL_H