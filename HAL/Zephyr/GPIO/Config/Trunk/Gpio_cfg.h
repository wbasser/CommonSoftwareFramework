/******************************************************************************
 * @file Gpio_cfg.h
 *
 * @brief GPIO configuration declarations
 *
 * This file provides the declarations for the GPIO configuration
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
 * \addtogroup Gpio
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GPIO_CFG_H
#define _GPIO_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the GPIO user pin enumerations
typedef enum  _GPIOPINENUM
{
  // add enumerations here
  GPIO_PIN_ENUM_STSLED = 0,
  GPIO_PIN_ENUM_PWRBTN,
  GPIO_PIN_ENUM_PWMBTN,
  
  // this ends the list - do not remove
  GPIO_PIN_ENUM_MAX,
  GPIO_PIN_ENUM_ILLEGAL = 0xFF
} GPIOPINENUM;

/// declare the GPIO interrupt pin enumerations
typedef enum  _GPIOIRQENUM
{
  // add enumerations here
  
  // this end s the list - do not remove
  GPIO_IRQ_ENUM_MAX,
  GPIO_IRQ_ENUM_ILLEGAL = 0xFF
} GPIOIRQENUM;

// global function prototypes --------------------------------------------------
extern  const CODE GPIOPINDEF   g_atGpioPinDefs[ ];
extern  const CODE GPIOIRQDEF   g_atGpioIrqDefs[ ];

/**@} EOF Gipo_cfg.h */

#endif  // _GPIO_CFG_H