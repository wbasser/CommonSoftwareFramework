/******************************************************************************
 * @file NCOToneGenerator_prv.h
 *
 * @brief tone generator private declarations
 *
 * This file provides the declarations for any private modules
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
 * \addtogroup NCOToneGenerator
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _NCOTONEGENERATOR_PRV_H
#define _NCOTONEGENERATOR_PRV_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum number of steps
#define MAX_NUM_STEPS                     ( 4096 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE  U16 awSineTable[ ];

// global function prototypes --------------------------------------------------

/**@} EOF NCOToneGenerator_prv.h */

#endif  // _NCOTONEGENERATOR_PRV_H