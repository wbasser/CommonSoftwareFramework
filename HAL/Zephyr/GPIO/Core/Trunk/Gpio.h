/******************************************************************************
 * @file Gpio.h
 *
 * @brief GPIO implementation declarations
 *
 * This file provides the delcaratiosn for the implementation for the GPIO
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
 * \addtogroup Gpio.h
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GPIO_H
#define _GPIO_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the GPIO errors
typedef enum _GPIOERR
{
  GPIO_ERR_NONE = 0,    ///< no error
  GPIO_ERR_ILLGPIO,     ///< illegal GPIO selection
  GPIO_ERR_ILLACT,      ///< illegal IOCTL action
  GPIO_ERR_ILLMODE,     ///< illegal mode, (I.e. trying to set an input pin )
  GPIO_ERR_ILLDIR,      ///< illegal direction, (i.e. tryint to change a direction on a non IO pin )
} GPIOERR;

/// enumerate the GPIO IOCTL actions
typedef enum _GPIOACT
{
  GPIO_ACT_NOOP = 0,    ///< no operation
  GPIO_ACT_SETDIR,      ///< set direction
  GPIO_ACT_SETMODE,     ///< set mode
  GPIO_ACT_ENBDSBIRQ,   ///< enable/disable IRQ
  GPIO_ACT_SETFUNCMUX,  ///< set min mux
  GPIO_ACT_MAX,         ///< maximum number
} GPIOACT;

// global function prototypes --------------------------------------------------
extern  BOOL    Gpio_Initialize( void );
extern  void    Gpio_Close( void );
extern  GPIOERR Gpio_Set( GPIOPINENUM eGpioSel, BOOL bState );
extern  GPIOERR Gpio_Get( GPIOPINENUM eGpioSel, PBOOL pbState );
extern  GPIOERR Gpio_Toggle( GPIOPINENUM eGpioSel );
extern  GPIOERR Gpio_Ioctl( GPIOPINENUM eGpioSel, GPIOACT eGpioAct, PVOID pvData );

/**@} EOF Gpio.h */

#endif  // _GPIO_H