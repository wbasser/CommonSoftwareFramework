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
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of arguments
#define ENCHAND_NUM_EVENTS                      ( 16 )

// enumerations ---------------------------------------------------------------
/// enumerate the notify types
typedef enum _ENCODERHANDNOTIFTYPE
{
  ENCODERHANDNOTIF_TYPE_NONE = 0,
  ENCODERHANDNOTIF_TYPE_TASK,
  ENCODERHANDNOTIF_TYPE_CALB,
  ENCODERHANDNOTIF_TYPE_MAX
} ENCODERHANDNOTIFTYPE;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void  ( *PVENCHANDCB )( U16 wValue );

/// define notification structure
typedef struct _ENCODERHANDDEF 
{
  ENCODERHANDNOTIFTYPE  eType;
  union
  {
    TASKSCHDENUMS eTaskEnum;
    PVENCHANDCB   pvCallback;
  } tCallbacks;
} ENCODERHANDDEF, *PENCODERHANDDEF;
#define ENCODERHANDDEF_SIZE                     sizeof( ENCODERHANDDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  EncoderHandler_Initialize( void );
extern  BOOL  EncoderHandler_ProcessDelta( TASKARG xArg );
extern  void  EncoderHandler_SetNotification( PENCODERHANDDEF ptDef );
extern  U16   EncoderHandler_GetValue( void );

/**@} EOF EncoderHandler.h */

#endif  // _ENCODERHANDLER_H