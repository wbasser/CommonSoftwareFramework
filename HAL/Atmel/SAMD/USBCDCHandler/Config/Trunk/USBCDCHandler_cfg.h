/******************************************************************************
 * @file USBCDCHandler_cfg.h
 *
 * @brief USB-CDC handler configuration declarations
 *
 * This file provides the declarations for the USB-CDC handler configuration
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
 * \addtogroup USBCDCHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBCDCHANDLER_CFG_H
#define _USBCDCHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "USBCDCHandler/USBCDCHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the vendor and project IDs
#define USBCDCHAND_VENDOR_ID                    ( 0x03EB )
#define USBCDCHAND_PRODUCT_ID                   ( 0x2044 )

// enumerations ---------------------------------------------------------------
/// enumerate the strings
typedef enum _USBCDCHANDSTRING
{
  USBCDCHAND_STRING_LANG = 0,
  USBCDCHAND_STRING_MANUF,
  USBCDCHAND_STRING_PROD,
  USBCDCHAND_STRING_SERNUM,
  USBCDCHAND_STRING_MAX
} USBCDCHANDSTRING;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const wchar_t       g_awStrings[ ][ 40 ];

// global function prototypes --------------------------------------------------
extern  void USBCDCHandler_LocalInitialize( void );

/**@} EOF USBCDCHandler_cfg.h */

#endif  // _USBCDCHANDLER_CFG_H