/******************************************************************************
 * @file CnvDAC7581.h
 *
 * @brief Converter DAC7581 driver declarations 
 *
 * This file provides the declarations for the DAC7581 converter
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
 * \addtogroup CnvDAC7581
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CNVDAC7581_H
#define _CNVDAC7581_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "CnvDAC7581/CnvDAC7581_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  CnvDAC7581_Initialize( void );
extern  void  CnvDAC7581_SetVoltsOut( FLOAT fValue );
extern  void  CnvDAC7581_SetCountsOut( U16 wValue );

/**@} EOF CnvDAC7581.h */

#endif  // _CNVDAC7581_H