/******************************************************************************
 * @file I2C_cfg.c
 *
 * @brief I2C configuraiton implementation
 *
 * This file pvoides the I2C configuration implementation
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "I2C/I2c_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// fill out the configuration table
const I2CDEF g_atI2cDefs[ I2C_DEV_ENUM_MAX ] = 
{
  // add items below using the below macros
  // I2C_DEVICE_BLKING( device, speed )
};

/**@} EOF I2c_cfg.c */
