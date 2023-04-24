/******************************************************************************
 * @file TimerTc.h
 *
 * @brief TimerTc declaration
 *
 * This file provides the declarations for the TimerTc module
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
 * \addtogroup TimerTc
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TIMERTC_H
#define _TIMERTC_H

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro to enable the IOCTL function
#define TIMERTC_ENABLE_IOCTL                    ( ON )

// enumerations ---------------------------------------------------------------
/// enumerate the TIMERTC errors
typedef enum _TIMERTCERR
{
  TIMERTC_ERR_NONE = 0,      ///< no error
  TIMERTC_ERR_ILLTIMER,      ///< illegal TIMER
  TIMERTC_ERR_ILLACTION,     ///< illegal IOCTL action
} TIMERTCERR;

/// enumerate the ioctl actions
typedef enum _TIMERTCIOCTL
{
  TIMERTC_IOCTL_CHANGEDEF = 0,    ///< change the TIMERTC definition
  TIMERTC_IOCTL_STOPSTART,        ///< stop/start the TIMERTC
  TIMERTC_IOCTL_GETCOUNT,         ///< get the count
  TIMERTC_IOCTL_SETCOUNT,         ///< set the count
  TIMERTC_IOCTL_GETPERIOD,        ///< get the period
  TIMERTC_IOCTL_SETPERIOD,        ///< set the period
  TIMERTC_IOCTL_GETCOMPAREVAL,    ///< get the compare value
  TIMERTC_IOCTL_SETCOMPAREVAL,    ///< set the compare value
  TIMERTC_IOCTL_SETCOMPAREPCT,    ///< set the compare in percent
  TIMERTC_IOCTL_GETCAPTURE,       ///< get the capture value
  TIMERTC_IOCTL_SETCAPTURE,       ///< set the capture value
  TIMERTC_IOCTL_GETPRESCALEDIV,   ///< get the prescale divisor
  TIMERTC_IOCTL_SETPPERIOD50,     ///< set period with 50% duty cycle
  TIMERTC_IOCTL_MAX
} TIMERTCIOCTL;

/// enumerate the callback event
typedef enum _TIMERTCCBEVENT
{
  TIMERTC_CBEVENT_NONE = 0,
  TIMERTC_CBEVENT_TMO,            ///< 1 - time out event
  TIMERTC_CBEVENT_CMPUP,          ///< 2 - compare match - counting up
  TIMERTC_CBEVENT_CMPDN,          ///< 3 - compare match - counting down
  TIMERTC_CBEVENT_PERUP,          ///< 4 - period match - counting up
  TIMERTC_CBEVENT_CAPRE,          ///< 5 - capture event - rising edge
  TIMERTC_CBEVENT_CAPFE,          ///< 6 - capture event - falling edge
  TIMERTC_CBEVENT_CAPTURE,        ///< 7 - capture event
} TIMERTCCBEVENT;

/// enumerate the TIMERTC channels
typedef enum _TIMERTCCHAN
{
  #ifdef _SAMD11_
    TIMERTC_CHAN_1 = 0,
    TIMERTC_CHAN_2,
  #elif
    TIMERTC_CHAN_3 = 0,
    TIMERTC_CHAN_4,                 ///< TIMERTC channel 4
    TIMERTC_CHAN_5,                 ///< TIMERTC channel 5
    #ifdef TC6
      TIMERTC_CHAN_6,                 ///< TIMERTC channel 6
    #endif  // TC6
    #ifdef TC7
      TIMERTC_CHAN_7,                 ///< TIMERTC channel 7
    #endif  // TC7
  #endif  // _SAMD11_
  TIMERTC_CHAN_MAX
} TIMERTCCHAN;

/// enumerate the TIMERTC compare/capture channels
typedef enum _TIMERTCCMPCAPCHAN
{
  TIMERTC_CMPCAP_CHAN_0 = 0,
  TIMERTC_CMPCAP_CHAN_1,
} TIMERTCCMPCAPCHAN;

/// enumerate the TIMERTC CT modes
typedef enum _TIMERTCTCMODE
{
  TIMERTC_MODE_WAVNF = 0,       ///< wave output, normal frequency
  TIMERTC_MODE_WAVMF,           ///< wave output, match frequency
  TIMERTC_MODE_PWM,             ///< PWM
  TIMERTC_MODE_CAPTURE,         ///< capture
  TIMERTC_MODE_CAPPPW,          ///< capture PPW
  TIMERTC_MODE_CAPPWP,          ///< capture PWP
  TIMERTC_MODE_MAX
} TIMERTCTCMODE;

/// enumerate the presclae values
typedef enum _TIMERTCPRESCALE
{
  TIMERTC_PRESCALE_1 = 0,         ///< no division
  TIMERTC_PRESCALE_2,             ///< divide by 2
  TIMERTC_PRESCALE_4,             ///< divide by 4
  TIMERTC_PRESCALE_8,             ///< divide by 8
  TIMERTC_PRESCALE_16,            ///< divide by 16
  TIMERTC_PRESCALE_64,            ///< divide by 64
  TIMERTC_PRESCALE_256,           ///< divide by 256
  TIMERTC_PRESCALE_1024,          ///< divide by 1024
  TIMERTC_PRESCALE_MAX
} TIMERTCPRESCALE;

/// enumerate the direction
typedef enum _TIMERTCDIRECTION
{
  TIMERTC_DIRECTION_UP = 0,        ///< count up
  TIMERTC_DIRECTION_DN,            ///< count done
  TIMERTC_DIRECTION_MAX
} TIMERTCDIRECTION;

/// enumerate the event action
typedef enum _TIMERTCEVENTACT
{
  TIMERTC_EVENTACT_OFF = 0,       ///< no action
  TIMERTC_EVENTACT_RETRIG,        ///< retrigger
  TIMERTC_EVENTACT_COUNT,         ///< count on event
  TIMERTC_EVENTACT_START,         ///< start TC on event
  TIMERTC_EVENTACT_PPW = 0x05,    ///< period capture in CC0, pulse width in CC1
  TIMERTC_EVENTACT_PWP,           ///< period capture in CC1, pulse width in CC0
  TIMERTC_EVENTACT_MAX
} TIMERTCEVENTACT;

/// enumerate the compare/capture channels
typedef enum _TIMERTCCMPCAP
{
  TIMERTC_CMPCAP_CHAN0 = 0,       ///< compare/capture 0
  TIMERTC_CMPCAP_CHAN1,           ///< compare/capture 1
  TIMERTC_CMPCAP_MAX
} TIMERTCCMPCAP;

// structures -----------------------------------------------------------------
/// define the callback structure
typedef void ( *PVTIMERTCCALLBACK )( TIMERTCCBEVENT, TIMERTCCMPCAP, U16 );

/// define the task handle
typedef PVOID   PTTIMERHANDLER;

/// define the flag structure
typedef struct _TIMEFLAGS
{
  BOOL  bOneShot          : 1;    ///< oneshot mode
  BOOL  bInitialOn        : 1;    ///< initially on
  BOOL  bRunStandby       : 1;    ///< run in standby
  BOOL  bOvfOutEnable     : 1;    ///< overflow output enable
  BOOL  bOvfIrqEnable     : 1;    ///< interrupt on overflow
  BOOL  bResetToZero      : 1;    ///< reset to zero
  BOOL  bTimesInUsec      : 1;    ///< times in USEC
  BOOL  bInvertEdge       : 1;    ///< invert the first edge in capture
  BOOL  bHighPriority     : 1;    ///< high priority
} TIMEFLAGS, *PTIMEFLAGS;
#define TIMEFLAGS_SIZE                          sizeof( TIMEFLAGS )

/// define the TIMERTC definition
typedef struct _TIMERTCDEF
{
  TIMERTCCHAN       eChannel;                               ///< TIMERTC channel
  TIMERTCTCMODE     eMode;                                  ///< TIMERTC TC mode
  TIMEFLAGS         tFlags;                                 ///< timer flags
  BOOL              abChanIrqEnables[ TIMERTC_CMPCAP_MAX ]; ///< enable the match/capture IRQ's
  BOOL              abChanOpsEnables[ TIMERTC_CMPCAP_MAX ]; ///< enable the match/capture channels
  BOOL              abInvertOutputs[ TIMERTC_CMPCAP_MAX ];  ///< TIMERTC output enables
  TIMERTCDIRECTION  eDirection;                             ///< direction
  TIMERTCPRESCALE   ePrescale;                              ///< prescale
  TIMERTCEVENTACT   eEventAction;                           ///< event action
  U16               uPwmPeriod;                             ///< period for PWM generation
  U16               awChanValues[ TIMERTC_CMPCAP_MAX ] ;    ///< rate in hertz
  PVTIMERTCCALLBACK pvCallback;                             ///< TIMERTC callback
} TIMERTCDEF, *PTIMERTCDEF;
#define TIMERTCDEF_SIZE                         sizeof( TIMERTCDEF )

/// define the IOCTL parameter structure
typedef struct _TIMERTCIOCTLPARAM
{
  TIMERTCCMPCAPCHAN   eCmpCapChan;    ///< compare/capture channel
  U32                 uValue;         ///< value
} TIMERTCIOCTLPARAM, *PTIMERTCIOCTLPARAM;
#define TIMERTCIOCTLPARAM_SIZE                  sizeof( TIMERTCIOCTLPARAM )

// global function prototypes --------------------------------------------------
extern  PTTIMERHANDLER  TimerTc_Create( PTIMERTCDEF ptTimerTcDef );
#if ( TIMERTC_ENABLE_IOCTL == ON )
  extern  TIMERTCERR      TimerTc_Ioctl( PTTIMERHANDLER ptTimer, TIMERTCIOCTL eAction, PVOID pvParam );
#endif

/**@} EOF TIMERTC.h */

#endif  // _TIMERTC_H
