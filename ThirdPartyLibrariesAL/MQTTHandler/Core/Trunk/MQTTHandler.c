/******************************************************************************
 * @file MQTTHandler.c
 *
 * @brief MQTT handler imlementation
 *
 * This file provides the implementation for the MQTT handler
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
 * \addtogroup MQTTHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MQTTHandler/MQTTHandler.h"
//#include "mqtt_pal.h"

// library includes -----------------------------------------------------------
#include "DebugManager/DebugManager.h"
#include "RTCManager/RTCManager.h"

// Macros and Defines ---------------------------------------------------------
// define the buffer size
#define LCL_BUF_SIZE                            ( 1024 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// redeifne the MQTT client structure
typedef struct mqtt_client  MQTTCLIENT;

/// redefine the MQTT response publush structure
typedef struct mqtt_response_publish  MQTTRESPPUBLISH;

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  SOCKET              cSocket;
static  U8                  anLclRcvBuffer[ LCL_BUF_SIZE ];
static  U8                  anLclSndBuffer[ LCL_BUF_SIZE ];
static  MQTTCLIENT          tMqttClient;
static  PVSUBSCRIBECALLBACK pvLclCallback;

// local function prototypes --------------------------------------------------
static  void  LclSubscribeCallback( PVOID* ppvState, MQTTRESPPUBLISH *ptPublish );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function MQTTHandler_Initialize
 *
 * @brief initialization
 *
 * This function will perofrm any needed local initialization
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL MQTTHandler_Initialize( void )
{
  // set the callback to null
  pvLclCallback = NULL;

  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function MQTTHandler_ConnectToBroker
 *
 * @brief connect to the broker
 *
 * This function will register the callback and 
 *
 * @param[in]   pvCallback    pointer to the callback
 *
 *****************************************************************************/
BOOL MQTTHandler_ConnectToBroker( IPADDR tAddr, U16 wPort, PC8 pszUserName, PC8 pszPassword )
{
  SOCKET  cSocket;
  enum    MQTTErrors  eError;

  // create a socket
  cSocket = MLAWINC1500Handler_OpenSock( tAddr, wPort, MLAWINC1500HAND_SKTTYPE_TCP, MLAWINC1500HAND_CLTSVR_CLIENT, NULL, NULL );

  // check for a valid socket
  if ( cSocket != -1  )
  {
    // initialze MQTT/open a client
    eError = mqtt_init( &tMqttClient, cSocket, anLclSndBuffer, LCL_BUF_SIZE, anLclRcvBuffer, LCL_BUF_SIZE, NULL );
    DebugManager_AddElement( 0x8901, eError );

    // connect to a client
    eError = mqtt_connect( &tMqttClient, "test", NULL, NULL, 0, pszUserName, pszPassword, 0, 400 );
    DebugManager_AddElement( 0x8902, eError );

    // check for error
    if ( eError == MQTT_OK )
    {
      // enable the refresh thread
      TaskManager_EnableDisable( TASK_SCHD_ENUM_MQTRFESH, ON );
    }
  }
}

 /******************************************************************************
 * @function MQTTHandler_RefreshMqtt
 *
 * @brief process MQTT refresh
 *
 * This function will process an event
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE to flush event
 *
 *****************************************************************************/
BOOL MQTTHandler_RefreshMqtt( TASKARG xArg )
{
  // refresh the MQTT
  mqtt_sync( &tMqttClient );
}

/******************************************************************************
 * @function MQTTHandler_Publish
 *
 * @brief publish
 *
 * This function will pulish the  message to the topic
 *
 * @param[in]   pnTopic       pointer to the topic
 * @param[in]   pnMEssage     pointer to the message
 * @param[in]   wMessageLen   length of the data
 *
 *****************************************************************************/
void MQTTHandler_Publish( PU8 pnTopic, PU8 pnMessage, U16 wMessageLen )
{
  mqtt_publish( &tMqttClient, pnTopic, pnMessage, wMessageLen, MQTT_PUBLISH_QOS_0 );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
S32 mqtt_pal_sendall(int fd, const void* buf, size_t len, int flags) 
{
  S32 lResult;

  // write the block
  MLAWINC1500Handler_Send( fd, ( PU8 )buf, len );

  // set the length
  lResult = len;

  // return the length
  return( lResult );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
S32 mqtt_pal_recvall(int fd, void* buf, size_t bufsz, int flags) 
{
  S32   lSize;
  BOOL  bRunFlag = TRUE;
  
  // get the data
  while( bRunFlag )
  {
    // try to read a block
    lSize = MLAWINC1500Handler_Recv( fd, buf, bufsz );

    // check for valid read
    if ( lSize > 0 )
    {
      // move the buffer pointer/decreement the pointer
      buf += lSize;
      bufsz -= lSize;
    }
    else if ( lSize < 0 )
    {
      // set the error
      lSize = MQTT_ERROR_SOCKET_ERROR;

      // clear the runflag
      bRunFlag = OFF;
    }
    else if ( lSize == 0 )
    {
      // clear the runflag
      bRunFlag = OFF;
    }
  }

  // return the size
  return( lSize );
}


/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
int mqtt_pal_sockopen(const char* addr, U16 wPort, int af) 
{
  IPADDR      tAddr;
  enum        MQTTErrors  eError;

  // zero the address
  tAddr.uAddress = 0;

  // get the port

  // create a socket
  cSocket = MLAWINC1500Handler_OpenSock( tAddr, wPort, MLAWINC1500HAND_SKTTYPE_TCP, MLAWINC1500HAND_CLTSVR_CLIENT, NULL, NULL );

  // check for a valid socket
  if ( cSocket != -1  )
  {
    // initialze MQTT/open a client
//    eError = mqtt_init( &tMqttClient, cSocket, anLclSndBuffer, LCL_BUF_SIZE, anLclRcvBuffer, LCL_BUF_SIZE, LclPublishCallback );
    DebugManager_AddElement( 0x8901, eError );

    // connect to a client
    eError = mqtt_connect( &tMqttClient, "DeviceClient", NULL, NULL, 0, NULL, NULL, 0, 400 );
    DebugManager_AddElement( 0x8902, eError );

    // check for error
    if ( eError == MQTT_OK )
    {
      // subscribe to the 3event
//      mqtt_subscribe( &tMqttClient, TOPIC_SUB, 0 );

      // enable the refresh thread
      TaskManager_EnableDisable( TASK_SCHD_ENUM_MQTRFESH, ON );
    }
  }

  // return csocket
  return( cSocket );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
 mqtt_pal_time_t MQTTHandler_GetTimeInSeconds( void )
{
  DATETIME  tDateTime;
  U64       uTimeSecs;

  // get the current date/time
  RTCManager_GetDateTime( &tDateTime );

  // now convertit to time
  uTimeSecs = TimeHandler_TimeToHuge( TIME_OS_UNIX, &tDateTime );

  // return 0 for now
  return( uTimeSecs );
}

static void  LclSubscribeCallback( PVOID* ppvState, MQTTRESPPUBLISH *ptPublish )
{
}


/**@} EOF MQTTHandler.c */
