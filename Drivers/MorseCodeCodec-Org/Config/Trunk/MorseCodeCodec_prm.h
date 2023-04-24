/******************************************************************************
 * @file MorseCodeCodec_prm.h
 *
 * @brief Morse code codec parameter declarations
 *
 * This file provides the parameter declarations for the Morse Code CODEC
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
#ifndef _MORSECODECODEC_PRM_H
#define _MORSECODECODEC_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "QueueManager/QueueManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the ouptut character queue
#define MORSECODECODEC_OUTPUT_QUEUE_ENUM          ( QUEUE_ENUM_ILLEGAL )

/**@} EOF MorseCodeCodec_prm.h */

#endif  // _MORSECODECODEC_PRM_H