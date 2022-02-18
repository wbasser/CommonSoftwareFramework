/******************************************************************************
 * @file DdsAD9833.h
 *
 * @brief direct digital synthesizer AD9833 declarations 
 *
 * This file provides the declarations for the DDS AD9833
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
 * \addtogroup DdsAD9833
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DDSAD9833_H
#define _DDSAD9833_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DdsAD9833/DdsAd9833_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _DDSAD9833ERR
{
  DDSAD9833_ERR_NONE = 0,
  DDSAD9833_ERR_ILLFVAL = 0xF0,
  DDSAD9833_ERR_ILLFSEL,
  DDSAD9833_ERR_ILLPVAL,
  DDSAD9833_ERR_ILLPSEL,
  DDSAD9833_ERR_ILLMODE,
  DDSAD9833_ERR_ILLWAVE,
} DDSAD9833ERR;

// enumerate the modes
typedef enum _DDSAD9833MODE
{
  DDSAD9833_MODE_CW = 0,
  DDSAD9833_MODE_SWEEP,
  DDSAD9833_MODE_MAX
} DDSAD9833MODE;

/// enumerate the wave types
typedef enum _DDSAD9833WAVE
{
  DDSAD9833_WAVE_NONE = 0,
  DDSAD9833_WAVE_SIN,
  DDSAD9833_WAVE_TRI,
  DDSAD9833_WAVE_SQR,
  DDSAD9833_WAVE_MAX
} DDSAD9833WAVE;

// enumerate the frequency selects
typedef enum _DDSAD9833FREQSEL
{
  DDSAD9833_FREQSEL_0 = 0,
  DDSAD9833_FREQSEL_1,
  DDSAD9833_FREQSEL_MAX
} DDSAD9833FREQSEL;

// enumerate the Phase selects
typedef enum _DDSAD9833PHASESEL
{
  DDSAD9833_PHASESEL_0 = 0,
  DDSAD9833_PHASESEL_1,
  DDSAD9833_PHASESEL_MAX
} DDSAD9833PHASESEL;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void          DdsAD9833_Initialize( void );
extern  void          DdsAD9833_Control( BOOL bState );
extern  DDSAD9833ERR  DdsAD9833_SetMode( DDSAD9833MODE eMode );
extern  DDSAD9833ERR  DdsAD9833_SetWave( DDSAD9833WAVE eWave );
extern  DDSAD9833ERR  DdsAD9833_SetFrequency( DDSAD9833FREQSEL eFreqSel, U32 uFreqVal );
extern  DDSAD9833ERR  DdsAD9833_SetPhase( DDSAD9833PHASESEL ePhaseSel, U16 wFreqVal );

/**@} EOF DdsAD9833.h */

#endif  // _DDSAD9833_H