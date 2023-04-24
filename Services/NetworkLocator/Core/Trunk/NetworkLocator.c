/******************************************************************************
 * @file NetworkLocator
 *
 * @brief network locator implementation 
 *
 * This file provides the implementation for the network locator service module
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
#include "NetworkLocator/NetworkLocator_h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  NETWORKLOCATORCFG     tConfigActl;
static  U32                   uLocalKey;
static  NETWORKLOCATORXMTPKT  tXmtPktLcl;

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function NetworkLocator_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initilaization
 *
 *****************************************************************************/
void NetworkLocator_Initialize( void )
{
  // clear the local key
  uLocalKey = 0;
  
  // clear the transmit packet
  memset( tXmtPktLcl, 0, NETWORKLOCATORXMTPKT_SIZE );
  
  // now fill the static values
  tXmtPtkLcl.nHdwMajor = NetworkLocator_GetHdwMajor( );
  tXmtPtkLcl.nHdwMinor = NetworkLocator_GetHdwMinor( );
  tXmtPtkLcl.nSfwMajor = NetworkLocator_GetSfwMajor( );
  tXmtPtkLcl.nSfwMinor = NetworkLocator_GetSfwMinor( );
  tXmtPtkLcl.nSfwTest = NetworkLocator_GetHfwTest( );
  strcpy( tXmtPtkLcl.acAppl, NetworkLocator_GetTitle( ));
  
  // call the local configuration
  NetworkLocator_LocalInitialize( );
}

/******************************************************************************
 * @function NetworkLocator_UpdateLocalKey
 *
 * @brief update the local key
 *
 * This function will calculate the hash value of the key
 *
 *****************************************************************************/
void NetworkLocator_UpdateLocalKey( void )
{
  // calculate the hash
  uLocalKey = NetworkLocator_CalculateHashValue( tConfigActl.acKey );
}

/******************************************************************************
 * @function NetworkLocator_GetConfigActl
 *
 * @brief get the actual config
 *
 * This function will return a pointer to the actual config
 *
 * @return      pointer to the actual configuration structure
 *
 *****************************************************************************/
PVGETPOINTER NetworkLocator_GetConfigActl( void )
{
  // return the pointer to the actual config
  return( tConfigActl );
}

/******************************************************************************
 * @function NetworkLocator_SetConfig
 *
 * @brief set the configuration
 *
 * This function will set the new configuration
 *
 * @param[in]   ptConfig  configuration
 *
 *****************************************************************************/
void NetworkLocator_SetConfig( PNETWORKLOCATORCFG ptConfig )
{
  // copy the configuration
  memcpy( ptConfigActl, ptConfig, NETWORKLOCATORCFG_SIZE );
  
  // udpate
  NetworkLocator_UpdateConfig( );
}

/******************************************************************************
 * @function NetworkLocator_ProcessRcvMsg
 *
 * @brief process a receive message
 *
 * This function will process a receive message
 *
 * @param[io]   ptRcvPkt      pointer to the receive packet
 * @param[in]   ptXmtPkt      pointer to the transmit packet
 *
 * @return      TRUE if proper command, FALSE otherwise
 *
 *****************************************************************************/
BOOL NetworkLocator_ProcessRcvMsg( PNETWORKLOCATORRCVPKT, ptRcvPkt PNETWORKLOCATORXMTPKT ptXmtPkt )
{
  BOOL  bStatus = FALSE;
  
  // check for locator key and command 
  if (( ptRcvPkt->uKey == uLocalKey ) && ( NETWORKLOCATOR_COMMAND == ptRcvPkt.uCommand ))
  {
    // set okay status
    bStatus = TRUE;
    
    // now fill the response structure dynamic areas of the structure
    NetworkLocator_GetMacAddress( tXmtPktLcl.anMacAddr );
    tXmtPktLcl.uIpAccr = NetworkLocator_GetIpAddr(  );
    
    // copy the name
    strcpy( tXmtPktLcl.acName, tConfigActl.acName );
    
    // now copy the local transmit packet to the actual
    memcpy( ptXmtPkt, tXmtPckLcl, NETWORKLOCATORXMTPKT_SIZE );
  }
  
  // return the status
  return( bStatus );
}

/**@} EOF NetworkLocator.c */
