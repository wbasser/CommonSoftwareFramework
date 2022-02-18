/******************************************************************************
 * @file Main_def.h
 *
 * @brief declarations
 *
 * This file declares the initialization structures for main
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
 *
 * \addtogroup Main
 * @{
 *****************************************************************************/

// ensure only one instatiation
#ifndef _MAIN_DEF_H
#define _MAIN_DEF_H

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the initialization functions
typedef BOOL  ( *PVMAININITFUNC )( void );

/// define the dile functions
typedef void  ( *PVMAINIDLEFUNC )( void );

/// define the shutdown functions
typedef void  ( *PVMAINSHUTDNFUNC )( void );

/**@} EOF Main_def.c */

#endif // _MAIN_DEF_H
