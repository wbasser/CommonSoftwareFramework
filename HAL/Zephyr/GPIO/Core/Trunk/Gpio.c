/******************************************************************************
 * @file Gpio.c
 *
 * @brief GPIO implementation
 *
 * This file provides the implementation for the GPIO 
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
 * \addtogroup GPIO
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

/******************************************************************************
 * @function Gpio_Initialize
 *
 * @brief initialize the GPIO handler
 *
 * This function initialized the GPIO handler, configures each pin in the list
 *
 * @return  TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL Gpio_Initialize( void )
{
  BOOL          bStatus = FALSE;
  PGPIOPINDEF   ptPinDef;
  U32           uFlags;
  U8            nIdx;

  // for each pin def in the list
  for( nIdx = 0; nIdx < GPIO_PIN_ENUM_MAX; nIdx++ )
  {
    // get a pointer to the definition
    ptPinDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ nIdx ];

    // get the device
    bStatus |= ( gpio_is_ready_dt( &ptPinDef->tGpioDt )) ? TRUE : FALSE;

    // set the mode
    uFlags = ( ptPinDef->bInvert ) ? GPIO_ACTIVE_LOW : GPIO_ACTIVE_HIGH;
    switch( ptPinDef->eMode )
    {
      case GPIO_MODE_INPUT :
        uFlags |= GPIO_INPUT;
        break;

      case GPIO_MODE_INPUT_PULLDN :
        uFlags |= GPIO_INPUT | GPIO_PULL_DOWN;
        break;

      case GPIO_MODE_INPUT_PULLUP :
        uFlags |= GPIO_INPUT | GPIO_PULL_UP;
        break;

      case GPIO_MODE_OUTPUT_INPDSB :
        uFlags |= GPIO_OUTPUT;
        uFlags |= GPIO_OUTPUT_INIT_LOGICAL;
        break;

      case GPIO_MODE_OUTPUT_INPENB :
        uFlags |= GPIO_OUTPUT;
        uFlags |= GPIO_INPUT;
        uFlags |= GPIO_OUTPUT_INIT_LOGICAL;
        break;

      default :
        break;
    }

    // configure the output
    bStatus |= ( gpio_pin_configure_dt( &ptPinDef->tGpioDt, uFlags ) != 0 ) ? TRUE : FALSE;
  }      
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function Gpio_Close
 *
 * @brief close all GPIO functions
 *
 * This function will turn off all GPIO interrups
 *
 *****************************************************************************/
void Gpio_Close( void )
{
}

/******************************************************************************
 * @function Gpio_Set
 *
 * @brief set a GPIO pin 
 *
 * This function will set a GPIO pin to a given state
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   bState    desired state of the pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Set( GPIOPINENUM eGpioSel, BOOL bState )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptPinDef;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptPinDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];

    // check for an output pin
    if (( ptPinDef->eMode != GPIO_MODE_OUTPUT_INPDSB ) || ( ptPinDef->eMode != GPIO_MODE_OUTPUT_INPENB ))
    {
      // set the pin to the state
      gpio_pin_set_dt( &ptPinDef->tGpioDt, bState );
    }
    else
    {
      // set the error
      eError = GPIO_ERR_ILLDIR;
    }    
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Get
 *
 * @brief get a GPIO pin's value
 *
 * This function will return a GPIO's pin value
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[io]   pbState   pointer to the strorage element to return the state
 *                        of the GPIO pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Get( GPIOPINENUM eGpioSel, PBOOL pbState )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptPinDef;

  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptPinDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];

    // get the state
    *pbState = gpio_pin_get_dt( &ptPinDef->tGpioDt );
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_GetIrq
 *
 * @brief get a GPIO IRQ pin's value
 *
 * This function will return a GPIO's pin value
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[io]   pbState   pointer to the strorage element to return the state
 *                        of the GPIO pin
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_GetIrq( GPIOIRQENUM eGpioSel, PBOOL pbState )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  
  // test for valid GPIO select
  if ( eGpioSel < GPIO_IRQ_ENUM_MAX )
  {
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Toggle
 *
 * @brief toggle a GPIO pin
 *
 * This function will toggle the state of a GPIO pin
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Toggle( GPIOPINENUM eGpioSel )
{
  GPIOERR     eError = GPIO_ERR_NONE;
  PGPIOPINDEF ptPinDef;

  // test for valid GPIO select
  if ( eGpioSel < GPIO_PIN_ENUM_MAX )
  {
    // get a pointer to the definition
    ptPinDef = ( PGPIOPINDEF )&g_atGpioPinDefs[ eGpioSel ];

    // toggle it
    gpio_pin_toggle_dt( &ptPinDef->tGpioDt );
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLGPIO;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Ioctl
 *
 * @brief GPIO I/O control
 *
 * This function allows the state, interrupt level, interrupt callback and
 * other parameters to be set/retrieved
 *
 * @param[in]   eGpioSel  GPIO pin enumeration
 * @param[in]   eGpioAct  IOCTL action
 * @param[i0]   pvData    pointer to the data to set/retrieve
 *
 * @return      GPIOERR   appropriate error if any
 *
 *****************************************************************************/
GPIOERR Gpio_Ioctl( GPIOPINENUM eGpioSel, GPIOACT eGpioAct, PVOID pvData )
{
  GPIOERR           eError = GPIO_ERR_NONE;

  // return the error
  return( eError );
}

/**@} EOF Gpio.c */
