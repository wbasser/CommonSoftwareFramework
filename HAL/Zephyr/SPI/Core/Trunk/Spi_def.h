/******************************************************************************
 * @file Spi_def.h
 *
 * @brief IC@ definitions
 *
 * This file provides the definitions for the SPI hal module
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
 * \addtogroup Spi
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _SPI_DEF_H
#define _SPI_DEF_H

// system includes ------------------------------------------------------------
#include <zephyr.h>
#include <devicetree.h>
#include <drivers/spi.h>

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

#define SPIBUFFER struct spi_buf
#define PSPIBUFFER SPIBUFFER*

#define SPIBUFFER_SET struct spi_buf_set
#define PSPIBUFFER_SET SPIBUFFER_SET* 

/// Helper macro to create a spi instance 
#define SPI_DEVICE( device, opflags, freq, slave ) \
  { \
    .pcName       = DT_LABEL( DT_ALIAS( device )), \
    .wOperation   = opflags, \
    .wSlave       = slave, \
    .eFrequency   = freq \
  }

// enumerations ---------------------------------------------------------------

/// Note I am only including freqs only in actual use
typedef enum _SPIFREQUENCY
{
  SPIFREQUENCY_1M,
  SPIFREQUENCY_4M,
  SPIFREQUENCY_8M,
} SPIFREQUENCY;

// structures -----------------------------------------------------------------

typedef struct _SPIDEF
{
  PC8           pcName;     
  U16           wOperation;
  U16           wSlave;
  SPIFREQUENCY  eFrequency;
} SPIDEF, *PSPIDEF;

/**@} EOF Spi_def.h */

#endif // _SPI_DEF_H