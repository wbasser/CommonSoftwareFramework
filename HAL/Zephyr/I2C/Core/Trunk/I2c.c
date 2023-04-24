/******************************************************************************
 * @file I2c.c
 *
 * @brief I2C implementation
 *
 * This file provides the implementation of the I2C
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
#include <drivers/i2c.h>

// local includes -------------------------------------------------------------
#include "I2C/I2c.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _LCLCTL
{
  const struct device*  ptI2c;
  BOOL                  bOkay;
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE         sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  LCLCTL      atLclCtls[ I2C_DEV_ENUM_MAX ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function I2c_Initialize
 *
 * @brief I2C initialization
 *
 * This function all devices in the table
 *
 * @return  TRUE if errors detected, FALSE if otherwise
 *
 *****************************************************************************/
BOOL I2c_Initialize( void )
{
  BOOL        bStatus = FALSE;
  PI2CDEF     ptDef;
  I2CDEVENUM  eDev;
  PLCLCTL     ptCtl;
  U32         uFlags;
  
  // for each device
  for ( eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // clear the flags
    uFlags = 0;
    
    // clear the ok flag
    ptCtl->bOkay = FALSE;
    
    // get the bindings
    if (( ptCtl->ptI2c = device_get_binding( ptDef->pcName )) != NULL )
    {
      // configure it
      switch( ptDef->eSpeedSel )
      {
        case I2C_SPDSEL_100KHZ :
          uFlags = I2C_SPEED_STANDARD;
          break;
          
        case I2C_SPDSEL_400KHZ :
          uFlags = I2C_SPEED_FAST;
          break;
          
        case I2C_SPDSEL_1MHZ :
          uFlags = I2C_SPEED_FAST_PLUS;
          break;
          
        case I2C_SPDSEL_3MHZ4 :
          uFlags = I2C_SPEED_HIGH;
          break;
          
        case I2C_SPDSEL_5MHZ :
          uFlags = I2C_SPEED_ULTRA;
          break;
          
        default :
          uFlags = I2C_SPEED_STANDARD;
          break;
      }
      
      // now set master mode
      uFlags |= I2C_MODE_MASTER;

      // configure it
      if ( i2c_configure( ptCtl->ptI2c, uFlags ) == 0 )
      {
        // set the ok flag
        ptCtl->bOkay = TRUE;
        
        // Originally was setting bStatus to true, when it should be false (to indicate no errors)
        bStatus = FALSE;
      }
    }
    else
    {
      // set error flag
      bStatus |= TRUE;
    }
  }
  
  // return status
  return( bStatus );
}

/******************************************************************************
 * @function I2c_CLoseAll
 *
 * @brief I2C close all channels
 *
 * This function turns off all I2C channels
 *
 *****************************************************************************/
void I2c_CloseAll( void )
{
  PI2CDEF                 ptDef;
  I2CDEVENUM              eDev;
  
  // for each device
  for ( eDev = 0; eDev < I2C_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
  }
}

/******************************************************************************
 * @function I2c_Write
 *
 * @brief write some characters to the I2C
 *
 * This function will write some characters to the I2C buffer if room
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ptXftCtl    pointer to the transfer control
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERROR I2c_Write( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl )
{
  I2CERROR 	  eError = I2C_ERROR_NONE;
  PI2CDEF 	  ptDef;
  PLCLCTL 	  ptCtl;
  PU8         pnBuffer;
  U16         wIdx, wBufSize;

  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointer to the definition structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // check for valid device
    if ( ptCtl->bOkay )
    {
      // allocate the buffer
      wBufSize = ptXfrCtl->nAddrLen + ptXfrCtl->wDataLen;
      if (( pnBuffer = k_malloc( wBufSize )) != NULL )
      {
        // copy the address and data  
        for ( wIdx = 0; wIdx < ptXfrCtl->nAddrLen; wIdx++ )
        {
          // copy the address bytes
          *( pnBuffer + wIdx ) = ptXfrCtl->tAddress.anValue[ wIdx ];
        }
        
        // now copy the data
        memcpy((  pnBuffer + wIdx ), ptXfrCtl->pnData, ptXfrCtl->wDataLen );

        // do it
        if ( i2c_write( ptCtl->ptI2c, pnBuffer, wBufSize, ptXfrCtl->nDevAddr ) != 0 )
        {
          // set an error
          eError = I2C_ERROR_TIMEOUT;
        }
        
        // free the memory
        k_free( pnBuffer );
      }
    }
    else
    {
      // set the error
      eError = I2C_ERROR_DEVERR;
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }

  // return the status
  return( eError );
}

/******************************************************************************
 * @function I2c_Read
 *
 * @brief read some characters from the I2C
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ptXftCtl    pointer to the transfer control
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERROR I2c_Read( I2CDEVENUM eDev, PI2CXFRCTL ptXfrCtl )
{
  I2CERROR    eError = I2C_ERROR_NONE;
  PI2CDEF     ptDef;
  PLCLCTL     ptCtl;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];
    
    // check for valid device
    if ( ptCtl->bOkay )
    {
      // now send the address and read
      if ( i2c_write_read( ptCtl->ptI2c, ptXfrCtl->nDevAddr, ( PCV )&ptXfrCtl->tAddress, ptXfrCtl->nAddrLen, ptXfrCtl->pnData, ptXfrCtl->wDataLen ) != 0 )
      {
        // set the error
        eError = I2C_ERROR_TIMEOUT;
      }
    }
    else
    {
      // set the error
      eError = I2C_ERROR_DEVERR;
    }
  }
  else
  {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }
  
  // return the error
  return(( I2CERROR )eError );
}

/******************************************************************************
 * @function I2c_Ioctl
 *
 * @brief I2C IOCTL functions
 *
 * This function provides functionality to modify the I2Cs parameters
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERROR I2c_Ioctl( I2CDEVENUM eDev, I2CACTION eAction, PVOID pvData )
{
  I2CERROR    eError = I2C_ERROR_NONE;
  PI2CDEF     ptDef;
  PLCLCTL     ptCtl;
  PU32        puData;
  U8          nData;
  PI2CCHKBSY  ptBusyParams;

  // set the pointer to avoid compiler errors
  puData = ( PU32 )pvData;
  
  // check for a valid I2C
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PI2CDEF )&g_atI2cDefs[ eDev ];
    ptCtl = &atLclCtls[ eDev ];

    // process the action
    switch( eAction )
    {
      case I2C_ACTION_POLL_DEVICE :
        // get the parameters
        ptBusyParams = ( PI2CCHKBSY )pvData;
        
        // check read/write
        if ( ptBusyParams->bReadMode )
        {
          // perform a read          
          if ( i2c_read( ptCtl->ptI2c, &nData, 1, ptBusyParams->nDevAddr ) != 0 )
          {
            // set the error
            eError = I2C_ERROR_SLVNAK;
          }
        }
        else
        {
          // perform a write
          nData = 0;
          if ( i2c_write( ptCtl->ptI2c, &nData, 1, ptBusyParams->nDevAddr ) != 0 )
          {
            // set the error
            eError = I2C_ERROR_SLVNAK;
          }
        }
        break;
        
      case I2C_ACTION_SET_DEVADDR :
        break;
        
      case I2C_ACTION_GET_DEVADDR :
        break;
        
      default :
        // illegal action
        eError = I2C_ERROR_ILLACT;
        break;
    }
  }
  else
 {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function I2c_Close
 *
 * @brief Close the I2C
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
I2CERROR I2c_Close( I2CDEVENUM eDev )
{
  I2CERROR       eError = I2C_ERROR_NONE;

  // check for a valid UART
  if ( eDev < I2C_DEV_ENUM_MAX )
  {
  }
  else
  {
    // illegal device
    eError = I2C_ERROR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/**@} EOF I2c.c */
