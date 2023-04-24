/******************************************************************************
 * @file DdsAD9833.c
 *
 * @brief Direct Digital Synthesizer AD9833 implementation
 *
 * This file provides the implementation for the DDS AD9833
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
 * \addtogroup DdsAD9833
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DdsAD9833/DdsAD9833.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the maximum frequency
#define DDS_MAX_FREQ                            ( 12500000 )

/// define the maximum phase
#define DDS_MAX_PHASE                           ( 4096 )

/// define the lo/hi 14 bit macros          
#define LO14BITS( val )                         (( U16 )( val & 0x3FF ))
#define HI14BITS( val )                         (( U16)(( val >> 14 ) & 0x3FF ))

// enumerations ---------------------------------------------------------------
/// enumerate the registers
typedef enum _REGADDR
{
  REG_ADDR_CTL    = 0,
  REG_ADDR_FRQ0,
  REG_ADDR_FRQ1,
  REG_ADDR_PHASE
} REGADDR;

// structures -----------------------------------------------------------------
/// define the control structure
typedef union _CTLREGBITS
{
  struct 
  {
    U16 bReserved1  : 1;    /// reserved #1
    U16 bMode       : 1;    ///< mode 0 = SIN, 1 = Triangle
    U16 bReserved2  : 1;    ///< reserved #2
    U16 bDivide2    : 1;    ///< divide by 2
    U16 bReserved3  : 1;    ///< reserved #3
    U16 bOpBitEnab  : 1;    ///< output bit enable
    U16 bSleep12    : 1;    ///< sleep 12 bit - DAC sleep enable
    U16 bSleep1     : 1;    ///< sleep 1 bit - PHase accumulator enable
    U16 bReset      : 1;    ///< reeset internal regs
    U16 bReserved4  : 1;    ///< reserved #4
    U16 bPhaseSel   : 1;    ///< phase register select
    U16 bFreqSel    : 1;    ///< frequency register select
    U16 bHlbSel     : 1;    ///< half load select ( 0 = LSB, 1 = MSB )
    U16 bSizSel     : 1;    ///< load mode ( 0 = 14 bit, 1 = 28 bit )
    U16 nRegSel     : 2;    ///< register select
  } tFields;
  U16 wWord;
} CTLREGBITS, *PCTLREGBITS;
#define CTLREGBITS_SIZE                 sizeof( CTLREGBITS )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  DDSAD9833MODE eCurMode;
static  DDSAD9833WAVE eCurWave;
static  U32UN         tCurFreq1;
static  U32UN         tCurFreq2;
static  U32UN         tCurPhase1;
static  U32UN         tCurPhase2;
static  U32UN         tCurSweep;
static  CTLREGBITS    tCurCtlReg;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function DdsAD9833_Initialize
 *
 * @brief initialization
 *
 * This function clear the registers and reset the device
 *
 *****************************************************************************/
void DdsAD9833_Initialize( void )
{
  // first put the device in reset
  tCurCtlReg.wWord = 0;
  tCurCtlReg.tFields.bReset = ON;
  DdsAD9833_WriteReg( tCurCtlReg.wWord );

  // now clear the frequency values
  DdsAD9833_SetFrequency( DDSAD9833_FREQSEL_0, 0 );
  DdsAD9833_SetFrequency( DDSAD9833_FREQSEL_1, 0 );

  // clear the phase values
  DdsAD9833_SetPhase( DDSAD9833_PHASESEL_0, 0 );
  DdsAD9833_SetPhase( DDSAD9833_PHASESEL_1, 0 );
}

/******************************************************************************
 * @function DdsAD9833_Control
 *
 * @brief disables/enables the frequency generation
 *
 * This function will disable/enable the frequency generation
 *
 * @param[in]   bState      desired state
 *
 *****************************************************************************/
void DdsAD9833_Control( BOOL bState )
{
  // clear the reset/sleep bits
  tCurCtlReg.tFields.bReset = ~bState;
  tCurCtlReg.tFields.bSleep1 = ~bState;
  tCurCtlReg.tFields.bSleep12 = ~bState;

  // write the control
  DdsAD9833_WriteReg( tCurCtlReg.wWord );
}

/******************************************************************************
 * @function DdsAD9833_SetMode
 *
 * @brief set the mode of operation
 *
 * This function will set the mode
 *
 * @param[in]   eMode     desired mode
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DDSAD9833ERR DdsAD9833_SetMode( DDSAD9833MODE eMode )
{
  DDSAD9833ERR  eError = DDSAD9833_ERR_NONE;

  // return the error
  return( eError );
}

/******************************************************************************
 * @function DdsAD9833_SetWave
 *
 * @brief set the wave type
 *
 * This function will set the wave type
 *
 * @param[in]   eWave       desired wave
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DDSAD9833ERR DdsAD9833_SetWave( DDSAD9833WAVE eWave )
{
  DDSAD9833ERR  eError = DDSAD9833_ERR_NONE;

  // check for a valid waveform
  if ( eWave < DDSAD9833_WAVE_MAX )
  {
    // set the current waveform
    eCurWave = eWave;

    // update the control register
    switch( eWave )
    {
      case DDSAD9833_WAVE_SIN :
        tCurCtlReg.tFields.bOpBitEnab = OFF;
        tCurCtlReg.tFields.bMode = OFF;
        break;

      case DDSAD9833_WAVE_TRI :
        tCurCtlReg.tFields.bOpBitEnab = OFF;
        tCurCtlReg.tFields.bMode = ON;
        break;

      case DDSAD9833_WAVE_SQR :
        tCurCtlReg.tFields.bOpBitEnab = ON;
        tCurCtlReg.tFields.bMode = OFF;
        break;

      default :
        break;
    }

    // if we are running
    if ( tCurCtlReg.tFields.bReset == OFF )
    {
      // update the control word
      DdsAD9833_WriteReg( tCurCtlReg.wWord );
    }
  }
  else
  {
    // set the error
    eError = DDSAD9833_ERR_ILLWAVE;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function DdsAD9833_SetFrequency
 *
 * @brief set the frequency
 *
 * This function will set the selected frequency value to the desired freq
 *
 * @param[in]   eFreqSel    frequency select
 * @param[in]   uFreqVal    frequency value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DDSAD9833ERR DdsAD9833_SetFrequency( DDSAD9833FREQSEL eFreqSel, U32 uFreqVal )
{
  DDSAD9833ERR  eError = DDSAD9833_ERR_NONE;
  U16           wTemp;

  // check for a valid frequency select
  if ( eFreqSel < DDSAD9833_FREQSEL_MAX )
  {
    // check for a valid frequency
    if ( uFreqVal <= DDS_MAX_FREQ )
    {
      // select the correct register/set mode to 28 bit
 //     tCurCtlReg.tFields.nRegSel =   // check for a valid frequency select
      if ( eFreqSel < DDSAD9833_FREQSEL_MAX )
      {
        // check for a valid frequency
        if ( uFreqVal <= DDS_MAX_FREQ )
        {
          // select the correct register/set mode to 28 bit
          tCurCtlReg.tFields.nRegSel = ( eFreqSel == DDSAD9833_FREQSEL_0 ) ? REG_ADDR_FRQ0 : REG_ADDR_FRQ1;
          tCurCtlReg.tFields.bSizSel = ON;
          DdsAD9833_WriteReg( tCurCtlReg.wWord );

          // write the lower 14 bits
          wTemp = LO14BITS( uFreqVal );
          DdsAD9833_WriteReg( wTemp );

          // write the upper 14 bits
          wTemp = HI14BITS( uFreqVal );
          DdsAD9833_WriteReg( wTemp );
        }
        else
        {
          // illegal frequency value
          eError = DDSAD9833_ERR_ILLFVAL;
        }
      }
      else
      {
        // illegal frequency select
        eError = DDSAD9833_ERR_ILLFSEL;
      }

;
      tCurCtlReg.tFields.bFreqSel = eFreqSel;
      tCurCtlReg.tFields.bSizSel = ON;
      DdsAD9833_WriteReg( tCurCtlReg.wWord );

      // write the lower 14 bits
      wTemp = LO14BITS( uFreqVal );
      DdsAD9833_WriteReg( wTemp );

      // write the upper 14 bits
      wTemp = HI14BITS( uFreqVal );
      DdsAD9833_WriteReg( wTemp );
    }
    else
    {
      // illegal frequency value
      eError = DDSAD9833_ERR_ILLFVAL;
    }
  }
  else
  {
    // illegal frequency select
    eError = DDSAD9833_ERR_ILLFSEL;
  }

  // return the error
  return( eError );
}

/******************************************************************************
 * @function DdsAD9833_SetPhase
 *
 * @brief set the phase
 *
 * This function will set the selected phase value to the desired phase
 *
 * @param[in]   ePhaseSel    Phase select
 * @param[in]   wPhaseVal    phase value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
DDSAD9833ERR DdsAD9833_SetPhase( DDSAD9833PHASESEL ePhaseSel, U16 wPhaseVal )
{
  DDSAD9833ERR  eError = DDSAD9833_ERR_NONE;

  // check for a valid frequency select
  if ( ePhaseSel < DDSAD9833_PHASESEL_MAX )
  {
    // check for a valid frequency
    if ( wPhaseVal <= DDS_MAX_PHASE )
    {
      // select the correct register/set mode to 28 bit
      tCurCtlReg.tFields.nRegSel = REG_ADDR_PHASE;
      tCurCtlReg.tFields.bSizSel = ePhaseSel;
      DdsAD9833_WriteReg( tCurCtlReg.wWord );

      // now write the value
      DdsAD9833_WriteReg( wPhaseVal );
    }
    else
    {
      // illegal phase value
      eError = DDSAD9833_ERR_ILLPVAL;
    }
  }
  else
  {
    // illegal phase select
    eError = DDSAD9833_ERR_ILLPSEL;
  }

  // return the error
  return( eError );
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
 
/**@} EOF DdsAD9833.c */
