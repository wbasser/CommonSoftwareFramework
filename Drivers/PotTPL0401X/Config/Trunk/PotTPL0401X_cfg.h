/******************************************************************************
 * @file PotTPL0401X_cfg.h
 *
 * @brief TPL0401X Digital pot declarations
 *
 * This file provides the declarations for the TPL0401X Digital Potentiometer
 * device driver
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
 
// ensure only one instantiation
#ifndef _POTTPL0401X_CFG_H
#define _POTTPL0401X_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL	PotTPL0401X_WriteReg( U8 nDeviceAddr, U8 nValue );
extern  BOOL	PotTPL0401X_ReadReg( U8 nDeviceAddr, PU8 pnValue );

/**@} EOF PotTPL0401X_cfg.h */

#endif  // _POTTPL0401X_CFG_H