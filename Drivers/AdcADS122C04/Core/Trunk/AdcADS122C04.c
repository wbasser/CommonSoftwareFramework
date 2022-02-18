/******************************************************************************
 * @file AdcADS122C04.c
 *
 * @brief ADC ADS122C04 implementation
 *
 * This file provides the implementation
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
#include "AdcADS122C04/AdcADS122C04.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function AdcADS122C04_Initialize
 *
 * @brief initializaiton
 *
 * This function will perform any needed initialization
 *
 * @return      TRUE if errors found, FALSE otherwise
 *
 *****************************************************************************/
BOOL AdcADS122C04_Initialize( void )
{
  BOOL bStatus = FALSE;
  
  // call the local initialization
  bStatus = AdcADS122C04_LocalInitialize( );
  
  // send a reset command
  bStatus = AdcADS122C04_WriteCommand( ADCADS122C04_CMD_RESET );
  
  // return the status
  return( bStatus );
}

/******************************************************************************
 * @function AdcADS122C04_StartConversion
 *
 * @brief start a conversion
 *
 * This function will start a conversion 
 *
 * @param[in]   eChan   channel enumeration
 *
 * @return      appropriate error
 *
 *****************************************************************************/
ADCADS122C04ERR AdcADS122C04_StartConversion( ADCADS122C04CHANENUM eChan )
{
  ADCADS122C04ERR     eError = ADCADS122C04_ERR_NONE;
  PADCADS122C04DEF    ptDef;
  ADCADS122C04CFGREGS tRegs;
  BOOL                bErrorDetected;
  
  // check for a valid channel
  if ( eChan < ADCADS122C04_CHAN_ENUM_MAX )
  {
    // get the channel definition
    ptDef = ( PADCADS122C04DEF )&g_atAdcADS122C04Defs[ eChan ];
    
    // now fill the config register 0
    tRegs.tConfig0.nByte = 0;
    tRegs.tConfig0.tFields.eGain = ptDef->eGain;
    tRegs.tConfig0.tFields.eMux = ptDef->eMux;
    tRegs.tConfig0.tFields.bPgaBypass = OFF;
   
    // now fill the config register 1
    tRegs.tConfig1.nByte = 0;
    tRegs.tConfig1.tFields.eSampRate = ptDef->eSampRate;
    tRegs.tConfig1.tFields.eVrefSel = ptDef->eVrefSel;

    // now fill the config register 2
    tRegs.tConfig2.nByte = 0;

    // now fill the config register 2
    tRegs.tConfig3.nByte = 0;

    // write config 0
    if (( bErrorDetected = AdcADS122C04_WriteRegister( ADCADS122C04REG_ENUM_CONFIG0, tRegs.tConfig0.nByte )) == FALSE )
    {
      // write config 1
      if (( bErrorDetected = AdcADS122C04_WriteRegister( ADCADS122C04REG_ENUM_CONFIG1, tRegs.tConfig1.nByte )) == FALSE )
      {
        // write config 2
        if (( bErrorDetected = AdcADS122C04_WriteRegister( ADCADS122C04REG_ENUM_CONFIG2, tRegs.tConfig2.nByte )) == FALSE )
        {
          // write config 3
          if (( bErrorDetected = AdcADS122C04_WriteRegister( ADCADS122C04REG_ENUM_CONFIG2, tRegs.tConfig2.nByte )) == FALSE )
          {
            // now start the conversion
            bErrorDetected = AdcADS122C04_WriteCommand( ADCADS122C04_CMD_START );
          }
        }
      }
    }

    // set the appropriate error
    eError = ( bErrorDetected ) ? ADCADS122C04_ERR_BUSERROR : ADCADS122C04_ERR_NONE;
  }
  else
  {
    // set the error
    eError = ADCADS122C04_ERR_ILLCHAN;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function AdcADS122C04_GetResults
 *
 * @brief get a conversion result
 *
 * This function will get the conversion result
 *
 * @param[in]   puValue   pointer to store the result in
 *
 * @return      appropriate error
 *
 *****************************************************************************/
ADCADS122C04ERR AdcADS122C04_GetResults( PU32 puValue )
{
  ADCADS122C04ERR eError = ADCADS122C04_ERR_NONE;

  // test for data ready
  if ( AdcADS122C04_GetDataReady( ))
  {
    // read the data value
    if ( AdcADS122C04_ReadData( puValue ) == TRUE )
    {
      // set the data error status
      eError = ADCADS122C04_ERR_BUSERROR;
    }
  }
  else
  {
    // set the data not ready error
    eError = ADCADS122C04_ERR_DATANOTREADY;
  }

  // return the error
  return( eError );
}

/**@} EOF AdcADS122C04.c */
