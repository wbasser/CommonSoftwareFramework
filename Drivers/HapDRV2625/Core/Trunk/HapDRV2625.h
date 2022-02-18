/******************************************************************************
 * @file HapDRV2625.h
 *
 * @brief DRV2625 haptic driver declarations 
 *
 * This file provides the declarations for the DRV2625 haptic driver
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
 * \addtogroup HapDRV2625
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _HAPDRV2625_H
#define _HAPDRV2625_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HapDRV2625/HapDRV2625_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  HapDRV2625_Initialize( void );
extern  void  HapDRV2625_PlayTone( U8 nTone, BOOL bState );
extern  void  HapDRV2625_PlaySequence( PHAPDRV2625SEQDEF ptHapSequence );

/**@} EOF HapDRV2625.h */

#endif  // _HAPDRV2625_H