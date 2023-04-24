/****************************************7**************************************
 * @file  DALIMessageHandler.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
 *
 * @copyright Copyright (c) 2012 Nan Technologies
 * This document contains proprietary data and information of Nan Technologies
 * LLC. It is the exclusive property of Nan Technologies, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * Nan Technologies, LLC. This document may not be reproduced or further used
 * without the prior written permission of Nan Technologies, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 *
 *
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIMESSAGEHANDLER_H
#define _DALIMESSAGEHANDLER_H

// system includes ------------------------------------------------------------

// library includes -----------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIMessageHandler/DALIMessageHandler_cfg.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------
extern  PDALICTL  g_ptDaliCtl;  ///< pointer to the control structure

// global function prototypes --------------------------------------------------
extern  BOOL              DALIMessageHandler_Initialize( void );
extern	void              DALIMessageHandler_Control( BOOL bState );
extern  void              DALIMessageHandler_EventHandler( DALIMSGHANDEVENT eEvent );
extern  void              DALIMessageHandler_TimeoutHandler( void );
extern	void              DALIMessageHandler_OutputHandler( void );
extern  void              DALIMessageHandler_BusFailHandler( void );
extern  DALIMSGHANDCFGSTS DALIMessageHandler_GetConfigStatus( void );
extern  void              DALIMessageHandler_ForceMessage( U8 nAddr, U8 nDataCmd );

/**@} EOF DALIMessageHandler.h */

#endif  // _DALIMESSAGEHANDLER_H
