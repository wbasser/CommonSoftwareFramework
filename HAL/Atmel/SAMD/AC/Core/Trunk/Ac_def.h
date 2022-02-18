/******************************************************************************
 * @file Ac_def.h
 *
 * @brief Analog comparator definitions 
 *
 * This file provides the definitioans for the Analog Comparator
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
 * \addtogroup Ac
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AC_DEF_H
#define _AC_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for creating entries
#define ACCHANDEFBLKM( chan, posinp, neginp, filtlen, speedsel, hystenb, singshot, inpevnenb, outevnenb, outmode, outedge ) \
  { \
    .eChannel       = chan, \
    .ePosChan       = posinp, \
    .eNegChan       = neginp, \
    .eFiltLen       = filtlen, \
    .eSpeedSel      = speedsel, \
    .bHystEnb       = hystenb, \
    .bSingleShot    = singshot, \
    .bEventInpEnb   = inpevnenb, \
    .bEventOutEnb   = outebnenb, \
    .eOutMode       = outmode, \
    .eOutEdge       = outedge, \
    .pvCallback     = NULL, \
  }

/// define the helper macro for creating entries
#define ACCHANDEFNBLKM( chan, posinp, neginp, filtlen, speedsel, hystenb, singshot, inpevnenb, outevnenb, outmode, outedge, callback ) \
  { \
    .eChannel       = chan, \
    .ePosChan       = posinp, \
    .eNegChan       = neginp, \
    .eFiltLen       = filtlen, \
    .eSpeedSel      = speedsel, \
    .bHystEnb       = hystenb, \
    .bSingleShot    = singshot, \
    .bEventInpEnb   = inpevnenb, \
    .bEventOutEnb   = outebnenb, \
    .eOutMode       = outmode, \
    .eOutEdge       = outedge, \
    .pvCallback     = callback, \
  }

// enumerations ---------------------------------------------------------------
/// define the channels
typedef enum _ACCHANNEL
{
  AC_CHANNEL_0,             ///< channel 0
  AC_CHANNEL_1,             ///< channel 1
  AC_CHANNEL_MAX
} ACCHANNEL;

/// enumerate the  positive channels
typedef enum _ACPOSCHAN
{
  AC_POSCHAN_AIN00 = 0,     ///< AC AIN0 pin
  AC_POSCHAN_AIN01,         ///< AC AIN1 pin
  AC_POSCHAN_AIN02,         ///< AC AIN2 pin
  AC_POSCHAN_AIN03,         ///< AC AIN3 pin
  AC_POSCHAN_MAX
} ACPOSCHAN;

/// enumerate the negative channels
typedef enum _ACNEGCHAN
{
  AC_NEGCHAN_AIN00 = 0,       ///< AC AIN0 pin
  AC_NEGCHAN_AIN01,           ///< AC AIN1 pin
  AC_NEGCHAN_AIN02,           ///< AC AIN2 pin
  AC_NEGCHAN_AIN03,           ///< AC AIN3 pin
  AC_NEGCHAN_GND,             ///< ground
  AC_NEGCHAN_VSCALE,          ///< VDD scaler
  AC_NEGCHAN_BANDGAP,         ///< internal bandgap
  AC_NEGHCAN_DAC,             ///< DAC output
  AC_NEGCHAN_MAX
} ACNEGCHAN;

/// enumerate the output mode
typedef enum _ACOUTMODE
{
  AC_OUTMODE_OFF = 0,      ///< output off
  AC_OUTMODE_ASYNC,        ///< async output
  AC_OUTMODE_SYNC,         ///< sync output
  AC_OUTMODE_MAX
} ACOUTMODE;

/// enumerate the output edge
typedef enum _ACOUTEDGE
{
  AC_OUTEDGE_TOGGLE = 0,      ///< toggle output
  AC_OUTEDGE_RISING,          ///< rising edge
  AC_OUTEDGE_FALLING,         ///< falling edge
  AC_OUTEDGE_EOC,             ///< end of compare
  AC_OUTEDGE_MAX
} ACOUTEDGE;

/// enumerate the filter length
typedef enum _ACFILTLEN
{
  AC_FILTLEN_OFF = 0,         ///< filter length = 0
  AC_FILTLEN_MAJ3,            ///< filter length - 3 bit majority
  AC_FILTLEN_MAJ5,            ///< filter length - 5 bit majority
  AC_FILTLEN_MAX
} ACFILTLEN;

/// enumerate the speed selection
typedef enum _ACSPEEDSEL
{
  AC_SPEEDSEL_LOW = 0,        ///< low speed
  AC_SPEEDSEL_HIGH,           ///< high speed
  AC_SPEEDSEL_MAX
} ACSPEEDSEL;

// structures -----------------------------------------------------------------
/// define the callback 
typedef void ( *PVACCALLBACK )( void );

/// define the AC channel definition structure
typedef struct _ACDEF
{
  ACCHANNEL     eChannel;     ///< selected channel
  ACPOSCHAN     ePosChan;     ///< positive channel
  ACNEGCHAN     eNegChan;     ///< negative channel
  ACFILTLEN     eFiltLen;     ///< filter length
  ACSPEEDSEL    eSpeedSel;    ///< speed select
  BOOL          bHystEnb;     ///< hystersis enable
  BOOL          bSingleShot;  ///< single shot
  BOOL          bEventInpEnb; ///< input event enable
  BOOL          bEventOutEnb; ///< output event enable
  ACOUTMODE     eOutMode;     ///< output mode
  ACOUTEDGE     eOutEdge;     ///< output edge
  PVACCALLBACK  pvCallback;   ///< callback
} ACDEF, *PACDEF;
#define ACDEF_SIZE     sizeof( ACDEF )

/**@} EOF Ac_def.h */

#endif  // _AC_DEF_H