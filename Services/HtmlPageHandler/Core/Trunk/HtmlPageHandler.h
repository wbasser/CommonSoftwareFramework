/******************************************************************************
 * @file HtmlPageHandler.h
 *
 * @brief Stellaris LWIP HTTP handler declarations
 *
 * This file provides the declarations for the LWIP HTTP handler
 *
 * @copyright Copyright (c) 2012 Cyber Intergration
 * This document contains proprietary data and information of Endurance Products 
 * LLC. It is the exclusive property of Endurance Products, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * Endurance Products, LLC. This document may not be reproduced or further used 
 * without the prior written permission of Endurance Products, LLC.
 *
 * Version History
 * ======
 * $Log: $
 * 
 *
 * \addtogroup HtmlPageHandler
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _HTMLPAGEHANDLER_H
#define _HTMLPAGEHANDLER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HtmlPageHandler/HtmlPageHandler_def.h"

// library includes -----------------------------------------------------------
#include "HTMLPageDefs/HTMLPageDefs.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  HtmlPageHandler_Initialize( void );
extern  PU8   HtmlPageHandler_Generate( HTMLDYNPAGE *ptPage, PU16 pwPageSize );
extern  PU8   HtmlPageHandler_PageNotFound( void );

/**@} EOF HtmlPageHandler.h */

#endif  // _HTMLPAGEHANDLER_H