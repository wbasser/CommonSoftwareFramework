/******************************************************************************
 * @file CnvDAC7581.c
 *
 * @brief converter DAC7581 implementation
 *
 * This file provides the implementation for the DAC7581 converter
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
 * \addtogroup CnvDAC7581
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "CnvDAC7581/CnvDAC7581.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the maximum counts
#define DAC_MAX_COUNTS                          ( 65536 )

// enumerations ---------------------------------------------------------------
/// enumerate the load modes
typedef enum _LOADMODE
{
  LOAD_MODE_WRTTEMP = 0,
  LOAD_MODE_WRTTEMP_DAC,
  LOAD_MODE_UPDATE_DAC,
  LOAD_MODE_ALL,
  LOAD_MODE_MAX
} LOADMODE;

// structures -----------------------------------------------------------------
/// define the control register
typedef union _CTLREG
{
  struct  
  {
    BOOL  bPowerDnEn  : 1;    ///< power down enable
    BOOL  bReserved1  : 1;    ///< must be 0
    BOOL  bBroadcast  : 1;    ///< broadcast
    BOOL  bReserved2  : 1;    ///< must be 0
    U8    nLoadMode   : 2;    ///< load mode
    U8    nReserved3  : 2;    ///< reserved must be 0
  } tBits;
  U8  nByte;
} CTLREG, *PCTLREG;
#define CTLREG_SIZE                             sizeof( CTLREG )


// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function CnvDAC7581_Initialize
 *
 * @brief initialization
 *
 * This function will initialize the device
 *
 *****************************************************************************/
void CnvDAC7581_Initialize( void )
{
}

/******************************************************************************
 * @function CnvDAC7581_SetVoltsOut
 *
 * @brief set voltage out
 *
 * This function will set the DAC to the desired volts out
 *
 * @param[in]   fValue     desired voltage
 *
 *****************************************************************************/
void CnvDAC7581_SetVoltsOut( FLOAT fValue )
{
  U16 wCounts;

  // calculate the counts for the value
  if ( fValue <= CNVDAC7581_FULLSCALE_VOLTS )
  {
    // calculate the counts
    wCounts = ( U16 )(( fValue / CNVDAC7581_FULLSCALE_VOLTS ) * DAC_MAX_COUNTS );

    // output it
    CnvDAC7581_SetCountsOut( wCounts );
  }
}

/******************************************************************************
 * @function CnvDAC7581_SetCountsOut
 *
 * @brief set counts out
 *
 * This function will set the DAC to the desired counts
 *
 * @param[in]   wValue      value
 *
 *****************************************************************************/
void CnvDAC7581_SetCountsOut( U16 wValue )
{
  // set the counts out
  CTLREG tCtlReg = 
  {
    .tBits =
    {
      .bPowerDnEn = OFF;
      .bReserved1 = OFF;
      .bBroadcast = OFF;
      .bReserved2 = OFF;
      .nLoadMode  = LOAD_MODE_WRTTEMP_DAC;
      .nReserved3 = 0;
    },
  };

  // output the value
  CnvDAC7581_WriteRegisters( tCtlReg.nByte, wValue );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
 
/**@} EOF .c */
