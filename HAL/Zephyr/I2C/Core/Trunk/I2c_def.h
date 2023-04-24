/******************************************************************************
 * @file I2c_def.h
 *
 * @brief IC@ definitions
 *
 * This file provides the definitions for the I2C hal module
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
 * \addtogroup I2c
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _I2C_DEF_H
#define _I2C_DEF_H

// system includes ------------------------------------------------------------
#include <zephyr.h>
#include <devicetree.h>
#include <drivers/i2c.h>

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
#define I2C_DEVICE_BLKING( device, speed ) \
  { \
    .pcName = DT_LABEL( DT_ALIAS( device )), \
    .eSpeedSel = speed, \
  }
  
// enumerations ---------------------------------------------------------------
/// enumerate the speed
typedef enum _I2CSPDSEL
{
  I2C_SPDSEL_100KHZ = 0,      ///< standard 100khz
  I2C_SPDSEL_400KHZ,          ///< fast 400khz
  I2C_SPDSEL_1MHZ,            ///< fast plsu 1MHZ
  I2C_SPDSEL_3MHZ4,           ///< high speed 3.4MHZ
  I2C_SPDSEL_5MHZ,            ///< ultra high speed 5MHZ
  I2C_SPDSEL_MAX
} I2CSPDSEL;

// structures -----------------------------------------------------------------
/// define the pin definition structure
typedef struct _I2CDEF
{ 
  PC8       pcName;
  I2CSPDSEL eSpeedSel;
} I2CDEF, *PI2CDEF;
#define I2CDEF_SIZE                             sizeof( I2CDEF )

/**@} EOF I2c_def.h */

#endif // _I2C_DEF_H