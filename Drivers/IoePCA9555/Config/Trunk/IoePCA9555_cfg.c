/******************************************************************************
 * @file IoePCA9555_cfg.c
 *
 * @brief PCA9555 port expander configuration implementation 
 *
 * This file provides the configuration for the PCA9534 port expander module
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup IoePCA9555
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IoePCA9555/IoePCA9555_cfg.h"

// library includes -----------------------------------------------------------
#include "LocalBusHandler/LocalBusHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// global function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE IOEPCA9555DEVDEF  g_atIoePca9555Devs[ IOEPCA9555_DEV_ENUM_NAX ] =
{
  //// configure a device and then configure the number of pins
  {
    .eDeviceAddress = IOEPCA9555_DEV_ADDR_20,
    .atPinDefs[ 0 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 1 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 2 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 3 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 4 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 5 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 6 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 7 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 8 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 9 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 10 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 11 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 12 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 13 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 14 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 15 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
  },
  {
    .eDeviceAddress = IOEPCA9555_DEV_ADDR_21,
    .atPinDefs[ 0 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 1 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 2 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 3 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 4 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 5 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 6 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 7 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 8 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 9 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 10 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 11 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 12 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 13 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 14 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
    .atPinDefs[ 15 ] = 
    { 
      .eDirection   = IOEPCA9555_DIR_IN,
      .bInvert      = ON,
      .bInitial     = OFF
    },
  }
};

/******************************************************************************
 * @function IoePCA9555_WriteDataToDevice
 *
 * @brief write data to the device
 *
 * This function will set up the transfer structure, and write the data to the
 * device
 *
 * @param[in]   eDeviddr      IOEPCA9555 device address
 * @param[in]   nBaseReg      base register
 * @param[in]   pnData        pointer data to write
 * @param[in]	nCount        count of bytes to write
 *
 * @return      FALSE if OK, TRUE if error
 *
 *****************************************************************************/
BOOL IoePCA9555_WriteDataToDevice( IOEPCA9555DEVADDR eDevAddr, U8 nBaseReg, PU8 pnData, U8 nCount )
{
  BOOL      bStatus = FALSE;
  I2CXFRCTL tI2cXfrCtl;
  I2CERR    eError;

  // now fill the I2C transfer structure
  tI2cXfrCtl.nDevAddr = eDevAddr;
  tI2cXfrCtl.tAddress.anValue[ LE_U32_LSB_IDX ] = nBaseReg;
  tI2cXfrCtl.nAddrLen = 1;
  tI2cXfrCtl.uTimeout = 5;
  tI2cXfrCtl.wDataLen = nCount;
  tI2cXfrCtl.pnData = pnData;

  // now write it
  if (( eError = I2c_Write( g_pvLclBusHandle, &tI2cXfrCtl )) != I2C_ERR_NONE )
  {
    // set the fault flag
    bStatus = TRUE;
  }

  // return good status
  return( bStatus );
}
 
/******************************************************************************
 * @function IoePCA9555_ReadDataFromDevice
 *
 * @brief read data from the device
 *
 * This function will set up the transfer structure, and write the data to the
 * device
 *
 * @param[in]   eDeviddr      IOEPCA9555 device address
 * @param[in]   nBaseReg      base register
 * @param[in]   pnData        pointer data to write
 * @param[in]   nCount        count of bytes to write
 *
 * @return      FALSE if OK, TRUE if error
 *
 *****************************************************************************/
BOOL IoePCA9555_ReadDataFromDevice( IOEPCA9555DEVADDR eDevAddr, U8 nBaseReg, PU8 pnData, U8 nCount )
{
  BOOL      bStatus = FALSE;
  I2CXFRCTL tI2cXfrCtl;
  I2CERR    eError;

  // now fill the I2C transfer structure
  tI2cXfrCtl.nDevAddr = eDevAddr;
  tI2cXfrCtl.tAddress.anValue[ LE_U32_LSB_IDX ] = nBaseReg;
  tI2cXfrCtl.nAddrLen = 1;
  tI2cXfrCtl.uTimeout = 5;
  tI2cXfrCtl.wDataLen = nCount;
  tI2cXfrCtl.pnData = pnData;

  // now write it
  if (( eError = I2c_Read( g_pvLclBusHandle, &tI2cXfrCtl )) != I2C_ERR_NONE )
  {
    // set the fault flag
    bStatus = TRUE;
  }

  // return good status
  return( bStatus );
}
 
/**@} EOF IoePCA9555_cfg.c */
