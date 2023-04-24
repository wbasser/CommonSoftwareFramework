/******************************************************************************
 * @file HtmlPageHandler.c
 *
 * @brief LWIP HTTP handler
 *
 * This file provides the HTTP functionality for the LWIP HTTP handler
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

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "HtmlPageHandler/HtmlPageHandler.h"

// library includes -----------------------------------------------------------
#include "HtmlPageDefs/HtmlPageDefs.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum size of the dynamic page buffer
#define HTML_BUFFER_SIZE                        ( 4096 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  U8      anDynPageBuffer[ HTML_BUFFER_SIZE ];

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function HtmlPageHandler_Initialize
 *
 * @brief LWIP HTTP handler initialization
 *
 * This function initializes the LWIP HTTP implementation.  It also provides
 * the file handling for local files
 *
 * @return  TRUE if errors found, FALSE if not
 *
 *****************************************************************************/
BOOL HtmlPageHandler_Initialize( void )
{
  // return OK
  return( FALSE );
}

/******************************************************************************
 * @function HtmlPageHandler_Generate
 *
 * @brief generate a dynamic page
 *
 * This function will generate a dynamic page
 *
 * @param[in]   ptPage      pointer to the page definition
 * @param[io]   pwPageSize  size of the actual page
 *
 * @return      page size
 *
 *****************************************************************************/
PU8 HtmlPageHandler_Generate( HTMLDYNPAGE *ptPage, PU16 pwPageSize )
{
  U16 wNumBytes = 0;

  // clear the dynamic page
  memset( anDynPageBuffer, 0, HTML_BUFFER_SIZE );

  // generate the page header
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, g_szPageHtb );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, g_szPageHdb );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, g_szMetaBeg );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, ptPage->pcMetaAction );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szMetaCnt, ptPage->nTime );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, g_szMetaEnd );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, g_szPageTtb );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, ptPage->pcTitle );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, g_szPageTte );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, g_szPageHde );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, g_szPageBdb );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFmtStrn, g_szPageSty, ptPage->pcAlign );

  // generate the title
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szPageClr, ptPage->uPageBackColor, ptPage->uPageLinkColor, ptPage->uPageVlnkColor, ptPage->uPageAlnkColor );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szHtmlLbk );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szPageBfn, ptPage->nFontSize, ptPage->uPageTextColor, g_szFntTahoma );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szPageH1b );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, ptPage->pcTitle );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szPageH1e );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szPageHrw, ptPage->nSepWidth, ptPage->uSepColor );

  // generate the form beginning
  if ( ptPage->bEnableForm != NULL )
  {
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFormAct, ptPage->pcFormMethod );
  }

  // now print the contents
  if ( ptPage->pvTableGen != NULL )
  {
    // do it
    wNumBytes = ptPage->pvTableGen( anDynPageBuffer, HTML_BUFFER_SIZE, wNumBytes );
  }

  // generate a separator
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szPageHrw, ptPage->nSepWidth, ptPage->uSepColor );

  // only populate the submit button if label is not null
  if ( ptPage->pcLabel != NULL )
  {
    // generate the submit button
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szTablBeg );
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szTablRwb );
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szTablTdb );
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szBtnsSub, ptPage->pcLabel );
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szTablTde );
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szTablRwe );
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szTablEnd );
  }

  // generate the form end
  if ( ptPage->bEnableForm != NULL )
  {
    // generate the table footer
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szFormEnd );
  }

  // generate the return link
  if ( ptPage->pcReturnLabel != NULL )
  {
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szPageHrw, ptPage->nSepWidth, ptPage->uSepColor );
    wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szHtmlLnk, ptPage->pcReturnLink, ptPage->pcReturnLabel );
  }

  // generate the bottom of page
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szPageBde );
  wNumBytes += snprintf( &anDynPageBuffer[ wNumBytes ], HTML_BUFFER_SIZE, g_szPageHte );
  
  // set the number of bytes
  *( pwPageSize ) = wNumBytes;

  // return the page buffer
  return( anDynPageBuffer );
}

PU8 HtmlPageHandler_PageNotFound( void )
{
}

/**@} EOF HtmlPageHandler.c */
