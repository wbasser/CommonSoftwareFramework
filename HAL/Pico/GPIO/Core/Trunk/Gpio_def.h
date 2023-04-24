/******************************************************************************
 * @file Gpio_def.h
 *
 * @brief GPIO module configuration definitions
 *
 * This file provides the definitions of the structures and enumerations
 * used in defining the GPIO pins and usages
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup GPIO
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GPIO_DEF_H
#define _GPIO_DEF_H

// system includes ------------------------------------------------------------
#include <RP2040.h>

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating output pins
#define GPIOPINDEF( pin, mode, drive, invert, init ) \
  { \
    .nPin           = pin, \
    .eMode          = mode, \
    .eDrive         = drive, \
    .bInvert        = invert, \
    .bInitialState  = init \
  }

/// helper macro for creating a function output pin  
#define GPIOFNCDEF( pin, func ) \
  { \
    .nPin           = pin, \
    .eFunc          = func, \
  }
  
/// define the helper macro for creating an interrupt pin
#define GPIOIRQDEF( pin, pupdn, sense, wakeup, filter, callback, event, initon ) \
  { \
    .nPin           = pin, \
    .ePupdn         = pupdn, \
    .eSense         = sense, \
    .bWakeupEnb     = wakeup, \
    .bFilterEnb     = filter, \
    .pvCallback     = callback, \
    .nEvent         = event, \
    .bEventType     = FALSE, \
    .bInitOn        = initon \
  }


// enumerations ---------------------------------------------------------------
/// enumerate the pin mode
typedef enum _GPIOMODE
{
  GPIO_MODE_DISABLED = 0,                       ///< disabled - analog I/O
  GPIO_MODE_INPUT,                              ///< input
  GPIO_MODE_INPUT_PULLDN,                       ///< input, pull down
  GPIO_MODE_INPUT_PULLUP,                       ///< input, pull up
  GPIO_MODE_INPUT_PULLUPDN,                     ///< input, pull up/down
  GPIO_MODE_OUTPUT_SLOW,                        ///< output, slow slew
  GPIO_MODE_OUTPUT_FAST,                        ///< output, fast slew
  GPIO_MODE_MAX
} GPIOMODE;

/// enumerate the pin multiplexing
typedef enum _GPIOFUNCMUX
{
  GPIO_FUNCMUX_XIP = 0,                         ///< XIP function
  GPIO_FUNCMUX_SPI,                             ///< SPI function
  GPIO_FUNCMUX_UART,                            ///< UART function
  GPIO_FUNCMUX_I2C,                             ///< I2C function
  GPIO_FUNCMUX_PWM,                             ///< PWM function
  GPIO_FUNCMUX_SIO,                             ///< SIO function
  GPIO_FUNCMUX_PIO0,                            ///< PIO0 function
  GPIO_FUNCMUX_PIO1,                            ///< PIO1 function
  GPIO_FUNCMUX_GPCK,                            ///< GPCK function
  GPIO_FUNCMUIX_USB,                            ///< USB function
  GPIO_FUNCMUX_MAX
} GPIOFUNCMUX;

/// enumerate the IRQ pin edge
typedef enum _GPIOIRQSENSE
{
  GPIO_SENSE_NONE = 0,       ///< no sense
  GPIO_SENSE_RISE,          ///< rising edge
  GPIO_SENSE_FALL,          ///< falling edge
  GPIO_SENSE_BOTH,          ///< both edges
  GPIO_SENSE_HILVL,         ///< HI level
  GPIO_SENSE_LOLVL,         ///< LO level
  GPIO_EDGE_MAX
} GPIOIRQSENSE;

/// enumerate the GPIO pin direction
typedef enum _GPIODIR
{
  GPIO_DIR_IN = 0,          ///< input
  GPIO_DIR_OUT,             ///< output
  GPIO_DIR_MAX          
} GPIODIR;

/// enumerate the GPIO output strength
typedef enum _GPIODRIVE
{
  GPIO_DRIVE_2MA = 0,
  GPIO_DRIVE_4MA,
  GPIO_DRIVE_8MA,
  GPIO_DRIVE_12MA
  GPIO_DRIVE_MAX
} GPIODRIVE;

/// enumerate the GPIO IRQ pullup modes
typedef enum _GPIOIRQPUPDN
{
  GPIO_IRQPUPDN_NONE = 0,   ///< no pull up/down
  GPIO_IRQPUPDN_PULLDN,     ///< pull down
  GPIO_IRQPUPDN_PULLUP,     ///< pull up
  GPIO_IRQPUPDN_MAX
} GPIOIRQPUPDN;

// structures -----------------------------------------------------------------
/// define the callback
typedef void ( *PVGPIOIRQCALLBACK )( U8 nIrq, U8 nEvent, BOOL bState );

/// define the pin definition structure
typedef struct _GPIOPINDEF
{
  U8                nPin;           ///< port pin
  GPIOMODE          eMode;          ///< mode
  GPIODRIVE         eDrive;
  BOOL              bInvert;        ///< pin is inverted
  BOOL              bInitialState;  ///< pins initial state
} GPIOPINDEF, *PGPIOPINDEF;
#define GPIOPINDEF_SIZE  sizeof( GPIOPINDEF )

/// define the pin definition structure
typedef struct _GPIOFNCDEF
{
  U8                nPin;           ///< port pin
  GPIOFUNCMUX       eFunc;          ///< port function
} GPIOFNCDEF, *PGPIOFNCDEF;
#define GPIOFNCDEF_SIZE  sizeof( GPIOFNCDEF )

/// define the pin definition structure
typedef struct _GPIOIRQDEF
{
  U8                nPin;           ///< port pin
  GPIOIRQPUPDN      ePupdn;         ///< pull up/down
  GPIOIRQSENSE      eSense;         ///< sense
  BOOL              bWakeupEnb;     ///< wakeup enable
  BOOL              bFilterEnb;     ///< filter enable
  PVGPIOIRQCALLBACK pvCallback;     ///< callback
  U8                nEvent;         ///< callback event
  BOOL              bEventType;     ///< event type
  BOOL              bInitOn;        ///< initialial on
} GPIOIRQDEF, *PGPIOIRQDEF;
#define GPIOIRQDEF_SIZE  sizeof( GPIOIRQDEF )

/**@} EOF Gpio_def.h */

#endif  // _GPIO_DEF_H