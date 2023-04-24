/******************************************************************************
 * @file Uart.c
 *
 * @brief UART implementation
 *
 * This file provides the implemntation for the UART peripherals
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
 * $Log: $
 * 
 *
 * \addtogroup UART
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------
#include <drivers/uart.h>
#include <hal/nrf_uart.h>


// local includes -------------------------------------------------------------
#include "UART/Uart.h"

// library includes -----------------------------------------------------------

#if ( UART_DEBUG_ENABLE == ON )
  #include "DebugManager/DebugManager.h"
#endif

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the buffer control structure
typedef struct _BUFCTL
{
  U16 wWrIdx;     ///< write index
  U16 wRdIdx;     ///< read index
  U16 wCount;     ///< buffer count
  PU8 pnRxBuf;
  PU8 pnTxBuf;
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE   sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  const struct device*    ptUart;
  BUFCTL                  tRxBuf;       ///< receive buffer control
  BUFCTL                  tTxBuf;       ///< transmit buffer control
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE   sizeof( LCLCTL )

// local parameter declarations -----------------------------------------------
static  UARTDEVENUM aePhyToDefMap[ UART_CHAN_MAX ];
static  LCLCTL      atUartLclCtls[ UART_DEV_ENUM_MAX ];

volatile static struct uart_event* uartEvt;

#if ( UART_DMA_ASYNC_IN_USE == ON)
static PUARTDoubleBuffer pRxBuffs = &sRxData;
#endif

// local function prototypes --------------------------------------------------
static  U16   ComputeBaudrate( PUARTDEF ptDef );
static  void  IrqCommonHandler( UARTDEVENUM eDev );

/// Specific to Zephyr API-style UART callback handler
static  void  ZephyrUartCallback(const struct device *unused, struct uart_event *evt, void *userData);

K_SEM_DEFINE(tx_done, 0, 1);
K_SEM_DEFINE(tx_aborted, 0, 1);

/******************************************************************************
 * @function Uart_Initialize
 *
 * @brief UART initialization
 *
 * This function will initialize UART module. Rx will be disabled on initialization,
 * and only enabled when it's needed. Hopefully after all initialization.
 *
 * @return  TRUE if errors detected, FALSE otherwise
 *
 *****************************************************************************/
BOOL Uart_Initialize( void )
{
  PUARTDEF                    ptDef;
  PLCLCTL                     ptCtl;
  U16                         wBaudRate;
  UARTDEVENUM                 eDev;
  S32                         lRet;
  struct uart_config          uartCfg;
  BOOL                        bRet = FALSE;


  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atUartLclCtls[ eDev ];

    // clear the receive/transmit buffers
    ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
    ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;

    ptCtl->ptUart = device_get_binding( ptDef->pcName );

    uartCfg.baudrate  = ptDef->uBaudRate;
    uartCfg.parity    = UART_CFG_PARITY_NONE;
    uartCfg.stop_bits = UART_CFG_STOP_BITS_1;
    uartCfg.data_bits = UART_CFG_DATA_BITS_8;
    uartCfg.flow_ctrl = UART_CFG_FLOW_CTRL_RTS_CTS;

    if (ptCtl->ptUart != NULL)
    {
      lRet = uart_configure(ptCtl->ptUart, &uartCfg);
      if (lRet != 0)
      {
        bRet = TRUE;
      }

      // Pass the UART dev enum into the *user_data field
      uart_callback_set(ptCtl->ptUart, ZephyrUartCallback, &eDev);

      #if ( UART_DMA_ASYNC_IN_USE == ON )
      memset(sRxData.pBufA, 0, sRxData.wBufLen);
      memset(sRxData.pBufB, 0, sRxData.wBufLen);

      sRxData.pActiveBuf = sRxData.pBufA;
      sRxData.pNextBuf = sRxData.pBufB;
      
      // uart_rx_enable(ptCtl->ptUart, sRxData.pActiveBuf, 
      //                sRxData.wBufLen, UART_RX_TIMEOUT_MS);
      #else
      // uart_rx_enable(ptCtl->ptUart, ptDef->pnRxBuffer, 
      //                ptDef->wRxBufSize, UART_RX_TIMEOUT_MS);
      
      #endif


    }
  }

  return( bRet );
}

/******************************************************************************
 * @function Uart_CloseAll
 *
 * @brief UART close all devices
 *
 * This function will close all defined devices
 *
 *****************************************************************************/
void Uart_CloseAll( void )
{
  PUARTDEF                    ptDef;
  UARTDEVENUM                 eDev;
  PLCLCTL                     ptCtl;
  
  // for each defined UART
  for ( eDev = 0; eDev < UART_DEV_ENUM_MAX; eDev++ )
  {
    // get pointer to the definition/control structures
    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atUartLclCtls[ eDev ];

    // First, ensure that any ongoign transmissions are stopped
    uart_tx_abort(ptCtl->ptUart);

    // Now disable receiving
    uart_rx_disable(ptCtl->ptUart);

  }

  return;
}  // Uart_CloseAll()

/******************************************************************************
 * @function Uart_Write
 *
 * @brief write some characters to the UART
 *
 * This function will write some characters to the UART buffer if room
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesWritten  the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Write( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  UARTERR             eError = UART_ERR_NONE;
  PUARTDEF            ptDef;
  PLCLCTL             ptCtl;
  U16                 wBytesRemaining;
  U8                  sendAmount = 0; 
  S32                 lWriteRet = 0;
 
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atUartLclCtls[ eDev ];
    
    ptCtl->tTxBuf.pnTxBuf = pnData;
    wBytesRemaining = wLength; 
    ptCtl->tTxBuf.wCount = 0; // We're going to use this to count the sent bytes
    ptCtl->tTxBuf.wWrIdx = 0;

    sendAmount = ((wLength > UART_TX_LIMIT_BYTES) ? UART_TX_LIMIT_BYTES : wLength);

    // Do a blocking while loop her to frce out all the data
    while (ptCtl->tTxBuf.wCount < wLength)
    {     
      

      lWriteRet = uart_tx( ptCtl->ptUart, ptCtl->tTxBuf.pnTxBuf + ptCtl->tTxBuf.wWrIdx
                          , sendAmount, UART_TX_TIMEOUT_MS * 1000);
    
      if (lWriteRet == -EBUSY)
      {
        if(k_sem_take(&tx_done, K_MSEC(UART_TX_TIMEOUT_MS)) != 0) 
        {
          uart_tx_abort(ptCtl->ptUart);
          k_sem_take(&tx_done, K_MSEC(UART_TX_TIMEOUT_MS));
        }
      }

      wBytesRemaining = wLength - ptCtl->tTxBuf.wCount;
      sendAmount = ((wBytesRemaining > UART_TX_LIMIT_BYTES) ? UART_TX_LIMIT_BYTES : wBytesRemaining);
    }

    // Even on an abort the count will be updated so this is valid 
    *pwBytesWritten += ptCtl->tTxBuf.wCount;

  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Read
 *
 * @brief read some characters from the UART
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   pnData      pointer to the data buffer
 * @param[in]   wLength     size of the data buffer
 * @param[i0]   pwBytesRead the storage to return the number of bytes written
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Read( UARTDEVENUM eDev, PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  UARTERR   eError = UART_ERR_NONE;
  PUARTDEF  ptDef;
  PLCLCTL   ptCtl;
  U16       wBytesAvailable, wIdx;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
    // get pointers to the control/def structure
    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atUartLclCtls[ eDev ];
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Ioctl
 *
 * @brief UART IOCTL functions
 *
 * This function provides functionality to modify the UARTs parameters
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   eAction     action to take
 * @param[io]   pvData      pointer to data storage/retrieval
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Ioctl( UARTDEVENUM eDev, UARTACTION eAction, PVOID pvData )
{
  UARTERR eError = UART_ERR_NONE;
  
  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Close
 *
 * @brief Close the UART
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Close( UARTDEVENUM eDev )
{
  UARTERR       eError = UART_ERR_NONE;
  PUARTDEF                    ptDef;
  PLCLCTL                     ptCtl;

  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {

    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atUartLclCtls[ eDev ]; 

    // First, ensure that any ongoign transmissions are stopped
    uart_tx_abort(ptCtl->ptUart);

    // Now disable receiving
    uart_rx_disable(ptCtl->ptUart);
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_Open
 *
 * @brief Close the UART
 *
 * This function will disable any interrupt, unregister the interrupt handler
 *
 * @param[in]   eDev        Device enumeration
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_Open( UARTDEVENUM eDev )
{
  UARTERR       eError = UART_ERR_NONE;
  PUARTDEF                    ptDef;
  PLCLCTL                     ptCtl;

  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {

    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atUartLclCtls[ eDev ]; 

    #if ( UART_DMA_ASYNC_IN_USE == ON )
      memset(sRxData.pBufA, 0, sRxData.wBufLen);
      memset(sRxData.pBufB, 0, sRxData.wBufLen);

      sRxData.pActiveBuf = sRxData.pBufA;
      sRxData.pNextBuf = sRxData.pBufB;
      
      uart_rx_enable(ptCtl->ptUart, sRxData.pActiveBuf, 
                     sRxData.wBufLen, UART_RX_TIMEOUT_MS);
    #else
      uart_rx_enable(ptCtl->ptUart, ptDef->pnRxBuffer, 
                     ptDef->wRxBufSize, UART_RX_TIMEOUT_MS);
      
    #endif
  }
  else
  {
    // illegal device
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}  // Uart_Open()

/******************************************************************************
 * @function Uart_ChangeBaud
 *
 * @brief Change Uart baud rate
 *
 * @param[in]   eDev        Device enumeration
 * @param[in]   ubaud       New device baud rate
 *
 * @return      appropriate error value
 *
 *****************************************************************************/
UARTERR Uart_ChangeBaud( UARTDEVENUM eDev, U32 uBaud )
{
  UARTERR eErr = UART_ERR_NONE;
  PUARTDEF                    ptDef;
  PLCLCTL                     ptCtl;
  struct uart_config          uartCfg;
  S32                         lRet;

  // check for a valid UART
  if ( eDev < UART_DEV_ENUM_MAX )
  {

    ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
    ptCtl = &atUartLclCtls[ eDev ]; 

    uartCfg.baudrate  = uBaud;
    uartCfg.parity    = UART_CFG_PARITY_NONE;
    uartCfg.stop_bits = UART_CFG_STOP_BITS_1;
    uartCfg.data_bits = UART_CFG_DATA_BITS_8;
    uartCfg.flow_ctrl = UART_CFG_FLOW_CTRL_RTS_CTS;

    lRet = uart_configure(ptCtl->ptUart, &uartCfg);
    if (lRet != 0)
    {
      eErr = UART_ERR_ILLDEV;
      printk("Failed to change baud rate - %d\n", lRet);
    }

    // Pass the UART dev enum into the *user_data field
    uart_callback_set(ptCtl->ptUart, ZephyrUartCallback, &eDev);

    #if ( UART_DMA_ASYNC_IN_USE == ON )
      memset(sRxData.pBufA, 0, sRxData.wBufLen);
      memset(sRxData.pBufB, 0, sRxData.wBufLen);

      sRxData.pActiveBuf = sRxData.pBufA;
      sRxData.pNextBuf = sRxData.pBufB;
    #endif

  }
  else
  {
    // illegal device
    eErr = UART_ERR_ILLDEV;
  }

  return eErr;
}  // Uart_ChangeBaud

/******************************************************************************
 * @function ZephyrUartCallback
 *
 * @brief UARTE-DMA callback handler - Zephyr-specific
 * 
 * @param [in]  unused    - pointer to the Zephyr UART device instance
 * @param [in]  evt       - pointer to uart_event struct
 * @param [in]  userData  - pointer to UART ENUM
 *
 * Callback handler to handle UART events. Specific to Zephyr-style API
 *
 *****************************************************************************/
static void ZephyrUartCallback(const struct device *unused, 
                           struct uart_event *evt, void *userData)
{
  PUARTDEF      ptDef;
  PLCLCTL       ptCtl;
  
  uartEvt = evt;
  
  // get pointers to the control/def structure
  ptDef = ( PUARTDEF )&g_atUartDefs[ UART_DEV_ENUM_DOCKSER ];
  ptCtl = &atUartLclCtls[ UART_DEV_ENUM_DOCKSER ];
  
  switch (evt->type)
  {
    case UART_RX_RDY:
      ptDef->pvCallback( (PVOID)uartEvt );       
      break;
    case UART_TX_DONE:
      ptCtl->tTxBuf.wCount += evt->data.tx.len;
      ptCtl->tTxBuf.wWrIdx += evt->data.tx.len;
      k_sem_give(&tx_done);
      break;
    case UART_TX_ABORTED:
      ptCtl->tTxBuf.wCount += evt->data.tx.len;
      k_sem_give(&tx_aborted);
    case UART_RX_BUF_REQUEST:

      #if ( UART_DMA_ASYNC_IN_USE == ON )
        uart_rx_buf_rsp(ptCtl->ptUart, sRxData.pNextBuf, sRxData.wBufLen);
      #else 
      // Ensure that the receive buf pointer is back to the start of the buffer
      uart_rx_buf_rsp(ptCtl->ptUart, ptDef->pnRxBuffer, ptDef->wRxBufSize);
      #endif
      
      break;
    case UART_RX_BUF_RELEASED:

      #if ( UART_DMA_ASYNC_IN_USE == ON )
        sRxData.pNextBuf = evt->data.rx_buf.buf;
      #endif
      
      break;

    case UART_RX_DISABLED:
    case UART_RX_STOPPED:
    default:
      break;
  }

  return;
}  // ZephyrUartCallback()

#ifdef SERCOM0
#if ( UART_CHAN0_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM0_Handler
 *
 * @brief interrupt SERCOM 0 handler
 *
 * This function handles the interrupts from SERCOM 0
 *
 *****************************************************************************/
void SERCOM0_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_0 ] );
}
#endif // UART_CHAN0_IN_USE
#endif // SERCOM0

#ifdef SERCOM1
#if ( UART_CHAN1_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM1_Handler
 *
 * @brief interrupt SERCOM 1 handler
 *
 * This function handles the interrupts from SERCOM 1
 *
 *****************************************************************************/
void SERCOM1_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_1 ] );
}
#endif // UART_CHAN1_IN_USE
#endif // SERCOM1

#ifdef SERCOM2
#if ( UART_CHAN2_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM2_Handler
 *
 * @brief interrupt SERCOM 2 handler
 *
 * This function handles the interrupts from SERCOM 2
 *
 *****************************************************************************/
void SERCOM2_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_2 ] );
}
#endif // UART_CHAN2_IN_USE
#endif // SERCOM2

#ifdef SERCOM3
#if ( UART_CHAN3_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM3_Handler
 *
 * @brief interrupt SERCOM 3 handler
 *
 * This function handles the interrupts from SERCOM 3
 *
 *****************************************************************************/
void SERCOM3_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_3 ] );
}
#endif // UART_CHAN3_IN_USE
#endif // SERCOM3

#ifdef SERCOM4
#if ( UART_CHAN4_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM4_Handler
 *
 * @brief interrupt SERCOM 4 handler
 *
 * This function handles the interrupts from SERCOM 4
 *
 *****************************************************************************/
void SERCOM4_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_4 ] );
}
#endif // UART_CHAN4_IN_USE
#endif // SERCOM4

#ifdef SERCOM5
#if ( UART_CHAN5_IN_USE == 1 )
/******************************************************************************
 * @function SERCOM5_Handler
 *
 * @brief interrupt SERCOM 5 handler
 *
 * This function handles the interrupts from SERCOM 5
 *
 *****************************************************************************/
void SERCOM5_Handler( void )
{
  // call the common handler
  IrqCommonHandler( aePhyToDefMap[ UART_CHAN_5 ] );
}
#endif // UART_CHAN5_IN_USE
#endif // SERCOM5

/******************************************************************************
 * @function IrqCommandHandler
 *
 * @brief common interrupt handler
 *
 * This function checks for the type of interrupts and processes them
 * appropriately
 *
 * @param[in]   eDev        Device enumeration
 *
 *****************************************************************************/
static void IrqCommonHandler( UARTDEVENUM eDev )
{
  PUARTDEF                  ptDef;
  PLCLCTL                   ptCtl;
  U8                        nOption;
  PVUARTINTCALLBACK         pvCallBack;
  U32                       uIntFlags;
  UARTIRQEVENTS             eEvent = UART_IRQ_EVENT_NONE;
  
  // get pointer to the definition/control structures
  ptDef = ( PUARTDEF )&g_atUartDefs[ eDev ];
  ptCtl = &atUartLclCtls[ eDev ];
  
  // get the callback
  pvCallBack = ptDef->pvCallback;
  
}

/******************************************************************************
 * @function ComputeBaudRate
 *
 * @brief computes the step and divisor for a given baudrate
 *
 * This function will determine the best step and divisor for a given
 * baud rate
 *
 * @param[in]   uBaudrate   desired baudrate
 *
 * @return      baud rate value, 0 if not available
 *
 *****************************************************************************/
static U16 ComputeBaudrate( PUARTDEF ptDef )
{
  U32   uPeripheralClock, uBaudIntermediate;
  U8    nSampleRate, nFracIdx;
  BOOL  bFractional;
  U64   hScale, hRatio;
  U16   wBaudRate = 0;
  
  // get the sample rate
  switch( ptDef->eSamp )
  {
    case UART_SAMP_16XARITH :
      nSampleRate = 16;
      bFractional = FALSE;
      break;
      
    case UART_SAMP_16XFRAC :
      nSampleRate = 16;
      bFractional = TRUE;
      break;
      
    case UART_SAMP_8XARITH :
      nSampleRate = 8;
      bFractional = FALSE;
      break;
      
    case UART_SAMP_8XFRAC :
      nSampleRate = 8;
      bFractional = TRUE;
      break;
      
    case UART_SAMP_3XARITH :
      nSampleRate = 3;
      bFractional = TRUE;
      break;
      
    default :
      nSampleRate = 1;
      bFractional = FALSE;
      break;
  }
  
  
  // return the baud rate
  return( wBaudRate );
}


/**@} EOF Uart.c */
