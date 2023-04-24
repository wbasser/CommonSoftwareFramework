/******************************************************************************
 * @file AdcADS122C04_def.h
 *
 * @brief ADC ADS122C04 definition declarations 
 *
 * This file provides the declarations for the implementation of the ADC ADS122C04
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Cyber Integration 
 * LLC. It is the exclusive property of Cyber Integration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Cyber Integration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Cyber Integration, LLC.
 *
 * \addtogroup AdcADS122C04
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _ADCADS122C04_DEF_H
#define _ADCADS122C04_DEF_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the commands
#define ADCADS122C04_CMD_RESET                  ( 0x06 )
#define ADCADS122C04_CMD_START                  ( 0x08 )
#define ADCADS122C04_CMD_PWRDWN                 ( 0x02 )
#define ADCADS122C04_CMD_RDATA                  ( 0x10 )
#define ADCADS122C04_CMD_RREG                   ( 0x20 )
#define ADCADS122C04_CMD_WREG                   ( 0x40 )

/// define the macro for creating channel entries
#define ADCADS122C04DEFM( mux, fain, pgadisable, vrefsel, samprate ) \
  { \
    .eMux = mux, \
    .eGain = gain, \
    .bPgaBypass = pgadisabble, \
    .eVrefSel = vrefsel, \
    .eSampRate = samprate, \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the registers
typedef enum _ADCADS122C04REG
{
  ADCADS122C04REG_ENUM_CONFIG0 = 0,
  ADCADS122C04REG_ENUM_CONFIG1 = 0x04,
  ADCADS122C04REG_ENUM_CONFIG2,
  ADCADS122C04REG_ENUM_CONFIG3,
} ADCADS122C04REG;

/// enumerate the input multiplexer configurations
typedef enum _ADCADS122C04INPMUX
{
  ADCADS122C04_INPMUX_AIN0PAIN1N = 0,
  ADCADS122C04_INPMUX_AIN0PAIN2N,
  ADCADS122C04_INPMUX_AIN0PAIN3N,
  ADCADS122C04_INPMUX_AIN1PAIN0N,
  ADCADS122C04_INPMUX_AIN1PAIN2N,
  ADCADS122C04_INPMUX_AIN1PAAIN3N,
  ADCADS122C04_INPMUX_AIN2PAINN3N,
  ADCADS122C04_INPMUX_AIN0PAVSS,
  ADCADS122C04_INPMUX_AIN1PAVSS,
  ADCADS122C04_INPMUX_AIN2PAVSS,
  ADCADS122C04_INPMUX_AIN3PAVSS,
  ADCADS122C04_INPMUX_REFDIFF,
  ADCADS122C04_INPMUX_SUPDIFF,
  ADCADS122C04_INPMUX_SUPTEST,
  ADCADS122C04_INPMUX_MAX
} ADCADS122C04INPMUX;

/// enumerate the gain
typedef enum _ADCADS122C04GAIN
{
  ADCADS122C04_GAIN_1 = 0,
  ADCADS122C04_GAIN_2,
  ADCADS122C04_GAIN_4,
  ADCADS122C04_GAIN_8,
  ADCADS122C04_GAIN_16,
  ADCADS122C04_GAIN_32,
  ADCADS122C04_GAIN_64,
  ADCADS122C04_GAIN_128,
} ADCADS122C04GAIN;

/// enumerate the sample rate
typedef enum _ADCADS122C04SMPRATE
{
  ADCADS122C04_SMPRATE_20SPS = 0,
  ADCADS122C04_SMPRATE_45SPS,
  ADCADS122C04_SMPRATE_90SPS,
  ADCADS122C04_SMPRATE_175PS,
  ADCADS122C04_SMPRATE_330SPS,
  ADCADS122C04_SMPRATE_600SPS,
  ADCADS122C04_SMPRATE_1000SPS,
  ADCADS122C04_SMPRATE_MAX
} ADCADS122C04SMPRATE;

/// enumerate the voltage refenrence selection
typedef enum _ADCADS122C04VREFSEL
{
  ADCADS122C04_VREFSEL_INT2V084 = 0,
  ADCADS122C04_VREFSEL_EXTREF,
  ADCADS122C04_VREFSEL_SUPPLY,
  ADCADS122C04_VREFSEL_MAX
} ADCADS122C04VREFSEL;

/// enumerate the IDAC settings
typedef enum  _ADCADS122C04IDAC
{
  ADCADS122C04_IDAC_OFF = 0,
  ADCADS122C04_IDAC_10UA,
  ADCADS122C04_IDAC_50UA,
  ADCADS122C04_IDAC_100UA,
  ADCADS122C04_IDAC_250UA,
  ADCADS122C04_IDAC_500UA,
  ADCADS122C04_IDAC_1000UA,
  ADCADS122C04_IDAC_1500UA,
} ADCADS122C04IDAC;

/// enumerate the CRC enable
typedef enum  _ADCADS122C04CRCSEL
{
  ADCADS122C04_CRCSEL_DISABLED = 0,
  ADCADS122C04_CRCSEL_DATACOMP,
  ADCADS122C04_CRCSEL_CRC16,
  ADCADS122C04_CRCSEL_MAX
} ADCADS122C04CRCSEL;

/// enmerate the IDAC conection
typedef enum _ADCADS122C04IDACSEL
{
  ADCADS122C04_IDACSEL_DIABLED = 0,
  ADCADS122C04_IDACSEL_AIN0,
  ADCADS122C04_IDACSEL_AIN1,
  ADCADS122C04_IDACSEL_AIN2,
  ADCADS122C04_IDACSEL_AIN3,
  ADCADS122C04_IDACSEL_REFP,
  ADCADS122C04_IDACSEL_REFN,
  ADCADS122C04_IDACSEL_MAX,
} ADCADS122C04IDACSEL;

// structures -----------------------------------------------------------------
/// define the configuration 0 register structure
typedef union _ADCADS122C04CFGREG0
{
  struct
  {
    BOOL                bPgaBypass  : 1;      ///< PGA bypass enable
    ADCADS122C04GAIN    eGain       : 3;      ///< gain selection
    ADCADS122C04INPMUX  eMux        : 4;      ///< mux selection
  } tFields;
  U8  nByte;
} ADCADS122C04CFGREG0, *PADCADS122C04CFGREG0;
#define ADCADS122C04CFGREG0_SIZE                 sizeof( ADCADS122C04CFGREG0 )

/// define the configuration 1 register structure
typedef union _ADCADS122C04CFGREG1
{
  struct
  { 
    BOOL                bTempSense    : 1;    ///< temperature sense enable
    ADCADS122C04VREFSEL eVrefSel      : 2;    ///< voltage reference selection
    BOOL                bContConvert  : 1;    ///< continuous conversion
    BOOL                bTurboMode    : 1;    ///< turbo mode
    ADCADS122C04SMPRATE eSampRate     : 3;    ///< sample rate
  } tFields;
  U8  nByte;
} ADCADS122C04CFGREG1, *PADCADS122C04CFGREG1;
#define ADCADS122C04CFGREG1_SIZE                sizeof( ADCADS122C04CFGREG1 )

/// define the configration 2 register structure
typedef union _ADCADS122C04CFGREG2
{
  struct
  {
    ADCADS122C04IDAC    eIdacSel      : 3;    ///< current select
    BOOL                bBurnoutEnb   : 1;    ///< enable the burnont current source
    ADCADS122C04CRCSEL  eCrcSel       : 2;    ///< CRC selection
    BOOL                bDataCntEnb   : 1;    ///< data counter enable
    BOOL                bDataReady    : 1;    ///< data ready bit
  } tFields;
  U8  nByte;
} ADCADS122C04CFGREG2, *PADCADS122C04CFGREG2;
#define ADCADS122C04CFGREG2_SIZE                sizeof( ADCADS122C04CFGREG2 )

/// define the configration 3 register structure
typedef union _ADCADS122C04CFGREG3
{
  struct
  {
    U8                  nReserved     : 2;
    ADCADS122C04IDACSEL eIdac2Sel     : 3;    ///< IDAC 2 selection
    ADCADS122C04IDACSEL eIdac1Sel     : 3;    ///< IDAC 1 selection
  } tFields;
  U8  nByte;
} ADCADS122C04CFGREG3, *PADCADS122C04CFGREG3;
#define ADCADS122C04CFGREG3_SIZE                sizeof( ADCADS122C04CFGREG3 )

/// define the configuration register global structure
typedef struct _ADCADS122C04CFGREGS
{
  ADCADS122C04CFGREG0   tConfig0;
  ADCADS122C04CFGREG1   tConfig1;
  ADCADS122C04CFGREG2   tConfig2;
  ADCADS122C04CFGREG3   tConfig3;
} ADCADS122C04CFGREGS, *PADCADS122C04CFGREGS;
#define ADCADS122C04CFGREGS_SIZE                sizeof( ADCADS122C04CFGREGS );

/// define the configuration structure
typedef struct _ADCADS122C04DEF
{
  ADCADS122C04INPMUX  eMux;             ///< mux selection
  ADCADS122C04GAIN    eGain;            ///< gain selection
  BOOL                bPgaBypass;       ///< PGA bypass enable
  ADCADS122C04VREFSEL eVrefSel;         ///< reference select
  ADCADS122C04SMPRATE eSampRate;        ///< sample rate
} ADCADS122C04DEF, *PADCADS122C04DEF;
#define ADCADS122C04DEF_SIZE                    sizeof( ADCADS122C04DEF )

/**@} EOF AdcADS122C04_def.h */

#endif  // _ADCADS122C04_DEF_H