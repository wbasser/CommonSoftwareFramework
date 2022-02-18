/******************************************************************************
 * @file RtcPCF8523_cfg.c
 *
 * @brief Real Time Clock PCV8523 implementation
 *
 * This file provides the implementation for the configuration of the PCF8523
 * real time clock
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup RtcPCF8523
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RtcPCF8523/RtcPCF8523_cfg.h

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function RtcPCF8523_LocalInitialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void RtcPCF8523_LocalInitialize( void )
{
}

/******************************************************************************
 * @function RtcPCF8523_WriteRegisters
 *
 * @brief write registers
 *
 * This function will write the register(s) with the supplied dat
 *
 * @param[in]   nBaseRegister     base register
 * @param[in]   nNumberRegisters  number of registers to write
 * @param[in]   pnData            pointer to the data to write
 *
 * @return      appropriate error
 *
 *****************************************************************************/
RTCPCF8523ERROR RtcPCF8523_WriteRegisters( U8 nBaseRegister, U8 nNumRegisters, PU8 pnData )
{
  RTCPCF8523ERROR eError = RTCPCF8523_ERR_XFR;
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function RtcPCF8523_ReadRegisters
 *
 * @brief read registers
 *
 * This function will read the register(s) with the supplied dat
 *
 * @param[in]   nBaseRegister     base register
 * @param[in]   nNumberRegisters  number of registers to read
 * @param[io]   pnData            pointer to data storage
 *
 * @return      appropriate error
 *
 *****************************************************************************/
RTCPCF8523ERROR RtcPCF8523_ReadRegisters( U8 nBaseRegister, U8 nNumRegisters, PU8 pnData )
{
  RTCPCF8523ERROR eError = RTCPCF8523_ERR_XFR;
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function RtcPCF8523_IrqCallback
 *
 * @brief IRQ callback
 *
 * This function will handler the IRQ callback
 *
 * @param[in]   nArg1     argument #1
 * @param[in]   nArg2     argument #2
 *
 *****************************************************************************/
void RtcPCF8523_IrqCallback( U8 nArg1, U8 nArg2 )
{
}

/**@} EOF RtcPCF8523_cfg.c */
