/******************************************************************************
 * @file MLAWINC1500Handler.h
 *
 * @brief WINC1500 handler declarations
 *
 * This file provides the declarations for the WINC1500 handler
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
 
// ensure only one instantiation
#ifndef _MLAWINC1500HANDLER_H
#define _MLAWINC1500HANDLER_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "MLAWINC1500Handler/MLAWINC1500Handler_cfg.h"

// library includes -----------------------------------------------------------
#include "winc1500_api.h"

// Macros and Defines ---------------------------------------------------------
// define the maximum size of the IP addr
#define MAX_IPADDR_LEN                          ( 16 )

// enumerations ---------------------------------------------------------------

/// enumerate the socket types
typedef enum _MLAWINC1500HANDSKTTYPE
{
  MLAWINC1500HAND_SKTTYPE_TCP = SOCK_STREAM,
  MLAWINC1500HAND_SKTTYPE_UDP = SOCK_DGRAM,
} MLAWINC1500HANDSKTTYPE;

/// enumerate the client/server selection
typedef enum _MLAWINC1500HANDCLTSVR
{
  MLAWINC1500HAND_CLTSVR_SERVER = 0,
  MLAWINC1500HAND_CLTSVR_CLIENT,
  MLAWINC1500HAND_CLTSVR_MAX
} MLAWINC1500HANDCLTSVR;

/// enumerate the system callback events
typedef enum _MLAWINC1500HANDSYSCBEVENT
{
  MLAWINC1500HAND_SYSCBEVENT_DISCONNECTED = 0,
  MLAWINC1500HAND_SYSCBEVENT_CONFIGDONE,
  MLAWINC1500HAND_SYSCBEVENT_WAITFORDHCP,
  MLAWINC1500HAND_SYSCBEVENT_CONNECTED,
  MLAWINC1500HAND_SYSCBEVNET_NONE
} MLAWINC1500HANDSYSCBEVENT;

/// enumerate the socket callback events
typedef enum _MLAWINC1500HANDSKTCBEVENT
{
   MLAWINC1500HAND_SKTCBEVENT_BIND = M2M_SOCKET_BIND_EVENT,
   MLAWINC1500HAND_SKTCBEVENT_LISTEN,
   MLAWINC1500HAND_SKTCBEVENT_DNS_RESOLVE,
   MLAWINC1500HAND_SKTCBEVENT_ACCEPT,
   MLAWINC1500HAND_SKTCBEVENT_CONNECT,
   MLAWINC1500HAND_SKTCBEVENT_RECV,
   MLAWINC1500HAND_SKTCBEVENT_SEND,
   MLAWINC1500HAND_SKTCBEVENT_SENDTO,
   MLAWINC1500HAND_SKTCBEVENT_RECVFROM,
   MLAWINC1500HAND_SKTCBEVENT_PINGRESPONSE,
   MLAWINC1500HAND_SKTCBEVENT_SKTCLOSE,
   MLAWINC1500HAND_SKTCBEVNET_XMITDONE,
} MLAWINC1500HANDSKTCBEVENT;

// structures -----------------------------------------------------------------
/// define the receive callback function
typedef void ( *PVSKTRCVCALLBACK )( MLAWINC1500HANDSKTCBEVENT eEvent, C8 cClientSkt, PU8 pnData, U16 wLength, IPADDR tAddr, U16 wPort );

/// define the server accept callback function
typedef void ( *PVSERVACPTCALLBACK )( SOCKET cSock, PVSKTRCVCALLBACK* ppvCallback, PU8* ppnData, PU16 pwLength );

/// define the WIFI event callback function
typedef void ( *PVSYSCALLBACK )( MLAWINC1500HANDSYSCBEVENT eEvent );

/// define the Socket event callback function
typedef void ( *PVSKTCALLBACK )( MLAWINC1500HANDSKTCBEVENT eEvent, SOCKET cSock, t_socketEventData* ptEventData );

/// define the error event callback function
typedef void  ( *PVERRCALLBACK )( U32 uErrorCode );

/// define the configuration structure
typedef struct _MLAWINC1500WIFICONFIG
{
  BOOL    bAutoConnect;
  U8      nConnectionMode;
  U8      nListenChannel;
  U8      nWepKeyIndex;
  U8      nSecurityType;
  U8      nSsideHide;
  C8      acIpAddr[ MAX_IPADDR_LEN ];
  C8      acSSID[ M2M_MAX_SSID_LEN ];
  C8      acPassword[ M2M_MAX_PSK_LEN ];
  C8      acDeviceName[ M2M_DEVICE_NAME_MAX ];
  C8      acWepKey[ WEP_104_KEY_STRING_SIZE + 1 ];
} MLAWINC1500WIFICONFIG, *PMLAWINC1500WIFICONFIG;
#define MLAWINC1500WIFICONFIG_SIZE                     sizeof( MLAWINC1500WIFICONFIG )

// global parameter declarations -----------------------------------------------
#if ( MLAWINC1500HANDLER_ENABLE_DBGCMDS == ON )
  extern  const CODE ASCCMDENTRY g_atMLAWINC1500HandlerCommandsTable[ ];
#endif

// global function prototypes --------------------------------------------------
extern  BOOL            MLAWINC1500Handler_Initialize( void );
extern  void            MLAWINC1500Handler_SysInit( PVSYSCALLBACK pvSysCallback, PVSKTCALLBACK pvSktCallback, PVERRCALLBACK pvErrCallback );
extern  void            MLAWINC1500Handler_Connect( void );
extern  void            MLAWINC1500Handler_Disconnect( void );
extern  PVGETPOINTER    MLAWINC1500Handler_GetActualConfig( void );
extern  void            MLAWINC1500Handler_SetConfiguration( PMLAWINC1500WIFICONFIG ptConfig );
extern  void            MLAWINC1500Handler_IdleTask( void );
extern  U32             MLAWINC1500Handler_GetIpAddress( void );
extern  BOOL            MLAWINC1500Handler_GetConnectionStatus( void );
extern  void            MLAWINC1500Handler_GetMacAddress( PU8 pnMacAddress );
extern  void            MLAWINC1500Handler_IrqCallback( U8 nIrq, U8 nEvent, BOOL bState );
extern  void            MLAWINC1500Handler_WifiCallback( t_m2mWifiEventType eEventCode, t_wifiEventData* ptEventData );
extern  void            MLAWINC1500Handler_SocketCallback( SOCKET cSock, t_m2mSocketEventType eEventCode, t_socketEventData* ptEventData );
extern  void            MLAWINC1500Handler_ErrorCallback( U32 uErrorCode );
extern  SOCKET          MLAWINC1500Handler_OpenSock( BOOL bAutoSocket, IPADDR tAddr, U16 wPort, MLAWINC1500HANDSKTTYPE eUdpTcp, MLAWINC1500HANDCLTSVR eClientSvr, PVSKTRCVCALLBACK pvCallback, PVSERVACPTCALLBACK pvGetRcvBuffer );
extern  S32             MLAWINC1500Handler_Recv( SOCKET cSock, PU8 pnData, U16 wLength );
extern  S32             MLAWINC1500Handler_RecvFrom( SOCKET cSock, PU8 pnData, U16 wLength );
extern  C8              MLAWINC1500Handler_Send( SOCKET cSock, PU8 pnData, U16 wLength );
extern  C8              MLAWINC1500Handler_SendTo( SOCKET cSock, PU8 pnData, U16 wLength, IPADDR tAddr, U16 wPort );
extern  void            MLAWINC1500Handler_CloseSocket( SOCKET cSock );
extern  void            MLAWINC1500Handler_GetHostByName( PC8 pszHostName );

/**@} EOF MLAWINC1500Handler.h */

#endif  // _MLAWINC1500HANDLER_H