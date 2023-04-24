/******************************************************************************
 * @file IOEPCA9555_cfg.h
 *
 * @brief IOEPCA9555 configuration declarations
 *
 * This file provides the declarations for the IOEPCA9555 module configuration
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
 * \addtogroup IOEPCA9555
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _IOEPCA9555_CFG_H
#define _IOEPCA9555_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IOEPCA9555/IOEPCA9555_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the devices
typedef enum _IOEPCA9555DEVENUM
{
  // add user enuemrations below
  IOEPCA9555_DEV_ENUM_LOC0015 = 0,
  IOEPCA9555_DEV_ENUM_LOC1631,
  
  // do not remove the entries below
  IOEPCA9555_DEV_ENUM_NAX,
  IOEPCA9555_DEV_ENUM_ILLEGAL = 255
} IOEPCA9555DEVENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE IOEPCA9555DEVDEF  g_atIoePca9555Devs[ ];

// global function prototypes --------------------------------------------------
extern	BOOL 	IoePCA9555_WriteDataToDevice( IOEPCA9555DEVADDR eDevAddr, U8 nBaseReg, PU8 pnData, U8 nCount );
extern	BOOL	IoePCA9555_ReadDataFromDevice( IOEPCA9555DEVADDR eDevAddr, U8 nBaseReg, PU8 pnData, U8 nCount );

/**@} EOF IoePCA9555_cfg.h */

#endif  // _IOEPCA9555_CFG_H