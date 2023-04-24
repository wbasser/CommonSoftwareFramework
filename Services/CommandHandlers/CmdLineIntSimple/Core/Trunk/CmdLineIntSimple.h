/******************************************************************************
 * @file CmdLineIntSimple.h
 *
 * @brief	Command Line Interface simple declarations 
 *
 * This file provides the declaratins for the Command Line Interface - simple
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
 
// ensure only one instantiation
#ifndef _CMDLINEINTSIMPLE_H
#define _CMDLINEINTSIMPLE_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// helper macro to create entries
#define CDMLINIFCMD_ENTRY( cmd, args, handler ) \
  { \
    .pszCommand = ( PC8 )&cmd, \
    .nNumArgs = args, \
    .pvCmdHandler = handler \
  }

/// define the helper macro to create the table end
#define CMDLINIFCMD_ENDTBL( ) \
  { \
    .pszCommand = NULL \
  }

// enumerations ---------------------------------------------------------------
/// enumerate the errors
typedef enum _CMDLINIFERR
{
  CMDLINIF_ERR_NONE = 0,            ///< no error
  CMDLINIF_ERR_ILLHANDLE,           ///< illegal handler
  CMDLINIF_ERR_ILLARGINDEX,         ///< illegal argument index
} CMDLINIFERR;

/// enumerate the return statuss
typedef enum _CMDLINIFCMDSTS
{
  CMDLINIFCMD_STS_NONE,             ///< no errors
  CMDLINIFCMD_STS_BUFOVFL,          ///< buffer overflow
  CMDLINIFCMD_STS_ILLPROTENUM,      ///< illegal protocol enum
  CMDLINIFCMD_STS_NOCMDTABLE,       ///< on command table defined
  CMDLINIFCMD_STS_ILLCOMMAND,       ///< illegal command
  CMDLINIFCMD_STS_OUTPUTBUFFER,     ///< output the buffer
  CMDLINIFCMD_STS_MSGPROCESSED,     ///< message processed
  CMDLINIFCMD_STS_ILLNUMARGS,       ///< illegal number of coammands
} CMDLINIFCMDSTS;

// structures -----------------------------------------------------------------
/// define the Command line interface handle
typedef PVOID	( PVCMDLINFHANDLE );

/// define the write char function type
typedef void ( *PVCMDLINIFWRITEFUNC )( PC8, U16 );

/// define the command handler function
typedef CMDLINIFCMDSTS  ( *PVCMDLINIFCMDHANDLERFUNC )( PU32 puArgs );

/// define the command structure
typedef struct _CMDLINIFCMDENTRY
{
  PC8                       pszCommand;     ///< pointer to the command
  U8                        nNumArgs;       ///< number of arguments
  PVCMDLINIFCMDHANDLERFUNC	pvCmdHandler;   ///< command handler
} CMDLINIFCMDENTRY, *PCMDLINIFCMDENTRY;
#define CMDLINIFCMDENTRY_SIZE    sizeof( CMDLINIFCMDENTRY )

/// define the definition structure
typedef struct _CMDLINIFDEF
{
	U8 										nCmdSize;
	U8 										nNumArgs;
	BOOL 									bEnableEcho;
	C8 										cPrompt;
	C8 										cEndOfLine;
	PVCMDLINIFWRITEFUNC		pvWriteFunc; 
	PCMDLINIFCMDENTRY 		ptCmdTable;
} CMDLINIFDEF, *PCMDLINIFDEF;
#define	CMDLINIFDEF_SIZE				sizeof( CMDLINIFDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void            CmdLineIntSimple_Initialize( void );
extern  PVCMDLINFHANDLE CmdLineIntSimple_Create( PCMDLINIFDEF ptCmdLinInfDef );
extern  CMDLINIFERR     CmdLineIntSimple_ProcessChar( PVCMDLINFHANDLE ptHandle, C8 cRcvChar );

/**@} EOF CmdLineIntSimple.h */

#endif  // _CMDLINEINTSIMPLE_H