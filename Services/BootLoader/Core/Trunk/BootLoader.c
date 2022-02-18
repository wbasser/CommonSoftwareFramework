/******************************************************************************
 * @file BootLoader.c
 *
 * @brief Boot Loader implementation
 *
 * This file provides the implementation for the boot laoder
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup BootLoader
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------
#include  "Interrupt/Interrupt.h"
#include  "Main/Main.h"

// local includes -------------------------------------------------------------
#include "BootLoader/BootLoader.h"

// Macros and Defines ---------------------------------------------------------

// local function prototypes --------------------------------------------------
static  BOOL  bBootRequestActive;

/******************************************************************************
 * @function BootLoader_Enter
 *
 * @brief enter the boot loader
 *
 * This function will turn off all interrupts, shutdown the micro, reset the 
 * vector table, reset the stack pointer and jump to the boot laoder
 *
 * @return  TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL BootLoader_Initialize( void )
{
  // clear the boot load request active flg
  bBootRequestActive = FALSE;
  
  // return false for no errors
  return( FALSE );
}
 
/******************************************************************************
 * @function BootLoader_Enter
 *
 * @brief enter the boot loader
 *
 * This function will turn off all interrupts, shutdown the micro, reset the 
 * vector table, reset the stack pointer and jump to the boot laoder
 *
 *****************************************************************************/
void BootLoader_Enter( void )
{
  // set the boot load request active flg
  bBootRequestActive = TRUE;
}

/******************************************************************************
 * @function BootLoader_ProcessCallback
 *
 * @brief enter the boot loader
 *
 * This function will turn off all interrupts, shutdown the micro, reset the 
 * vector table, reset the stack pointer and jump to the boot laoder
 *
 *****************************************************************************/
void BootLoader_ProcessCallback( U8 nOption )
{
  U32 uBootLoadSignature;

  // check for a boot request active
  if ( bBootRequestActive )
  {
    /// disable all interrupt
    Interrupt_Disable( );

    // shut down the microprocessor
    Main_Shutdown( );

    // load the boot load signature with the magic key
    uBootLoadSignature = RAM_START_ADDRESS;
    *( PU32 )uBootLoadSignature = BOOT_LOAD_KEY;

    // set the boot loader entry
    NVIC_SystemReset( );
  }
}

/**@} EOF BootLoader.c */
