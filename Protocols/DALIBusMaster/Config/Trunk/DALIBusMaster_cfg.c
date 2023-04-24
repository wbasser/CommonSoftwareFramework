/******************************************************************************
 * @file DALIBusMaster_cfg.c
 *
 * @brief DALI Bus Master configuration implementaion 
 *
 * This file provides the implementation for the configurable section of the 
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusMaster/DALIBusMaster.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "ManchesterCodec/ManchesterCodec.h"

// Macros and Defines ---------------------------------------------------------
/// define the put event for the incoming queue
#define DALIBUSMASTER_XMTMSG_QUEUEEVENT         ( QUEUEPUT_EVENT( QUEUE_ENUM_DALIMASTERXMT ))

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function DALIBusMaster_ProcessCtlEvent
 *
 * @brief process the event
 *
 * This function process the bus event 
 *
 * @param[in]   xArg    event argument
 *
 * @return      TRUE to flush the event from the event queue
 *
 *****************************************************************************/
BOOL DALIBusMaster_ProcessCtlEvent( TASKARG xArg )
{
  // call the handler
  DALIBusMaster_CtlEventHandler( xArg );

  // return true to flush events
  return( TRUE );
}

/******************************************************************************
 * @function DALIBusMaster_ProcessXmtEvent
 *
 * @brief process the event
 *
 * This function process the event
 *
 * @param[in]   xArg    event argument
 *
 * @return      TRUE to flush the event from the event queue
 *
 *****************************************************************************/
BOOL DALIBusMaster_ProcessXmtEvent( TASKARG xArg )
{
  // call the handler
  DALIBusMaster_XmtEventHandler( xArg );

  // return true to flush events
  return( TRUE );
}

/******************************************************************************
 * @function DALIBusMaster_ProcessMonEvent
 *
 * @brief process the event
 *
 * This function process the event
 *
 * @param[in]   xArg    event argument
 *
 * @return      TRUE to flush the event from the event queue
 *
 *****************************************************************************/
BOOL DALIBusMaster_ProcessMonEvent( TASKARG xArg )
{
  // just call the handler
  DALIBusMaster_MonEventHandler( );

  // return true to flush events
  return( TRUE );
}

/******************************************************************************
 * @function DALIBusMaster_Transmit
 *
 * @brief transmit a message out on the bus
 *
 * This function will all the underlying transmit method 
 *
 * @param[in]   pnBuffer    pointer to the buffer
 * @param[in]   nLength     length of the buffer
 *
 *****************************************************************************/
void DALIBusMaster_Transmit( PU8 pnBuffer, U8 nLength )
{
  // call the manchester transmit 
  ManchesterCodec_Xmit( pnBuffer, nLength );
}

/******************************************************************************
 * @function DALIBusMaster_Receive
 *
 * @brief receive a message from the bus
 *
 * This function will call the underlying receive method
 *
 * @param[in]   pnBuffer    pointer to the buffer
 * @param[in]   nLength     length of the buffer
 *
 *****************************************************************************/
void DALIBusMaster_Receive( PU8 pnBuffer, U8 nLength )
{
  // call the manchester receive
  ManchesterCodec_Recv( pnBuffer, nLength );
}
 
/******************************************************************************
 * @function DALIBusMaster_StopReceive
 *
 * @brief stop reception
 *
 * This function will call the underlying stop receive method
 *
 *****************************************************************************/
void DALIBusMaster_StopReceive( void )
{
  // stop the manchester receive
  ManchesterCodec_StopRecv( );
}

/******************************************************************************
 * @function DALIBusMaster_StartXmtTaskTimer
 *
 * @brief start a transmit task timer
 *
 * This function starts a timer for the transmit task
 *
 * @param[in]   uTimeMsecs        time in milliseconds
 *
 *****************************************************************************/
void DALIBusMaster_StartXmtTaskTimer( U32 uTimeMsecs )
{
  // start the timer
//  TaskManager_StartTimer( TASK_SCHD_ENUM_DMSXMIT, TASK_TIME_MSECS( uTimeMsecs ));
}

/******************************************************************************
 * @function DALIBusMaster_GetBusState
 *
 * @brief returns the state of the DALI RX pin
 *
 * This function returns the state of the Bus monitor
 *
 * @return  current state of the bus pin
 *
 *****************************************************************************/
BOOL DALIBusMaster_GetBusState( void )
{
  BOOL bBusState = OFF;

  // get the bus state pin
  Gpio_GetIrq( GPIO_IRQ_ENUM_DALIRX, &bBusState );

  // return the state
  return( bBusState );
}

/******************************************************************************
 * @function DALIBusMaster_QueuePutRcvMsg
 *
 * @brief post the latest message to the queue
 *
 * This function posts the message to a queue
 *
 * @param[in]   ptCurMessage    pointer to the message
 *
 *****************************************************************************/
void DALIBusMaster_QueuePutRcvMsg( PDALIBUSMASTERMSG ptCurMessage )
{
  PVDALIBUSMSTRCB pvCallback;

  // check to see if this is a callback
  if (( pvCallback = ptCurMessage->pvCallbackFunc ) != NULL )
  {
    // execute the callback
    pvCallback(( PVOID)ptCurMessage );
  }
  else
  {
    // must be a queue entry, post the event
    QueueManager_PutTail( ptCurMessage->nOption, ( PU8 )ptCurMessage );
  }
}

/******************************************************************************
 * @function DALIBusMaster_QueueGetXmtMsg
 *
 * @brief get the lastest transmit message from the queue
 *
 * This function gets the latest message from the transmit queue
 *
 * @param[in]   ptCurMessage    pointero the message structure
 *
 *****************************************************************************/
void DALIBusMaster_QueueGetXmtMsg( PDALIBUSMASTERMSG ptCurMessage )
{
  // get the message from the queue
  QueueManager_Get( QUEUE_ENUM_DALIMASTERXMT, ( PU8 )ptCurMessage );
}

/******************************************************************************
 * @function DALIBusMaster_PostBusTask
 *
 * @brief post bus task
 *
 * This function will post an event to the bus task
 *
 * @param[in]   eEvent    event to post
 *
 *****************************************************************************/
void DALIBusMaster_PostCtlTask( DALIBUSMASTERCTLEVENT eEvent )
{
  // post the event
  TaskManager_PostEvent( TASK_SCHD_ENUM_DMSCNTL, eEvent );
}

/******************************************************************************
 * @function DALIBusMaster_PostXmtTask
 *
 * @brief post xmit task
 *
 * This function will post an event to the transmit task
 *
 * @param[in]   eEvent    event to post
 *
 *****************************************************************************/
void DALIBusMaster_PostXmtTask( DALIBUSMASTERXMTEVENT eEvent )
{
  // post the event
  TaskManager_PostEvent( TASK_SCHD_ENUM_DMSXMIT, eEvent );
}

/******************************************************************************
 * @function DALIBusMaster_StartXmtTaskTimer
 *
 * @brief start a transmit task timer
 *
 * This function starts a timer for the transmit task
 *
 * @param[in]   bState            state of the timer
 * @param[in]   uTimeMsecs        time in milliseconds
 *
 *****************************************************************************/
void DALIBusMaster_XmtTaskTimer( BOOL bState, U32 uTimeMsecs )
{
  // check for state
  if ( bState )
  {
    // start the timer
    TaskManager_StartTimer( TASK_SCHD_ENUM_DMSXMIT, TASK_TIME_MSECS( uTimeMsecs ));
  }
  else
  {
    // stop the timer
    TaskManager_StopTimer( TASK_SCHD_ENUM_DMSXMIT );
  }
}
 
/**@} EOF DALIBusMaster_cfg.c */
