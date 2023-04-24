/******************************************************************************
 * @file Uart_cfg.h
 *
 * @brief UART configuration declarations
 *
 * This file contains the UARt configuration declarations
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be
 * disclosed in any form to any party without prior written permission of
 * CyberIntegration, LLC. This document may not be reproduced or further used
 * without the prior written permission of CyberIntegration, LLC.
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup UART
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _UART_CFG_H
#define _UART_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart_def.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// set these to one when selecting a channel to use
#define UART_CHAN0_IN_USE     ( OFF )
#define UART_CHAN1_IN_USE     ( OFF )
#define UART_CHAN2_IN_USE     ( OFF )
#define UART_CHAN3_IN_USE     ( OFF )
#define UART_CHAN4_IN_USE     ( OFF )
#define UART_CHAN5_IN_USE     ( OFF )

/// How long to wait since last byte received before issuing a timeout
#define UART_RX_TIMEOUT_MS    100

/// Increased to 500ms 
#define UART_TX_TIMEOUT_MS    500

/// UART RX/TX buf size
#define UART_RX_BUF_SIZE      100
#define UART_TX_BUF_SIZE      100

/// TODO: Identify if 255-byte write is Zephyr-imposed or nRF52-imposed
#define UART_TX_LIMIT_BYTES   255

/// TODO Clean this up dude - DR
#define DOCKCOMMS_DUMMY_BUF_SIZE  1

/// Turn this ON to enable ASYNC usage
#define UART_DMA_ASYNC_IN_USE  ( ON )  

#define UART_DEBUG_ENABLE      ( OFF )

// enumerations ---------------------------------------------------------------
/// declare the UART enumerations
typedef enum  _UARTDEVENUM
{
  // enumerate devices below
  UART_DEV_ENUM_DOCKSER = 0,

  // do not remove these entries
  UART_DEV_ENUM_MAX,
  UART_DEV_ENUM_ILLEGAL = 0xFF
} UARTDEVENUM;


// structures -----------------------------------------------------------------

/// Struct for UART RX events
typedef struct _SUartTx
{
  UARTDEVENUM   eDev;
  PU8           pnTxBuf;
  U16           wRemaining;
} SUartTx, *PUartTx;
#define SIZEOF_UARTTX    sizeof(SUartTx)

// global function prototypes --------------------------------------------------
extern  const UARTDEF g_atUartDefs[ ];
extern UARTDoubleBuffer sRxData;

/**@} EOF Uart_cfg.h */

#endif  // _UART_CFG_H
