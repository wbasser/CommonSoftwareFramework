/******************************************************************************
 * @file Interrupt.h
 *
 * @brief Interrupt manager declarations
 *
 * This file provides the interrupt manager declarations
 *
 * @copyright Copyright (c) 2021 Strongarm Tech Inc
 * The copyright to the computer software herein is the property of 
 * Strongarm Tech Inc. This software may not be used and/or copied without
 * explicit written permission of Strongarm Tech Inc., and only in accordance
 * with the terms and conditions stipulated in the license agreement and/or
 * contract under which the software has been supplied.
 *
 * Version History
 * ======
 *
 * \addtogroup Interrupt
 * @{
 *****************************************************************************/

// ensure only one instatiation
#ifndef _INTERRUPT_H
#define _INTERRUPT_H

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// enumerations ---------------------------------------------------------------

// global function prototypes --------------------------------------------------
extern  void   Interrupt_Initialize( void );
extern  void   Interrupt_Disable( void );
extern  BOOL   Interrupt_Enable( void );

/**@} EOF Interrupt.h */

#endif  // _INTERRUPT_H