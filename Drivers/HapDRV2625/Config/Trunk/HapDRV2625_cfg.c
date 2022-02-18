/******************************************************************************
 * @file HapDRV2625_cfg.c
 *
 * @brief DRV2565 haptic driver configuration implementation
 *
 * This file provides the implementation for the DRV2565 haptic driver
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
#include "HapDRV2565/HapDRV2565_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function HapDRV2565_WriteRegisters
 *
 * @brief write registers
 *
 * This function will write the prescribed data to the registers
 *
 * @param[in]   nBaseReg    base registers
 * @param[in]   pnData      pointer to the data to be written
 * @param[in]   nNumOfRegs  number of registers to write
 *
 * @return      TRUE if errors detected, FALSE if OK
 *
 *****************************************************************************/
BOOL HapDRV2565_WriteRegisters( U8 nBaseReg, PU8 pnData, U8 nNumOfRegs )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function HapDRV2565_ReadRegisters
 *
 * @brief read registers
 *
 * This function will read the prescribed data to the registers
 *
 * @param[in]   nBaseReg    base registers
 * @param[in]   pnData      pointer to the storage for the data read
 * @param[in]   nNumOfRegs  number of registers to read
 *
 * @return      TRUE if errors detected, FALSE if OK
 *
 *****************************************************************************/
BOOL HapDRV2565_ReadRegisters( U8 nBaseReg, PU8 pnData, U8 nNumOfRegs )
{
  BOOL  bStatus = TRUE;
  
  // return status
  return( bStatus );
}

/**@} EOF HapDRV2565_cfg.c */
