/******************************************************************************
 * @file PhyKSZ8081RNACA.h
 *
 * @brief KSZ8051 physical interface declarations
 *
 * This file provides the declarations for the KSZ8051 PHY
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
 * \addtogroup PhyKSZ8081RNACA
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _PHYKSZ8081RNACA_H
#define _PHYKSZ8081RNACA_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"
#include "GMAC/Gmac.h"

// local includes -------------------------------------------------------------
#include "PhyKSZ8081RNACA/PhyKSZ8081RNACA_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  GMACSTATUS PhyKSZ8081RNACA_Initialize( PU32 puMacAddr, U8 nPhyAddr );
extern  GMACSTATUS PhyKSZ8081RNACA_SetLink( PU32 puMacAddr, U8 nPhyAddr, U8 nFlags );
extern  GMACSTATUS PhyKSZ8081RNACA_AutoNegotiate( PU32 puMacAddr, U8 nPhyAddr );
extern  GMACSTATUS PhyKSZ8081RNACA_Reset( PU32 puMacAddr, U8 nPhyAddr );

/**@} EOF PhyKSZ8081RNACA.h */

#endif  // _PHYKSZ8081RNACA_H