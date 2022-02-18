/******************************************************************************
 * @file IoePCA9555.c
 *
 * @brief IOEPCA9555 port expander implementation
 *
 * This file provides the implementation for the IOEPCA9555 port expander
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
 * \addtogroup IoePCA9555
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "IoePCA9555/IoePCA9555.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the registers
typedef enum _IOEPCA9555REG
{
  IOEPCA9555_REG_INP0 = 0,      ///< input 
  IOEPCA9555_REG_INP1,          ///< input 
  IOEPCA9555_REG_OUT0,          ///< output
  IOEPCA9555_REG_OUT1,          ///< output
  IOEPCA9555_REG_POL0,          ///< polarity inversion
  IOEPCA9555_REG_POL1,          ///< polarity inversion
  IOEPCA9555_REG_CFG0,          ///< configuration
  IOEPCA9555_REG_CFG1,          ///< configuration
} IOEPCA9555REG;

/// define the macros for command/data offsets
#define CMD_OFFSET          ( LE_U16_LSB_IDX )
#define DAT_OFFSET          ( LE_U16_MSB_IDX )

// structures -----------------------------------------------------------------
/// define the control structure
typedef struct _LCLCTL
{
  U16UN   tConfig;              ///< config
  U16UN   tPolarity;            ///< polarity
  U16UN   tOutput;              ///< output
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE         sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL  atLclCtl[ IOEPCA9555_DEV_ENUM_NAX ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function IOEPCA9555_Initialize
 *
 * @brief initialization
 *
 * This function will initialize all devices
 *
 * return TRUE if errors, FALSE otherwise
 *
 *****************************************************************************/
BOOL IoePCA9555_Initialize( void )
{
  BOOL                bStatus = FALSE;
  IOEPCA9555DEVENUM   eDevEnum;
  IOEPCA9555PINENUM   ePinEnum;
  PIOEPCA9555PINDEF   ptPinDef;
  PIOEPCA9555DEVDEF   ptDevDef;
  IOEPCA9555DIRENUM   ePinDir;
  BOOL                bPolarity, bInitial;
  PLCLCTL             ptLclCtl;
  IOEPCA9555DEVADDR   eDevAddr;
  
  // for each defined device
  for( eDevEnum = 0; eDevEnum < IOEPCA9555_DEV_ENUM_NAX; eDevEnum++ )
  {
    // get the device definition
    ptDevDef = ( PIOEPCA9555DEVDEF )&g_atIoePca9555Devs[ eDevEnum ];
    ptLclCtl = &atLclCtl[ eDevEnum ];
    
    // clear the config/poarity
    ptLclCtl->tConfig.wValue = 0;
    ptLclCtl->tOutput.wValue = 0;
    ptLclCtl->tPolarity.wValue = 0;
    
    // now process the pins
    for ( ePinEnum = 0; ePinEnum < IOEPCA9555_PIN_ENUM_MAX; ePinEnum++ )
    {
      // get a pointer to the pin definition
      ptPinDef = ( PIOEPCA9555PINDEF )&ptDevDef->atPinDefs[ ePinEnum ];
      
      // get direction
      ePinDir = PGM_RDBYTE( ptPinDef->eDirection );
      ptLclCtl->tConfig.wValue |= BITS( ePinDir, ePinEnum );
      
      // get polarity/initial value
      bPolarity = PGM_RDBYTE( ptPinDef->bInvert );

      // test for an input
      if ( IOEPCA9555_DIR_IN == ePinDir )
      {
        // set the polarity
        ptLclCtl->tPolarity.wValue |= BITS( bPolarity, ePinEnum );
      }
      else
      {
        // get the initial value
        bInitial = PGM_RDBYTE( ptPinDef->bInitial );
        
        // set the output
        ptLclCtl->tOutput.wValue |= BITS(( bPolarity ^ bInitial ), ePinEnum );
      }
    }
    
    // get device address
    eDevAddr = PGM_RDBYTE( ptDevDef->eDeviceAddress );

    // write the config
    if ( !( bStatus |= IoePCA9555_WriteDataToDevice( eDevAddr, IOEPCA9555_REG_CFG0, ( PU8 )&ptLclCtl->tConfig.wValue, sizeof( U16 ))))
    {
      // write the poliary
      if ( !( bStatus |= IoePCA9555_WriteDataToDevice( eDevAddr, IOEPCA9555_REG_POL0, ( PU8 )&ptLclCtl->tPolarity.wValue, sizeof( U16 ))))
      {
        // now write the output
        bStatus |= IoePCA9555_WriteDataToDevice( eDevAddr, IOEPCA9555_REG_OUT0, ( PU8 )&ptLclCtl->tOutput.wValue, sizeof( U16 ));
      }
    }
  }

  // return it
  return( bStatus );
}

/******************************************************************************
 * @function IOEPCA9555_SetOutput
 *
 * @brief set a pin as an ouput
 *
 * This function will set a pin's output state if configured as a output
 *
 * @param[in]   eDeviceEnum     device enumeration
 * @param[in]   ePinEnum        pin enumeration
 * @param[in]   bState          state of the pin
 *
 * @return      appropriate error
 *
 *****************************************************************************/
IOEPCA9555ERR IoePCA9555_SetOutput( IOEPCA9555DEVENUM eDeviceEnum, IOEPCA9555PINENUM ePinEnum, BOOL bState )
{
  IOEPCA9555ERR     eError = IOEPCA9555_ERR_NONE;
  PIOEPCA9555PINDEF ptPinDef;
  PIOEPCA9555DEVDEF ptDevDef;
  PLCLCTL           ptLclCtl;
  U16               wMask;
  IOEPCA9555DEVADDR eDevAddr;
  
  // valid device
  if ( eDeviceEnum < IOEPCA9555_DEV_ENUM_NAX )
  {
    // check for valid pin
    if ( ePinEnum < IOEPCA9555_PIN_ENUM_MAX )
    {
      // get the device definition
      ptDevDef = ( PIOEPCA9555DEVDEF )&g_atIoePca9555Devs[ eDeviceEnum ];
      ptPinDef = ( PIOEPCA9555PINDEF )&ptDevDef->atPinDefs[ ePinEnum ];
      ptLclCtl = &atLclCtl[ eDeviceEnum ];
      
      // check for this pin being an output
      if ( PGM_RDBYTE( ptPinDef->eDirection ) == IOEPCA9555_DIR_OUT )
      {
        // now calculate the mask
        wMask = BIT( ePinEnum );
        
        // check the state
        if ( PGM_RDBYTE( ptPinDef->bInvert ) ^ bState )
        {
          // set the bit
          ptLclCtl->tOutput.wValue |= wMask;
        }
        else
        {
          // clear the bit
          ptLclCtl->tOutput.wValue &= ~wMask;
        }

        // get the address/device
        eDevAddr = PGM_RDBYTE( ptDevDef->eDeviceAddress );

        // write it
        if ( IoePCA9555_WriteDataToDevice( eDevAddr, IOEPCA9555_REG_OUT0, ( PU8 )&ptLclCtl->tOutput.wValue, sizeof( U16 )))
        {
          // error detected
          eError = IOEPCA9555_ERR_I2CXFR;
        }
      }
      else
      {
        // set the error
        eError = IOEPCA9555_ERR_ILLACT;
      }
    }
    else
    {
      // set the error
      eError = IOEPCA9555_ERR_ILLPIN;
    }
  }
  else
  {
    // set the error
    eError = IOEPCA9555_ERR_ILLDEV;
  }
  
  // reteurn the error
  return( eError );
}

/******************************************************************************
 * @function IOEPCA9555_GetInput
 *
 * @brief get a pin's state
 *
 * This function will get a pin's input state
 *
 * @param[in]   eDeviceEnum     device enumeration
 * @param[in]   ePinEnum        pin enumeration
 * @param[in]   pbState         pointer to store the state of the pin
 *
 * @return      appropriate error
 *
 *****************************************************************************/
IOEPCA9555ERR IoePCA9555_GetInput( IOEPCA9555DEVENUM eDeviceEnum, IOEPCA9555PINENUM ePinEnum, PBOOL pbState )
{
  IOEPCA9555ERR		eError = IOEPCA9555_ERR_NONE;
  PIOEPCA9555PINDEF  	ptPinDef;
  PIOEPCA9555DEVDEF  	ptDevDef;
  PLCLCTL         	ptLclCtl;
  U16              	wMask, wData;
  IOEPCA9555DEVADDR     eDevAddr;
  
  // valid device
  if ( eDeviceEnum < IOEPCA9555_DEV_ENUM_NAX )
  {
    // check for valid pin
    if ( ePinEnum < IOEPCA9555_PIN_ENUM_MAX )
    {
      // get the device definition
      ptDevDef = ( PIOEPCA9555DEVDEF )&g_atIoePca9555Devs[ eDeviceEnum ];
      ptPinDef = ( PIOEPCA9555PINDEF )&ptDevDef->atPinDefs[ ePinEnum ];
      ptLclCtl = &atLclCtl[ eDeviceEnum ];
        
      // now calculate the mask
      wMask = BIT( ePinEnum );
      
      // get the address/device
      eDevAddr = PGM_RDBYTE( ptDevDef->eDeviceAddress );
			
      // read the current value
      if ( !IoePCA9555_ReadDataFromDevice( eDevAddr, IOEPCA9555_REG_OUT0, ( PU8 )&wData, sizeof( U16 )))
      {
        // now determine the data
        wData &= wMask;
        if ( PGM_RDBYTE( ptPinDef->bInvert ))
        {
          // invert the data
          wData ^= wMask;
        }

        // now stuff the result
        *( pbState ) = ( wData != 0 ) ? TRUE : FALSE;
      }
      else
      {
        // error detected
        eError = IOEPCA9555_ERR_I2CXFR;
      }
    }
    else
    {
      // set the error
      eError = IOEPCA9555_ERR_ILLPIN;
    }
  }
  else
  {
    // set the error
    eError = IOEPCA9555_ERR_ILLDEV;
  }
  // reteurn the error
  return( eError );
}

/******************************************************************************
 * @function IOEPCA9555_SetAll
 *
 * @brief set a pin as an ouput
 *
 * This function will set a pin's output state if configured as a output
 *
 * @param[in]   eDeviceEnum     device enumeration
 * @param[in]   wValue          state of the pins
 *
 * @return      appropriate error
 *
 *****************************************************************************/
IOEPCA9555ERR IoePCA9555_SetAll( IOEPCA9555DEVENUM eDeviceEnum, U16 wValue )
{
  IOEPCA9555ERR       eError = IOEPCA9555_ERR_NONE;
  PIOEPCA9555DEVDEF   ptDevDef;
  IOEPCA9555DEVADDR   eDevAddr;
  
  // valid device
  if ( eDeviceEnum < IOEPCA9555_DEV_ENUM_NAX )
  {
    // get the device definition
    ptDevDef = ( PIOEPCA9555DEVDEF )&g_atIoePca9555Devs[ eDeviceEnum ];

    // get the address/device
    eDevAddr = PGM_RDBYTE( ptDevDef->eDeviceAddress );
            
    // read the current value
    if ( IoePCA9555_WriteDataToDevice( eDevAddr, IOEPCA9555_REG_OUT0, ( PU8 )&wValue, sizeof( U16 )))
    {
      // error detected
      eError = IOEPCA9555_ERR_I2CXFR;
    }
  }
  else
  {
    // set the error
    eError = IOEPCA9555_ERR_ILLDEV;
  }
  // reteurn the error
  return( eError );
}

/******************************************************************************
 * @function IOEPCA9555_GetAll
 *
 * @brief read all inputs
 *
 * This function will set a pin's output state if configured as a output
 *
 * @param[in]   eDeviceEnum     device enumeration
 * @param[io]   pwValue         state of the pins
 *
 * @return      appropriate error
 *
 *****************************************************************************/
IOEPCA9555ERR IoePCA9555_GetAll( IOEPCA9555DEVENUM eDeviceEnum, PU16 pwValue )
{
  IOEPCA9555ERR       eError = IOEPCA9555_ERR_NONE;
  PIOEPCA9555DEVDEF   ptDevDef;
  U16                 wData;
  IOEPCA9555DEVADDR   eDevAddr;
  
  // valid device
  if ( eDeviceEnum < IOEPCA9555_DEV_ENUM_NAX )
  {
    // get the device definition
    ptDevDef = ( PIOEPCA9555DEVDEF )&g_atIoePca9555Devs[ eDeviceEnum ];

    // get the address/device
    eDevAddr = PGM_RDBYTE( ptDevDef->eDeviceAddress );
            
    // read the current value
    if ( !IoePCA9555_ReadDataFromDevice( eDevAddr, IOEPCA9555_REG_INP0, ( PU8 )&wData, sizeof( U16 )))
    {
      // store the data
      *( pwValue ) = wData;
    }
    else
    {
      // error detected
      eError = IOEPCA9555_ERR_I2CXFR;
    }
  }
  else
  {
    // set the error
    eError = IOEPCA9555_ERR_ILLDEV;
  }

  // reteurn the error
  return( eError );
}

/**@} EOF IOEPCA9555.c */
