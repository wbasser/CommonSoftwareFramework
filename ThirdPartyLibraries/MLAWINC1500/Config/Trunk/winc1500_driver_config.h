/******************************************************************************
 * @file winc1500_driver_config.c
 *
 * @brief WINC1500 driver config implementation
 *
 * This file provides the confriguration for the driver
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
 * \addtogroup winc1500_driver_config
 * @{
 *****************************************************************************/

#ifndef __WINC1500_DRIVER_CONFIG_H
#define __WINC1500_DRIVER_CONFIG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
// define the pointer size
#define M2M_POINTER_SIZE_IN_BYTES               ( 4 )

// comment out those features not needed
#define M2M_ENABLE_ERROR_CHECKING        // error checking code
#define M2M_ENABLE_PRNG                  // prng (psuedo-random number) code
#define M2M_ENABLE_SOFT_AP_MODE          // SoftAP feature
#define M2M_ENABLE_WPS                   // WPS feature
#define M2M_WIFI_ENABLE_P2P              // P2P feature
#define M2M_ENABLE_HTTP_PROVISION_MODE   // HTTP provisioning mode
#define M2M_ENABLE_SCAN_MODE             // Wi-Fi scanning
#define M2M_ENABLE_SPI_FLASH             // supports host firmware update utility

// The WINC1500, by default, outputs debug information from its UART.  Unless 
// debugging, this should be disabled to save power and increase WINC1500 efficiency.
// Comment out this define to enable the WINC1500 debug output.
#define M2M_DISABLE_FIRMWARE_LOG

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/**@} EOF winc1500_driver_config.h */

#endif // __WINC1500_DRIVER_CONFIG_H
