/******************************************************************************
 * @file NetworkLocator.h
 *
 * @brief network locator declarations 
 *
 * This file provides the declarations for the network locator service module
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
 
// ensure only one instantiation
#ifndef _NETWORKLOCATOR_H
#define _NETWORKLOCATOR_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "NetworkLocator/NetworkLocator_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void          NetworkLocator_Initialize( void );
extern  void          NetworkLocator_UpdateLocalKey( void );
extern  PVGETPOINTER  NetworkLocator_GetConfigActl( void );
extern  void          NetworkLocator_SetConfig( PNETWORKLOCATORCFG ptConfig );
extern  BOOL          NetworkLocator_ProcessRcvMsg( PNETWORKLOCATORRCVPKT, ptRcvPkt PNETWORKLOCATORXMTPKT ptXmtPkt );

/**@} EOF NetworkLocator.h */

#endif  // _NETWORKLOCATOR_H