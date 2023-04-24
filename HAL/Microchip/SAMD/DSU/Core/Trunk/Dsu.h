/******************************************************************************
 * @file Dsu.h
 *
 * @brief Device Service Unit (DSU) declarations
 *
 * This file provides the declarations for the DSU
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
 * \addtogroup Dsu
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DSU_H
#define _DSU_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  Dsu_CalculateCRC( U32 uStartAddress, U32 uLength, PU32 puCrc );

/**@} EOF Dsu.h */

#endif  // _DSU_H