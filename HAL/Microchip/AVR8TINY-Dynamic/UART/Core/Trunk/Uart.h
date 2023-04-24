/******************************************************************************
 * @file Uart.h
 *
 * @brief 
 *
 * This file 
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
 * $Log: $
 * 
 *
 * \addtogroup Uart
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _UART_H
#define _UART_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "Types/Types.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the error values
typedef enum _UARTERR
{
  UART_ERR_NONE = 0,        ///< no error
  UART_ERR_ILLDEV,          ///< illegal device
  UART_ERR_ILLPRM,          ///< illegal parameter
  UART_ERR_ILLPER,          ///< illegal peripheral
  UART_ERR_ILLACT           ///< illegal IOCTL action
} UARTERR;

/// enumerate the UART channels
typedef enum _UARTCHAN
{
  UART_CHAN_0 = 0,
  UART_CHAN_1,
  UART_CHAN_MAX
} UARTCHAN  ;

/// enumerate the callback events
typedef enum _UARTIRQEVENTS
{
  UART_IRQ_EVENT_RXCHAR,  ///< receive character event
  UART_IRQ_EVENT_RXERR,   ///< receive error event
  UART_IRQ_EVENT_TXEMP,   ///< transmit empty event
  UART_IRQ_EVENT_TXCMP,   ///< transmit complete event
  UART_IRQ_EVENT_BRKDET   ///< break detect event
} UARTIRQEVENTS;

/// enumerate the modes
typedef enum _UARTMODE
{
  UART_MODE_ASYNC = 0,
  UART_MODE_SYNC, 
  UART_MODE_MSTSPI = 3
} UARTMODE;

/// enumerate the word lengths
typedef enum _UARTWLEN
{
  UART_WLEN_5 = 0,
  UART_WLEN_6,
  UART_WLEN_7,
  UART_WLEN_8,
  UART_WLEN_9 = 7
} UARTWLEN;

/// enumerate the stop bits
typedef enum _UARTSTOP
{
  UART_STOP_1 = 0,
  UART_STOP_2
} UARTSTOP;

/// enumerate the parity
typedef enum _UARTPARITY
{
  UART_PARITY_NONE = 0,
  UART_PARITY_EVEN = 2,
  UART_PARITY_ODD
} UARTPARITY;

// structures -----------------------------------------------------------------
/// define the callback function
typedef void ( *PVUARTIRQCALLBACK )( UARTIRQEVENTS eIrqEvent, U8 nOption );

/// define the task handle
typedef PVOID   PTUARTHANDLE;

/// define the UART control structure
typedef struct _UARTDEF
{
  UARTMODE          eMode;        ///< uart mode
  UARTCHAN          eChan;        ///< usart channel
  U32               uBaudRate;    ///< baud rate
  UARTWLEN          eWordLen;     ///< word length
  UARTPARITY        eParity;      ///< parity
  UARTSTOP          eStopBits;    ///< stop bits
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    U16               wRxBufSize;   ///< RX buffer size
  #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_MINIMAL
  U16               wTxBufSize;   ///< RX buffer size
  PVUARTIRQCALLBACK pvCallback;   ///< event callback handler
} UARTDEF, *PUARTDEF;
#define UARTDEF_SIZE    sizeof( UARTDEF )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  PTUARTHANDLE  Uart_Configure( UARTDEF* ptDef );
extern  UARTERR       Uart_Write( PTUARTHANDLE ptHandle, PU8 pnData, U16 wLength, PU16 pwBytesWritten );
#if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
  extern  UARTERR Uart_Read( PTUARTHANDLE ptHandle, PU8 pnData, U16 wLength, PU16 pwBytesRead );
#endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_MINIMAL
extern  UARTERR       Uart_Close( PTUARTHANDLE ptHandle );

/**@} EOF Uart.h */

#endif  // _UART_H