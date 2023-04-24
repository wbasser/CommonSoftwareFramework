/******************************************************************************
 * @file HtmlPageHandler_def.h
 *
 * @brief Stellaris LWIP HTTP handler definition declarations
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
#ifndef _HTMLPAGEHANDLER_DEF_H
#define _HTMLPAGEHANDLER_DEF_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the helper macro for the dynamic web page entries
#define HTMLDYNPAGEDEF( page, title, align, time, rtnlabel, rtnlink, enableform, pbckclr, txtclr, lnkclr, vlnkclr, alnkclr, fontsize, sepcolor, sepwidth, genfunc, linkfunc, btnlabel, metaaction, formmethod ) \
  { \
    .pcPage = ( PCC8 )page, \
    .pcTitle = ( PCC8 )title, \
    .pcAlign = ( PCC8 )align, \
    .nTime = time, \
    .pcReturnLabel = ( PC8 )rtnlabel, \
    .pcReturnLink = ( PC8 )rtnlink, \
    .bEnableForm = enableform, \
    .uPageBackColor = pbckclr, \
    .uPageTextColor = txtclr, \
    .uPageLinkColor = lnkclr, \
    .uPageVlnkColor = vlnkclr, \
    .uPageAlnkColor = alnkclr, \
    .nFontSize = fontsize, \
    .uSepColor = sepcolor, \
    .nSepWidth = sepwidth, \
    .pvTableGen = genfunc, \
    .pvLinkGen = linkfunc, \
    .pcLabel = ( PC8 )btnlabel, \
    .pcMetaAction = ( PC8 )metaaction, \
    .pcFormMethod = ( PC8 )formmethod \
  }

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the dynamic page generation function type
typedef U16 ( *PVDYNHTMLPAGE )( PU8, U16, U16 );

/// define the dynamic web page entry
typedef struct _HTMLDYNPAGE
{
  PCC8          pcPage;         ///< page name
  PCC8          pcTitle;        ///< title
  PCC8          pcAlign;        ///< alignment
  U8            nTime;          ///< expire/refresh time
  PCC8          pcReturnLabel;  ///< return label
  PCC8          pcReturnLink;   ///< return link
  BOOL          bEnableForm;    ///< form action
  U32           uPageBackColor; ///< background color
  U32           uPageTextColor; ///< text color
  U32           uPageLinkColor; ///< link color
  U32           uPageVlnkColor; ///< virtual link color
  U32           uPageAlnkColor; ///< actual link color
  U8            nFontSize;      ///< title font size
  U32           uSepColor;      ///< separator color
  U8            nSepWidth;      ///< separator width
  U8            nColSpan;       ///< column span
  U16           wColWidth;      ///< column width
  U8            nNumCols;       ///< number of columns
  U8            nBorderWidth;   ///< border width
  U8            nCellSpacing;   ///< cell spacing
  U16           wCellWidth;     ///< cell width
  U32           uBackColor;     ///< background color
  PVDYNHTMLPAGE pvTableGen;     ///< table content generator function
  PVDYNHTMLPAGE pvLinkGen;      ///< link content generator function
  PCC8          pcLabel;        ///< button label
  PCC8          pcMetaAction;   ///< meta action
  PCC8          pcFormMethod;   ///< form method
} HTMLDYNPAGE, *PHTMLDYNPAGE;
#define HTMLDYNPAGE_SIZE  sizeof( HTMLDYNPAGE )

// global parameter declarations -----------------------------------------------

/**@} EOF HtmlPageHandler_def.h */

#endif  // _HTMLPAGEHANDLER_DEF_H