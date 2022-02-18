/******************************************************************************
 * @file IoePCA9555_def.h
 *
 * @brief PCA9555 port expander driver definitions 
 *
 * This file provides the definitions for the PCA 9534
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
 * \addtogroup IoePCA9555
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _IOEPCA9555_DEF_H
#define _IOEPCA9555_DEF_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// define the helper macro for creating a device
#define DEFIOEPCA9555DEVM( addr, dir00, inv00, init00, dir01, inv01, init01, dir02, inv02, init02, dir03, inv03, init03, dir04, inv04, init04, dir05, inv05, init05, dir06, inv06, init06, dir07, inv07, init07, \
                                 dir08, inv08, init08, dir09, inv09, init09, dir10, inv10, init10, dir11, inv11, init11, dir12, inv12, init12, dir13, inv13, init13, dir14, inv14, init14, dir15, inv15, init15 ) \
  { \
    .eDeviceAddress = addr, \
    .atPinDefs[ 0 ] = \
    { \
      .eDirection   = dir00, \
      .bInvert      = inv00, \
      .bInitial     = init00 \
    }, \
    .atPinDefs[ 1 ] = \
    { \
      .eDirection   = dir01, \
      .bInvert      = inv01, \
      .bInitial     = init01 \
    }, \
    .atPinDefs[ 2 ] = \
    { \
      .eDirection   = dir02, \
      .bInvert      = inv02, \
      .bInitial     = init02 \
    }, \
    .atPinDefs[ 3 ] = \
    { \
      .eDirection   = dir03, \
      .bInvert      = inv03, \
      .bInitial     = init03 \
    }, \
    .atPinDefs[ 4 ] = \
    { \
      .eDirection   = dir04, \
      .bInvert      = inv04, \
      .bInitial     = init04 \
    }, \
    .atPinDefs[ 5 ] = \
    { \
      .eDirection   = dir05, \
      .bInvert      = inv05, \
      .bInitial     = init05 \
    }, \
    .atPinDefs[ 6 ] = \
    { \
      .eDirection   = dir06, \
      .bInvert      = inv06, \
      .bInitial     = init06 \
    }, \
    .atPinDefs[ 7 ] = \
    { \
      .eDirection   = dir07, \
      .bInvert      = inv07, \
      .bInitial     = init07 \
    }, \
    .atPinDefs[ 8 ] = \
    { \
      .eDirection   = dir08, \
      .bInvert      = inv08, \
      .bInitial     = init08 \
    }, \
    .atPinDefs[ 9 ] = \
    { \
      .eDirection   = dir09, \
      .bInvert      = inv09, \
      .bInitial     = init09 \
    }, \
    .atPinDefs[ 10 ] = \
    { \
      .eDirection   = dir10, \
      .bInvert      = inv10, \
      .bInitial     = init10 \
    }, \
    .atPinDefs[ 11 ] = \
    { \
      .eDirection   = dir11, \
      .bInvert      = inv11, \
      .bInitial     = init11 \
    }, \
    .atPinDefs[ 12 ] = \
    { \
      .eDirection   = dir12, \
      .bInvert      = inv12, \
      .bInitial     = init12 \
    }, \
    .atPinDefs[ 13 ] = \
    { \
      .eDirection   = dir13, \
      .bInvert      = inv13, \
      .bInitial     = init13 \
    }, \
    .atPinDefs[ 14 ] = \
    { \
      .eDirection   = dir14, \
      .bInvert      = inv14, \
      .bInitial     = init14 \
    }, \
    .atPinDefs[ 15 ] = \
    { \
      .eDirection   = dir15, \
      .bInvert      = inv15, \
      .bInitial     = init15 \
    }, \
  } \

// enumerations ---------------------------------------------------------------
/// enumerate the direction
typedef enum _IOEPCA9555DIRENUM
{
  IOEPCA9555_DIR_OUT = 0,
  IOEPCA9555_DIR_IN,
  IOEPCA9555_DIR_MAX
} IOEPCA9555DIRENUM;

/// enumerate the device addresses
typedef enum  _IOEPCA9555DEVADDR
{
  IOEPCA9555_DEV_ADDR_20 = 0x20,     ///< base device
  IOEPCA9555_DEV_ADDR_21,            ///< base device + 1
  IOEPCA9555_DEV_ADDR_22,            ///< base device + 2
  IOEPCA9555_DEV_ADDR_23,            ///< base device + 3
  IOEPCA9555_DEV_ADDR_24,            ///< base device + 4
  IOEPCA9555_DEV_ADDR_25,            ///< base device + 5
  IOEPCA9555_DEV_ADDR_26,            ///< base device + 6
  IOEPCA9555_DEV_ADDR_27,            ///< base device + 7
} IOEPCA9555DEVADDR;

/// enuemrate the pins
typedef enum _IOEPCA9555PINENUM
{
  IOEPCA9555_PIN_ENUM_00 = 0,       ///< pin 0
  IOEPCA9555_PIN_ENUM_01,           ///< pin 1
  IOEPCA9555_PIN_ENUM_02,           ///< pin 2
  IOEPCA9555_PIN_ENUM_03,           ///< pin 3
  IOEPCA9555_PIN_ENUM_04,           ///< pin 4
  IOEPCA9555_PIN_ENUM_05,           ///< pin 5
  IOEPCA9555_PIN_ENUM_06,           ///< pin 6
  IOEPCA9555_PIN_ENUM_07,           ///< pin 7
  IOEPCA9555_PIN_ENUM_08,           ///< pin 8
  IOEPCA9555_PIN_ENUM_09,           ///< pin 9
  IOEPCA9555_PIN_ENUM_10,           ///< pin 10
  IOEPCA9555_PIN_ENUM_11,           ///< pin 11
  IOEPCA9555_PIN_ENUM_12,           ///< pin 12
  IOEPCA9555_PIN_ENUM_13,           ///< pin 13
  IOEPCA9555_PIN_ENUM_14,           ///< pin 14
  IOEPCA9555_PIN_ENUM_15,           ///< pin 15
  IOEPCA9555_PIN_ENUM_MAX
} IOEPCA9555PINENUM;

// structures -----------------------------------------------------------------
/// define the pin defnition structure
typedef struct _IOEPCA9555PINDEF
{
  IOEPCA9555DIRENUM eDirection;     ///< direction
  BOOL              bInvert;        ///< invert
  BOOL              bInitial;       ///< initial state
} IOEPCA9555PINDEF, *PIOEPCA9555PINDEF;
#define IOEPCA9555PINDEF_SIZE         sizeof( IOEPCA9555PINDEF )

/// define the device deinition structure
typedef struct _IOEPCA9555DEVDEF
{
  IOEPCA9555DEVADDR  eDeviceAddress;                 ///< device address
  IOEPCA9555PINDEF   atPinDefs[ IOEPCA9555_PIN_ENUM_MAX ];  ///< pin definitions
} IOEPCA9555DEVDEF, *PIOEPCA9555DEVDEF;
#define IOEPCA9555DEVDEF_SIZE          sizeof( IOEPCA9555DEVDEF )

/**@} EOF IOEPCA9555_def.h */

#endif  // _IOEPCA9555_DEF_H