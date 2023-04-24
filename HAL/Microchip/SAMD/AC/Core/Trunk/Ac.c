/******************************************************************************
 * @file Ac.c
 *
 * @brief analog comparator implementation
 *
 * This file provides the implementation for the analog comparator
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AC/Ac.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PVACCALLBACK  apvCallbacks[ AC_CHANNEL_MAX ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Ac_Initilaize
 *
 * @brief initilaization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void Ac_Initialize( void )
{
  ACENUM    eAcEnum;
  PACDEF    ptAcDef;
  ACCHANNEL eChannel;

  // for each channel defined
  for ( eAcEnum = 0; eAcEnum < AC_ENUM_MAX; eAcEnum++ )
  {
    // get the definition file
    ptAcDef = ( PACDEF )&g_atAcDefs[ eAcEnum ];

    // get the channel
    eChannel = ptAcDef->eChannel;

    // save the callback
    apvCallbacks[ eChannel ] = ptAcDef->pvCallback;

    // if callback is not zero - enable interrupt
    if ( ptAcDef->pvCallback != NULL )
    {
      AC->INTENSET.reg |= ( eChannel == AC_CHANNEL_1 ) ? AC_INTENSET_COMP1 : AC_INTENSET_COMP0;
    }

    // now set up the event control
    if ( ptAcDef->bEventInpEnb )
    {
      // enable the event input
      AC->EVCTRL.reg |= ( eChannel == AC_CHANNEL_1 ) ? AC_EVCTRL_COMPEI1 : AC_EVCTRL_COMPEI0;
    }

    if ( ptAcDef->bEventOutEnb )
    {
      // enable the event output
      AC->EVCTRL.reg |= ( eChannel == AC_CHANNEL_1 ) ? AC_EVCTRL_COMPEO1 : AC_EVCTRL_COMPEO0;
    }

    // now set the comparator control
    AC->COMPCTRL[ eChannel ].bit.SINGLE = ptAcDef->bSingleShot;
    AC->COMPCTRL[ eChannel ].bit.SPEED = ptAcDef->eSpeedSel;
    AC->COMPCTRL[ eChannel ].bit.INTSEL = ptAcDef->eOutMode;
    AC->COMPCTRL[ eChannel ].bit.MUXNEG = ptAcDef->eNegChan;
    AC->COMPCTRL[ eChannel ].bit.MUXPOS = ptAcDef->ePosChan;
    AC->COMPCTRL[ eChannel ].bit.OUT = ptAcDef->eOutEdge;
    AC->COMPCTRL[ eChannel ].bit.HYST = ptAcDef->bHystEnb;
    AC->COMPCTRL[ eChannel ].bit.FLEN = ptAcDef->eFiltLen;

    // enable it
    AC->COMPCTRL[ eChannel ].bit.ENABLE = ON;

    // enable the AC
    AC->CTRLA.bit.ENABLE = ON;
  }
}

/******************************************************************************
 * @function Ac_Ioctl
 *
 * @brief analog comparator IOCTL function
 *
 * This function provides the IOCTL functions for controlling the analog
 * comparator
 *
 * @param[in]   eAction       IOCTL action
 * @param[in]   pvValue       pointer to the data value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
ACERROR Ac_Ioctl( ACIOCTL eAction, PVOID pvValue )
{
  ACERROR eError = AC_ERROR_NONE;

  // return the error
  return( eError );
}
 
/******************************************************************************
 * @function AC_Handler
 *
 * @brief AC interrupt handler
 *
 * This function will process the AC handler
 *
 *****************************************************************************/
void AC_Handler( void )
{
  PVACCALLBACK  pvCallback;

  // determine the channel
  if ( AC->INTFLAG.bit.COMP0 )
  {
      pvCallback = apvCallbacks[ AC_CHANNEL_0 ];
      pvCallback( );
  }

  if ( AC->INTFLAG.bit.COMP1 )
  {
      pvCallback = apvCallbacks[ AC_CHANNEL_1 ];
      pvCallback( );
  }
}

/**@} EOF Ac.c */
