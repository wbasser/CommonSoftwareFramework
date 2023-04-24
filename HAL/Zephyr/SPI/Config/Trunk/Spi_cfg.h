/******************************************************************************
* @file Spi_cfg.h
 *
 * @brief SPI configuration declarations
 *
 * This file contains the SPI configuration declarations
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
 
// ensure only one instatiation
#ifndef _SPI_CFG_H
#define _SPI_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// How long to wait since last byte received before issuing a timeout
#define SPI_RX_TIMEOUT_MS 10
#define SPI_TX_TIMEOUT_MS 10

/// SPI RX/TX buf size 
/// Documented max spi transfer for nRF52832 
#define SPI_BUF_SIZE      255 

// enumerations ---------------------------------------------------------------
/// declare the SPI enuemrations
typedef enum  _SPIDEVENUM
{
  // add enumerations below
  SPI_DEV_ENUM_FLASHSPI = 0,
 
  // do not remove the below items
  SPI_DEV_ENUM_MAX,
  SPI_DEV_ENUM_ILLEGAL = 0xFF
} SPIDEVENUM;

// structures -----------------------------------------------------------------
 
// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern const SPIDEF g_atSpiDefs[ ];

/**@} EOF Spi_cfg.h */

#endif  // _SPI_CFG_H
