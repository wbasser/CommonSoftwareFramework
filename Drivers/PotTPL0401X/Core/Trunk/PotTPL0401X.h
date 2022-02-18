/******************************************************************************
 * @file PotTPL0401X.h
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
#ifndef _POTTPL0401X_H
#define _POTTPL0401X_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PotTPL0401X/PotTPL0401X_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the device
typedef enum _POTTPL0401XDEV
{
  POT_TPL0401X_DEV_A = 0,
  POT_TPL0401X_DEV_B,
  POT_TPL0401X_DEV_MAX  
} POTTPL0401XDEV;

/// enumerate the errors
typedef enum _POTTPL0401ERR
{
  POT_TPL0401X_ERR_NONE = 0,
  POT_TPL0401X_ERR_ILLDEV = 0xF7,  
	POT_TPL0401X_ERR_TRNFER,
} POTTPL0401ERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL           PotTPL0401X_Initialize( void );
extern  POTTPL0401ERR  PotTPL0401X_SetLevel( POTTPL0401XDEV eDevice, U8 nValue );
extern  POTTPL0401ERR  PotTPL0401X_GetLevel( POTTPL0401XDEV eDevice, PU8 pnValue );

/**@} EOF TPL0401XDigitalPot.h */

#endif  // _x_H