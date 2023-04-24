/******************************************************************************
 * @file TofVL53L0X.h
 *
 * @brief ST TofVL530X Time-of-Flight driver declarations
 *
 * This file provides the declarations for the ST TofVL530X Time-of-Flight
 * Laser-Ranging sensor.
 *
 * @copyright Copyright (c) 2017 Cyber Intergration
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
 * \addtogroup TofVL53L0X
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TOFVL53L0X_H
#define _TOFVL53L0X_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "TofVL53L0X/TofVL53L0X_cfg.h"

// library includes -----------------------------------------------------------

// thire party library includes
#include "vl53l0x_api.h"
#include "vl53l0x_api_core.h"
#include "vl53l0x_types.h"
#include "vl53l0x_def.h"

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the configuration da ta
typedef struct _TOFVL53L0XCFG
{
  BOOL            bCalibrated;
	U32             uRefSpadCount;
	U8              nIsApertureSpads;
	U8              nVhvSettings;
	U8              nPhaseCal;
  S32             lOffsetMicroMeter;
  FixPoint1616_t  tXTalkCompRateMegaCps;
} TOFVL53L0XCFG, *PTOFVL53L0XCFG;
#define TOFVL53L0XCFG_SIZE                      sizeof( TOFVL53L0XCFG )

// global  function prototypes -
extern  void  TofVL53L0X_Initialize( void );
extern  PU8   TofVL53L0X_GetConfigDefault( void );
extern  PU8   TofVL53L0X_GetConfigActual( void );
extern  void  TofVL53L0X_SetConfiguration( PTOFVL53L0XCFG ptConfig );
extern  BOOL  TovVL53L0X_ReInitialize( U16 wCalDistance, BOOL bForceCal );
extern  void  TofVL53L0X_StartMeasurement( void );
extern  BOOL  TofVL53L0X_GetDataReady( void );
extern  BOOL  TofVL53L0X_GetDistance( PU16 wDistance );
extern  void  TofVL53L0X_ProcessDataReady( void );

/**@} EOF TofVL53L0X.h */

#endif  // _TOFVL53L0X_H
