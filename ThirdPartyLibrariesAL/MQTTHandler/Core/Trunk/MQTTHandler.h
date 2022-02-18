/******************************************************************************
 * @file MQTTHandler.h
 *
 * @brief MQTT handler declarations
 *
 * This file provides the declarations for the MQTT handler
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
 
// ensure only one instantiation
#ifndef _MQTTHANDLER_H
#define _MQTTHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"
#include "MLAWINC1500Handler/MLAWINC1500Handler.h"
#include "mqtt.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of events/execution rate for the MQTT refresh handler
#define MQTTHANDLER_NUM_EVENTS   ( 2 )
#define MQTTHANDLER_EXEC_MSECS   ( 10 )
#define MQTTHANDLER_EXEC_RATE    ( TASK_TIME_MSECS( MQTTHANDLER_EXEC_MSECS ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the publish callback
typedef void ( *PVSUBSCRIBECALLBACK )( PU8 pnTopic, U16 wTopicLen, PU8 pnMessage, U16 wMessageLen );

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  MQTTHandler_Initialize( void );
extern  BOOL  MQTTHandler_ConnectToBroker( IPADDR tAddr, U16 wPort, PC8 pszUserName, PC8 pszPassword );
extern  void  MQTTHandler_DiscconectFromBroker( void );
extern  BOOL  MQTTHandler_RefreshMqtt( TASKARG xArg );
extern  U64   MQTTHandler_GetTimeInSeconds( void );
extern  void  MQTTHandler_Subscribe( PU8 pnTopic );
extern  void  MQTTHandler_Publish( PU8 pnTopic, PU8 pnMessage, U16 wMessageLen ); 

/**@} EOF MQTTHandler.h */

#endif  // _MQTTHANDLER_H