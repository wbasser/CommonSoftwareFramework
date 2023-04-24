/******************************************************************************
 * @file FaultHandler_cfg.h
 *
 * @brief Fault Handler configuration declarations 
 *
 * This file provides the declarations for the FaultHandler
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
#ifndef _FAULTHANDLER_CFG_H
#define _FAULTHANDLER_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  FaultHandler_LocalInitialize( void );
extern  void  FaultHandler_GpioConfigure( void );
extern  void  FaultHandler_GpioControl( BOOL bState );
extern  void  FaultHandler_DelayMsec( U16 wMsec );

/**@} EOF FaultHandler_cfg.h */

#endif  // _FAULTHANDLER_CFG_H