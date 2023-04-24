/******************************************************************************
 * @file Interrupt.c
 *
 * @brief interrupt manager
 *
 * This file provides the interrupt control and management
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
 * \addtogroup  Interrupt 
 * @{
 *****************************************************************************/
// system includes
#include <zephyr/irq.h>

// local includes -------------------------------------------------------------
#include "Interrupt/Interrupt.h"
#include "Interrupt/Interrupt_prm.h"

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U32   auPrvIrq[ IRQ_STACK_LEVEL ];
static  U8    nIrqIndex;

// local function prototypes --------------------------------------------------

/******************************************************************************
 * @function Interrupt_Initializle
 *
 * @brief intialize the interrupt handler
 *
 * This function resets the stack level index
 *
 *****************************************************************************/
void Interrupt_Initialize( void )
{
  // reset the index
  nIrqIndex = 0;
}


/******************************************************************************
 * @function Interrupt_Disable
 *
 * @brief disable the global interrupts
 *
 * This function will disable the interrupts and if there is room in the stack
 * store the previous function of the interrupt enable state
 *
 *****************************************************************************/
void Interrupt_Disable( void )
{
   // if room, store previous
   if ( nIrqIndex < IRQ_STACK_LEVEL )
   {
     // save current value, clear interrupt
     auPrvIrq[ nIrqIndex++ ] = irq_is_enabled( 1 );
   }

   // clear interrupt enable
   irq_disable( 1 );

}

/******************************************************************************
 * @function Interrupt_Enable
 *
 * @brief restores the previous state of the interrupt enable
 *
 * This function will restore the previous state if one is in the stack
 * otherwise it will just enable interrupts.
 *
 * @return  the current interrupt enable state
 *
 *****************************************************************************/
BOOL Interrupt_Enable( void )
{
  BOOL  bIrqStatus = FALSE;
  
  // if items on stack
  if ( nIrqIndex > 0 )
  {
    // restore previous state
    bIrqStatus = ( auPrvIrq[ --nIrqIndex ] );
  }
  else
  {
    // just enable interrupts
    bIrqStatus = TRUE;
  }

  //  test for enable
  if ( bIrqStatus )
  {
    irq_enable( 1 );
  }

  // return the state of the interrupts
  return( bIrqStatus );
}

void HardFault_Handler( void )
{
  FOREVER
  {
  }
}

/**@} EOF Interrupt.c */




