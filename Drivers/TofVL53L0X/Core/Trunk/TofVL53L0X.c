/******************************************************************************
 * @file TofVL53L0X.c
 *
 * @brief ST Time-of-Flight (ToF) sensor implementation 
 *
 * This file provides the implementation for the ST Time-of-Flight 
 * Laser-Ranging sensor.
 *
 * @copyright Copyright (c) 2017 Cyber Intergration
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TofVL53L0X/TOfVL53L0X.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the base address
#define VL53L0X_DEV_ADDR                  ( 0x29 )

/// define the sigma/signal limits
#define VL53L0X_TIMING_BUDGET_USEC        ( 20000 )

/// define the threashold range ignore
#define VL53L0X_RANGE_THRESHOLD           (( FixPoint1616_t )( 1.5 * 0.023 * 65535 ))

/// define the max transfer size
#define MAX_I2C_XFER_SIZE                 ( 128 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// define the data structure

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  VL53L0X_Dev_t tMyDevice;
static  U16           wDistance;
static  BOOL          bTofError;
static  TOFVL53L0XCFG tConfigActl;

// local function prototypes --------------------------------------------------
static  BOOL  PerformCalibration( U16 wCalDistance ); 
static  BOOL  SetTofConfiguration( void );
static  BOOL  ReadNVMValues( void );

// constant parameter initializations -----------------------------------------
static  const CODE TOFVL53L0XCFG tConfigDflt =
{
       FALSE,
  0x0000000A,
        0x01,
        0x1D,
        0x41,
       -2000,
          55
};

/******************************************************************************
 * @function TofVL53L0X_Initialization
 *
 * @brief TOF setup
 *
 * This function will initialize the TOF sensor
 *
 *****************************************************************************/
void TofVL53L0X_Initialize( void )
{
//  FixPoint1616_t  tLimit;
//  U32             uTemp;
  VL53L0X_Error   eStatus;

  // set the address
  tMyDevice.I2cDevAddr = VL53L0X_DEV_ADDR;
  tMyDevice.comms_speed_khz = 100;
  tMyDevice.comms_type = 1;

  // run the initialization
  eStatus = TovVL53L0X_ReInitialize( TOFVL53L0X_CALIBRATE_DISTANCE, FALSE );

  // clear the TOF error
  bTofError = ( eStatus == VL53L0X_ERROR_NONE ) ? FALSE : TRUE;

  // call the local initialization
  TofVL53L0X_LocalInitialize( );
}  

/******************************************************************************
 * @function TofVL53L0X_GetConfigDefault
 *
 * @brief get the pointer to default config
 *
 * This function will return a pointer to the default config
 *
 * @return    pointer to the default config
 *
 *****************************************************************************/
PU8 TofVL53L0X_GetConfigDefault( void )
{
  // return a pointer to the default config
  return(( PU8 )&tConfigDflt );
}

/******************************************************************************
 * @function TofVL53L0X_GetConfigDefault
 *
 * @brief get the pointer to default config
 *
 * This function will return a pointer to the default config
 *
 * @return    pointer to the default config
 *
 *****************************************************************************/
PU8 TofVL53L0X_GetConfigActual( void )
{
  // return a pointer to the actual config
  return(( PU8 )&tConfigActl );
}

/******************************************************************************
 * @function TovVL53L0X_ReInitialize
 *
 * @brief run calibration
 *
 * This function will re-initialize and run calibration
 *
 * @param[in]   wCalDistance      distance to use for calibration
 *
 * @return    TRUE if errors, FALSE if not
 *
 *****************************************************************************/
BOOL TovVL53L0X_ReInitialize( U16 wCalDistance, BOOL bForceCal )
{
//  FixPoint1616_t  tLimit;
  U32             uTemp;
  VL53L0X_Error   eStatus;

  // reset the device
  #if ( TOFVL53L0X_RESET_ENABLED == TRUE )
    TofVL53L0X_Reset_Device( );
  #endif // TOFVL53L0X_RESET_ENABLED

  // Device Data Initialization
  if (( eStatus = VL53L0X_DataInit( &tMyDevice )) == VL53L0X_ERROR_NONE )
  {
    // Device Static Initialization
    if (( eStatus = VL53L0X_StaticInit( &tMyDevice )) == VL53L0X_ERROR_NONE )
    {
      // check for calibration performed
      if ( TRUE == tConfigActl.bCalibrated )
      {
        // read the NVM values
        eStatus = ReadNVMValues( );
      }

      #if ( TOFVL53L0X_ENABLE_INLINE_CALIBRATION == TRUE )
        // check for factory mode
        if (( TofVL53L0X_GetFactoryTestStatus( ) == TRUE ) || ( bForceCal == TRUE ))
        {
          // for now perform calibration
          eStatus = ( VL53L0X_Error )PerformCalibration( wCalDistance );
        }
        else
        {
          // just set configuration using stored values
          eStatus = ( VL53L0X_Error )SetTofConfiguration( );
        }
      #else
        // just set configuration using stored values
        eStatus = ( VL53L0X_Error )SetTofConfiguration( );
      #endif // TOFVL53L0X_ENABLE_INLINE_CALIBRATION 
  
      // Setup in single ranging mode ranging mode
      if ( eStatus == VL53L0X_ERROR_NONE )
      {
        // set device mode
        if (( eStatus = VL53L0X_SetDeviceMode( &tMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING )) == VL53L0X_ERROR_NONE )
        {
          // set sigma check limit
          if (( eStatus = VL53L0X_SetLimitCheckEnable( &tMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1 )) == VL53L0X_ERROR_NONE )
          {
            // set signal check limit
            if (( eStatus = VL53L0X_SetLimitCheckEnable( &tMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1 )) == VL53L0X_ERROR_NONE )
            {
              // set range innore thrshold
              if (( eStatus = VL53L0X_SetLimitCheckEnable( &tMyDevice, VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD, 1 )) == VL53L0X_ERROR_NONE )
              {
                // set range innore thrshold
                if (( eStatus = VL53L0X_SetLimitCheckEnable( &tMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1 )) == VL53L0X_ERROR_NONE )
                {
                  // set range innore thrshold
                  if (( eStatus = VL53L0X_SetLimitCheckValue( &tMyDevice, VL53L0X_CHECKENABLE_RANGE_IGNORE_THRESHOLD, VL53L0X_RANGE_THRESHOLD )) == VL53L0X_ERROR_NONE )
                  {
                    // Set timing budget duration
                    uTemp = VL53L0X_TIMING_BUDGET_USEC;
                    eStatus = VL53L0X_SetMeasurementTimingBudgetMicroSeconds( &tMyDevice, uTemp ); 
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  #if ( TOFVL53L0X_ENABLE_IRQ_OPERATION == TRUE )
    // enable the IRQ 
    VL53L0X_SetGpioConfig( &tMyDevice, 
                           0, 
                           VL53L0X_DEVICEMODE_SINGLE_RANGING, 
                           VL53L0X_GPIOFUNCTIONALITY_NEW_MEASURE_READY, 
                           VL53L0X_INTERRUPTPOLARITY_LOW );

    // enable the interrupt
    TofVL53L0X_IrqControl( ON );
  #endif // TOFVL53L0X_ENABLE_IRQ_OPERATION

  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function TofVL53L0X_StartMeasurement
 *
 * @brief start measurement
 *
 * This function will start a measurement
 *
 *****************************************************************************/
void TofVL53L0X_StartMeasurement( void )
{
  // start the measurement
  VL53L0X_StartMeasurement( &tMyDevice );

  #if ( TOFVL53L0X_ENABLE_IRQ_OPERATION == TRUE )
    // enable the interrupt
    VL53L0X_EnableInterruptMask( &tMyDevice, VL53L0X_GPIOFUNCTIONALITY_NEW_MEASURE_READY );
  #endif // TOFVL53L0X_ENABLE_IRQ_OPERATION
}

/******************************************************************************
 * @function TofVL53L0X_GetDataReady
 *
 * @brief return data ready status
 *
 * This function will return the state of data ready
 * 
 * @return  TRUE if data ready, FALSE if not
 *
 *****************************************************************************/
BOOL TofVL53L0X_GetDataReady( void )
{
  BOOL  bState;

  // gete data ready
  VL53L0X_GetMeasurementDataReady( &tMyDevice, &bState );

  // return the state
  return( bState );
}

/******************************************************************************
 * @function TofVL53L0X_GetDistance
 *
 * @brief return the current distance and status
 *
 * This function willl return the current distance and status
 *
 * @return    FALSE if no error, TRUE if error
 *
 *****************************************************************************/
BOOL TofVL53L0X_GetDistance( PU16 pwDistance )
{
  // set the distance
  *( pwDistance ) = wDistance;
  return( bTofError );
}

/******************************************************************************
 * @function TofVL53L0X_PerformCalibration
 *
 * @brief perform a manufacturing calibration
 *
 * This function will perform a initial factory calibration
 *
 * @return    TRUE if errors detected, FALSE if not
 *
 *****************************************************************************/
void TofVL53L0X_ProcessDataReady( void )
{
  VL53L0X_RangingMeasurementData_t  tRangeData;

  // get the measured data
  if ( VL53L0X_GetRangingMeasurementData( &tMyDevice, &tRangeData ) == VL53L0X_ERROR_NONE )
  {
    // set the distance to the millimeters
    wDistance = tRangeData.RangeMilliMeter;
    bTofError = FALSE;
  }
  else
  {
    // set the distance to 0
    wDistance = 0;
    bTofError = TRUE;
  }
}

/******************************************************************************
 * @function TofVL53L0X_RdByte
 *
 * @brief read a byte from the device
 *
 * This function will read a byte from the selected register
 *
 * @param[in]   ptDev         pointer to the device structure
 * @param[in]   nIndex        register to read from
 * @param[in]   pnData        pointer to data which is read
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_RdByte( VL53L0X_DEV ptDev, U8 nIndex, U8 *pnData )
{
  VL53L0X_Error tStatus;
  
  // read a byte
  tStatus = ( TofVL53L0X_Read( ptDev->I2cDevAddr, nIndex, pnData, sizeof( U8 )) == TRUE ) ? VL53L0X_ERROR_CONTROL_INTERFACE : VL53L0X_ERROR_NONE;

  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function TofVL53L0X_RdWord
 *
 * @brief read 2 bytes from an I2C register
 *
 * This function will read 2 bytes from the selected I2C register
 *
 * @param[in]   ptDev         pointer to the device structure
 * @param[in]   nIndex        register to read from
 * @param[in]   pwData        pointer to data which is read
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_RdWord( VL53L0X_DEV ptDev, U8 nIndex, uint16_t *pwData )
{
  VL53L0X_Error  tStatus;
  U16UN             tLclBuffer;

  tStatus = ( TofVL53L0X_Read( ptDev->I2cDevAddr, nIndex, ( PU8 )&tLclBuffer, sizeof( U16 )) == TRUE ) ? VL53L0X_ERROR_CONTROL_INTERFACE : VL53L0X_ERROR_NONE;

  // build the data
  *pwData = MAKEU16( tLclBuffer.anValue[ BE_U16_MSB_IDX ], tLclBuffer.anValue[ BE_U16_LSB_IDX ]);
		
  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function TofVL53L0X_RdDWord
 *
 * @brief read 4 bytes from an I2C register
 *
 * This function will read 4 bytes from the selected I2C register
 *
 * @param[in]   ptDev         pointer to the device structure
 * @param[in]   nIndex        register to read from
 * @param[in]   puData        pointer to data which is read
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_RdDWord( VL53L0X_DEV ptDev, U8 nIndex, U32 *puData )
{
  VL53L0X_Error  tStatus;
  U32UN             tLclBuffer;

  tStatus = ( TofVL53L0X_Read( ptDev->I2cDevAddr, nIndex, ( PU8 )&tLclBuffer, sizeof( U32 )) == TRUE ) ? VL53L0X_ERROR_CONTROL_INTERFACE : VL53L0X_ERROR_NONE;

  // build the data
  *puData = MAKEU32( tLclBuffer.anValue[ BE_U32_MSB_IDX ], tLclBuffer.anValue[ BE_U32_MS2_IDX ], tLclBuffer.anValue[ BE_U32_MS1_IDX ], tLclBuffer.anValue[ BE_U32_LSB_IDX ]);
		
  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function TofVL53L0X_RdMulti
 *
 * @brief read multiple bytes
 *
 * This function will read 4 bytes from the selected I2C register
 *
 * @param[in]   ptDev         pointer to the device structure
 * @param[in]   nIndex        register to read from
 * @param[in]   pnData        pointer to data which is read
 * @param[in]   uCount        number of bytes tor ead
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_ReadMulti( VL53L0X_DEV ptDev, U8 nIndex, U8 *pnData, U32 uCount )
{
  VL53L0X_Error  tStatus;
  
  // check for max size
  if ( uCount < MAX_I2C_XFER_SIZE )
  {
    // read the data
    tStatus = ( TofVL53L0X_Read( ptDev->I2cDevAddr, nIndex, pnData, uCount ) == TRUE ) ? VL53L0X_ERROR_CONTROL_INTERFACE : VL53L0X_ERROR_NONE;
  }
  else
  {
    tStatus = VL53L0X_ERROR_INVALID_PARAMS;
  }

  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function TofVL53L0X_WrByte
 *
 * @brief Write an I2C register
 *
 * This function will write to the selected register
 *
 * @param[in]   ptDev         pointer to the device structure
 * @param[in]   nIndex        register to write to
 * @param[in]   nData         data to write
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_WrByte( VL53L0X_DEV ptDev, U8 nIndex, U8 nData)
{
  VL53L0X_Error tStatus;
  
  // write a byte
  tStatus = ( TofVL53L0X_Write( ptDev->I2cDevAddr, nIndex, &nData, sizeof( U8 )) == TRUE ) ? VL53L0X_ERROR_CONTROL_INTERFACE : VL53L0X_ERROR_NONE;

  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function TofVL53L0X_WrWord
 *
 * @brief Write an I2C register
 *
 * This function will write to the selected register
 *
 * @param[in]   ptDev         pointer to the device structure
 * @param[in]   nIndex        register to write to
 * @param[in]   wData         data to write
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_WrWord( VL53L0X_DEV ptDev, U8 nIndex, U16 wData )
{
  VL53L0X_Error  tStatus;
  U16UN             tData;
  
  // fill the data
  tData.anValue[ BE_U16_MSB_IDX ] = HI16( wData );
  tData.anValue[ BE_U16_LSB_IDX ] = LO16( wData );
  
  // write a byte
  tStatus = ( TofVL53L0X_Write( ptDev->I2cDevAddr, nIndex, ( PU8 )&tData, sizeof( U16 )) == TRUE ) ? VL53L0X_ERROR_CONTROL_INTERFACE : VL53L0X_ERROR_NONE;

  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function TofVL53L0X_WrDWord
 *
 * @brief Write an I2C register
 *
 * This function will write to the selected register
 *
 * @param[in]   ptDev         pointer to the device structure
 * @param[in]   nIndex        register to write to
 * @param[in]   uData         data to write
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_WrDWord( VL53L0X_DEV ptDev, U8 nIndex, U32 uData )
{
  VL53L0X_Error  tStatus;
  U32UN             tData;
  
  // fill the data
  tData.anValue[ BE_U32_MSB_IDX ] = MSB32( uData );
  tData.anValue[ BE_U32_MS2_IDX ] = MS232( uData );
  tData.anValue[ BE_U32_MS1_IDX ] = MS132( uData );
  tData.anValue[ BE_U32_LSB_IDX ] = LSB32( uData );
  
  // write a byte
  tStatus = ( TofVL53L0X_Write( ptDev->I2cDevAddr, nIndex, ( PU8 )&tData, sizeof( U32 )) == TRUE ) ? VL53L0X_ERROR_CONTROL_INTERFACE : VL53L0X_ERROR_NONE;

  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function TofVL53L0X_WriteMulti
 *
 * @brief Write several bytes to an I2C register
 *
 * This function will write to the selected register
 *
 * @param[in]   ptDev         pointer to the device structure
 * @param[in]   nIndex        register to write to
 * @param[in]   pnData        ptoiner to data to write
 * @param[in]   uCount        number of bytes tor ead
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_WriteMulti( VL53L0X_DEV ptDev, U8 nIndex, U8 *pnData, U32 uCount)
{
  VL53L0X_Error  tStatus;
  
  // check for max size
  if ( uCount < MAX_I2C_XFER_SIZE )
  {
    // read the data
    tStatus = ( TofVL53L0X_Write( ptDev->I2cDevAddr, nIndex, pnData, uCount ) == TRUE ) ? VL53L0X_ERROR_CONTROL_INTERFACE : VL53L0X_ERROR_NONE;
  }
  else
  {
    tStatus = VL53L0X_ERROR_INVALID_PARAMS;
  }

  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function TofVL53L0X_UpdateByte
 *
 * @brief Read modify Write a byte at an I2C register
 *
 * This function will read modify then write to the selected I2C register
 *
 * @param[in]   ptDev         pointer to the device structure
 * @param[in]   nIndex        register to write to
 * @param[in]   nAndMask      and mask
 * @param[in]   nOrMask       or mask
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_UpdateByte( VL53L0X_DEV ptDev, U8 nIndex, U8 nAndMask, U8 nOrMask )
{
  VL53L0X_Error  tStatus;
  U8                nData;
  
  // read a byte of data
  if (( tStatus = VL53L0X_RdByte( ptDev, nIndex, &nData )) == VL53L0X_ERROR_NONE )
  {
    // apply the masks
    nData &= nAndMask;
    nData |= nOrMask;
    
    // now write it
    tStatus = VL53L0X_WrByte( ptDev, nIndex, nData );
  }
    
  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function VL53L0X_PollingDelay
 *
 * @brief polling delay
 *
 * This function will wait for a delay
 *
 * @param[in]   ptDev         pointer to the device structure
 *
 * @return      appropriate status
 *
 *****************************************************************************/
VL53L0X_Error VL53L0X_PollingDelay( VL53L0X_DEV Dev)
{
	VL53L0X_Error tStatus = VL53L0X_ERROR_NONE;

  // return the status
  return( tStatus );
}

/******************************************************************************
 * @function TofVL53L0X_PerformCalibration
 *
 * @brief perform a manufacturing calibration
 *
 * This function will perform a initial factory calibration
 *
 * @param[in]   wCalDistance      distance to use for calibration
 *
 * @return    TRUE if errors detected, FALSE if not
 *
 *****************************************************************************/
static BOOL PerformCalibration( U16 wCalDistance )
{
  BOOL            bStatus = FALSE;
  FixPoint1616_t  tCalDistance;

  // Perform Reference Calibration
  bStatus = ( VL53L0X_PerformRefCalibration( &tMyDevice, &tConfigActl.nVhvSettings, &tConfigActl.nPhaseCal ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;

  // Device Reference SPAD Management
  bStatus = ( VL53L0X_PerformRefSpadManagement( &tMyDevice, &tConfigActl.uRefSpadCount, &tConfigActl.nIsApertureSpads ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;              

  #if ( TOFVL53L0X_ENABLE_OFFSET_CALIBRATION == TRUE )
    // off set calibration
    tCalDistance = ( FixPoint1616_t )( wCalDistance << 16 );
    bStatus = ( VL53L0X_PerformOffsetCalibration( &tMyDevice, tCalDistance, &tConfigActl.lOffsetMicroMeter ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;
  #endif // TOFVL53L0X_ENABLE_OFFSET_CALIBRATION

  #if ( TOFVL53L0X_ENABLE_XTALK_CALIBRATION == TRUE )
    // set the crosstalk calibration
    bStatus = ( VL53L0X_PerformXTalkCalibration( &tMyDevice, tCalDistance, &tConfigActl.tXTalkCompRateMegaCps ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;
  #endif // TOFVL53L0X_ENABLE_XTALK_CALIBRATION

  // update the configuration
  if ( bStatus == FALSE )
  {
    // set the calibrated value
    tConfigActl.bCalibrated = TRUE;

    // update the configuration
    VL53L0X_UpdateConfig( );
  }

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function SetTofConfiguration
 *
 * @brief set TOF configuration
 *
 * This function will set the TOF configuration
 *
 * @return    TRUE if errors detected, FALSE if not
 *
 *****************************************************************************/
static BOOL SetTofConfiguration( void )
{
  BOOL  bStatus = FALSE;

  // set the reference SPAD management
  bStatus = ( VL53L0X_SetReferenceSpads( &tMyDevice, tConfigActl.uRefSpadCount, tConfigActl.nIsApertureSpads ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;

  // set reference calibration
  bStatus = ( VL53L0X_SetRefCalibration( &tMyDevice, tConfigActl.nVhvSettings, tConfigActl.nPhaseCal ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;

  #if ( TOFVL53L0X_ENABLE_OFFSET_CALIBRATION == TRUE )
    // set the offset calibration
    bStatus = ( VL53L0X_SetOffsetCalibrationDataMicroMeter( &tMyDevice, tConfigActl.lOffsetMicroMeter ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;
  #endif // TOFVL53L0X_ENABLE_OFFSET_CALIBRATION

  #if ( TOFVL53L0X_ENABLE_XTALK_CALIBRATION == TRUE )
    // set the crosstalk/enable it
    bStatus = ( VL53L0X_SetXTalkCompensationRateMegaCps( &tMyDevice, tConfigActl.tXTalkCompRateMegaCps ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE; 
  #endif // TOFVL53L0X_ENABLE_XTALK_CALIBRATION

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function ReadNVMValues
 *
 * @brief read the stored NVM values
 *
 * This function will read the stored NVM values
 *
 * @return    TRUE if errors detected, FALSE if not
 *
 *****************************************************************************/
static BOOL ReadNVMValues( void )
{
  BOOL bStatus = FALSE;

  // read the reference spads
  bStatus = ( VL53L0X_GetReferenceSpads( &tMyDevice, &tConfigActl.uRefSpadCount, &tConfigActl.nIsApertureSpads ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;

  // read the reference calibration
  bStatus = ( VL53L0X_GetRefCalibration( &tMyDevice, &tConfigActl.nVhvSettings, &tConfigActl.nPhaseCal ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;

  // read the offset calibration
  bStatus = ( VL53L0X_GetOffsetCalibrationDataMicroMeter( &tMyDevice, &tConfigActl.lOffsetMicroMeter ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;

  #if ( TOFVL53L0X_ENABLE_XTALK_CALIBRATION == TRUE )
    // set the crosstalk/enable it
    bStatus = ( VL53L0X_GetXTalkCompensationRateMegaCps( &tMyDevice, &tConfigActl.tXTalkCompRateMegaCps ) != VL53L0X_ERROR_NONE ) ? TRUE : FALSE;
  #endif // TOFVL53L0X_ENABLE_XTALK_CALIBRATION

  // return the status
  return( bStatus );
}

/**@} EOF TofVL53L0X.c */
