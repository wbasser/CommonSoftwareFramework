/******************************************************************************
 * @file HapDRV2625_cfg.h
 *
 * @brief DRV2625 haptic driver configuration declarations 
 *
 * This file provides the declarations for the DRV2625 haptic driver
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
 
// ensure only one instantiation
#ifndef _HAPDRV2625_CFG_H
#define _HAPDRV2625_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  HapDRV2565_WriteRegisters( U8 nBaseReg, PU8 pnData, U8 nNumOfRegs );
extern  BOOL  HapDRV2565_ReadRegisters( U8 nBaseReg, PU8 pnData, U8 nNumOfRegs );

/**@} EOF HapDRV2625_cfg.h */

#endif  // _HAPDRV2625_CFG_H