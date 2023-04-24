/******************************************************************************
 * @file Pwm.h
 *
 * @brief PWM declarations
 *
 * This file provides the declarations for the PWM HAL module
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
#ifndef _PWM_H
#define _PWM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PWM/Pwm_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the PWM errors
typedef enum _PWMERR
{
  PWM_ERR_NONE = 0,   ///< no error
  PWM_ERR_ILLEGAL,    ///< illegal PWM selection
  PWM_ERR_ILLVALUE,   ///< illegal value
} PWMERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void    Pwm_Initialize( void );
extern  void    Pwm_Close( void );
extern  PWMERR  Pwm_PulseWidthPercent( PWMENUM ePwmEnum, U8 nPercent );
extern  PWMERR  Pwm_SetFrequency( PWMENUM ePwmEnum, U32 nFreqCps );

/**@} EOF Pwm.h */

#endif  // _PWM_H