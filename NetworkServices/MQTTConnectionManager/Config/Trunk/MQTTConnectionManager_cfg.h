/******************************************************************************
 * @file MQTTConnectionManager_cfg.h
 *
 * @brief MQTT connection manager configuration declarations
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
#ifndef _MQTTCONNECTIONMANAGER_CFG_H
#define _MQTTCONNECTIONMANAGER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "MLAWINC1500Handler/MLAWINC1500Handler.h"

// Macros and Defines ---------------------------------------------------------
/// define the user name/password max length
#define MQTT_USER_NAME_MAXLEN                   ( 32 )
#define MQTT_USER_PSWD_MAXLEN                   ( 32 )

/// define the topic string length
#define MQTT_USER_TOPIC_MAXLEN                  ( 32 )

/// define the publish time for on demand
#define MQTT_PUBLISH_ONDEMAN_TIME               ( 0 )

// enumerations ---------------------------------------------------------------
/// enuemrate the publish topics
typedef enum _MQTTPUBENUM
{
  // do not remove this entry
  MQTT_PUB_ENUM_MAX
} MQTTPUBENUM;

// structures -----------------------------------------------------------------
/// define the callback structure for formatting a topic payload
typedef U16 ( *PVMQTTFORMATPUB )( PU8 pnBuffer, U16 wBufSize );

/// define the connection configuration structure
typedef struct _MQTTCONNMGRCFG
{
  C8  acIpAddr[ M2M_HOSTNAME_MAX_SIZE ];
  C8  acUserName[ MQTT_USER_NAME_MAXLEN ];
  C8  acUserPswd[ MQTT_USER_PSWD_MAXLEN ];
  U16 wPort;
  U16 wKeepAlive;
} MQTTCONNMGRCFG, *PMQTTCONNMGRCFG;
#define MQTTCONNMGRCFG_SIZE                     sizeof( MQTTCONNMGRCFG )

/// define topic publish structure
typedef struct _MQTTCONNMGRPUB
  C8                acTopicString[ MQTT_USER_TOPIC_MAXLEN ];
  U16               wPublishTimeMsecs;
  PVMQTTFORMATPUB   pvFormatPub;
} MQTTCONNMGRPUB, *PMQTTCONNMGRPUB;
#define MQTTCONNMGRPUB                          sizeof( MQTTCONNMGRPUB )

// global parameter declarations -----------------------------------------------
extern  MTTCONNMGRCFG   g_tMqqtConnCfg;
extern  MQTTCONNMGRPUB  g_atMqqtPubCfg[ ];

// global function prototypes --------------------------------------------------
exterm  BOOL  MQTTConnectionManager_LocalInitialize( void );
extern  BOOL  MQTTConnectionManager_ProcessCtlEvent( TASKARG xArg );
extern  BOOL  MQTTConnectionManager_ProcessPubEvent( TASKARG xArg );

/**@} EOF MQTTConnectionManager_cfg.h */

#endif  // _MQTTCONNECTIONMANAGER_CFG_H