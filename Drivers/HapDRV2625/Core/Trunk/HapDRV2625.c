/******************************************************************************
 * @file HapDRV2625.c
 *
 * @brief DRV2625 haptic driver implementation
 *
 * This file provides the implementation for the DRV2625 haptic driver
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
 * \addtogroup HapDRV2625
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HapDRV2625/HapDRV2625.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the chip drive
#define DRV2625_CHIP_ID                         ( 0x13 )

/// define the default rated voltage - constant
#define DRV2625_DFLT_RATED_VOLTS_K              ( 21.8e-3 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static BOOL RunCalibration( void );
      
// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function HapDRV2625_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void HapDRV2625_Initialize( void )
{
  REG00CHIPID     tChipId;
  HAPDRV2625CALIB tCalibration;
  REGS            tRegs;

  // validate that the device is present
  if ( !( HapDRV2625_ReadRegisters( DRV2625_REG_00_ID, &tRegs.nByte, 1 )))
  {
    // validate chip ID
    if ( DRV2625_CHIP_ID == tRegs.nByte )
    {
      // valid chip - continue with initialization
      if ( HapDRV2625_GetConfiguration( &tCalibration ))
      {
        // write the values to the chip
        HapDRV2625_ReadRegisters( DRV2625_REG_35_GAINCTL, &tRegs.nByte, 1 );
        tRegs.tReg35.nBefmGain = tCalibration.nBemfGain;
        HapDRV2625_WriteRegisters( DRV2625_REG_35_GAINCTL, &tRegs.nByte, 1 );
        tRegs.nByte = tCalibration.nACalComp;
        HapDRV2625_WriteRegisters( DRV2625_REG_33_ACALCOMP, &tRegs.nByte, 1 );
        tRegs.nByte = tCalibration.nACalBemf;
        HapDRV2625_WriteRegisters( DRV2625_REG_34_ACALBEMF, &tRegs.nByte, 1 );
      }
      else
      {
        // perform an automatic calibration
        if ( RunCalibration( ) == TRUE )
        {
          // read and store the values
          HapDRV2625_ReadRegisters( DRV2625_REG_35_GAINCTL, &tRegs.nByte, 1 );
          tCalibration.nBemfGain = tRegs.tReg35.nBefmGain;
          HapDRV2625_ReadRegisters( DRV2625_REG_33_ACALCOMP, &tRegs.nByte, 1 );
          tCalibration.nACalComp = tRegs.nByte;
          HapDRV2625_ReadRegisters( DRV2625_REG_34_ACALBEMF, &tRegs.nByte, 1 );
          tCalibration.nACalBemf = tRegs.nByte;

          // store it
          HapDRV2625_SetConfiguration( &tCalibration );
        }
      }

      // set the acutator type
      tRegs.nByte = 0;
      tRegs.tReg08.bErmLraSel = DRV2625_ACTUATOR_TYPE_SELECTED;
      tRegs.tReg08.bClosedLoop  = TRUE;
      HapDRV2625_WriteRegisters( DRV2625_REG_08_CTRL2, &tRegs.nByte, 1 );

      // set the playback control
      tRegs.nByte = 0;
      tRegs.tReg13.bLibSel = ON;
      HapDRV2625_WriteRegisters( DRV2625_REG_13_PLAYCTRL, &tRegs.nByte, 1 );

      // set up the trig funciton bit
      tRegs.nByte = 0;
      tRegs.tReg07.nTrigPinFunc = TRIG_PIN_FUNC_INT;
      HapDRV2625_WriteRegisters( DRV2625_REG_07_CTRL1, &tRegs.nByte, 1 );
    }
  }
}

/******************************************************************************
 * @function HapDRV2625_PlayTone
 *
 * @brief play a tone
 *
 * This function will play a tone
 *
 * @param[in]   nTone   desired tone
 * @param[in[   bState  desired state
 *
 *****************************************************************************/
void HapDRV2625_PlayTone( U8 nTone, BOOL bState )
{
  REGS  tRegs;

  // check for on
  if ( bState == ON )
  {
    // get the current reg 7/set RTP mode
    HapDRV2625_ReadRegisters( DRV2625_REG_07_CTRL1, &tRegs.nByte, 1 );
    tRegs.tReg07.nMode = OP_MODE_RTP;
    HapDRV2625_WriteRegisters( DRV2625_REG_07_CTRL1, &tRegs.nByte, 1 );
  
    // now play the tone
    tRegs.nByte = nTone;
    HapDRV2625_WriteRegisters( DRV2625_REG_14_RTPINPUT, &tRegs.nByte, 1 );

    // set the go bit
    tRegs.nByte = 0;
    tRegs.tReg12.bGo = ON;
  }
  else
  {
    // clear the go bit
    tRegs.nByte = 0;
  }

  // now write the correct go bit state
  HapDRV2625_WriteRegisters( DRV2625_REG_12_GO, &tRegs.nByte, 1 );
}

/******************************************************************************
 * @function HapDRV2625_PlaySequence
 *
 * @brief play a sequence
 *
 * This function will play a sequence
 *
 * @param[in]   ptHapSequence   pointer to the desired sequence
 * @param[in[   bState  desired state
 *
 *****************************************************************************/
void HapDRV2625_PlaySequence( PHAPDRV2625SEQDEF ptHapSequence )
{
  REGS              tRegs;
  REGWAVESEQ        tRegSeq;
  U8                nIdx, nLoop;

  // clear the registers
  memset( &tRegSeq, 0, REGWAVESEQ_SIZE );

  // copy the sequence to the registers
  for( nIdx = 0; nIdx < HAPDRV2625_MAX_SEQS; nIdx++ )
  {
    // set the value
    tRegSeq.atRegWS[ nIdx ].nSeqSelDly = ptHapSequence->atSeqEntry[ nIdx ].tWaveSeq.nSeqSelDly;
    tRegSeq.atRegWS[ nIdx ].bDlySel = ptHapSequence->atSeqEntry[ nIdx ].tWaveSeq.bDlySel;

    // get the loop control
    nLoop = ptHapSequence->atSeqEntry[ nIdx ].eSeqLoop;

    // set the loop control register
    switch( nIdx )
    {
      case 0 :
        tRegSeq.tRegLC14.eLoop15 = nLoop;
        break;

      case 1 :
        tRegSeq.tRegLC14.eLoop26 = nLoop;
        break;

      case 2 :
        tRegSeq.tRegLC14.eLoop37 = nLoop;
        break;

      case 3 :
        tRegSeq.tRegLC14.eLoop48 = nLoop;
        break;

      case 4 :
        tRegSeq.tRegLC58.eLoop15 = nLoop;
        break;

      case 5 :
        tRegSeq.tRegLC58.eLoop26 = nLoop;
        break;

      case 6 :
        tRegSeq.tRegLC58.eLoop37 = nLoop;
        break;

      case 7 :
        tRegSeq.tRegLC58.eLoop48 = nLoop;
        break;

      default :
        break;
    }
    
    // now set the main loop control
    tRegSeq.tRegMNLOOP.eMainLoop = ptHapSequence->tMainLoop.eMainLoop;

    // now write the sequence registers
    HapDRV2625_WriteRegisters( DRV2625_REG_15_WAVESEQ1, ( PU8 )&tRegSeq, REGWAVESEQ_SIZE );

    // get the current reg 7/set RTP mode
    HapDRV2625_ReadRegisters( DRV2625_REG_07_CTRL1, &tRegs.nByte, 1 );
    tRegs.tReg07.nMode = OP_MODE_WSEQ;
    HapDRV2625_WriteRegisters( DRV2625_REG_07_CTRL1, &tRegs.nByte, 1 );

    // now write the correct go bit state
    HapDRV2625_WriteRegisters( DRV2625_REG_12_GO, &tRegs.nByte, 1 );
  }
}

/******************************************************************************
 * @function RunCalibration
 *
 * @brief run a auto calibration
 *
 * This function will run the auto calibration routine
 *
 * @return      TRUE if OK, FALSE otherwise
 *
 *****************************************************************************/
static BOOL RunCalibration( void )
{
  BOOL    bStatus = FALSE;
  REGS    tRegs;
  BOOL    bRunFlag;

  // select auto cal
  tRegs.nByte = 0;
  tRegs.tReg07.nMode = OP_MODE_AUTOCAL;
  HapDRV2625_WriteRegisters( DRV2625_REG_07_CTRL1, &tRegs.nByte, 1 );

  // set up defaults
  tRegs.nByte = 0;
  tRegs.tReg35.nFbBrake = 3;
  tRegs.tReg35.nLoopGain = LOOP_GAIN_FAST;
  HapDRV2625_WriteRegisters( DRV2625_REG_35_GAINCTL, &tRegs.nByte, 1 );

  // set the rated voltage
  tRegs.nByte = ( U8 )( DRV2625_DFLT_RATED_VOLTS / DRV2625_DFLT_RATED_VOLTS_K );
  HapDRV2625_WriteRegisters( DRV2625_REG_31_RATEDVOLT, &tRegs.nByte, 1 );
  HapDRV2625_WriteRegisters( DRV2625_REG_32_OVDCLAMP, &tRegs.nByte, 1 );

  // set the auto cal time
  tRegs.nByte = 0;
  tRegs.tReg42.nCalTime = AUTO_CALTIME_1000MS;
  HapDRV2625_WriteRegisters( DRV2625_REG_42_AUTOCALTIME, &tRegs.nByte, 1 );

  // set the drive time
  tRegs.nByte = 0;
  tRegs.tReg39.nDriveTime = DRIVE_TIME_LRA0X7ERM1X4MS;
  HapDRV2625_WriteRegisters( DRV2625_REG_39_LRADRVTIME, &tRegs.nByte, 1 );

  // set the blanking time/IDISS time
  tRegs.nByte = 0;
  tRegs.tReg40.nBlnkTime = BLANK_TIME_LRA025ERM075US;
  tRegs.tReg40.nIdissTime = IDISS_TIME_LRA025ERM075US;
  HapDRV2625_WriteRegisters( DRV2625_REG_40_BLNKTIME, &tRegs.nByte, 1 );

  // set the ZC cross time, sample time, od clamp time
  tRegs.nByte = 0;
  tRegs.tReg41.nZcDetTime = ZC_DET_TIME_100US;
  tRegs.tReg41.nSampleTime = SAMP_TIME_300US;
  tRegs.tReg41.nOdClmpTime = OD_CLAMP_TIME_AUTO;
  HapDRV2625_WriteRegisters( DRV2625_REG_41_OVDSAMPZCTIME, &tRegs.nByte, 1 );

  // now execute the auto cal
  tRegs.nByte = 0;
  tRegs.tReg12.bGo = ON;
  HapDRV2625_WriteRegisters( DRV2625_REG_12_GO, &tRegs.nByte, 1 );

  // now wait till done
  bRunFlag = ON;
  do
  {
    // read the go register 
    HapDRV2625_ReadRegisters( DRV2625_REG_12_GO, &tRegs.nByte, 1 );

    // check for done
    if ( tRegs.tReg12.bGo == OFF )
    {
      // clear the run flag
      bRunFlag = OFF;
    }
  }
  while( bRunFlag == ON );

  // now check for proper operation
  HapDRV2625_ReadRegisters( DRV2625_REG_01_STATUS, &tRegs.nByte, 1 );
  if (( tRegs.tReg01.bProcessDone == TRUE ) && ( tRegs.tReg01.bDiagFailed == FALSE ))
  {
    // return good status
    bStatus = TRUE;
  }

  // return status
  return( bStatus );
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

/**@} EOF HapDRV2625.c */
