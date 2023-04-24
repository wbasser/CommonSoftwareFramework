/******************************************************************************
 * @file MQTTConnectionManager
 *
 * @brief MQTT connection manager implementation
 *
 * This file provides the implementation for the MQTT connection manager
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
 * \addtogroup MQTTConnectionManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MQTTConnectionManager/MQTTConnectionManager.h"

// library includes -----------------------------------------------------------
#include "MQTTPacket/MQTTPacket.h"
#include "MQTTPacket/transport.h"
#include "RTCManager/RTCManager.h"
#include "SensorManager/SensorManager.h"
#include "SerialNumber/SerialNumber.h"

/////////////////////////////////////////////////
// FOR DEBUG ONLY
#include "DebugManager/DebugManager.h"
/////////////////////////////////////////////////

// Macros and Defines ---------------------------------------------------------
/// define the DNS resolve time
#define DNS_RESOLVE_MAX_TIME_SECS               ( 30 )
#define DNS_RESOLVE_MAX_TIME_COUNTS             ( TASK_TIME_SECS( DNS_RESOLVE_MAX_TIME_SECS ))

/// define the HTTP Buffer size
#define MQTT_BUFFER_SIZE                        ( 256 )

// enumerations ---------------------------------------------------------------
/// enumerate the MQTT states
typedef enum  _MQTTSTATE
{
  MQTT_STATE_NONNCONNECTED = 0,
  MQTT_STATE_CONNECTED,
  MQTT_STATE_MAX
} MQTTSTATE;

/// enumerate the recieve states
typedef enum _RCVSTATE
{
  RCV_STATE_IDLE = 0,
  RCV_STATE_WAITFORDONE,
  RCV_STATE_RECEIVED,
  RCV_STATE_MAX
} RCVSTATE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  RCVSTATE                eRcvState;
static  IPADDR                  tServerAddr;
static  SOCKET                  tSocket;
static  MQTTCONNMGRCFG          tConfigActl;
static  U8                      anBuffer[ MQTT_BUFFER_SIZE ];
static  U8                      anRcvBuffer[ MQTT_BUFFER_SIZE ];
static  U16                     wRcvBufferCount;
static  U16                     wRcvBufferRdIdx;
static  MQTTTransport           tMqttTransport;
static  transport_iofunctions_t tTransportIof;
static  int                     iTransportSocket;
static  C8                      szClientId[ SERNUM_GUID_LENGTH  ];
static  U8                      anTopicPayload[ 256 ];
static  BOOL                    bConnected;

// local function prototypes --------------------------------------------------
static  void  GetParameters( SOCKET cSock, PVSKTRCVCALLBACK* ppvCallback, PU8* ppnData, PU16 pwLength );
static  void  ProcessReceive( t_socketRecv* ptRecvMsg );
static  S32   LclRecvData( PU8 pnData, U32 uMaxBYtes );
static  S32   LclXmitData( PU8 pnData, U32 uBytes );

// constant parameter initializations -----------------------------------------
// instantiate the defaults
static  const MQTTCONNMGRCFG  tConfigDflt =
{
  "killerbunnies.strongarmtech.io",
  "bbasser",
  "Triangle12",
  "CYBEnviromental",
  1883,
  60,
};

/// define the publish string
static  const C8  szTopicPayload[ ] = 
{
  "\"TIME\": \"%4d%02d%02d-%02d%02d%02d\",\"BATVOLT\":%1.2f,\"TEMP\":%2.1f,\"HUMD\":%2.1f,\"PRESS\":%4.1f"
};

/******************************************************************************
 * @function MQTTConnectionManager_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the MQTT connection manager
 *
 * @return TRUE if errors detected, FALSE if not
 *
 *****************************************************************************/
BOOL MQTTConnectionManager_Initialize( void )
{
  // clear the current socket
  tSocket = -1;

  // initialize the read/write for the socket
  tTransportIof.recv = LclRecvData;
  tTransportIof.send = LclXmitData;

  // open the transport socket
  iTransportSocket = transport_open( &tTransportIof );

  // fill in the MQTT tansport
  tMqttTransport.sck = &iTransportSocket;
  tMqttTransport.getfn = transport_getdatanb;
  tMqttTransport.state = 0;
  
  // clear the receive state
  eRcvState = RCV_STATE_IDLE;

  // clear the connected state
  bConnected = FALSE;

  // return the status
  return( FALSE );
}

/******************************************************************************
 * @function MQTTConnectionManager_GetConfigDflt
 *
 * @brief get the default pointer
 *
 * This function will return the pointer to the default configuration
 *
 * @return pointer to the default
 *
 *****************************************************************************/
PVGETPOINTER MQTTConnectionManager_GetConfigDflt( void )
{
  // return the pointer
  return(( PVGETPOINTER )&tConfigDflt );
}

/******************************************************************************
 * @function MQTTConnectionManager_GetConfigActl
 *
 * @brief get the actual pointer
 *
 * This function will return the pointer to the actual configuration
 *
 * @return pointer to the actual
 *
 *****************************************************************************/
PVGETPOINTER MQTTConnectionManager_GetConfigActl( void )
{
  // return the pointer
  return(( PVGETPOINTER )&tConfigActl );
}

/******************************************************************************
 * @function MQTTConnectionManager_SetServerIpAddr
 *
 * @brief set the server IP Address
 *
 * This function will set the server IP address
 *
 * @param[in]   tAddr   IP address
 *
 *****************************************************************************/
void MQTTConnectionManager_SetServerIpAddr( IPADDR tAddr )
{
  // set the address
  tServerAddr = tAddr;

  // post the DNS found event
  TaskManager_PostEvent( TASK_SCHD_ENUM_MQTTCONN, MQTTCONMGR_EVENT_DNSRESOLVED );
}

///******************************************************************************
// * @function MQTTConnectionManager_GetSocketNumber
// *
// * @brief get the current socket asssociated with MQQT
// *
// * This function will return the socket number
// *
// * @return socket number
// *
// *****************************************************************************/
//SOCKET MQTTConnectionManager_GetSocketNumber( void )
//{
//  // return the socket
//  return( tSocket );
//}

/******************************************************************************
 * @function MQTTConnectionManager_ProcessSktCallback 
 *
 * @brief socket callback
 *
 * This function processes the socket call back
 *
 * @param[in]   eEvent              event received
 * @param[in]   cSock               socket number
 * @param[in]   tSocketEventData*   pointer to the socket event data
 *
 *****************************************************************************/
void MQTTConnectionManager_ProcessSktCallback( MLAWINC1500HANDSKTCBEVENT eEvent, SOCKET cSock, t_socketEventData* ptEventData )
{
/////////////////////////////////////////////////
// FOR DEBUG ONLY
DebugManager_AddElement( 0x4000, eEvent );
/////////////////////////////////////////////////
  // process the event
  switch( eEvent )
  {
    case M2M_SOCKET_CONNECT_EVENT :
      // post the connected event
      TaskManager_PostEvent( TASK_SCHD_ENUM_MQTTCONN, MQTTCONMGR_EVENT_CONNECTED );
      break;

    case M2M_SOCKET_SEND_EVENT :
      // post a process received to force a read
      TaskManager_PostEvent( TASK_SCHD_ENUM_MQTTCONN, MQTTCONMGR_EVENT_SENDDONE );
      break;

    case M2M_SOCKET_RECV_EVENT :
      // process the receive
      ProcessReceive( &ptEventData->recvMsg );
      break;

    case M2M_SOCKET_DNS_RESOLVE_EVENT :
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function MQTTConnectionManager_ProcessEvent
 *
 * @brief event handler
 *
 * This function will post any event posted to the connection manager
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE
 *
 *****************************************************************************/
BOOL MQTTConnectionManager_ProcessCtlEvent( TASKARG xArg )
{
  BOOL                    bFlushEvents = TRUE;
  MQTTPacket_connectData  tConnectData = MQTTPacket_connectData_initializer;
  U16                     wLength;
  int                     iResult;
  U8                      nSessionPresent, nConnectResultCode;

/////////////////////////////////////////////////
// FOR DEBUG ONLY
DebugManager_AddElement( 0x4100, xArg );
/////////////////////////////////////////////////
  // determine event
  switch( xArg )
  {
    case TASK_TIMEOUT_EVENT :
      // timeout ocurred/force a restart
      TaskManager_PostEvent( TASK_SCHD_ENUM_MQTTCONN, MQTTCONMGR_EVENT_BEGIN );
      break;

    case MQTTCONMGR_EVENT_BEGIN :
      // resolve the server name
      MLAWINC1500Handler_GetHostByName( tConfigActl.acIpAddr );

      // start a timer
      TaskManager_StartTimer( TASK_SCHD_ENUM_MQTTCONN, DNS_RESOLVE_MAX_TIME_COUNTS );
      break;

    case MQTTCONMGR_EVENT_DNSRESOLVED :
      // stop the timer
      TaskManager_StopTimer( TASK_SCHD_ENUM_MQTTCONN );

      // open a socket
      tSocket = MLAWINC1500Handler_OpenSock( FALSE, tServerAddr, tConfigActl.wPort, MLAWINC1500HAND_SKTTYPE_TCP, MLAWINC1500HAND_CLTSVR_CLIENT, NULL, NULL );
      break;

    case MQTTCONMGR_EVENT_CONNECTED :
      // get the guid from serial number as the client id
      SerialNumber_FormatGuid( szClientId, SERNUM_GUID_LENGTH );

      // send the conneciton structure
      tConnectData.MQTTVersion = 3,
      tConnectData.clientID.cstring = szClientId;
      tConnectData.keepAliveInterval = tConfigActl.wKeepAlive * 2;
      tConnectData.username.cstring = tConfigActl.acUserName;
      tConnectData.password.cstring = tConfigActl.acUserPswd;

      // serialize it
      wLength = MQTTSerialize_connect( anBuffer, MQTT_BUFFER_SIZE, &tConnectData );

      // send it
      transport_sendPacketBuffer( iTransportSocket, anBuffer, wLength );
      break;

    case MQTTCONMGR_EVENT_SENDDONE :
      if ( !bConnected )
      {
        // call receive and check for a CONNACK
        if (( iResult = MQTTPacket_readnb( anBuffer, MQTT_BUFFER_SIZE, &tMqttTransport )) == CONNACK )
        {
          /// check for accepted connection
          iResult = MQTTDeserialize_connack( &nSessionPresent, &nConnectResultCode, anBuffer, MQTT_BUFFER_SIZE );

          // check for good
          if (( iResult != 1 ) || ( nConnectResultCode != 0 ))
          {
            // post a restart command
            TaskManager_PostEvent( TASK_SCHD_ENUM_MQTTCONN, MQTTCONMGR_EVENT_BEGIN );
          }
          else
          {
            // set the connection state
            bConnected = TRUE;

            // enable the publish
            TaskManager_EnableDisable( TASK_SCHD_ENUM_MQTTPUBL, ON );
          }
        }
        else if ( iResult == -1 )
        {
          // post a restart command
          TaskManager_PostEvent( TASK_SCHD_ENUM_MQTTCONN, MQTTCONMGR_EVENT_BEGIN );
        }
        else
        {
          // do not flush event
          bFlushEvents = FALSE;
        }
      }
      break;

    case MQTTCONMGR_EVENT_CONNLOST :
      // disable the publish
      TaskManager_EnableDisable( TASK_SCHD_ENUM_MQTTPUBL, ON );

      // post a restart command
      TaskManager_PostEvent( TASK_SCHD_ENUM_MQTTCONN, MQTTCONMGR_EVENT_BEGIN );
      break;

    default :
      break;
  }

  // flush event
  return ( bFlushEvents );
}

/******************************************************************************
 * @function MQTTConnectionManager_ProcessPubEvent
 *
 * @brief publish event handler
 *
 * This function will post any event posted to the connection manager
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE
 *
 *****************************************************************************/
BOOL MQTTConnectionManager_ProcessPubEvent( TASKARG xArg )
{
  MQTTString  tTopic = MQTTString_initializer;
  FLOAT       fBatVolt, fEvnTemp, fEvnHumd, fEvnPres;
  DATETIME    tDateTime;
  int         iLength, iResult;

/////////////////////////////////////////////////
// FOR DEBUG ONLY
DebugManager_AddElement( 0x4200, xArg );
/////////////////////////////////////////////////
  // determine event
  switch( xArg )
  {
    case TASK_TIMEOUT_EVENT :
      // timeout ocurred
      tTopic.cstring = tConfigActl.acTopicString;

      // get the values
      SensorManager_GetValue( SENMAN_ENUM_BATVOLT, &fBatVolt );
      //SensorManager_GetValue( SENMAN_ENUM_ENVTEMP, &fEvnTemp );
      //SensorManager_GetValue( SENMAN_ENUM_ENVHUMD, &fEvnHumd );
      //SensorManager_GetValue( SENMAN_ENUM_ENVPRES, &fEvnPres );

      // get the date/time
      RTCManager_GetDateTime( &tDateTime );

      // fill the publish string
      iLength = sprintf( anTopicPayload, ( PCC8 )szTopicPayload,
                         tDateTime.wYear, tDateTime.nMonth, tDateTime.nDay,
                         tDateTime.nHours, tDateTime.nMinutes, tDateTime.nSeconds,
                         fBatVolt, fEvnTemp, fEvnHumd, fEvnPres );

      // serialize it
      iLength = MQTTSerialize_publish( anBuffer, MQTT_BUFFER_SIZE, 0, 0, 0, 0, tTopic, anTopicPayload, iLength );

      // send it
      if (( iResult != transport_sendPacketBuffer( iTransportSocket, anBuffer, iLength )) == iLength )
      {
        // clear the connected state
        bConnected = FALSE;

        // error on publish, turn off task
        TaskManager_EnableDisable( TASK_SCHD_ENUM_MQTTPUBL, OFF );

        // post an event to start
        TaskManager_PostEvent( TASK_SCHD_ENUM_MQTTCONN, MQTTCONMGR_EVENT_BEGIN );
      }

      break;

    default :
      break;
  }

  // flush event
  return ( TRUE );
}

/******************************************************************************
 * @function ProcessReceive
 *
 * @brief process the receive data
 *
 * This function will process the received data
 *
 * @param[in]   ptRecvMsg     pointer to the receive message data
 *
 *****************************************************************************/
static void ProcessReceive( t_socketRecv* ptRecvMsg )
{
/////////////////////////////////////////////////
// FOR DEBUG ONLY
/////////////////////////////////////////////////
DebugManager_AddElement( 0x4600, ptRecvMsg->bufSize );
  // set the receive count
  wRcvBufferCount = ptRecvMsg->bufSize;

  // change state
  eRcvState = RCV_STATE_RECEIVED;
}

/******************************************************************************
 * @function LclRecvData
 *
 * @brief receive data
 *
 * This function will read data from the network
 *
 * @param[io]   pnData      pointer to the data
 * @param[in]   uMaxBytes   maximum number of bytes that the buffer can handle
 *
 * @return      -1 for error, number of actual bytes received
 *
 *****************************************************************************/
static S32 LclRecvData( PU8 pnData, U32 uMaxBytes )
{
  S32 lStatus = -1;

/////////////////////////////////////////////////
// FOR DEBUG ONLY
/////////////////////////////////////////////////
DebugManager_AddElement( 0x4400 | eRcvState, uMaxBytes );

  // process the state
  switch( eRcvState )
  {
    case RCV_STATE_IDLE :
      // clear the counts and indices
      wRcvBufferCount = wRcvBufferRdIdx = 0;

      // request a receive
      if ( MLAWINC1500Handler_Recv( tSocket, anRcvBuffer, MQTT_BUFFER_SIZE ) == SOCK_ERR_NO_ERROR )
      {
        // wait for done
        eRcvState = RCV_STATE_WAITFORDONE;

        // set the status to 0
        lStatus = 0;
      }
      break;

    case RCV_STATE_WAITFORDONE :
      lStatus = 0;
      break;

    case RCV_STATE_RECEIVED :
      // check for a timeout
      if ( wRcvBufferCount > 0 )
      {
        // set the number of bytes received
        lStatus = MIN( uMaxBytes, wRcvBufferCount );
      
        // copy the data
        memcpy( pnData, &anRcvBuffer[ wRcvBufferRdIdx ], lStatus );
        wRcvBufferRdIdx += lStatus;
        wRcvBufferCount -= lStatus;
      
        // test for empy
        if ( wRcvBufferCount == 0 )
        {
          // go back to idle
          eRcvState = RCV_STATE_IDLE;
        }
      }
      else
      {
        // check for closed
        if ( wRcvBufferCount <= 0 )
        {
          // reset the state
          lStatus = -1;
          eRcvState = RCV_STATE_IDLE;
        }
      }
      break;

    default :
      // force back to idle
      eRcvState - RCV_STATE_IDLE;
      break;
  }

/////////////////////////////////////////////////
// FOR DEBUG ONLY
/////////////////////////////////////////////////
DebugManager_AddElement( 0x4500, lStatus );

  // return the statu
  return( lStatus );
}

/******************************************************************************
 * @function LclXmitData
 *
 * @brief transmit data
 *
 * This function will send data from the network
 *
 * @param[io]   pnData      pointer to the data
 * @param[in]   uBytes      number of bytes to send
 *
 * @return      -1 for error, number of bytes sent
 *
 *****************************************************************************/
static S32 LclXmitData( PU8 pnData, U32 uBytes )
{
  S32 lStatus = -1;

  // send the data
  if ( MLAWINC1500Handler_Send( tSocket, pnData, uBytes ) == SOCK_ERR_NO_ERROR )
  {
    // set the sent status to the bytes sent
    lStatus = uBytes;
  }

  // return the number of bytes or error
  return( lStatus );
}

/**@} EOF MQTTConnectionManager.c */
