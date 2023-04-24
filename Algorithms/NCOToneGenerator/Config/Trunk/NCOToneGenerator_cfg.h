/******************************************************************************
 * @file NCOToneGenerator.h
 *
 * @brief tone generator declarations
 *
 * This file provides the declarations for the tone generator
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
 * \addtogroup NCOToneGenerator
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _NCOTONEGENERATOR_H
#define _NCOTONEGENERATOR_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Timers/Timers.h"

// Macros and Defines ---------------------------------------------------------
#define NCOTONEGENERATOR_SAMPLE_RATE                         ( 8000 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  NCOToneGenerator_Initialize( void );
extern  void  NCOToneGenerator_Control( BOOL bState );
extern  void  NCOToneGenerator_SetFrequency( U16 wFrequency  );
extern  void  NCOToneGenerator_TimerCallback( TIMERCBEVENT eEvent, TIMERCMPCAP eChan, U16 wValue );  

/**@} EOF NCOToneGenerator.h */

#endif  // _NCOTONEGENERATOR_H