/******************************************************************************
 * @file PotTPL0401X.c
 *
 * @brief TPL0401X Digital pot implementation
 *
 * This file provides the implementation for TPL0401X digital pot device driver
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
 * \addtogroup PotTPL0401X
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "PotTPL0401X/PotTPL0401X.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
static  const CODE  U8  anDeviceAddress[ POT_TPL0401X_DEV_MAX ] =
{
  0x2E,         ///< Type A device
  0x3E,         ///< Type B device
};

/******************************************************************************
 * @function PotTPL0401X_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization 
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL PotTPL0401X_Initialize( void )
{
  // return ok
  return( FALSE );
}

/******************************************************************************
 * @function PotTPL0401X_SetLevel
 *
 * @brief set a level
 *
 * This function will validate the device, and write the value to the device
 *
 * @param[in]   eDevice       device enumeration
 * @param[in]   nValue        value to write
 *
 * @return      appropriate error
 *
 *****************************************************************************/
POTTPL0401ERR PotTPL0401X_SetLevel( POTTPL0401XDEV eDevice, U8 nValue )
{
  POTTPL0401ERR  eError = POT_TPL0401X_ERR_NONE;
  
  // test for valid device
  if ( eDevice < POT_TPL0401X_DEV_MAX )
  {
		// write the register
		eError =  ( PotTPL0401X_WriteReg( anDeviceAddress[ eDevice ], nValue )) ? POT_TPL0401X_ERR_TRNFER : POT_TPL0401X_ERR_NONE;
  }
  else
  {
    // set the error
    eError = POT_TPL0401X_ERR_ILLDEV;
  }    

  // return the error
  return( eError );  
}

/******************************************************************************
 * @function PotTPL0401X_GetLevel
 *
 * @brief read a value
 *
 * This function will validate the devie and read the value from that device
 *
 * @param[in]   eDevice       device enumeration
 * @param[in]   pnValue       pointer to store the read value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
POTTPL0401ERR PotTPL0401X_GetLevel( POTTPL0401XDEV eDevice, PU8 pnValue )
{
  POTTPL0401ERR  eError = POT_TPL0401X_ERR_NONE;
  
  // test for valid device
  if ( eDevice < POT_TPL0401X_DEV_MAX )
  {
		// write the register
		eError =  ( PotTPL0401X_ReadReg( anDeviceAddress[ eDevice ], pnValue )) ? POT_TPL0401X_ERR_TRNFER : POT_TPL0401X_ERR_NONE;
  }
  else
  {
    // set the error
    eError = POT_TPL0401X_ERR_ILLDEV;
  }

  // return the error
  return( eError );
}
 
/**@} EOF PotTPL0401X.c */
