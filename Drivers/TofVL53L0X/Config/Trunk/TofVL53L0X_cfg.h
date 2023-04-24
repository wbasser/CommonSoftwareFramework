/******************************************************************************
 * @file TofVL53L0X_cfg.h
 *
 * @brief Time of FLight VL53L0x configuration definition file 
 *
 * This file provides the definitions for the configuraiton of the Time Of Flight
 * VL53l0X sensor
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
 * \addtogroup TofVL53L0X
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _TOFVL53L0X_CFG_H
#define _TOFVL53L0X_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// set the macro to enable interrupt operation
#define TOFVL53L0X_ENABLE_IRQ_OPERATION         ( FALSE )

/// define the macro to enable reset generation
#define TOFVL53L0X_RESET_ENABLED                ( TRUE )

/// define for local scan task
#define TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE       ( TRUE )

/// define the macro for enabling inline calibration
#define TOFVL53L0X_ENABLE_INLINE_CALIBRATION    ( TRUE )

/// define the macro for enabling offset calibration
#define TOFVL53L0X_ENABLE_OFFSET_CALIBRATION    ( TRUE )

/// define the macro for enabling crosstalk calibration
#define TOFVL53L0X_ENABLE_XTALK_CALIBRATION     ( FALSE )

#if ( TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE == TRUE )
  /// define the poll rate/number of events
  #define TOFVL53L0X_NUM_EVENTS                   ( 2 )
  #define TOFVL53L0X_POLL_RATE_MSECS              ( 50 )
  #define TOFVL53L0X_POLL_RATE                    ( TASK_TIME_MSECS( TOFVL53L0X_POLL_RATE_MSECS ))
#endif // TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE

/// define the I2C device enumeration
#define TOFVL53L0X_I2C_ENUM                     ( I2C_DEV_ENUM_LCLBUS )

/// define the calibration distance
#define TOFVL53L0X_CALIBRATE_DISTANCE           ( 23 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  TofVL53L0X_LocalInitialize( void );
extern  BOOL  TofVL53L0X_GetFactoryTestStatus( void );
extern  void  VL53L0X_UpdateConfig( void );
#if ( TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE == TRUE )
  extern  BOOL  TofVL53L0X_ProcessPoll( TASKARG xArg );
#endif // TOFVL53L0X_LOCAL_SCAN_TASK_ENABLE
#if ( TOFVL53L0X_ENABLE_IRQ_OPERATION == TRUE )
  extern  void  TofVL53L0X_IrqControl( BOOL bState );
  extern  void  TofVL53L0X_Irq_Callback( U8 nIrq, U8 nEvent, BOOL bState );
#endif // TOFVL53L0X_ENABLE_IRQ_OPERATION
#if ( TOFVL53L0X_RESET_ENABLED == TRUE )
  extern  void  TofVL53L0X_Reset_Device( void );
#endif // TOFVL53L0X_RESET_ENABLED
extern  BOOL  TofVL53L0X_Write( U8 nDevAddr, U8 nIndex, PU8 pnData, U16 wLength );
extern  BOOL  TofVL53L0X_Read( U8 nDevAddr, U8 nIndex, PU8 pnData, U16 wLength );

/**@} EOF TofVL53L0X_cfg.h */

#endif  // _TOFVL53L0X_CFG_H