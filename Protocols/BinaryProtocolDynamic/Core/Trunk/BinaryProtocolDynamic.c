/******************************************************************************
 * @file BinaryProtocolDynamic.c
 *
 * @brief Cyber Binary protocol implementation 
 *
 * This file provides the implementation for the Cyber Protocol
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
 * \addtogroup BinaryProtocolDynamic
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "BinaryProtocolDynamic/BinaryProtocolDynamic.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
// special characters
#define	CH_SOH	0x01
#define	CH_STX	0x02
#define	CH_EOT	0x04
#define	CH_DLE	0x10
#define	CH_SPC	0xEF

// enumerations ---------------------------------------------------------------
/// enumerate the states
typedef enum _DECSTATE
{
  DEC_STATE_IDLE = 0,
  DEC_STATE_DSTADR,
  DEC_STATE_SRCADR,
  DEC_STATE_COMMAND,
  DEC_STATE_OPTION1,
  DEC_STATE_OPTION2,
  DEC_STATE_SEQUENCE,
  DEC_STATE_DATA,
  DEC_STATE_CONTROL,
  DEC_STATE_CHKLSB,
  DEC_STATE_MAX
} DECSTATE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  DECSTATE          eDecState;
static  DECSTATE          ePrvState;
static  U8                nTxRxChecksum;
static  BINPROTDYNSTS     eCurStatus;
static  BINPROTDYNCFG     tConfig;
static  U8                nXmtSequence;

// local function prototypes --------------------------------------------------
static  BINPROTDYNSTS ProcessProtocol( U8 nData );
static  BINPROTDYNRSP ParseMessage( void );
static  BOOL          StuffData( U8 nData );
static  void          OutputData( U8 nData, BOOL bDelimit );
static  void          SendMessage( void );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function BinaryProtocolDynamic_Initialize
 *
 * @brief initailization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void BinaryProtocolDynamic_Initialize( void )
{
  // reset the decode state
  eDecState = DEC_STATE_IDLE;  
  tConfig.bMultiDropEnable = FALSE;
  tConfig.bSequenceEnable = FALSE;
  eCurStatus = BINPROTDYN_STS_IDLE;
  
  // clear the transmit sequence
  nXmtSequence = 0;
}

/******************************************************************************
 * @function BinaryProtocolDynamic_ProcessChar
 *
 * @brief process a character
 *
 * This function will process a character and decode the message
 *
 * @param[in]   bMultiDrop      multidrop enable
 * @param[in]   bSequence       sequence enable
 * @param[in]   pnBuffer        pointer to the message buffer
 * @param[in]   wBufSize        buffer size
 *
 * @return      appropriate status
 *
 *****************************************************************************/
void BinaryProtocolDynamic_Configure( PBINPROTDYNCFG ptConfig )
{
  // set the configuration
  memcpy( &tConfig, ptConfig, BINPROTDYNCFG_SIZE );
}

/******************************************************************************
 * @function BinaryProtocolDynamic_ProcessChar
 *
 * @brief process a character
 *
 * This function will process a character and decode the message
 *
 * @param[in]   nChar     character to process
 *
 * @return      appropriate status
 *
 *****************************************************************************/
BINPROTDYNSTS BinaryProtocolDynamic_ProcessChar( U8 nChar )
{
  // process the character
  switch(( eCurStatus = ProcessProtocol( nChar )))
  {
    case BINPROTDYN_STS_MSGCOMP :
      // clear the transmit length
      tConfig.ptMessage->wTxLength = 0;

      // process the response
      switch( ParseMessage( ))
      {
        case BINPROTDYN_RSP_ACK :
        case BINPROTDYN_RSP_ACKDAT :
          // send a message
          tConfig.ptMessage->nOption1 = CH_ACK;
          SendMessage( );
          break;
        
        case BINPROTDYN_RSP_NAK :
          // send a message
          tConfig.ptMessage->nOption1 = CH_NAK;
          SendMessage( );
          break;

        case BINPROTDYN_RSP_NOCMD :
          // send a message
          tConfig.ptMessage->nOption1 = CH_NAK;
          tConfig.ptMessage->nOption2 = 0xFC;
          break;

        default :
          break;
      }
      break;

    case BINPROTDYN_STS_MSGERR :
      tConfig.ptMessage->nOption1 = CH_NAK;
      tConfig.ptMessage->nOption2 = 0xFE;
      tConfig.ptMessage->wTxLength = 0;
      SendMessage( );
      break;

    default :
      break;
  }
  
  // return the status
  return( eCurStatus );
}  
 
/******************************************************************************
 * @function ProcessProtocol
 *
 * @brief process a character
 *
 * This function will process a character and decode the message
 *
 * @param[in]   nChar     character to process
 *
 * @return      appropriate status
 *
 *****************************************************************************/
static BINPROTDYNSTS ProcessProtocol( U8 nData )
{
  BINPROTDYNSTS eLclStatus;
  
  // add the data to the checksum
  nTxRxChecksum += nData;
  
  // check for control only if we are not in idle or control
  if (( eDecState > DEC_STATE_IDLE ) && ( eDecState < DEC_STATE_CONTROL ) && ( nData == CH_DLE ))
  {
    // set the previous state
    ePrvState = eDecState;

    // force control state
    eDecState = DEC_STATE_CONTROL;
  }
  else
  {
    // determine the state
    switch( eDecState )
    {
      case DEC_STATE_IDLE :
        // check for delimiter
        if ( nData == CH_DLE )
        {
          // go to control state
          eDecState = DEC_STATE_CONTROL;

          // set message in progress
          eLclStatus = BINPROTDYN_STS_MSGINPROG;  
        }
        break;
      
      case DEC_STATE_DSTADR :
          // store the address/move to source
          tConfig.ptMessage->nDstAdr = nData;
          eDecState = DEC_STATE_SRCADR;
        break;
      
      case DEC_STATE_SRCADR :
        // store the address/move to command
        tConfig.ptMessage->nSrcAdr = nData;
        eDecState = DEC_STATE_COMMAND;
        break;
      
      case DEC_STATE_COMMAND :
        // store the command/goto option
        tConfig.ptMessage->nCommand = nData;
        eDecState = DEC_STATE_OPTION1;
        break;
      
      case DEC_STATE_OPTION1 :
        // store the option/goto appropriate state
        tConfig.ptMessage->nOption1 = nData;
        eDecState = DEC_STATE_OPTION2;
        break;
      
      case DEC_STATE_OPTION2 :
        // store the option/goto appropriate state
        tConfig.ptMessage->nOption2 = nData;
        eDecState = ( tConfig.bSequenceEnable ) ? DEC_STATE_SEQUENCE : DEC_STATE_DATA;
        break;
      
      case DEC_STATE_SEQUENCE :
        // store the sequence/goto to data state
        tConfig.ptMessage->nRcvSequence = nData;
        eDecState = DEC_STATE_DATA;
        break;
    
      case DEC_STATE_DATA :
        // stuff the data/back to data
        if ( StuffData( nData ) == TRUE )
        {
          // back to idle clear message in progress
          eDecState = DEC_STATE_IDLE;
          eLclStatus = BINPROTDYN_STS_IDLE;
        }
        break;
      
      case DEC_STATE_CONTROL :
        // now process control characters
        switch( nData )
        {
          case CH_SOH :
            // set the receive checksum
            nTxRxChecksum = CH_DLE + CH_SOH;
          
            // set the decode state 
            eDecState = ( tConfig.bMultiDropEnable ) ? DEC_STATE_DSTADR : DEC_STATE_COMMAND;
          
            // clear the buffer index
            tConfig.ptMessage->wRxLength = 0;
            break;
          
          case CH_STX :
            // goto data state
            eDecState = DEC_STATE_DATA;
            break;
          
          case CH_EOT :
            // goto checksum state
            eDecState = DEC_STATE_CHKLSB;
            break;
          
          case CH_SPC :
            switch( ePrvState )
            {
              case DEC_STATE_DSTADR :
                // set the destination/go to source
                tConfig.ptMessage->nDstAdr= CH_DLE;
                eDecState = DEC_STATE_SRCADR;
                break;

              case DEC_STATE_SRCADR :
                // set the source/goto command
                tConfig.ptMessage->nSrcAdr = CH_DLE;
                eDecState = DEC_STATE_COMMAND;
                break;

              case DEC_STATE_COMMAND :
                // set the command/goto option1
                tConfig.ptMessage->nCommand = CH_DLE;
                eDecState = DEC_STATE_OPTION1;
                break;

              case DEC_STATE_OPTION1 :
                // set the option1/goto to option2
                tConfig.ptMessage->nOption1 = CH_DLE;
                eDecState = DEC_STATE_OPTION2;
                break;

              case DEC_STATE_OPTION2 :
                // set the option2/goto either sequence or data
                tConfig.ptMessage->nOption2 = CH_DLE;
                eDecState = ( tConfig.bSequenceEnable ) ? DEC_STATE_SEQUENCE : DEC_STATE_DATA;
                break;

              case DEC_STATE_SEQUENCE :
                // set the sequence/goto data
                tConfig.ptMessage->nRcvSequence = CH_DLE;
                eDecState == DEC_STATE_DATA;
                break;

              case DEC_STATE_DATA :
                // stuff the data/back to data
                if ( StuffData( CH_DLE ) == FALSE )
                {
                  // no error - goto data state
                  eDecState = DEC_STATE_DATA;
                }
                else
                {
                  // back to idle clear message in progress
                  eDecState = DEC_STATE_IDLE;
                  eLclStatus = BINPROTDYN_STS_IDLE;
                }
                break;

               default :
                // back to idle clear message in progress
                eDecState = DEC_STATE_IDLE;
                eLclStatus = BINPROTDYN_STS_IDLE;
                break;
            }
            break;

          default :
            // back to idle clear message in progress
            eDecState = DEC_STATE_IDLE;
            eLclStatus = BINPROTDYN_STS_IDLE;
            break;
        }
        break;
      
      case DEC_STATE_CHKLSB :
        // twos complement the checksum
        nTxRxChecksum = ~nTxRxChecksum;
        nTxRxChecksum++;

        // check for zero
        eLclStatus = ( nTxRxChecksum == 0 ) ? BINPROTDYN_STS_MSGCOMP : BINPROTDYN_STS_MSGERR;

        // reset state back to idle
        eDecState = DEC_STATE_IDLE;
        break;
      
      default :
        // reset state
        eLclStatus = BINPROTDYN_STS_IDLE;
      
        // should never happen
        eDecState = DEC_STATE_IDLE;
        break;
    }
  }

  // return the status
  return( eLclStatus );
}

/******************************************************************************
 * @function ParseMessage
 *
 * @brief parse the message
 *
 * This function will parse the message, find the entry in the command table
 * and execute it
 *
 * @return  appropriate response
 *
 *****************************************************************************/
static BINPROTDYNRSP ParseMessage( void )
{
  BOOL            bRunFlag = TRUE;
  PBINPROTCMDTBL  ptCmdEntry;
  PVCMDHANDLER    pvCmdHandler;
  BINPROTDYNRSP   eStatus = BINPROTDYN_RSP_NOCMD;
  
  // get a pointer to a command entry
  if (( ptCmdEntry = tConfig.ptCommandTable ) != NULL )
  {
    // search the table
    while( bRunFlag )
    {
      // check for end of table
      if ( ptCmdEntry->nCommand == 0 )
      {
        // exit
        bRunFlag = FALSE;
      }
      else
      {
        // does this match command
        if ( tConfig.ptMessage->nCommand == ptCmdEntry->nCommand )
        {
          // check for a command handler
          if (( pvCmdHandler = ptCmdEntry->pvCmdHandler ) != NULL )
          {
            // execute it
            eStatus = pvCmdHandler( );
            
            // clear the run flag
            bRunFlag = FALSE;
          }
        }
        else
        {
          // increment the pointer
          ptCmdEntry++;
        }
      }
    }
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function StuffData
 *
 * @brief stuff data in the buffer
 *
 * This function will stuff data in the buffer if there is room
 *
 * @param[in]   nData     data to stuff
 *
 * @return      TRUE if buffer full, FALSE if not
 *
 *****************************************************************************/
static BOOL StuffData( U8 nData )
{
  BOOL  bBufferFull = FALSE;
  
  // check for room
  if ( tConfig.ptMessage->wRxLength < tConfig.wBufferSize )
  {
    // stuff the data/incremenet the index
    *( tConfig.pnTxRxBuffer + tConfig.ptMessage->wRxLength++ ) = nData;
  }    
  else
  {
    // set the error status
    bBufferFull = TRUE;
  }    
   
  // return the state
  return( bBufferFull );
}

/******************************************************************************
 * @function SendMessage
 *
 * @brief send message
 *
 * This function will send the message
 *
 *****************************************************************************/
static void SendMessage( void )
{
  U16   wMsgIdx;

  // clear the transmit checksum
  nTxRxChecksum = 0;

  // output the header
  OutputData( CH_DLE, FALSE );
  OutputData( CH_SOH, FALSE );

  // now check for source/destination
  if ( tConfig.bMultiDropEnable )
  {
    OutputData( tConfig.ptMessage->nSrcAdr, TRUE );
    OutputData( tConfig.ptMessage->nDstAdr, TRUE );
  }

  // output the command/option
  OutputData( tConfig.ptMessage->nCommand, TRUE );
  OutputData( tConfig.ptMessage->nOption1, TRUE );
  OutputData( tConfig.ptMessage->nOption2, TRUE );

   // check for sequence
  if ( tConfig.bSequenceEnable )
  {
    // output the sequence number
    OutputData( nXmtSequence++, TRUE );
  }

  // is there data
  if ( tConfig.ptMessage->wTxLength != 0 )
  {
    // stuff the data header
    OutputData( CH_DLE, FALSE );
    OutputData( CH_STX, FALSE );

    // now output the data
    for ( wMsgIdx = 0; wMsgIdx < tConfig.ptMessage->wTxLength; wMsgIdx ++ )
    {
      // output the data
      OutputData( *( tConfig.pnTxRxBuffer + wMsgIdx ), TRUE );
    }
  }

  // output the trailer
  OutputData( CH_DLE, FALSE );
  OutputData( CH_EOT, FALSE );

  // two's complement the checksum
  nTxRxChecksum = ~nTxRxChecksum;
  nTxRxChecksum++;

  // output it
  OutputData( nTxRxChecksum, FALSE );
}

/******************************************************************************
 * @function output data
 *
 * @brief output a byte of data
 *
 * This function will output a byge of data using the write function
 * if the delimit flag is set, it will test for special characters
 *
 * @param[in]   nData     data to send
 * @param[in]   bDelimit  delimit enable flag
 *
 *****************************************************************************/
static void OutputData( U8 nData, BOOL bDelimit )
{
  // output the data/add to checksum
  tConfig.pvWriteFunc( nData );
  nTxRxChecksum += nData;

  // now check for delimit
  if (( bDelimit == TRUE ) && ( nData == CH_DLE ))
  {
    // output a special character/add to checkum
    tConfig.pvWriteFunc( CH_SPC );
    nTxRxChecksum += CH_SPC;
  }
}

/**@} EOF BinaryProtocolDynamic.c */
