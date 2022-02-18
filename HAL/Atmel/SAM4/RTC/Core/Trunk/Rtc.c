/******************************************************************************
 * @file Rtc.c
 *
 * @brief Real Time Clock implementation
 *
 * This file the interface implementation to the Real Time Clock Peripheral
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup RTC
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Rtc/Rtc.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PVRTCALARMCB  pvAlarmCallback;
static  BOOL          bRecurringAlarm;
static  BOOL          bValid;
static  VBOOL         bTimeSetPending;
static  U32           uCurTimeSet;
static  U32           uCurDateSet;

// local function prototypes --------------------------------------------------
static  U8  HexToBcd( U8 nHexValue );
static  U8  BcdToHex( U8 nBcdVal );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function RTC_Initialize
 *
 * @brief  RTC Initialization
 *
 * This function will initialize the Real Time Clock (RTC) in calender mode. 
 *
 * @return  TRUE if errors, FALSE otherwise
 *
 *****************************************************************************/
BOOL Rtc_Initialize( void )
{
  // clear the valid flag
  bValid = bTimeSetPending = FALSE;

  // clear the callback
  pvAlarmCallback = NULL;

  // enable the second periodic event interrupt
  REG_RTC_IER = RTC_IER_SECEN;

  // enable the interrupt
  NVIC_EnableIRQ( ID_RTC );
}

/******************************************************************************
 * @function RTC_Close
 *
 * @brief  RTC close
 *
 * This function will close the RTC and disable all interrupts
 *
 *****************************************************************************/
void Rtc_Close( void )
{
}

/******************************************************************************
 * @function RTC_SetAlarm
 *
 * @brief  RTC Initialization
 *
 * This function will initialize the Real Time Clock (RTC) in calender mode. 
 *
 * @param[in]   ptAlarmTime   alarm time
 * @param[in]   eALarmType    alarm type
 * @param[in]   bRecurring    true if recurring alarm
 * @param[in]   pvCallback    pointer to the callback
 *
 *****************************************************************************/
void Rtc_SetAlarm( PDATETIME ptAlarmTime, RTCALMTYPE eAlarmType, BOOL bRecurring, PVRTCALARMCB pvCallback )
{
  // copy the callback/alarm type/recurring
  pvAlarmCallback = pvCallback;
  bRecurringAlarm = bRecurring;
}

/******************************************************************************
 * @function Rtc_SetDateTime
 *
 * @brief RTC set the date and time
 *
 * This function will set the date and time in the RTC Mode 2 calender register
 *
 * @param[in] ptDateTime
 *
 *****************************************************************************/
void Rtc_SetDateTime( PDATETIME ptDateTime )
{
  // create the current time
  uCurTimeSet = RTC_TIMR_SEC( HexToBcd( ptDateTime->nSeconds ));
  uCurTimeSet |= RTC_TIMR_MIN( HexToBcd( ptDateTime->nMinutes ));
  uCurTimeSet |= RTC_TIMR_HOUR( HexToBcd( ptDateTime->nHours ));

  // create the current date
  uCurDateSet = RTC_CALR_DATE( HexToBcd( ptDateTime->nDay ));
  uCurDateSet |= RTC_CALR_MONTH( HexToBcd( ptDateTime->nMonth ));
  uCurDateSet |= RTC_CALR_YEAR( HexToBcd( ptDateTime->wYear % 100 ));
  uCurDateSet |= RTC_CALR_CENT( HexToBcd( ptDateTime->wYear / 100 ));

  // request a time set
  bTimeSetPending = TRUE;
}

/******************************************************************************
 * @function Rtc_GetDateTime
 *
 * @brief RTC Get Date and Time
 *
 * This function gets the current date and time stored in the RTC Clock register
 *
 * @param[in]   ptDateTime
 *
 * @return      
 *
 *****************************************************************************/
void Rtc_GetDateTime( PDATETIME ptDateTime )
{
  U32 uRegRd1, uRegRd2;

  // ensure valid time read twice
  do
  {
    uRegRd1 = REG_RTC_TIMR;
    uRegRd2 = REG_RTC_TIMR;
  } while( uRegRd1 != uRegRd2 );

  // now parse the values
  ptDateTime->nSeconds = BcdToHex(( RTC_TIMR_SEC_Msk & uRegRd1 ) >> RTC_TIMR_SEC_Pos );
  ptDateTime->nMinutes = BcdToHex(( RTC_TIMR_MIN_Msk & uRegRd1 ) >> RTC_TIMR_MIN_Pos );
  ptDateTime->nHours = BcdToHex(( RTC_TIMR_HOUR_Msk & uRegRd1 ) >> RTC_TIMR_HOUR_Pos );

  // ensure valid date time read
  do
  {
    uRegRd1 = REG_RTC_CALR;
    uRegRd2 = REG_RTC_CALR;
  } while( uRegRd1 != uRegRd2 );

  // now parse the values
  ptDateTime->nDay = BcdToHex(( RTC_CALR_DATE_Msk & uRegRd1 ) >> RTC_CALR_DATE_Pos );
  ptDateTime->nMonth = BcdToHex(( RTC_CALR_MONTH_Msk & uRegRd1 ) >> RTC_CALR_MONTH_Pos );
  ptDateTime->wYear = BcdToHex(( RTC_CALR_YEAR_Msk & uRegRd1 ) >> RTC_CALR_YEAR_Pos );
  ptDateTime->wYear += ((( RTC_CALR_CENT_Msk & uRegRd1 ) >> RTC_CALR_CENT_Pos ) * 100 );
}

/******************************************************************************
 * @function Rtc_GetValid
 *
 * @brief RTC get validity flag
 *
 * This function will return the validity flag
 *
 * @return  the current state of the validity flag
 *
 *****************************************************************************/
BOOL Rtc_GetValid( void )
{
  // return he validiy flag
  return( bValid );
}

/******************************************************************************
 * @function RTC_Handler
 *
 * @brief interrupt RTC handler
 *
 * This function handles the interrupts from RTC
 *
 *****************************************************************************/
void RTC_Handler( void )
{
  U32 uStatusReg;

  // get the status register
  uStatusReg = REG_RTC_SR;

  // check for a second event 
  if ( RTC_SR_SEC_SECEVENT & uStatusReg )
  {
    // clear the event
    REG_RTC_SCCR = RTC_SCCR_SECCLR;

    // if time set pending
    if ( bTimeSetPending )
    {
      // clear the time pending flag
      bTimeSetPending = FALSE;

      // set the update time/cal bits
      REG_RTC_CR |= RTC_CR_UPDCAL | RTC_CR_UPDTIM;

      // enable the ack update interrupt
      REG_RTC_IER = RTC_IER_ACKEN;
    }
  }

  // now check for Update
  if ( RTC_SR_ACKUPD_UPDATE & uStatusReg )
  {
    // clear the event
    REG_RTC_SCCR = RTC_SCCR_ACKCLR;

    // disable the interrupt
    REG_RTC_IDR = RTC_IDR_ACKDIS;

    // now update the registers
    REG_RTC_TIMR = uCurTimeSet;
    REG_RTC_CALR = uCurDateSet;

    // clear the update flags
    REG_RTC_CR &= ~( RTC_CR_UPDCAL | RTC_CR_UPDTIM );

    // set the valid flag
    bValid = TRUE;
  }
  
  // now check for alarm event
  if ( RTC_SR_ALARM_ALARMEVENT & uStatusReg )
  {
    // clear the event
    REG_RTC_SCCR = RTC_SCCR_ALRCLR;

    // if callback is not null
    if ( NULL != pvAlarmCallback )
    {
      // call the callback
      pvAlarmCallback( );
    }
  }
}

/******************************************************************************
 * @function HexToBcd
 *
 * @brief convert a hex value to a bcd value
 *
 * This function will convert the hex valud to bcd value
 *
 * @param[in]   nHexValue   hex value to convert
 *
 *****************************************************************************/
static U8 HexToBcd( U8 nHexValue )
{
  U8	nBcd = 0;
  U8	nMsb;

  // convert it
  nMsb = nHexValue / 10;
  nBcd = nHexValue - ( nMsb * 10 );
  nBcd |= ( nMsb << 4 );

  // return the value
  return( nBcd );
}

/******************************************************************************
 * @function BcdToHex
 *
 * @brief convert a BCD value to a HEX value
 *
 * This function will convert the bcd value to a hex value
 *
 * @param[in]   nBcdVal    BCD value
 *
 * @return hex value of the BCD encoded number
 *
 *****************************************************************************/
static U8 BcdToHex( U8 nBcdVal )
{
  U8 nValue;

  // convert to hex 
  nValue = ( nBcdVal >> 4 ) * 10;
  nValue |= ( nBcdVal & 0x0F );

  // return the value
  return( nValue );
}

/**@} EOF Rtc.c */
