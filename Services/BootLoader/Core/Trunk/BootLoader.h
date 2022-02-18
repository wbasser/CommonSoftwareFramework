/******************************************************************************
 * @file BootLoader.h
 *
 * @brief Boot Loader declarations
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
#ifndef _BOOTLOADER_H
#define _BOOTLOADER_H

// library includes -----------------------------------------------------------
#include "BootLoader/BootLoader_prm.h"

// global function prototypes --------------------------------------------------
extern  BOOL  BootLoader_Initialize( void );
extern  void  BootLoader_Enter( void );
extern  void  BootLoader_ProcessCallback( U8 nOption );

/**@} EOF BootLoader.h */

#endif  // _BOOTLOADER_H