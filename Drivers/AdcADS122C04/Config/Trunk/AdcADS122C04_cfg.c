/******************************************************************************
 * @file AdcADS122C04_cfg.c
 *
 * @brief ADC ADS122C04 definition implementation
 *
 * This file provides the implementation for the configuration
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 *
 * \addtogroup AdcADS122C04
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AdcADS122C04/AdcADS122C04_cfg.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  VBOOL     vbDataReady;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const CODE  ADCADS122C04DEF g_atAdcADS122C04Defs[ ADCADS122C04_CHAN_ENUM_MAX ] =
{
  // populate channels here using the below macro
  // ADCADS122C04DEFM( mux, fain, pgadisable, vrefsel, samprate ) 
};

/******************************************************************************
 * @function AdcADS122C04_LocalInitialize
 *
 * @brief initializaiton
 *
 * This function will perform any needed initialization
 *
 * @return      TRUE if errors found, FALSE otherwise
 *
 *****************************************************************************/
BOOL AdcADS122C04_LocalInitialize( void )
{
  BOOL bStatus = FALSE;

  // clear the dta ready
  vbDataReady = FALSE;
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function AdcADS122C04_WriteCommand
 *
 * @brief write command
 *
 * This function will write a command to the unit
 *
 * @param[in]   nCommand      command to write
 *
 * @return      TRUE if errors found, FALSE otherwise
 *
 *****************************************************************************/
BOOL AdcADS122C04_WriteCommand( U8 nCommand )
{
  BOOL bStatus = FALSE;
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function AdcADS122C04_WriteRegister
 *
 * @brief write registers
 *
 * This function will write some number of registers
 *
 * @param[in]   eReg          register
 * @param[in]   nData         data to write
 *
 * @return      TRUE if errors found, FALSE otherwise
 *
 *****************************************************************************/
BOOL AdcADS122C04_WriteRegister( ADCADS122C04REG eReg, U8 nData )
{
  BOOL bStatus = FALSE;
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function AdcADS122C04_ReadRegister
 *
 * @brief read register
 *
 * This function will read some number of registers
 *
 * @param[in]   eReg          register
 * @param[io]   pnData        pointer to store the data
 *
 * @return      TRUE if errors found, FALSE otherwise
 *
 *****************************************************************************/
BOOL AdcADS122C04_ReadRegister( ADCADS122C04REG eReg, PU8 pnData )
{
  BOOL bStatus = FALSE;
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function AdcADS122C04_ReadData
 *
 * @brief read data
 *
 * This function will read the conversion data
 *
 * @param[io]   pnData      pointer to store the data
 *
 * @return      TRUE if errors found, FALSE otherwise
 *
 *****************************************************************************/
BOOL AdcADS122C04_ReadData( PU32 puData )
{
  BOOL bStatus = FALSE;
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function AdcADS122C04_GetDataReady
 *
 * @brief test for data ready
 *
 * This function will return the state of data ready
 *
 * @return      current state of data ready
 *
 *****************************************************************************/
BOOL AdcADS122C04_GetDataReady( void )
{
  BOOL  bReady;

  // set the return status
  bReady = vbDataReady;

  // clear it
  Interrupt_Disable( );
  vbDataReady = FALSE;
  Interrupt_Enable( );

  // return the status
  return( bReady );
}

/******************************************************************************
 * @function AdcADS122C04_IrqCallback
 *
 * @brief IRQ callback
 *
 * This function will handler the IRQ callback
 *
 * @param[in]   nPin      pin that caused the interrupt
 * @param[in]   nEvent    event
 * @param[in]   bState    state of the pin
 *
 *****************************************************************************/
void AdcADS122C04_IrqCallback( U8 nPin, U8 nEvent, BOOL bState )
{
  // set the data ready flag
  vbDataReady = TRUE;
}

/**@} EOF AdcADS122C04_cfg.c */
