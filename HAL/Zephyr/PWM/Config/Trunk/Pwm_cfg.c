/******************************************************************************
 * @file Pwm_cfg.c
 *
 * @brief PWM configuration implemantation
 *
 * This file provides the implementation for the PWM HAL configuration module
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PWM/Pwm_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the aliases
#define PWM_LED0_NODE   DT_ALIAS( pwm_led0)

#if DT_NODE_HAS_STATUS(PWM_LED0_NODE, okay)
  #define PWM_CTLR DT_PWMS_CTLR(PWM_LED0_NODE)
  #define PWM_CHANNEL DT_PWMS_CHANNEL(PWM_LED0_NODE)
  #define PWM_FLAGS DT_PWMS_FLAGS(PWM_LED0_NODE)
#else
#error "Unsupported board: pwm-led0 devicetree alias is not defined"
  #define PWM_CTLR DT_INVALID_NODE
  #define PWM_CHANNEL 0
  #define PWM_FLAGS 0
#endif

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------
const PWMDEF g_atPwmDefs[ PWM_ENUM_MAX ] = 
{
  // declare the pins using one of the helper macros below
  // PWMDEFM( label, chan, freqcps, invert )
  //PWMDEFM( pwm_led0, 0, 10000, FALSE ),
};

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

void Pwm_LocalInitialize( )
{
}
 
/**@} EOF Pwm_cfg.c */
