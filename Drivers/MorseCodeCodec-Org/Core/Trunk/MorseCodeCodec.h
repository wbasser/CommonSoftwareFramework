/******************************************************************************
 * @file MorseCodeCodec.h
 *
 * @brief Morse Code CODEC declarations
 *
 * This file provides the declarations for the Morse Code CODEC
 *
 * @copyright Copyright (c) 2017 Cyber Intergration
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
#ifndef _MORSECODECODEC_H
#define _MORSECODECODEC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MorseCodeCodec/MorseCodeCodec_prv.h"

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of queue entries for transmit
#define MORSECODECODEC_NUM_QUEUEENTRIES         ( 80 )
#define MORSECODECODEC_SIZE_QUEUEENTRY          ( CODEENTRY_SIZE )

/// define the transmit task number of entries
#define MORSECODECODEC_XMT_NUM_EVENTS           ( MORSECODECODEC_NUM_QUEUEENTRIES + 4 )

/// define the receive task number of entries
#define MORSECODECODEC_RCV_NUM_EVENTS           ( 4 )

/// define the tone/on off events
#define MORSECODECODEC_TONE_ON_EVENT            ( 0x4523 )
#define MORSECODECODEC_TONE_OFF_EVENT           ( 0x4524 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  MorseCodeCodec_Initialize( void );
extern  BOOL  MorseCodeCodec_ProcessTransmit( TASKARG xArg );
extern  BOOL  MorseCodeCodec_ProcessReceive( TASKARG xArg );
extern  BOOL  MorseCodeCodec_TransmitChar( C8 cChar );

/**@} EOF MorseCodeCodec.h */

#endif  // _MORSECODECODEC_H