/******************************************************************************
 * @file CmdLineIntSimple.c
 *
 * @brief command line interface implementation
 *
 * This file provides the implementaion for the command line interface
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
 * \addtogroup CmdLineIntSimple
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include <ctype.h>

// local includes -------------------------------------------------------------
#include "CmdLineIntSimple/CmdLineIntSimple.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PCMDLINIF )handle )

// enumerations ---------------------------------------------------------------
/// enumerate the state
typedef enum _PARSESTATE
{
  PARSE_STATE_CMD = 0,
  PARSE_STATE_ARG,
  PARSE_STATE_MAX
} PARSESTATE;

// structures -----------------------------------------------------------------
/// define the local control structure
typedef struct _CMDLINIF
{
  struct _CMDLINIF*   ptSignature;    ///< signature
  struct _CMDLINIF*   ptNextCmdLinIf; ///< pointer to the next command line interface
  U8                  nArgIdx;       	///< argument index
	U8									nCmdIdx;				///< command index
  PC8                 pcCmd;       		///< pointer to the buffer
  PU32                puArgs;         ///< pointer to the numeric arguments
  PARSESTATE          eParsestate;    ///< parse state
	CMDLINIFDEF					tDefs;					///< definitions
} CMDLINIF, *PCMDLINIF;
#define CMDLINIF_SIZE       sizeof( CMDLINIF )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PCMDLINIF   ptFirstCmdLinIf;          

// local function prototypes --------------------------------------------------
static  BOOL        CheckForDelimiter( C8 cChar );
static  CMDLINIFERR ParseMessage( PCMDLINIF ptCmdLinInf );

// constant parameter initializations -----------------------------------------
/// declare a standard set of delimiters
static  const C8  acDelims[ ] = { ',', ' ', '.', '\0' };

/******************************************************************************
 * @function CmdLineIntSimple_Initialize
 *
 * @brief initialization
 *
 * This function will perform any needed initialization
 *
 *****************************************************************************/
void CmdLineIntSimple_Initialize( void )
{
  // reset the first pointer
  ptFirstCmdLinIf = NULL;
}

/******************************************************************************
 * @function CmdLineIntSimple_Create
 *
 * @brief create a command line interface
 *
 * This function will create a command line interface
 *
 * @param[in]   ptCmdLinInfDef		pointer to the definition
 *
 * @return      handle or NULL if error
 *
 *****************************************************************************/
PVCMDLINFHANDLE CmdLineIntSimple_Create( PCMDLINIFDEF ptCmdLinInfDef )
{
  PCMDLINIF ptNewCmdLinIf, ptCurCmdLinIf, ptLstCmdLinIf;
  
  // allocate space for new task
  if (( ptNewCmdLinIf = malloc( CMDLINIF_SIZE )) != NULL )
  {
    // now allocate space for the BUFFER
    if (( ptNewCmdLinIf->pcCmd = malloc( ptCmdLinInfDef->nCmdSize + 1 )) != NULL )
    {
			// allocate the argument space
			if (( ptNewCmdLinIf->puArgs = malloc( sizeof( U32 ) * ptCmdLinInfDef->nNumArgs )) != NULL )
			{
				// now initialize the control structure
				ptNewCmdLinIf->ptSignature    = ptNewCmdLinIf;
				ptNewCmdLinIf->ptNextCmdLinIf = NULL;
				ptNewCmdLinIf->nArgIdx       	= 0;
				ptNewCmdLinIf->nCmdIdx				= 0;
        ptNewCmdLinIf->eParsestate    = PARSE_STATE_CMD;
				
				// copy the config
				memcpy( &ptNewCmdLinIf->tDefs, ptCmdLinInfDef, CMDLINIFDEF_SIZE );
				
				// search for the entry point
				for ( ptCurCmdLinIf = ptFirstCmdLinIf, ptLstCmdLinIf = NULL; ptCurCmdLinIf != NULL; ptLstCmdLinIf = ptCurCmdLinIf, ptCurCmdLinIf = ptCurCmdLinIf->ptNextCmdLinIf );

				// add at end
				ptLstCmdLinIf->ptNextCmdLinIf = ptNewCmdLinIf;
			}
			else
			{
				// error - not enough room
				free( ptNewCmdLinIf->puArgs );
				free( ptNewCmdLinIf->pcCmd );
				free( ptNewCmdLinIf );
				ptNewCmdLinIf = NULL;
			}
		}
    else
		{
			// error - not enough room
			free( ptNewCmdLinIf->pcCmd );
			free( ptNewCmdLinIf );
			ptNewCmdLinIf = NULL;
		}
	}
	else
	{
		// error - not enough room
		free( ptNewCmdLinIf );
		ptNewCmdLinIf = NULL;
	}
  
  // return the handle
  return(( PVCMDLINFHANDLE )ptNewCmdLinIf );
}

/******************************************************************************
 * @function CmdLineIntSimple_ProcessChar
 *
 * @brief processing incoming characters
 *
 * This function will process all incoming characters
 *
 * @param[in]   ptHandle    pointer to the command line handler
 * @param[in]   cRcvChar    character received
 *
 * @return      appropriate error
 *
 *****************************************************************************/
CMDLINIFERR CmdLineIntSimple_ProcessChar( PVCMDLINFHANDLE ptHandle, C8 cRcvChar )
{
  CMDLINIFERR eError = CMDLINIF_ERR_NONE;
  PCMDLINIF   ptCmdLinIf;
  BOOL        bDelimFound, bEndOfLine, bParseMessage;

  // clear the parse message
  bParseMessage = FALSE;

  // map the pointer
  ptCmdLinIf = MAP_HANDLE_TO_POINTER( ptHandle );

  // first validate that the task is valid
  if ( ptCmdLinIf == ptCmdLinIf->ptSignature )
  {
    // check for echo
    if ( ptCmdLinIf->tDefs.bEnableEcho )
    {
      // echo character
      ptCmdLinIf->tDefs.pvWriteFunc( &cRcvChar, 1 );
    }

    // check for a delimiter/end of line
    bDelimFound = CheckForDelimiter( cRcvChar );
    bEndOfLine = ( ptCmdLinIf->tDefs.cEndOfLine == cRcvChar ) ? TRUE : FALSE;
    
    // now process the characters
    switch( ptCmdLinIf->eParsestate )
    {
      case PARSE_STATE_CMD :
        // check for delimeter or end of line
        if ( bDelimFound || bEndOfLine )
        {
          // mark the end of the command

          *( ptCmdLinIf->pcCmd + ptCmdLinIf->nCmdIdx ) = '\0';

          // if not eol, go to arguments
          if ( !bEndOfLine )
          {
            // change state to arguments
            ptCmdLinIf->eParsestate = PARSE_STATE_ARG;
          }
          else
          {
            // return end of message
            bParseMessage = ( ptCmdLinIf->nCmdIdx != 0 ) ? TRUE : FALSE;
          }
        }
        else
        {
          // check for a ascii character
          if ( isalnum( cRcvChar ))
          {
            // check fo room
            if ( ptCmdLinIf->nCmdIdx < ptCmdLinIf->tDefs.nCmdSize )
            {
              // make it capitaql/add the character
              cRcvChar = toupper( cRcvChar );
              *( ptCmdLinIf->pcCmd + ptCmdLinIf->nCmdIdx++ ) = cRcvChar;
            }
            else
            {
              // set error for too long of a command
              eError = CMDLINIFCMD_STS_BUFOVFL;
            }
          }
        }
        break;

      case PARSE_STATE_ARG :
        // check for delimeter or end of line
        if ( bDelimFound || bEndOfLine )
        {
          // increment the argument index
          ptCmdLinIf->nArgIdx++;

          // end of command
          bParseMessage = bEndOfLine;
        }
        else
        {
          // check for a valid digit
          if ( isdigit( cRcvChar ))
          {
            // multiply by ten
            ptCmdLinIf->puArgs[ ptCmdLinIf->nArgIdx ] *= 10;
            ptCmdLinIf->puArgs[ ptCmdLinIf->nArgIdx ] += ( cRcvChar - '0' );
          }
        }
        break;

      default :
        // should never happen
        ptCmdLinIf->eParsestate = PARSE_STATE_CMD;
        break;
    }

    // now determine if we have the end of a message
    if ( bParseMessage )
    {
      // parse the message
      eError = ParseMessage( ptCmdLinIf );
    }
  }
  else
  {
    // set the error
    eError = CMDLINIF_ERR_ILLHANDLE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function CheckForDelimiter 
 *
 * @brief CheckForDelimiter
 *
 * This function will check to see if this is a delimiter
 *
 * @param[in]   cChar   character to test
 *
 * @return      TRUE if found, FALSE otherwise
 *
 *****************************************************************************/
static BOOL CheckForDelimiter( C8 cChar )
{
  BOOL  bFound = FALSE;
  U8    nIdx, nDelim;

  // search the dlimiters
  nIdx = 0;
  while(( nDelim = acDelims[ nIdx++ ]) != 0 )
  {
    // test for match
    if ( nDelim == cChar )
    {
      // set found/break loop
      bFound = TRUE;
      break;
    }
  }

  // return the status
  return( bFound );
}

/******************************************************************************
 * @function ParseMessage
 *
 * @brief parse the message
 *
 * This function will search the command line and process it if found
 *
 * @param[in]   ptCmdLinInf   poitner to the controls tructure
 *
 * @return      appropriate error
 *
 *****************************************************************************/
static CMDLINIFERR ParseMessage( PCMDLINIF ptCmdLinInf )
{
  CMDLINIFERR               eError = CMDLINIF_ERR_NONE;
  U8                        nIdx;
  BOOL                      bRunFlag;
  PCMDLINIFCMDENTRY         ptCmdTableEntry;
  PVCMDLINIFCMDHANDLERFUNC  pvCmdFunction;

  // check for a table present
  if ( NULL != ptCmdLinInf->tDefs.ptCmdTable )
  {
    // clear the table index/set runflag to true
    nIdx = 0;
    bRunFlag = TRUE;

    while( bRunFlag )
    {
      // get the pointer to the table
      ptCmdTableEntry = &ptCmdLinInf->tDefs.ptCmdTable[ nIdx ];

      // check for end of table
      if ( NULL != ptCmdTableEntry->pszCommand )
      {
        // compare to the entered command
        if ( strcmp( ptCmdTableEntry->pszCommand, ptCmdLinInf->pcCmd ) == 0 )
        {
          // clear the run flag
          bRunFlag = FALSE;

          // command found - check for number of commands
          if ( ptCmdTableEntry->nNumArgs == ptCmdLinInf->nArgIdx )
          {
            // get the entry - execute it
            if ( NULL != ( pvCmdFunction = ptCmdTableEntry->pvCmdHandler ))
            {
              // execute it
              pvCmdFunction( ptCmdLinInf->puArgs );
            }
          }
          else
          {
            // set the error, illegal number of arguments
            eError = CMDLINIFCMD_STS_ILLNUMARGS;
          }
        }
        else
        {
          // increment the index
          nIdx++;
        }
      }
      else
      {
        // indicate command not found/clear run flag
        eError = CMDLINIFCMD_STS_ILLCOMMAND;
      }
    }
  }
  else
  {
    // set the error
    eError = CMDLINIFCMD_STS_NOCMDTABLE;
  }
  
  // return the status
  return( eError );
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/

/**@} EOF CmdLineIntSimple.c */
