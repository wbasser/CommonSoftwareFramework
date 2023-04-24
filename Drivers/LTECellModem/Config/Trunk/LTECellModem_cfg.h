/******************************************************************************
 * @file LTECellModem_cfg.h
 *
 * @brief LTE Cellular Modem driver configuratin decalarations
 *
 * This file provides the declarations for the LTE cellular modem
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
 * \addtogroup LTECellModem
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _LTECELLMODEM_CFG_H
#define _LTECELLMODEM_CFG_H

// system includes ------------------------------------------------------------
#include "Types/Types.h"

// local includes -------------------------------------------------------------
#include "LTECellModem/LTECellModem.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the number of events for the control task
#define LTECELLMODEM_CTLTASK_NUM_EVENTS             ( 4 )

/// define the number of events for the character task
#define LTECELLMODEM_CHRTASK_NUM_EVENTS             ( 80 )

/// define the receive/transmit buffer sizes
#define LTECELLMODEM_RCVBUF_SIZE                    ( 0 )
#define LTECELLMODEM_XMTBUF_SIZE                    ( 80 )

// enumerations ---------------------------------------------------------------
/// enumerate the local events
typedef enum _LTELCLEVENT
{
  LTE_LCLEVENT_XMTMSG = 0xCE30,
  LTE_LCLEVENT_RCVMSG,
  LTE_LCLEVENT_TIMEOUT = TASK_TIMEOUT_EVENT,
} LTELCLEVENT;

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  LTECellModem_LocalInitialize( void );
extern  void  LTECellModem_Write( PC8 pcData, U16 wLength );
extern  BOOL  LTECellModem_ProcessChar( TASKARG xArg );
extern  BOOL  LTECellModem_ProcessCtrl( TASKARG xArg );
extern  void  LTECellModem_StartStopTime( U16 wTimeMsecs );
extern  void  LTECellModem_PowerControl( BOOL bState );
extern  void  LTECellModem_PostCtrlEvent( LTELCLEVENT eEvent );

/**@} EOF LTECellModem_cfg.h */

#endif  // _LTECELLMODEM_CFG_H