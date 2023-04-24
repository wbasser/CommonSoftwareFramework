/******************************************************************************
 * @file QueueScheduler_prm.h
 *
 * @brief Queue Scheduler configuraiton
 *
 * This file provides the configuration for the queue scheduler
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
 * \addtogroup QueueScheduler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _QUEUESCHEDULER_PRM_H
#define _QUEUESCHEDULER_PRM_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the macros for enabling and disabling queue functions
#define QUEUESCHEDULER_INCLUDE_PUTHEAD          ( FALSE )
#define QUEUESCHEDULER_INCLUDE_PEEK             ( FALSE )
#define QUEUESCHEDULER_INCLUDE_POP              ( FALSE )
#define QUEUESCHEDULER_INCLUDE_FLUSh            ( FALSE )
#define QUEUESCHEDULER_INCLUDE_GETREMAINING     ( FALSE )
#define QUEUESCHEDULER_INCLUDE_GETSTATUS        ( FALSE )

/**@} EOF QueueScheduler_prm.h */

#endif  // _QUEUESCHEDULER_PRM_H