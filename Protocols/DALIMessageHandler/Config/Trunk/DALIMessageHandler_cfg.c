/******************************************************************************
 * @file  DALIMessageHandler_cfg.c
 *
 * @brief
 *
 * This file
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 *
 *
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "LightControlHandler/LightControlHandler.h"
#include "SerialNumber/SerialNumber.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function  DALImessageHandler_ProcessEvent
 *
 * @brief process the event 
 *
 * This function will call the appropriate handler
 *
 * @param[in]   xArg    event argument
 *
 * @return    TRUE to flush events
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ProcessEvent( TASKARG xArg )
{
  DALIMSGHANDEVENT  eEvent;

  // determine the argument
  switch( xArg )
  {
    case TASK_TIMEOUT_EVENT :
      eEvent = DALIMSGHAND_EVENT_TIMEOUT;
      break;

    case DALI_MANCHESTER_RCV_DONE_EVENT :
      eEvent = DALIMSGHAND_EVENT_RCVDONE;
      break;

    case DALI_MANCHESTER_RCV_EROR_EVENT :
      eEvent = DALIMSGHAND_EVENT_RCVERR;
      break;
    
    case DALI_MANCHESTER_XMT_DONE_EVENT : 
      eEvent = DALIMSGHAND_EVENT_XTMDONE;
      break;

    default :
      eEvent = DALIMSGHAND_EVENT_MAX;
      break;
  }

  // call the event handler
  DALIMessageHandler_EventHandler( eEvent );

  // return TRUE to flush events
  return( TRUE );
}

/******************************************************************************
 * @function  DALImessageHandler_ProcessTimer
 *
 * @brief process the timer
 *
 * This function will process the timerout event
 *
 * @param[in] xArg      argument
 *
 * @return    TRUE to flush events
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ProcessTimer( TASKARG xArg )
{
  // call the timeout handler
  DALIMessageHandler_TimeoutHandler( );

  // return true
  return( TRUE );
}

/******************************************************************************
 * @function  DALImessageHandler_ProcessOutput
 *
 * @brief process the output
 *
 * This function will process the output
 *
 * @param[in]  xArg   argument
 *
 * @return    TRUE to flush events
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ProcessOutput( TASKARG xArg )
{
  // call the output handler
  DALIMessageHandler_OutputHandler( );

  // return true
  return( TRUE );
}

/******************************************************************************
 * @function  DALImessageHandler_ProcessBusFail
 *
 * @brief process the bus fail event
 *
 * This function will process the bus fail event
 *
 * @param[in]  xArg   argument
 *
 * @return    TRUE to flush events
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ProcessBusFail( TASKARG xArg )
{
  // call the bus fail monitor
  DALIMessageHandler_BusFailHandler( );

  // return true
  return( TRUE );
}


/******************************************************************************
 * @function  DALIMessageHandler_OutputXmtMsg
 *
 * @brief outputs a transmit message
 *
 * This function will output a transmit response
 *
 * @param[in]     nResponse
 *
 *****************************************************************************/
void DALIMessageHandler_OutputXmtMsg( U8 nResponse )
{
  #if (( DALI_DEBUG_LEVEL == DALI_DEBUG_LEVEL_XMTONLY  ) || ( DALI_DEBUG_LEVEL == DALI_DEBUG_LEVEL_BOTH ))
  #endif
}

/******************************************************************************
 * @function  DALIMessageHandler_OutputRcvMsg
 *
 * @brief sets a receive message
 *
 * This function call the appropriate handler to output a receive message
 *
 * @param[in]     kptMsg    pointer to the message
 *
 *****************************************************************************/
void DALIMessageHandler_OutputRcvMsg( PDALIMSG ptMsg )
{
  #if (( DALI_DEBUG_LEVEL == DALI_DEBUG_LEVEL_RCVONLY  ) || ( DALI_DEBUG_LEVEL == DALI_DEBUG_LEVEL_BOTH ))
  #endif
}

/******************************************************************************
 * @function  DALIMessageHandler_StartReceive
 *
 * @brief start a receive
 *
 * This function will start the manchester coder to start a recevie message
 *
 * @param[in]   pnData    pointer to the data
 * @param[in]   nLength   expected length of the message
 *
 *****************************************************************************/
void DALIMessageHandler_StartReceive( PU8 pnData, U8 nLength )
{
  // call the manchester codec start message
  ManchesterCodec_Recv( pnData, nLength );
}

/******************************************************************************
 * @function  DALIMessageHandler_StopReceive
 *
 * @brief  stop the receive
 *
 * This function will stop/abort the receive message
 *
 *****************************************************************************/
void DALIMessageHandler_StopReceive( void )
{
  // call the stop receive
  ManchesterCodec_StopRecv( );
}

/******************************************************************************
 * @function  DALIMessageHandler_StartTransmit
 *
 * @brief starts a transmit process
 *
 * This function start the transmit process
 *
 * @param[in]   pnData    pointer to the data
 * @param[in]   nLength   expected length of the message
 *
 *****************************************************************************/
void DALIMessageHandler_StartTransmit( PU8 pnData, U8 nLength )
{
  // start the transmit process
  ManchesterCodec_Xmit( pnData, nLength );
}

/******************************************************************************
 * @function  DALImessageHandler_SetLedLevel
 *
 * @brief sets the led and table select 
 *
 * This function will set the LED level to the desired table and level
 *
 * @param[in]     wLevel        level to set to
 *
 *****************************************************************************/
void DALIMessageHandler_SetLightLevel( U16 wLevel )
{
  // insert your function call here
  LightControlHandler_SetLightLevel( wLevel );
}

/******************************************************************************
 * @function  DALImessageHandler_GetLedStatus
 *
 * @brief returns the state of the LED controller
 *
 * This function gets and returns the state of the LED controller
 *
 * @return    state of the LED controller
 *
 *****************************************************************************/
BOOL DALIMessageHandler_GetLightStatus( void )
{
  BOOL bStatus = FALSE;
  
  // insert the function call to get the current status of the LED controller
  
  // return the state of the led controller status
  return( bStatus );
}

/******************************************************************************
 * @function  DALImessageHandler_SeedRandom
 *
 * @brief seeds the random generator
 *
 * This function will seed the rnadom number generator
 *
 *****************************************************************************/
void DALIMessageHandler_SeedRandom( void )
{
  SERIALNUMBER tSerNum;

  // get the serial number
  SerialNumber_Get( &tSerNum );

  // seed the random number generator
  srand( tSerNum.tWords.tWord0.uValue );
}

/******************************************************************************
 * @function  DALIMessageHandler_GenerateRandom
 *
 * @brief generate the random number
 *
 * This function generates a random number
 *
 * @return    random number
 *
 *****************************************************************************/
U32 DALIMessageHandler_GenerateRandom( void )
{
  U32 uRandom = 0;
  
  // call the random number generator
  uRandom = rand( );
  
  // return the random number
  return( uRandom );
}

/******************************************************************************
 * @function  DALIMessageHandler_WriteMemoryBlock
 *
 * @brief write a memory block
 *
 * This function will write a memory block value
 *
 * @param[in]   nMemoryBlock    memory block #
 * @param[in]   nBlockAddress   block address
 * @param[in]   bBypassLockByte bypass thelock byte
 *
 * @return    TRUE if OK, FALSE if errors
 *
 *****************************************************************************/
BOOL DALIMessageHandler_WriteMemoryBlock(U8 nMemoryBlock, U8 nBlockAddress, U8 nValue, BOOL bBypassLockByte )
{
}

/******************************************************************************
 * @function  DALIMessageHandler_ReadMemoryBlock
 *
 * @brief read a memory block value
 *
 * This function will read a memory block value
 *
 * @param[in]   nMemoryBlock    memory block #
 * @param[in]   nBlockAddress   block address
 * @param[io]   pnValue         pointer to the read value storage
 *
 * @return    TRUE if OK, FALSE if errors
 *
 *****************************************************************************/
BOOL DALIMessageHandler_ReadMemoryBlock( U8 nMemoryBlock, U8 nBlockAddress, PU8 pnValue )
{
}

/******************************************************************************
 * @function  DALIMessageHandler_WriteMBParameter
 *
 * @brief write a memory block parameter
 *
 * This function will write a memory block value
 *
 * @param[in]     nParam          parameter number
 * @param[in]     pnValue         pointer to the parameter storage
 * @param[in]     bBypassLockByte enable the lock bypass
 *
 * @return    TRUE if OK, FALSE if not
 *
 *****************************************************************************/
BOOL DALIMessageHandler_WriteMBParameter( U8 nParam, U8 nValue, BOOL bBypassLockByte )
{
}

/******************************************************************************
 * @function  DALImessageHandler_GetDefaultValue
 *
 * @brief get a parameter value
 *
 * This function will get a parameter value
 *
 * @param[in]     nParam    parameter number
 * @param[in]     pnValue   pointer to the parameter storage
 *
 * @return    TRUE if OK, FALSE if not
 *
 *****************************************************************************/
BOOL DALIMessageHandler_GetDefaultValue( U8 nParam, PU8 pnValue )
{
}

/******************************************************************************
 * @function  DALImessageHandler_PutValue
 *
 * @brief put a parameter value
 *
 * This function will write the parameter value
 *
 * @param[in]     nParam    parameter number
 * @param[in]     nValue    parameter value
 *
 * @return    TRUE if OK, FALSE if not
 *
 *****************************************************************************/
BOOL DALIMessageHandler_PutValue( U8 nParam, U8 nValue )
{
}

/******************************************************************************
 * @function  DALImessageHandler_GetValue
 *
 * @brief get a parameter value
 *
 * This function will get a parameter value
 *
 * @param[in]     nParam    parameter number
 * @param[in]     pnValue   pointer to the parameter storage
 *
 * @return    TRUE if OK, FALSE if not
 *
 *****************************************************************************/
BOOL DALIMessageHandler_GetValue( U8 nParam, PU8 pnValue )
{
}

/******************************************************************************
 * @function  DALIMessageHandler_ResetUserDefaults
 *
 * @brief reset user defaults
 *
 * This function will reset the defaults to
 *
 *****************************************************************************/
void DALIMessageHandler_ResetUserDefaults( void )
{
}

/******************************************************************************
 * @function  DALIMessageHandler_GetBusStatus
 *
 * @brief  get the bus status
 *
 * This function will return the value of the bus
 *
 * @return      current status of the bus
 *
 *****************************************************************************/
BOOL DALIMessageHandler_GetBusStatus( void )
{
  BOOL  bStatus;

  // get the state of the GPIO pin
  Gpio_GetIrq( GPIO_IRQ_ENUM_DALIRX, &bStatus );

  // return the status of the bus
  return( bStatus );
}

/******************************************************************************
 * @function  DALIMessageHandler_StartEventTimer
 *
 * @brief  start the event timer
 *
 * This function will start the event timer
 *
 * @param[in]   delay time
 *
 *****************************************************************************/
void DALIMessageHandler_StartEventTimer( U32 uTimeUsec )
{
  // call the task manager start timer function
  TaskManager_StartTimer( TASK_SCHD_ENUM_DALIEVN, uTimeUsec );
}

/******************************************************************************
 * @function  DALIMessageHandler_StopEventTimer
 *
 * @brief  stop the event timer
 *
 * This function will call the stop timer function
 *
 *****************************************************************************/
void DALIMessageHandler_StopEventTimer( void )
{
  // call the task manager stop timer
  TaskManager_StopTimer( TASK_SCHD_ENUM_DALIEVN );
}

/******************************************************************************
 * @function  DALIMessageHandler_StartInitTimer
 *
 * @brief  start the init timer
 *
 * This function will start the initialize timer
 *
 *****************************************************************************/
void DALIMessageHandler_StartInitTimer( void )
{
  // call the task manager start timer
  TaskManager_StartTimer( TASK_SCHD_ENUM_DALITIM, 0 );
}

/******************************************************************************
 * @function  DALIMessageHandler_StopInitTimer
 *
 * @brief  stop the init timer
 *
 * This function will stop the initialize timer
 *
 *****************************************************************************/
void DALIMessageHandler_StopInitTimer( void )
{
  // call the task manager start timer
  TaskManager_StopTimer( TASK_SCHD_ENUM_DALITIM );
}

/**@} EOF DALIMessageHandler_cfg.c */
