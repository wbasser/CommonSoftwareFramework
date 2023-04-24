/******************************************************************************
 * @file Ac_cfg.h
 *
 * @brief Analog Comparator Configuration Delcaration file 
 *
 * This file provides the declarations for the AC configuration module
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
 * \addtogroup Ac
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AC_CFG_H
#define _AC_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AC/Ac_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the AC channels
typedef enum _ACENUM
{
  // add your enumerations here
 
  // do not remove the below entries
  AC_ENUM_MAX,       ///< maximum number of AC defs
  AC_ENUM_ILLEGAL
} ACENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const ACDEF  g_atAcDefs[ ];

/**@} EOF Ac_cfg.h */

#endif  // _AC_CFG_H