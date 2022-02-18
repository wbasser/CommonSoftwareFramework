/******************************************************************************
 * @file FilterAveraging.h
 *
 * @brief Filter averaing declarations
 *
 * This file provides the declaration for the average filter implementation
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup FilterAveraging
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _FILTERAVERAGING_H
#define _FILTERAVERAGING_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "FilterAveraging/FilterAveraging_prm.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// determine the size of the buffer
#if ( FILTERAVERAGEING_ENABLE_HUGE == ON )
  typedef U32   FILTBUFARG;
  typedef PU32  PFILTBUFARG;
  typedef U64   FILTSUMARG;
#else
  typedef U16   FILTBUFARG;
  typedef PU16  PFILTBUFARG;
  typedef U32   FILTSUMARG;
#endif // FILTERAVERAGEING_ENABLE_HUGE

/// define the macro for naming the buffer
#define FILTERAVERAGEBUFFER( bufname ) \
    ax ## bufname ## AvgFilt

/// define the helper macro for defining the average filter buffer
#define FILTAVERAGESTORAGE( bufname, size ) \
  static  FILTBUFARG FILTERAVERAGEBUFFER( bufname)[ size ];

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the average filter definition structure
typedef struct _FILTAVERAGEDEF
{
  PFILTBUFARG   pxBuffer;                     ///< pointer to the buffer
  U8            nNumberSamples;               ///< number of samples
} FILTAVERAGEDEF, *PFILTAVERAGEDEF;
#define FILTAVERAGEDEF_SIZE   sizeof( FILTAVERAGEDEF )

/// define the average filter control structure
typedef struct _FILTAVERAGECTL
{
  FILTAVERAGEDEF  tDef;                         ///< definition
  U8              nSampleIndex;                 ///< sample index
  FILTSUMARG      xSum;                         ///< sum of all samples
  BOOL            bValid;                       ///< valid
} FILTAVERAGECTL, *PFILTAVERAGECTL;
#define FILTAVERAGECTL_SIZE   sizeof( FILTAVERAGECTL )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
//extern  void        FilterAveraging_Initialize( PFILTAVERAGECTL ptCtl, U8 nNumberSamples, PFILTBUFARG pxBuffer );
extern  void        FilterAveraging_Initialize( PFILTAVERAGECTL ptCtl );
extern  BOOL        FilterAveraging_Execute( PFILTAVERAGECTL ptCtl, FILTBUFARG xNewSample );
extern  FILTBUFARG  FilterAveraging_GetValue( PFILTAVERAGECTL ptCtl );

/**@} EOF FilterAveraging.h */

#endif  // _FILTERAVERAGING_H