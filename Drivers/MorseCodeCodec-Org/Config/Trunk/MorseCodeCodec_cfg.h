/******************************************************************************
 * @file MorseCodeCodec_cfg.h
 *
 * @brief Morse Code Codec configuration declarations
 *
 * This file provides the declarations for the morse code codec configuration
 * file
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

// local includes -------------------------------------------------------------
#include "MorseCodeCodec/MorseCodeCodec_prm.h"
#include "MorseCodeCodec/MorseCodeCodec_prv.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  MorseCodeCodec_LocalInitialize( void )
extern  void  MorseCodeCodec_RcvTimerControl( BOOL bState, U32 uTimeUsecs );
extern  void  MorseCodeCodec_XmtTimerControl( BOOL bState, U32 uTimeUsecs );
extern  void  MorseCodeCodec_PostRecvCode( C8 cChar );
extern  void  MorseCodeCodec_PostXmitCode( CODEENTRY tCodeEntry );
extern  void  MorseCodeCodec_TransmitControl( BOOL bState );

/**@} EOF MorseCodeCodec_cfg.h */

#endif  // _MORSECODECODEC_CFG_H