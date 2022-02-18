/******************************************************************************
 * @file Main.h
 *
 * @brief Main include files
 *
 * This file provides the declarations for the main functions
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
 *
 * \addtogroup Main
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _MAIN_H
#define _MAIN_H

// local includes -------------------------------------------------------------
#include "Main_cfg.h"

// library includes
#if ( MAIN_ENABLE_SYSTIMECMDS == ON )
  #include "AsciiCommandHandler/AsciiCommandHandler.h"
#endif // MAIN_ENABLE_SYSTIMECMDS

// global paramter declarations  ------------------------------------------------
#if ( MAIN_ENABLE_SYSTIMECMDS  == ON )
  extern  const CODE ASCCMDENTRY g_atMainCmdHandlerTable[ ];
#endif // MAIN_ENABLE_SYSTIMECMDS

// global function prototypes --------------------------------------------------
extern  void  Main_Shutdown( void );

/**@} EOF Main.h */

#endif  // _MAIN_H