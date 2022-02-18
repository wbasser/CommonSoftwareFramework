/******************************************************************************
 * @file HysteresisControl_prm.h
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
#ifndef _HYSTERESISCONTROL_PRM_H
#define _HYSTERESISCONTROL_PRM_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the argument types
#define HYSTCONTROL_ARGTYPE_FLOAT               ( 0 )
#define HYSTCONTROL_ARGTYPE_C8                  ( 1 )
#define HYSTCONTROL_ARGTYPE_S16                 ( 2 )
#define HYSTCONTROL_ARGTYPE_S32                 ( 3 )

/// define argument type
#define HYSTCONTROL_ARG_TYPE                    ( HYSTCONTROL_ARGTYPE_FLOAT )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

/**@} EOF HysteresisControl_prm.h */

#endif  // _HYSTERESISCONTROL_PRM_H