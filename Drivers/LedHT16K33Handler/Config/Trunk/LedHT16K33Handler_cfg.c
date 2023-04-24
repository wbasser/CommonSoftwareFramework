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
#include "I2C/I2c.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
U8  g_anLedHT16K33CharOffset[ LEDHT16K33_OFFSET_MAX ] =
{
  0, 1, 3, 4, 2
};

/******************************************************************************
 * @function LedHT16K33Handler_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any needed local initialization
 *
 * @return    TRUE if errors, FALSE otherwise
 *
*****************************************************************************/
BOOL LedHT16K33Handler_LocalInitialize( void )
{
  BOOL  bStatus = FALSE;

  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function LedHT16K33Handler_WriteData
 *
 * @brief write data
 *
 * This function will write the data to the device
 *
 * @param[in]   nDevice device ID
 * @param[in]   nCmd    command to send
 * @param[in]   pnData  pointer to the data to send
 * @param[in]   nLength length of the data
 *
 * @return      TRUE if errors found, FALSE if not
 *
 *****************************************************************************/
BOOL LedHT16K33Handler_WriteData( U8 nDevice, U8 nCmd, PU8 pnData, U8 nLength )
{
  BOOL bStatus = FALSE;

  I2CXFRCTL tXfrCtl =
  {
    .nDevAddr   = nDevice + LEDHT16K33_BASE_ADDRESS,
    .nAddrLen   = 1,
    .tAddress   =
    {
      .uValue   = nCmd,
    },          
    .pnData     = pnData,
    .wDataLen   = nLength,
    .uTimeout   = 100
  };

  // now write the it out
  bStatus = ( I2c_Write( I2C_DEV_ENUM_LCLBUS, &tXfrCtl ) == I2C_ERROR_NONE ) ? FALSE : TRUE;

  // return the status
  return( bStatus );
}
 
/**@} EOF LedHT16K33Handler.c */
