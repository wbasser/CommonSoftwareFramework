/******************************************************************************
 * @file MorseCodeCodec.c
 *
 * @brief morse code codec implementation
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
#include <ctype.h>

// local includes -------------------------------------------------------------
#include "MorseCodeCodec/MorseCodeCodec.h"

// library includes -----------------------------------------------------------
#if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
  #include "DebugManager/DebugManager.h"
#endif
#include "FilterAveraging/FilterAveraging.h"

// Macros and Defines ---------------------------------------------------------
// define the length of the table
#define	CODE_TABLE_LENGTH	'Z' - ' ' + 1

// define the code table offset
#define	CODE_TABLE_OFFSET	' '

/// define the special characters
#define MORSECODE_CODEC_CH_SPC                      ( ' ' )
#define MORSECODE_CODEC_CH_ERR                      ( '#' )

/// define the unit time for a wpm
#define WPM_BASE_TIME_MSEC                          ( 1200 )

/// define the element constants
#define DOT_ELEMENT_K                               ( 1 )
#define INTERELEMENT_K                              ( 1 )
#define DASH_ELEMENT_K                              ( 3 )
#define INTERCHAR_K                                 ( 2 )
#define SPACE_ELEMENT_K                             ( 7 )

/// define the buffer size
#define RCVDLEM_AVERAGE_SIZE                        ( 16 )

// enumerations ---------------------------------------------------------------

/// enumerate the low level state for transmit
typedef enum _XMTCTLSTATE
{
  XMT_CTL_STATE_IDLE = 0,
  XMT_CTL_STATE_KEYUP,
  XMT_CTL_STATE_KEYDN,
  XMT_CTL_STATE_MAX
} XMTCTLSTATE;

/// enumerate the receive state
typedef enum _RCVCTLSTATE
{
  RCV_CTL_STATE_KEYUP = 0,
  RCV_CTL_STATE_KEYDN,
  RCV_CTL_STATE_MAX
} RCVCTLSTATE;

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  XMTCTLSTATE       eXmtCtlState;
static  RCVCTLSTATE       eRcvCtlState;
static  MORSEECODECODEDEF tCurXmtCode;
static  MORSEECODECODEDEF tCurRcvCode;
static  U8                nCurXmtCounts;
static  U8                nXmtDotElementCount;
static  U16               wCurRcvDotElementCount;
static  U8                nCurRcvWpm;
static  U16               wTimeBase;
static  FILTAVERAGECTL    tRcvDotFilter;

// declare the buffer
FILTAVERAGESTORAGE( RcvDot, RCVDLEM_AVERAGE_SIZE );

// local function prototypes --------------------------------------------------
static  C8    CodeToAscii( PMORSEECODECODEDEF ptCode, BOOL bEnableSpace );
static  BOOL  AsciiToCode( C8 cChar, PMORSEECODECODEDEF ptCode );
static  U8    GetElementCounts( void );
static  void  OutputRcvCode( PMORSEECODECODEDEF ptCode );

// constant parameter initializations -----------------------------------------
const CODE MORSEECODECODEDEF  atCodes[ CODE_TABLE_LENGTH ] =
{
  { 0xFF, 0xFF },   // 0x20 - no code
  { 0x00, 0x00 },   // 0x21 - no code
  { 0x00, 0x00 },   // 0x22 - no code
  { 0x00, 0x00 },   // 0x23 - no code
  { 0x07, 0x09 },   // 0x24 - $
  { 0x00, 0x00 },   // 0x25 - no code
  { 0x00, 0x00 },   // 0x26 - no code
  { 0x06, 0x33 },   // 0x27 - '
  { 0x00, 0x00 },   // 0x28 - no code
  { 0x00, 0x00 },   // 0x29 - no code
  { 0x06, 0x05 },   // 0x2A - *
  { 0x05, 0x0A },   // 0x2B - +
  { 0x06, 0x1E },   // 0x2C - ,
  { 0x05, 0x11 },   // 0x2D - -
  { 0x06, 0x38 },   // 0x2E - .
  { 0x00, 0x00 },   // 0x2F - no code
  { 0x05, 0x1F },   // 0x30 - 0
  { 0x05, 0x0F },   // 0x31 - 1
  { 0x05, 0x07 },   // 0x32 - 2
  { 0x05, 0x03 },   // 0x33 - 3
  { 0x05, 0x01 },   // 0x34 - 4
  { 0x05, 0x00 },   // 0x35 - 5
  { 0x05, 0x10 },   // 0x36 - 6
  { 0x05, 0x18 },   // 0x37 - 7
  { 0x05, 0x1C },   // 0x38 - 8
  { 0x05, 0x1E },   // 0x39 - 9
  { 0x06, 0x0C },   // 0x3A - :
  { 0x00, 0x00 },   // 0x3B - no code
  { 0x00, 0x00 },   // 0x3C - no code
  { 0x00, 0x00 },   // 0x3D - no code
  { 0x00, 0x00 },   // 0x3E - no code
  { 0x05, 0x01 },   // 0x3F - ?
  { 0x00, 0x00 },   // 0x40 - no code
  { 0x02, 0x01 },   // 0x41 - A
  { 0x04, 0x08 },   // 0x42 - B
  { 0x04, 0x0A },   // 0x43 - C
  { 0x03, 0x04 },   // 0x44 - D
  { 0x01, 0x00 },   // 0x45 - E 
  { 0x04, 0x02 },   // 0x46 - F
  { 0x03, 0x06 },   // 0x47 - G
  { 0x04, 0x00 },   // 0x48 - H  
  { 0x02, 0x00 },   // 0x49 - I
  { 0x04, 0x07 },   // 0x4A - J
  { 0x03, 0x05 },   // 0x4B - K
  { 0x04, 0x04 },   // 0x4C - L
  { 0x02, 0x03 },   // 0x4D - M
  { 0x02, 0x02 },   // 0x4E - N
  { 0x03, 0x07 },   // 0x4F - O
  { 0x04, 0x06 },   // 0x50 - P
  { 0x04, 0x0D },   // 0x51 - Q
  { 0x03, 0x02 },   // 0x52 - R
  { 0x03, 0x00 },   // 0x53 - S
  { 0x01, 0x01 },   // 0x54 - T
  { 0x03, 0x01 },   // 0x55 - U
  { 0x04, 0x01 },   // 0x56 - V
  { 0x03, 0x03 },   // 0x57 - W
  { 0x04, 0x09 },   // 0x58 - X
  { 0x04, 0x0B },   // 0x59 - Y
  { 0x04, 0x0C },   // 0x5A - Z
};

/******************************************************************************
 * @function MorseCodeCodec_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL MorseCodeCodec_Initialize( void )
{
  // call the local initialization
  MorseCodeCodec_LocalInitialize( );

  // set the xmit top level state to idle
  eXmtCtlState = XMT_CTL_STATE_IDLE;
  eRcvCtlState = RCV_CTL_STATE_KEYUP;

  // clear the current receive character
  tCurRcvCode.wValue = 0;

  // create the filter control
  tRcvDotFilter.tDef.pxBuffer = axRcvDotAvgFilt;
  tRcvDotFilter.tDef.nNumberSamples = RCVDLEM_AVERAGE_SIZE;
  FilterAveraging_Initialize( &tRcvDotFilter );

  // call the local config
  MorseCodeCodec_Config( TRUE );

  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function MorseCodeCodec_Config
 *
 * @brief configuration
 *
 * @param[in]   bState      run state
 *
 * This function will configure the parameters
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
void MorseCodeCodec_Config( BOOL bState )
{
  U8        nXmtWpm, nIdx;
  S32       lTimeBase, lDotTimeUsec;

  // if we are turning on
  if ( ON == bState )
  {
    // get the desired WPM time
    nCurRcvWpm = nXmtWpm = MorseCodeCodec_GetXmtWpm( );

    // calculate the element time
    nXmtDotElementCount = WPM_BASE_TIME_MSEC / nXmtWpm / MORSECODECODEC_XMT_EXEC_MSECS;

    // calculate the receiv element time
    lDotTimeUsec = ( WPM_BASE_TIME_MSEC / nXmtWpm ) * 1000;
    lTimeBase = MorseCodeCodec_GetRecvTimerRate( );
    wTimeBase = 1000000 / lTimeBase;
    wCurRcvDotElementCount = lDotTimeUsec / wTimeBase;
    #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
      DebugManager_AddElement( 0xBFFF, wCurRcvDotElementCount );
    #endif

    // fill the average buffer
    for ( nIdx = 0; nIdx < RCVDLEM_AVERAGE_SIZE + 1; nIdx++ )
    {
      // add the default
      FilterAveraging_Execute( &tRcvDotFilter, wCurRcvDotElementCount );
    }
  }
}

/*****************************************************************************
 * @function MorseCodeCodec_GetRcvWpm
 *
 * @brief get the word per minute
 *
 * This function will return the current receive WPM
 *
 * @return      receive WPM
 *
 *****************************************************************************/
U8 MorseCodeCodec_GetRcvWpm( void )
{
  // return the WPM
  return( nCurRcvWpm );
}

/******************************************************************************
 * @function MorseCodeCodec_XmitChar
 *
 * @brief transmit a character
 *
 * This function will translate the character into a valid morse code entry
 * and post it to the transmit queue
 *
 * @param[in]   cChar   character to transmit
 *
 *****************************************************************************/
void MorseCodeCodec_XmitChar( C8 cChar )
{
  MORSEECODECODEDEF tCode;

  // translate code
  if ( TRUE == AsciiToCode( cChar, &tCode ))
  {
    // post code to the queue
    MorseCodeCodec_PutTransmitQueue( tCode );
  }
}

/******************************************************************************
 * @function MorseCodeCodec_ProcessXmtCtl
 *
 * @brief xmit task low level task
 *
 * This function will process events for the low level task
 *
 *****************************************************************************/
void MorseCodeCodec_ProcessXmtCtl( void )
{
  // check for time remaining
  if ( nCurXmtCounts != 0 )
  {
    // decrement count
    nCurXmtCounts--;
  }
  else
  {
    // determine the state
    switch( eXmtCtlState )
    {
      case XMT_CTL_STATE_IDLE :
        // check for an entry in the queue
        if ( TRUE == MorseCodeCodec_GetXmitOutQueue( &tCurXmtCode ))
        {
          #if ( MORSECODECODEC_ENABLE_TXDEBUG == ON )
            DebugManager_AddElement( 0xC800 + eXmtCtlState, 0 );
          #endif
          // go to keyup
          eXmtCtlState = XMT_CTL_STATE_KEYUP;

          // echo it
          MorseCodeCodec_OutputRcvChar( CodeToAscii( &tCurXmtCode, TRUE ));
        }
        break;
  
      case XMT_CTL_STATE_KEYUP :
        // check for a space
        if ( 0xFFFF == tCurXmtCode.wValue )
        {
          // set timeout to inter word time
          nCurXmtCounts = nXmtDotElementCount * SPACE_ELEMENT_K;
          nCurXmtCounts--;

          // clear the length
          tCurXmtCode.tFields.nLength = 0;

          // back to idle
          eXmtCtlState = XMT_CTL_STATE_IDLE;
          #if ( MORSECODECODEC_ENABLE_TXDEBUG == ON )
            DebugManager_AddElement( 0xC900, nCurXmtCounts );
          #endif
        }
        else
        {
          // check for reminaing elements
          if ( 0 != tCurXmtCode.tFields.nLength )
          {
            // decrement the length
            tCurXmtCode.tFields.nLength--;

            // get the element time for the next element
            nCurXmtCounts = GetElementCounts( );

            // turn on the output
            MorseCodeCodec_OutputControl( ON );

            // go to key down
            eXmtCtlState = XMT_CTL_STATE_KEYDN;
            #if ( MORSECODECODEC_ENABLE_TXDEBUG == ON )
              DebugManager_AddElement( 0xCA00, nCurXmtCounts );
            #endif
          }
          else
          {
            // set the counts to inter character
            nCurXmtCounts = nXmtDotElementCount * INTERCHAR_K;
            nCurXmtCounts -= 2;

            // go to idle
            eXmtCtlState = XMT_CTL_STATE_IDLE;
          }
          #if ( MORSECODECODEC_ENABLE_TXDEBUG == ON )
            DebugManager_AddElement( 0xCB00, nCurXmtCounts );
          #endif
        }
        break;

      case XMT_CTL_STATE_KEYDN :
        #if ( MORSECODECODEC_ENABLE_TXDEBUG == ON )
          DebugManager_AddElement( 0xCC00, nCurXmtCounts );
        #endif
        // turn on the output
        MorseCodeCodec_OutputControl( OFF );

        // set the time for inter element tim
        nCurXmtCounts = nXmtDotElementCount * DOT_ELEMENT_K;
        nCurXmtCounts--;

        // go to key down
        eXmtCtlState = XMT_CTL_STATE_KEYUP;
        #if ( MORSECODECODEC_ENABLE_TXDEBUG == ON )
          DebugManager_AddElement( 0xCC00, nCurXmtCounts );
        #endif
        break;

      default :
        // should never happen - reset to idle
        eXmtCtlState = XMT_CTL_STATE_KEYUP;
        break;
    }
  }
}

/******************************************************************************
 * @function MorseCodeCodec_ProcessRcvCtl
 *
 * @brief xmit task low level task
 *
 * This function will process events for the low level task
 *
 * @param[in]   xEvent    event to handle
 *
 *****************************************************************************/
void MorseCodeCodec_ProcessRcvCtl( MORSECODE_CODEC_TASKARG xEvent )
{
  MORSECODECODECDATA  tData;
  U8                  nBit;
  U32                 uTimeoutMsec;
  FLOAT               fTemp;

  #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
    DebugManager_AddElement( 0xC000 + eRcvCtlState, xEvent );
  #endif

  // determine the current state
  switch( eRcvCtlState )
  {
    case RCV_CTL_STATE_KEYUP :
      // process the event
      switch( xEvent )
      {
        case MORSECODECODEC_TIMEOUT_EVENT :
          // timeout - output the character if the previous character had length
          if ( tCurRcvCode.tFields.nLength != 0 )
          {
            // output the character
            OutputRcvCode( &tCurRcvCode );

            uTimeoutMsec = WPM_BASE_TIME_MSEC / nCurRcvWpm;
            uTimeoutMsec *= 5;
            MorseCodeCodec_RcvTimerControl( uTimeoutMsec );
          }
          else
          {
            // output a space
            tCurRcvCode.wValue = 0;
            OutputRcvCode( &tCurRcvCode );
          }
          #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
            DebugManager_AddElement( 0xC600, tCurRcvCode.wValue );
          #endif

          // clear the previous code
          tCurRcvCode.wValue = 0;
          break;

        case RCVTASK_CTL_QUEUE_EVENT :
          // kill the timer
          MorseCodeCodec_RcvTimerControl( 0 );

          // get the queue entry
          MorseCodeCodec_GetRcvInpQueue( &tData );

          #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
            DebugManager_AddElement( 0xC100 | tData.eEdge, tData.wValue );
          #endif

          // only allow key down events
          if ( MORSECODE_EDGE_KEYDN == tData.eEdge )
          {
            // go to key down
            eRcvCtlState = RCV_CTL_STATE_KEYDN;

            // check for inter-character space
            if (( tCurRcvCode.tFields.nLength != 0 ) && ( tData.wValue >= ( wCurRcvDotElementCount * 1.75 )))
            {
              // output the character
              OutputRcvCode( &tCurRcvCode );

              #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
                DebugManager_AddElement( 0xC600, tCurRcvCode.wValue );
              #endif

              // clear the previous code
              tCurRcvCode.wValue = 0;
            }
          }
          break;

        default :
          break;
      }
      break;

    case RCV_CTL_STATE_KEYDN :
        // get the queue entry
        MorseCodeCodec_GetRcvInpQueue( &tData );

        #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
          DebugManager_AddElement( 0xC100 | tData.eEdge, tData.wValue );
        #endif

        // only allow key down events
        if ( MORSECODE_EDGE_KEYUP == tData.eEdge )
        {
          // shift the code
          tCurRcvCode.tFields.nCode <<= 1;

          // deteremine dash/dot
          if ( tData.wValue > ( wCurRcvDotElementCount << 1 ))
          {
            // dash element
            tCurRcvCode.tFields.nCode |= 0x01;

            // adjust the value
            tData.wValue /= DASH_ELEMENT_K;
          }

          // add to the filter
          FilterAveraging_Execute( &tRcvDotFilter, tData.wValue );
          wCurRcvDotElementCount =  FilterAveraging_GetValue( &tRcvDotFilter );
        
          // increment the length
          tCurRcvCode.tFields.nLength++;
        
          #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
            DebugManager_AddElement( 0xC200, wCurRcvDotElementCount );
          #endif

          #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
            DebugManager_AddElement( 0xC300, tCurRcvCode.wValue );
          #endif
        
          // recalculate the WPM
          fTemp = (( FLOAT )WPM_BASE_TIME_MSEC * 1000.0 ) / (( FLOAT )wCurRcvDotElementCount * ( FLOAT )wTimeBase );
          nCurRcvWpm = ( U8 )( fTemp + 0.5 );
          #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
            DebugManager_AddElement( 0xC400, nCurRcvWpm );
          #endif

          // start a timer for timeout
          uTimeoutMsec = ( WPM_BASE_TIME_MSEC / nCurRcvWpm ) * SPACE_ELEMENT_K;
          MorseCodeCodec_RcvTimerControl( uTimeoutMsec );
          #if ( MORSECODECODEC_ENABLE_RXDEBUG == ON )
            DebugManager_AddElement( 0xC500, uTimeoutMsec );
          #endif
        }

        // go to key down
        eRcvCtlState = RCV_CTL_STATE_KEYUP;
      break;

    default :
      // reset back to key up
      eRcvCtlState = RCV_CTL_STATE_KEYUP;
      break;
  }
}

/******************************************************************************
 * @function CodeToAscii
 *
 * @brief convert a code to an ASCII character
 *
 * This function will attempt to find a matching code entry in the table
 * and returns it equivalent ASCII character
 *
 * @param[in]   ptCode        pointer to the code entry
 * @param[in]   bEnableSpace  enable space
 *
 * @return      Character  space if code length is zero, # if invalid entry
 *
 *****************************************************************************/
static C8 CodeToAscii( PMORSEECODECODEDEF ptCode, BOOL bEnableSpace )
{
  C8          cChar = MORSECODE_CODEC_CH_SPC;
  U8          nIndex;
  PMORSEECODECODEDEF  ptEntry;

  // check for zero length
  if( ptCode->tFields.nLength != 0 )
  {
    // now search the table for an entry
    for ( nIndex = 0; nIndex < CODE_TABLE_LENGTH; nIndex++ )
    {
      // get a pointer to the table
      ptEntry = ( PMORSEECODECODEDEF )&atCodes[ nIndex ];

      // if length and code match
      if ( ptCode->wValue == ptEntry->wValue )
      {
        // character found
        cChar = nIndex + CODE_TABLE_OFFSET;

        // exit search loop
        break;
      }
    }

    // test for character not found
    if (( cChar == MORSECODE_CODEC_CH_SPC ) && ( FALSE == bEnableSpace ))
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
static BOOL AsciiToCode( C8 cChar, PMORSEECODECODEDEF ptCode )
{
  BOOL  bResult = FALSE;

  // make it upper case
  cChar = toupper( cChar );

  // adjust for table offset
  cChar -= CODE_TABLE_OFFSET;

  // check if character within range
  if ( cChar < CODE_TABLE_LENGTH )
  {
    // copy the code entry
    MEMCPY_P( ptCode, &atCodes[ cChar ], MORSEECODECODEDEF_SIZE );

    // check for zero
    if ( 0 != ptCode->wValue )
    {
      // set good flag
      bResult = TRUE;
    }
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
static U8 GetElementCounts( void )
{
  U8  nElementCounts;
  U8  nMask;

  // get the element time
  nMask = BIT( tCurXmtCode.tFields.nLength );
  nElementCounts = ( nMask & tCurXmtCode.tFields.nCode ) ? DASH_ELEMENT_K : DOT_ELEMENT_K;
  nElementCounts *= nXmtDotElementCount;
  nElementCounts--;

  // return the element time
  return( nElementCounts );
}

/******************************************************************************
 * @function OutputRcvCode
 *
 * @brief output a receive code
 *
 * This function will translate the code and output the data
 *
 * @param[in]   ptCode    pointer to the current received code
 *
 *****************************************************************************/
static void OutputRcvCode( PMORSEECODECODEDEF ptCode )
{
  C8  cChar;

  // translate the code
  cChar = CodeToAscii( ptCode, FALSE );

  // output the received character
  MorseCodeCodec_OutputRcvChar( cChar );
}

/**@} EOF MorseCodeCodec.c */
