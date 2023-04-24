/******************************************************************************
 * @file TimerTc.c
 *
 * @brief timers implementation
 *
 * This file provides the implementation for the timers module
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

// local includes -------------------------------------------------------------
#include "TimerTc/TimerTc.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "PowerManager/PowerManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the base IRQ number
#ifdef _SAMD11_
  #define NVIC_IRQ_BASE   TC1_IRQn
#else
  #define NVIC_IRQ_BASE   TC3_IRQn
#endif

/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PLCLCTL )handle )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the struct for local control
typedef struct _LCLCTL
{
  struct _LCLCTL*   ptSignature;  ///< signature
  Tc*               ptTc;         ///< pointer to the control registers for TC3-7
  CLOCKMUXID        eMuxId;       ///< clock mux id
  U32               uPwrMngrMask; ///< power manager mask
  TIMERTCDEF        tDef;         ///< definition structure
  U32               uActClock;    ///< clock/divided by prescale
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

/// define the physical to control map
typedef struct _PHYMAP
{
  PLCLCTL   ptTimer;
} PHYMAP, *PPHYMAP;
#define PHYMAP_SIZE   sizseof( PHYMAP )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PHYMAP  atPhyMaps[ TIMERTC_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  void        CommonTcInterruptHandler( PLCLCTL ptLclCtl );
static  void        ConfigureTcBase( PLCLCTL ptLclCtl );
static  void        ConfigureTcCapture( PLCLCTL ptLclCtl );
static  void        GetTimerChannel( PLCLCTL ptLclCtl );

// constant parameter initializations -----------------------------------------
const U16 awTimerPrescales[ TIMERTC_PRESCALE_MAX ] = 
{
  1, 2, 4, 8, 16, 64, 256, 1024
};

/******************************************************************************
 * @function TimerTc_Create
 *
 * @brief timer creation
 *
 * This function initialization all configured timers
 *
 *****************************************************************************/
PTTIMERHANDLER TimerTc_Create( PTIMERTCDEF ptTcDef )
{
  PLCLCTL ptTimer = NULL;
  
  // create the timer
  if (( ptTimer = malloc( LCLCTL_SIZE )) != NULL )
  {
    // set the physical to level translation/add to maps
    ptTimer->ptSignature = ptTimer;
    atPhyMaps[ ptTcDef->eChannel ].ptTimer = ptTimer;
    
    // copy the config
    memcpy( &ptTimer->tDef, ptTcDef, TIMERTCDEF_SIZE );

    // get the timer channel
    GetTimerChannel( ptTimer );

    // now enable the clock and power mask
    Clock_PeriphEnable( ptTimer->eMuxId, CLOCK_GENID_0 );
    PowerManager_DisableEnablePeriphC( ptTimer->uPwrMngrMask, ON );

    // determine the mode
    switch( ptTimer->tDef.eMode )
    {
      case TIMERTC_MODE_WAVNF :
      case TIMERTC_MODE_WAVMF :
      case TIMERTC_MODE_PWM :
        // configure the wave output
        ConfigureTcBase( ptTimer );
        break;

      case TIMERTC_MODE_CAPTURE :
      case TIMERTC_MODE_CAPPPW :
      case TIMERTC_MODE_CAPPWP :
        // configure the capture
        ConfigureTcCapture( ptTimer );
        break;

      default :
        break;
    }
  }
  
  // return the pointer
  return(( PTTIMERHANDLER )ptTimer );
}

#if ( TIMERTC_ENABLE_IOCTL == ON )
  /******************************************************************************
   * @function TimerTc_Ioctl
   *
   * @brief perform an IOCTL action on a selected timer
   *
   * This function will check for a valid action and perform that action on
   * a given timer
   *
   * @param[in]   ptTimer   timer handler
   * @param[in]   eAction     IOCTL action
   * @param[io]   pvParam     pointer to the parameter
   *
   * @return      appropriate error
   *
   *****************************************************************************/
  TIMERTCERR TimerTc_Ioctl( PTTIMERHANDLER ptTimer, TIMERTCIOCTL eAction, PVOID pvParam )
  {
    PLCLCTL             ptLclCtl;
    TIMERTCERR          eError = TIMERTC_ERR_NONE;
    PTIMERTCIOCTLPARAM  ptParam;
    U32                 uTemp, uPrescale, uParam;
    CLOCKMUXID          eClockMux;
    FLOAT               fPercent;
    BOOL                bState;

    // map handle to pointer
    ptLclCtl = MAP_HANDLE_TO_POINTER( ptTimer );

    // valid timer
    if ( ptLclCtl == ptLclCtl->ptSignature )
    {
      // determine the action
      switch( eAction )
      {
        case TIMERTC_IOCTL_CHANGEDEF :
          break;
        
        case TIMERTC_IOCTL_STOPSTART :
          // get the state
          bState = *( PBOOL )pvParam;

          // clear all interrupt flags
          ptLclCtl->ptTc->COUNT16.INTFLAG.reg = TC_INTFLAG_MASK;

          // check for capture
          if ( ptLclCtl->tDef.eMode == TIMERTC_MODE_CAPTURE )
          {
            // set the initial edge
            ptLclCtl->ptTc->COUNT16.EVCTRL.bit.TCINV = ptLclCtl->tDef.tFlags.bInvertEdge;
          }

          // enable the timer
          ptLclCtl->ptTc->COUNT16.CTRLA.bit.ENABLE = bState;
          while ( ptLclCtl->ptTc->COUNT16.STATUS.bit.SYNCBUSY );
          break;
        
        case TIMERTC_IOCTL_GETCOUNT :
          break;
        
        case TIMERTC_IOCTL_SETCOUNT :
          break;
        
        case TIMERTC_IOCTL_GETPERIOD :
          break;
        
        case TIMERTC_IOCTL_SETPERIOD :
          // determine the waveform mode
          switch( ptLclCtl->tDef.eMode )
          {
            case TIMERTC_MODE_WAVNF :
              break;

            case TIMERTC_MODE_WAVMF :
              break;

            default :
              break;
          }
          break;
        
        case TIMERTC_IOCTL_GETCOMPAREVAL :
          break;
        
        case TIMERTC_IOCTL_SETCOMPAREVAL :
          break;
        
        case TIMERTC_IOCTL_SETCOMPAREPCT :
          // get a pointer to the parameter
          ptParam = ( PTIMERTCIOCTLPARAM )pvParam;

          // compute the divider
          uTemp = 65535;
          fPercent = ( FLOAT )( ptParam->uValue / 1000.0 );
          uTemp *= fPercent;
          ptLclCtl->ptTc->COUNT16.CC[ ptParam->eCmpCapChan ].reg = uTemp;
          break;
        
        case TIMERTC_IOCTL_GETCAPTURE :
          break;
        
        case TIMERTC_IOCTL_SETCAPTURE :
          break;
        
        case TIMERTC_IOCTL_GETPRESCALEDIV :
         *(( PU32 )pvParam ) = awTimerPrescales[ ptLclCtl->tDef.ePrescale ];
          break;

        case TIMERTC_IOCTL_SETPPERIOD50 :
          // determine the waveform mode
          switch( ptLclCtl->tDef.eMode )
          {
            case TIMERTC_MODE_WAVNF :
            break;

            case TIMERTC_MODE_WAVMF :
            break;

            default :
            break;
          }
          break;

        default :
          // set the illegal timer error
          eError = TIMERTC_ERR_ILLACTION;
          break;
      }
    }
    else
    {
      // set the illegal timer error
      eError = TIMERTC_ERR_ILLTIMER;
    }
  
    return( eError );
  }
#endif

#ifdef _SAMD11_
  /******************************************************************************
   * @function TC1_Handler
   *
   * @brief TC1 interrupt handler
   *
   * This function handles the interrupt for TC1
   *
   *****************************************************************************/
  void TC1_Handler( void )
  {
    // call the common interrupt handler
    CommonTcInterruptHandler( atPhyMaps[ TIMERTC_CHAN_1 ].ptTimer );
  }

  /******************************************************************************
   * @function TC2_Handler
   *
   * @brief TC2 interrupt handler
   *
   * This function handles the interrupt for TC2
   *
   *****************************************************************************/
  void TC2_Handler( void )
  {
    // call the common interrupt handler
    CommonTcInterruptHandler( atPhyMaps[ TIMERTC_CHAN_2 ].ptTimer );
  }
#else
  #ifdef TCC3
    /******************************************************************************
     * @function TC3_Handler
     *
     * @brief TCC0 interrupt handler
     *
     * This function handles the interrupt for TCC0
     *
     *****************************************************************************/
    void TC3_Handler( void )
    {
      // call the common interrupt handler
      CommonTcInterruptHandler( atPhyMaps[ TIMERTC_CHAN_3 ].ptTimer );
    }
  #endif // TCC3 

  #ifdef TC4
    /******************************************************************************
     * @function TC4_Handler
     *
     * @brief TC4 interrupt handler
     *
     * This function handles the interrupt for TC4
     *
     *****************************************************************************/
    void TC4_Handler( void )
    {
      // call the common interrupt handler
      CommonTcInterruptHandler( atPhyMaps[ TIMERTC_CHAN_4 ].ptTimer );
    }
  #endif // TC4

  #ifdef TC5
    /******************************************************************************
     * @function TC5_Handler
     *
     * @brief TC5 interrupt handler
     *
     * This function handles the interrupt for TC5
     *
     *****************************************************************************/
    void TC5_Handler( void )
    {
      // call the common interrupt handler
      CommonTcInterruptHandler( atPhyMaps[ TIMERTC_CHAN_5 ].ptTimer );
    }
  #endif //TC5

  #ifdef TC6
    /******************************************************************************
     * @function TC6_Handler
     *
     * @brief TC6 interrupt handler
     *
     * This function handles the interrupt for TC6
     *
     *****************************************************************************/
    void TC6_Handler( void )
    {
      // call the common interrupt handler
      CommonTcInterruptHandler( atPhyMaps[ TIMERTC_CHAN_6 ].ptTimer );
    }
  #endif  // TC6

  #ifdef TC7
    /******************************************************************************
     * @function TC7_Handler
     *
     * @brief TC7 interrupt handler
     *
     * This function handles the interrupt for TC7
     *
     *****************************************************************************/
    void TC7_Handler( void )
    {
      // call the common interrupt handler
      CommonTcInterruptHandler( atPhyMaps[ TIMERTC_CHAN_7 ].ptTimer );
    }
  #endif  // TC7
#endif

/******************************************************************************
 * @function CommonInterruptTcHandler
 *
 * @brief common interrupt handler for TC
 *
 * This function handles all the interrupts for all  channels
 *
 * @param[in]   eTimer    timer enumeration
 *
 *****************************************************************************/
static void CommonTcInterruptHandler( PLCLCTL ptLclCtl )
{
  PVTIMERTCCALLBACK   pvCallback;
  TIMERTCCBEVENT      eEvent = TIMERTC_CBEVENT_NONE;
  TIMERTCCMPCAP       eCmpCap = TIMERTC_CMPCAP_MAX;
  U16                 wCapValue = 0;
  TC_INTFLAG_Type     tIntFlag;

  // get the interrupt flags
  tIntFlag.reg = ptLclCtl->ptTc->COUNT16.INTFLAG.reg;
  ptLclCtl->ptTc->COUNT16.INTFLAG.reg = TC_INTFLAG_MASK;

  // get the callback
  pvCallback = ptLclCtl->tDef.pvCallback;

  // check for capture mode
  if ( ptLclCtl->tDef.eMode == TIMERTC_MODE_CAPTURE )
  {
    // reset the counter
    if ( ptLclCtl->tDef.tFlags.bResetToZero )
    {
      ptLclCtl->ptTc->COUNT16.COUNT.reg = 0;
    }

    // set the event/channel/value
    eEvent = ( ptLclCtl->ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMERTC_CBEVENT_CAPFE : TIMERTC_CBEVENT_CAPRE;
    wCapValue = ptLclCtl->ptTc->COUNT16.CC[ TIMERTC_CMPCAP_CHAN0 ].reg;

    // toggle the edge
    ptLclCtl->ptTc->COUNT16.EVCTRL.bit.TCINV ^= 1;
  }
  
  // check for overflow
  if ( tIntFlag.bit.OVF && ptLclCtl->ptTc->COUNT16.INTENSET.bit.OVF )
  {
    eEvent = TIMERTC_CBEVENT_TMO;
  }
  else if ( tIntFlag.bit.MC0 && ptLclCtl->ptTc->COUNT16.INTENSET.bit.MC0 )
  {
    // set the channel
    eCmpCap = TIMERTC_CMPCAP_CHAN0;

    // determine the mode
    switch( ptLclCtl->tDef.eMode )
    {
      case TIMERTC_MODE_WAVNF :
      case TIMERTC_MODE_WAVMF :
      case TIMERTC_MODE_PWM :
        // set the event/channel/value
        eEvent = ( ptLclCtl->tDef.eDirection == TIMERTC_DIRECTION_UP ) ? TIMERTC_CBEVENT_CMPUP : TIMERTC_CBEVENT_CMPDN;
        break;

      case TIMERTC_MODE_CAPPPW :
        // set the event/channel/value
        eEvent = ( ptLclCtl->ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMERTC_CBEVENT_CAPFE : TIMERTC_CBEVENT_CAPRE;
        wCapValue = ptLclCtl->ptTc->COUNT16.CC[ TIMERTC_CMPCAP_CHAN0 ].reg;
        break;

      case TIMERTC_MODE_CAPPWP :
        // set the event/channel/value
        eEvent = ( ptLclCtl->ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMERTC_CBEVENT_CAPFE : TIMERTC_CBEVENT_CAPRE;
        wCapValue = ptLclCtl->ptTc->COUNT16.CC[ TIMERTC_CMPCAP_CHAN1 ].reg;
        break;

      default :
        break;
    }
  }
  else if ( tIntFlag.bit.MC1 && ptLclCtl->ptTc->COUNT16.INTENSET.bit.MC1 )
  {
    // set the channel
    eCmpCap = TIMERTC_CMPCAP_CHAN1;

    // determine the mode
    switch( ptLclCtl->tDef.eMode )
    {
      case TIMERTC_MODE_WAVNF :
      case TIMERTC_MODE_WAVMF :
      case TIMERTC_MODE_PWM :
        // set the event/channel/value
        eEvent = ( ptLclCtl->tDef.eDirection == TIMERTC_DIRECTION_UP ) ? TIMERTC_CBEVENT_CMPUP : TIMERTC_CBEVENT_CMPDN;
        break;

      case TIMERTC_MODE_CAPPPW :
        // set the event/channel/value
        eEvent = ( ptLclCtl->ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMERTC_CBEVENT_CAPRE : TIMERTC_CBEVENT_CAPFE;
        wCapValue = ptLclCtl->ptTc->COUNT16.CC[ TIMERTC_CMPCAP_CHAN1 ].reg;
        break;

      case TIMERTC_MODE_CAPPWP :
        // set the event/channel/value
        eEvent = ( ptLclCtl->ptTc->COUNT16.EVCTRL.bit.TCINV ) ? TIMERTC_CBEVENT_CAPRE : TIMERTC_CBEVENT_CAPFE;
        wCapValue = ptLclCtl->ptTc->COUNT16.CC[ TIMERTC_CMPCAP_CHAN0 ].reg;
        break;

      default :
        break;
    }
  }

  // if callback not null and event
  if (( pvCallback != NULL ) && ( eEvent != TIMERTC_CBEVENT_NONE ))
  {
    // call the callback
    pvCallback( eEvent, eCmpCap, wCapValue );
  }
}

/******************************************************************************
 * @function CongiureTcBase
 *
 * @brief TImers TC base configuration
 *
 * This function configures a TC timer for wave operation
 *
 * @param[in]   ptLclCtl     pointer to a DEF function
 *
 *****************************************************************************/
static void ConfigureTcBase( PLCLCTL ptLclCtl )
{
  TC_CTRLA_Type     tCtrlA;
  TC_CTRLBSET_Type  tCtrlB;
  TC_CTRLC_Type     tCtrlC;
  TC_EVCTRL_Type    tEvent;
  U32               uDivider;
  
  // compute the divider for the counter
  uDivider = Clock_GetPeriphClock( ptLclCtl->eMuxId ) / awTimerPrescales[ ptLclCtl->tDef.ePrescale ];
  ptLclCtl->uActClock = uDivider;
  uDivider /= ptLclCtl->tDef.awChanValues[ TIMERTC_CMPCAP_CHAN0 ];
  
  // reset it/wait for sync
  ptLclCtl->ptTc->COUNT16.CTRLA.bit.SWRST = ON;
  while(( ptLclCtl->ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );

  // store the top value in CC0
  if ( ptLclCtl->tDef.eMode == TIMERTC_MODE_PWM )
  {
    ptLclCtl->ptTc->COUNT16.CC[ 0 ].reg = 0;
    ptLclCtl->ptTc->COUNT16.CC[ 1 ].reg = 0;
  }
  else
  {
    ptLclCtl->ptTc->COUNT16.CC[ 0 ].reg = uDivider;
    ptLclCtl->ptTc->COUNT16.CC[ 1 ].reg = 0;
  }
  
  // set up CTRLA
  tCtrlA.reg = TC_CTRLA_MODE_COUNT16;
  tCtrlA.reg |= TC_CTRLA_WAVEGEN( ptLclCtl->tDef.eMode );
  tCtrlA.reg |= TC_CTRLA_PRESCALER( ptLclCtl->tDef.ePrescale );
  tCtrlA.reg |= ( ptLclCtl->tDef.tFlags.bRunStandby ) ? TC_CTRLA_RUNSTDBY : 0;
  ptLclCtl->ptTc->COUNT16.CTRLA.reg = tCtrlA.reg;
  while(( ptLclCtl->ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // set up CTLRB
  tCtrlB.reg = ( ptLclCtl->tDef.eDirection == TIMERTC_DIRECTION_DN ) ? TC_CTRLBSET_DIR : 0;
  tCtrlB.reg |= ( ptLclCtl->tDef.tFlags.bOneShot ) ? TC_CTRLBSET_ONESHOT : 0;
  ptLclCtl->ptTc->COUNT16.CTRLBSET.reg = tCtrlB.reg;
  while(( ptLclCtl->ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // set up CTRLC
  tCtrlC.reg = ( ptLclCtl->tDef.abInvertOutputs[ TIMERTC_CMPCAP_CHAN0 ] ) ? TC_CTRLC_INVEN0 : 0;
  tCtrlC.reg |= ( ptLclCtl->tDef.abInvertOutputs[ TIMERTC_CMPCAP_CHAN1 ] ) ? TC_CTRLC_INVEN1 : 0;
  ptLclCtl->ptTc->COUNT16.CTRLC.reg = tCtrlC.reg;
  while(( ptLclCtl->ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // set up the event registers
  tEvent.reg = ( ptLclCtl->tDef.tFlags.bOvfOutEnable ) ? TC_EVCTRL_OVFEO : 0;
  tEvent.reg |= ( ptLclCtl->tDef.abChanOpsEnables[ TIMERTC_CMPCAP_CHAN0 ] ) ? TC_EVCTRL_MCEO0  : 0;
  tEvent.reg |= ( ptLclCtl->tDef.abChanOpsEnables[ TIMERTC_CMPCAP_CHAN1 ] ) ? TC_EVCTRL_MCEO1  : 0;
  ptLclCtl->ptTc->COUNT16.EVCTRL.reg = tEvent.reg;
  
  // determine if there is an inverrupt
  if ( ptLclCtl->tDef.pvCallback != NULL )
  {
    // set the IRQ bits
    ptLclCtl->ptTc->COUNT16.INTENSET.bit.OVF = ptLclCtl->tDef.tFlags.bOvfIrqEnable;
    ptLclCtl->ptTc->COUNT16.INTENSET.bit.MC0 = ptLclCtl->tDef.abChanOpsEnables[ TIMERTC_CMPCAP_CHAN0 ];
    ptLclCtl->ptTc->COUNT16.INTENSET.bit.MC1 = ptLclCtl->tDef.abChanOpsEnables[ TIMERTC_CMPCAP_CHAN1 ];
    
    // check for a high priority
    if ( ptLclCtl->tDef.tFlags.bHighPriority )
    {
      // set it to a higher priority
      NVIC_SetPriority( NVIC_IRQ_BASE + ptLclCtl->tDef.eChannel, 1 );
    }

    // enable the interrupt
    NVIC_EnableIRQ( NVIC_IRQ_BASE + ptLclCtl->tDef.eChannel );
  }
  
  // now enable the control register
  if ( ptLclCtl->tDef.tFlags.bInitialOn )
  {
    ptLclCtl->ptTc->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
    while(( ptLclCtl->ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  }
}

/******************************************************************************
 * @function CongiureTcCapture
 *
 * @brief TImers 3-7 capture configuration
 *
 * This function configures a timer 3-7 for capture operation
 *
 * @param[in]   ptDef     pointer to a DEF function
 *
 *****************************************************************************/
static void ConfigureTcCapture( PLCLCTL ptLclCtl )
{
  LCLCTL            tLclCtl;
  TC_CTRLA_Type     tCtrlA;
  TC_CTRLC_Type     tCtrlC;
  TC_EVCTRL_Type    tEvent;
  TC_INTENSET_Type  tIntenSet;
  
  // reset it/wait for sync
  ptLclCtl->ptTc->COUNT16.CTRLA.bit.SWRST = ON;
  while(( ptLclCtl->ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
  
  // clear the control C registes
  tCtrlC.reg = 0;
  tCtrlC.reg |= TC_CTRLC_CPTEN0;
  ptLclCtl->ptTc->COUNT16.CTRLC.reg = tCtrlC.reg;
  while(( ptLclCtl->ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );

  // set up the event registers
  tEvent.reg = 0;
  tEvent.reg = ( ptLclCtl->tDef.tFlags.bOvfOutEnable ) ? TC_EVCTRL_OVFEO : 0;
  tEvent.reg |= TC_EVCTRL_TCEI;
  tEvent.reg |= ( ptLclCtl->tDef.tFlags.bInvertEdge ) ? TC_EVCTRL_TCINV : 0;
  ptLclCtl->ptTc->COUNT16.EVCTRL.reg = tEvent.reg;

  // clear the int set
  tIntenSet.reg = 0;
  
  // determine if there is an inverrupt
  if ( ptLclCtl->tDef.pvCallback != NULL )
  {
    // set the IRQ bits
    tIntenSet.bit.OVF = ptLclCtl->tDef.tFlags.bOvfIrqEnable;
    tIntenSet.bit.MC0 = ON;
    ptLclCtl->ptTc->COUNT16.INTENSET.reg = tIntenSet.reg;
    
    // clear all pending interrupts
    ptLclCtl->ptTc->COUNT16.INTFLAG.reg = TC_INTFLAG_MASK;

    // enable the interrupt
    NVIC_EnableIRQ( NVIC_IRQ_BASE + ptLclCtl->tDef.eChannel );
  }
  
  // set up CTRLA
  tCtrlA.reg = TC_CTRLA_MODE_COUNT16;
  tCtrlA.reg |= TC_CTRLA_PRESCALER( ptLclCtl->tDef.ePrescale );
  tCtrlA.reg |= ( ptLclCtl->tDef.tFlags.bRunStandby ) ? TC_CTRLA_RUNSTDBY : 0;
  tCtrlA.reg |= ( ptLclCtl->tDef.tFlags.bInitialOn ) ? TC_CTRLA_ENABLE : 0;
  ptLclCtl->ptTc->COUNT16.CTRLA.reg = tCtrlA.reg;
  while(( ptLclCtl->ptTc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY ) != 0 );
}

/******************************************************************************
 * @function GetTimerchannel
 *
 * @brief gets a pointer to the timer channel
 *
 * This function returns a pointer to the control structuer for a given channel
 *
 * @param[in]   eChannel    desired channel
 * @param[io]   ptLclCtl    pointer for the storage of the lcocal control
 *
 *****************************************************************************/
static void GetTimerChannel( PLCLCTL ptLclCtl )
{
  // determine the channel
  switch( ptLclCtl->tDef.eChannel )
  {
    #ifdef _SAMD11_
      case TIMERTC_CHAN_1 :
        ptLclCtl->ptTc = TC1;
        ptLclCtl->eMuxId = CLOCK_MUXID_TC12;
        ptLclCtl->uPwrMngrMask = PM_APBCMASK_TC1;
        break;

      case TIMERTC_CHAN_2 :
        ptLclCtl->ptTc = TC2;
        ptLclCtl->eMuxId = CLOCK_MUXID_TC12;
        ptLclCtl->uPwrMngrMask = PM_APBCMASK_TC2;
        break;
    #else
      #ifdef TC3
      case TIMERTC_CHAN_3 :
        ptLclCtl->ptTc = TC3;
        ptLclCtl->eMuxId = CLOCK_MUXID_TCC2TC3;
        ptLclCtl->uPwrMngrMask = PM_APBCMASK_TC3;
        break;
      #endif  // TC3

      #ifdef TC4
      case TIMERTC_CHAN_4 :
        ptLclCtl->ptTc = TC4;
        ptLclCtl->eMuxId = CLOCK_MUXID_TC45;
        ptLclCtl->uPwrMngrMask = PM_APBCMASK_TC4;
        break;
      #endif  // TC4

      #ifdef TC5
      case TIMERTC_CHAN_5 :
        ptLclCtl->ptTc = TC5;
        ptLclCtl->eMuxId = CLOCK_MUXID_TC45;
        ptLclCtl->uPwrMngrMask = PM_APBCMASK_TC5;
        break;
      #endif  // TC5

      #ifdef TC6
      case TIMERTC_CHAN_6 :
        ptLclCtl->ptTc = TC6;
        ptLclCtl->eMuxId = CLOCK_MUXID_TC67;
        ptLclCtl->uPwrMngrMask = PM_APBCMASK_TC6;
        break;
      #endif  // TC6

      #ifdef TC7
      case TIMERTC_CHAN_7 :
        ptLclCtl->ptTc = TC7;
        ptLclCtl->eMuxId = CLOCK_MUXID_TC67;
        ptLclCtl->uPwrMngrMask = PM_APBCMASK_TC7;
        break;
      #endif  // TC7
    #endif
    
    default :
      ptLclCtl->ptTc = NULL;
      ptLclCtl->eMuxId = CLOCK_MUXID_EOT;
      ptLclCtl->uPwrMngrMask = 0;
      break;
  }
}

/**@} EOF TimerTc.c */
