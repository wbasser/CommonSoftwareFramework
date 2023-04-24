/******************************************************************************
 * @file GraphBasic_def.h
 *
 * @brief graphics basic library definitions declaration 
 *
 * This file provides the declarations for the graphics library
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
 * \addtogroup GraphBasic
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GRAPHBASIC_DEF_H
#define _GRAPHBASIC_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the colors
typedef enum _GRAPHBASICCLR
{
  GRAPHBASIC_CLR_ENUM_BLK = 0,
  GRAPHBASIC_CLR_ENUM_RED,
  GRAPHBASIC_CLR_ENUM_GRN,
  GRAPHBASIC_CLR_ENUM_BLU,
  GRAPHBASIC_CLR_ENUM_YEL,
  GRAPHBASIC_CLR_ENUM_CYN,
  GRAPHBASIC_CLR_ENUM_PUR,
  GRAPHBASIC_CLR_ENUM_WHT,
  GRAPHBASIC_CLR_MAX
} GRAPHBASICCLR;

/// enumerate the pixel acton
typedef enum _GRAPHBASICPIXACT
{
  GRAPHBASIC_PIXACT_CLR = 0,
  GRAPHBASIC_PIXACT_SET,
  GRAPHBASIC_PIXACT_XOR,
  GRAPHBASIC_PIXACT_MAX
} GRAPHBASICPIXACT;

// structures -----------------------------------------------------------------

/**@} EOF GraphBasic_def.h */

#endif  // _GRAPHBASIC_DEF_H