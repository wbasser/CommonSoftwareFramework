/******************************************************************************
 * @file AgilionTagComm.c
 *
 * @brief Agilion Tag Communications Protocol implementation
 *
 * This file provides the implementation for the Agilion Communications Protocol
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
 * \addtogroup AgilionTagComm
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AgilionTagComm/AgilionTagComm.h"

// library includes -----------------------------------------------------------
//#include "DbgAscCmdHandler/DbgAscCmdHandler.h"
#include "StateExecutionEngine/StateExecutionEngine.h"

// Macros and Defines ---------------------------------------------------------
/// define the start delimiter
#define START_DELIMITER                         ( 0x7F )

/// define the end delimiter
#define END_DELIMITER                           ( 0x8F )

/// define the escape delimiter
#define ESCAPE_DELIMITER                        ( 0x1B )

/// define the escape MASK
#define ESCAPE_MASK                             ( 0x20 )

// enumerations ---------------------------------------------------------------
/// enumerate the decode states
typedef enum _DECSATE
{
  DEC_STATE_IDLE  = 0,
  DEC_STATE_MSGID,
  DEC_STATE_DATA,
  DEC_STATE_ESCAPE,
  DEC_STATE_MAX
} DECSATE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8                  nXmtChecksum;
static  U8                  nRcvChecksum;
static  U16                 wXmtBufferIdx;
static  U16                 wRcvBufferIdx;
static  STATEEXECENGCONTROL tStateCtl;
static  PAGILIONMESSAGES    ptLclRcvMsg;
static  AGILIONSTS          eCurDecStatus;

// local function prototypes --------------------------------------------------
static  BOOL  StuffData( U8 nData, PU8 pnBuffer, U16 wMaxLength );

// state function
/// DEC_STATE_IDLE functions
static  void  DecStateIdleExt( void );

/// DEC_STATE_MSGID funtions
static  U8    DecStateMsgidExc( STATEEXECENGARG xArg );

/// DEC_STATE_DATA functions
static  U8    DecStateDataExc( STATEEXECENGARG xArg );

/// DEC_STATE_ESCAPE functions
static  U8    DecStateEscapeExc( STATEEXECENGARG xArg );

// constant parameter initializations -----------------------------------------
/// initialize the idle events
static  const CODE STATEEXECENGEVENT  atIdleEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( START_DELIMITER, DEC_STATE_MSGID, TRUE ),
  STATEEXECENGEVENT_END( )
};

/// initialize the any state events
static  const CODE STATEEXECENGEVENT  atAnyEvents[ ] =
{
  STATEEXECENGEVENT_ENTRY( ESCAPE_DELIMITER, DEC_STATE_ESCAPE, FALSE ),
  STATEEXECENGEVENT_END( )
};

/// initialize the main state table
static  const CODE STATEEXECENGTABLE  atDecStates[ DEC_STATE_MAX ] =
{
  STATEXECENGETABLE_ENTRY( DEC_STATE_IDLE,   NULL,           NULL,              DecStateIdleExt, atIdleEvents ),
  STATEXECENGETABLE_ENTRY( DEC_STATE_MSGID,  NULL,           DecStateMsgidExc,  NULL,            atAnyEvents  ),
  STATEXECENGETABLE_ENTRY( DEC_STATE_DATA,   NULL,           DecStateDataExc,   NULL,            atAnyEvents  ),
  STATEXECENGETABLE_ENTRY( DEC_STATE_ESCAPE, NULL,           DecStateEscapeExc, NULL,            atAnyEvents  ),
};

/******************************************************************************
 * @function AgilionTagComm_Initialize
 *
 * @brief initializtion
 *
 * This function will perofrm any needed initilization
 *
 *****************************************************************************/
void AgilionTagComm_Initialize( void )
{
  // set the state engine state/initialize it
  tStateCtl.ptStates = atDecStates;
  StateExecutionEngine_Initialize( &tStateCtl, DEC_STATE_IDLE );
}

/******************************************************************************
 * @function AgilionTagComm_ProcessChar
 *
 * @brief process an incoming character
 *
 * This function will process an incoming character and report the current
 * state of the decode
 *
 * @param[io]   ptRcvMessage  pointer to the receive message structure
 * @param[in]   nChar         incoming character
 *
 * @return      approrpiate status
 *
 *****************************************************************************/
AGILIONSTS AgilionTagComm_ProcessChar( PAGILIONMESSAGES ptRcvMessage, U8 nChar )
{
  //DbgAscCmdHandler_PutHexByte( nChar );
  
  // set the current status
  eCurDecStatus = AGILION_STS_NONE;
  
  // set the pointer
  ptLclRcvMsg = ptRcvMessage;
  
  // process the event through the state engine
  StateExecutionEngine_Process( &tStateCtl, nChar );
  
  // return the status
  return( eCurDecStatus );  
}

/******************************************************************************
 * @function AgilionTagComm_FormatBuffer
 *
 * @brief format a message
 *
 * This function will format a message in the passed transmit buffer pointer
 *
 * @param[in]   ptMessage     pointer to the data to be sent
 * @param[io]   pnXmitBuffer  pointer to the transmit buffer
 * @param[in]   wBufLength    maximum length of the transmit buffer
 * @param[in]   wMsgLength    length of the message content
 *
 * @return      length of the transmit buffer
 *
 *****************************************************************************/
U16 AgilionTagComm_FormatBuffer( PAGILIONMESSAGES ptMessage, PU8 pnXmitBuffer, U16 wBufLength, U16 wMsgLength )
{
  U16   wIdx, wDataLength;
  BOOL  bError = FALSE;
  
  // clear the buffer index
  wXmtBufferIdx = 0;
  
  // clear the transmit checksum
  nXmtChecksum = 0;
  
  // stuff the start character
  *( pnXmitBuffer + wXmtBufferIdx++ ) = START_DELIMITER; 
  
  // add the message ID
  StuffData( ptMessage->eMsgId, pnXmitBuffer, wBufLength );
  
  // now compute the length
  switch( ptMessage->eMsgId )
  {
    case AGILION_MSGID_FREEFALLSET :
    case AGILION_MSGID_ACCELSET :
      wDataLength = AGILIONMSGFFACLT_SIZE;
      break;
      
    case AGILION_MSGID_UICTOKYRA :
      wDataLength = wMsgLength;
      break;
      
    default :
      wDataLength = 0;
      break;
  }
  
  // now copy the data
  for ( wIdx = 0; wIdx < wDataLength; wIdx++ )
  {
    if (( bError = StuffData( ptMessage->tContent.anByte[ wIdx ], pnXmitBuffer, wBufLength )) == TRUE )
    {
      // set data length to 0
      wXmtBufferIdx = 0;
      
      // break out of loop
      break;
    }
  }
  
  // if no error
  if ( !bError )
  {
    // two's complement the checksum
    nXmtChecksum = ~nXmtChecksum;
    
    // stuff it
    *( pnXmitBuffer + wXmtBufferIdx++ ) = nXmtChecksum;
    
    // stuff the end delimiter
    *( pnXmitBuffer + wXmtBufferIdx++ ) = END_DELIMITER;
  }
  
  // return the buffer length
  return( wXmtBufferIdx );
}

static BOOL StuffData( U8 nData, PU8 pnBuffer, U16 wMaxLength )
{
  BOOL  bError = FALSE;
  
  // check for valid length
  if ( wXmtBufferIdx < wMaxLength - 6 )
  {

    // add to checksum
    nXmtChecksum += nData;

    // check for delimiter
    if (( nData == START_DELIMITER ) || ( nData == END_DELIMITER ) || ( nData == ESCAPE_DELIMITER ))
    {
      // stuff the escape
      *( pnBuffer + wXmtBufferIdx++ ) = ESCAPE_DELIMITER;
      *( pnBuffer + wXmtBufferIdx++ ) = nData ^ ESCAPE_MASK;
    }
    else
    {
      // stuff in buffer
      *( pnBuffer + wXmtBufferIdx++ ) = nData;
    }   
  }
  else
  {
    // return error
    bError = TRUE;
  }
  
  // return the status
  return( bError );
}  

// state function
/// DEC_STATE_IDLE functions
static void DecStateIdleExt( void )
{
  // clear the receive checksum/buffer index
  nRcvChecksum = 0;
  wRcvBufferIdx = 0;
  
  // set message in progress
  eCurDecStatus = AGILION_STS_DECINPRG;
}

/// DEC_STATE_MSGID funtions
static U8 DecStateMsgidExc( STATEEXECENGARG xArg )
{
  // store the mesage id
  ptLclRcvMsg->eMsgId = ( AGILIONMSGID )xArg;
  nRcvChecksum += ( U8 )xArg;
  
  // return the next state
  return( DEC_STATE_DATA );
}

static U8 DecStateDataExc( STATEEXECENGARG xArg )
{
  U8  nNextState = STATEEXECENG_STATE_NONE;

  // check for the end delimiter
  if ( xArg == END_DELIMITER )
  {
    // compare checksum
    eCurDecStatus = ( nRcvChecksum == 0xFF ) ? AGILION_STS_VALIDMSG : AGILION_STS_CHECKERR;
    
    // always go to idle
    nNextState = DEC_STATE_IDLE;
  }
  else
  {
    // add data to buffer.checksum
    ptLclRcvMsg->tContent.anByte[ wRcvBufferIdx++ ] = ( U8 )xArg;
    nRcvChecksum += ( U8 )xArg;
  }
  
  // go to the previous state
  return( nNextState );
}

/// DEC_STATE_ESCAPE functions
static U8 DecStateEscapeExc( STATEEXECENGARG xArg )
{
  U8  nData;
  U8  nNextState = STATEEXECENG_STATE_NONE;
  
  // create the data
  nData = ( U8 )( xArg ^ ESCAPE_MASK );
  nRcvChecksum += nData;
  
  switch( tStateCtl.nLastState )
  {
    case DEC_STATE_MSGID :
      ptLclRcvMsg->eMsgId = ( AGILIONMSGID )nData;
      nNextState = DEC_STATE_DATA;
      break;
      
    case DEC_STATE_DATA :
      // apply the mask/stuff in buffer
      ptLclRcvMsg->tContent.anByte[ wRcvBufferIdx++ ] = nData;
      nNextState = DEC_STATE_DATA;
      break;
      
    default :
      // protocol error
      nNextState = DEC_STATE_IDLE;
      
      // set message back to none
      eCurDecStatus = AGILION_STS_NONE;
      break;
  }
  
  // go to the previous state
  return( nNextState );
}

/**@} EOF AgilionTagComm.c */
