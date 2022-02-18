/******************************************************************************
 * @file HardwareDebug.h
 *
 * @brief hardware debug declarations
 *
 * This file provides the declarations for the hardware debug 
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
 * \addtogroup HardwareDebug
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _HARDWAREDEBUG_H
#define _HARDWAREDEBUG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HardwareDebug/HardwareDebug_prm.h"

// library includes -----------------------------------------------------------
#include "AsciiCommandHandler/AsciiCommandHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE ASCCMDENTRY g_atHardwareDebugCommandsTable[ ];

// global function prototypes --------------------------------------------------
extern  BOOL  HardwareDebug_Initialize( void );

/**@} EOF HardwareDebug.h */

#endif  // _HARDWAREDEBUG_H