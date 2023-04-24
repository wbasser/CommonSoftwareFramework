/******************************************************************************
 * @file MorseCodeCodec.h
 *
 * @brief Morse code codec delcaratons
 *
 * This file provides the declarations for the morse code codedc
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
#ifndef _MORSECODECODEC_H
#define _MORSECODECODEC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MorseCodeCodec/MorseCodeCodec_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  MorseCodeCodec_Initialize( void );
extern  void  MorseCodeCodec_Config( BOOL bState );
extern  U8    MorseCodeCodec_GetRcvWpm( void );
extern  void  MorseCodeCodec_XmitChar( C8 cChar );
extern  void  MorseCodeCodec_ProcessXmtCtl( void );
extern  void  MorseCodeCodec_ProcessRcvCtl( MORSECODE_CODEC_TASKARG xArg );

/**@} EOF MorseCodeCodec.h */

#endif  // _MORSECODECODEC_H