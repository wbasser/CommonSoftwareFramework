/******************************************************************************
 * @file Gpio_cfg.c
 *
 * @brief GPIO configuraiton implementation
 *
 * This file provides the configuration implementation for the GPIO
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

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_cfg.h"

// Macros and Defines ---------------------------------------------------------

// constant parameter initializations -----------------------------------------
const GPIOPINDEF g_atGpioPinDefs[GPIO_PIN_ENUM_MAX] =
{
  // declare the pins using one of the helper macros below
  // GPIOPINALIASDEF( pin, mode, invert, initval )
  // GPIOPININSTDEF( label, mode, invert, init )
  GPIOPINALIASDEF( led1, GPIO_MODE_OUTPUT_INPDSB, TRUE, OFF ),
  GPIOPINALIASDEF( sw0,  GPIO_MODE_INPUT_PULLUP,  TRUE, OFF ),
  GPIOPINALIASDEF( sw1,  GPIO_MODE_INPUT_PULLUP,  TRUE, OFF ),

};

const CODE GPIOIRQDEF g_atGpioIrqDefs[ GPIO_IRQ_ENUM_MAX ] =
{
  // declare the pins using one of the helper macros below
  // GPIOIRQDEF( port, pin, pupdn, sense, wakeup, filter, callback, event, initon )
  // GPIOEVNDEF( port, pin, pupdn, sense, wakeup, filter )
};

/**@} EOF GPIO_cfg.c */
