/******************************************************************************
 * @file NetworkLocator_cfg.c
 *
 * @brief  network locator configuration implementation
 *
 * This file provides the implementation for the network location configuration
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
 * \addtogroup NetworkLocator
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "NetowrkLocator/NetworkLocator.h"

// library includes -----------------------------------------------------------
#include "ManufInfo/ManufInfo.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
static  const CODE NETWORKLOCATORCFG tConfigDflt =
{
  .acName = "DeviceName",
  .acKey  = "DEADBEEF_GARBAGE$",
  .wPort  = 7600
};

/******************************************************************************
 * @function NetworkLocator_LocalInitialize
 *
 * @brief initialization
 *
 * This function will perform any needed initilaization
 *
 *****************************************************************************/
void NetworkLocator_LocalInitialize( void )
{
}

/******************************************************************************
 * @function NetworkLocator_GetConfigDflt
 *
 * @brief get the default config
 *
 * This function will return a pointer to the default config
 *
 * @return      pointer to the default configuration structure
 *
 *****************************************************************************/
PVGETPOINTER NetworkLocator_GetConfigDflt( void )
{
  // return the pointer to the default config
  return( tConfigDflt );
}

/******************************************************************************
 * @function NetworkLocator_GetConfigSize
 *
 * @brief get config size
 *
 * This function will get the configuration size
 *
 * @return  size of the configuration block
 *
 *****************************************************************************/
U16 NetworkLocator_GetConfigSize( void )
{
  // return the config size
  return( NETWORKLOCATORCFG_SIZE );
}


/******************************************************************************
 * @function NetworkLocator_GetHdwMajor
 *
 * @brief get the hardware major revision
 *
 * This function will return the revision
 *
 * @return      revision
 *
 *****************************************************************************/
U8 NetworkLocator_GetHdwMajor( void )
{
  // return the hardware major revision
  return( ManufInfo_GetHdwMajor( ));
}

/******************************************************************************
 * @function NetworkLocator_GetHdwMinor
 *
 * @brief get the hardware minor revision
 *
 * This function will return the hard minor revision
 *
 * @return      revision
 *
 *****************************************************************************/
U8 NetworkLocator_GetHdwMinor( void )
{
  // return the hardware major revision
  return( ManufInfo_GetHdwMinor( ));
}

/******************************************************************************
 * @function NetworkLocator_GetHSfwMajor
 *
 * @brief get the software major revision
 *
 * This function will return the revision
 *
 * @return      revision
 *
 *****************************************************************************/
U8 NetworkLocator_GetSfwMajor( void )
{
  // return revision
  return( ManufInfo_GetSfwMajor( ));
}

/******************************************************************************
 * @function NetworkLocator_GetSfwMinor
 *
 * @brief get the software minor revision
 *
 * This function will return the revision
 *
 * @return      revision
 *
 *****************************************************************************/
U8 NetworkLocator_GetSfwMinor( void )
{
  // return revision
  return( ManufInfo_GetSfwMinor( ));
}

/******************************************************************************
 * @function NetworkLocator_GetHSfwTest
 *
 * @brief get the software test revision
 *
 * This function will return the revision
 *
 * @return      revision
 *
 *****************************************************************************/
 U8 NetworkLocator_GetSfwTest( void )
 {
  // return revision
  return( ManufInfo_GetSfwTest( ));
 }
 
/******************************************************************************
 * @function NetworkLocator_GetTitle
 *
 * @brief get the title
 *
 * This function will return the title
 *
 * @return      title
 *
 *****************************************************************************/
PC8 NetworkLocator_GetTitle( void )
{
  // return the title
  return( ManufInfo_GetTitle( ));
}

/******************************************************************************
 * @function NetworkLocator_UpdateConfig
 *
 * @brief update config
 *
 * This function will update the config
 *
 *
 *****************************************************************************/
void NetworkLocator_UpdateConfig( void )
{
}
 
/**@} EOF NetworkLocator_cfg.c */
