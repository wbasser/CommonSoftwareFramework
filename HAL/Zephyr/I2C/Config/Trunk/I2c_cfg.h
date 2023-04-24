/******************************************************************************
* @file I2C_cfg.h
 *
 * @brief I2C configuration declarations
 *
 * This file contains the I2C configuration declarations
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
 
// ensure only one instatiation
#ifndef _I2C_CFG_H
#define _I2C_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// declare the I2C enuemrations
typedef enum  _I2CDEVENUM
{
  // add enumerations below
 
  // do not remove the below items
  I2C_DEV_ENUM_MAX,
  I2C_DEV_ENUM_ILLEGAL
} I2CDEVENUM;

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  const I2CDEF g_atI2cDefs[ ];

/**@} EOF I2c_cfg.h */

#endif  // _I2C_CFG_H
