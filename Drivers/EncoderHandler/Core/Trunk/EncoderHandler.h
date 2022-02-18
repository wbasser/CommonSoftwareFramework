/******************************************************************************
 * @file EncoderHandler.h
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
#ifndef _ENCODERHANDLER_H
#define _ENCODERHANDLER_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "EncoderHandler/EncoderHandler_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the events for CW/CCW rotation
#define ENCHANDLER_CW_EVENT                     ( 0xF0 )
#define ENCHANDLER_CCW_EVENT                    ( 0xF1 )
#define ENCHANDLER_CWX10_EVENT                  ( 0xF2 )
#define ENCHANDLER_CCWX10_EVENT                 ( 0xF3 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  EncoderHandler_Initialize( void );
extern  void  EncoderHandler_EventProcess( U8 nEvent );

/**@} EOF EncoderHandler.h */

#endif  // _ENCODERHANDLER_H