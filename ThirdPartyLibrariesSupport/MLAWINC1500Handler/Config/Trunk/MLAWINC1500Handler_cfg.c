/******************************************************************************
 * @file MLAWINC1500Handler_cfg.c
 *
 * @brief MLA WINC1500 hahandler configuration implementation
 *
 * This file provides the implementation for the MLA 1500 handler
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
 * \addtogroup MLAWINC1500Handler_cfg
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "MLAWINC1500Handler/MLAWINC1500Handler.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
/// instantiate the default values
static  const MLAWINC1500WIFICONFIG  tWifiConfigDflt =
{
  { "CyberDevLcl24" },
  { "Cyber&%%)"     },
};

/******************************************************************************
 * @function MLAWINC1500Handler_LocalInitialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void MLAWINC1500Handler_LocalInitialize( void )
{
}

/******************************************************************************
 * @function WINC1500Handler_ProcessTask
 *
 * @brief process task
 *
 * This function will process the task
 *
 * @param[in]   xArg    task argument
 *
 * @return      TRUE to flush the events
 *
 *****************************************************************************/
BOOL WINC1500Handler_ProcessTask( TASKARG xArg )
{
  // call the task handler
  MLAWINC1500Handler_IdleTask( );
 
  // return TRUE
  return( TRUE );
}

/******************************************************************************
 * @function MLAWINC1500Handler_GetDefaultConfig
 *
 * @brief get the default config
 *
 * This function will return the pointer to the default config
 *
 * @return      pointer to the default
 *
 *****************************************************************************/
PVGETPOINTER MLAWINC1500Handler_GetDefaultConfig( void )
{
  // return the default parameters
  return(( PVGETPOINTER )&tWifiConfigDflt );
}

/******************************************************************************
 * @function MLAWINC1500Handler_GetConfigSize
 *
 * @brief get config size
 *
 * This function will get the configuration size
 *
 * @return  size of the configuration block
 *
 *****************************************************************************/
U16 MLAWINC1500Handler_GetConfigSize( void )
{
  // return the config size
  return( MLAWINC1500WIFICONFIG_SIZE );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
 
/**@} EOF MLAWINC1500Handler_cfg.c */
