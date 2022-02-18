/******************************************************************************
 * @file TofVL53L0X_cfg.c
 *
 * @brief Time of FLight VL53L0x configuration implementaion file 
 *
 * This file provides the definitions for the implementation of the Time Of Flight
 * VL53l0X sensor
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
 * \addtogroup TofVL53L0X
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TofVL53L0X/TofVL53L0X.h"

// library includes -----------------------------------------------------------
#include "ConfigManager/ConfigManager.h"
#include "GPIO/Gpio.h"
#include "HatWornHandler/HatWornHandler.h"
#include "I2C/I2c.h"
#include "SystemControlManager/SystemControlManager.h"
#include "SystemTick/SystemTick.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
#if ( TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE == TRUE )
  /// enumerate the poll task states
  typedef enum _POLLSCANSTATE
  {
    POLLSCAN_STATE_MEAS = 0,
    POLLSCAN_STATE_READ,
    POLLSCAN_STATE_MAX
  } POLLSCANSTATE;
#endif // TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
#if ( TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE == TRUE )
  static  POLLSCANSTATE ePollScanState;
#endif // TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function TofVL53L0X_LocalInitialize
 *
 * @brief local initialization
 *
 * This function performs any local initialization
 *
 *****************************************************************************/
void TofVL53L0X_LocalInitialize( void )
{
  #if ( TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE == TRUE )
    ePollScanState = POLLSCAN_STATE_MEAS;
  #endif // TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE
}

/******************************************************************************
 * @function TofVL53L0X_GetFactoryTestStatus
 *
 * @brief return the state of factory test
 *
 * This function return the state of factory test
 *
 * @return      TRUE if in factory test, FALSE otherwise
 *
 *****************************************************************************/
BOOL TofVL53L0X_GetFactoryTestStatus( void )
{
  // return the state of factory test
  return( SystemControlManager_GetFactoryModeStatus( ));
}

/******************************************************************************
 * @function VL53L0X_UpdateConfig
 *
 * @brief update the configuration
 *
 * This function update the configuration in EEPROM
 *
 *****************************************************************************/
void VL53L0X_UpdateConfig( void )
{
  // force an update
  ConfigManager_UpdateConfig( CONFIG_TYPE_TOFSENS );
}

#if ( TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE == TRUE )
  /******************************************************************************
   * @function TofVL53L0X_ProcessPoll
   *
   * @brief poll the device
   *
   * This function will process the poll oepration
   *
   * @param[in]   xArg      event argument
   *
   * @return      TRUE to flush event
   *
   *****************************************************************************/
  BOOL TofVL53L0X_ProcessPoll( TASKARG xArg )
  {
    // deterine the state
    switch( ePollScanState )
    {
      case POLLSCAN_STATE_MEAS :
        // start measurement/goto read state
        TofVL53L0X_StartMeasurement( );
        ePollScanState = POLLSCAN_STATE_READ;
        break;

      case POLLSCAN_STATE_READ :
        // check for done
        if ( TofVL53L0X_GetDataReady( ) == TRUE )
        {
          // call the data sample 
          TofVL53L0X_ProcessDataReady( );
        }

        // post the data ready event to the hat worn handler
        TaskManager_PostEventIrq( TASK_SCHD_ENUM_HATWORNSCAN, HATWORNHANDLER_NEW_DATA_EVENT );

        // go to read
        ePollScanState = POLLSCAN_STATE_MEAS;
        break;

      default :
        // should never happen
        ePollScanState = POLLSCAN_STATE_MEAS;
        break;
    }

    // return true to flush event
    return( TRUE );
  }
#endif // TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE

#if ( TOFVL53L0X_RESET_ENABLED == TRUE )
  /******************************************************************************
   * @function TofVL53L0X_Reset_Device
   *
   * @brief reset device
   *
   * This function will reset the device
   *
   *****************************************************************************/
  void TofVL53L0X_Reset_Device( void )
  {
    // Reset the VL53L0X
    Gpio_Set( GPIO_PIN_ENUM_TOFPWR, 0 );  // Turn OFF Sensor
    SystemTick_DelayMsec( 1 );
    Gpio_Set( GPIO_PIN_ENUM_TOFPWR, 1 );  // Turn ON Sensor
    SystemTick_DelayMsec( 1 );
  }
#endif // TOFVL53L0X_RESET_ENABLED

#if ( TOFVL53L0X_ENABLE_IRQ_OPERATION == TRUE )
  /******************************************************************************
   * @function TofVL53L0X)IrqControl
   *
   * @brief enable/disable the interrupt
   *
   * This function will enable/disable the interrupt
   *
   * @param[in]   bState    state of the interrupt
   *
   *****************************************************************************/
  void TofVL53L0X_IrqControl( BOOL bState )
  {
    // enable/disable the irq
    Gpio_Ioctl( GPIO_IRQ_ENUM_TOF, GPIO_ACT_ENBDSBIRQ, ( PVOID )&bState );
  }

  /******************************************************************************
   * @function TofVL53L0X_Irq_Callback
   *
   * @brief interrupt callback
   *
   * This function will execute the interrupt callback
   *
   * @param[in]   nIrq    IRQ tht caused the callback
   * @param[in]   nEvent  IRQ event
   * @param[in]   bSTate  state of the irq pin at the time of IRQ
   *
   *****************************************************************************/
  void TofVL53L0X_Irq_Callback( U8 nIrq, U8 nEvent, BOOL bState )
  {
    // call the data sample 
    TofVL53L0X_ProcessDataReady( );

    // post the data ready event to the hat worn handler
    TaskManager_PostEventIrq( TASK_SCHD_ENUM_HATWORNSCAN, HATWORNHANDLER_NEW_DATA_EVENT );
  }
#endif // TOFVL53L0X_ENABLE_IRQ_OPERATION

/******************************************************************************
 * @function TofVL53L0X_Write
 *
 * @brief write data to the device
 *
 * This function will write the data to the device
 *
 * @param[in]   nDevAddr  device address
 * @param[in]   nIndex    register index
 * @param[in]   pnData    pointer to the data to write
 * @param[in]   wLength   length of data to write
 *
 * @return      TRUE if errors detected, FALSE if no errors
 *
 *****************************************************************************/
BOOL TofVL53L0X_Write( U8 nDevAddr, U8 nIndex, PU8 pnData, U16 wLength )
{
  I2CXFRCTL tXfrCtl;
  I2CERR    eI2cErr;
  BOOL      bErrorsFound = FALSE;
  
  // build the transfer control
  tXfrCtl.nDevAddr = nDevAddr;
  tXfrCtl.tAddress.anValue[ LE_U32_LSB_IDX ] = nIndex;
  tXfrCtl.nAddrLen = 1;
  tXfrCtl.pnData = pnData;
  tXfrCtl.wDataLen = wLength;
  tXfrCtl.uTimeout = 100;
  
  // execute the write
  bErrorsFound = ( I2c_Write( TOFVL53L0X_I2C_ENUM, &tXfrCtl ) != I2C_ERR_NONE ) ? TRUE : FALSE;
  
  // return the status
  return( bErrorsFound );
}

/******************************************************************************
 * @function TofVL53L0X_Read
 *
 * @brief read data from the device
 *
 * This function will read the data fro the device
 *
 * @param[in]   nDevAddr  device address
 * @param[in]   nIndex    register index
 * @param[in]   pnData    pointer to the data to read
 * @param[in]   wLength   length of data to read
 *
 * @return      TRUE if errors detected, FALSE if no errors
 *
 *****************************************************************************/
BOOL TofVL53L0X_Read( U8 nDevAddr, U8 nIndex, PU8 pnData, U16 wLength )
{
  I2CXFRCTL tXfrCtl;
  I2CERR    eI2cErr;
  BOOL      bErrorsFound = FALSE;
  
  // build the transfer control
  tXfrCtl.nDevAddr = nDevAddr;
  tXfrCtl.tAddress.anValue[ LE_U32_LSB_IDX ] = nIndex;
  tXfrCtl.nAddrLen = 1;
  tXfrCtl.pnData = pnData;
  tXfrCtl.wDataLen = wLength;
  tXfrCtl.uTimeout = 500;
  
  // execute the read
  bErrorsFound = ( I2c_Read( TOFVL53L0X_I2C_ENUM, &tXfrCtl ) != I2C_ERR_NONE ) ? TRUE : FALSE;
  
  // return the status
  return( bErrorsFound );
}

/**@} EOF TofVL53L0X_cfg.c */
