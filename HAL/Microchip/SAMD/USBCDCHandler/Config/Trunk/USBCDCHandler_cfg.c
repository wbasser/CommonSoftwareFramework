/******************************************************************************
 * @file USBCDCHandler_cfg.c
 *
 * @brief USB-CDC handler configuration implementation
 *
 * This file provides the configuration implementation for the USB-CDC handler
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
 * \addtogroup USBCBCHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include "SystemConfig/SystemConfig_prm.h"

// local includes -------------------------------------------------------------
#include "USBCDCHandler/USBCDCHandler_cfg.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "GPIO/Gpio.h"
#include "PowerManager/PowerManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

/// enumerate the standard request types

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/// define the strings
const W16 g_awStrings[ USBCDCHAND_STRING_MAX ][ 40 ] =
{
  L"",
  L"Cyber Integration",
  L"HC11 Test",
  L"E00000001"
};

/******************************************************************************
 * @function USBCDCHandler_LocalInitialize
 *
 * @brief USB CDC handler initialization
 *
 * This function will implement any needed initialization
 *
 *****************************************************************************/
void USBCDCHandler_LocalInitialize( void )
{
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL ) || ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKSCHEDULER )
    // enable the USB power
    PowerManager_DisableEnablePeriphB( PM_APBBMASK_USB, ON );

    // enable the clock
    Clock_PeriphEnable( CLOCK_MUXID_USB, CLOCK_GENID_0 );

    // set the I/O pads
    Gpio_Configure( SYSTEM_CONFIG_USB_PORT, SYSTEM_CONFIG_USB_DMPPIN, GPIO_MODE_OUTPUT_INPENB, OFF, SYSTEM_CONFIG_USB_FUNCMUX, FALSE );
    Gpio_Configure( SYSTEM_CONFIG_USB_PORT, SYSTEM_CONFIG_USB_DMNPIN, GPIO_MODE_OUTPUT_INPENB, OFF, SYSTEM_CONFIG_USB_FUNCMUX, FALSE );
  #endif
}

/**@} EOF USBCDCHandler_cfg.c */
