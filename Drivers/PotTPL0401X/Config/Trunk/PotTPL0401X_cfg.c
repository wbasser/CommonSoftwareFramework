/******************************************************************************
 * @file PotTPL0401X_cfg.c
 *
 * @brief TPL0401X Digital pot implementation
 *
 * This file provides the implementation for TPL0401X digital pot device driver
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
 * \addtogroup PotTPL0401X
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PotTPL0401X/PotTPL0401X_cfg.h"

// library includes -----------------------------------------------------------
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  I2CXFRCTL tI2cXfr;

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function PotTPL0401X_Write
 *
 * @brief write a value
 *
 * This function will validate the device, and write the value to the device
 *
 * @param[in]   eDevice       device enumeration
 * @param[in]   nValue        value to write
 *
 * @return      TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL PotTPL0401X_WriteReg( U8 nDeviceAddr, U8 nValue )
{
	BOOL bStatus = FALSE;
	
	// fill the transfer control
	tI2cXfr.nDevAddr = anDeviceAddress[ eDevice ];
	tI2cXfr.nAddrLen = 1;
	tI2cXfr.tAddress.anValue[ LE_U32_LSB_IDX ] = 0;
	tI2cXfr.wDataLen = 1;
	tI2cXfr.pnData = &nValue; 
	tI2cXfr.uTimeout = 1000;
	
	// now write it
	bStatus = ( I2c_Write( TPL0401XDIGITALPOT_I2C_ENUM, &tI2cXfr ) == I2C_ERR_NONE ) ? FALSE : TRUE;

  // return the error
  return( bStatus );  
}

/******************************************************************************
 * @function PotTPL0401X_ReadReg
 *
 * @brief read a value
 *
 * This function will validate the devie and read the value from that device
 *
 * @param[in]   eDevice       device enumeration
 * @param[in]   pnValue       pointer to store the read value
 *
 * @return      TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL PotTPL0401X_ReadReg( U8 nDeviceAddr, PU8 pnValue )
{
	BOOL bStatus = FALSE;

	// fill the transfer control
	tI2cXfr.nDevAddr = anDeviceAddress[ eDevice ];
	tI2cXfr.nAddrLen = 1;
	tI2cXfr.tAddress.anValue[ LE_U32_LSB_IDX ] = 0;
	tI2cXfr.wDataLen = 1;
	tI2cXfr.pnData = pnValue;
	tI2cXfr.uTimeout = 1000;
	
	// now write it
	bStatus = ( I2c_Read( TPL0401XDIGITALPOT_I2C_ENUM, &tI2cXfr ) == I2C_ERR_NONE ) ? FALSE : TRUE;

  // return the error
  return( bStatus );
}
 
/**@} EOF PotTPL0401X_cfg.c */
