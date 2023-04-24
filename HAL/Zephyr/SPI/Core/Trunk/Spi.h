/******************************************************************************
 * @file SPI.h
 *
 * @brief SPI sercom declarations
 *
 * This file provides the declarations for the SPI sercom module
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

// ensure only one instantiation
#ifndef _SPI_H
#define _SPI_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _SPIERROROR
{
  SPI_ERR_NONE = 0,       ///< no error
  SPI_ERR_BLKING,         ///< device is blocked
  SPI_ERR_ILLDEV,         ///< illegal device
  SPI_ERR_ILLPRM,         ///< illegal parameter
  SPI_ERR_ILLACT,         ///< illegal IOCTL action
  SPI_ERR_TIMEOUT,        ///< timeout
  SPI_ERR_BUSBUSY,        ///< bus busy
  SPI_ERR_BUSFAULT,       ///< bus fault
  SPI_ERR_DEVERR,         ///< device error
} SPIERROR;

// structures -----------------------------------------------------------------
/*
typedef struct _SPIXFRCTL
{
  PU8   pnTxBuf;        ///< pointer to the tx data
  U16    nTxLen;         ///< tx data length
  PU8   pnRxBuf;        ///< pointer to the rx data
  U16    nRxLen;         ///< rx data length
} SPIXFRCTL, *PSPIXFRCTL;
#define SPIXFRCTL_SIZE                          sizeof(SPIXFRCTL)
*/ 

typedef struct _SPIXFRCTL
{
  SPIDEVENUM     eDevice; 
  PSPIBUFFER_SET ptTxSet;        ///< pointer to the tx data
  PSPIBUFFER_SET ptRxSet;        ///< pointer to the rx data
} SPIXFRCTL, *PSPIXFRCTL;
#define SPIXFRCTL_SIZE                          sizeof(SPIXFRCTL)

/// define the check for busy structure
typedef struct _SPICHKBSY
{
  U8    nDevAddr;       ///< address
  BOOL  bReadMode;      ///< read mode
} SPICHKBSY, *PSPICHKBSY;
#define SPICHKBSY_SIZE                          sizeof(SPICHKBSY)

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern BOOL     Spi_Initialize( void );

extern SPIERROR Spi_Write(SPIDEVENUM eDev, PSPIBUFFER_SET ptSet);
extern SPIERROR Spi_Read(SPIDEVENUM eDev, PSPIBUFFER_SET ptSet);

extern SPIERROR Spi_Close( SPIDEVENUM eDev );

/**@} EOF Spi.h */

#endif  // _SPI_H