/******************************************************************************
 * @file MorseCodeCodec_cfg.c
 *
 * @brief Morse Code Codec configuration implementation
 *
 * This file provides the implementation for the morse code codec implementation
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
#include "MorseCodeCodec/MorseCodeCodec_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function MorseCodeCodec_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void MorseCodeCodec_LocalInitialize( void )
{
}

/******************************************************************************
 * @function MorseCodeCodec_RcvTimerControl
 *
 * @brief morse code cocec receive timer control
 *
 * This function will start/stop the receive timer
 *
 * @param[in]   bState    state of the timer
 * @param[in]   uTime     time in microseconds
 *
 *****************************************************************************/
void MorseCodeCodec_RcvTimerControl( BOOL bState, U32 uTimeUsecs )
{
}

/******************************************************************************
 * @function MorseCodeCodec_XmtTimerControl
 *
 * @brief morse code cocec trnasmit timer control
 *
 * This function will start/stop the transmit timer
 *
 * @param[in]   bState    state of the timer
 * @param[in]   uTime     time in microseconds
 *
 *****************************************************************************/
void MorseCodeCodec_XmtTimerControl( BOOL bState, U32 wTime )
{
}

/******************************************************************************
 * @function MorseCodeCodec_PostRecvCode
 *
 * @brief post the receive code to the target device
 *
 * This function will post the latest received character to the target
 *
 * @param[in]   cChar     character received
 *
 *****************************************************************************/
void MorseCodeCodec_PostRecvCode( C8 cChar )
{
}

/******************************************************************************
 * @function MorseCodeCodec_PostXmitCode
 *
 * @brief post transmit code
 *
 * This function will post the transmit code to the queue
 *
 * @param[in]   tCodeEntry    code entry
 *
 *****************************************************************************/
void MorseCodeCodec_PostXmitCode( CODEENTRY tCodeEntry )
{
}

/******************************************************************************
 * @function MorseCodeCodec_TransmitControl
 *
 * @brief transit control
 *
 * This function will control the control line/tone generation
 *
 * @param[in]   bState    state of the control
 *
 *****************************************************************************/
void MorseCodeCodec_TransmitControl( BOOL bState )
{
}

/**@} EOF MorseCodeCodec_cfg.c */
