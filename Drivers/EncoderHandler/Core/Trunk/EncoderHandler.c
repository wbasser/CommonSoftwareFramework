/******************************************************************************
 * @file EncoderHandler.c
 *
 * @brief encoder handler implemenation
 *
 * This file provides the implementation for the encoder handler
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "EncoderHandler/EncoderHandler.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the phase states
typedef enum _ENCHANDSTATE
{
  ENCHAND_STATE_PHA0PHB0 = 0,
  ENCHAND_STATE_PHA1PHB0,
  ENCHAND_STATE_PHA0PHB1,
  ENCHAND_STATE_PHA1PHB1
} ENCHANDSTATE;

// structures -----------------------------------------------------------------
/// define the control structure1
typedef union _ENCHANDCTL
{
  struct
  {
    BOOL  bPha : 1;
    BOOL  bPhb : 1;
  } tBits;
  U8  nByte;
} ENCHANDCTL, *PENCHANDCTL;
#define ENCHANDCTL_SIZE                         sizeof( ENCHANDCTL )


// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  ENCODERHANDDEF  tCallback;
static  ENCHANDCTL      tCurCtl;
static  U16             wCurCounts;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function EncoderHandler_Initiailize
 *
 * @brief initialization
 *
 * This function will perform any needed initializaton
 *
 * @return    TRUE if errors occured, FALSE otherwise
 *
 *****************************************************************************/
BOOL EncoderHandler_Initialize( void )
{
  BOOL bStatus = FALSE;

  // clear the current control
  tCurCtl.nByte = ENCHAND_STATE_PHA0PHB0;

  // call the local initialization
  bStatus = EncoderHandler_LocalInitialize( );

  // clear the callback
  tCallback.eType = ENCODERHANDNOTIF_TYPE_NONE;

  // clear the counts
  wCurCounts = 0;

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function EncoderHandler_ProcessDelta
 *
 * @brief encoder handler event handler
 *
 * This function will process the events posted to this handler
 *
 * @param[in]   nEvent    event
 *
 * @return    True to flush events
 * 
 *****************************************************************************/
BOOL EncoderHandler_ProcessDelta( TASKARG xArg )
{
  ENCHANDCTL      tNewCtl;
  BOOL            bPostEvent = FALSE;

  // build the new state
  tNewCtl.nByte = xArg & 0x03;

  // now determine the new state
  switch( tCurCtl.nByte )
  {
    case ENCHAND_STATE_PHA0PHB0 :
      if ( tNewCtl.nByte == ENCHAND_STATE_PHA1PHB1 )
      {
        // increment counts
        wCurCounts++;
        bPostEvent = TRUE;
      }
      break;

    case ENCHAND_STATE_PHA0PHB1 :
      if ( tNewCtl.nByte == ENCHAND_STATE_PHA1PHB0 )
      {
        // decrement counts
        wCurCounts--;
        bPostEvent = TRUE;
      }
      break;

    case ENCHAND_STATE_PHA1PHB0 :
    case ENCHAND_STATE_PHA1PHB1 :
    default :
      break;
  }

  // save the new control
  tCurCtl.nByte = tNewCtl.nByte;

  // if post event
  if ( bPostEvent == TRUE )
  {
    // process the type
    switch( tCallback.eType )
    {
      case ENCODERHANDNOTIF_TYPE_TASK :
        TaskManager_PostEvent( tCallback.tCallbacks.eTaskEnum, wCurCounts );
        break;

      case ENCODERHANDNOTIF_TYPE_CALB :
        tCallback.tCallbacks.pvCallback( wCurCounts );
        break;

      default :
        break;
    }
  }

  // return true to flush event
  return( TRUE );
}

/******************************************************************************
 * @function EncoderHandler_SetNotification
 *
 * @brief set the notification handler
 *
 * This function will set the notificiation type/handler
 *
 * @param[in]   ptDef     pointert to a callback definition
 *
 *****************************************************************************/
void EncoderHandler_SetNotification( PENCODERHANDDEF ptDef )
{
  // copy the definition
  memcpy( &tCallback, ptDef, ENCODERHANDDEF_SIZE );
}

/******************************************************************************
 * @function EncoderHandler_GetValue
 *
 * @brief get the current counts
 *
 * This function will return the current counts
 *
 * @return      current counts
 *
 *****************************************************************************/
U16 EncoderHandler_GetValue( void )
{
  // return the current counts
  return( wCurCounts );
}

/**@} EOF EncoderHandler.c */
