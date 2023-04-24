/******************************************************************************
 * @file USBGenHandler.c
 *
 * @brief USB Handler implementation 
 *
 * This file provides the implementation for the USB handler
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
#include <wchar.h>

// local includes -------------------------------------------------------------
#include "UsbGenHandler/UsbGenHandler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the endpoint call back
typedef void ( *PVENDPNTDALLBACK )( U16 wCount );

// the define the endpoint callbacks
typedef struct _ENDPNTCALLBACK
{
  PVENDPNTDALLBACK pvOut;
  PVENDPNTDALLBACK pvIn;
} ENDPNTCALLBACK, *PENDPNTCALLBACK;
#define ENDPNTCALLBACK_SIZE                     sizeof( ENDPNTCALLBACK )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  ENDPNTCALLBACK  atEndpoints[ USB_NUM_ENDPOINTS ];
static  U8              nConfig;
static  ALIGNED4 U8     anStringBuffer[ USB_MAX_PACKET_SIZE ];
static  VBOOL           bWriteInProgress;

// local function prototypes --------------------------------------------------
static  void  ProcessDescriptorRequest( PUSBREQUEST ptRequest );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function UsbGenHandler_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed local initialization and call the USB 
 * device initialization
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL UsbGenHandler_Initialize ( void )
{
  BOOL bStatus = FALSE;

  // clear the config
  nConfig = 0;

  // clear the write in progress
  bWriteInProgress = FALSE;

  // call the local initialization
  bStatus = UsbGenHandler_LocalInitialize( );
  
  // call the USB initilization
  bStatus = Usb_Initialize( );

  // check for auto attach
  if ( ON == USBGENHANDLER_ENABLE_AUTOATTACH )
  {
    Usb_AttachDetach( OFF );
  }

  // return status
  return( bStatus );
}

/******************************************************************************
 * @function UsbGenHandler_IsConfigured
 *
 * @brief checks for configuration
 *
 * This function will return the status of the configuration
 *
 * @return  TRUE if configured, FALSE otherwise
 *
 *****************************************************************************/
BOOL UsbGenHandler_IsConfigured( void )
{
  // return the state of the configuration
  return( nConfig != 0 );
}

/******************************************************************************
 * @function UsbGenHandler_Send
 *
 * @brief send data
 *
 * This function will send data on the given endpoint
 *
 * @param[in]   nEndpoint   endpoint selection
 * @param[in]   pnData      pointer to the data
 * @param[in]   wLength     length of data to receive
 * @param[in]   pvCallback  pointer to the callback function
 *
 *****************************************************************************/
void UsbGenHandler_Send( U8 nEndpoint, PU8 pnData, U16 wLength, PVSNDRCVCALLBACK pvCallback )
{
  // register the callback/call the send data event
  atEndpoints[ nEndpoint ].pvIn = pvCallback;
  bWriteInProgress = TRUE;
  Usb_Send( nEndpoint, pnData, wLength );

  // wait for done
  while( bWriteInProgress );
}

/******************************************************************************
 * @function UsbGenHandler_Recv
 *
 * @brief receive data
 *
 * This function will receive data on the given endpoint
 *
 * @param[in]   nEndpoint   endpoint selection
 * @param[in]   pnData      pointer to the data
 * @param[in]   wLength     length of data to receive
 * @param[in]   pvCallback  pointer to the callback function
 *
 *****************************************************************************/
void UsbGenHandler_Recv( U8 nEndpoint, PU8 pnData, U16 wLength, PVSNDRCVCALLBACK pvCallback )
{
  // register the callback/call the send data event
  atEndpoints[ nEndpoint ].pvOut = pvCallback;
  Usb_Recv( nEndpoint, pnData, wLength );
}

/******************************************************************************
 * @function UsbGenHandler_RecvCallback
 *
 * @brief receive callback
 *
 * This function will test for a valid callback and execute it
 *
 * @param[in]   nEp     endpoint selection
 *
 *****************************************************************************/
void UsbGenHandler_RecvCallback( U8 nEndpoint, U16 wCount )
{
  PVSNDRCVCALLBACK  pvCallback;
  
  // if callback is not null
  if (( pvCallback = atEndpoints[ nEndpoint ].pvOut ) != NULL )
  {
    // execute it
    pvCallback( wCount );
  }
}

/******************************************************************************
 * @function UsbGenHandler_SendCallback
 *
 * @brief send callback
 *
 * This function will test for a valid callback and execute it
 *
 * @param[in]   nEp     endpoint selection
 *
 *****************************************************************************/
void UsbGenHandler_SendCallback( U8 nEndpoint, U16 wCount )
{
  PVSNDRCVCALLBACK  pvCallback;

  // clear the write in progress
  bWriteInProgress = FALSE;

  // if callback is not null
  if (( pvCallback = atEndpoints[ nEndpoint ].pvIn ) != NULL )
  {
    // execute it
    pvCallback( wCount );
  }
}

/******************************************************************************
 * @function UsbGenHandler_HandleStandardRequest
 *
 * @brief process standard requests
 *
 * This function will parse the request and call the appropriate functions
 *
 * @param[in]   ptRequest   pointer to the standard request
 *
 *****************************************************************************/
void UsbGenHandler_HandleStandardRequest( PUSBREQUEST ptRequest )
{
  U16                     wTemp;
  U8                      nEp, nDir;
  PUSBDESCHEADR           ptDescHeader;
  PUSBENDPOINTDESCRIPTOR  ptDescEndpoint;
  
  // determine the request
  switch(( ptRequest->nRequest << 8 ) | ptRequest->nRequestType )
  {
    case USB_CMD( GETDESCRIPTOR, IN, STANDARD, DEVICE ) :
      // process descriptor request
      ProcessDescriptorRequest( ptRequest );
      break;

    case USB_CMD( SETADDRESS, OUT, STANDARD, DEVICE ) :
      // send a ZLP/set the address
      Usb_ControlSendZlp( );
      Usb_SetAddress( ptRequest->wValue );
      break;

    case USB_CMD( SETCONFIGURATION, OUT, STANDARD, DEVICE ) :
      // set the config/send a ZLP
      nConfig = ( U8 )ptRequest->wValue;
      Usb_ControlSendZlp( );
      
      // if config is not zero
      if ( nConfig != 0 )
      {
        // brute force
        Usb_ConfigureEndpoint( USB_CDCENDPOINT_RCV, USB_ENDPNTTYPE_BULK, 64 );
        Usb_ConfigureEndpoint( USB_CDCENDPOINT_XMT | USB_ENDPOINT_IN, USB_ENDPNTTYPE_BULK, 64 );
        Usb_ConfigureEndpoint( USB_CDCENDPOINT_CTL | USB_ENDPOINT_IN, USB_ENDPNTTYPE_OTHER, 64 );
//        // get the size of the config
//        wTemp = g_tUsbConfig.tCfgDescriptor.wTotalLength;
//
//        // get the descriptor header
//        ptDescHeader = ( PUSBDESCHEADR )&g_tUsbConfig;
//        
//        // check for done
//        while( wTemp != 0 )
//        {
//          // is this and endpoint descriptor
//          if ( ptDescHeader->nDescriptorType == USB_DESCTYPES_ENDPOINT )
//          {
//            // cast the pointer
//            ptDescEndpoint = ( PUSBENDPOINTDESCRIPTOR )ptDescHeader;
//            
//            // configure the endpoint
//            Usb_ConfigureEndpoint( ptDescEndpoint->nEndpointAddress, ptDescEndpoint->nAttributes, ptDescEndpoint->wMaxPacketSize );
//          }
//          
//          // adjust the size/increment pointer
//          wTemp -= ptDescHeader->nLength;
//          ptDescHeader = ( PUSBDESCHEADR )(( PU8 )ptDescHeader + ptDescHeader->nLength );
//        }
        
        // call the local callback
        UsbGenHandler_ProcessControlCallback( nConfig );
      }
      break;

    case USB_CMD( GETCONFIGURATION, IN, STANDARD, DEVICE ) :
      // send the config
      Usb_ControlSend( &nConfig, sizeof( U8 ));
      break;

    case USB_CMD( GETSTATUS, IN, STANDARD, DEVICE ) :
    case USB_CMD( GETSTATUS, IN, STANDARD, INTERFACE ) :
      // return 0
      wTemp = 0;
      Usb_ControlSend(( PU8 )&wTemp, sizeof( U16 ));
      break;

    case USB_CMD( GETSTATUS, IN, STANDARD, ENDPOINT ) :
      // get the endpoint and direction/clear the status
      nEp = ptRequest->wIndex & USB_ENDPOINT_INDEX_MASK;
      nDir = ptRequest->wIndex & USB_ENDPOINT_DIRECTION_MASK;
      wTemp = 0;

      // is endpoint configured
      if ( Usb_IsEndpointConfigured( nEp, nDir ))
      {
        // get the current status/send it
        wTemp = Usb_EndpointGetStatus( nEp, nDir );
        Usb_ControlSend(( PU8 )&wTemp, sizeof( U16 ));
      }
      else
      {
        // send a stall
        Usb_ControlStall( );
      }
      break;

    case USB_CMD( SETFEATURE, OUT, STANDARD, DEVICE ) :
      // stall
      Usb_ControlStall( );
      break;

    case USB_CMD( SETFEATURE, OUT, STANDARD, INTERFACE ) :
      // send a ZLP
      Usb_ControlSendZlp( );
      break;

    case USB_CMD( SETFEATURE, OUT, STANDARD, ENDPOINT ) :
      // get the endpoint and direction/clear the status
      nEp = ptRequest->wIndex & USB_ENDPOINT_INDEX_MASK;
      nDir = ptRequest->wIndex & USB_ENDPOINT_DIRECTION_MASK;

      // check to see if the value is not zero/valid endpoint address/endpoint configured
      if (( ptRequest->wValue != 0 ) && ( nEp > 0 ) && ( Usb_IsEndpointConfigured( nEp, nDir ) == TRUE ))
      {
        // set tthe feature/send a ZLP
        Usb_EndpointSetFeature( nEp, nDir );
        Usb_ControlSendZlp( );
      }
      else
      {
        // send a stall
        Usb_ControlStall( );
      }
      break;

    case USB_CMD( CLEARFEATURE, OUT, STANDARD, DEVICE ) :
      // stall
      Usb_ControlStall( );
      break;

    case USB_CMD( CLEARFEATURE, OUT, STANDARD, INTERFACE ) :
      // send a ZLP
      Usb_ControlSendZlp( );
      break;

    case USB_CMD( CLEARFEATURE, OUT, STANDARD, ENDPOINT ) :
      // get the endpoint and direction/clear the status
      nEp = ptRequest->wIndex & USB_ENDPOINT_INDEX_MASK;
      nDir = ptRequest->wIndex & USB_ENDPOINT_DIRECTION_MASK;

      // check to see if the value is not zero/valid endpoint address/endpoint configured
      if (( ptRequest->wValue == 0 ) && ( nEp > 0 ) && ( Usb_IsEndpointConfigured( nEp, nDir ) == TRUE ))
      {
        // set tthe feature/send a ZLP
        Usb_EndpointClearFeature( nEp, nDir );
        Usb_ControlSendZlp( );
      }
      else
      {
        // send a stall
        Usb_ControlStall( );
      }
      break;

    default:
      // call the request handler and check if done
      if ( UsbGenHandler_HandleOtherRequests( ptRequest ) != TRUE )
      {
        // stall
        Usb_ControlStall( );
      }
      break;
  }
}

/******************************************************************************
 * @function ProcessDescriptorRequest
 *
 * @brief process standard requests
 *
 * This function will parse the request and call the appropriate functions
 *
 * @param[in]   ptRequest   pointer to the standard request
 *
 *****************************************************************************/
static void ProcessDescriptorRequest( PUSBREQUEST ptRequest )
{
  U8  nType, nIndex, nStrSize;
  U16 wMaxLength, wActLength;
  wchar_t* pwTemp;
  PU8  pnOutputDesc;

  // get the type/index/max length
  nType = ptRequest->wValue >> 8;
  nIndex = ptRequest->wValue & 0xff;
  wMaxLength = ptRequest->wLength;
  
  // process the type
  switch( nType )
  {
    case USB_DESCTYPES_DEVICE :
      // set the length/send it
      wActLength = MIN( wMaxLength, g_tUsbDevDescriptor.tHeader.nLength );
      Usb_ControlSend(( PU8 )&g_tUsbDevDescriptor, wActLength );
      break;
      
    case USB_DESCTYPES_CONFIGURATION :
      // set the transmit legnth/send it
      wActLength = MIN( wMaxLength, g_tUsbConfig.tCfgDescriptor.wTotalLength );
      Usb_ControlSend(( PU8 )&g_tUsbConfig, wActLength );
      break;
      
    case USB_DESCTYPES_STRING :
      // check for index 0
      if ( nIndex == 0 )
      {
        wActLength = MIN( wMaxLength, g_tUsbStringZeroDescriptor.tHeader.nLength );
        Usb_ControlSend(( PU8 )&g_tUsbStringZeroDescriptor, wActLength );
      }
      else if ( nIndex < USB_STRINDEX_MAX )
      {
        // allocate space for the string
        nStrSize = USBDESCHEADR_SIZE + ( wcslen( g_awStrings[ nIndex ] ) * 2 );
        pnOutputDesc = malloc( nStrSize );
        *( pnOutputDesc + 0 ) = nStrSize;
        *( pnOutputDesc+ 1 ) = USB_DESCTYPES_STRING;

        // now copy the string
        pwTemp = ( wchar_t* )g_awStrings[ nIndex ];
        wcscpy(( wchar_t* )( pnOutputDesc + 2 ), pwTemp );

        // send it
        wActLength = MIN( wMaxLength, nStrSize ); 
        Usb_ControlSend( pnOutputDesc, wActLength );

        // de-allocate the buffer
        free( pnOutputDesc );
      }
      else
      {
        // stall
        Usb_ControlStall( );
      }
      break;
      
    default :
      // stall
      Usb_ControlStall( );
      break;
  }
}

/**@} EOF UsbGenHandler.c */
