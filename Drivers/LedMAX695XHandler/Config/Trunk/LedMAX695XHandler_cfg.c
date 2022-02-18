/******************************************************************************
 * @file LedMAX695XHandler_cfg.c
 *
 * @brief MAX695X led configurtion implementation
 *
 * This file provides the implementation for the configuration
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
 * \addtogroup LedMAX695XHandler_cfg
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedMAX695XHandler/LedMAX695XHandler_cfg.h"

// library includes -----------------------------------------------------------
#include "LocalBusHandler/LocalBusHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function LedMAX695XHandler_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any needed local initialization
 *
 *****************************************************************************/
void LedMAX695XHandler_LocalInitialize( void )
{
}

/******************************************************************************
 * @function LedMAX695XHandler_WriteData
 *
 * @brief write data
 *
 * This function will write the data to the device
 *
 * @param[in]   nCmd    command to send
 * @param[in]   pnData  pointer to the data to send
 * @param[in]   nLength length of the data
 *
 * @return      TRUE if errors found, FALSE if not
 *
 *****************************************************************************/
BOOL LedMAX695XHandler_WriteData( U8 nCmd, PU8 pnData, U8 nLength )
{
  BOOL      bStatus = FALSE;

  // fill the transfer control
  I2CXFRCTL tCtl =
  {
    .nDevAddr                           = LEDMAX695X_BASE_ADDRESS,
    .nAddrLen                           = 1,
    .tAddress.anValue[ LE_U32_LSB_IDX ] = nCmd,
    .wDataLen                           = nLength,
    .pnData                             = pnData,
    .uTimeout                           = 100,
  };

	// now write it
	bStatus = ( I2c_Write( g_pvLclBusHandle, &tCtl ) == I2C_ERR_NONE ) ? FALSE : TRUE;

  // return the status
  return( bStatus );
}
 
/******************************************************************************
 * @function LedMAX695XHandler_ReadData
 *
 * @brief write data
 *
 * This function will read the data from the device
 *
 * @param[in]   nCmd    command to send
 * @param[in]   pnData  pointer to the data to send
 * @param[in]   nLength length of the data
 *
 * @return      TRUE if errors found, FALSE if not
 *
 *****************************************************************************/
BOOL LedMAX695XHandler_ReadData( U8 nCmd, PU8 pnData, U8 nLength )
{
  BOOL      bStatus = FALSE;

  // fill the transfer control
  I2CXFRCTL tCtl =
  {
    .nDevAddr                           = LEDMAX695X_BASE_ADDRESS,
    .nAddrLen                           = 1,
    .tAddress.anValue[ LE_U32_LSB_IDX ] = nCmd,
    .wDataLen                           = nLength,
    .pnData                             = pnData,
    .uTimeout                           = 100,
  };

	// now write it
	bStatus = ( I2c_Read( g_pvLclBusHandle, &tCtl ) == I2C_ERR_NONE ) ? FALSE : TRUE;

  // return the status
  return( bStatus );
}
 
/**@} EOF LedMAX695XHandler.c */
