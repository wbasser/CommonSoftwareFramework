/******************************************************************************
 * @file ManchesterCodec_cfg.c
 *
 * @brief Manchester Codec configuration implementation
 *
 * This file provides the implementation for the configuration of the
 * Manchester Codec
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
 * $Rev: $
 * 
 *
 * \addtogroup ManchesterCodec
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "ManchesterCodec/ManchesterCodec.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "GPIO/Gpio.h"
#if ( MANCHESTERCODEC_ENABLE_INPUT_QUEUE == ON )
  #include "QueueManager/QueueManager.h"
#endif // MANCHESTERCODEC_ENABLE_INPUT_QUEUE

// Macros and Defines ---------------------------------------------------------
/// define the index for the edge and value
#define VALUE_INDEX                             ( LE_U32_LSW_IDX )
#define EDGE_INDEX                              ( LE_U32_MSW_IDX )

#if ( MANCHESTERCODEC_ENABLE_INPUT_QUEUE == ON )
  # define the task argument for a queue event
  #define LCL_QUEUEPUT_EVENT                      ( QUEUEPUT_EVENT( QUEUE_ENUM_MANCHESTERRCV ))
#endif // MANCHESTERCODEC_ENABLE_INPUT_QUEUE

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function ManchesterCodec_LocalInitialize
 *
 * @brief initialize the manchester codec
 *
 * This function will perform all initialization for the manchester encoder/
 * decoder
 *
 * @param[in]   uHalfBitTime      half time bit
 *
 * @return  TRUE if errors detected, FALSE if OK
 *
 *****************************************************************************/
BOOL  ManchesterCodec_LocalInitialize( U32 uHalfBitTime )
{
  // return no errors
  return( FALSE );
}

/******************************************************************************
 * @function  ManchesterCodec_GetClockFreq
 *
 * @brief  get the current clock frequency used by the manchester codec
 *
 * This function will return the base clock frequency for the manchester
 * codec
 *
 *****************************************************************************/
U32 ManchesterCodec_GetClockFreq( void )
{
  U32 uClockFreq;

  // get the clock freqency
  Timers_Ioctl( MANCHESTERCODEC_RXTIMER_SELECT, TIMER_IOCTL_GETCLKFRQ, ( PVOID )&uClockFreq );

  // return the clock frequency
  return( uClockFreq );
}

/******************************************************************************
 * @function  ManchesterCodec_TransmitCallback
 *
 * @brief  transmit timer callback function
 *
 * This function will process the transmit timer callback 
 *
 * @param[in]   eEvent    event that is generated
 * @param[in]   eChan     compare/capture channel
 * @param[in]   wValue    optional value
 *
 *****************************************************************************/
void ManchesterCodec_TransmitCallback( TIMERCBEVENT eEvent, TIMERCMPCAP eChan, U16 wValue )
{
  // process the bit timer
  ManchesterCodec_ProcessXmtTimer( );
}

/******************************************************************************
 * @function  ManchesterCodec_ReceiveCallback
 *
 * @brief  receive capture callback function
 *
 * This function will process the receive capture callback 
 *
 * @param[in]   eEvent    event that is generated
 * @param[in]   eChan     compare/capture channel
 * @param[in]   wValue    optional value
 *
 *****************************************************************************/
void ManchesterCodec_ReceiveCallback( TIMERCBEVENT eEvent, TIMERCMPCAP eChan, U16 wValue )
{
  U32UN tValue;

  // build the value
  tValue.awValue[ VALUE_INDEX ] = wValue;
  tValue.awValue[ EDGE_INDEX ] = ( TIMER_CBEVENT_CAPRE == eEvent ) ? MANCHESTERCODEC_EDGE_RISE : MANCHESTERCODEC_EDGE_FALL;

  #if ( MANCHESTERCODEC_ENABLE_INPUT_QUEUE == ON )
    // post to queue
    QueueManager_PutTail( QUEUE_ENUM_MANCHESTERRCV, ( PU8 )&tValue );
  #else
    // post it
    TaskManager_PostEventIrq( TASK_SCHD_ENUM_MANRECV, ( TASKARG )tValue.uValue );
  #endif // MANCHESTERCODEC_ENABLE_INPUT_QUEUE
}

/******************************************************************************
 * @function  ManchesterCodec_RecvTimerControl
 *
 * @brief  set timer for receive
 *
 * This function will configure and start the timer for receive
 *
 * @param[in]   bState    desired state of the transmit timer
 *
 *****************************************************************************/
void ManchesterCodec_RecvTimerControl( BOOL bState )
{
  // turn off/on the receive timer
  Timers_Ioctl( TIMER_ENUM_MANCHESTERRCV,  TIMER_IOCTL_STOPSTART,  ( PVOID )&bState );
}

/******************************************************************************
 * @function  ManchesterCodec_FlushRecvEents
 *
 * @brief  flush the receive events if any
 *
 * This function will flush the receive events
 *
 *****************************************************************************/
void ManchesterCodec_FlushRecvEvents( void )
{
  TaskManager_FlushEvents( TASK_SCHD_ENUM_MANRECV );
}

/******************************************************************************
 * @function  ManchesterCodec_OutputControl
 *
 * @brief  control the output bit
 *
 * This function will set/clear the output bit
 *
 * @param[in]   bCurBit     current bit state
 *
 *****************************************************************************/
void ManchesterCodec_OutputControl( BOOL bCurBit )
{
  // set the appropriate state
  Gpio_Set( GPIO_PIN_ENUM_DALITX, bCurBit );
}

/******************************************************************************
 * @function  ManchesterCodec_PostXmitEvent
 *
 * @brief  post an event to transmit task
 *
 * This function will post an event to the calling task
 *
 * @param[in]   uEvent      event to send
 *
 *****************************************************************************/
void ManchesterCodec_PostXmitEvent( U32 uEvent )
{
  // post the event 
  //TaskManager_PostEvent( TASK_SCHD_ENUM_DMSXMIT, uEvent );
}

/******************************************************************************
 * @function  ManchesterCodec_PosRecvtEvent
 *
 * @brief  post an event to receive task
 *
 * This function will post an event to the calling task
 *
 * @param[in]   uEvent      event to send
 *
 *****************************************************************************/
void ManchesterCodec_PostRecvEvent( U32 uEvent )
{
  // post the event 
 // TaskManager_PostEvent( TASK_SCHD_ENUM_DMSXMIT, uEvent );
}

/******************************************************************************
 * @function  ManchesterCodec_ProcessCaptureValue
 *
 * @brief  process the receive value event
 *
 * This function will get the value and call the receive routine
 *
 * @param[in]   xArg      event aggument
 *
 * @return      TRUE flush events
 *
 *****************************************************************************/
BOOL ManchesterCodec_ProcessCaptureValue( TASKARG xArg )
{
  U32UN tValue;

  #if ( MANCHESTERCODEC_ENABLE_INPUT_QUEUE == ON )
    // post to queue
    QueueManager_Get( QUEUE_ENUM_MANCHESTERRCV, ( PU8 )&tValue );
  #else
    // set the value
    tValue.uValue = xArg;
  #endif // MANCHESTERCODEC_ENABLE_INPUT_QUEUE
  
  // call the receive event
  ManchesterCodec_ProcessReceive( FALSE, tValue.awValue[ VALUE_INDEX ], tValue.awValue[ EDGE_INDEX ] );

  // return true
  return( TRUE );
}



/**@} EOF ManchesterCodec_cfg.c */
