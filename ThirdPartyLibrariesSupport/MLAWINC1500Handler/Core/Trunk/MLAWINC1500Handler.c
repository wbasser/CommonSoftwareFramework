/******************************************************************************
 * @file MLAWINC1500Handler.c
 *
 * @brief WINC1500 handler implementation
 *
 * This file provides the implementation for the WINC1500 handler
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
 * \addtogroup MLAWINC1500Handler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MLAWINC1500Handler/MLAWINC1500Handler.h"

// library includes -----------------------------------------------------------
#include "winc1500_driver_config.h"

#if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
  #include "DebugManager/DebugManager.h"
#endif

// Macros and Defines ---------------------------------------------------------
/// define the lengths of the SSID and the passkey
#define WLAN_SSID_MAX_LEN           ( M2M_MAX_SSID_LEN )
#define WLAN_PASS_MAX_LEN           ( M2M_MAX_PSK_LEN )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _SOCKETCTL
{
  BOOL                    bAutoSocket;    ///< auto socket
  MLAWINC1500HANDSKTTYPE  eUdpTcp;        ///< UDP/TCP selector
  MLAWINC1500HANDCLTSVR   eCltSvr;        ///< client server
  PVSKTRCVCALLBACK        pvCallback;     ///< callback function
  PVSERVACPTCALLBACK      pvGetRcvBuffer; ///< get receive buffer
  PU8                     pnRcvBuffer;    ///< pointer to get data
  PU8                     pnXmtBuffer;    ///< pointer to the transmit data
  U16                     wBufSize;       ///< buffer size
  U16                     wXmitRemaining; ///< transmit left
} SOCKETCTL, *PSOCKETCTL;
#define SOCKETCTL_SIZE                      sizeof( SOCKETCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  MLAWINC1500WIFICONFIG tWifiConfigActl;
static  BOOL                  bConfigDone;
static  BOOL                  bConnected;
static  U32                   uCurrIpAddr;
static  SOCKETCTL             atSocketCtl[ MAX_SOCKET ];
static  PVSYSCALLBACK         pvLclSysCallback;
static  PVSKTCALLBACK         pvLclSktCallback;
static  PVERRCALLBACK         pvLclErrCallback;

// local function prototypes --------------------------------------------------
static  void      ConnectToNetwork( void );
#if ( MLAWINC1500HANDLER_ENABLE_DBGCMDS == ON )
  static  ASCCMDSTS CmdQryMacAddr( ASCCMDENUM eEnum );
  static  ASCCMDSTS CmdQryIpAddr( ASCCMDENUM eEnum );
  static  ASCCMDSTS CmdQryChipInfo( ASCCMDENUM eEnum );
#endif

// constant parameter initializations -----------------------------------------
#if ( MLAWINC1500HANDLER_ENABLE_DBGCMDS == ON )
  /// command strings
  static  const C8  szCmdQryMacAddr[ ]  = { "QRYMACADDR" };
  static  const C8  szCmdQryIpAddr[ ]   = { "QRYIPADDR"  };
  static  const C8  szCmdQryChipInfo[ ] = { "QRYCHIPINFO" };

  /// response strings
  static  const C8  szRspWifiMacAdr[ ]  = { "MACADDR: %02X.%02X.%02X.%02X.%02X.%02X\n\r" };
  static  const C8  szRspWifiIpAdr[ ]   = { "IPADDR: %d.%d.%d.%d\n\r" };
  static  const C8  szRspChipInfo1[ ]   = { "CHIPINFO: %08X\n\r" };
  static  const C8  szRspChipInfo2[ ]   = { "\tFirmware Revision: %d.%d.%d\n\r" };
  static  const C8  szRspChipInfo3[ ]   = { "\tDriver Revision: %d.%d.%d\n\r" };
  static  const C8  szRspChipInfo4[ ]   = { "\tDate/Time: %s, %s\n\r" };

  /// command table
  const CODE ASCCMDENTRY g_atMLAWINC1500HandlerCommandsTable[ ] =
  {
    ASCCMD_ENTRY( szCmdQryMacAddr, 10, 0, ASCFLAG_COMPARE_NONE,   0, CmdQryMacAddr  ),
    ASCCMD_ENTRY( szCmdQryIpAddr,   9, 0, ASCFLAG_COMPARE_NONE,   0, CmdQryIpAddr   ),
    ASCCMD_ENTRY( szCmdQryChipInfo, 11, 0, ASCFLAG_COMPARE_NONE,  0, CmdQryChipInfo ),
  };
#endif

/******************************************************************************
 * @function MLAWINC1500Handler_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 * @return  TRUE if errors, FALSE if none
 *
*****************************************************************************/
BOOL MLAWINC1500Handler_Initialize( void )
{
  BOOL  bStatus = FALSE;
  U8    nIdx;

  // call the local initialization
  bStatus = MLAWINC1500Handler_LocalInitialize( );

  // clear the configuration/connection statuses
  bConfigDone = bConnected = FALSE;

  // clear the system callback
  pvLclSysCallback = NULL;
  pvLclSktCallback = NULL;

  // set all socket controls to 0xFF
  for ( nIdx = 0; nIdx < MAX_SOCKET; nIdx++ )
  {
    // set all socket control info
    memset( &atSocketCtl[ nIdx ], 0xFF, SOCKETCTL_SIZE );
  }

  // return no errors 
  return( bStatus );
}

/******************************************************************************
 * @function MLAWINC1500Handler_SysInit
 *
 * @brief system initializaiton 
 *
 * This function will set the callback and initialize the system
 *
 * @param[in]   pvSysCallback    pointer to the system callback
 * @param[in]   pvSktCallback    pointer to the system callback
 *
 *****************************************************************************/
void MLAWINC1500Handler_SysInit( PVSYSCALLBACK pvSysCallback, PVSKTCALLBACK pvSktCallback, PVERRCALLBACK pvErrCallback )
{
  // store the callback
  pvLclSysCallback = pvSysCallback;
  pvLclSktCallback = pvSktCallback;
  pvLclErrCallback = pvErrCallback;

  // call the initialization function
  m2m_wifi_init( );
}

/******************************************************************************
 * @function MLAWINC1500Handler_Connect
 *
 * @brief connect to the network
 *
 * This function will connect to the network
 *
 *
 *****************************************************************************/
void MLAWINC1500Handler_Connect( void )
{
  // connect to the network
  ConnectToNetwork( );
}

/******************************************************************************
 * @function MLAWINC1500Handler_Disconnect
 *
 * @brief disconnect from the network
 *
 * This function will disconnect from the network
 *
 *****************************************************************************/
void MLAWINC1500Handler_Disconnect( void )
{
  // call the disconnect function
  m2m_wifi_disconnect( );
}

/******************************************************************************
 * @function MLAWINC1500Handler_IdleTask
 *
 * @brief process task
 *
 * This function will process the task
 *
 *****************************************************************************/
void MLAWINC1500Handler_IdleTask( void )
{
  // call the task handler
  m2m_wifi_task( );
}

/******************************************************************************
 * @function MLAWINC1500Handler_GetActualConfig
 *
 * @brief get the actual config
 *
 * This function will return the pointer to the actual config
 *
 * @return      pointer to the actual
 *
 *****************************************************************************/
PVGETPOINTER MLAWINC1500Handler_GetActualConfig( void )
{
  // return the default parameters
  return(( PVGETPOINTER )&tWifiConfigActl );
}

/******************************************************************************
 * @function MLAWINC1500Handler_SetCOnfiguration
 *
 * @brief set the configuration
 *
 * This function update the configuration
 *
 * @param[in]  ptConfig   pointer to the new config
 *
 *****************************************************************************/
void MLAWINC1500Handler_SetConfiguration( PMLAWINC1500WIFICONFIG ptConfig )
{
  // copy the config to actual
  memcpy( &tWifiConfigActl, ptConfig, MLAWINC1500WIFICONFIG_SIZE );
  MLAWINC1500Handler_UpdateConfig( );
}

/******************************************************************************
 * @function MLAWINC1500Handler_GetIpAddress
 *
 * @brief get the IP address
 *
 * This function will return the IP address
 *
 * @return      current Ip address
 *
 *****************************************************************************/
U32 MLAWINC1500Handler_GetIpAddress( void )
{
  U32 uAddress = 0;

  // if connected
  if ( bConnected )
  {
    // get the current address
    uAddress = uCurrIpAddr;
  }

  // return the address
  return( uAddress );
}

/******************************************************************************
 * @function MLAWINC1500Handler_GetConnectionStatus
 *
 * @brief get the connection status
 *
 * This function will rteurn the connection status
 *
 * @return  connection status
 *
 *****************************************************************************/
BOOL MLAWINC1500Handler_GetConnectionStatus( void )
{
  // return the connection status
  return( bConnected );
}

/******************************************************************************
 * @function MLAWINC1500Handler_GetMacAddress
 *
 * @brief get the AMAC address
 *
 * This function will rteurn the MAC aaddress
 *
 * @return  mad address
 *
 *****************************************************************************/
void MLAWINC1500Handler_GetMacAddress( PU8 pnMacAddress )
{
  // get the mac address
  m2m_wifi_get_mac_address( pnMacAddress );
}

/******************************************************************************
 * @function MLAWINC1500Handler_IrqCallback
 *
 * @brief IRQ callback
 *
 * This function will handler the IRQ callback
 *
 * @param[in]   nIrq    IRQ number
 * @param[in]   nEvent  passed event
 * @param[in]   bState  state of the IRQ pin
 *
 *****************************************************************************/
void MLAWINC1500Handler_IrqCallback( U8 nIrq, U8 nEvent, BOOL bState )
{
  // call the default handler
  m2m_EintHandler( );
}

/******************************************************************************
 * @function MLAWINC1500Handler_WifiCallback
 *
 * @brief WIFI callback handler
 *
 * This function process the wifi callback event
 *
 * @param[in]   eEventCode    event code
 * @param[in]   ptEventData   event data
 *
 *****************************************************************************/
void MLAWINC1500Handler_WifiCallback( t_m2mWifiEventType eEventCode, t_wifiEventData* ptEventData )
{
  MLAWINC1500HANDSYSCBEVENT eLclEvent = MLAWINC1500HAND_SYSCBEVNET_NONE;

  #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
    DebugManager_AddElement( 0x2000, eEventCode );
  #endif

  // deteremine the event
  switch( eEventCode )
  {
    case M2M_WIFI_DRIVER_INIT_EVENT :
      // set the config done flag
      bConfigDone = TRUE;

      // set the local event
      eLclEvent = MLAWINC1500HAND_SYSCBEVENT_CONFIGDONE;

      // test for auto connect
      if (( TRUE == tWifiConfigActl.bAutoConnect ) && ( MLAWINC1500HANDLER_CONMODE_NRM == tWifiConfigActl.nConnectionMode ))
      {
        // call the connect function
        ConnectToNetwork( );
      }
      break;

    case M2M_WIFI_RSSI_EVENT :
      break;

    case M2M_WIFI_DEFAULT_CONNNECT_EVENT :
      break;

    case M2M_WIFI_CONN_STATE_CHANGED_EVENT :
      // test for connect or disconnect
      if ( ptEventData->connState.u8CurrState == M2M_WIFI_CONNECTED )
      {
        eLclEvent = MLAWINC1500HAND_SYSCBEVENT_WAITFORDHCP;
      }
      else if ( ptEventData->connState.u8CurrState == M2M_WIFI_DISCONNECTED )
      {
        // clear the connected flag
        bConnected = FALSE;
        
        // set the local event
        eLclEvent = MLAWINC1500HAND_SYSCBEVENT_DISCONNECTED;

        // test for auto connect
        if ( TRUE == tWifiConfigActl.bAutoConnect )
        {
          // call the connect function
          ConnectToNetwork( );
        }
      }
      break;

    case M2M_WIFI_WPS_EVENT :
      break;

    case M2M_WIFI_CONN_INFO_RESPONSE_EVENT :
      break;

    case M2M_WIFI_PROVISION_INFO_EVENT :
      break;

    case M2M_WIFI_SCAN_DONE_EVENT :
      break;

    case M2M_WIFI_SCAN_RESULT_EVENT :
      break;

    case M2M_WIFI_SYS_TIME_EVENT :
      MLAWINC1500Handler_ProcessUpdateTime( ptEventData );
      break;

    case M2M_WIFI_PRNG_EVENT :
      break;

    case M2M_WIFI_IP_ADDRESS_ASSIGNED_EVENT :
      // set the connected flag
      bConnected = TRUE;

      // set the local event
      eLclEvent =MLAWINC1500HAND_SYSCBEVENT_CONNECTED;

      // get the IP address
      uCurrIpAddr = ptEventData->ipConfig.u32StaticIp;
      #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
        DebugManager_AddElement( 0x2001, HI32( uCurrIpAddr ));
        DebugManager_AddElement( 0x2002, LO32( uCurrIpAddr ));
      #endif
      break;

    case M2M_WIFI_IP_CONFLICT_EVENT :
      break;

    case M2M_WIFI_INVALID_WIFI_EVENT :
    default :
      break;
  }

  // check for a valid callback and a valid event
  if (( pvLclSysCallback != NULL ) && ( MLAWINC1500HAND_SYSCBEVNET_NONE != eLclEvent ))
  {
    // do it
    pvLclSysCallback( eLclEvent );
  }
}

/******************************************************************************
 * @function MLAWINC1500Handler_SocketCallback
 *
 * @brief socket callback
 *
 * This function will process the socket callback
 *
 * @param[in]   cSock       socket number
 * @param[in]   eEventCode  event code
 * @param[in]   ptEventData event data
 *
 *****************************************************************************/
void MLAWINC1500Handler_SocketCallback( SOCKET cSock, t_m2mSocketEventType eEventCode, t_socketEventData* ptEventData )
{
  PSOCKETCTL          ptCurCtl, ptNewCtl;
  SOCKET              cClientSkt;
  IPADDR              tLclAddr;
  PVSERVACPTCALLBACK  pvLclCallback;
  U16                 wPort;
  #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
    U16                 wArg1, wArg2;
  #endif

  // set the socket control
  ptCurCtl = &atSocketCtl[ cSock ];

  // clear the address
  tLclAddr.uAddress = 0;

  // check for automation
  if ( ON == ptCurCtl->bAutoSocket )
  {
    #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
      DebugManager_AddElement( 0x2100, eEventCode );
      DebugManager_AddElement( 0x2199, cSock);
    #endif

    // process the event
    switch( eEventCode )
    {
      case M2M_SOCKET_BIND_EVENT :
        // check for a successful bind
        if ( ptEventData->bindStatus == 0 )
        {
          // is this a TCP connection
          if ( MLAWINC1500HAND_SKTTYPE_TCP == ptCurCtl->eUdpTcp )
          {
            // listen for a connection request
            listen( cSock, 0 );
            #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
              wArg1 = 0x2101; 
              wArg2 = 0;
            #endif
          }
          else
          {
            // get the receive buffer
            ptCurCtl->pvGetRcvBuffer( cSock, &ptNewCtl->pvCallback, &ptNewCtl->pnRcvBuffer, &ptNewCtl->wBufSize );

            // setup a receive
            recvfrom( cSock, ptCurCtl->pnRcvBuffer, ptCurCtl->wBufSize, 0 );
            #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
              wArg1 = 0x2102; 
              wArg2 = 0;
            #endif
          }
        }
        else
        {
          // close the socket
          close( cSock );

          // call the handler
          if ( ptCurCtl->pvCallback != NULL )
          {
            #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
              wArg1 = 0x2103; 
              wArg2 = 0;
            #endif
            // call it
            ptCurCtl->pvCallback( MLAWINC1500HAND_SKTCBEVENT_SKTCLOSE, -1, NULL, 0,  tLclAddr, 0 );
          }
        }
        break;

      case M2M_SOCKET_LISTEN_EVENT :
        // check for a successful bind
        if ( ptEventData->listenStatus == 0 )
        {
          #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
            wArg1 = 0x2104; 
            wArg2 = 0;
          #endif
          // now listen for connection
          accept( cSock, NULL, NULL );
        }
        else
        {
          // close the socket
          close( cSock );

          // call the handler
          if ( ptCurCtl->pvCallback != NULL )
          {
            // call it
            ptCurCtl->pvCallback( MLAWINC1500HAND_SKTCBEVENT_SKTCLOSE, -1, NULL, 0, tLclAddr, 0 );
          }
          #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
            wArg1 = 0x2105; 
            wArg2 = 0;
          #endif
        }
        break;

      case M2M_SOCKET_DNS_RESOLVE_EVENT :
        // if the socket event callback it not null
        if ( NULL != pvLclSktCallback )
        {
          pvLclSktCallback( eEventCode, cSock, ptEventData );
        }
        break;

      case M2M_SOCKET_ACCEPT_EVENT :
        // get the client clocket
        cClientSkt = ptEventData->acceptResponse.sock;
        ptNewCtl = &atSocketCtl[ cClientSkt ];

        // copy the servers
        memcpy( ptNewCtl, ptCurCtl, SOCKETCTL_SIZE );

        // get the receive buffer
        ptCurCtl->pvGetRcvBuffer( cClientSkt, &ptNewCtl->pvCallback, &ptNewCtl->pnRcvBuffer, &ptNewCtl->wBufSize );

        // queue up a receive
        recv( cClientSkt, ptNewCtl->pnRcvBuffer, ptNewCtl->wBufSize, 0 );
        #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
          wArg1 = 0x2106; 
          wArg2 = cClientSkt;
        #endif
      break;

      case M2M_SOCKET_CONNECT_EVENT :
        // check for a valid socket
        if ( ptCurCtl->eCltSvr == 0xFF )
        {
            #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
              wArg1 = 0x210D; 
              wArg2 = 0xFFFF;
            #endif
        }
        else
        {
          // check for success
          if ( ptEventData->connectResponse.error == 0 )
          {
            // is this a TCP connection
            if ( MLAWINC1500HAND_SKTTYPE_UDP == ptCurCtl->eUdpTcp )
            {
              // get the receive buffer
              ptCurCtl->pvGetRcvBuffer( cClientSkt, &ptNewCtl->pvCallback, &ptNewCtl->pnRcvBuffer, &ptNewCtl->wBufSize );

              // setup a receive
              recvfrom( cSock, ptCurCtl->pnRcvBuffer, ptCurCtl->wBufSize, 0 );
              #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
                wArg1 = 0x2107; 
                wArg2 = 0;
              #endif
            }
          }
          else
          {
            // close the socket
            close( cSock );

            // call the handler
            if ( ptCurCtl->pvCallback != NULL )
            {
              // call it
              ptCurCtl->pvCallback( MLAWINC1500HAND_SKTCBEVENT_SKTCLOSE, -1, NULL, 0, tLclAddr, 0 );
            }
            #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
              wArg1 = 0x2108; 
              wArg2 = 0;
            #endif
          }
        }
        break;

      case M2M_SOCKET_RECV_EVENT :
      case M2M_SOCKET_RECVFROM_EVENT :
        // check for data
        if ( ptEventData->recvMsg.bufSize > 0 )
        {
          // check for a valid callback
          if ( ptCurCtl->pvCallback != NULL )
          {
            // get the address/port
            tLclAddr.uAddress = ptEventData->recvMsg.ai_addr.sin_addr.s_addr;
            wPort = ptEventData->recvMsg.ai_addr.sin_port;

            // call the callback
            ptCurCtl->pvCallback( MLAWINC1500HAND_SKTCBEVENT_RECV, cSock, ptEventData->recvMsg.p_rxBuf, ptEventData->recvMsg.bufSize, tLclAddr, wPort );
          }
          #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
            wArg1 = 0x2109; 
            wArg2 = 0;
          #endif
        }
        else
        {
          // close the socket
          close( cSock );
          #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
            wArg1 = 0x2109; 
            wArg2 = 0xFFFF;
          #endif
        }
        break;

      case M2M_SOCKET_SEND_EVENT  :
        #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
          wArg1 = 0x210A; 
          wArg2 = ptEventData->numSendBytes;
        #endif
        if ( ptCurCtl->wXmitRemaining != 0 )
        {
          // send the next block
          MLAWINC1500Handler_Send( cSock, ptCurCtl->pnXmtBuffer, ptCurCtl->wXmitRemaining );          
        }
        else
        {
          // send an event
          ptCurCtl->pvCallback( MLAWINC1500HAND_SKTCBEVNET_XMITDONE, cSock, NULL, 0, tLclAddr, 0 );
        }
        break;

      case M2M_SOCKET_SENDTO_EVENT :
        #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
          wArg1 = 0x210B; 
          wArg2 = ptEventData->numSendBytes;
        #endif
        break;

      case M2M_SOCKET_PING_RESPONSE_EVENT :
        #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
          wArg1 = 0x210C; 
          wArg2 = 0;
        #endif
        break;

      default :
        #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
          wArg1 = 0x210E; 
          wArg2 = 0xFFFF;
        #endif
        break;
    }
    #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
      DebugManager_AddElement( wArg1, wArg2 );
    #endif
  }
  else
  {
    // get the pointer to the callback
    if ( pvLclSktCallback != NULL )
    {
      // call the callback
      pvLclSktCallback( eEventCode, cSock, ptEventData );
    }
  }
}

/******************************************************************************
 * @function MLAWINC1500Handler_ErrorCallback
 *
 * @brief error callback
 *
 * This function will process the error callback
 *
 * @param[in]   uErrorCode    error code
 *
 *****************************************************************************/
void MLAWINC1500Handler_ErrorCallback( U32 uErrorCode )
{
  // check for valid error callback
  if ( pvLclErrCallback != NULL )
  {
    pvLclErrCallback( uErrorCode );
  }
}

/******************************************************************************
 * @function MLAWINC1500Handler_OpenSock
 *
 * @brief creat/open a socket
 *
 * This function will create and open a socket
 *
 * @param[in]   bAutoSocke      enable auto socket 
 * @param[in]   tAddr           address of the server connection
 * @param[in]   wPort           port number
 * @param[in]   eUdpTcp         type of socket
 * @param[in]   eClientSvr      client/server selector
 * @param[in]   pvCallback      callback function
 * @param[in]   pvGetRcvBuffer  callback to get the receive buffer
 *
 * @return      socket number of -1 if error
 *
 *****************************************************************************/
SOCKET MLAWINC1500Handler_OpenSock( BOOL bAutoSocket, IPADDR tAddr, U16 wPort, MLAWINC1500HANDSKTTYPE eUdpTcp, MLAWINC1500HANDCLTSVR eClientSvr, PVSKTRCVCALLBACK pvCallback, PVSERVACPTCALLBACK pvGetRcvBuffer )
{
  struct sockaddr_in    tAddress;
  SOCKET                cSocket;
  PSOCKETCTL            ptCurCtl;

  // create the socket
  tAddress.sin_family = AF_INET;
  tAddress.sin_port = _htons( wPort );
  tAddress.sin_addr.s_addr = tAddr.uAddress;

  // attempt to open it
  if (( cSocket = socket( AF_INET, eUdpTcp, 0 )) > -1 )
  {
    // get an address to the socket control
    ptCurCtl = &atSocketCtl[ cSocket ];

    // store the info
    ptCurCtl->eCltSvr = eClientSvr;
    ptCurCtl->eUdpTcp = eUdpTcp;
    ptCurCtl->pvCallback = pvCallback;
    ptCurCtl->pvGetRcvBuffer = pvGetRcvBuffer;
    ptCurCtl->bAutoSocket = bAutoSocket;

    #if ( MLAWWINC1500HANDLER_ENABLE_DEBUG == ON )
      DebugManager_AddElement( 0x2200, HI32( tAddr.uAddress ));
      DebugManager_AddElement( 0x2201, LO32( tAddr.uAddress ));
      DebugManager_AddElement( 0x2202, wPort );
      DebugManager_AddElement( 0x2203, MAKEU16( eUdpTcp, eClientSvr ));
    #endif
    // check for server
    if ( MLAWINC1500HAND_CLTSVR_SERVER == eClientSvr )
    {
      // bind to the socket
      bind( cSocket, ( struct sockaddr* )&tAddress, sizeof( struct sockaddr_in ));
    }
    else
    {
      // check for a TCP
      if ( MLAWINC1500HAND_SKTTYPE_TCP == eUdpTcp )
      {
        // connect to the socket
        connect( cSocket, ( struct sockaddr* )&tAddress, sizeof( struct sockaddr_in ));
      }
    }
  }

  // return the error
  return( cSocket );
}

/******************************************************************************
 * @function MLAWINC1500Handler_Recv
 *
 * @brief receve data on the socket
 *
 * This function will receve data
 *
 * @param[in]   cSock       socket number
 * @param[in]   pnRcvBuf    pointer to the receive buffer
 * @param[in]   wBufSize    size of the receive buffer
 *
 * @return    -1 if error, 0 if no bytes, or number of bytes
 *
 *****************************************************************************/
S32 MLAWINC1500Handler_Recv( SOCKET cSock, PU8 pnData, U16 wLength )
{
  S32 lResult;

  // read the data
  lResult = recv( cSock, pnData, wLength, 0 );

  // return the result
  return( lResult );
}

/******************************************************************************
 * @function MLAWINC1500Handler_RecvFrom
 *
 * @brief receve data on the socket
 *
 * This function will receve data
 *
 * @param[in]   cSock       socket number
 * @param[in]   pnRcvBuf    pointer to the receive buffer
 * @param[in]   wBufSize    size of the receive buffer
 *
 * @return    -1 if error, 0 if no bytes, or number of bytes
 *
 *****************************************************************************/
S32 MLAWINC1500Handler_RecvFrom( SOCKET cSock, PU8 pnData, U16 wLength )
{
  S32 lResult;

  // read the data
  lResult = recvfrom( cSock, pnData, wLength, 0 );

  // return the result
  return( lResult );
}

/******************************************************************************
 * @function MLAWINC1500Handler_Send
 *
 * @brief send data on the socket
 *
 * This function will send data
 *
 * @param[in]   cSock       socket number
 * @param[in]   pnRcvBuf    pointer to the receive buffer
 * @param[in]   wBufSize    size of the receive buffer
 *
 * @return    appropriat error code
 *
 *****************************************************************************/
C8 MLAWINC1500Handler_Send( SOCKET cSock, PU8 pnData, U16 wLength )
{
  U16 wTransmitCount;
  C8  cStatus;

  // check for length greater than max length
  if ( wLength > SOCKET_BUFFER_MAX_LENGTH )
  {
    // determine the amount of data to transmit
    wTransmitCount = MIN( wLength, SOCKET_BUFFER_MAX_LENGTH );

    // store the pointer
    atSocketCtl[ cSock ].pnXmtBuffer = pnData + wTransmitCount;
    atSocketCtl[ cSock ].wXmitRemaining = wLength - wTransmitCount;
  }
  else
  {
    // clear the remaining transmit count
    atSocketCtl[ cSock ].wXmitRemaining = 0;
    wTransmitCount = wLength;
  }

  // send the data
  cStatus = send( cSock, pnData, wTransmitCount, 0 );

  // return the status 
  return( cStatus );
}

/******************************************************************************
 * @function MLAWINC1500Handler_SendTo
 *
 * @brief send data to a specific device
 *
 * This function will send data to a specific device
 *
 * @param[in]   cSock       socket number
 * @param[in]   pnRcvBuf    pointer to the receive buffer
 * @param[in]   wBufSize    size of the receive buffer
 * @param[in]   tAddr       address of the server connection
 * @param[in]   wPort       port number
 *
 * @return    appropriat error code
 *
 *****************************************************************************/
C8 MLAWINC1500Handler_SendTo( SOCKET cSock, PU8 pnData, U16 wLength, IPADDR tAddr, U16 wPort )
{
  struct sockaddr_in  tAddress;
  C8                  cStatus;
  
  // create the socket
  tAddress.sin_family = AF_INET;
  tAddress.sin_port = wPort;
  tAddress.sin_addr.s_addr = tAddr.uAddress;

  // send it
  cStatus = sendto( cSock, pnData, wLength, 0, ( struct sockaddr* )&tAddress, sizeof( struct sockaddr_in ));

  // return the status 
  return( cStatus );
}

/******************************************************************************
 * @function MLAWINC1500Handler_Close
 *
 * @brief close
 *
 * This function will close a socket
 *
 * @param[in]   cSock       socket number
 *
 *****************************************************************************/
void MLAWINC1500Handler_CloseSocket( SOCKET cSock )
{
  // close it
  close( cSock );
}

void MLAWINC1500Handler_GetHostByName( PC8 pszHostName )
{
  // get a host IPADDR by name
  gethostbyname( pszHostName );
}


/******************************************************************************
 * @function ConnectToNetwork
 *
 * @brief connect to the network
 *
 * This function will attempt to connect to the network
 *
 *****************************************************************************/
static void ConnectToNetwork( void )
{
  tstrM2MAPConfig tApConfig;
  IPADDR          tIpAddr;

  switch ( tWifiConfigActl.nConnectionMode )
  {
    case MLAWINC1500HANDLER_CONMODE_NRM :
      // connect to network  
      m2m_wifi_connect( tWifiConfigActl.acSSID, strlen(tWifiConfigActl.acSSID ), tWifiConfigActl.nSecurityType, ( PVOID )tWifiConfigActl.acPassword, M2M_WIFI_CH_ALL );
      break;

    case MLAWINC1500HANDLER_CONMODE_APP :
      strcpy( tApConfig.au8SSID, tWifiConfigActl.acSSID );
      tApConfig.u8ListenChannel = tWifiConfigActl.nListenChannel;
      tApConfig.u8KeyIndx = tWifiConfigActl.nWepKeyIndex;
      tApConfig.u8KeySz = strlen( tWifiConfigActl.acPassword );
      strcpy( tApConfig.au8WepKey, tWifiConfigActl.acWepKey );
      tApConfig.u8SecType = tWifiConfigActl.nSecurityType;
      tApConfig.u8SsidHide = tWifiConfigActl.nSsideHide;
      inet_pton4( tWifiConfigActl.acIpAddr, &tIpAddr.uAddress );
      tApConfig.au8DHCPServerIP[ 0 ] = tIpAddr.anOctets[ LE_U32_LSB_IDX ];
      tApConfig.au8DHCPServerIP[ 1 ] = tIpAddr.anOctets[ LE_U32_MS1_IDX ];
      tApConfig.au8DHCPServerIP[ 2 ] = tIpAddr.anOctets[ LE_U32_MS2_IDX ];
      tApConfig.au8DHCPServerIP[ 3 ] = tIpAddr.anOctets[ LE_U32_MSB_IDX ];
      strcpy( tApConfig.au8Key, tWifiConfigActl.acPassword );

      // start the AP service
      m2m_wifi_enable_ap( &tApConfig );
      break;

    case MLAWINC1500HANDLER_CONMODE_P2P :
      // set the device name and turn on pwp
      m2m_wifi_set_device_name( tWifiConfigActl.acDeviceName, strlen( tWifiConfigActl.acDeviceName ));
      m2m_wifi_p2p( tWifiConfigActl.nListenChannel );
      break;

    default :
      break;
  }
}

#if ( MLAWINC1500HANDLER_ENABLE_DBGCMDS == ON )
  /******************************************************************************
   * @function CmdQryMacAddr
   *
   * @brief query the MAC address
   *
   * This function will query the MAC address
   *
   * @param[in]   eCmdEnum command enum
   *
   * @return  appropriate status
   * 
   *****************************************************************************/
  static ASCCMDSTS CmdQryMacAddr( ASCCMDENUM eEnum )
  {
    PC8   pcBuffer;
    U8    anMacAddress[ M2M_MAC_ADDRESS_LEN ];

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( eEnum, &pcBuffer );

    // query the mac address
    MLAWINC1500Handler_GetMacAddress( anMacAddress );

    // now output it
    sprintf( pcBuffer, ( PCC8 )szRspWifiMacAdr, 
             anMacAddress[ 0 ], anMacAddress[ 1 ], anMacAddress[ 2 ], anMacAddress[ 3 ], anMacAddress[ 4 ], anMacAddress[ 5 ] );

    // return output buffer
    return( ASCCMD_STS_OUTPUTBUFFER );
  }

  /******************************************************************************
   * @function CmdQryIpAddr
   *
   * @brief query the IP address
   *
   * This function will query the IP address
   *
   * @param[in]   eCmdEnum command enum
   *
   * @return  appropriate status
   * 
   *****************************************************************************/
  static ASCCMDSTS CmdQryIpAddr( ASCCMDENUM eEnum )
  {
    U32UN   tIpAddr;
    PC8     pcBuffer;

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( eEnum, &pcBuffer );

    // query the mac address
    tIpAddr.uValue = MLAWINC1500Handler_GetIpAddress( );

    // now output it
    sprintf( pcBuffer, ( PCC8 )szRspWifiIpAdr, 
             tIpAddr.anValue[ LE_U32_LSB_IDX ], tIpAddr.anValue[ LE_U32_MS1_IDX ], tIpAddr.anValue[ LE_U32_MS2_IDX ], tIpAddr.anValue[ LE_U32_MSB_IDX ] );

    // return output buffer
    return( ASCCMD_STS_OUTPUTBUFFER );
  }

  /******************************************************************************
   * @function CmdQryChipInfo
   *
   * @brief query the chip info
   *
   * This function will query the chip info
   *
   * @param[in]   eCmdEnum command enum
   *
   * @return  appropriate status
   * 
   *****************************************************************************/
  static  ASCCMDSTS CmdQryChipInfo( ASCCMDENUM eEnum )
  {
    tstrM2mRev  tChipInfo;
    PC8         pcBuffer;

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( eEnum, &pcBuffer );

    // get the chip info
    nm_get_firmware_info( &tChipInfo );

    // now output it
    sprintf( pcBuffer, ( PCC8 )szRspChipInfo1, tChipInfo.u32Chipid );
    AsciiCommandHandler_OutputBuffer( eEnum );
    sprintf( pcBuffer, ( PCC8 )szRspChipInfo2, tChipInfo.u8FirmwareMajor, tChipInfo.u8FirmwareMinor, tChipInfo.u8FirmwarePatch );
    AsciiCommandHandler_OutputBuffer( eEnum );
    sprintf( pcBuffer, ( PCC8 )szRspChipInfo3, tChipInfo.u8DriverMajor, tChipInfo.u8DriverMinor, tChipInfo.u8DriverPatch );
    AsciiCommandHandler_OutputBuffer( eEnum );
    sprintf( pcBuffer, ( PCC8 )szRspChipInfo4, tChipInfo.BuildDate, tChipInfo.BuildTime );

    // return output buffer
    return( ASCCMD_STS_OUTPUTBUFFER );
  }
#endif

/**@} EOF MLAWINC1500Handler.c */
