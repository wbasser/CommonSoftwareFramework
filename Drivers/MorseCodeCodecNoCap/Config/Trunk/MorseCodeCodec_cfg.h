/******************************************************************************
 * @file MorseCodeCodec_cfg.h
 *
 * @brief morse code codec configurtion declarations
 *
 * This file provides the declarations for the morse code code aconfiguration
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
 * \addtogroup MorseCodeCodec
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MORSECODECODEC_CFG_H
#define _MORSECODECODEC_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "MorseCodeCodec/MorseCodeCodec_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the number of events/exec rate for the low level transmit task
#define MORSECODECODEC_XMT_NUM_EVENTS           ( 4 )
#define MORSECODECODEC_XMT_EXEC_MSECS           ( 2 )
#define MORSECODECODEC_XMT_EXEC_RATE            ( TASK_TIME_MSECS( MORSECODECODEC_XMT_EXEC_MSECS ))

/// define the number of receive queue entries
#define MORSECODECODEC_RCV_NUM_EVENTS           ( 4 )
#define MORSECODECODEC_RCV_EXEC_MSECS           ( 1 )
#define MORSECODECODEC_RCV_EXEC_RATE            ( TASK_TIME_MSECS( MORSECODECODEC_RCV_EXEC_MSECS ))

/// define the number of queue entries
#define MORSECODECODEC_XMT_NUM_ENTRIES          ( 80 )
#define MORSECODECODEC_RCV_NUM_ENTRIES          ( MORSECODECODEC_XMT_NUM_EVENTS )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the task argument
typedef U16     MORSECODE_CODEC_TASKARG;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  MorseCodeCodec_LocalInitialize( void );
extern  U8    MorseCodeCodec_GetXmtWpm( void );
extern  void  MorseCodeCodec_SetXmitTimerRate( U32 uTimerRate );
extern  void  MorseCodeCodec_PutTransmitQueue( MORSEECODECODEDEF tCode );
extern  void  MorseCodeCodec_PostEventToRcvTask( U16 wEvent );
extern  BOOL  MorseCodeCodec_GetXmitOutQueue( PMORSEECODECODEDEF ptCode );
extern  void  MorseCodeCodec_GetRcvInpQueue( PMORSECODECODECDATA ptData );
extern  void  MorseCodeCodec_OutputControl( BOOL bState );
extern  void  MorseCodeCodec_OutputRcvChar( C8 cChar );

/**@} EOF MorseCodeCodec_cfg.h */

#endif  // _MORSECODECODEC_CFG_H