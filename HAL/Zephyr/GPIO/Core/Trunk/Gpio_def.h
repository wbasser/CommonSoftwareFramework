/******************************************************************************
 * @file Gpio_def.h
 *
 * @brief GPIO definition files
 *
 * This file provides the definitions for the GPIO file
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
#ifndef _GPIO_DEF_H
#define _GPIO_DEF_H

// system includes ------------------------------------------------------------
#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating pins from label
#define GPIOPINALIASDEF( label, mode, invert, init ) \
  { \
    .tGpioDt        = GPIO_DT_SPEC_GET( DT_ALIAS( label ), gpios), \
    .eMode          = mode, \
    .bInvert        = invert, \
    .bInitialState  = init \
  }

// enumerations ---------------------------------------------------------------

/// enumerate the pin mode
typedef enum _GPIOMODE
{
  GPIO_MODE_DISABLED = 0,   ///< disabled - analog I/O
  GPIO_MODE_INPDSB_PULLDN,  ///< pull down, input disabled
  GPIO_MODE_INPDSB_PULLUP,  ///< pull up, input disabled
  GPIO_MODE_INPUT,          ///< input
  GPIO_MODE_INPUT_PULLDN,   ///< input, pull down
  GPIO_MODE_INPUT_PULLUP,   ///< input, pull up
  GPIO_MODE_OUTPUT_INPDSB,  ///< output, input disabled
  GPIO_MODE_OUTPUT_INPENB,  ///< output, input enabled
  GPIO_MODE_MAX
} GPIOMODE;

// structures -----------------------------------------------------------------
/// define the pin definition structure
typedef  struct gpio_dt_spec  GPIODT;

/// define the structure
typedef struct _GPIOPINDEF
{
  GPIODT    tGpioDt;
  GPIOMODE  eMode;
  BOOL      bInvert;
  BOOL      bInitialState;
} GPIOPINDEF, *PGPIOPINDEF;
#define GPIOPINDEF_SIZE  sizeof( GPIOPINDEF )

/// define the pin definition structure
typedef struct _GPIOIRQDEF
{
} GPIOIRQDEF, *PGPIOIRQDEF;
#define GPIOIRQDEF_SIZE  sizeof( GPIOIRQDEF )

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_H