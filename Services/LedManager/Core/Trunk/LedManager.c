/******************************************************************************
 * @file  LedManager.c 
 *
 * @brief sound mananger implementation
 *
 * This file provides the implementation of the sound manager
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
 * $Log: $
 * 
 *
 * \addtogroup LedManager
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "LedManager/LedManager.h"

// system includes ------------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  #include <zephyr/kernel.h>
#endif // SYSTEMDEFINE_OS_ZEPHYR

// library includes -----------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )
  #include "SystemTick/SystemTick.h"
#endif // SYSTEMDEFINE_OS_TASKMANAGER
#if ( LEDMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  #include "SystemControlManager/SystemControlManager.h"
#endif  // LEDMANAGER_ENABLE_DEBUG_COMMANDS

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the play states
typedef enum _ANIMATIONSTATE
{
  ANIMATION_STATE_IDLE = 0,     ///< no animation in progress
  ANIMATION_STATE_START,        ///< start a sequence
  ANIMATION_STATE_EXEC,         ///< execute sequences
} ANIMATIONSTATE;

/// enumerate the current state
typedef enum _LEDSTATE
{
  LED_STATE_OFF = 0,          ///< led is off
  LED_STATE_ON,               ///< led is on
  LED_STATE_BLNKOFF,          ///< led is blink off
  LED_STATE_BLNKON,           ///< led is blink on
  LED_STATE_PULSE             ///< led is in pulse
} LEDSTATE;

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LEDCTL
{
  LEDSTATE  eCurState;          ///< current state
  U16       wCounts;            ///< current counts for this led
  U16       wOption;            ///< option
  U8        nIntensity;         ///< intensity
  BOOL      bNewRequest;        ///< new request
  BOOL      bAllOffOnDisabled;  ///< disable all off/on
} LEDCTL, *PLEDCTL;
#define LEDCTL_SIZE     sizeof( LEDCTL )

/// define the stack entry structure
typedef struct _SEQSTACK
{
  LEDMNGRANIMENUM eCurrentAnimation;
  U8              nAnimationIdx;
} SEQSTACK, *PSEQSTACK;
#define SEQSTACK_SIZE   sizeof( SEQSTACK )

// local parameter declarations -----------------------------------------------
static  LEDCTL          atLedCtls[ LEDMANAGER_ENUM_MAX ];
#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  static  ANIMATIONSTATE  eAnimationState;
  static  LEDMNGRANIMENUM eCurrentAnimation;
  static  U16             wAnimationOption;
  static  U8              nCurAnimationIdx;
  static  PLEDSEQENTRY    ptCurAnimation;
  static  U16             wCurrentCount;
  #if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
    static  SEQSTACK        atSeqStack[ LEDMANAGER_ANIMATION_CALLSTACK_DEPTH ];
    static  U8              nStackIndex;
  #endif
#endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
  static  U8              anColVals[ LEDMANAGER_MATRIX_MAX_NUM_ROWS ];
  static  U8              nCurScanRow;
#endif  // MATRIX DEFS

// local function prototypes --------------------------------------------------
#if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
  static  void  SetLedAction( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption, U8 nIntensity );
  static  void  ChangeLedState( PLEDDEF ptDef, BOOL bState, U8 nIntensity );
#else
  static  void  SetLedAction( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption );
  static  void  ChangeLedState( PLEDDEF ptDef, BOOL bState );
#endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS

/// command handlers
#if ( LEDMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  static  ASCCMDSTS CmdSetLed( U8 nCmdEnum );
  #if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
    static  ASCCMDSTS CmdSetRgb( U8 nCmdEnum );
  #endif  // LEDMANAGER_RGB_LEDS_ENABLED
#endif  // LEDMANAGER_ENABLE_DEBUG_COMMANDS

// constant parameter initializations -----------------------------------------
// commands
#if ( LEDMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  static  const CODE C8 szSetLed[ ]   = { "SETLED" };
    #if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
    static  const CODE C8 szSetRgb[ ]   = { "SETRGB" };
    #endif  // LEDMANAGER_RGB_LEDS_ENABLED
#endif  // LEDMANAGER_ENABLE_DEBUG_COMMANDS

#if ( LEDMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  const CODE ASCCMDENTRY g_atLedManagerCmdHandlerTable[ ] = 
  {
    #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
      ASCCMD_ENTRY( szSetLed, 6, 3, ASCFLAG_COMPARE_GE, SYSCTRLMNGR_MODE_14_DIAGNOSTICS,  CmdSetLed ),
    #else
      ASCCMD_ENTRY( szSetLed, 6, 2, ASCFLAG_COMPARE_GE, SYSCTRLMNGR_MODE_14_DIAGNOSTICS,  CmdSetLed ),
    #endif
    
    // populate command table  
    #if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
      ASCCMD_ENTRY( szSetRgb, 6, 5, ASCFLAG_COMPARE_GE, SYSCTRLMNGR_MODE_14_DIAGNOSTICS,  CmdSetRgb ),
    #endif  // LEDMANAGER_RGB_LEDS_ENABLED

    // the entry below must be here
    ASCCMD_ENDTBL( )
  };
#endif  // LEDMANAGER_ENABLE_DEBUG_COMMANDS

// define the 7 segment characters
#if ( LEDMANAGER_7SEG_LEDS_ENABLED == ON )
  const CODE U8 anLedChars[ LED_7SEGVAL_MAX ] =
  {
    0x1F, 0x06, 0x5B, 0x3F, 0x66, 0x6D, 0x7D, 0x07,
    0x7F, 0x67, 0x76, 0x38, 0x73, 0x73, 0x5C, 0x58,
    0x74, 0x50
  };
#endif // LEDMANAGER_7SEG_LEDS_ENABLED

/******************************************************************************
 * @function LedManager_Initialize
 *
 * @brief sound manager initialization
 *
 * This function will initialize the sound manager
 *
 * @return  TRUE if errors detected, FALSE if OK
 *
 *****************************************************************************/
BOOL LedManager_Initialize( void )
{
  BOOL bStatus = FALSE;

  // clear the control
  memset( &atLedCtls, 0, ( LEDCTL_SIZE * LEDMANAGER_ENUM_MAX ));

  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    // reset the stack index
    #if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
    nStackIndex = 0;
    #endif
    
    // clear the animation state
    eAnimationState = ANIMATION_STATE_IDLE;  
  #endif // SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL
  
  // call the local initialization
  bStatus = LedManager_LocalInitialize( );
  
  // return the status
  return( bStatus );
}

#if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
  /******************************************************************************
   * @function LedManager_Control
   *
   * @brief control a LED
   *
   * This function will issue a command to control an led
   *
   * @param[in]   eLedSel     LED selection
   * @param[in]   eAction     LED action
   * @param[in]   wOption     optional parameter 
   * @param[in]   nIntensity  PWM intensity
   *
   * @return      an approriate error
   * 
   *****************************************************************************/
  LEDMANAGERERR LedManager_Control( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption, U8 nIntensity )
#else
  /******************************************************************************
   * @function LedManager_Control
   *
   * @brief control a LED
   *
   * This function will issue a command to control an led
   *
   * @param[in]   eLedSel     LED selection
   * @param[in]   eAction     LED action
   * @param[in]   wOption     optional parameter 
   *
   * @return      an approriate error
   * 
   *****************************************************************************/
  LEDMANAGERERR LedManager_Control( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption )
#endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
{
  LEDMANAGERERR     eError = LEDMANAGER_ERR_NONE;
  LEDACTION         eLclAction;
  LEDMANAGERSELENUM eLclLed;
  
  // check for all leds
  if ( eLedSel == LEDMANAGER_ENUM_ALL )
  {
    // now check action
    if (( eAction >= LED_ACTION_ALLOFF ) && ( eAction < LED_ACTION_ALLMAX ))
    {
      // now for each led
      for ( eLclLed = 0; eLclLed < LEDMANAGER_ENUM_MAX; eLclLed++ )
      {
        switch( eAction )
        {
          case LED_ACTION_ALLOFF :
            eLclAction = LED_ACTION_OFF;
            break;
        
          case LED_ACTION_ALLON :
            eLclAction = LED_ACTION_ON;
            break;
        
          case LED_ACTION_ALLBLINKSLOW :
            eLclAction = LED_ACTION_BLINKSLOW;
            break;
        
          case LED_ACTION_ALLBLINKFAST :
            eLclAction = LED_ACTION_BLINKFAST;
            break;
            
          default :
            eLclAction = LED_ACTION_OFF;
            break;
        }
          
        // set the action
        #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
          SetLedAction( eLclLed, eLclAction, wOption, nIntensity );
        #else
          SetLedAction( eLclLed, eLclAction, wOption );
        #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
      }
    }
    else
    {
      // report the error
      eError = LEDMANAGER_ERR_ILLACT;
    }      
  }
  else if ( eLedSel < LEDMANAGER_ENUM_MAX )
  {
    // now check action
    if ( eAction  < LED_ACTION_MAX )
    {
      // set the action
      #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
        SetLedAction( eLclLed, eLclAction, wOption, nIntensity );
      #else
        SetLedAction( eLedSel, eAction, wOption );
      #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
    }
    else
    {
      // report the error
      eError = LEDMANAGER_ERR_ILLACT;
    }
  }
  else
  {
    // set the error
    eError = LEDMANAGER_ERR_ILLLED;
  }
  
  // return the error
  return( eError );
}

#if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
  #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
    /******************************************************************************
     * @function LedManager_RgbControl
     *
     * @brief control a RGB LED
     *
     * This function will issue a command to control an RGB led
     *
     * @param[in]   eLedSel     LED selection
     * @param[in]   eAction     LED action
     * @param[in]   eColor      LED color
     * @param[in]   wOption     option
     * @param[in]   nIntRed     intensity red
     * @param[in]   nIntGrn     intensity green
     * @param[in]   nIntBlu     intensity blue
     *
     * @return      an approriate error
     * 
     *****************************************************************************/
    LEDMANAGERERR LedManager_RgbControl( LEDMANAGERRGBENUM eLedSel, LEDACTION eAction, LEDRGBCOLOR eColor, U16 wOption, U8 nIntRed, U8 nIntGrn, U8 nIntBlu )
    {
      LEDMANAGERERR     eError = LEDMANAGER_ERR_NONE;
      LEDRGBDEF         tRgbDef;

      if ( eLedSel < LEDMANAGER_RGBENUM_MAX )
      {
        // now check action
        if ( eAction  < LED_ACTION_MAX )
        {
          // now check for color
          if ( eColor < LED_RGBCOLOR_MAX )
          {
            // get the def structure for this RGB LED
            MEMCPY_P( &tRgbDef, &g_atLedRgbDefs[ eLedSel ], LEDRGBDEF_SIZE );

            // now for each LED - set the action
            switch( eColor )
            {
              case LED_RGBCOLOR_BLK :
                // just call set local action
                SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0, 0 );
                SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0, 0 );
                SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0, 0 );
                break;

              case LED_RGBCOLOR_BLU :
                // just call set local action
                SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0, 0 );
                SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0, 0 );
                SetLedAction( tRgbDef.nBluEnum, eAction, wOption, nIntBlu );
                break;

              case LED_RGBCOLOR_GRN :
                // just call set local action
                SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0, 0 );
                SetLedAction( tRgbDef.nGrnEnum, eAction, wOption, nIntGrn );
                SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0, 0 );
                break;

              case LED_RGBCOLOR_CYN :
                SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0, 0 );
                SetLedAction( tRgbDef.nGrnEnum, eAction, wOption, nIntGrn );
                SetLedAction( tRgbDef.nBluEnum, eAction, wOption, nIntBlu );
                break;

              case LED_RGBCOLOR_RED :
                SetLedAction( tRgbDef.nRedEnum, eAction, wOption, nIntRed );
                SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0, 0 );
                SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0, 0 );
                break;

              case LED_RGBCOLOR_VIO :
                SetLedAction( tRgbDef.nRedEnum, eAction, wOption, nIntRed );
                SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0, 0 );
                SetLedAction( tRgbDef.nBluEnum, eAction, wOption, nIntBlu );
                break;

              case LED_RGBCOLOR_ORN :
                SetLedAction( tRgbDef.nRedEnum, eAction, wOption, nIntRed );
                SetLedAction( tRgbDef.nGrnEnum, eAction, wOption, nIntGrn );
                SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0, 0 );
                break;

              case LED_RGBCOLOR_WHT :
                SetLedAction( tRgbDef.nRedEnum, eAction, wOption, nIntRed );
                SetLedAction( tRgbDef.nGrnEnum, eAction, wOption, nIntGrn );
                SetLedAction( tRgbDef.nBluEnum, eAction, wOption, nIntBlu );
                break;

              default :
                break;
            }
          }
          else
          {
            // report the error
            eError = LEDMANAGER_ERR_ILLCLR;
          }
        }
        else
        {
          // report the error
          eError = LEDMANAGER_ERR_ILLACT;
        }
      }
      else
      {
        // set the error
        eError = LEDMANAGER_ERR_ILLLED;
      }
  
      // return the error
      return( eError );
    }
  #else
    /******************************************************************************
     * @function LedManager_RgbControl
     *
     * @brief control a RGB LED
     *
     * This function will issue a command to control an RGB led
     *
     * @param[in]   eLedSel     LED selection
     * @param[in]   eAction     LED action
     * @param[in]   eColor      LED color
     * @param[in]   wOption     option
     *
     * @return      an approriate error
     * 
     *****************************************************************************/
    LEDMANAGERERR LedManager_RgbControl( LEDMANAGERRGBENUM eLedSel, LEDACTION eAction, LEDRGBCOLOR eColor, U16 wOption )
    {
      LEDMANAGERERR     eError = LEDMANAGER_ERR_NONE;
      LEDRGBDEF         tRgbDef;

      if ( eLedSel < LEDMANAGER_RGBENUM_MAX )
      {
        // now check action
        if ( eAction  < LED_ACTION_MAX )
        {
          // now check for color
          if ( eColor < LED_RGBCOLOR_MAX )
          {
            // get the def structure for this RGB LED
            MEMCPY_P( &tRgbDef, &g_atLedRgbDefs[ eLedSel ], LEDRGBDEF_SIZE );

            // now for each LED - set the action
            switch( eColor )
            {
              case LED_RGBCOLOR_BLK :
                // just call set local action
                SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0 );
                SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0 );
                SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0 );
                break;

              case LED_RGBCOLOR_BLU :
                // just call set local action
                SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0 );
                SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0 );
                SetLedAction( tRgbDef.nBluEnum, eAction, wOption );
                break;

              case LED_RGBCOLOR_GRN :
                // just call set local action
                SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0 );
                SetLedAction( tRgbDef.nGrnEnum, eAction, wOption );
                SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0 );
                break;

              case LED_RGBCOLOR_CYN :
                SetLedAction( tRgbDef.nRedEnum, LED_ACTION_OFF, 0 );
                SetLedAction( tRgbDef.nGrnEnum, eAction, wOption );
                SetLedAction( tRgbDef.nBluEnum, eAction, wOption );
                break;

              case LED_RGBCOLOR_RED :
                SetLedAction( tRgbDef.nRedEnum, eAction, wOption );
                SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0 );
                SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0 );
                break;

              case LED_RGBCOLOR_VIO :
                SetLedAction( tRgbDef.nRedEnum, eAction, wOption );
                SetLedAction( tRgbDef.nGrnEnum, LED_ACTION_OFF, 0 );
                SetLedAction( tRgbDef.nBluEnum, eAction, wOption );
                break;

              case LED_RGBCOLOR_ORN :
                SetLedAction( tRgbDef.nRedEnum, eAction, wOption );
                SetLedAction( tRgbDef.nGrnEnum, eAction, wOption );
                SetLedAction( tRgbDef.nBluEnum, LED_ACTION_OFF, 0 );
                break;

              case LED_RGBCOLOR_WHT :
                SetLedAction( tRgbDef.nRedEnum, eAction, wOption );
                SetLedAction( tRgbDef.nGrnEnum, eAction, wOption );
                SetLedAction( tRgbDef.nBluEnum, eAction, wOption );
                break;

              default :
                break;
            }
          }
          else
          {
            // report the error
            eError = LEDMANAGER_ERR_ILLCLR;
          }
        }
        else
        {
          // report the error
          eError = LEDMANAGER_ERR_ILLACT;
        }
      }
      else
      {
        // set the error
        eError = LEDMANAGER_ERR_ILLLED;
      }
  
      // return the error
      return( eError );
    }
  #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

#if ( LEDMANAGER_7SEG_LEDS_ENABLED == ON )
  /******************************************************************************
   * @function LedManager_7SegControl
   *
   * @brief control a 7 segment led
   *
   * This function will issue a command to control a 7 segment led
   *
   * @param[in]   eLedSel     LED selection
   * @param[in]   eAction     LED action
   * @param[in]   eValue      LED value
   * @param[in]   bDecimal    decimal enable
   * @param[in]   wOption     option
   *
   * @return      an approriate error
   * 
   *****************************************************************************/
  LEDMANAGERERR LedManager_7SegControl( LEDMANAGER7SEGENUM eLedSel, LEDACTION eAction, LED7SEGVALENUM eValue, BOOL bDecimal, U16 wOption )
  {
    LEDMANAGERERR eError = LEDMANAGER_ERR_NONE;
    LED7SEGDEF    t7SegDef;
    U8            nMasks;

    // check for a special LED
    if (( eLedSel >= LED_7SEGVAL_SPC ) || ( eLedSel < LEDMANAGER_7SEGENUM_MAX ))
    {
      // now check action
      if ( eAction  < LED_ACTION_MAX )
      {
        // get the def structure for this RGB LED
        MEMCPY_P( &t7SegDef, &g_atLeds7SegDef[ eLedSel ], LED7SEGDEF_SIZE );

        // now check for special
        if ( eLedSel >= LED_7SEGVAL_SPC )
        {
          // get the masks
          nMasks = eLedSel & 0x7F;
        }
        else
        {
          nMasks = anLedChars[ eLedSel ];
        }

        // set the bits
        SetLedAction( t7SegDef.nSegAEnum, ( nMasks & BIT( 0 )) ? eAction : LED_ACTION_OFF, wOption );
        SetLedAction( t7SegDef.nSegBEnum, ( nMasks & BIT( 1 )) ? eAction : LED_ACTION_OFF, wOption );
        SetLedAction( t7SegDef.nSegCEnum, ( nMasks & BIT( 2 )) ? eAction : LED_ACTION_OFF, wOption );
        SetLedAction( t7SegDef.nSegDEnum, ( nMasks & BIT( 3 )) ? eAction : LED_ACTION_OFF, wOption );
        SetLedAction( t7SegDef.nSegEEnum, ( nMasks & BIT( 4 )) ? eAction : LED_ACTION_OFF, wOption );
        SetLedAction( t7SegDef.nSegFEnum, ( nMasks & BIT( 5 )) ? eAction : LED_ACTION_OFF, wOption );
        SetLedAction( t7SegDef.nSegGEnum, ( nMasks & BIT( 6 )) ? eAction : LED_ACTION_OFF, wOption );
        SetLedAction( t7SegDef.nSegDpEnum, ( bDecimal ) ? eAction : LED_ACTION_OFF, wOption );
      }
      else
      {
        // report the error
        eError = LEDMANAGER_ERR_ILLACT;
      }
    }
    else
    {
      // set the error
      eError = LEDMANAGER_ERR_ILLLED;
    }

    // return the error
    return( eError );
  }
#endif // LEDMANAGER_7SEG_LEDS_ENABLED 

#if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  /******************************************************************************
   * @function LedManager_PlayAnimation
   *
   * @brief play an animation
   *
   * This function will play a led animation
   *
   * @param[in]   eLedSeq   led animation
   *
   * @return      an approriate error
   * 
   *****************************************************************************/
  LEDMANAGERERR LedManager_PlayAnimation( LEDMNGRANIMENUM eAnimation, U16 wOption )
  {
    LEDMANAGERERR     eError = LEDMANAGER_ERR_NONE;

    // check to see if this is a stop request
    if ( eAnimation == LEDMNGR_ANIMATION_STOP )
    {
      // stop it
      eAnimationState = ANIMATION_STATE_IDLE;

      #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
        // clear all leds
        LedManager_Control( LEDMANAGER_ENUM_ALL, LED_ACTION_ALLOFF, 0, 0 );
      #else
        // clear all leds
        LedManager_Control( LEDMANAGER_ENUM_ALL, LED_ACTION_ALLOFF, 0 );
      #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
    }
    else 
    {
      // check for a valid animation
      if ( eAnimation < LEDMNGR_ANIMATION_MAX )
      {
        // if an animation is currently running, turn it off
        if ( eAnimationState != ANIMATION_STATE_IDLE )
        {
          // stop it
          eAnimationState = ANIMATION_STATE_IDLE;
        }
    
        // reset the index/set the current animation/store the option
        eCurrentAnimation = eAnimation;
        wAnimationOption = wOption;
    
        // start the sequence
        eAnimationState = ANIMATION_STATE_START;
      }
      else
      {
        // set the error
        eError = LEDMANAGER_ERR_ILLANI;
      }
    }
  
    // return the error
    return( eError );
  }
#endif // ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )

/******************************************************************************
 * @function LedManager_ProcessAnimation
 *
 * @brief sound manager task handler
 *
 * This function processes the led animiation task
 *
 * @return      TRUE    pop the event of the stack
 *****************************************************************************/
void LedManager_ProcessAnimation( void )
{
  BOOL              bRunFlag;
  LEDMANAGERSELENUM eLed;
  PLEDCTL           ptCtl;
  LEDDEF            tDef;
  U8                nIntensity = 0;
  
  #if ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    LEDSEQENTRY       tSequence;

    // run till done
    do
    {
      // check to see if there is an animation running
      switch( eAnimationState )
      {
        case ANIMATION_STATE_IDLE :
          // clear the run flag
          bRunFlag = FALSE;
          break;
      
        case ANIMATION_STATE_START :
          // get the current entry
          ptCurAnimation = ( PLEDSEQENTRY )PGM_RDWORD( g_apLedAnimationsDef[ eCurrentAnimation ] );

          // process the first event/switch to execution state
          eAnimationState = ANIMATION_STATE_EXEC;
          nCurAnimationIdx = 0;
          wCurrentCount = 1;
          break;
        
        case ANIMATION_STATE_EXEC :
          // decrement the count and test for next state
          if ( --wCurrentCount == 0 )
          {
            // get and act on the first entry
            MEMCPY_P( &tSequence, &ptCurAnimation[ nCurAnimationIdx++ ], LEDSEQENTRY_SIZE );
            if ( tSequence.eEvent != LED_SEQEVENT_CALL )
            {
              // check for RGB
              if ( tSequence.bRgbFlag )
              {
                // process it as a RGB
                #if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
                  #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
                    LedManager_RgbControl( tSequence.nLedEnum, tSequence.eAction, tSequence.eRgbColor, tSequence.wOption, tSequence.nIntRed, tSequence.nIntGrn, tSequence.nIntBlu );
                  #else
                    LedManager_RgbControl( tSequence.nLedEnum, tSequence.eAction, tSequence.eRgbColor, tSequence.wOption );
                  #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
                #endif  // LEDMANAGER_RGB_LEDS_ENABLED
              }
              else
              {
                #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
                  // process it as a single
                  LedManager_Control( tSequence.nLedEnum, tSequence.eAction, tSequence.wOption, tSequence.nIntRed );
                #else
                  // process it as a single
                  LedManager_Control( tSequence.nLedEnum, tSequence.eAction, tSequence.wOption );
                #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
              }
            }
      
            // determine the next state
            switch( tSequence.eEvent )
            {
              case LED_SEQEVENT_JMPBEG :
                // set the index back to 0
                nCurAnimationIdx = 0;

                // force a reloop
                wCurrentCount = 1;
                break;
              
              case LED_SEQEVENT_JMPSEQ :
                // set the index to the passed option
                nCurAnimationIdx = tSequence.wOption;

                // force a reloop
                wCurrentCount = 1;
                break;
              
              case LED_SEQEVENT_DONE :
                // test for a return from a call
                #if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
                  if ( nStackIndex != 0 )
                  {
  
                    // get the value from the stack
                    eCurrentAnimation = atSeqStack[ --nStackIndex ].eCurrentAnimation;
                    nCurAnimationIdx = atSeqStack[ nStackIndex ].nAnimationIdx;

                    // get the current entry
                    ptCurAnimation = ( PLEDSEQENTRY )PGM_RDWORD( g_apLedAnimationsDef[ eCurrentAnimation ] );

                    // force a reloop
                    wCurrentCount = 1;
                  } 
                  else
                  {
                    // turn off the animation
                    eAnimationState = ANIMATION_STATE_IDLE;
                
                    // post an event
                    LedManager_NotifyDone( );
                  }
                #else
                  // turn off the animation
                  eAnimationState = ANIMATION_STATE_IDLE;
                
                  // post an event
                  LedManager_NotifyDone( );
                #endif
              
                // exit loop
                bRunFlag = FALSE;
                break;
                          
              case LED_SEQEVENT_WAIT :
                // now set the delay - goto to wait
                if ( wAnimationOption != 0 )
                {
                  // use passed time
                  wCurrentCount = wAnimationOption / LEDMANAGER_ANIMATE_RATE_MSECS;
                }
                else
                {
                  // use default time
                  wCurrentCount = tSequence.wDurationMsecs / LEDMANAGER_ANIMATE_RATE_MSECS;
                }
              
                // exit loop
                bRunFlag = FALSE;
                break;
              
              case LED_SEQEVENT_NEXT :    // check for PWM
                // force a reloop
                wCurrentCount = 1;
                break;
              
              #if ( LEDMANAGER_ANIMATION_CALLSTACK_DEPTH != 0 )
                case LED_SEQEVENT_CALL :
                  // test for room on stack
                  if ( nStackIndex < LEDMANAGER_ANIMATION_CALLSTACK_DEPTH )
                  {
                    // stuff the current index
                    atSeqStack[ nStackIndex ].eCurrentAnimation = eCurrentAnimation;
                    atSeqStack[ nStackIndex++ ].nAnimationIdx = nCurAnimationIdx;
                  }
                
                  // set the new animation
                  eCurrentAnimation = tSequence.wOption;
                  nCurAnimationIdx = 0;

                  // get the current entry
                  ptCurAnimation = ( PLEDSEQENTRY )PGM_RDWORD( g_apLedAnimationsDef[ eCurrentAnimation ] );
              
                  // force a reloop
                  wCurrentCount = 1;
                break;
              #endif
         
              default :
                break;
            }
          }
          break;
        
        default :
          break;
      }
    } while( bRunFlag );
  #endif // ( SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  
  // process the led's based on their action
  for ( eLed = 0; eLed < LEDMANAGER_ENUM_MAX; eLed++ )
  {
    // get a pointer to the control
    ptCtl = &atLedCtls[ eLed ];

    // copy the control structure
    MEMCPY_P( &tDef, &g_atLedDefs[ eLed ], LEDDEF_SIZE );

    // check for PWM
    if ( tDef.eDriveType == LED_DRIVETYPE_PWM )
    {
      // set the intensity
      nIntensity = ptCtl->nIntensity;
    } 

    // process the state
    switch( ptCtl->eCurState )
    {
      case LED_STATE_OFF :
        // turn off the GPIO
        #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
          ChangeLedState( &tDef, OFF, 0 );
        #else
          ChangeLedState( &tDef, OFF );
        #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
        break;
        
      case LED_STATE_ON :
        #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
          ChangeLedState( &tDef, ON, nIntensity );
        #else
          ChangeLedState( &tDef, ON );
        #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
        break;
        
      case LED_STATE_BLNKOFF :
        // decrement count if not zero
        if ( ptCtl->wCounts == 0 )
        {
          // set the state to on
          #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
            ChangeLedState( &tDef, ON, nIntensity );
          #else
            ChangeLedState( &tDef, ON );
          #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
          ptCtl->wCounts = ptCtl->wOption;
          ptCtl->eCurState = LED_STATE_BLNKON;
        }
        else
        {
          // decrement the count
          ptCtl->wCounts--;
        }
        break;
        
      case LED_STATE_BLNKON :
        // decrement count if not zero
        if ( ptCtl->wCounts == 0 )
        {
          // turn off the GPIO
          #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
            ChangeLedState( &tDef, OFF, 0 );
          #else
            ChangeLedState( &tDef, OFF );
          #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
          ptCtl->wCounts = ptCtl->wOption;
          ptCtl->eCurState = LED_STATE_BLNKOFF;
        }
        else
        {
          // decrement the count
          ptCtl->wCounts--;
        }
        break;
        
      case LED_STATE_PULSE :
        // decrement count and test for zero
        if ( --ptCtl->wCounts == 0 )
        {
          // turn off the led/set the state to off
          #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
            ChangeLedState( &tDef, OFF, 0 );
          #else
            ChangeLedState( &tDef, OFF );
          #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
          ptCtl->eCurState = LED_STATE_OFF;
        }
        else
        {
          // refresh it to on
          #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
            ChangeLedState( &tDef, ON, nIntensity );
          #else
            ChangeLedState( &tDef, ON );
          #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
        }
        break;
        
      default :
        break;
    }
  }
}

#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
/******************************************************************************
 * @function LedManager_ScanTask
 *
 * @brief led manager scan task handler
 *
 * This function processes scanning the LED matrix
 *
 *****************************************************************************/
void LedManager_ProcessScan( void )
{
  GPIOPINENUM ePin;
  U8          nIdx;
  BOOL        bColsEnabled = FALSE;
  BOOL        bColEnb;
  
  // now turn off the row
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
    ePin = g_aeLedMatrixRows[ nCurScanRow ];
  #else
    ePin = PGM_RDBYTE( g_aeLedMatrixRows[ nCurScanRow ] );
  #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL
  LedManager_GpioSet( ePin, !LEDMANAGER_MATRIX_ROW_ACTIVE_LEVEL );

  // now turn off all columns in this row
  for ( nIdx = 0; nIdx < LEDMANAGER_MATRIX_MAX_NUM_COLS; nIdx++ )
  {
    // get the pin
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
      ePin = g_aeLedMatrixCols[ nIdx ];
    #else
      ePin = PGM_RDBYTE( g_aeLedMatrixCols[ nIdx ] );
    #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL
    LedManager_GpioSet( ePin, !LEDMANAGER_MATRIX_COL_ACTIVE_LEVEL );
  }

  // increment the row
  nCurScanRow++;
  nCurScanRow %= LEDMANAGER_MATRIX_MAX_NUM_ROWS;
  
  // now set the columns for this row
  for ( nIdx = 0; nIdx < LEDMANAGER_MATRIX_MAX_NUM_COLS; nIdx++ )
  {
    // is this column on
    bColEnb = anColVals[ nCurScanRow ] & BIT( nIdx );
    bColsEnabled |= bColEnb;
    if ( bColEnb )
    {
      // turn on this column
      // get the pin
      #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
        ePin = g_aeLedMatrixCols[ nIdx ];
      #else
        ePin = PGM_RDBYTE( g_aeLedMatrixCols[ nIdx ] );
      #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL
      LedManager_GpioSet( ePin, LEDMANAGER_MATRIX_COL_ACTIVE_LEVEL );
    }
  }
  
  // now turn on the row
  if ( bColsEnabled )
  {
    // get the pin
    #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL )
    ePin = g_aeLedMatrixRows[ nCurScanRow ];
    #else
    ePin = PGM_RDBYTE( g_aeLedMatrixRows[ nCurScanRow ] );
    #endif // SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_MINIMAL
    LedManager_GpioSet( ePin, LEDMANAGER_MATRIX_ROW_ACTIVE_LEVEL );
  }
}
#endif  // MATRIX DEFS

#if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
  /******************************************************************************
   * @function SetLedAction
   *
   * @brief set the led action
   *
   * This function will set the action for a given led
   *
   * @param[in]   eLedSel     LED selection
   * @param[in]   eAction     LED action
   * @param[in]   wOption     optional parameter
   * @param[in]   nIntensity  intensity
   *
   *****************************************************************************/
  static void SetLedAction( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption, U8 nIntensity )
  {
    PLEDCTL       ptCtl;
  
    // get a pointer to the control
    ptCtl = &atLedCtls[ eLedSel ];
    
    // process the action
    switch( eAction )
    {
      case LED_ACTION_OFF :
        if ( ptCtl->bAllOffOnDisabled == FALSE )
        {
          // set the state off
          ptCtl->eCurState = LED_STATE_OFF;
          ptCtl->nIntensity = 0;

        }
        break;
      
      case LED_ACTION_ON :
        if ( ptCtl->bAllOffOnDisabled == FALSE )
        {
          // set the state to on
          ptCtl->eCurState = LED_STATE_ON;
          ptCtl->nIntensity = nIntensity;
        }
        break;
      
      case LED_ACTION_BLINKSLOW :
        // set the state to blink off
        ptCtl->eCurState = LED_STATE_BLNKOFF;
        ptCtl->wOption = LEDMANAGER_FLASH_SLOW_TIME / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->wCounts = 0;
        ptCtl->nIntensity = nIntensity;
        ptCtl->bAllOffOnDisabled = FALSE;
        break;
      
      case LED_ACTION_BLINKFAST :
        // set the state to blink on
        ptCtl->eCurState = LED_STATE_BLNKOFF;
        ptCtl->wOption = LEDMANAGER_FLASH_FAST_TIME / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->wCounts = 0;
        ptCtl->nIntensity = nIntensity;
        ptCtl->bAllOffOnDisabled = FALSE;
        break;
      
      case LED_ACTION_PULSE :
        // set the state to pulse
        ptCtl->eCurState = LED_STATE_PULSE;
        ptCtl->wCounts = wOption  / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->bAllOffOnDisabled = FALSE;
        ptCtl->nIntensity = nIntensity;
        break;

      case LED_ACTION_BLINKSLOW_LOCK :
        // set the state to blink off
        ptCtl->eCurState = LED_STATE_BLNKOFF;
        ptCtl->wOption = LEDMANAGER_FLASH_SLOW_TIME / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->wCounts = 0;
        ptCtl->nIntensity = nIntensity;
        ptCtl->bAllOffOnDisabled = TRUE;
        break;
      
      case LED_ACTION_BLINKFAST_LOCK :
        // set the state to blink on
        ptCtl->eCurState = LED_STATE_BLNKOFF;
        ptCtl->wOption = LEDMANAGER_FLASH_FAST_TIME / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->wCounts = 0;
        ptCtl->nIntensity = nIntensity;
        ptCtl->bAllOffOnDisabled = TRUE;
        break;
      
      case LED_ACTION_PULSE_LOCK :
        // set the state to pulse
        ptCtl->eCurState = LED_STATE_PULSE;
        ptCtl->wCounts = wOption / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->nIntensity = nIntensity;
        ptCtl->bAllOffOnDisabled = TRUE;
        break;

      default :
        break;
    }
  }

  /******************************************************************************
   * @function ChangeLedState
   *
   * @brief change the state of an led
   *
   * This function will change the state of the LED
   *
   * @param[in]   ptDef       pointer to the definition structure
   * @param[in]   bState      state of the pin if direct
   * @param[in]   nIntensity  intensity of the PWM
   *
   *****************************************************************************/
  static void ChangeLedState(PLEDDEF ptDef, BOOL bState, U8 nIntensity)
  {
    // determine led type
    switch( ptDef->eDriveType )
    {
      case LED_DRIVETYPE_DIRECT :
        LedManager_GpioSet( ptDef->tRowDrive.eDrivePin, bState );
        break;
      
      #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
      case LED_DRIVETYPE_MATRIX :
        // determine the action
        if (( nIntensity != 0  ) ^ ( LEDMANAGER_MATRIX_COL_ACTIVE_LEVEL == HI ))
        {
          // clear the bit
          anColVals[ ptDef->tRowDrive.nRowIndex ] &= ~BIT( ptDef->nColIndex );
        }
        else
        {
          // set the bit
          anColVals[ ptDef->tRowDrive.nRowIndex ] |= BIT( ptDef->nColIndex );
        }
        break;
      #endif
    
      case LED_DRIVETYPE_SPECIAL :
        ptDef->tRowDrive.pvSpclFunc( ptDef->nColIndex, bState );
        break;

      case LED_DRIVETYPE_PWM :
        LedManager_PwmSet( ptDef->tRowDrive.eDrivePin, nIntensity );
        break;
    
      default :
        break;
    }
  }
#else
  /******************************************************************************
   * @function SetLedAction
   *
   * @brief set the led action
   *
   * This function will set the action for a given led
   *
   * @param[in]   eLedSel     LED selection
   * @param[in]   eAction     LED action
   * @param[in]   wOption     optional parameter
   *
   *****************************************************************************/
  static void SetLedAction( LEDMANAGERSELENUM eLedSel, LEDACTION eAction, U16 wOption )
  {
    PLEDCTL       ptCtl;
  
    // get a pointer to the control
    ptCtl = &atLedCtls[ eLedSel ];
    
    // process the action
    switch( eAction )
    {
      case LED_ACTION_OFF :
        if ( ptCtl->bAllOffOnDisabled == FALSE )
        {
          // set the state off
          ptCtl->eCurState = LED_STATE_OFF;

        }
        break;
      
      case LED_ACTION_ON :
        if ( ptCtl->bAllOffOnDisabled == FALSE )
        {
          // set the state to on
          ptCtl->eCurState = LED_STATE_ON;
        }
        break;
      
      case LED_ACTION_BLINKSLOW :
        // set the state to blink off
        ptCtl->eCurState = LED_STATE_BLNKOFF;
        ptCtl->wOption = LEDMANAGER_FLASH_SLOW_TIME / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->wCounts = 0;
        ptCtl->bAllOffOnDisabled = FALSE;
        break;
      
      case LED_ACTION_BLINKFAST :
        // set the state to blink on
        ptCtl->eCurState = LED_STATE_BLNKOFF;
        ptCtl->wOption = LEDMANAGER_FLASH_FAST_TIME / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->wCounts = 0;
        ptCtl->bAllOffOnDisabled = FALSE;
        break;
      
      case LED_ACTION_PULSE :
        // set the state to pulse
        ptCtl->eCurState = LED_STATE_PULSE;
        ptCtl->wCounts = wOption  / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->bAllOffOnDisabled = FALSE;
        break;

      case LED_ACTION_BLINKSLOW_LOCK :
        // set the state to blink off
        ptCtl->eCurState = LED_STATE_BLNKOFF;
        ptCtl->wOption = LEDMANAGER_FLASH_SLOW_TIME / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->wCounts = 0;
        ptCtl->bAllOffOnDisabled = TRUE;
        break;
      
      case LED_ACTION_BLINKFAST_LOCK :
        // set the state to blink on
        ptCtl->eCurState = LED_STATE_BLNKOFF;
        ptCtl->wOption = LEDMANAGER_FLASH_FAST_TIME / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->wCounts = 0;
        ptCtl->bAllOffOnDisabled = TRUE;
        break;
      
      case LED_ACTION_PULSE_LOCK :
        // set the state to pulse
        ptCtl->eCurState = LED_STATE_PULSE;
        ptCtl->wCounts = wOption / LEDMANAGER_ANIMATE_RATE_MSECS;
        ptCtl->bAllOffOnDisabled = TRUE;
        break;

      default :
        break;
    }
  }

  /******************************************************************************
   * @function ChangeLedState
   *
   * @brief change the state of an led
   *
   * This function will change the state of the LED
   *
   * @param[in]   ptDef       pointer to the definition structure
   * @param[in]   bState      state of the pin if direct
   *
   *****************************************************************************/
  static void ChangeLedState(PLEDDEF ptDef, BOOL bState)
  {
    // determine led type
    switch( ptDef->eDriveType )
    {
      case LED_DRIVETYPE_DIRECT :
        LedManager_GpioSet( ptDef->tRowDrive.eDrivePin, bState );
        break;
      
      #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
      case LED_DRIVETYPE_MATRIX :
        // determine the action
        if (( nIntensity != 0  ) ^ ( LEDMANAGER_MATRIX_COL_ACTIVE_LEVEL == HI ))
        {
          // clear the bit
          anColVals[ ptDef->tRowDrive.nRowIndex ] &= ~BIT( ptDef->nColIndex );
        }
        else
        {
          // set the bit
          anColVals[ ptDef->tRowDrive.nRowIndex ] |= BIT( ptDef->nColIndex );
        }
        break;
      #endif
    
      case LED_DRIVETYPE_SPECIAL :
        ptDef->tRowDrive.pvSpclFunc( ptDef->nColIndex, bState );
        break;

      default :
        break;
    }
  }
#endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS

#if ( LEDMANAGER_ENABLE_DEBUG_COMMANDS == 1 )
  /******************************************************************************
   * @function CmdSetLed
   *
   * @brief set an led to a state
   *
   * This function sets an LED to an appropriate state
   *
   * @param[in]   nCmdEnum    command handler enumeration
   *
   * @return  appropriate status
   *****************************************************************************/
  static  ASCCMDSTS CmdSetLed( U8 nCmdEnum )
  {
    ASCCMDSTS         eStatus = ASCCMD_STS_NONE;
    LEDMANAGERERR     eError;
    LEDMANAGERSELENUM eLedSel;
    LEDACTION         eAction; 
    U32UN             tValue;
    PC8               pcBuffer;
    
    // get the led selection/action
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );
    eLedSel = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 1, &tValue.uValue );
    eAction = tValue.anValue[ LE_U32_LSB_IDX ];
    #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
      AsciiCommandHandler_GetValue( nCmdEnum, 2, &tValue.uValue );
    #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
    
    // set the led
    #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
      eError = LedManager_Control( eLedSel, eLedAction, 0, tValue.anValue[ LE_U32_LSB_IDX ] );
    #else
      eError = LedManager_Control( eLedSel, eAction, 0 );
    #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
    if ( eError != LEDMANAGER_ERR_NONE )
    {
      // get the buffer
      AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );
      
      // report the error
      sprintf( pcBuffer, g_szAsciiErrStrn, eError, eError );
      eStatus = ASCCMD_STS_OUTPUTBUFFER;
    } 
    
    // return the status
    return( eStatus );
  }

  #if ( LEDMANAGER_RGB_LEDS_ENABLED == 1 )
    static  ASCCMDSTS CmdSetRgb( U8 nCmdEnum )
    {
      ASCCMDSTS         eStatus = ASCCMD_STS_NONE;
      LEDMANAGERERR     eError;
      LEDMANAGERSELENUM eLedSel;
      LEDACTION         eAction;
      U32UN             tValue;
      PC8               pcBuffer;
      #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
        U8                nIntRed, nIntGrn, nIntBlu;
      #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS

      // get the led selection/action
      AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );
      eLedSel = tValue.anValue[ LE_U32_LSB_IDX ];
      AsciiCommandHandler_GetValue( nCmdEnum, 1, &tValue.uValue );
      eAction = tValue.anValue[ LE_U32_LSB_IDX ];
      AsciiCommandHandler_GetValue( nCmdEnum, 2, &tValue.uValue );
      #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
        AsciiCommandHandler_GetValue( nCmdEnum, 3, &tValue.uValue );
        nIntRed = tValue.anValue[ LE_U32_LSB_IDX ];
        AsciiCommandHandler_GetValue( nCmdEnum, 4, &tValue.uValue );
        nIntGrn = tValue.anValue[ LE_U32_LSB_IDX ];
        AsciiCommandHandler_GetValue( nCmdEnum, 52, &tValue.uValue );
        nIntBlu = tValue.anValue[ LE_U32_LSB_IDX ];
      #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
      
      // set the led  
      #if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
        eError = LedManager_Control( eLedSel, eLedAction, 0, tValue.anValue[ LE_U32_LSB_IDX ], 0, nIntRed, nIntGrn, nIntBlu );
      #else
        eError = LedManager_RgbControl( eLedSel, eAction, ( LEDRGBCOLOR )tValue.anValue[ LE_U32_LSB_IDX ], 0 );
      #endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS
      if ( eError != LEDMANAGER_ERR_NONE )
      {
        // get the buffer
        AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );
        
        // report the error
        sprintf( pcBuffer, g_szAsciiErrStrn, eError, eError );
        eStatus = ASCCMD_STS_OUTPUTBUFFER;
      }
      
      // return the status
      return( eStatus );
    }
    #endif  // LEDMANAGER_RGB_LEDS_ENABLED
#endif  // LEDMANAGER_ENABLE_DEBUG_COMMANDS

/**@} EOF LedManager.c */
