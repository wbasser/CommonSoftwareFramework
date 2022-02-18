/****************************************7**************************************
 * @file  DALIMessageHandler_cfg.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 *
 *
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIMESSAGEHANDLER_CFG_H
#define _DALIMESSAGEHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_def.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of event
#define	DALIMESSAGE_EVENT_NUM_EVENTS            ( 4 )
#define	DALIMESSAGE_TIMER_NUM_EVENTS            ( 1 )
#define DALIMESSAGE_OUTPUT_NUM_EVENTS           ( 1 )
#define DALIMESSAGE_BUSFAIL_NUM_EVENTS          ( 1 )

/// define the execution times
#define DALIMESSAGE_TIMER_EXEC_MINS             ( 15 )
#define DALIMESSAGE_OUTPUT_EXEC_MSECS           ( 1 )
#define DALIMESSAGE_BUSFAIL_EXEC_MSECS          ( 5 )

/// define the timer execution rate
#define	DALIMESSAGE_TIMER_EXEC_RAT1E            ( TASK_TIME_MINS( DALIMESSAGE_TIMER_EXEC_MINS ))
#define DALIMESSAGE_OUTPUT_EXEC_RATE            ( TASK_TIME_MSECS( DALIMESSAGE_OUTPUT_EXEC_MSECS ))
#define DALIMESSAGE_BUSFAIL_EXEC_RATE           ( TASK_TIME_MSECS( DALIMESSAGE_BUSFAIL_EXEC_MSECS ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  DALIMessageHandler_ProcessEvent( TASKARG xArg );
extern  BOOL  DALIMessageHandler_ProcessTimer( TASKARG xArg );
extern	BOOL  DALIMessageHandler_ProcessOutput( TASKARG xArg );
extern  BOOL  DALIMessageHandler_ProcessBusFail( TASKARG xArg );
extern  void  DALIMessageHandler_OutputXmtMsg( U8 nResponse );
extern  void  DALIMessageHandler_OutputRcvMsg( PDALIMSG ptMsg );
extern  void  DALIMessageHandler_StartReceive( PU8 pnData, U8 nLength );
extern  void  DALIMessageHandler_StopReceive( void );
extern  void  DALIMessageHandler_StartTransmit( PU8 pnData, U8 nLength );
extern  void  DALIMessageHandler_SetLightLevel( U16 wLevel );
extern  BOOL  DALIMessageHandler_GetLightStatus( void );
extern  void  DALIMessageHandler_SeedRandom( void);
extern  U32   DALIMessageHandler_GenerateRandom( void );
extern  BOOL  DALIMessageHandler_WriteMemoryBlock( U8 nMemoryBlock, U8 nBlockAddress, U8 nValue, BOOL bBypassLockByte );
extern  BOOL  DALIMessageHandler_ReadMemoryBlock( U8 nMemoryBlock, U8 nBlockAddress, PU8 pnValue );
extern  BOOL  DALIMessageHandler_WriteMBParameter( U8 nParam, U8 nValue, BOOL bBypassLockByte );
extern  BOOL  DALIMessageHandler_GetDefaultValue( U8 nParam, PU8 pnValue );
extern  BOOL  DALIMessageHandler_PutValue( U8 nParam, U8 nValue );
extern  BOOL  DALIMessageHandler_GetValue( U8 nParam, PU8 pnValue );
extern  void  DALIMessageHandler_ResetUserDefaults( void );
extern  BOOL  DALIMessageHandler_GetBusStatus( void );
extern  void  DALIMessageHandler_StartEventTimer( U32 uTimeUsec );
extern  void  DALIMessageHandler_StopEventTimer( void );
extern  void  DALIMessageHandler_StartInitTimer( void );
extern  void  DALIMessageHandler_StopInitTimer( void );

/**@} EOF DALIMessageHandler_cfg.h */
#endif  // _DALIMESSAGEHANDLER_CFG_H
