/******************************************************************************
 * @file LogHandler_cfg.c
 *
 * @brief log handler configuration implementation
 *
 * This file provides the implementation of the log handler configuration
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup LogHandler
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "LogHandler/LogHandler_cfg.h"

// library includes -----------------------------------------------------------
#include "EepromHandler/Eepromhandler.h"

// global parameter declarations ----------------------------------------------
const CODE  LOGENTRY g_atLogEntries[ LOG_TYPE_MAX ] = 
{
  LOGENTRY_CREATE( "Initilization" ),
  LOGENTRY_CREATE( "System Reset" ),
  LOGENTRY_CREATE( "Config Reset" ),
  /// create log entries here
  
};

/******************************************************************************
 * @function LogHandler_GetSystemTime
 *
 * @brief gets the system time
 *
 * This function respond with the current system time
 *
 * @return      system time in USEC
 *
 *****************************************************************************/
U32 LogHandler_GetSystemTime( void )
{
  U32 uTime = 0xBEEFDEAD;
  
  return( uTime ); 
}

/******************************************************************************
 * @function LogHandler_RdWord
 *
 * @brief read a word from the eeprom
 *
 * This function will return read a word from the EEPROM
 *
 * @param[in]     wAddress        address to read
 * @param[io]     pwValue         pointer to store the value read
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL LogHandler_RdWord( U16 wAddress, PU16 pwValue )
{
  BOOL bError = FALSE;

  // return the error  // read a word from the eeprom
  bError = EepromHandler_RdWord( wAddress, pwValue );

  return( bError );
}

/******************************************************************************
 * @function LogHandler_RdBlock
 *
 * @brief read a block from the eeprom
 *
 * This function will read a block from the EEPROM
 *
 * @param[in]     wAddress        address to read
 * @param[io]     pnValue         pointer to store the value read
 * @param[in]     wLength         length to read
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL LogHandler_RdBlock( U16 wAddress, PU8 pnData, U16 wLength )
{
  BOOL bError = FALSE;

  // read a word from the eeprom
  bError = EepromHandler_RdBlock( wAddress, wLength, pnData );

  // return the error
  return( bError );
}

/******************************************************************************
 * @function LogHandler_WrWord
 *
 * @brief write a word from the eeprom
 *
 * This function will return write a word to the EEPROM
 *
 * @param[in]     wAddress        address to write
 * @param[io]     wValue          the value to write
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL LogHandler_WrWord( U16 wAddress, U16 wValue )
{
  BOOL bError = FALSE;

  // read a word from the eeprom
  bError = EepromHandler_WrWord( wAddress, wValue );

  // return the error
  return( bError );
}

/******************************************************************************
 * @function LogHandler_WrBlock
 *
 * @brief write a block to the eeprom
 *
 * This function will wriute a block to the EEPROM
 *
 * @param[in]     wAddress        address to write
 * @param[io]     pnValue         pointer to store the value write
 * @param[in]     wLength         length to write
 *
 * @return        TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL LogHandler_WrBlock( U16 wAddress, PU8 pnData, U16 wLength )
{
  BOOL bError = FALSE;

  // read a word from the eeprom
  bError = EepromHandler_WrBlock( wAddress, wLength, pnData );

  // return the error
  return( bError );
}

/**@} EOF LogHandler_cfg.c */
