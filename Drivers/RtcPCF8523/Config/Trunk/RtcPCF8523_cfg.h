/******************************************************************************
 * @file RtcPCF8253_cfg.h
 *
 * @brief Real Time Clock PCV8523 implementation declarations
 *
 * This file provides the declarations for the implementation of the PCF8523
 * real time clock
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
#ifndef _RTCPCF8253_CFG_H
#define _RTCPCF8253_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "RtcPCF8523/RtcPCF8523.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            RtcPCF8523_LocalInitialize( void );
extern  RTCPCF8523ERROR RtcPCF8523_WriteRegisters( U8 nBaseRegister, U8 nNumRegisters, PU8 pnData );
extern  RTCPCF8523ERROR RtcPCF8523_ReadRegisters( U8 nBaseRegister, U8 nNumRegisters, PU8 pnData );
extern  void            RtcPCF8523_IrqCallback( U8 nArg1, U8 nArg2 );

/**@} EOF RtcPCF8523_cfg.h */

#endif  // _RTCPCF8253_CFG_H