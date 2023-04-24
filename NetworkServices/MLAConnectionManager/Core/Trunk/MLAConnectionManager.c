/******************************************************************************
 * @file MLAConnectionManager.c
 *
 * @brief network command handloer implementation
 *
 * This file provides the implementation for the network command handler
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
 * \addtogroup MLAConnectionManager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MLAConnectionManager/MLAConnectionManager.h"\

// library includes -----------------------------------------------------------
#include "MLAWINC1500Handler/MLAWINC1500Handler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  BOOL                    bConnectExecuted = 0;
static  BOOL                    bRecievedFlag;
static  BOOL                    bApModeEnabled;

// local function prototypes --------------------------------------------------
static  void  SystemCallback( MLAWINC1500HANDSYSCBEVENT eEvent );
static  void  SocketCallback( MLAWINC1500HANDSKTCBEVENT eEvent, SOCKET cSock, t_socketEventData* ptEventData );

// constant declarations ------------------------------------------------------

/******************************************************************************
 * @function MLAConnectionManager_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL MLAConnectionManager_Initialize( void )
{
  // clear the flag
  bConnectExecuted = bApModeEnabled = FALSE;

  // call the system initialization
  MLAWINC1500Handler_SysInit( SystemCallback, SocketCallback, NULL );

  // return false
  return( FALSE );
}

/******************************************************************************
 * @function MLAConnectionManager_Control
 *
 * @brief control handler
 *
 * This function will determine if we are enabled/disabled and perform
 * appropriate functionality
 *
 * @param[in]   bState    state of the taskh handler
 *
 *****************************************************************************/
void MLAConnectionManager_Control( BOOL bState )
{
  if (( !bState ) && ( bConnectExecuted ))
  {
    // call the disconnect events
    MLAConnectionManager_CallDisconnectEvents( );

    // disconnect
    MLAWINC1500Handler_Disconnect( );
    
    // clear the flag
    bConnectExecuted = FALSE;
  }
}

/******************************************************************************
 * @function MLAConnectionManager_ProcessEvent
 *
 * @brief process event
 *
 * This function will process an event
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE to flush event
 *
 *****************************************************************************/
BOOL MLAConnectionManager_ProcessEvent( MLACONNECTIONMANAGEREVENT eArg )
{
  // get the event
  switch( eArg )
  {
    case MLACONNECTIONMANAGER_EVENT_DISCONNECT :
      // call the disconnect events
      MLAConnectionManager_CallDisconnectEvents( );
      break;
      
    case MLACONNECTIONMANAGER_EVENT_CONNECT :
      // call the connect events
      MLAConnectionManager_CallConnectEvents( );
      break;

    default :
      break;
    }

  // return TRUE to flush events
  return( TRUE );
}

/******************************************************************************
 * @function SystemCallback 
 *
 * @brief system callback
 *
 * This function will process the system callback
 *
 * @param[in]   eEvent    event
 *
 *****************************************************************************/
static void SystemCallback( MLAWINC1500HANDSYSCBEVENT eEvent )
{
  IPADDR    tAddr;
  SOCKET    tSocket;

  // process the argument
  switch( eEvent )
  {
    case MLAWINC1500HAND_SYSCBEVENT_DISCONNECTED :
      // post the event
      MLAConnectionManager_PostEvent( MLACONNECTIONMANAGER_EVENT_DISCONNECT );
      break;

    case MLAWINC1500HAND_SYSCBEVENT_CONFIGDONE :
      break;

    case MLAWINC1500HAND_SYSCBEVENT_WAITFORDHCP :
      break;

    case MLAWINC1500HAND_SYSCBEVENT_CONNECTED :
      // post the event
      MLAConnectionManager_PostEvent( MLACONNECTIONMANAGER_EVENT_CONNECT );
      break;

    default :
      break;
  }
}

/******************************************************************************
 * @function SocketCallback 
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
static void SocketCallback( MLAWINC1500HANDSKTCBEVENT eEvent, SOCKET cSock, t_socketEventData* ptEventData )
{
  // process the event
  switch( eEvent )
  {
    case MLAWINC1500HAND_SKTCBEVENT_CONNECT :
      break;

    case MLAWINC1500HAND_SKTCBEVENT_RECV :
      break;

    default :
      break;
  }
}
 
/**@} EOF MLAConnectionManager.c */
