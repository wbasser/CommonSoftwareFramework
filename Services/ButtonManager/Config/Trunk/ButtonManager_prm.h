/******************************************************************************
 * @file ButtonManager_prm.h
 *
 * @brief button manager parameter declarations 
 *
 * This file provides the parameter declaratons for the button manager
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup ButtonManager
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _BUTTONMANAGER_PRM_H
#define _BUTTONMANAGER_PRM_H

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
#define BTNMANAGER_PROCESS_RATE_MSECS            ( 25 )

/// define the task argument argument overide
#define BTNMNGR_ARGSIZE_OVERRIDE_TASKARGSIZE  ( 0 )
#define BTNMNGR_ARGSIZE_OVERRIDE_SIZE1        ( 1 )

// define the button manager argument size override
#define BTNMNGR_ARGSIZE_OVERRIDE_SELECTION    ( BTNMNGR_ARGSIZE_OVERRIDE_SIZE1 )

/**@} EOF ButtonManager_prm.h */

#endif  // _BUTTONMANAGER_PRM_H