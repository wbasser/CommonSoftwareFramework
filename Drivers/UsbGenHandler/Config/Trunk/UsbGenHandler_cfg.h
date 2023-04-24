/******************************************************************************
 * @file USBHandler_cfg.h
 *
 * @brief USB handler configuration declarations
 *
 * This file provides the declara
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
 * \addtogroup USBHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _USBHANDLER_CFG_H
#define _USBHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UsbGenHandler/UsbGenHandler_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the auto connector enable macro
#define USBGENHANDLER_ENABLE_AUTOATTACH             ( ON )

/// define the serial number as a GUID string macro
#define USBGENHANDLER_ENABLE_GUIDSERIAL             ( OFF )

// enumerations ---------------------------------------------------------------
/// enumerate the CDC endpoints
typedef enum _USBCDCENDPOINT
{
  USB_CDCENDPOINT_RCV = 1,
  USB_CDCENDPOINT_XMT,
  USB_CDCENDPOINT_CTL,
  USB_CDCENDPOINT_MAX
} USBCDCENDPOINT;

/// enumerate the strings
typedef enum _USBSTRINDEX
{
  USB_STRINDEX_LANG = 0,
  USB_STRINDEX_MANUF,
  USB_STRINDEX_PROD,
  USB_STRINDEX_SERN,
  USB_STRINDEX_MAX
} USBSTRINDEX;

// structures -----------------------------------------------------------------
/// define the configuration structure
typedef struct PACKED _USBCONFIG
{
  // configuration descriptor
  USBCFGDESCRIPTOR        tCfgDescriptor;

  // CDC Control Interface
  USBINTERFACEDESCRIPTOR  tCdcCCIInterface;
  USBHDRFUNCDESCIPTOR     tCdcFuncHeader;
  USBACMDESCRIPTOR        tCdcFuncAcm;
  USBUNIONFUNCDESCRIPTOR  tCdcFuncUnion;
  USBENDPOINTDESCRIPTOR   tCdcControlEndpoint;

  // CDC Data Interface
  USBINTERFACEDESCRIPTOR  tCdcDCIInterface;
  USBENDPOINTDESCRIPTOR   tCdcDataOutEndpoint;
  USBENDPOINTDESCRIPTOR   tCdcDataInEndpoint;
} USBCONFIG, *PUSBCONFIG;
#define USBCONFIG_SIZE                          sizeof( USBCONFIG )

// global parameter declarations ----------------------------------------------
extern  const USBDEVDDESCRIPTOR       g_tUsbDevDescriptor;
extern  const USBCONFIG               g_tUsbConfig;
extern  const USBSTRINGZERODESCRIPTOR g_tUsbStringZeroDescriptor;
extern  const USBSTRINGDESCRIPTOR     g_tUsbStringDescriptor[ ];

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
extern  BOOL  UsbGenHandler_LocalInitialize( void );
extern  void  UsbGenHandler_ProcessControlCallback( U8 nConfig );
extern  BOOL  UsbGenHandler_HandleOtherRequests( PUSBREQUEST ptRequest );

/**@} EOF USBHandler_cfg.h */

#endif  // _USBHANDLER_CFG_H