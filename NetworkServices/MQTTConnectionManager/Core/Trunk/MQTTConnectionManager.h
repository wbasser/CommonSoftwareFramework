/******************************************************************************
 * @file MQTTConnectionManager
 *
 * @brief MQTT connection manager
 *
 * This file provides the declarations for the MQTT connection manager
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
 
// ensure only one instantiation
#ifndef _MQTTCONNECTIONMANAGER_H
#define _MQTTCONNECTIONMANAGER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "MLAWINC1500Handler/MLAWINC1500Handler.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of events
#define MQTTCONNECTMGR_CTLNUM_EVENTS            ( 4 )

/// define the execution rate
#define MQTTCONNECTMNGR_PUBEXEC_RATE_MINS       ( 1 )
#define MQTTCONNECTMNGR_PUBEXEC_RATE_COUNTS     ( TASK_TIME_MINS( MQTTCONNECTMNGR_PUBEXEC_RATE_MINS ))

/// define the number of events
#define MQTTCONNECTMGR_PUBNUM_EVENTS            ( 2 )

/// define the user name/password max length
#define USER_NAME_MAXLEN                        ( 32 )
#define USER_PSWD_MAXLEN                        ( 32 )

/// define the topic string length
#define USER_TOPIC_MAXLEN                       ( 32 )

// enumerations ---------------------------------------------------------------
/// enumerate the events
typedef enum _MQTTCONMGREVENT
{
  MQTTCONMGR_EVENT_BEGIN = 0x3240,
  MQTTCONMGR_EVENT_DNSRESOLVED,
  MQTTCONMGR_EVENT_CONNLOST,
  MQTTCONMGR_EVENT_CONNECTED,
  MQTTCONMGR_EVENT_WAITPAKCET,
  MQTTCONMGR_EVENT_SENDDONE,
  MQTTCONMGR_EVENT_MAX,
} MQTTCONMGREVENT;

// structures -----------------------------------------------------------------
/// define the configuration structure
typedef struct _MQTTCONNMGRCFG
{
  C8  acIpAddr[ M2M_HOSTNAME_MAX_SIZE ];
  C8  acUserName[ USER_NAME_MAXLEN ];
  C8  acUserPswd[ USER_PSWD_MAXLEN ];
  C8  acTopicString[ USER_TOPIC_MAXLEN ];
  U16 wPort;
  U16 wKeepAlive;
} MQTTCONNMGRCFG, *PMQTTCONNMGRCFG;
#define MQTTCONNMGRCFG_SIZE                     sizeof( MQTTCONNMGRCFG )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL          MQTTConnectionManager_Initialize( void );
extern  PVGETPOINTER  MQTTConnectionManager_GetConfigDflt( void );
extern  PVGETPOINTER  MQTTConnectionManager_GetConfigActl( void );
extern  void          MQTTConnectionManager_SetServerIpAddr( IPADDR tAddr );
//extern  SOCKET        MQTTConnectionManager_GetSocketNumber( void );
extern  void          MQTTConnectionManager_ProcessSktCallback( MLAWINC1500HANDSKTCBEVENT eEvent, SOCKET cSock, t_socketEventData* ptEventData );
extern  BOOL          MQTTConnectionManager_LocalProcessCtlEvent( TASKARG xArg );
extern  void          MQTTConnectionManager_LocalProcessPubEvent( TASKARG xArg );

/**@} EOF MQTTConnectionManager.h */

#endif  // _MQTTCONNECTIONMANAGER_H