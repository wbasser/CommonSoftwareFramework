/******************************************************************************
 * @file BootLoader_prm.h
 *
 * @brief Boot Loader configuration declarations
 *
 * This file provides the declarations for the boot laoder
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
 * \addtogroup BootLoader
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BOOTLOADER_PRM_H
#define _BOOTLOADER_PRM_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the boot load key
#define BOOT_LOAD_KEY           ( 0xFEEDBEEF )

/// define the ram start address
#define RAM_START_ADDRESS       ( 0x20004000 )

/**@} EOF BootLoader_prm.h */

#endif  // _BOOTLOADER_PRM_H