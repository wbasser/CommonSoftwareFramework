/******************************************************************************
 * @file MorseCodeCodec_cfg.c
 *
 * @brief morse code codec configuration implementation
 *
 * This file provides the implementation for the morse code codec
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
 * \addtogroup MorseCodeCodec
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MorseCodeCodec/MorseCodeCodec.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the transmit default WPM
#define XMT_WPM_DFLT                            ( 5 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function MorseCodeCodec_LocalInitialize
 *
 * @brief morse code codec local initiialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void MorseCodeCodec_LocalInitialize( void )
{
}

/******************************************************************************
 * @function MorseCodeCodec_GetXmtWpm
 *
 * @brief get the word per minute
 *
 * This function will return the current transmit WPM
 *
 * @return      transmit WPM
 *
 *****************************************************************************/
U8 MorseCodeCodec_GetXmtWpm( void )
{
  U8  nXmtWpm = XMT_WPM_DFLT;

  // return the WPM
  return( nXmtWpm );
}

/******************************************************************************
 * @function MorseCodeCodec_SetXmitTimerRate
 *
 * @brief set the transmit timer rate
 *
 * This function will set the transmit timer rate
 *
 * @param[in]   uTimerRate
 *
 *****************************************************************************/
void MorseCodeCodec_SetXmitTimerRate( U32 uTimerRate )
{
}

/******************************************************************************
 * @function MorseCodeCodec_PutTransmitQueue
 *
 * @brief put a character into the transmit queue
 *
 * This function will put the new character into the transmit queue
 *
 * @param[in]   tCode     code to transmit
 *
 *****************************************************************************/
void MorseCodeCodec_PutTransmitQueue( MORSEECODECODEDEF tCode )
{
}

/******************************************************************************
 * @function MorseCodeCOdec_GetXmitOutQueue
 *
 * @brief get an entry from the xmit output queue
 *
 * This function will get the first entry in the output queue
 *
 * @param[io]   ptCode      pointer to store the code
 *
 * @return      TRUE if valid, FALSE if Queue empty
 *
 *****************************************************************************/
BOOL MorseCodeCOdec_GetXmitOutQueue( PMORSEECODECODEDEF ptCode )
{
  BOOL  bQueueNotEmpty = FALSE;

  // return the status
  return( bQueueNotEmpty );
}

/******************************************************************************
 * @function MorseCodeCodec_GetRcvInpQueue
 *
 * @brief get an entry from the input queue
 *
 * This function will return the first entry in the input queue
 *
 * @param[io]   ptData    pointer to store the data
 *
 *****************************************************************************/
void MorseCodeCodec_GetRcvInpQueue( PMORSECODECODECDATA ptData )
{
}

/******************************************************************************
 * @function MorseCodeCodec_TimerCallback
 *
 * @brief timer callback
 *
 * This function will process the timer callback from a capture or
 * a overflow
 *
 * @param[in]   eEvent      timer event
 * @param[in]   eCapture    capture channel
 * @param[in]   wValue      capture value
 *
 *****************************************************************************/
//void MorseCodeCodec_TimerCallback( TIMERCBEVENT eEvent, TIMERCMPCAP eCapture, U16 wValue )
//{
//  MORSECODECODECDATA  tData;
  
//  // determine the event
//  switch( eEvent )
//  {
//    case TIMER_CBEVENT_CAPRE :
//    case TIMER_CBEVENT_CAPFE :
//      // post a value to the queue
//      tData.eEdge = ( eEvent == TIMER_CBEVENT_CAPRE ) ? MORSECODE_EDGE_KEYUP : MORSECODE_EDGE_KEYDN;
//      tData.wValue = wValue;
//      // QueueManager_PutTail( QUEUE_ENUM_KEYINP, ( PU8 )&tData );
//      break;
      
//    default :
//      break;
//  }
//}

/******************************************************************************
 * @function MorseCodeCodec_OutputControl
 *
 * @brief turn on/off the output control
 *
 * This function will turn on/off the output
 *
 * @param[in]   bState    current state
 *
 *****************************************************************************/
void MorseCodeCodec_OutputControl( BOOL bState )
{
}

/******************************************************************************
 * @function MorseCodeCodec_OutputRcvChar
 *
 * @brief output the received character
 *
 * This function will output the received character
 *
 * @param[in]   cChar   character to output
 *
 *****************************************************************************/
void MorseCodeCodec_OutputRcvChar( C8 cChar )
{
}

/**@} EOF MorseCodeCodec_cfg.c */
