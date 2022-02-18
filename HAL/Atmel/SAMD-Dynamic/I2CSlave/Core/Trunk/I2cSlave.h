/******************************************************************************
 * @file I2cSlave.h
 *
 * @brief I2CSLAVE sercom declarations
 *
 * This file provides the declarations for the I2CSLAVE sercom module
 *
 * @copyright Copyright (c) 2014 CyberIntegration
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
 * \addtogroup I2CSLAVESLAVE
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _I2CSLAVESLAVE_H
#define _I2CSLAVESLAVE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _I2CSLAVEERR
{
  I2CSLAVE_ERR_NONE = 0,         ///< no error
  I2CSLAVE_ERR_BLKING,           ///< device is blocked
  I2CSLAVE_ERR_ILLDEV = 0xE0,    ///< illegal device
  I2CSLAVE_ERR_ILLPRM,           ///< illegal parameter
  I2CSLAVE_ERR_ILLACT,           ///< illegal IOCTL action
  I2CSLAVE_ERR_TIMEOUT,          ///< timeout
  I2CSLAVE_ERR_BUSBUSY,          ///< bus busy
  I2CSLAVE_ERR_BUSFAULT,         ///< bus fault
  I2CSLAVE_ERR_ARBLOST,          ///< arbitration lost
  I2CSLAVE_ERR_SLVNAK,           ///< slave NAK
} I2CSLAVEERR;

/// enumerate the IOCTL actions
typedef enum _I2CSLAVEACTION
{
  I2CSLAVE_ACTION_NONE = 0,      ///< no action
  I2CSLAVE_ACTION_SET_DEVADDR,   ///< set device address
  I2CSLAVE_ACTION_GET_DEVADDR,   ///< get device address
  I2CSLAVE_ACTION_MAX            ///< maximum action selection
} I2CSLAVEACTION;

/// enumerate the I2CSLAVE channels
typedef enum _I2CSLAVECHAN
{
  I2CSLAVE_CHAN_0 = 0,          ///< SERCOM channel 0
  I2CSLAVE_CHAN_1,              ///< SERCOM channel 1
  #ifdef SERCOM2
  I2CSLAVE_CHAN_2,              ///< SERCOM channel 2
  #endif // SERCOM2
  #ifdef SERCOM3
  I2CSLAVE_CHAN_3,              ///< SERCOM channel 3
  #endif // SERCOM3
  #ifdef SERCOM4
  I2CSLAVE_CHAN_4,              ///< SERCOM channel 4
  #endif // SERCOM4
  #ifdef SERCOM5
  I2CSLAVE_CHAN_5,              ///< SERCOM channel 5
  #endif // SERCOM5
  I2CSLAVE_CHAN_MAX
} I2CSLAVECHAN;

// structures -----------------------------------------------------------------
/// define the callback type
typedef BOOL	( *PVI2CSLAVERDIRQCALLBACK )( PU8 pnByte );
typedef BOOL  ( *PVI2CSLAVEWRIRQCALLBACK )( U8 nByte );

/// define the task handle
typedef PVOID   PVI2CSLAVEHANDLE;

/// define the structure to define a UART channel
typedef struct _I2CSLAVEDEF
{
  I2CSLAVECHAN            eChan;        ///< I2C channel
  BOOL                    bRunStandby;  ///< run in standby
  BOOL                    bFastSpeed;   ///< fast speed
  GPIOPORT                eDevPort;     ///< device port
  GPIOFUNCMUX             eDevMux;      ///< device mux
  U8                      nSdaPin;      ///< SDA pin
  U8                      nSclPin;      ///< SCL pin
  PVI2CSLAVERDIRQCALLBACK pvRdCallback; ///< read callback
  PVI2CSLAVEWRIRQCALLBACK pvWrCallback; ///< write callback
} I2CSLAVEDEF, *PI2CSLAVEDEF;
#define I2CSLAVEDEF_SIZE        sizeof( I2CSLAVEDEF )


// global function prototypes --------------------------------------------------
extern  PVI2CSLAVEHANDLE I2cSlave_Configure( PI2CSLAVEDEF ptI2cSlaveDef );
extern  I2CSLAVEERR      I2cSlave_Ioctl( PVI2CSLAVEHANDLE pvI2cSlaveHandle, I2CSLAVEACTION eAction, PVOID pvData );
extern  I2CSLAVEERR      I2cSlave_Close( PVI2CSLAVEHANDLE pvI2cSlaveHandle );

/**@} EOF I2cSlave.h */

#endif  // _I2CSLAVE_H