/******************************************************************************
 * @file Spi.c
 *
 * @brief SPI implementation
 *
 * This file provides the implementation of the SPI
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
 * \addtogroup 
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SPI/Spi.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

/// Enable/disable this modules logging 
//#define HALSPI_PRINTK(...) printk (__VA_ARGS__);
#define HALSPI_PRINTK(...)

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTL
{
  const struct device*  ptSpi;
  struct spi_config     tSpiCfg;
  BOOL                  bOkay;
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE         sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL atLclCtls[ SPI_DEV_ENUM_MAX ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Spi_Initialize
 *
 * @brief SPI initialization
 *
 * This function initializes all devices in the table
 *
 * @return  TRUE if errors detected, FALSE if otherwise
 *
 *****************************************************************************/
BOOL Spi_Initialize( void )
{
  BOOL        bStatus = FALSE;
  SPIDEVENUM  eDev;
  PSPIDEF     ptDef = NULL;
  PLCLCTL     ptCtl = NULL;
  
  // for each device
  for ( eDev = 0; eDev < SPI_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PSPIDEF )&g_atSpiDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // clear the ok flag
    ptCtl->bOkay = FALSE;
    
    // get the bindings
    if (( ptCtl->ptSpi = device_get_binding( ptDef->pcName )) != NULL )
    {
      HALSPI_PRINTK("SpiDef: Name: %s \n", ptDef->pcName);
      HALSPI_PRINTK("SpiDef: Freq: %d \n", ptDef->eFrequency);

      // clear the operational flags
      ptCtl->tSpiCfg.operation = ptDef->wOperation;

      // configure this instance of spi
      switch (ptDef->eFrequency)
      {
        case SPIFREQUENCY_8M:
          ptCtl->tSpiCfg.frequency = 8000000; // SPIM_FREQUENCY_FREQUENCY_M8;
          break;
          
        case SPIFREQUENCY_4M:
          ptCtl->tSpiCfg.frequency = 4000000; // SPIM_FREQUENCY_FREQUENCY_M4;
          break;

        case SPIFREQUENCY_1M:
        default :
          ptCtl->tSpiCfg.frequency = 1000000; // SPIM_FREQUENCY_FREQUENCY_M1;
          break;
      }
      
      // Spi users do their own chip select control. 
      ptCtl->tSpiCfg.cs = NULL; 

      // set the ok flag
        ptCtl->bOkay = TRUE;
    }
    else
    {
      HALSPI_PRINTK("Spi: binding null \n");

      // set error flag
      bStatus |= TRUE;
    }
  }
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function Spi_Write
 *
 * @brief SPI write a data set to the spi interface.
 * There is no data length limitation when using the zephyr platform.
 * Data is chunked up in the underlying implementation for us. 
 *
 * @return  TRUE if errors detected, FALSE if otherwise
 *
 *****************************************************************************/
SPIERROR Spi_Write(SPIDEVENUM eDev, PSPIBUFFER_SET ptSet)
{
  SPIERROR 	  eError = SPI_ERR_NONE;
  PSPIDEF 	  ptDef;
  PLCLCTL 	  ptCtl;

  S32 status = 0; 

  // check for a valid SPI
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get our def and control structures
    ptDef = (PSPIDEF)&g_atSpiDefs[eDev];
    ptCtl = &atLclCtls[eDev];

    if (ptCtl->bOkay)
    {
      status = spi_write(ptCtl->ptSpi, &ptCtl->tSpiCfg, ptSet);

      HALSPI_PRINTK("spi_write %d \n", status); 

      if (status != SPI_ERR_NONE)
      {
        eError = SPI_ERR_BUSFAULT;
      }
    }
    else 
    {
      // Error
      eError = SPI_ERR_DEVERR;
    }
  }
  else 
  {
    // device out of range 
    eError = SPI_ERR_ILLDEV;
  }

  return (eError);


}

/******************************************************************************
 * @function Spi_Read
 *
 * @brief SPI read
 *
 * @return  TRUE if errors detected, FALSE if otherwise
 *
 *****************************************************************************/
SPIERROR Spi_Read(SPIDEVENUM eDev, PSPIBUFFER_SET ptSet)
{
  SPIERROR 	  eError = SPI_ERR_NONE;
  PSPIDEF 	  ptDef;
  PLCLCTL 	  ptCtl;

  S32 status = 0; 

  // check for a valid SPI
  if ( eDev < SPI_DEV_ENUM_MAX )
  {
    // get our def and control structures
    ptDef = (PSPIDEF)&g_atSpiDefs[eDev];
    ptCtl = &atLclCtls[eDev];

    if (ptCtl->bOkay)
    {
      status = spi_read(ptCtl->ptSpi, &ptCtl->tSpiCfg, ptSet);

      HALSPI_PRINTK("spi_read %d \n", status); 

      if (status != SPI_ERR_NONE)
      {
        eError = SPI_ERR_BUSFAULT;
      }
    }
    else 
    {
      // Error
      eError = SPI_ERR_DEVERR;
    }
  }
  else 
  {
    // device out of range 
    eError = SPI_ERR_ILLDEV;
  }

  return (eError);

}

/**@} EOF Spi.c */

