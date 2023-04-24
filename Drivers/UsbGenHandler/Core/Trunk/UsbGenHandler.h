/******************************************************************************
 * @file UsbGenHandler.h
 *
 * @brief USB handler declarations
 *
 * This file provides the declarations for the USB handler
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
 * \addtogroup UsbGenHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBGENHANDLER_H
#define _USBGENHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UsbGenHandler/UsbGenHandler_cfg.h"
#include "USB/Usb.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the callback structure
typedef void ( *PVSNDRCVCALLBACK )( U16 wCount );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  UsbGenHandler_Initialize ( void );
extern  void  UsbGenHandler_Send( U8 nEndpoint, PU8 pnData, U16 wLength, PVSNDRCVCALLBACK pvCallback );
extern  void  UsbGenHandler_Recv( U8 nEndpoint, PU8 pnData, U16 wLength, PVSNDRCVCALLBACK pvCallback );
extern  void  UsbGenHandler_HandleStandardRequest( PUSBREQUEST ptRequest );
extern  void  UsbGenHandler_RecvCallback( U8 nEndpoint, U16 wCount );
extern  void  UsbGenHandler_SendCallback( U8 nEndpoint, U16 wCount );
extern  BOOL  UsbGenHandler_IsConfigured( void );

/**@} EOF UsbGenHandler.h */

#endif  // _USBGENHANDLER_H