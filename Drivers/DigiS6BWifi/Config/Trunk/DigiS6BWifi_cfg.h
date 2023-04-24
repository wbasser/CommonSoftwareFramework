/******************************************************************************
 * @file DigiS6BWifi_cfg.h
 *
 * @brief DIGI S6F Wifi module
 *
 * This file provides the declarations for the DIGI S6B WIFI module
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
 * \addtogroup DigiS6BWifi
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DIGIS6BWIFI_CFG_H
#define _DIGIS6BWIFI_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  DigiS6BWifi_LocalInitialize( void );
extern  void  DigiS6BWifi_PutChar( U8 nChar );
extern  void  DigiS6BWifi_Write( PU8 pnData, U16 wLength );

/**@} EOF DigiS6BWifi_cfg.h */

#endif  // _DIGIS6BWIFI_CFG_H