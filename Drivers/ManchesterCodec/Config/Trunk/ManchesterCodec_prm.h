/******************************************************************************
 * @file ManchesterCodec_prm.h
 *
 * @brief 
 *
 * This file 
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
 
// ensure only one instantiation
#ifndef _MANCHESTERCODEC_PRM_H
#define _MANCHESTERCODEC_PRM_H

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
// define the size of the arguments
#define MANCHESTERCODEC_ARG_SIZE_BYTES          ( TASK_TSKARG_SIZE_BYTES )

/// define the number of events for the task manager
#define MANCHESTERCODEC_RECV_NUM_EVENTS         ( 16 )

/// define the macro to enable dynamic initialization
#define MANCHESTERCODEC_ENALBE_DYNAMICINIT      ( OFF )

#if ( MANCHESTERCODEC_ENALBE_DYNAMICINIT == OFF )
  /// define the manchester baud rate
  #define MANCHESTERCODEC_BAUD_RATE             ( 1200 )

  /// define the manchester half bit time
  #define MANCHESTERCODEC_HALFBIT_FREQ          ( MANCHESTERCODEC_BAUD_RATE * 2 )

  /// define the number of sync bits
  #define MANCHESTERCODEC_NUM_SYNC_BITS         ( 1 )

  /// define the number of stop bits
  #define MANCHESTERCODEC_NUM_STOP_BITS         ( 2 )

  /// define the macro to invert the receive polarity
  #define MANCHESTERCODEC_ENABLE_RCVPOLARITY    ( OFF )

  /// define the macro to define bit tolerance
  #define MANCHESTERCODEC_BIT_TOLERANCE_PCT     ( 20 )
#endif

/// define the macro to use queues for incoming data
#define MANCHESTERCODEC_ENABLE_INPUT_QUEUE      ( ON )

/// define the number of queue entries 
#if ( MANCHESTERCODEC_ENABLE_INPUT_QUEUE == ON )
  #define MANCHESTERCOCDEC_RCVQUEUE_NUMENTRIES  ( MANCHESTERCODEC_RECV_NUM_EVENTS ) 
  #define MANCHESTERCODEC_RCVQUEUE_ENTRYSIZE    ( sizeof ( U32 ))
#endif // MANCHESTERCODEC_ENABLE_INPUT_QUEUE

/// define the TX timer
#define MANCHESTERCODEC_TXTIMER_SELECT          ( TIMER_CHAN_4 )

/// define the RX timer
#define MANCHESTERCODEC_RXTIMER_SELECT          ( TIMER_CHAN_5 )

/// define the debug enable
#define MANCHESTERCODEC_RCVDBG_ENABLE           ( 0 )

/// define the transmit debug enable
#define MANCHESTERCODEC_XMTDBG_ENABLE           ( 0 )

/// define the base value for transmit debug
#define MANCHESTERCODEC_DBGXMT_BASE             ( 0x4000 )

/// define the base value for receive debug
#define MANCHESTERCODEC_DBGRCV_BASE             ( 0x5000 )

/**@} EOF ManchesterCodec_prm.h */

#endif  // _MANCHESTERCODEC_PRM_H
