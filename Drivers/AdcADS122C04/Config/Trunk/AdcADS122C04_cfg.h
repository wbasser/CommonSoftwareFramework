/******************************************************************************
 * @file AdcADS122C04_cfg.h
 *
 * @brief ADC ADS122C04 configuration declarations 
 *
 * This file provides the configuration declarations
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 *
 * \addtogroup AdcADS122C04
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADCADS122C04_CFG_H
#define _ADCADS122C04_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AdcADS122C04/AdcADS122C04_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the base address
#define ADCADS122C04CHIPADDR                    ( 0x60 )

// enumerations ---------------------------------------------------------------
/// enumerate the channels here
typedef enum _ADCADS122C04CHANENUM
{
  // enumarate the channels here
  
  // do not remove this entry
  ADCADS122C04_CHAN_ENUM_MAX
} ADCADS122C04CHANENUM;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  const CODE  ADCADS122C04DEF g_atAdcADS122C04Defs[ ];

// global function prototypes --------------------------------------------------
extern  BOOL  AdcADS122C04_LocalInitialize( void );
extern  BOOL  AdcADS122C04_WriteCommand( U8 nCommand );
extern  BOOL  AdcADS122C04_WriteRegister( ADCADS122C04REG eReg, U8 nData );
extern  BOOL  AdcADS122C04_ReadRegister( ADCADS122C04REG eReg, PU8 pnData );
extern  BOOL  AdcADS122C04_ReadData( PU32 puData );
extern  BOOL  AdcADS122C04_GetDataReady( void );
extern  void  AdcADS122C04_IrqCallback( U8 nPin, U8 nEvent, BOOL bState );

/**@} EOF AdcADS122C04_cfg.h */

#endif  // _ADCADS122C04_CFG_H