/******************************************************************************
 * @file NetworkLocator_cfg.h
 *
 * @brief network locator configuration declarations
 *
 * This file provides the configuration declarations for the network locator
 * wervice module
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
#ifndef _NETWORKLOCATOR_CFG_H
#define _NETWORKLOCATOR_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "NetworkLocator/NetworkLocator_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void          NetworkLocator_LocalInitialize( void );
extern  PVGETPOINTER  NetworkLocator_GetConfigActl( void );
extern  U16           NetworkLocator_GetConfigSize( void );
extern  U8            NetworkLocator_GetHdwMajor( void );
extern  U8            NetworkLocator_GetHdwMinor( void );
extern  U8            NetworkLocator_GetSfwMajor( void );
extern  U8            NetworkLocator_GetSfwMinor( void );
extern  U8            NetworkLocator_GetSfwTest( void );
extern  PC8           NetworkLocator_GetTitle( void );
extern  void          NetworkLocator_UpdateConfig( void );

/**@} EOF NetworkLocator_cfg.h */

#endif  // _NETWORKLOCATOR_CFG_H