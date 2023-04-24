/******************************************************************************
 * @file DdsAD9833_cfg.h
 *
 * @brief Direct Digital Syntheizer AD9833 configuration declarations
 *
 * This file provides the declarations for the DDS AD9833
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
 * \addtogroup DdsAD9833
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DDSAD9833_CFG_H
#define _DDSAD9833_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  DdsAD9833_WriteReg( U16 wData );

/**@} EOF DdsAD9833_cfg.h */

#endif  // _x_H