/******************************************************************************
 * @file Pwm_cfg.h
 *
 * @brief PWM HAL configuration declarations
 *
 * This file provides the declarations for the PWM HAL configurmation module
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
#ifndef _PWM_CFG_H
#define _PWM_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PWM/Pwm_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// define the PWM enumerations
typedef enum _PWMENUM
{
  // add user PWM enumerations
  //PWM_ENUM_LED0 = 0,


  // do not remove below
  PWM_ENUM_MAX,
  PWM_ENUM_ILLEGAL = 0xFF
} PWMENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE PWMDEF   g_atPwmDefs[ ];

// global function prototypes --------------------------------------------------

/**@} EOF Pwm_cfg.h */

#endif  // _PWM_CFG_H