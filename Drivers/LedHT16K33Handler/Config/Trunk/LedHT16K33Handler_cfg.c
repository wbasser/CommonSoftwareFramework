/******************************************************************************
 * @file LedHT16K33Handler_cfg.c
 *
 * @brief HT163K33 led configurtion implementation
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
 * \addtogroup LedHT16K33Handler_cfg
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "LedHT16K33Handler/LedHT16K33Handler_cfg.h"

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
 * @function LedHT16K33Handler_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any needed local initialization
 *
 *****************************************************************************/
void LedHT16K33Handler_LocalInitialize( void )
{
}

/******************************************************************************
 * @function LedHT16K33Handler_WriteData
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
BOOL LedHT16K33Handler_WriteData( U8 nCmd, PU8 pnData, U8 nLength )
{
  BOOL      bStatus = FALSE;

  // fill the transfer control
  I2CXFRCTL tCtl =
  {
    .nDevAddr                           = LEDHT16K33_BASE_ADDRESS,
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
 
/**@} EOF LedHT16K33Handler.c */
