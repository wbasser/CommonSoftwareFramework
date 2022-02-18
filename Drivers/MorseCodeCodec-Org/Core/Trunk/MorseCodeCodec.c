/******************************************************************************
 * @file MorseCodeCodec.c
 *
 * @brief Morse Code CODEC implementation
 *
 * This file provides the implementation for the morse code codec
 *
 * @copyright Copyright (c) 2017 Cyber Intergration
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MorseCodeCodec/MorseCodeCodec.h"
#include "MorseCodeCodec/MorseCodeCodec_prm.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "ParameterManager/ParameterManager.h"
#include "QueueManager/QueueManager.h"
#include "StateExecutionEngine/StateExecutionEngine.h"
#include "SystemTick/SystemTick.h"
#include "ToneGenerator/ToneGenerator.h"

// Macros and Defines ---------------------------------------------------------
/// define the special characters
#define MORSECODE_CODEC_CH_SPC                          ( ' ' )
#define MORSECODE_CODEC_CH_ERR                          ( '#' )

/// define the code put event
#define XMIT_CHAR_PUT_EVENT                             ( QUEUEGETEMPTY_EVENT( QUEUE_ENUM_MORSECODEXMT )) 

/// define the unit time for a wpm
#define WPM_BASE_TIME_MSEC                              ( 1200 )

// enumerations ---------------------------------------------------------------
/// enumerate the XMT state
typedef enum _XMTSTATE
{
  XMT_STATE_IDLE = 0,
  XMT_STATE_KYDN,
  XMT_STATE_KYUP,
  XMT_STATE_DELY,
  XMT_STATE_MAX
} XMTSTATE;

/// enumerate the RCV states
typedef enum _RCVSTATE
{
  RCV_STATE_IDLE = 0,
  RCV_STATE_TNON,
  RCV_STATE_TNOFF,
  RCV_STATE_ICDLY,
  RCV_STATE_MAX
} RCVSTATE;

/// enumerate the symbol types
typedef enum _SYMCODE
{
  SYMCODE_DOT = 0,
  SYMCODE_DASH
} SYMCODE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static	S32           lXmtDotTimeUsec;
static  U32           lRcvDotTimeUsec;
static	S32           lXmtSymTimeUsec;
STATEEXECENGCONTROL   tXmtStateCtl;
STATEEXECENGCONTROL   tRcvStateCtl;
static  CODEENTRY     tCurXmtCode;
static  CODEENTRY     tCurRcvCode;
static  S32           lCurTimeUsec;

// local function prototypes --------------------------------------------------
static  C8    CodeToAscii( PCODEENTRY ptCode );
static  BOOL  AsciiToCode( C8 cChar, PCODEENTRY ptCode );
static  S32   GetElementTimeUsec( void );
static  void  OutputControl( BOOL bState );
static  void  OutputCode( void );

// XMT_STATE_IDLE functionss
static  U8    XmtStateIdle_Exc( STATEEXECENGARG xArg );

// XMT_STATE_KYDN functions
static  void  XmtStateKydn_Ent( void );
static  U8    XmtStateKydn_Exc( STATEEXECENGARG xArg );

// XMT_STATE_KYUP functions
static  void  XmtStateKyup_Ent( void );
static  U8    XmtStateKyup_Exc( STATEEXECENGARG xArg );

// XMT_STATE_DELY functions
static  U8    XmtStateDely_Exc( STATEEXECENGARG xArg );

// RCV_STATE_IDLE functions
static  void  RcvStateIdle_Ext( void );

// RCV_STATE_TNON functions
static  void  RcvStateTnon_Ent( void );
static  U8    RcvStateTnon_Exc( STATEEXECENGARG xArg );

// RCV_STATE_TNOFF functions
static  void  RcvStateTnoff_Ent( void );
static  U8    RcvStateTnoff_Exc( STATEEXECENGARG xArg );

// RCV_STATE_ICDLY functions
static  void  RcvStateIcdly_Ent( void );
static  U8    RcvStateIcdly_Exc( STATEEXECENGARG xArg );

// constant parameter initializations -----------------------------------------
/// instantiate the XMT state table
static  const CODE STATEEXECENGTABLE  atXmtStates[ XMT_STATE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( XMT_STATE_IDLE, NULL,             XmtStateIdle_Exc, NULL, NULL ),
  STATEXECENGETABLE_ENTRY( XMT_STATE_KYDN, XmtStateKydn_Ent, XmtStateKydn_Exc, NULL, NULL ),
  STATEXECENGETABLE_ENTRY( XMT_STATE_KYUP, XmtStateKyup_Ent, XmtStateKyup_Exc, NULL, NULL ),
  STATEXECENGETABLE_ENTRY( XMT_STATE_DELY, NULL,             XmtStateDely_Exc, NULL, NULL ),
};

/// instantiate the events for the idle state
static  const CODE STATEEXECENGEVENT atRcvIdleEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( MORSECODECODEC_TONE_ON_EVENT, RCV_STATE_TNON, FALSE ),
  STATEEXECENGEVENT_END( )
};

/// instantiate the RCV state table
static  const CODE STATEEXECENGTABLE  atRcvStates[ RCV_STATE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( RCV_STATE_IDLE,  NULL,              NULL,              RcvStateIdle_Ext, atRcvIdleEvents  ),
  STATEXECENGETABLE_ENTRY( RCV_STATE_TNON,  RcvStateTnon_Ent,  RcvStateTnon_Exc,  NULL,             NULL             ),
  STATEXECENGETABLE_ENTRY( RCV_STATE_TNOFF, RcvStateTnoff_Ent, RcvStateTnoff_Exc, NULL,             NULL             ),
  STATEXECENGETABLE_ENTRY( RCV_STATE_ICDLY, RcvStateIcdly_Ent, RcvStateTnoff_Exc, NULL,             NULL             ),
};

/******************************************************************************
 * @function MorseCodeCodec_Initialize
 *
 * @brief morse code CODEC initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void MorseCodeCodec_Initialize( void )
{
  PARAMARG  xParam;

  // get the words per minute/calculate/set the transmit dot time
  ParameterManager_GetValue( PARAMSEL_ENUM_WPM, &xParam );
  lXmtDotTimeUsec = ( WPM_BASE_TIME_MSEC / xParam ) * 1000;
  lRcvDotTimeUsec = lXmtDotTimeUsec;

  // initialize the transmit/receive state engine
  StateExecutionEngine_Initialize( &tXmtStateCtl, XMT_STATE_IDLE );
  StateExecutionEngine_Initialize( &tRcvStateCtl, RCV_STATE_IDLE );
}

/******************************************************************************
 * @function MorseCodeCodec_ProcessTransmit
 *
 * @brief process the transmit
 *
 * This function will process transmit
 *
 * @param[in]   xArg      argument
 *
 * @return      TRUE    if event handler, FALSE if not
 *
 *****************************************************************************/
BOOL MorseCodeCodec_ProcessTransmit( TASKARG xArg )
{
  // set the flush event flag
  tXmtStateCtl.bFlushEvent = TRUE;

  // call the state execution engine
  StateExecutionEngine_Process( &tXmtStateCtl, ( STATEEXECENGARG )xArg );

  // return the flush event status
  return( tXmtStateCtl.bFlushEvent );
}

/******************************************************************************
 * @function MorseCodeCodec_Receive
 *
 * @brief process the receive
 *
 * This function will process receive events
 *
 * @param[in]   xArg      argument
 *
 * @return      TRUE    flush events
 *
 *****************************************************************************/
BOOL MorseCodeCodec_ProcessReceive( TASKARG xArg )
{
  // set the flush event flag
  tRcvStateCtl.bFlushEvent = TRUE;

  // call the state execution engine
  StateExecutionEngine_Process( &tRcvStateCtl, ( STATEEXECENGARG )xArg );

  // return the flush event status
  return( tRcvStateCtl.bFlushEvent );
}

/******************************************************************************
 * @function MorseCodeCodec_TransmitChar
 *
 * @brief transmit a character
 *
 * This function will translate an ASCII character in to a code, if valid, it
 * will post the new code to the queue
 *
 * @param[in]   cChar       transmit character
 *
 * @return TRUE if no errors, FALSE if error
 *
 *****************************************************************************/
BOOL MorseCodeCodec_TransmitChar( C8 cChar )
{
  BOOL  bResult = TRUE;

  CODEENTRY tCodeEntry;

  // translate it
  if (( bResult = AsciiToCode( cChar, &tCodeEntry )) == TRUE )
  {
    // valid character - add to queue
    QueueManager_PutTail( QUEUE_ENUM_MORSECODEXMT, ( PU8 )&tCodeEntry );
  }

  // return the result
  return( bResult );
}

/******************************************************************************
 * @function CodeToAscii
 *
 * @brief convert a code to an ASCII character
 *
 * This function will attempt to find a matching code entry in the table
 * and returns it equivalent ASCII character
 *
 * @param[in]   ptCode      pointer to the code entry
 *
 * @return      Character  space if code length is zero, # if invalid entry
 *
 *****************************************************************************/
static C8 CodeToAscii( PCODEENTRY ptCode )
{
  C8          cChar = MORSECODE_CODEC_CH_SPC;
  U8          nIndex;
  PCODEENTRY  ptEntry;

  // check for zero length
  if( ptCode->nLength != 0 )
  {
    // now search the table for an entry
    for ( nIndex = 0; nIndex < CODE_TABLE_LENGTH; nIndex++ )
    {
      // get a pointer to the table
      ptEntry = ( PCODEENTRY )&atCodes[ nIndex ];

      // if length and code match
      if (( ptCode->nCode == ptEntry->nCode ) && ( ptCode->nLength == ptEntry->nCode )) 
      {
        // character found
        cChar = nIndex + CODE_TABLE_OFFSET;

        // exit search loop
        break;
      }
    }

    // test for character not found
    if ( cChar == MORSECODE_CODEC_CH_SPC )
    {
      // error set the error character
      cChar = MORSECODE_CODEC_CH_ERR;
    }
  }

  // return the character
  return( cChar );
}

/******************************************************************************
 * @function AsciiToCode
 *
 * @brief convert an ASCII character to a code entry
 *
 * This function will convert an ascii to a code entry if a valid symbol
 *
 * @param[in]   cChar
 * @param[io]   pointer to store the code entry
 *
 * @return      TRUE if valid character, FALSE if not
 *
 *****************************************************************************/
static BOOL AsciiToCode( C8 cChar, PCODEENTRY ptCode )
{
  BOOL  bResult = FALSE;

	// adjust for table offset
	cChar -= CODE_TABLE_OFFSET;

  // check if character within range
	if ( cChar < CODE_TABLE_LENGTH )
  {
    // copy the code entry
    MEMCPY_P( ptCode, &atCodes[ cChar ], CODEENTRY_SIZE );

    // set good flag
    bResult = TRUE;
  }

  // return the status
  return( bResult );
}

/******************************************************************************
 * @function GetElementTImeUsec
 *
 * @brief get the next element time in usec
 *
 * This function will return the next element time in usecs if available
 *
 * @return  element time if available, or 0 if done
 *
 *****************************************************************************/
static S32 GetElementTimeUsec( void )
{
  S32 lElementTime = 0;

  // decrement the length and test for 0
  if ( tCurXmtCode.nLength-- != 0 )
  {
    // set the time
    lElementTime = ( tCurXmtCode.nCode & BIT( tCurXmtCode.nLength )) ? 3 : 1;
    lElementTime *= lXmtDotTimeUsec;
  }

  // return the element time
  return( lElementTime );
}

/******************************************************************************
 * @function OutputControl
 *
 * @brief control the transmit output
 *
 * This function will set/clear the key output as well as turn on/off tone
 * generator
 *
 * @param[in]   bState    desired state
 *
 *****************************************************************************/
static void OutputControl( BOOL bState )
{
  // set the appropriate state on the key down/control the tone
  Gpio_Set( GPIO_PIN_ENUM_KEYOUT, bState );
  ToneGenerator_Control( bState );
}

/******************************************************************************
 * @function OutputCode
 *
 * @brief output the code
 *
 * This function will post the translated code to the output queue
 *
 *****************************************************************************/
static void OutputCode( void )
{
  C8  cChar;

  // translate the character
  cChar = CodeToAscii( &tCurRcvCode );

  // post it to the queue
  QueueManager_PutTail( MORSECODECODEC_OUTPUT_QUEUE_ENUM, ( PU8 )&cChar );

  // clear the character
  tCurRcvCode.nLength = 0;
  tCurRcvCode.nCode = 0;
}

/******************************************************************************
 * XMT_STATE_IDLE functionss
 *****************************************************************************/
static U8 XmtStateIdle_Exc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // check for queue put
  if ( xArg == XMIT_CHAR_PUT_EVENT )
  {
    // get the entry from the Queue
    QueueManager_Get( QUEUE_ENUM_MORSECODEXMT, ( PU8 )&tCurXmtCode );

    // determine if this is a space
    if (( tCurXmtCode.nCode == 0 ) && ( tCurXmtCode.nLength == 0 ))
    {
      // set the next state to delay
      nNextState = XMT_STATE_DELY;

      // set the task time 
      TaskManager_StartTimer( TASK_SCHD_ENUM_MORSECODEXMIT, lXmtDotTimeUsec * 6 );
    }
    else
    {
      // set the task time
      TaskManager_StartTimer( TASK_SCHD_ENUM_MORSECODEXMIT, GetElementTimeUsec( ));

      // set next state to key down
      nNextState = XMT_STATE_KYDN;
    }
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * XMT_STATE_KYDN functions
 *****************************************************************************/
static void XmtStateKydn_Ent( void )
{
  // turn on the key
  OutputControl( ON );
}

static U8 XmtStateKydn_Exc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // if this is a timeout event
  if ( xArg == TASK_TIMEOUT_EVENT )
  {
    // set up for inter element
    TaskManager_StartTimer( TASK_SCHD_ENUM_MORSECODEXMIT, lXmtDotTimeUsec );
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * XMT_STATE_KYUP functions
 *****************************************************************************/
static void XmtStateKyup_Ent( void )
{
  // turn off the key
  OutputControl( OFF );
}

static U8 XmtStateKyup_Exc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * XMT_STATE_DELY functions
 *****************************************************************************/
static U8 XmtStateDely_Exc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // check for timeout
  if ( xArg == TASK_TIMEOUT_EVENT )
  {
    // go back to idle
    nNextState = XMT_STATE_IDLE;
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * RCV_STATE_IDLE functions
 *****************************************************************************/
static void RcvStateIdle_Ext( void )
{
  // clear the character length/code
  tCurRcvCode.nLength = 0;
  tCurRcvCode.nCode = 0;
}

/******************************************************************************
 * RCV_STATE_TNON functions
 *****************************************************************************/
static void RcvStateTnon_Ent( void )
{
  // get the current system time
  lCurTimeUsec = ( S32 )SystemTick_GetTimeUsec( );
}

static U8 RcvStateTnon_Exc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;
  S32 lCurTimeUsec;

  // check for argument
  if ( xArg == MORSECODECODEC_TONE_OFF_EVENT )
  {
    // get the current time
    lCurTimeUsec = SystemTick_GetTimeUsec( );

    // increment the code length/shift the code left
    tCurRcvCode.nLength++;
    tCurRcvCode.nCode <<= 1;

    // now determine dash/dot
    tCurRcvCode.nCode |= ( abs( lCurTimeUsec ) > ( lRcvDotTimeUsec << 1 )) ? SYMCODE_DASH : SYMCODE_DOT;

    // determine the last symbol
    if ( tCurRcvCode.nCode & SYMCODE_DASH )
    {
      // apply a 12/15 multiplier
      lRcvDotTimeUsec = (( lRcvDotTimeUsec * 12 ) * lCurTimeUsec ) / 15;
    }
    else
    {
      // apply a 4/5 multiplier
      lRcvDotTimeUsec = (( lRcvDotTimeUsec * 4 ) * lCurTimeUsec ) / 5;
    }

    // set the next state to tone off
    nNextState = RCV_STATE_TNOFF;
  }
  
  // return the next state
  return( nNextState );
}

/******************************************************************************
 * RCV_STATE_TNOFF functions
 *****************************************************************************/
static void RcvStateTnoff_Ent( void )
{
  // start a timer to determine intercharacter time
  TaskManager_StartTimer( TASK_SCHD_ENUM_MORSECODEXRCV, ( lRcvDotTimeUsec << 2 ));
}

static U8 RcvStateTnoff_Exc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // determine event
  switch( xArg )
  {
    case TASK_TIMEOUT_EVENT :
      // are there any symbols
      if ( tCurRcvCode.nLength != 0 )
      {
        // output the code
        OutputCode( );

        // go to the Inter character delay
        nNextState = RCV_STATE_TNON;
      }
      break;

    case MORSECODECODEC_TONE_ON_EVENT :
      // kill the timer
      TaskManager_StopTimer( TASK_SCHD_ENUM_MORSECODEXRCV );

      // back to the tone on state
      nNextState = RCV_STATE_TNON;
      break;

    default :
      break;
  }

  // return the next state
  return( nNextState );
}

/******************************************************************************
 * RCV_STATE_ICDLY functions
 *****************************************************************************/
static void RcvStateIcdly_Ent( void )
{
  // start a timer to determine intercharacter time
  TaskManager_StartTimer( TASK_SCHD_ENUM_MORSECODEXRCV, ( lRcvDotTimeUsec << 2 ));
}

static U8 RcvStateIcdly_Exc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // determine event
  switch( xArg )
  {
    case TASK_TIMEOUT_EVENT :
      // clear the current code
      tCurRcvCode.nLength = 0;
      tCurRcvCode.nCode = 0;

      // output the code
      OutputCode( );

      // go back to idle
      nNextState = RCV_STATE_IDLE;
      break;

    case MORSECODECODEC_TONE_ON_EVENT :
      // kill the timer
      TaskManager_StopTimer( TASK_SCHD_ENUM_MORSECODEXRCV );

      // back to the tone on state
      nNextState = RCV_STATE_TNON;
      break;

    default :
      break;
  }

  // return the next state
  return( nNextState );
}

/**@} EOF MorseCodeCodec.c */
