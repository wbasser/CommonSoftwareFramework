/******************************************************************************
 * @file RtcPCF8253.c
 *
 * @brief Real Time Clock PCV8523 implementation
 *
 * This file provides the implementation for the PCV8512 real time clock
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
 * \addtogroup RtcPCF8253
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RtcPCF8523/RtcPCF8523_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the registers addresses
typedef enum _REGADDR
{
  REG_ADDR_CTRL1 = 0,
  REG_ADDR_CTRL2,
  REG_ADDR_CTRL3,
  REG_ADDR_SECS,
  REG_ADDR_MINS,
  REG_ADDR_HRS,
  REG_ADDR_DAYS,
  REG_ADDR_WDAY,
  REG_ADDR_MONTH,
  REG_ADDR_YEAR,
  REG_ADDR_MINALM,
  REG_ADDR_HRALM,
  REG_ADDR_DAYLAM,
  REG_ADDR_WKDAYALM,
  REG_ADDR_OFFSET,
  REG_ADDR_CLKOUTCTL,
  REG_ADDR_TMRACTRL,
  REG_ADDR_TMRAVAL,
  REG_ADDR_TMRBCTRL,
  REG_ADDR_TMRBVAL,
  REG_ADDR_MAX
} REGADDR;

// structures -----------------------------------------------------------------
/// define the structure for the CTL1 register
typedef union _REGCTRL1
{
  struct
  {
    BOOL  bCieEnb   : 1;    ///< correction interupt enabled
    BOOL  bAieEnb   : 1;    ///< alaram interrupt enabled
    BOOL  bSieEnb   : 1;    ///< second interrupt enabled
    BOOL  b1224Sel  : 1;    ///< 12/24 hour selection
    BOOL  bSfwReset : 1;    ///< perform a software reset
    BOOL  bStopEnb  : 1;    ///< stop the RTC
    BOOL  bReserved : 1;    ///< reserved
    BOOL  bCapSel   : 1;    ///< cap selection
  } tFields;
  U8  nByte;
} REGCTRL1, *PREGCTRL1;
#define REGCTRL1_SIZE                           sizeof( REGCTRL1 )

/// define the structure for the CTRL2 register
typedef union _REGCTRL2
{
  struct
  {
    BOOL  bCtbEnb   : 1;    ///< counter b interrupt enabled
    BOOL  bCtaEnb   : 1;    ///< counter A interrupt enabled
    BOOL  bWtaEnb   : 1;    ///< watchdog interrupt enabled
    BOOL  bAlmFlag  : 1;    ///< alarm interrupt flag
    BOOL  bSecFlag  : 1;    ///< second interrupt flag
    BOOL  bCtbFlag  : 1;    ///< counter B interrupt flag
    BOOL  bCtaFlag  : 1;    ///< counter A interrupt flag
    BOOL  bWtaFlag  : 1;    ///< watchdog A interrupt flag
  } tFields;
  U8  nByte;
} REGCTRL2, *PREGCTRL2;
#define REGCTRL2_SIZE                           sizeof( REGCTRL2 )

/// define the structure for the control 3 register
typedef union _REGCTRL3
{
  struct
  {
    BOOL  bBlwEnb   : 1;    ///< Battery low interupt enabled
    BOOL  bBswEnb   : 1;    ///< battery switchover interrupt enabled
    BOOL  bBlwFlag  : 1;    ///< battery low interrupt flag
    BOOL  bBswFlag  : 1;    ///< battery switchover interrupt flag
    BOOL  bReserved : 1;    ///< reserved
    U8    nBswMode  : 3;    ///< battery switch mode
  } tFields;
  U8  nByte;
} REGCTRL3, *PREGCTRL3;
#define REGCTRL3_SIZE                           sizeof( REGCTRL3 )

/// define the structure for the seconds register
typedef union _REGSECS
{
  struct
  {
    U8    nSecOnes      : 4;      ///< seconds 
    U8    nSecTens      : 3;      ///< seconds - tens
    U8    bOscDisabled  : 1;      ///< osciallator disabled
  } tFields;
  U8  nByte;
} REGSECS, *PREGSECS;
#define REGSECS_SIZE                            sizeof( REGSECS )

/// define the structure for the minutes register
typedef union _REGMINS
{
  struct
  {
    U8    nMinOnes      : 4;      ///< minutes 
    U8    nMinTens      : 3;      ///< minutes - tens
    U8    nReserved     : 1;      ///< reserved
  } tFields;
  U8  nByte;
} REGMINS, *PREGMINS;
#define REGMINS_SIZE                            sizeof( REGMINS )

/// define the structure for the  register
typedef union _REGHRS
{
  struct
  {
    U8    nHrsOnes      : 4;      /// hours - ones
    union
    {
      struct
      {
        U8    nHrsTens  : 1;      ///< hours tens
        BOOL  bAmPm     : 1;      ///< AM = 0, PM = 1
      } tMode12;
      struct
      {
        U8    nHrsTens  : 2;      ///< hours tens
      } tMode24;
    } tMode;
    U8  nReserved       : 2;      ///< reserved
  } tFields;
  U8  nByte;
} REGHRS, *PREGHRS;
#define REGHRS_SIZE                             sizeof( REGHRS )

/// define the structure for the days register
typedef union _REGDAYS
{
  struct
  {
    U8    nDaysOnes       : 4;      ///< days ones
    U8    nDaysTens       : 2;      ///< days tens
    U8    nReserved       : 2;      ///< reserved
  } tFields;
  U8  nByte;
} REGDAYS, *PREGDAYS;
#define REGDAYS_SIZE                            sizeof( REGDAYS )

/// define the structure for the weekday register
typedef union _REGWDAY
{
  struct
  {
    U8    nWeekDay        : 2;      ///< week day
    U8    nReserved       : 6;      ///< reserved
  } tFields;
  U8  nByte;
} REGWDAY, *PREGWDAY;
#define REGWDAY_SIZE                            sizeof( REGWDAY )

/// define the structure for the months register
typedef union _REGMONTH
{
  struct
  {
    U8    nMonthOnes    : 4;      ///< month ones
    U8    nMonthTens    : 1;      ///< month tens
  } tFields;
  U8  nByte;
} REGMONTH, *PREGMONTH;
#define REGMONTH_SIZE                           sizeof( REGMONTH )

/// define the structure for the year register
typedef union _REGYEAR
{
  struct
  {
    U8    nYearOnes     : 4;      ///< year ones
    U8    nYearTens     : 4;      ///< year tens
  } tFields;
  U8  nByte;
} REGYEAR, *PREGYEAR;
#define REGYEAR_SIZE                            sizeof( REGYEAR )

/// define the structure for the minute alarm register
typedef union _REGMINALM
{
  struct
  {
    U8    nMinOnes      : 4;      ///< minutes ones
    U8    nMinTens      : 3;      ///< minutes tens
    BOOL  bEnabled      : 1;      ///< enabled
  } tFields;
  U8  nByte;
} REGMINALM, *PREGMINALM;
#define REGMINALM_SIZE                          sizeof( REGMINALM )

/// define the structure for the  hour alarm register
typedef union _REGHRALM
{
  struct
  {
    U8    nHrsOnes      : 4;      ///< hours ones
    union
    {
      struct
      {
        U8    nHrsTens    : 1;      ///< hours tens
        BOOL  bAmPm       : 1;      ///< AM/PM 
      } tMode12;
      struct
      {
        U8    nHrsTens    : 2;      ///< hours tens
      } tMode24;
    } tMode;
    U8    nReserved       : 1;      ///< reserved
    BOOL  bEnabled        : 1;      ///< enabled
  } tFields;
  U8  nByte;
} REGHRALM, *PREGHRALM;
#define REGHRALM_SIZE                           sizeof( REGHRALM )

/// define the structure for the days alarms register
typedef union _REGDAYALM
{
  struct
  {
    U8    nDaysOnes       : 4;      ///< days ones
    U8    nDaysTens       : 2;      ///< days tens
    U8    nReserved       : 1;      ///< reserved
    BOOL  bEnabled        : 1;      ///< enabled
  } tFields;
  U8  nByte;
} REGDAYALM, *PREGDAYALM;
#define REGDAYALM_SIZE                          sizeof( REGDAYALM )

/// define the structure for the week day alarm register
typedef union _REGWKADYALM
{
  struct
  {
    U8    nWkdayOnes      : 3;      ///< days ones
    U8    nReserved       : 4;      ///< reserved
    BOOL  bEnabled        : 1;      ///< enabled
  } tFields;
  U8  nByte;
} REGWKADYALM, *PREGWKADYALM;
#define REGWKADYALM_SIZE                        sizeof( REGWKADYALM )

/// define the structure for the offset register
typedef union _REGOFFSET
{
  struct
  {
    C8    cOffset         : 7;      ///< offset
    BOOL  bMode           : 1;      ///< mode
  } tFields;
  U8  nByte;
} REGOFFSET, *PREGOFFSET;
#define REGOFFSET_SIZE                          sizeof( REGOFFSET )

/// define the structure for the clock out control register
typedef union _REGCLKOUTCTL
{
  struct
  {
    BOOL  bTbcEnb         : 1;      ///< timer B Enable
    U8    nTaMode         : 2;      ///< timer A mode
    U8    nClkFreq        : 3;      ///< clock out frequency
    BOOL  bTbIrq          : 1;      ///< timer B interrupt type
    BOOL  bTaIrq          : 1;      ///< timer A interrupt type
  } tFields;
  U8  nByte;
} REGCLKOUTCTL, *PREGCLKOUTCTL;
#define REGCLKOUTCTL_SIZE                       sizeof( REGCLKOUTCTL )

/// define the structure for the timer a control register
typedef union _REGTMRACTRL
{
  U8    nFreqSrc;
} REGTMRACTRL, *PREGTMRACTRL;
#define REGTMRACTRL_SIZE                        sizeof( REGTMRACTRL )

/// define the structure for the timer A value register
typedef union _REGTMRAVAL
{
  U8    nValue;
} REGTMRAVAL, *PREGTMRAVAL;
#define REGTMRAVAL_SIZE                         sizeof( REGTMRAVAL )

/// define the structure for the timer B control register
typedef union _REGTMRBCTRL
{
  struct
  {
    U8    nTimBSrc        : 3;        ///< timer b clk source
    U8    nReserved1      : 1;        ///< reserved
    U8    nTimBWidth      : 1;        ///< timer B pulse width
    U8    nReserved2      : 1;        ///< reserved
  } tFields;
  U8  nByte;
} REGTMRBCTRL, *PREGTMRBCTRL;
#define REGTMRBCTRL_SIZE                        sizeof( REGTMRBCTRL )

/// define the structure for the timer C value register
typedef union _REGTMRBVAL
{
  U8    nValue;
} REGTMRBVAL, *PREGTMRBVAL;
#define REGTMRBVAL_SIZE                         sizeof( REGTMRBVAL )

/// define register array
typedef struct _REGS
{
  struct
  {
    REGCTRL1      tRegCtrl1;
    REGCTRL2      tRegCtrl2;
    REGCTRL3      tRegCtrl3;
    REGSECS       tRegSecs;
    REGMINS       tRegMins;
    REGHRS        tRegHrs;
    REGDAYS       tRegDays;
    REGWDAY       tRegWday;
    REGMONTH      tRegMonth;
    REGYEAR       tRegYear;
    REGMINALM     tRegMinAlm;
    REGHRALM      tRegHrAlm;
    REGDAYALM     tRegDayAlm;
    REGWKADYALM   tRegWkDayAlm;
    REGOFFSET     tRegOffset;
    REGCLKOUTCTL  tRegClkOutCtl;
    REGTMRACTRL   tRegTmrACtl;
    REGTMRAVAL    tRegTmrAVal;
    REGTMRBCTRL   tRegTmrBCtl;
    REGTMRBVAL    tRegTmrBVal;
  } tRegs;
  U8  anRegs[ 1 ];
} REGS, *PREGS;
#define REGS_SIZE                               sizeof( REGS )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  U8    HexToBcd( U8 nHexValue );
static  void  BcdToHex( U8 nBcdMsb, U8 nBcdLsb, PU8 pnHexValue );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function RtcPCF8523Initialize
 *
 * @brief initilization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void RtcPCF8523Initialize( void )
{
  REGS  tRegs;
  
  // call the local initialization
  RtcPCF8523_LocalInitialize( );
  
  // now read the base register and determine if there has been a power fail
  if ( RtcPCF8523_ReadRegisters( REG_ADDR_CTRL3, 1, ( PU8 )&tRegs.tRegs.tRegCtrl3 ) == RTCPCF8523_ERR_NONE )
  {
    // now test for switch mode
    if ( tRegs.tRegs.tRegCtrl3.tFields.nBswMode == RTCPCF8523_CTRL3_BATSW_DSBLBD )
    {
      // set up control 1
      tRegs.tRegs.tRegCtrl1.tFields.bCieEnb   = RTCPCF8523_CTRL1_CORIRQENB;
      tRegs.tRegs.tRegCtrl1.tFields.bAieEnb   = RTCPCF8523_CTRL1_ALMIRQENB;
      tRegs.tRegs.tRegCtrl1.tFields.bSieEnb   = RTCPCF8523_CTRL1_SECIRQENB;
      tRegs.tRegs.tRegCtrl1.tFields.b1224Sel  = RTCPCF8523MMODE_SELECT;

      // set up control 2
      tRegs.tRegs.tRegCtrl2.tFields.bCtbEnb   = RTCPCF8523_CTRL2_TMBIRQENB;
      tRegs.tRegs.tRegCtrl2.tFields.bCtaEnb   = RTCPCF8523_CTRL2_TMAIRQENB;
      tRegs.tRegs.tRegCtrl2.tFields.bWtaEnb   = RTCPCF8523_CTRL2_WTDIRQENB;

      // set up control 3
      tRegs.tRegs.tRegCtrl3.tFields.bBlwEnb   = RTCPCF8523_CTRL3_BLWIRQENB;
      tRegs.tRegs.tRegCtrl3.tFields.bBswEnb   = RTCPCF8523_CTRL3_BSWIRQENB;
      tRegs.tRegs.tRegCtrl3.tFields.nBswMode  = RTCPCF8523_CTRL3_BATSWMODE;

      // now write the register
      RtcPCF8523_WriteRegisters( REG_ADDR_CTRL1, 3, ( PU8 )&tRegs.tRegs.tRegCtrl1 );
    }
  }
}

/******************************************************************************
 * @function RtcPCF8523SetDateTime
 *
 * @brief set date and time
 *
 * This function will set the date time
 *
 * @param[in]   ptDateTime    pointer to the date time structure
 *
 * @return      approriate error
 *
 *****************************************************************************/
RTCPCF8523ERR RtcPCF8523SetDateTime( PDATETIME ptDateTime )
{
  REGS tRegs;
  
  // fill in the date time
  tRegs.tRegs.tRegSecs.nByte = HexToBcd( ptDateTime->nSeconds );
  tRegs.tRegs.tRegMins.nByte = HexToBcd( ptDateTime->nMinutes );
  tRegs.tRegs.tRegHrs.nByte = HexToBcd( ptDateTime->nHours );
  tRegs.tRegs.tRegDays.nByte = HexToBcd( ptDateTime->nDay );
  tRegs.tRegs.tRegWday.nByte = HexToBcd( ptDateTime->nDayOfWeek );
  tRegs.tRegs.tRegMonth.nByte = HexToBcd( ptDateTime->nMonth );
  tRegs.tRegs.tRegYear.nByte = HexToBcd( ptDateTime->wYear & 0xFF );

  //; now write it
  return( RtcPCF8523_WriteRegisters( REG_ADDR_SECS, 7, ( PU8 )&tRegs.tRegs.tRegSecs ));
}

/******************************************************************************
 * @function RtcPCF8523GetDateTime
 *
 * @brief get the date and time
 *
 * This function return the current date/time
 *
 * @param[in]   ptDateTime    pointer to store the date time
 *
 * @return      appropriate error
 *
 *****************************************************************************/
RTCPCF8523ERR RtcPCF8523GetDateTime( PDATETIME ptDateTime )
{
  REGS          tRegs;
  RTCPCF8523ERR eError;

  // read the date time
  if (( eError = RtcPCF8523_ReadRegisters( REG_ADDR_SECS, 7, ( PU8 )&tRegs.tRegs.tRegSecs )) == RTCPCF8523_ERR_NONE )
  {
    // copy the data
    BcdToHex( tRegs.tRegs.tRegSecs.tFields.nSecTens, tRegs.tRegs.tRegSecs.tFields.nSecOnes, &ptDateTime->nSeconds );
//    ptDateTime->nMinutes    = BcdToHex( tRegs.tRegs.tRegMins.nByte );
//    ptDateTime->nHours      = BcdToHex( tRegs.tRegs.tRegHrs.nByte );
//    ptDateTime->nDay        = BcdToHex( tRegs.tRegs.tRegDays.nByte );
//    ptDateTime->nDayOfWeek  = BcdToHex( tRegs.tRegs.tRegWday.nByte );
//    ptDateTime->nMonth      = BcdToHex( tRegs.tRegs.tRegMonth.nByte );
//    ptDateTime->wYear       = BcdToHex( tRegs.tRegs.tRegYear.nByte );
  }

  // return the status
  return( eError );
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
 * @param[in]   nBcdMsb    BCD msb
 * @param[in]   nBcdLsb    BCD lsb
 * @param[in]   pnHexValue  poitner to store hex value
 *
 *****************************************************************************/
static void BcdToHex( U8 nBcdMsb, U8 nBcdLsb, PU8 pnHexValue )
{
  // convert and store
  *( pnHexValue ) = ( nBcdMsb * 10 ) + nBcdLsb;
}

/**@} EOF RtcPCF8253.c */
