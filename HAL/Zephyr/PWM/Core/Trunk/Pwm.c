/******************************************************************************
 * @file Pwm.c
 *
 * @brief PWM HAL implementation
 *
 * This file provides the implementation for the PWM HAL
 *
 * @copyright Copyright (c) 2021 Strongarm Tech Inc
 * The copyright to the computer software herein is the property of 
 * Strongarm Tech Inc. This software may not be used and/or copied without
 * explicit written permission of Strongarm Tech Inc., and only in accordance
 * with the terms and conditions stipulated in the license agreement and/or
 * contract under which the software has been supplied.
 *
 * Version History
 * ======
 *
 * \addtogroup PWM
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include <zephyr.h>
#include <soc.h>

// local includes -------------------------------------------------------------
#include "PWM/Pwm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the control structure for the GPIO pins
typedef struct _PWMCTL
{
  const struct device *ptDevice;
  U32                 uFlags;
  U32                 uFreq;
} PWMCTL, *PPWMCTL;
#define PWMCTL_SIZE                            sizeof( PWMCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PWMCTL atPwmCtls[ PWM_ENUM_MAX ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Pwm_Initialize
 *
 * @brief initialization
 *
 * This function will initialize each PWM 
 *
 *****************************************************************************/
void Pwm_Initialize( void )
{
  PPWMDEF   ptPwmDef;
  PPWMCTL   ptPwmCtl;
  U8        nIdx;

  Pwm_LocalInitialize();

  // for each pin def in the list
  for( nIdx = 0; nIdx < PWM_ENUM_MAX; nIdx++ )
  {
    // // get a pointer to the definition
    // ptPwmDef = ( PPWMDEF )&g_atPwmDefs[ nIdx ];
    // ptPwmCtl = &atPwmCtls[ nIdx ];

    // // get the device
    // ptPwmCtl->ptDevice = device_get_binding( ptPwmDef->pcPort );

    // // set the mode
    // ptPwmCtl->uFlags = ( ptPwmDef->bInvert ) ? PWM_POLARITY_INVERTED : PWM_POLARITY_NORMAL;
    
    // // set the frequency
    // ptPwmCtl->uFreq = ptPwmDef->uFreqCps;

    // // configure the output
    // pwm_pin_set_cycles( ptPwmCtl->ptDevice, ptPwmDef->nChannel, ptPwmDef->uFreqCps, 0, ptPwmCtl->uFlags );
  }      
}

/******************************************************************************
 * @function Pwm_Close
 *
 * @brief close all PWMS
 *
 * This function will close all PWMS
 *
 *****************************************************************************/
void Pwm_Close( void )
{

}

/******************************************************************************
 * @function Pwm_PulseWidthPercent
 *
 * @brief set the pulse to a percent of the period
 *
 * This function will set the pulse width 
 *
 * @param[in]   ePwmEnum    PWM channel enumeration
 * @param[in]   nPercent    percent of period
 *
 * @return      appropriate error
 *
 *****************************************************************************/
PWMERR Pwm_PulseWidthPercent( PWMENUM ePwmEnum, U8 nPercent )
{
  PWMERR  eError = PWM_ERR_NONE;
  PPWMDEF ptPwmDef;
  PPWMCTL ptPwmCtl;
  U32     uNewPwmWidth;
  
  // test for valid PWM enumeration
  if ( ePwmEnum < PWM_ENUM_MAX )
  {
    // get a pointer to the definition
    ptPwmDef = ( PPWMDEF )&g_atPwmDefs[ ePwmEnum ];
    ptPwmCtl = &atPwmCtls[ ePwmEnum ];

    // calculate the PWM width
    if (( nPercent >= 0 ) && ( nPercent <= 100 ))
    {
      // test for zero
      if ( nPercent == 0)
      {
        // set pulse width to 0
        uNewPwmWidth = 0;
      }
      else
      {
        // calculate the new pulse width
        uNewPwmWidth = ( ptPwmCtl->uFreq * nPercent ) / 100;
      }
    }
    else
    {
      // set the error
      eError = PWM_ERR_ILLVALUE;
    }

    // set the pulse width
    pwm_pin_set_cycles( ptPwmCtl->ptDevice, ptPwmDef->nChannel, ptPwmCtl->uFreq, uNewPwmWidth, ptPwmCtl->uFlags ); 
  }
  else
  {
    // set the error
    eError = PWM_ERR_ILLEGAL;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function Pwm_SetFrequency
 *
 * @brief set frequency
 *
 * This function will set the frequency with a 50% pulse widht
 *
 * @param[in]   ePwmEnum    PWM channel enumeration
 * @param[in]   uFreqCps    freuqency of pulse width
 *
 * @return      appropriate error
 *
 *****************************************************************************/
PWMERR Pwm_SetFrequency( PWMENUM ePwmEnum, U32 uFreqCps )
{
  PWMERR  eError = PWM_ERR_NONE;
  U32     uPulseWidth;
  PPWMDEF ptPwmDef;
  PPWMCTL ptPwmCtl;

  // test for valid PWM enumeration
  if ( ePwmEnum < PWM_ENUM_MAX )
  {
    // get a pointer to the definition
    ptPwmDef = ( PPWMDEF )&g_atPwmDefs[ ePwmEnum ];
    ptPwmCtl = &atPwmCtls[ ePwmEnum ];

    // calculate the pulse width
    uPulseWidth = uFreqCps >> 2;

    // configure the output
    pwm_pin_set_cycles( ptPwmCtl->ptDevice, ptPwmDef->nChannel, uFreqCps, uPulseWidth, ptPwmCtl->uFlags );
  }
  else
  {
    // set the error
    eError = PWM_ERR_ILLEGAL;
  }

  // return the error
  return( eError );
}

/**@} EOF Pwm.c */
