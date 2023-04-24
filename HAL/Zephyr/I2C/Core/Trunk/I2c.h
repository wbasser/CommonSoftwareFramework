/******************************************************************************
 * @file I2C.h
 *
 * @brief I2C sercom declarations
 *
 * This file provides the declarations for the I2C sercom module
 *
 * @copyright Copyright (c) 2021 Strongarm Tech Inc
 * The copyright to the computer software herein is the property of 
 * Strongarm Tech Inc. This software may not be used and/or copied without
 * explicit written permission of Strongarm Tech Inc., and only in accordance
 * with the terms and conditions stipulated in the license agreement and/or
 * contract under which the software has been supplied.
 *
 * Version History
 * ======
 *
 * \addtogroup I2C
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _I2C_H
#define _I2C_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _I2CERROROR
{
  I2C_ERROR_NONE = 0,       ///< no error
  I2C_ERROR_BLKING,         ///< device is blocked
  I2C_ERROR_ILLDEV = 0xE0,  ///< illegal device
  I2C_ERROR_ILLPRM,         ///< illegal parameter
  I2C_ERROR_ILLACT,         ///< illegal IOCTL action
  I2C_ERROR_TIMEOUT,        ///< timeout
  I2C_ERROR_BUSBUSY,        ///< bus busy
  I2C_ERROR_BUSFAULT,       ///< bus fault
  I2C_ERROR_ARBLOST,        ///< arbitration lost
  I2C_ERROR_SLVNAK,         ///< slave NAK
  I2C_ERROR_DEVERR,         ///< device error
} I2CERROR;

/// enumerate the IOCTL actions
typedef enum _I2CACTION
{
  I2C_ACTION_NONE = 0,    ///< no action
  I2C_ACTION_POLL_DEVICE, ///< poll device
  I2C_ACTION_SET_DEVADDR, ///< set device address
  I2C_ACTION_GET_DEVADDR, ///< get device address
  I2C_ACTION_MAX          ///< maximum action selection
} I2CACTION;

// structures -----------------------------------------------------------------
typedef struct _I2CXFRCTL
{
  U8    nDevAddr;       ///< device address
  U8    nAddrLen;       ///< address length
  U32UN tAddress;       ///< address
  PU8   pnData;         ///< pointer to the data
  U16   wDataLen;       ///< data length
  U32   uTimeout;       ///< timeout in MSEC
} I2CXFRCTL, *PI2CXFRCTL;
#define I2CXFRCTL_SIZE                          sizeof(I2CXFRCTL)

/// define the check for busy structure
typedef struct _I2CCHKBSY
{
  U8    nDevAddr;       ///< address
  BOOL  bReadMode;      ///< read mode
} I2CCHKBSY, *PI2CCHKBSY;
#define I2CCHKBSY_SIZE                          sizeof(I2CCHKBSY)

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern BOOL     I2c_Initialize( void );
extern void     I2c_CloseAll( void );
extern I2CERROR I2c_Write( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl );
extern I2CERROR I2c_Read( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl );
extern I2CERROR I2c_Ioctl( I2CDEVENUM eDev, I2CACTION eAction, PVOID pvData );
extern I2CERROR I2c_Close( I2CDEVENUM eDev );

/**@} EOF I2c.h */

#endif  // _I2C_H