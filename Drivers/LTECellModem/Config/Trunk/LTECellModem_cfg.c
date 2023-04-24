/******************************************************************************
 * @file LTECellModem_cfg.c
 *
 * @brief LTE Cellular modem configuration implementation 
 *
 * This file provides the implementation for the LTE cellular modem
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
 * \addtogroup LTECellModem
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LTECellModem/LTECellModem.h"

// library includes -----------------------------------------------------------
#include "UART/Uart.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function LTECellModem_LocalIntialize
 *
 * @brief LTE cellular modem initialization
 *
 * This function initializes the cellular modem
 *
 *****************************************************************************/
void LTECellModem_LocalInitialize( void )
{
}

/******************************************************************************
 * @function LTECellModem_Write
 *
 * @brief output a block of characters
 *
 * This function will output a block of characters to the approrpiate device
 *
 * @param[in]    pcData     pointer to the data to send
 * @param[in]   nLength     length of the data
 *
 *****************************************************************************/
void LTECellModem_Write( PC8 pcData, U16 wLength )
{
  U16 wBytesWritten = 0;

  // write a byte of data to the device
  do
  {
    // write a block to the UART
    Uart_Write( UART_DEV_ENUM_LTE, ( PU8 )pcData, wLength, &wBytesWritten );
    wLength -= wBytesWritten;
    pcData += wBytesWritten;
  } while( wLength != 0 ); 
}

/******************************************************************************
 * @function LTECellModem_ProcessChar
 *
 * @brief process character task
 *
 * This function will process the incoming character
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE to flush events
 *
 *****************************************************************************/
BOOL LTECellModem_ProcessChar( TASKARG xArg )
{
  // call the handler
  LTECellModem_CharProcess(( U8 )xArg );

  // return true to flush event
  return( TRUE );
}

/******************************************************************************
 * @function LTECellModem_ProcessCtrl
 *
 * @brief process control task
 *
 * This function will process the control event
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE to flush events
 *
 *****************************************************************************/
BOOL LTECellModem_ProcessCtrl( TASKARG xArg )
{
  // call the base function
  LTECellModem_CtrlProcess( xArg ); 
  
  // return true to flush event
  return( TRUE );
}

/******************************************************************************
 * @function LTECellModem_StartStopTime
 *
 * @brief start/stop a timer
 *
 * This function will start/stop a timer based on time value
 *
 * @param[in]   wTimeMsecs      timer in milliseconds
 *
 *****************************************************************************/
void LTECellModem_StartStopTime( U16 wTimeMsecs )
{
  // check for value
  if ( wTimeMsecs == 0 )
  {
    // stop the timer
    TaskManager_StopTimer( TASK_SCHD_ENUM_LTECTRL );
  }
  else
  {
    // stop the timer
    TaskManager_StartTimer( TASK_SCHD_ENUM_LTECTRL, TASK_TIME_MSECS( wTimeMsecs ));
  }
}

/******************************************************************************
 * @function LTECellModem_PowerControl
 *
 * @brief turn off/on the power control
 *
 * This function will set the power control GPIO pin to its desired level
 *
 * @param[in]   bState
 *
 *****************************************************************************/
void LTECellModem_PowerControl( BOOL bState )
{
  // set the GPIO pin state
  Gpio_Set( GPIO_PIN_ENUM_LTEPWR, bState );
}

/******************************************************************************
 * @function LTECellModem_PostCtrlEvent
 *
 * @brief post an event to the control
 *
 * This function will post an event to the control task
 *
 * @param[in]   eEvent    event to post
 *
 *****************************************************************************/
void LTECellModem_PostCtrlEvent( LTELCLEVENT eEvent )
{
  // post the event to the control task
  TaskManager_PostEvent( TASK_SCHD_ENUM_LTECTRL, eEvent );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/

/**@} EOF LTECellModem_cfg.c */
