/******************************************************************************
 * @file USBHandler_cfg.c
 *
 * @brief USB handler configuration implementation 
 *
 * This file provides the implementation for the configuration of the USB
 * handler
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UsbGenHandler/UsbGenHandler_cfg.h"

// library includes -----------------------------------------------------------
#include "CommunicationsManager/CommunicationsManager.h"
#include "USB/Usb.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the DCD interface descriptors
typedef enum _INTFDESCENUM
{
  INTFDESC_ENUM_CDCCCI = 0,
  INTFDESC_ENUM_CDCDCI,
} INTFDESCENUM;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8                nCurrentCdcConnection;
static  USBCDCLINECODING  tCdcLineCoding;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const ALIGNED4 USBDEVDDESCRIPTOR g_tUsbDevDescriptor =
{
  .tHeader                =
  {
    .nLength              = USBDEVDDESCRIPTOR_SIZE,
    .nDescriptorType      = USB_DESCTYPES_DEVICE,
  },
  .wBcdUSB                = USB_VERSION_BCD( 1, 1, 0 ),
  .nDeviceClass           = USB_CDCDESCCSCP_CDCCLASS,
  .nDeviceSubClass        = USB_CDCDESCCSCP_NOSPECIFICSUBCLASS,
  .nDeviceProtocol        = USB_CDCDESCCSCP_NOSPECIFICPROTOCOL,
  .nMaxPacketSize         = USB_MAX_PACKET_SIZE,
  .wVendor                = 0x03EB,
  .wProduct               = 0x2044,
  .wBcdDevice             = USB_VERSION_BCD( 0, 0, 1 ),
  .nManufacturer          = USB_STRINDEX_MANUF,
  .nProduct               = USB_STRINDEX_PROD,
  .nSerialNumber          = USB_STRINDEX_SERN,
  .nNumConfigurations     = 1,
};

const ALIGNED4 USBCONFIG g_tUsbConfig =
{
  // configuration descriptor
  .tCfgDescriptor         =
  {
    .tHeader              =
    {
      .nLength            = USBINTERFACEDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_CONFIGURATION,
    },
    .wTotalLength         = USBCONFIG_SIZE,
    .nNumInterfaces       = 2,
    .nConfigurationValue  = 1,
    .nConfiguration       = 0,
    .nAttributes          = ( USB_DESCATTRS_RESERVED | USB_DESCATTRS_SELFPOWERED ),
    .nMaxPower            = USB_CONFIG_POWER_MA( 0 ),
  },


  // Interface descriptor
  .tCdcCCIInterface       =
  {
    .tHeader              =
    {
      .nLength            = USBINTERFACEDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_INTERFACE,
    },
    .nInterfaceNumber     = INTFDESC_ENUM_CDCCCI,
    .nAlternateSetting    = 0,
    .nNumEndpoints        = 1,
    .nInterfaceClass      = USB_CDCDESCCSCP_CDCCLASS,
    .nInterfaceSubClass   = USB_CDCDESCCSCP_ACMSUBCLASS,
    .nInterfaceProtocol   = USB_CDCDESCCSCP_ATCOMMANDPROTOCOL,
    .nInterface           = 0,
  },
  
  // function header
  .tCdcFuncHeader         =
  {
    .tHeader              =
    {
      .nLength            = USBHDRFUNCDESCIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_CSINTERFACE,
    },
    .nDescriptorSubType   = USB_CDCDSUBTYPES_CSINTF_HEADER,
    .wBcdCDC              = USB_VERSION_BCD( 1, 1, 0 ),
  },

  // function ACM
  .tCdcFuncAcm            =
  {
    .tHeader              =
    {
      .nLength            = USBACMDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_CSINTERFACE,
    },
    .nDescriptorSubType   = USB_CDCDSUBTYPES_CSINTF_ACM,
    .nCapabilities        = 0x06,
  },

  // function union
  .tCdcFuncUnion          =
  {
    .tHeader              =
    {
      .nLength            = USBUNIONFUNCDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_CSINTERFACE,
    },
    .nDescriptorSubType   = USB_CDCDSUBTYPES_CSINTF_UNION,
    .nMasterInterface     = INTFDESC_ENUM_CDCCCI, 
    .nSlaveInterface      = INTFDESC_ENUM_CDCDCI,
  },

  // control endpoint
  .tCdcControlEndpoint    =
  {
    .tHeader              =
    {
      .nLength            = USBENDPOINTDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_ENDPOINT,
    },
    .nEndpointAddress     = USB_CDCENDPOINT_CTL | USB_ENDPOINT_IN,
    .nAttributes          = USB_ENDPNTTYPE_INTERRUPT,
    .wMaxPacketSize       = USB_MAX_PACKET_SIZE,
    .nInterval            = 0xFF,
  },

  // CDC Data Interface
  .tCdcDCIInterface       =
  {
    .tHeader              =
    {
      .nLength            = USBINTERFACEDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_INTERFACE,
    },
    .nInterfaceNumber     = INTFDESC_ENUM_CDCDCI,
    .nAlternateSetting    = 0,
    .nNumEndpoints        = 2,
    .nInterfaceClass      = USB_CDCDESCCSCP_CDCDATACLASS,
    .nInterfaceSubClass   = USB_CDCDESCCSCP_NODATASUBCLASS,
    .nInterfaceProtocol   = USB_CDCDESCCSCP_NODATAPROTOCOL,
    .nInterface           = 0,
  },

  // data out endpoint
  .tCdcDataOutEndpoint    =
  {
    .tHeader              =
    {
      .nLength            = USBENDPOINTDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_ENDPOINT,
    },
    .nEndpointAddress     = USB_CDCENDPOINT_RCV,
    .nAttributes          = USB_ENDPNTTYPE_BULK,
    .wMaxPacketSize       = USB_MAX_PACKET_SIZE,
    .nInterval            = 0,
  },

  // data in endpoint
  .tCdcDataInEndpoint     =
  {
    .tHeader              =
    {
      .nLength            = USBENDPOINTDESCRIPTOR_SIZE,
      .nDescriptorType    = USB_DESCTYPES_ENDPOINT,
    },
    .nEndpointAddress     = USB_CDCENDPOINT_XMT | USB_ENDPOINT_IN,
    .nAttributes          = USB_ENDPNTTYPE_BULK,
    .wMaxPacketSize       = USB_MAX_PACKET_SIZE,
    .nInterval            = 0,
  },
};

/// inatantiate the zero string descriptor
const ALIGNED4 USBSTRINGZERODESCRIPTOR  g_tUsbStringZeroDescriptor =
{
  .tHeader              =
  {
    .nLength            = USBSTRINGZERODESCRIPTOR_SIZE,
    .nDescriptorType    = USB_DESCTYPES_STRING,
  },
  .wLANGID              = 0x0409,
};

/// define the strings
const ALIGNED4 USBSTRINGDESCRIPTOR g_tUsbStringDescriptor[ ] =
{
  // do not remove this entry
  USB_STRING_DESCRIPTOR( L"" ),

  // populate with the below macro
  // USB_STRING_DESCRIPTOR( String )
  USB_STRING_DESCRIPTOR( L"Cyber Integration" ),
  USB_STRING_DESCRIPTOR( L"Test Device" ),
  USB_STRING_DESCRIPTOR( L"E00000001" ),
};

/// initialize the line coding
const ALIGNED4 USBCDCLINECODING  tCdcLineCoding =
{
  // set up the default CDC line coding parameters
  .uBaudRate    = 115200,
  .nCharFormat  = 0,
  .nParityType  = 0,
  .nDataBits    = 8
};

/******************************************************************************
 * @function UsbGenHandler_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any needed local initliazaiton
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL UsbGenHandler_LocalInitialize( void )
{
  // clear the current CDC connection
  nCurrentCdcConnection = 0;

  // return OK
  return( FALSE );
}

/******************************************************************************
 * @function UsbGenHandler_ProcessControlCallback
 *
 * @brief process control callback
 *
 * This function will handle any additional callbacks for control packets
 *
 * @param[in]   nConfig   config value
 *
 *****************************************************************************/
void UsbGenHandler_ProcessControlCallback( U8 nConfig )
{
  // start receiving characters
  CommunicationsManager_StartReceive( );
}

/******************************************************************************
 * @function UsbGenHandler_HandleOtherRequests
 *
 * @brief handle other requests
 *
 * This function willl parse and handle other requests not processed by the
 * the standard request handler
 *
 * @param[in]   ptRequest     request buffer
 *
 * @return    TRUE if handled, otherwise false
 *
 *****************************************************************************/
BOOL UsbGenHandler_HandleOtherRequests( PUSBREQUEST ptRequest )
{
  BOOL bHandled = TRUE;
         
    // determine the request
  switch( MAKEU16( ptRequest->nRequest, ptRequest->nRequestType ))
  {
    case USB_CMD( GETLINECODING, IN, CLASS, INTERFACE ) :
      // send the current configuration
      Usb_ControlSend(( PU8 )&tCdcLineCoding, USBCDCLINECODING_SIZE );
      break;

    case USB_CMD( SETLINECODING, OUT, CLASS, INTERFACE ) :
      // send a zero length packet
      Usb_ControlSendZlp( );
      break;

    case USB_CMD( SETCTRLLINESTATE, OUT, CLASS, INTERFACE ) :
      // set the current connection/send a zero length packet
      nCurrentCdcConnection = ( U8 )ptRequest->wValue;
      Usb_ControlSendZlp( );
      break;

    default :
      bHandled = FALSE;
      break;
  }

  // return the handled status
  return( bHandled );
}
 
/**@} EOF USBHandler_cfg.c */
