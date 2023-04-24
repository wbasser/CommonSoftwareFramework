/******************************************************************************
 * @file DALIBusMaster_cfg.h  
 *
 * @brief DALIBus master configuration declarations
 *
 * This file provides the declarations for the configurable portion of the
 * DALI Bus Master
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
 * \addtogroup DALIBusMaster
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DALIBUSMASTER_CFG_H
#define _DALIBUSMASTER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusMaster/DALIBusMaster_def.h"

// library includes -----------------------------------------------------------
#include "QueueManager/QueueManager.h"
#include "TaskManager/TaskManager.h"
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of transmit queue events
#define DALIBUSMASTER_XMT_QUEUE_SIZE            ( 2 )

/// define the number of events
#define DALIBUSMASTER_XMT_NUM_EVENTS            ( 4 )

/// define the number of events for the bus monitor
#define DALIBUSMASTER_MON_NUM_EVENTS            ( 1 )

/// define the number of events for the bus manager
#define DALIBUSMASTER_CTL_NUM_EVENTS            ( 2 )

/// define the execution rate for the bus monitor
#define DALIBUSMASTER_BUSMON_EXEC_TIME          ( 250 )
#define DALIBUSMASTER_BUSMON_EXEC_RATE          ( TASK_TIME_MSECS( DALIBUSMASTER_BUSMON_EXEC_TIME ))

/// define the macro to enable the debug
#define DALIBUSMASTER_ENABLE_DEBUG              ( ON )

/// define the base values for both bus/transmit debug entries
#define DALIBUSMASTER_CTL_DEBUG_BASE            ( 0x7000 )
#define DALIBUSMASTER_XMT_DEBUG_BASE            ( 0x7100 )

/// define the argument size for bus commissoer
#define DALIBUSMASTER_ARG_SIZE                  ( TASK_TSKARG_SIZE_BYTES )

/// determine the event argument size
#if ( DALIBUSMASTER_ARG_SIZE == 1 )
  #define DALIBUSMASTER_CTLEVENT_MSGINQUEUE     ( 0xE3 )
  #define DALIBUSMASTER_CTLEVENT_DONE           ( 0xE4 )
#elif ( DALIBUSMASTER_ARG_SIZE == 2 )
  #define DALIBUSMASTER_CTLEVENT_MSGINQUEUE     ( 0xE3E3 )
  #define DALIBUSMASTER_CTLEVENT_DONE           ( 0xE4E4 )
#elif ( DALIBUSMASTER_ARG_SIZE == 4 )
  #define DALIBUSMASTER_CTLEVENT_MSGINQUEUE     ( 0xE3E3E3E3 )
  #define DALIBUSMASTER_CTLEVENT_DONE           ( 0xE4E4E4E4 )
#endif

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// defne the local taskarg type
typedef TASKARG     DALIBUSMASTERARG;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  DALIBusMaster_ProcessCtlEvent( TASKARG xArg );
extern  BOOL  DALIBusMaster_ProcessXmtEvent( TASKARG xArg );
extern  BOOL  DALIBusMaster_ProcessMonEvent( TASKARG xArg );
extern  void  DALIBusMaster_Transmit( PU8 pnBuffer, U8 nLength );
extern  void  DALIBusMaster_Receive( PU8 pnBuffer, U8 nLength );
extern  void  DALIBusMaster_StopReceive( void );
extern  BOOL  DALIBusMaster_GetBusState( void );
extern  void  DALIBusMaster_QueuePutRcvMsg( PDALIBUSMASTERMSG ptCurMessage );
extern  void  DALIBusMaster_QueueGetXmtMsg( PDALIBUSMASTERMSG ptCurMessage ); 
extern  void  DALIBusMaster_PostCtlTask( DALIBUSMASTERCTLEVENT eEvent );
extern  void  DALIBusMaster_PostXmtTask( DALIBUSMASTERXMTEVENT eEvent );
extern  void  DALIBusMaster_XmtTaskTimer( BOOL bState, U32 uTimeMsecs );

/**@} EOF DALIBusMaster_cfg.h */

#endif  // _DALIBUSMASTER_CFG_H