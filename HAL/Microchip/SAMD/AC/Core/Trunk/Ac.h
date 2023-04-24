/******************************************************************************
 * @file Ac.h
 *
 * @brief Analog comparator declarations
 *
 * This file provides the declarations for the analog comparator
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
 * \addtogroup Ac
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AC_H
#define _AC_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AC/Ac_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _ACERROR
{
  AC_ERROR_NONE = 0,          ///< no error
  AC_ILLEGAL_CHAN,            ///< illegal channel
  AC_ILLEGAL_IOCTL,           ///< illegal ioctl action
} ACERROR;

/// enumerate the ioctl actions
typedef enum _ACIOCTL
{
  AC_IOCTL_CONTROL = 0,       ///< control function(enable/disable)
  AC_IOCTL_START,             ///< start a comparison
  AC_IOCTL_SWAPINPS,          ///< swap inputs
  AC_IOCTL_MAX
} ACIOCTL;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void    Ac_Initialize( void );
extern  ACERROR Ac_Ioctl( ACIOCTL eAction, PVOID pvValue );

/**@} EOF Ac.h */

#endif  // _AC_H