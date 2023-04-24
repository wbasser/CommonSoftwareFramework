/******************************************************************************
 * @file MorseCodeCodec_prv.c
 *
 * @brief Morse Code CODEC private implementation
 *
 * This file provides the implementation for the private members for the morse
 * code CODEC
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MorseCodeCodec/MorseCodeCodec_prv.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE CODEENTRY  atCodes[ CODE_TABLE_LENGTH ] =
{
	{ 0x00,	0x00 },		// 0x20 - no code
	{ 0x00,	0x00 },		// 0x21 - no code
	{ 0x00,	0x00 },		// 0x22 - no code
	{ 0x00,	0x00 },		// 0x23 - no code
	{ 0x07,	0x09 },		// 0x24 - $
	{ 0x00,	0x00 },		// 0x25 - no code
	{ 0x00,	0x00 },		// 0x26 - no code
	{ 0x06,	0x33 },		// 0x27 - '
	{ 0x00,	0x00 },		// 0x28 - no code
	{ 0x00,	0x00 },		// 0x29 - no code
	{ 0x06,	0x05 },		// 0x2A - *
	{ 0x05,	0x0A },		// 0x2B - +
	{ 0x06,	0x1E },		// 0x2C - ,
	{ 0x05,	0x11 },		// 0x2D - -
	{ 0x06,	0x38 },		// 0x2E - .
	{ 0x00,	0x00 },		// 0x2F - no code
	{ 0x05,	0x1F },		// 0x30 - 0
	{ 0x05,	0x0F },		// 0x31 - 1
	{ 0x05,	0x07 },		// 0x32 - 2
	{ 0x05,	0x03 },		// 0x33 - 3
	{ 0x05,	0x01 },		// 0x34 - 4
	{ 0x05,	0x00 },		// 0x35 - 5
	{ 0x05,	0x10 },		// 0x36 - 6
	{ 0x05,	0x18 },		// 0x37 - 7
	{ 0x05,	0x1C },		// 0x38 - 8
	{ 0x05,	0x1E },		// 0x39 - 9
	{ 0x06,	0x0C },		// 0x3A - :
	{ 0x00,	0x00 },		// 0x3B - no code
	{ 0x00,	0x00 },		// 0x3C - no code
	{ 0x05,	0x11 },		// 0x3D - >		              *** ERROR SAME CODE AS 0x2D
	{ 0x00,	0x00 },		// 0x3E - no code
	{ 0x05,	0x01 },		// 0x3F - ?
	{ 0x00,	0x00 },		// 0x40 - no code
	{ 0x02,	0x01 },		// 0x41 - A
	{ 0x04,	0x08 },		// 0x42 - B
	{ 0x04,	0x0A },		// 0x43 - C
	{ 0x03,	0x04 },		// 0x44 - D
	{ 0x01,	0x00 },		// 0x45 - E 
	{ 0x04,	0x02 },		// 0x46 - F
	{ 0x03,	0x06 },		// 0x47 - G
	{ 0x04,	0x00 },		// 0x48 - H  
	{ 0x02,	0x00 },		// 0x49 - I
	{ 0x04,	0x07 },		// 0x4A - J
	{ 0x03,	0x05 },		// 0x4B - K
	{ 0x04,	0x04 },		// 0x4C - L
	{ 0x02,	0x03 },		// 0x4D - M
	{ 0x02,	0x02 },		// 0x4E - N
	{ 0x03,	0x07 },		// 0x4F - O
	{ 0x04,	0x06 },		// 0x50 - P
	{ 0x04,	0x0D },		// 0x51 - Q
	{ 0x03,	0x02 },		// 0x52 - R
	{ 0x03,	0x00 },		// 0x53 - S
	{ 0x01,	0x01 },		// 0x54 - T
	{ 0x03,	0x01 },		// 0x55 - U
	{ 0x04,	0x01 },		// 0x56 - V
	{ 0x03,	0x03 },		// 0x57 - W
	{ 0x04,	0x09 },		// 0x58 - X
	{ 0x04,	0x0B },		// 0x59 - Y
	{ 0x04,	0x0C },		// 0x5A - Z
};

/**@} EOF MorseCodeCodec_prv.c */
