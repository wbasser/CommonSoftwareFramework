/******************************************************************************
 * @file EncoderHandler_cfg.h
 *
 * @brief encoder handler declarations 
 *
 * This file provides the declarations for the encoder handler
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
 * \addtogroup EncoderHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ENCODERHANDLER_CFG_H
#define _ENCODERHANDLER_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------

/// define the events for CW/CCW rotation

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  EncoderHandler_LocalInitialize( void );
extern  void  EncoderHandler_IrqCallback( U8 nPin, U8 nEvent, BOOL bState );

/**@} EOF EncoderHandler_cfg.h */

#endif  // _ENCODERHANDLER_H