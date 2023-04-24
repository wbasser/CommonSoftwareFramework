/******************************************************************************
 * @file RtcPCF8253.h
 *
 * @brief Real Time Clock PCF823 declarations 
 *
 * This file provides the declarations for the PCV8523 real time clock
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
 * \addtogroup RtcPCF8253
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RTCPCF8253_H
#define _RTCPCF8253_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TimeHandler/TimeHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _RTCPCF8523ERR
{
  RTCPCF8523_ERR_NONE = 0,       ///< no error
  RTCPCF8523_ERR_XFR   = 0xB0,  ///< transfer error
} RTCPCF8523ERR;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	void            RtcPCF8523Initialize( void );
extern	RTCPCF8523ERR   RtcPCF8523SetDateTime( PDATETIME ptDateTime );
extern	RTCPCF8523ERR   RtcPCF8523GetDateTime( PDATETIME ptDateTime );

/**@} EOF RtcPCF8253.h */

#endif  // _RTCPCF8253_H