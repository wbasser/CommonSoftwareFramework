/******************************************************************************
 * @file Pwm_def.h
 *
 * @brief PWM definitions
 *
 * This file provides the definitions for the PWM peripheral
 *
 * @copyright Copyright (c) 2021 Strongarm Tech Inc
 * The copyright to the computer software herein is the property of 
 * Strongarm Tech Inc. This software may not be used and/or copied without
 * explicit written permission of Strongarm Tech Inc., and only in accordance
 * with the terms and conditions stipulated in the license agreement and/or
 * contract under which the software has been supplied.
 *
 * Version History
 * ======
 *
 * \addtogroup PWM
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _PWM_DEF_H
#define _PWM_DEF_H

// system includes ------------------------------------------------------------
#include <device.h>
#include <devicetree.h>
#include <drivers/pwm.h>

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
#define PWMDEFM( device, chan, freqcps, invert ) \
  { \
    .uDevice        = DEVICE_DT_GET( DT_ALIAS( device )), \
    .nChannel       = DT_PWMS_CHANNEL( DT_ALIAS( label )), \
    .uFreqCps       = freqcps, \
    .bInvert        = invert, \
  }

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the pin definition structure
typedef struct _PWMDEF
{
  U32       uDevice;       ///< port string
  U8        nChannel;     ///< channel
  U32       uFreqCps;     ///< frequency CPS
  BOOL      bInvert;      ///< invert the polarity
} PWMDEF, *PPWMDEF;
#define PWMDEF_SIZE  sizeof( PWMDEF )

/**@} EOF Pwm_def.h */

#endif  // _PWM_DEF_H