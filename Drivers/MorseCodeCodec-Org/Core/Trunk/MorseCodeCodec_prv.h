/******************************************************************************
 * @file MorseCodeCodec_prv.h
 *
 * @brief morse code codec private declarations
 *
 * This file provides the declarations for the private code for the morse code
 * CODEC
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
#ifndef _MORSECODECODEC_PRV_H
#define _MORSECODECODEC_PRV_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
// define the code table length
#define	CODE_TABLE_LENGTH	'Z' - ' ' + 1

// define the code table offset
#define	CODE_TABLE_OFFSET	' '

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the code lookup table
typedef struct _CODEENTRY
{
	U8	nLength;		// number of elements in morse code character
	U8	nCode;			// actual code - right justified
} CODEENTRY, *PCODEENTRY;
#define CODEENTRY_SIZE                    sizeof( CODEENTRY )

// global parameter declarations -----------------------------------------------
extern  const CODE CODEENTRY  atCodes[ ];

/**@} EOF MorseCodeCodec_prv.h */

#endif  // _MORSECODECODEC_PRV_H