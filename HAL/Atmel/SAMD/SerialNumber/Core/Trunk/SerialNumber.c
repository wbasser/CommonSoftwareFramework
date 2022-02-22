/******************************************************************************
 * @file SerialNumber.c
 *
 * @brief serial number implementation
 *
 * This file provides the implementation for the serial number
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
 * \addtogroup SerialNumber
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SerialNumber/SerialNumber.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the serial number address address
#define SERNUM_WORD_ADDR0                             ( 0x0080A00C )
#define SERNUM_WORD_ADDR1                             ( 0x0080A040 )
#define SERNUM_WORD_ADDR2                             ( 0x0080A044 )
#define SERNUM_WORD_ADDR3                             ( 0x0080A048 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
static  const C8  szMsgGuidFmt[ ]   = { "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X" };

/******************************************************************************
 * @function SerialNumber_Get
 *
 * @brief get the serial number
 *
 * This function will get the serial number
 *
 * @param[io]   ptSerNum    pointer to the serial number
 *
 *****************************************************************************/
void SerialNumber_Get( PSERIALNUMBER ptSerNum )
{
  // get the words
  ptSerNum->tWords.tWord0.uValue = *(( PU32 )( U32 )( SERNUM_WORD_ADDR0 ));
  ptSerNum->tWords.tWord1.uValue = *(( PU32 )( U32 )( SERNUM_WORD_ADDR1 ));
  ptSerNum->tWords.tWord2.uValue = *(( PU32 )( U32 )( SERNUM_WORD_ADDR2 ));
  ptSerNum->tWords.tWord3.uValue = *(( PU32 )( U32 )( SERNUM_WORD_ADDR3 ));
};

/******************************************************************************
 * @function SerialNumber_FormatGuid
 *
 * @brief generated a formatted GUID
 *
 * This function will generate a formatted GUID
 *
 * @param[io]   pnBUffer      pointer to the buffer
 * @param[in]   nBUfLength    buffer length
 *
 *****************************************************************************/
BOOL SerialNumber_FormatGuid( PU8 pnBuffer, U8 nBufLength )
{
  BOOL          bOkay = FALSE;
  SERIALNUMBER  tSerNum;

  // get the serial number
  SerialNumber_Get( &tSerNum );
  
  // check for room
  if ( nBufLength >= SERNUM_GUID_LENGTH )
  {
    // print it
    sprintf( pnBuffer, szMsgGuidFmt,
      tSerNum.anBytes[ 0 ], tSerNum.anBytes[ 1 ], tSerNum.anBytes[ 2 ], tSerNum.anBytes[ 3 ],
      tSerNum.anBytes[ 4 ], tSerNum.anBytes[ 5 ], tSerNum.anBytes[ 6 ], tSerNum.anBytes[ 7 ],
      tSerNum.anBytes[ 8 ], tSerNum.anBytes[ 9 ], tSerNum.anBytes[ 10 ], tSerNum.anBytes[ 11 ], 
      tSerNum.anBytes[ 12 ], tSerNum.anBytes[ 13 ], tSerNum.anBytes[ 14 ], tSerNum.anBytes[ 15 ] );
      
    // set status to true
    bOkay = TRUE;
  }
  
  // return the status
  return( bOkay );
};

/**@} EOF SerialNumber.c */
