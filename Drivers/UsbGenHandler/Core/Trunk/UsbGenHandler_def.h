/******************************************************************************
 * @file USBHandler_def.h
 *
 * @brief USB handler defintions declarations
 *
 * This file provides the defintion declarations for the USB handler
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
#ifndef _USBHANDLER_DEF_H
#define _USBHANDLER_DEF_H

// system includes ------------------------------------------------------------
#include <wchar.h>

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro for creating a command
#define USB_CMD(cmd, dir, type, rcpt) \
    ((USB_STDREQ_##cmd << 8) | (USB_DIR_##dir << 7) | (USB_REQTYPE_##type << 5) | (USB_RECIPIENT_##rcpt << 0))

/// define the macro for creating a BCD version
#define USB_VERSION_BCD(Maj, Min, Rev)  (((Maj & 0x0F) << 8) | ((Min & 0x0F) << 4) | ( Rev & 0xFF))

/// define the macro for creating a string
//#define USB_STRING_DESCRIPTOR( String ) \
  //{ \
  //  .tHeader = \
  //  { \
  //    .nLength = USBDESCHEADR_SIZE + ( sizeof(String) - 2 ), \
  //    .nDescriptorType = USB_DESCTYPES_STRING, \
  //  }, \
  //  .awString = ( W16 )String, \
  //}

/// define the macro for computing configuration power setting
#define USB_CONFIG_POWER_MA( power )  ( power >> 1 )

// enumerations ---------------------------------------------------------------
/// enumerate the standard request
typedef enum _USBSTDREQ
{
  USB_STDREQ_GETSTATUS                      = 0,
  USB_STDREQ_CLEARFEATURE                   = 1,
  USB_STDREQ_SETFEATURE                     = 3,
  USB_STDREQ_SETADDRESS                     = 5,
  USB_STDREQ_GETDESCRIPTOR                  = 6,
  USB_STDREQ_SETDESCRIPTOR                  = 7,
  USB_STDREQ_GETCONFIGURATION               = 8,
  USB_STDREQ_SETCONFIGURATION               = 9,
  USB_STDREQ_GETINTERFACE                   = 10,
  USB_STDREQ_SETINTERFACE                   = 11,
  USB_STDREQ_SYNCHFRAME                     = 12,
  USB_STDREQ_SETLINECODING                  = 32,
  USB_STDREQ_GETLINECODING                  = 33,
  USB_STDREQ_SETCTRLLINESTATE               = 34,
} USBSTDREQ;

/// enumerate the request types
typedef enum _USBREQTYPE
{
  USB_REQTYPE_STANDARD                      = 0,
  USB_REQTYPE_CLASS                         = 1,
  USB_REQTYPE_VENDOR                        = 2,
} USBREQTYPE;

/// enumerate the directions
typedef enum _USBDIR
{
  USB_DIR_OUT                               = 0,
  USB_DIR_IN                                = 1,
} USBDIR;

/// enumerate the recipients
typedef enum _USBRECIPIENT
{
  USB_RECIPIENT_DEVICE                      = 0,
  USB_RECIPIENT_INTERFACE                   = 1,
  USB_RECIPIENT_ENDPOINT                    = 2,
  USB_RECIPIENT_OTHER                       = 3,
} USBRECIPIENT;


/// enumerate the standard Descriptor Types
typedef enum _USBDESCTYPES
{
  USB_DESCTYPES_DEVICE                      = 0x01, ///< Indicates that the descriptor is a device descriptor
  USB_DESCTYPES_CONFIGURATION               = 0x02, ///< Indicates that the descriptor is a configuration descriptor
  USB_DESCTYPES_STRING                      = 0x03, ///< Indicates that the descriptor is a string descriptor
  USB_DESCTYPES_INTERFACE                   = 0x04, ///< Indicates that the descriptor is an interface descriptor
  USB_DESCTYPES_ENDPOINT                    = 0x05, ///< Indicates that the descriptor is an endpoint descriptor
  USB_DESCTYPES_DEVICEQUALIFIER             = 0x06, ///< Indicates that the descriptor is a device qualifier descriptor
  USB_DESCTYPES_OTHER                       = 0x07, ///< Indicates that the descriptor is of other type
  USB_DESCTYPES_INTERFACEPOWER              = 0x08, ///< Indicates that the descriptor is an interface power descriptor
  USB_DESCTYPES_OTG                         = 0x09,
  USB_DESCTYPES_DEBUG                       = 0x0A,
  USB_DESCTYPES_INTERFACE_ASSOCIATION       = 0x0B,
  USB_DESCTYPES_BINARY_OBJECT_STORE         = 0x0F,
  USB_DESCTYPES_DEVICE_CAPABILITY           = 0x10,
  USB_DESCTYPES_HID                         = 0x21,
  USB_DESCTYPES_HIDREPORT                   = 0x22,
  USB_DESCTYPES_HIDPHYSICAL                 = 0x23,
  USB_DESCTYPES_CSINTERFACE                 = 0x24, ///< Indicates that the descriptor is a class specific interface descriptor
  USB_DESCTYPES_CSENDPOINT                  = 0x25, ///< Indicates that the descriptor is a class specific endpoint descriptor
} USBDESCTYPES;

/// enumerate the feature selectors
typedef enum _USBFEATURESEL
{
  USB_FEATURESEL_ENDPOINTHALT               = 0x00,
  USB_FEATURESEL_DEVICEREMOTEWAKEUP         = 0x01,
  USB_FEATURESEL_TESTMODE                   = 0x02,
} USBFEATURESEL;

/// enuemrate the sub class 
typedef enum _USBDESCCSCP
{
  USB_DESCCSCP_NODEVICECLASS                = 0x00, ///< Descriptor Class value indicating that the device does not belong to a particular class at the device level.
  USB_DESCCSCP_NODEVICESUBCLASS             = 0x00, ///< Descriptor Subclass value indicating that the device does not belong to a particular subclass at the device level.
  USB_DESCCSCP_NODEVICEPROTOCOL             = 0x00, ///< Descriptor Protocol value indicating that the device does not belong to a particular protocol at the device level.
  USB_DESCCSCP_VENDORSPECIFICCLASS          = 0xFF, ///< Descriptor Class value indicating that the device/interface belongs to a vendor specific class.
  USB_DESCCSCP_VENDORSPECIFICSUBCLASS       = 0xFF, ///< Descriptor Subclass value indicating that the device/interface belongs to a vendor specific subclass.
  USB_DESCCSCP_VENDORSPECIFICPROTOCOL       = 0xFF, ///< Descriptor Protocol value indicating that the device/interface belongs to a vendor specific protocol.
  USB_DESCCSCP_IADDEVICECLASS               = 0xEF, ///< Descriptor Class value indicating that the device belongs to the Interface Association Descriptor class.
  USB_DESCCSCP_IADDEVICESUBCLASS            = 0x02, ///< Descriptor Subclass value indicating that the device belongs to the Interface Association Descriptor subclass.
  USB_DESCCSCP_IADDEVICEPROTOCOL            = 0x01, ///< Descriptor Protocol value indicating that the device belongs to the Interface Association Descriptor protocol.
} USBDESCCSCP;

/// enumerate the CDC sub class protocols
typedef enum _USBCDCDESCCSCP
{
  USB_CDCDESCCSCP_CDCCLASS                  = 0x02, ///< Descriptor Class value indicating that the device or interface belongs to the CDC class.
  USB_CDCDESCCSCP_NOSPECIFICSUBCLASS        = 0x00, ///< Descriptor Subclass value indicating that the device or interface belongs to no specific subclass of the CDC class.
  USB_CDCDESCCSCP_ACMSUBCLASS               = 0x02, ///< Descriptor Subclass value indicating that the device or interface belongs to the Abstract Control Model CDC subclass.
  USB_CDCDESCCSCP_ATCOMMANDPROTOCOL         = 0x01, ///< Descriptor Protocol value indicating that the device or interface belongs to the AT Command protocol of the CDC class.
  USB_CDCDESCCSCP_NOSPECIFICPROTOCOL        = 0x00, ///< Descriptor Protocol value indicating that the device or interface belongs to no specific protocol of the CDC class.
  USB_CDCDESCCSCP_VENDORSPECIFICPROTOCOL    = 0xFF, ///< Descriptor Protocol value indicating that the device or interface belongs to a vendor-specific protocol of the CDC class.
  USB_CDCDESCCSCP_CDCDATACLASS              = 0x0A, ///< Descriptor Class value indicating that the device or interface belongs to the CDC Data class.
  USB_CDCDESCCSCP_NODATASUBCLASS            = 0x00, ///< Descriptor Subclass value indicating that the device or interface belongs to no specific subclass of the CDC data class.
  USB_CDCDESCCSCP_NODATAPROTOCOL            = 0x00, ///< Descriptor Protocol value indicating that the device or interface belongs to no specific protocol of the CDC data class.
} USBCDCDESCCSCP;

// enumerate the CDC descriptor sub types
typedef enum _USBCDCDSUBTYPES
{
  USB_CDCDSUBTYPES_CSINTF_HEADER            = 0x00, ///< CDC class-specific Header functional descriptor
  USB_CDCDSUBTYPES_CSINTF_CALLMANAGEMENT    = 0x01, ///< CDC class-specific Call Management functional descriptor
  USB_CDCDSUBTYPES_CSINTF_ACM               = 0x02, ///< CDC class-specific Abstract Control Model functional descriptor
  USB_CDCDSUBTYPES_CSINTF_DIRECTLINE        = 0x03, ///< CDC class-specific Direct Line functional descriptor
  USB_CDCDSUBTYPES_CSINTF_TELEPHONERINGER   = 0x04, ///< CDC class-specific Telephone Ringer functional descriptor
  USB_CDCDSUBTYPES_CSINTF_TELEPHONECALL     = 0x05, ///< CDC class-specific Telephone Call functional descriptor
  USB_CDCDSUBTYPES_CSINTF_UNION             = 0x06, ///< CDC class-specific Union functional descriptor
  USB_CDCDSUBTYPES_CSINTF_COUNTRYSELECTION  = 0x07, ///< CDC class-specific Country Selection functional descriptor
  USB_CDCDSUBTYPES_CSINTF_TELEPHONEOPMODES  = 0x08, ///< CDC class-specific Telephone Operation Modes functional descriptor
  USB_CDCDSUBTYPES_CSINTF_USBTERMINAL       = 0x09, ///< CDC class-specific USB Terminal functional descriptor
  USB_CDCDSUBTYPES_CSINTF_NETWORKCHANNEL    = 0x0A, ///< CDC class-specific Network Channel functional descriptor
  USB_CDCDSUBTYPES_CSINTF_PROTOCOLUNIT      = 0x0B, ///< CDC class-specific Protocol Unit functional descriptor
  USB_CDCDSUBTYPES_CSINTF_EXTENSIONUNIT     = 0x0C, ///< CDC class-specific Extension Unit functional descriptor
  USB_CDCDSUBTYPES_CSINTF_MULTICHANNEL      = 0x0D, ///< CDC class-specific Multi-Channel Management functional descriptor
  USB_CDCDSUBTYPES_CSINTF_CAPI              = 0x0E, ///< CDC class-specific Common ISDN API functional descriptor
  USB_CDCDSUBTYPES_CSINTF_ETHERNET          = 0x0F, ///< CDC class-specific Ethernet functional descriptor
  USB_CDCDSUBTYPES_CSINTF_ATM               = 0x10, ///< CDC class-specific Asynchronous Transfer Mode functional descriptor
} USBCDCDSUBTYPES;

// enumerate the configuration descriptor attributes
typedef enum _USBDESCATTRS
{
  USB_DESCATTRS_RESERVED                    = 0x80, ///< atrribute reserve should be always be set
  USB_DESCATTRS_SELFPOWERED                 = 0x40, ///< self powered
  USB_DESCATTRS_REMOTEWAKEUP                = 0x20, ///< remote wakeup
} USBDESCATTRS;

// structures -----------------------------------------------------------------
/// define the wide character type
typedef wchar_t   W16;

/// define the request structure
typedef struct PACKED _USBREQUEST
{
  U8            nRequestType;
  U8            nRequest;
  U16           wValue;
  U16           wIndex;
  U16           wLength;
} USBREQUEST, *PUSBREQUEST;
#define USBREQUEST_SIZE                         sizeof( USBREQUEST )

/// define the descriptor header structure
typedef struct PACKED _USBDESCHEADR
{
  U8            nLength;
  U8            nDescriptorType;
} USBDESCHEADR, *PUSBDESCHEADR;
#define USBDESCHEADR_SIZE                       sizeof( USBDESCHEADR )

/// define thye device descriptor
typedef struct PACKED _USBDEVDDESCRIPTOR
{
  USBDESCHEADR  tHeader;
  U16           wBcdUSB;
  U8            nDeviceClass;
  U8            nDeviceSubClass;
  U8            nDeviceProtocol;
  U8            nMaxPacketSize;
  U16           wVendor;
  U16           wProduct;
  U16           wBcdDevice;
  U8            nManufacturer;
  U8            nProduct;
  U8            nSerialNumber;
  U8            nNumConfigurations;
} USBDEVDDESCRIPTOR, *PUSBDEVDDESCRIPTOR;
#define USBDEVDDESCRIPTOR_SIZE                  sizeof( USBDEVDDESCRIPTOR )

/// define the configuration description
typedef struct PACKED _USBCFGDESCRIPTOR
{
  USBDESCHEADR  tHeader;
  U16           wTotalLength;
  U8            nNumInterfaces;
  U8            nConfigurationValue;
  U8            nConfiguration;
  U8            nAttributes;
  U8            nMaxPower;
} USBCFGDESCRIPTOR, *PUSBCFGDESCRIPTOR;
#define USBCFGDESCRIPTOR_SIZE                   sizeof( USBCFGDESCRIPTOR )

/// define the interface descriptor
typedef struct PACKED _USBINTERFACEDESCRIPTOR
{
  USBDESCHEADR  tHeader;
  U8            nInterfaceNumber;
  U8            nAlternateSetting;
  U8            nNumEndpoints;
  U8            nInterfaceClass;
  U8            nInterfaceSubClass;
  U8            nInterfaceProtocol;
  U8            nInterface;
} USBINTERFACEDESCRIPTOR, *PUSBINTERFACEDESCRIPTOR;
#define USBINTERFACEDESCRIPTOR_SIZE             sizeof( USBINTERFACEDESCRIPTOR )

/// define the endpoint descriptor
typedef struct PACKED _USBENDPOINTDESCRIPTOR
{
  USBDESCHEADR  tHeader;
  U8            nEndpointAddress;
  U8            nAttributes;
  U16           wMaxPacketSize;
  U8            nInterval;
} USBENDPOINTDESCRIPTOR, *PUSBENDPOINTDESCRIPTOR;
#define USBENDPOINTDESCRIPTOR_SIZE              sizeof( USBENDPOINTDESCRIPTOR )             

/// define the string zero structure
typedef struct PACKED _USBSTRINGZERODESCRIPTOR
{
  USBDESCHEADR  tHeader;
  U16           wLANGID;
} USBSTRINGZERODESCRIPTOR, *PUSBSTRINGZERODESCRIPTOR;
#define USBSTRINGZERODESCRIPTOR_SIZE            sizeof( USBSTRINGZERODESCRIPTOR )

/// define the string descriptor structure
typedef struct PACKED _USBSTRINGDESCRIPTOR
{
  USBDESCHEADR  tHeader;
  W16           awString[ ];
} USBSTRINGDESCRIPTOR, *PUSBSTRINGDESCRIPTOR;
#define USBSTRINGDESCRIPTOR_SIZE                sizeof( USBSTRINGDESCRIPTOR )

/// define the structure for the HID device
typedef struct PACKED _USBHIDDESCRIPTOR
{
  USBDESCHEADR  tHeader;
  U16           wBcdHID;
  U8            nCountryCode;
  U8            nNumDescriptors;
  U8            nDescriptorType;
} USBHIDDESCRIPTOR, *PUSBHIDDESCRIPTOR;
#define USBHIDDESCRIPTOR_SIZE                   sizeof( USBHIDDESCRIPTOR )

/// define the functional header descriptor
typedef struct PACKED _USBHDRFUNCDESCIPTOR
{
  USBDESCHEADR  tHeader;
  U8            nDescriptorSubType;
  U16           wBcdCDC;
} USBHDRFUNCDESCIPTOR, *PUSBHDRFUNCDESCIPTOR;
#define USBHDRFUNCDESCIPTOR_SIZE                sizeof( USBHDRFUNCDESCIPTOR )

/// the ACM descriptor
typedef struct PACKED _USBACMDESCRIPTOR
{
  USBDESCHEADR  tHeader;
  U8            nDescriptorSubType;
  U8            nCapabilities;
} USBACMDESCRIPTOR, *PUSBACMDESCRIPTOR;
#define USBACMDESCRIPTOR_SIZE                   sizeof( USBACMDESCRIPTOR )

/// define the union function descriptor
typedef struct PACKED _USBUNIONFUNCDESCRIPTOR
{
  USBDESCHEADR  tHeader;
  U8            nDescriptorSubType;
  U8            nMasterInterface;
  U8            nSlaveInterface;
} USBUNIONFUNCDESCRIPTOR, *PUSBUNIONFUNCDESCRIPTOR;
#define USBUNIONFUNCDESCRIPTOR_SIZE             sizeof( USBUNIONFUNCDESCRIPTOR )

/// define the call management function descriptor
typedef struct PACKED _USBCALLLMNGFUNCDESCRIPTOR
{
  USBDESCHEADR  tHeader;
  U8            nDescriptorSubType;
  U8            nCapabilities;
  U8            nDatainterface;
} USBCALLLMNGFUNCDESCRIPTOR, *PUSBCALLLMNGFUNCDESCRIPTOR;
#define USBCALLLMNGFUNCDESCRIPTOR_SIZE          sizeof( USBCALLLMNGFUNCDESCRIPTOR )

/// define the CDC line coding
typedef struct PACKED _USBCDCLINECODING
{
  U32           uBaudRate;
  U8            nCharFormat;
  U8            nParityType;
  U8            nDataBits;
} USBCDCLINECODING, *PUSBCDCLINECODING;
#define USBCDCLINECODING_SIZE                   sizeof( USBCDCLINECODING )

/**@} EOF USBHandler_def.h */

#endif  // _USBHANDLER_DEF_H