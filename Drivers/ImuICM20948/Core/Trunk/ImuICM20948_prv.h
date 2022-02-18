/******************************************************************************
 * @file ImuICM20948_prv.h
 *
 * @brief IMU ICM20948 private declarations 
 *
 * This file provides the declarations for the ICM29048 IMU driver
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
 * \addtogroup ImuICM20948
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _IMUICM20948_PRV_H
#define _IMUICM20948_PRV_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the bank select register
#define BANKX_BANK_SEL                              ( 127 )

/// define the magnetometer registers
#define BANKX_REGS_WIA1                             ( 0x01 )
#define BANKX_REGS_ST1                              ( 0x10 )
#define BANKX_REGS_MAGNHXL                          ( 0x11 )
#define BANKX_REGS_MAGNHXH                          ( 0x12 )
#define BANKX_REGS_MAGNHYL                          ( 0x13 )
#define BANKX_REGS_MAGNHYH                          ( 0x14 )
#define BANKX_REGS_MAGNHZL                          ( 0x15 )
#define BANKX_REGS_MAGNHZH                          ( 0x16 )
#define BANKX_REGS_ST2                              ( 0x18 )
#define BANKX_REGS_CNTL2                            ( 0x31 )
#define BANKX_REGS_CNTL3                            ( 0x32 )

/// define the value for chip id
#define CHIP_ID_VALUE                               ( 0xEA )

/// define the accelration decimator valiues
#define ACCEL_CONFIG2_DECIMATOR_1                   ( 0 )
#define ACCEL_CONFIG2_DECIMATOR_8                   ( 1 )
#define ACCEL_CONFIG2_DECIMATOR_16                  ( 2 )
#define ACCEL_CONFIG2_DECIMATOR_32                  ( 3 )

/// definethe accelerometer full scale values
#define ACCEL_FULLSCALE_2G                          ( 0 )
#define ACCEL_FULLSCALE_4G                          ( 1 )
#define ACCEL_FULLSCALE_8G                          ( 2 )
#define ACCEL_FULLSCALE_16G                         ( 3 )

/// define gyro full scale values
#define GYRO_FULL_SCALE_250DPS                      ( 0 )
#define GYRO_FULL_SCALE_500DSP                      ( 1 )
#define GYRO_FULL_SCALE_1000DPS                     ( 2 )
#define GYRO_FULL_SCALE_2000DPS                     ( 3 )

/// define the magnetometer address (AK09916)
#define MAGN_AK09916_I2C_ADDR                       ( 0x0C )

// enumerations ---------------------------------------------------------------
/// enumerate the Bank0 Registers
typedef enum _BANK0REGS
{
  BANK0_REGS_WHOAMI           = 0,
  BANK0_REGS_USERCTRL         = 3,
  BANK0_REGS_LPCONFIG         = 5,
  BANK0_REGS_PWRMGMT1,
  BANK0_REGS_PWRMGMT2,
  BANK0_REGS_INTPINCFG        = 15,
  BANK0_REGS_INTENABLE,
  BANK0_REGS_INTENABLE1,
  BANK0_REGS_INTENABLE2,
  BANK0_REGS_INTENABLE3,
  BANK0_REGS_I2CMSGSTS        = 23,
  BANK0_REGS_INTSTATUS        = 25,
  BANK0_REGS_INTSTATUS1,
  BANK0_REGS_INTSTATUS2,
  BANK0_REGS_INTSTATUS3,
  BANK0_REGS_DELAYTIMEH       = 40,
  BANK0_REGS_DELAYTIMEL,
  BANK0_REGS_ACCELXOUTH       = 45,
  BANK0_REGS_ACCELXOUTL,
  BANK0_REGS_ACCELYOUTH,
  BANK0_REGS_ACCELYOUTL,
  BANK0_REGS_ACCELZOUTH,
  BANK0_REGS_ACCELZOUTL,
  BANK0_REGS_GYROXOUTH,
  BANK0_REGS_GYROXOUTL,
  BANK0_REGS_GYROYOUTH,
  BANK0_REGS_GYROYOUTL,
  BANK0_REGS_GYROZOUTH,
  BANK0_REGS_GYROZOUTL,
  BANK0_REGS_TEMPOUTH,
  BANK0_REGS_TEMPOUTL,
  BANK0_REGS_EXTSLVSENSD00,
  BANK0_REGS_EXTSLVSENSD01,
  BANK0_REGS_EXTSLVSENSD02,
  BANK0_REGS_EXTSLVSENSD03,
  BANK0_REGS_EXTSLVSENSD04,
  BANK0_REGS_EXTSLVSENSD05,
  BANK0_REGS_EXTSLVSENSD06,
  BANK0_REGS_EXTSLVSENSD07,
  BANK0_REGS_EXTSLVSENSD08,
  BANK0_REGS_EXTSLVSENSD09,
  BANK0_REGS_EXTSLVSENSD10,
  BANK0_REGS_EXTSLVSENSD11,
  BANK0_REGS_EXTSLVSENSD12,
  BANK0_REGS_EXTSLVSENSD13,
  BANK0_REGS_EXTSLVSENSD14,
  BANK0_REGS_EXTSLVSENSD15,
  BANK0_REGS_EXTSLVSENSD16,
  BANK0_REGS_EXTSLVSENSD17,
  BANK0_REGS_EXTSLVSENSD18,
  BANK0_REGS_EXTSLVSENSD19,
  BANK0_REGS_EXTSLVSENSD20,
  BANK0_REGS_EXTSLVSENSD21,
  BANK0_REGS_EXTSLVSENSD22,
  BANK0_REGS_EXTSLVSENSD23,
  BANK0_REGS_FIFOEN1          = 102,
  BANK0_REGS_FIFOEN2,
  BANK0_REGS_FIFORST,
  BANK0_REGS_FIFOMODE,
  BANK0_REGS_FIFOCOUNTH       = 112,
  BANK0_REGS_FIFOCOUNTL,
  BANK0_REGS_FIFORW,
  BANK0_REGS_DATARDYSTS       = 116,
  BANK0_REGS_FIFCFG           = 118
} BANK0REGS;

/// enumerate the BANK1 registers
typedef enum _BANK1REGS
{
  BANK1_REGS_SELFTESTXGYRO    = 0x0100 + 2,
  BANK1_REGS_SELFTESTYGYRO,
  BANK1_REGS_SELFTESTZGYRO,
  BANK1_REGS_SELFTESTXACCEL   = 0x0100 + 14,
  BANK1_REGS_SELFTESTYACCEL,
  BANK1_REGS_SELFTESTZACCEL,
  BANK1_REGS_XAOFFSH          = 0x0100 + 20,
  BANK1_REGS_XAOFFSL,
  BANK1_REGS_YAOFFSH          = 0x0100 + 23,
  BANK1_REGS_YAOFFSL,
  BANK1_REGS_ZAOFFSH          = 0x0100 + 26,
  BANK1_REGS_ZAOFFSL,
  BANK1_REGS_TIMBSCOORPLL     = 0x0100 + 40
} BANK1REGS;

/// enumerate the BANK2 registers
typedef enum _BANK2REGS
{
  BANK2_REGS_GYROSMPLRT       = 0x0200 + 0,
  BANK2_REGS_GYROCONFIG1,
  BANK2_REGS_GYROCONFIG2,
  BANK2_REGS_XGOFFSUSRH,
  BANK2_REGS_XGOFFSUSRL,
  BANK2_REGS_YGOFFSUSRH,
  BANK2_REGS_YGOFFSUSRL,
  BANK2_REGS_ZGOFFSUSRH,
  BANK2_REGS_ZGOFFSUSRL,
  BANK2_REGS_ODRALIGNEN,
  BANK2_REGS_ACCELSMPLRTDIV1  = 0x0200 + 16,
  BANK2_REGS_ACCELSMPLRTDIV2,
  BANK2_REGS_ACCELINTELCTRL,
  BANK2_REGS_ACCELWOMTHR,
  BANK2_REGS_ACCELCONFIG,
  BANK2_REGS_ACCELCONFIG2,
  BANK2_REGS_FSYNCONFIG       = 0x0200 + 82,
  BANK2_REGS_TEMPCONFIG,
  BANK2_REGS_MODCTRLUSR
} BANK2REGS;

/// enumerate the BANK3 registers
typedef enum _BANK3REGS
{
  BANK3_REGS_I2CMSTODRCONFIG  = 0x0300 + 0,
  BANK3_REGS_IC2MSTCTRL,
  BANK3_REGS_I2CMSTDELAYCTRL,
  BANK3_REGS_I2CSLV0ADDR,
  BANK3_REGS_I2CSLV0REG,
  BANK3_REGS_I2CSLV0CTRL,
  BANK3_REGS_I2CSLV0DO,
  BANK3_REGS_I2CSLV1ADDR,
  BANK3_REGS_I2CSLV1REG,
  BANK3_REGS_I2CSLV1CTRL,
  BANK3_REGS_I2CSLV1DO,
  BANK3_REGS_I2CSLV2ADDR,
  BANK3_REGS_I2CSLV2REG,
  BANK3_REGS_I2CSLV2CTRL,
  BANK3_REGS_I2CSLV2DO,
  BANK3_REGS_I2CSLV3ADDR,
  BANK3_REGS_I2CSLV3REG,
  BANK3_REGS_I2CSLV3CTRL,
  BANK3_REGS_I2CSLV3DO,
  BANK3_REGS_I2CSLV4ADDR,
  BANK3_REGS_I2CSLV4REG,
  BANK3_REGS_I2CSLV4CTRL,
  BANK3_REGS_I2CSLV4DO,
  BANK3_REGS_I2CSLV4DI,
} BANK3REGS;

/// enumerate the magnetomer registers
typedef enum _MAGNREGS
{
  MAGN_REGS_WIA2 = 0,
  MAGN_REGS_ST1,
  MAGN_REGS_HXL,
  MAGN_REGS_HXH,
  MAGN_REGS_HYL,
  MAGN_REGS_HYH,
  MAGN_REGS_HZL,
  MAGN_REGS_HZH,
  MAGN_REGS_ST2,
  MAGN_REGS_CNTL2,
  MAGN_REGS_CNTL3,
  MAGN_REGS_TS1,
  MAGN_REGS_TS2
} MAGNREGS;

/// enumerate the accelerometer low pass filter
typedef enum _ACCELLOWPASS
{
  ACCEL_LOWPASS_246HZ = 0,
  ACCEL_LOWPASS_246HZ1,
  ACCEL_LOWPASS_111HZ4,
  ACCEL_LOWPASS_50HZ4,
  ACCEL_LOWPASS_23HZ9,
  ACCEL_LOWPASS_11HZ5,
  ACCEL_LOWPASS_5HZ7,
  ACCEL_LOWPASS_473ZH
} ACCELLOWPASS;

/// enumerat ethe gyro low pass filter
typedef enum _GYROLOWPASS
{
  GYRO_LOWPASS_196HZ6 = 0,
  GYRO_LOWPASS_151HZ8,
  GYRO_LOWPASS_119HZ5,
  GYRO_LOWPASS_51HZ2,
  GYRO_LOWPASS_23HZ9,
  GYRO_LOWPASS_11HZ6,
  GYRO_LOWPASS_5HZ7,
  GYRO_LOWPASS_361HZ4
} GYROLOWPASS;

/// enumerate the gyro averaging value
typedef enum _GYROAVERAGE
{
  GYRO_AVERAGE_1 = 0,
  GYRO_AVERAGE_2,
  GYRO_AVERAGE_4,
  GYRO_AVERAGE_8,
  GYRO_AVERAGE_16,
  GYRO_AVERAGE_32,
  GYRO_AVERAGE_64,
  GYRO_AVERAGE_128
} GYROAVERAGE;

/// enumerate the magnetometer sample rate
typedef enum _MAGNSAMPRATE
{
  MAGN_SAMPLERATE_1    = 0x01,
  MAGN_SAMPLERATE_10HZ = 0x02,
  MAGN_SAMPLERATE_20HZ = 0x04,
  MAGN_SAMPLERATE_50HZ = 0x06,
  MAGN_SAMPLERATE_100HZ = 0x08
} MAGNSAMPRATE;


// structures -----------------------------------------------------------------
/// define the BANKX bank select register
typedef union _BANKXBNKSEL
{
  struct
  {
    U8  nReserved1    : 4;
    U8  nBankSel      : 3;
    U8  nReserved2    : 1;
  } tFields;
  U8 nByte;
} BANKXBNKSEL, *PBANKXBNKSEL;
#define BANKXBNKSEL_SIZE                        sizeof( BANKXBNKSEL )

/// define the BANK0 User Control register
typedef struct _BANK0USERCTRL
{
  U8  bReserved1    : 1;
  U8  bI2cMstRst    : 1;
  U8  bSramRst      : 1;
  U8  bDmpRst       : 1;
  U8  bI2cIfDis     : 1;
  U8  bI2cMstEn     : 1;
  U8  bFifoEn       : 1;
  U8  bDmpEn        : 1;
} BANK0USERCTRL, *PBANK0USERCTRL;
#define BANK0USERCTRL_SIZE                      sizeof( BANK0USERCTRL )

/// define the BANK0 LpConfig
typedef struct _BANK0LPCONFIG
{
  U8  nReserved1    : 4;
  U8  bGyroCycle    : 1;
  U8  bAccelCycle   : 1;
  U8  bI2cMstCycle  : 1;
} BANK0LPCONFIG, *PBANK0LPCONFIG;
#define BANK0LPCONFIG_SIZE                      sizeof( BANK0LPCONFIG )

/// define the BANK0 Power Management 1
typedef struct _BANK0PWRMGNT1
{
  U8  nClkSel       : 3;
  U8  bTempDis      : 1;
  U8  bReserved2    : 1;
  U8  bLpEn         : 1;
  U8  bSleep        : 1;
  U8  bDeviceReset  : 1;
} BANK0PWRMGNT1, *PBANK0PWRMGNT1;
#define BANK0PWRMGNT1_SIZE                      sizeof( BANK0PWRMGNT1 )

/// define the BANK0 Power Management 2
typedef struct _BANK0PWRMGNT2
{
  U8  nDisableGyro  : 3;
  U8  nDisableAccel : 3;
} BANK0PWRMGNT2, *PBANK0PWRMGNT2;
#define BANK0PWRMGNT2_SIZE                      sizeof( BANK0PWRMGNT2 )

/// define the BANK0 Interrupt pin configuration
typedef struct _BANK0INTPINCFG
{
  U8  bReserved       : 1;
  U8  bByPassEn       : 1;
  U8  bFsyncIntModeEn : 1;
  U8  bActlFsync      : 1;
  U8  bIntAnyRd2Clr   : 1;
  U8  bInt1LatchEn    : 1;
  U8  bInt1OpenDrn    : 1;
  U8  bInt1Actl       : 1;
} BANK0INTPINCFG, *PBANK0INTPINCFG;
#define BANK0INTPINCFG_SIZE                     sizeof( BANK0INTPINCFG )

/// define the BANK0 interrupt enable
typedef struct _BANK0INTENABLE
{
  U8  bI2cMsgIntEn    : 1;
  U8  bDmpInt1En      : 1;
  U8  bPllRdyEn       : 1;
  U8  bWomIntEn       : 1;
  U8  Reserved        : 3;
  U8  bRegWofEn       : 1;
} BANK0INTENABLE, *PBANK0INTENABLE;
#define BANK0INTENABLE_SIZE                     sizeof( BANK0INTENABLE )

/// define the BANK0 interrupt enable 1
typedef struct _BANK0INTENABLE1
{
  U8  bRawData0RdyEn  : 1;
} BANK0INTENABLE1, *PBANK0INTENABLE1;
#define BANK0INTENABLE1_SIZE                    sizeof( BANK0INTENABLE1 )

/// define the BANK0 interrupt enable 2
typedef struct _BANK0INTENABLE2
{
  U8  nFifoOverFlowEn : 5;
} BANK0INTENABLE2, *PBANK0INTENABLE2;
#define BANK0INTENABLE2_SIZE                    sizeof( BANK0INTENABLE2 )

/// define the BANK0 interrupt enable 3
typedef struct _BANK0INTENABLE3
{
  U8  nFifoWmEn       : 5;
} BANK0INTENABLE3, *PBANK0INTENABLE3;
#define BANK0INTENABLE3_SIZE                    sizeof( BANK0INTENABLE3 )

/// define the BANK0 I2C Master Status
typedef struct _BANK0I2CMSTSTATUS
{
  U8  bI2cSlv0Nack    : 1;
  U8  bI2cSlv1Nack    : 1; 
  U8  bI2cSlv2Nack    : 1; 
  U8  bI2cSlv3Nack    : 1; 
  U8  bI2cSlv4Nack    : 1; 
  U8  bI2cLostArb     : 1;
  U8  bI2cSlv4Done    : 1;
  U8  bPassThrough    : 1;
} BANK0I2CMSTSTATUS, *PBANK0I2CMSTSTATUS;
#define BANK0I2CMSTSTATUS_SIZE                  sizeof( BANK0I2CMSTSTATUS )

/// define the BANK0 interrupt status
typedef struct _BANK0INTSTATUS
{
  U8  bI2cMstInt      : 1;
  U8  bDmpInt1        : 1;
  U8  bPllRdyInt      : 1;
  U8  bWomInt         : 1;
} BANK0INTSTATUS, *PBANK0INTSTATUS;
#define BANK0INTSTATUS_SIZE                     sizeof( BANK0INTSTATUS )

/// define the BANK0 interrupt status 1
typedef struct _BANK0INTSTATUS1
{
  U8  bRawData0RdyInt : 1;
} BANK0INTSTATUS1, *PBANK0INTSTATUS1;
#define BANK0INTSTATUS1_SIZE                    sizeof( BANK0INTSTATUS1 )

/// define the BANK0 interrupt status 2
typedef struct _BANK0INTSTATUS2
{
  U8  nFifoOverFlowInt : 4;
} BANK0INTSTATUS2, *PBANK0INTSTATUS2;
#define BANK0INTSTATUS2_SIZE                    sizeof( BANK0INTSTATUS2 )

/// define the BANK0 interrupt status 3
typedef struct _BANK0INTSTATUS3
{
  U8  nFifoWmInt      : 4;
} BANK0INTSTATUS3, *PBANK0INTSTATUS3;
#define BANK0INTSTATUS3_SIZE                    sizeof( BANK0INTSTATUS3 )

/// define the BANK0 FIFo Enable 1
typedef struct _BANK0FIFOEN1
{
  U8  bSlv0FifoEn     : 1; 
  U8  bSlv1FifoEn     : 1;
  U8  bSlv2FifoEn     : 1;
  U8  bSlv3FifoEn     : 1;
} BANK0FIFOEN1, *PBANK0FIFOEN1;
#define BANK0FIFOEN1_SIZE                      sizeof( BANK0FIFOEN1 )

/// define the BANK0 fifo enable 2
typedef struct _BANK0FIFOEN2
{
  U8  bTempFifoEn     : 1;
  U8  bGyroXFifoEn    : 1;
  U8  bGyroYFifoEn    : 1;
  U8  bGyroZFifoEn    : 1;
  U8  bAccelFifoEn    : 1;
} BANK0FIFOEN2, *PBANK0FIFOEN2;
#define BANK0FIFOEN2_SIZE                      sizeof( BANK0FIFOEN2 )

/// define the BANK0 Data Ready Status
typedef struct _BANK0DATARDYSTATUS
{
  U8  nRawDataRdy     : 4;
  U8  nReserved       : 3;
  U8  bWofStatus      : 1;
} BANK0DATARDYSTATUS, *PBANK0DATARDYSTATUS;
#define BANK0DATARDYSTATUS_SIZE                 sizeof( BANK0DATARDYSTATUS )

/// define the BANK2 Gyro Config 1
typedef struct _BANK2GYROCFG1
{
  U8  bGyroFChoice    : 1;
  U8  nGyroFsSelec    : 2;
  U8  nGyroDlpfCfg    : 3;
} BANK2GYROCFG1, *PBANK2GYROCFG1;
#define BANK2GYROCFG1_SIZE                      sizeof( BANK2GYROCFG1 )

/// define the BANK2 Gyro Config 2
typedef struct _BANK2GYROCFG2
{
  U8  nGyroAvgCfg     : 3;
  U8  bZGyroCten      : 1;
  U8  bYGyroCten      : 1;
  U8  bXGyroCten      : 1;
} BANK2GYROCFG2, *PBANK2GYROCFG2;
#define BANK2GYROCFG2_SIZE                      sizeof( BANK2GYROCFG2 )

/// define the BANK2 ODR alignment enable reister
typedef struct _BANK2ODRALIGNEN
{
  U8  bOdrALignEnab   : 1;
} BANK2ODRALIGNEN, *PBANK2ODRALIGNEN;
#define BANK2ODRALIGNEN_SIZE                    sizeof( BANK2ODRALIGNEN )

/// define the BANK2 Accel intel ctrl
typedef struct _BANK2ACCELINTELCTRL
{
  U8  bAccelIntelModeInt  : 1;
  U8  bAccelIntelEn       : 1;
} BANK2ACCELINTELCTRL, *PBANK2ACCELINTELCTRL;
#define BANK2ACCELINTELCTRL_SIZE                sizeof( BANK2ACCELINTELCTRL )

/// define the BANK2 Accel config
typedef struct _BANK2ACCELCONFIG
{
  U8  bAccelFChoice   : 1;
  U8  nAccelFSSel     : 2;
  U8  nAccelDPLFCfg   : 3;
} BANK2ACCELCONFIG, *PBANK2ACCELCONFIG;
#define BANK2ACCELCONFIG_SIZE                sizeof( BANK2ACCELCONFIG )

/// define the BANK2 Accel config 2
typedef struct _BANK2ACCELCONFIG2
{
  U8  nDec3Cfg        : 2;
  U8  bAXStEnReg      : 1;
  U8  bAYStEnReg      : 1;
  U8  bAZStEnReg      : 1;
} BANK2ACCELCONFIG2, *PBANK2ACCELCONFIG2;
#define BANK2ACCELCONFIG2_SIZE               sizeof( BANK2ACCELCONFIG2 )

/// define the BANK3 I2C master control
typedef struct _BANK3I2CMSTCTRL
{
  U8  nI2cMstClk      : 4;
  U8  bI2cMstPNsr     : 1;
  U8  bReserved       : 2;
  U8  bMultiMstEn     : 1;
} BANK3I2CMSTCTRL, *PBANK3I2CMSTCTRL;
#define BANK3I2CMSTCTRL_SIZE                sizeof( BANK3I2CMSTCTRL )

/// define the BANK3 SLVx Ctrl
typedef struct _BANK3I2CSLVXCTRL
{
  U8  nI2cSlvxLength  : 4;
  U8  bI2cSlvxGrp     : 1;
  U8  bI2cSlvxRegDix  : 1;
  U8  bI2cSlvxBytSwa  : 1;
  U8  bI2cSlvxEn      : 1;
} BANK3I2CSLVXCTRL, *PBANK3I2CSLVXCTRL;
#define BANK3I2CSLVXCTRL_SIZE               sizeof( BANK3I2CSLVXCTRL )

/// define the BANK3 SLVx Address
typedef struct _BANK33I2CSLVXADDR
{
  U8  nI2cSlvAddr     : 7;
  U8  bI2cSlvRdEn     : 1;
} BANK33I2CSLVXADDR, *PBANK33I2CSLVXADDR;
#define BANK33I2CSLVXADDR_SIZE              sizeof( BANK33I2CSLVXADDR )

/// define the bank enumerations
typedef union _BANKXREGS
{
  BANK0USERCTRL         tBank0UserCtrl;
  BANK0LPCONFIG         tBank0LpConfig;
  BANK0PWRMGNT1         tBank0PwrMgnt1;
  BANK0PWRMGNT2         tBank0PwrMgnt2;
  BANK0INTPINCFG        tBank0IntPinCfg;
  BANK0INTSTATUS        tBank0IntStatus;
  BANK0INTSTATUS1       tBank0IntStatus1;
  BANK0INTSTATUS2       tBank0IntStatus2;
  BANK0INTENABLE        tBank0IntEnable;
  BANK0INTENABLE3       tBank0IntEnable3;
  BANK0I2CMSTSTATUS     tBank0I2cMstStatus;
  BANK0FIFOEN1          tBank0FifoEn1;
  BANK0FIFOEN2          tBank0FifoEn2;
  BANK0DATARDYSTATUS    tBank0DataRdyStatus;
  BANK2GYROCFG1         tBank2GyroCfg1;
  BANK2GYROCFG2         tBank2GyroCfg2;
  BANK2ODRALIGNEN       tBank2OdrAlignEn;
  BANK2ACCELINTELCTRL   tBank2AccelIntelCtrl;
  BANK2ACCELCONFIG      tBank2AccelConfig;
  BANK2ACCELCONFIG2     tBank2AccelConfig2;
  BANK3I2CMSTCTRL       tBank3I2cMstCtrl;
  BANK3I2CSLVXCTRL      tBank3I2cSlvxCtrl;
  BANK33I2CSLVXADDR     tBank3I2cSlvxAddr;
  U8                    nByte;
} BANKXREGS,  *PBANKXREGS;
#define BANKXREGS_SIZE                          sizeof( BANKXREGS )

/// define the basic data structure
typedef struct _IMUICM20948BASE
{
  U16UN   tValueX;          ///< X value
  U16UN   tValueY;          ///< Y value
  U16UN   tValueZ;          ///< Z value
} IMUICM20948BASE, *PIMUICM20948BASE;
#define  IMUICM20948BASE_SIZE                   sizeof( IMUICM20948BASE )

/// define the data structure
typedef struct _IMUICM20948DATA
{
  IMUICM20948BASE tAccel;   ///< acceleration
  IMUICM20948BASE tGyro;    ///< gyro
  IMUICM20948BASE tMagn;    ///< magnetometer
  U16UN           tTemp;    ///< temperature
} IMUICM20948DATA, *PIMUICM20948DATA;
#define IMUICM20948DATA_SIZE                    sizeof( IMUICM20948DATA )


/**@} EOF ImuICM20948_prv.h */

#endif  // _IMUICM20948_PRV_H