/******************************************************************************
 * @file Gpio.c
 *
 * @brief GPIO implementation
 *
 * This file provides the implementation for the HAL GPio module
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup Gpio
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GPIO/Gpio.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the shift counts
#define PORT_SHIFT_CNT              ( 4 )
#define PIN_SHIFT_CNT               ( 1 )

/// define the masks
#define VALID_ENUM_MASK             ( 0x80 )
#define PORT_PIN_MASK               ( 0x07 )
#define DIR_INV_MASK                ( 0x01 )

/// define the macro for generating the GPIO enumerator
#define GEN_ENUM( port, pin, invert ) \
  ( VALID_ENUM_MASK | ( port << PORT_SHIFT_CNT ) | ( pin << PIN_SHIFT_CNT ) | invert )
  
/// define the macros for extracting port/pin/dir/invert from enum
#define EXT_PORT( enum ) \
  (( enum >> PORT_SHIFT_CNT ) & PORT_PIN_MASK )
#define EXT_PIN( enum ) \
  (( enum >> PIN_SHIFT_CNT ) & PORT_PIN_MASK )
#define EXT_DIR( enum ) \
  (( enum >> DIR_SHIFT_CNT ) & DIR_INV_MASK )
#define EXT_INV( enum ) \
  ( enum & DIR_INV_MASK )

  
// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the port register structure
typedef struct _PORTREG
{
  PU8 pnPortReg;
  PU8 pnDirReg;
  PU8 pnPinReg;
  PU8 pnMaskReg;
} PORTREG, *PPORTREG;
#define PORTREG_SIZE                sizeof( PORTREG )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PVGPIOIRQCALLBACK   apIrqCallback[ GPIO_PORT_MAX ];

// local function prototypes --------------------------------------------------
static  void  GetRegisters( GPIOPORT ePort, PPORTREG ptRegs );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Gpio_ConfigurePin
 *
 * @brief configure a GPIO pin
 *
 * This function will configure a GPIO pin for direction, mode and set the 
 * initial state
 *
 * @param[in]   ePort       port 
 * @param[in]   nPin        pin number
 * @param[in]   eDir        direction
 * @param[in]   eMode       Mode
 * @param[in]   bInvert     inverted state
 * @param[in]   bInitial    initial value
 * @param[in]   pvCallback  callback if this pin is an interrupt
 *
 * @return      port enumeration
 *
 *****************************************************************************/
GPIOPINENUM Gpio_ConfigurePin( GPIOPORT ePort, U8 nPin, GPIODIR eDir, GPIOMODE eMode, BOOL bInvert, BOOL bInitial, PVGPIOIRQCALLBACK pvCallback )
{
  GPIOPINENUM eEnum = 0;
  PORTREG     tRegs;
  U8          nMask;
  
  // check for valid port
  if ( ePort < GPIO_PORT_MAX )
  {
    // get the registers
    GetRegisters( ePort, &tRegs );
    
    // calculate the mask
    nMask = BIT( nPin );
    
    // set the initial state
    if ( bInitial )
    {
      // set the state
      *( tRegs.pnPortReg ) |= nMask;
    }
    else
    {
      // clear the state
      *( tRegs.pnPortReg ) &= ~nMask;
    }

    // set the direction
    *( tRegs.pnDirReg ) |= nMask;

    // check for an interrupt
    if ( pvCallback != NULL )
    {
      // store the callback
      apIrqCallback[ ePort ] = pvCallback;

      // set the interrupt mask
      *( tRegs.pnMaskReg ) |= nMask;

      // set the GIFR mask 
      GIMSK |= BIT(( PCIE0 + ePort ));
    }

    // create the enumerator
    eEnum = GEN_ENUM( ePort, nPin, bInvert );
  }
  
  // return the enumerator
  return( eEnum );
}
 
/******************************************************************************
 * @function Gpio_Set
 *
 * @brief set a bit in a GPIO
 *
 * This function will set the bit inside of a GPIO
 *
 * @param[in]   eEnum   GPIOP pin enum
 * @param[in]   bState  state of the pin
 *
 * @return      appropriate error
 *
 *****************************************************************************/
GPIOERR Gpio_Set( GPIOPINENUM eEnum, BOOL bState )
{
  GPIOERR   eError = GPIO_ERR_NONE;
  U8        nMask;
  PORTREG   tRegs;

  // test for a valid GPIO port
  if (( eEnum & VALID_ENUM_MASK ) == VALID_ENUM_MASK )
  {
    // get the registers
    GetRegisters( EXT_PORT( eEnum ), &tRegs );

    // get the mask
    nMask = BIT( EXT_PIN( eEnum ));

    // now set/clear the bit
    bState ^= EXT_INV( eEnum );
    if ( bState )
    {
      // set the pin
      *( tRegs.pnPortReg ) |= nMask;
    }
    else
    {
      // clear the pin
      *( tRegs.pnPortReg ) &= ~nMask;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Get
 *
 * @brief get a bit 
 *
 * This function will read the port and return the state of a pin
 *
 * @param[in]   eEnum   GPIOP pin enum
 * @param[io]   pbState state of the pin
 *
 * @return      appropriate error
 *
 *****************************************************************************/
GPIOERR Gpio_Get( GPIOPINENUM eEnum, PBOOL pbState )
{
  GPIOERR eError = GPIO_ERR_NONE;
  U8        nMask;
  PORTREG   tRegs;

  // test for a valid GPIO port
  if (( eEnum & VALID_ENUM_MASK ) == VALID_ENUM_MASK )
  {
    // get the registers
    GetRegisters( EXT_PORT( eEnum ), &tRegs );

    // get the mask
    nMask = BIT( EXT_PIN( eEnum ));

    // read the value
    *( pbState ) = *( tRegs.pnPinReg ) & nMask;
    *( pbState ) ^= EXT_INV( eEnum );
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_Toggle
 *
 * @brief toggle a GPIO pin
 *
 * This function will toggle the GPIO pin
 *
 * @param[in]   eEnum   pin enum
 *
 * @return      appropriate error
 *
 *****************************************************************************/
GPIOERR Gpio_Toggle( GPIOPINENUM eEnum )
{
  GPIOERR eError = GPIO_ERR_NONE;
  U8        nMask;
  PORTREG   tRegs;

  // test for a valid GPIO port
  if (( eEnum & VALID_ENUM_MASK ) == VALID_ENUM_MASK )
  {
    // get the registers
    GetRegisters( EXT_PORT( eEnum ), &tRegs );

    // get the mask
    nMask = BIT( EXT_PIN( eEnum ));

    // now read the current state
    nMask = ( *( tRegs.pnPinReg ) & nMask ) ^ nMask;

    // now set it
    if ( nMask != 0 )
    {
      // set the pin
      *( tRegs.pnPortReg ) |= nMask;
    }
    else
    {
      // clear the pin
      *( tRegs.pnPortReg ) &= ~nMask;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Gpio_SetDir
 *
 * @brief set the direction of a pin
 *
 * This function will set the appropriate direction
 *
 * @param[in]   eEnum   GPIOP pin enum
 * @param[io]   eDir    direction of the pin
 *
 * @return      appropriate error
 *
 *****************************************************************************/
GPIOERR Gpio_SetDir( GPIOPINENUM eEnum, GPIODIR eDir )
{
  GPIOERR eError = GPIO_ERR_NONE;
  U8        nMask;
  PORTREG   tRegs;

  // test for a valid GPIO port
  if (( eEnum & VALID_ENUM_MASK ) == VALID_ENUM_MASK )
  {
    // get the registers
    GetRegisters( EXT_PORT( eEnum ), &tRegs );

    // get the mask
    nMask = BIT( EXT_PIN( eEnum ));

    // set the direction
    if ( eDir == GPIO_DIR_OUT )
    {
      // set the pin as an output
      *( tRegs.pnDirReg ) |= nMask;

      // clear it
      *( tRegs.pnMaskReg ) &= ~nMask;
    }
    else
    {
      // set the pin as an input
      *( tRegs.pnDirReg ) &= ~nMask;
    }
  }
  else
  {
    // set the error
    eError = GPIO_ERR_ILLENUM;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function GetRegisters
 *
 * @brief get the registers
 *
 * This function sets the port addresses in the control structure
 *
 * @param[in]   ePort     port
 * @param[io]   ptRegs    pointer to the control structure
 *
 *****************************************************************************/
static void GetRegisters( GPIOPORT ePort, PPORTREG ptRegs )
{
  // set the controls
  switch( ePort )
  {
    #ifdef PORTA
    case GPIO_PORT_A :
      ptRegs->pnPortReg = ( PU8 )&PORTA;
      ptRegs->pnDirReg = ( PU8 )&DDRA;
      ptRegs->pnPinReg = ( PU8 )&PINA;
      ptRegs->pnMaskReg = ( PU8 )&PCMSK0;
      break;
    #endif  // PORTA

    #ifdef PORTB
    case GPIO_PORT_B :
      ptRegs->pnPortReg = ( PU8 )&PORTB;
      ptRegs->pnDirReg = ( PU8 )&DDRB;
      ptRegs->pnPinReg = ( PU8 )&PINB;
      ptRegs->pnMaskReg = ( PU8 )&PCMSK1;
      break;
    #endif  // PORTB

    #ifdef PORTC
    case GPIO_PORT_C :
      ptRegs->pnPortReg = ( PU8 )&PORTC;
      ptRegs->pnDirReg = ( PU8 )&DDRC;
      ptRegs->pnPinReg = ( PU8 )&PINC;
      ptRegs->pnMaskReg = ( PU8 )&PCMSK2;
      break;  
    #endif  // PORTC

    #ifdef PORTD
    case GPIO_PORT_D :
      ptRegs->pnPortReg = ( PU8 )&PORTD;
      ptRegs->pnDirReg = ( PU8 )&DDRD;
      ptRegs->pnPinReg = ( PU8 )&PIND;
      ptRegs->pnMaskReg = ( PU8 )&PCMSK3;
      break;
    #endif  // PORTD
      
    default :
      break;
  }
}

#ifdef PCINT0_vect
/******************************************************************************
 * @function PCINT0_vect
 *
 * @brief PCINT0 interrupt vector
 *
 * This provides the interrupt handler for PCINt0
 *
 *****************************************************************************/
ISR( PCINT0_vect )
{
  PVGPIOIRQCALLBACK   pIrqCallback;

  // if not null
  if (( pIrqCallback = apIrqCallback[ GPIO_PORT_A ] ) != NULL )
  {
    pIrqCallback( PCMSK0 & PINA, PINA );
  }
}
#endif

#ifdef PCINT1_vect
/******************************************************************************
 * @function PCINT1_vect
 *
 * @brief PCINT1 interrupt vector
 *
 * This provides the interrupt handler for PCINt1
 *
 *****************************************************************************/
ISR( PCINT1_vect )
{
  PVGPIOIRQCALLBACK   pIrqCallback;

  // if not null
  if (( pIrqCallback = apIrqCallback[ GPIO_PORT_B ] ) != NULL )
  {
    pIrqCallback( PCMSK1 & PINB, PINB );
  }
}
#endif

#ifdef PCINT2_vect
/******************************************************************************
 * @function PCINT2_vect
 *
 * @brief PCINT2 interrupt vector
 *
 * This provides the interrupt handler for PCINt2
 *
 *****************************************************************************/
ISR( PCINT2_vect )
{
  PVGPIOIRQCALLBACK   pIrqCallback;

  // if not null
  if (( pIrqCallback = apIrqCallback[ GPIO_PORT_C ] ) != NULL )
  {
    pIrqCallback( PCMSK2 & PINC, PINC );
  }
}
#endif

#ifdef PCINT3_vect
/******************************************************************************
 * @function PCINT3_vect
 *
 * @brief PCINT3 interrupt vector
 *
 * This provides the interrupt handler for PCINt3
 *
 *****************************************************************************/
ISR( PCINT3_vect )
{
  PVGPIOIRQCALLBACK   pIrqCallback;

  // if not null
  if (( pIrqCallback = apIrqCallback[ GPIO_PORT_D ] ) != NULL )
  {
    pIrqCallback( PCMSK0 & PIND, PIND );
  }
}
#endif

/**@} EOF Gpio.c */
