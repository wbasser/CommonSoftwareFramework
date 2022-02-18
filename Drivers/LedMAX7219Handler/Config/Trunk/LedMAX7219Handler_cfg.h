/******************************************************************************
 * @file LedMAX72119Handler_cfg.h
 *
 * @brief MAX7219 configuration declarations
 *
 * This file provides the declarations for the ocnfigutaion
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
 * \addtogroup LedMAX72119Handler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LEDMAX72119HANDLER_CFG_H
#define _LEDMAX72119HANDLER_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the number of digita
#define LEDMAX7219HANDLER_NUM_DIGITS                      ( 1 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void LedMAX72119Handler_WriteData( U16UN tValue );

/**@} EOF LedMAX72119Handler_cfg.h */

#endif  // _LEDMAX72119HANDLER_CFG_H