/******************************************************************************
 * @file IOEPCA9555.h
 *
 * @brief IOEPCA9555 port expander delcrations
 *
 * This file provides the declarations for the module
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
#ifndef _IOEPCA9555_H
#define _IOEPCA9555_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IoePCA9555/IoePCA9555_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _IOEPCA9555ERR
{
  IOEPCA9555_ERR_NONE = 0,         ///< no error
  IOEPCA9555_ERR_ILLPIN = 0x80,    ///< illegal pin enumeration
  IOEPCA9555_ERR_ILLDEV,           ///< illegal device enumeration
  IOEPCA9555_ERR_ILLACT,           ///< illegal action, output on an input pin
  IOEPCA9555_ERR_I2CXFR,           ///< error on I2C transfer
} IOEPCA9555ERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL            IoePCA9555_Initialize( void );
extern  IOEPCA9555ERR   IoePCA9555_SetOutput( IOEPCA9555DEVENUM eDeviceEnum, IOEPCA9555PINENUM ePinEnum, BOOL bState );
extern  IOEPCA9555ERR   IoePCA9555_GetInput( IOEPCA9555DEVENUM eDeviceEnum, IOEPCA9555PINENUM ePinEnum, PBOOL pbState );
extern  IOEPCA9555ERR   IoePCA9555_SetAll( IOEPCA9555DEVENUM eDeviceEnum, U16 wValue );
extern  IOEPCA9555ERR   IoePCA9555_GetAll( IOEPCA9555DEVENUM eDeviceEnum, PU16 pwValue );
 
/**@} EOF IoePCA9555.h */

#endif  // _IOEPCA9555_H