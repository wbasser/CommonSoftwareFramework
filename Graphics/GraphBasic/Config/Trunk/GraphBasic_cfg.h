/******************************************************************************
 * @file GraphBasic_cfg.h
 *
 * @brief	graphics basic configuraiton declarations 
 *
 * This file provides the declarations for the graph basic
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
 * \addtogroup GraphBasic_cfg
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _GRAPHBASIC_CFG_H
#define _GRAPHBASIC_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "GraphBasic/GraphBasic_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern	BOOL  GraphBasic_LocalInitialize( void );
extern  void  GraphBasic_RefreshScreen( void );
extern  U16   GraphBasic_GetMaxX( void );
extern  U16   GraphBasic_GetMaxY( void );
extern  U16   GraphBasic_GetFontX( void );
extern  void  GraphBasic_PixelDraw( U16 wX, U16 wY, GRAPHBASICPIXACT eAction, GRAPHBASICCLR eColor );

/**@} EOF GraphBasic_cfg.h */

#endif  // _GRAPHBASIC_CFG_H