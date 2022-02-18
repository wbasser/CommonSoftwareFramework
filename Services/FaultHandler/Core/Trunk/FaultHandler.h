/******************************************************************************
 * @file FaultHandler.h
 *
 * @brief fault handler declarations 
 *
 * This file provides the declarations for the fault handler
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
 * \addtogroup FaultHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _FAULTHANDLER_H
#define _FAULTHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "FaultHandler/FaultHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  FaultHandler_Initialize( void );
extern  void  FaultHandler_Display( U32 uFault );

/**@} EOF FaultHandler.h */

#endif  // _FAULTHANDLER_H