/******************************************************************************
 * @file Hardwaredebug_prm.h
 *
 * @brief hardware debug parameter declarations
 *
 * This file 
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
 * \addtogroup Hardwaredebug
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _HARDWAREDEBUG_PRM_H
#define _HARDWAREDEBUG_PRM_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "I2C/I2c.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the macro to disable/enable GPIO debug
#define HARDWAREDEBUG_ENABLE_GPIO               ( 1 )

/// define the macro to disable/enable I2C debug
#define HARDWAREDEBUG_ENABLE_I2C                ( 1 )
#if ( HARDWAREDEBUG_ENABLE_I2C == 1 )
  #define HARDWAREDEBUG_I2C_BUSENUM             ( I2C_DEV_ENUM_LCLBUS )
#endif // HARDWAREDEBUG_ENABLE_I2C

/**@} EOF Hardwaredebug_prm.h */

#endif  // _HARDWAREDEBUG_PRM_H