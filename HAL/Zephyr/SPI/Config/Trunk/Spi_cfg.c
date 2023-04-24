/******************************************************************************
 * @file SPI_cfg.c
 *
 * @brief SPI configuraiton implementation
 *
 * This file pvoides the SPI configuration implementation
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
 * \addtogroup SPI
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// fill out the configuration table
const SPIDEF g_atSpiDefs[ SPI_DEV_ENUM_MAX ] = 
{
  // add items below using the below macros
  SPI_DEVICE(flsspi, \
             (SPI_WORD_SET(8) | SPI_TRANSFER_MSB | \
              SPI_OP_MODE_MASTER | SPI_LINES_SINGLE), \
             SPIFREQUENCY_8M, 0), \
};

/**@} EOF Spi_cfg.c */ 
