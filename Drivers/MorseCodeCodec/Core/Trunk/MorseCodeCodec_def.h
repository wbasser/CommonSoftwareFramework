/******************************************************************************
 * @file MorseCodeCodec_def.h
 *
 * @brief Morse code codec definitions delcaratons
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
#ifndef _MORSECODECODEC_DEF_H
#define _MORSECODECODEC_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// define the key/up event
typedef enum _MORSECODEEDGE 
{
  MORSECODE_EDGE_KEYDN = 0,
  MORSECODE_EDGE_KEYUP,
  MORSECODE_EDGE_MAX
} MORSECODEEDGE;

// structures -----------------------------------------------------------------
typedef struct _MORSECODECODECDATA
{
  MORSECODEEDGE eEdge;     ///< edge
  U16           wValue;    ///< captured value
} MORSECODECODECDATA, *PMORSECODECODECDATA;
#define MORSECODECODECDATA_SIZE   sizeof( MORSECODECODECDATA )

/// define the code lookup table
typedef union _MORSEECODECODEDEF
{
  struct
  {
    U8 nLength;     // number of elements in morse code character
    U8 nCode;       // actual code - right justified
  } tFields;
  U16 wValue;
} MORSEECODECODEDEF, *PMORSEECODECODEDEF;
#define MORSEECODECODEDEF_SIZE                    sizeof( MORSEECODECODEDEF )

/**@} EOF MorseCodeCodec_Def.h */

#endif  // _MORSECODECODEC_DEF_H