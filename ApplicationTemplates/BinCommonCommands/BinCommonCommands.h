/******************************************************************************
 * @file CommandCommandsBin.h
 *
 * @brief binary common commands 
 *
 * This file provides the declarations of the binary common commands
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
 * \addtogroup CommandCommandsBin
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef BINCOMMONCOMMANDS_H
#define BINCOMMONCOMMANDS_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "BinaryCommandHandler/BinaryCommandHandler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the commands
typedef enum _BINCMNCMDS
{
  // normal commands
  BINCMN_CMD_GETPNG = 0x80,       ///< ping and resond
  BINCMN_CMD_GETREV,              ///< get the software revision
  BINCMN_CMD_RSTDFL,              ///< reset defaults
  BINCMN_CMD_QRYMOD,              ///< query mode
  BINCMN_CMD_SETMOD,              ///< set mode
  BINCMN_CMD_QRYSNM,              ///< query the serial number
  BINCMN_CMD_GETCRV = 0x8E,       ///< get configuration revision
  BINCMN_CMD_BOOTEN = 0x90,       ///< enter boot mode

  // do not remove the below entry
  BINCMN_CMD_MAX
} BINCMNCMDS;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern const CODE BINCMDSLVENTRY g_atBinCommonCommandsTable[ ];

// global function prototypes --------------------------------------------------

/**@} EOF BinCommandCommands.h */

#endif  // BINCOMMONCOMMANDS_H