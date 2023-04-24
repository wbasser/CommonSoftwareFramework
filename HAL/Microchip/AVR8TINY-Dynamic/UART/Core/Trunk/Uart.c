/******************************************************************************
 * @file Uart.c
 *
 * @brief Uart implementation 
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "UART/Uart.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"
#include "Clock/Clock.h"

// Macros and Defines ---------------------------------------------------------
/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( PLCLCTL )handle )

// define the masks
#define RXCIE_MASK    BIT( 7 )
#define TXCIE_MASK    BIT( 6 )
#define UDRE_MASK     BIT( 5 )
#define RXEN_MASK     BIT( 4 )
#define TXEN_MASK     BIT( 3 )
#define FRMERR_MASK   BIT( 4 )
#define PARERR_MASK   BIT( 2 )
#define TXC_MASK      BIT( 6 )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
/// define the buffer control structure
typedef struct _BUFCTL
{
  U16 wWrIdx;     ///< write index
  U16 wRdIdx;     ///< read index
  U16 wCount;     ///< buffer count
  U16 wSize;      ///< buffer size
  PU8 pnBuffer;   ///< pointer to the buffer
} BUFCTL, *PBUFCTL;
#define BUFCTL_SIZE sizeof( BUFCTL )

/// define the local control structure
typedef struct _LCLCTL
{
  struct _LCLCTL*   ptSignature;  ///< signature
  PU8               pnUDR;      ///< data register
  PU8               pnUCSRA;    ///< control/status register A
  PU8               pnUCSRB;    ///< control/status register B
  PU8               pnUCSRC;    ///< control/status register C
  PU8               pnUCSRD;    ///< control/status register D
  PU16              pwUBRR;     ///< baud rate register
  #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
    BUFCTL            tRxBuf;     ///< receive buffer control
  #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_MINIMAL
  BUFCTL            tTxBuf;     ///< transmit buffer control
  PVUARTIRQCALLBACK pvCallback; ///< pointer to the callback
} LCLCTL, *PLCLCTL;
#define LCLCTL_SIZE sizeof( LCLCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  PLCLCTL     aptLclCtls[ UART_CHAN_MAX ];

// local function prototypes --------------------------------------------------
static  void  IrqCommonHandler( UARTCHAN eChan, UARTIRQEVENTS eEvent );
static  void  GetRegisters( UARTCHAN eChan, PLCLCTL ptCtl );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Uart_Configure
 *
 * @brief create a UART port
 *
 * This function will initialize the UART to its defined operating
 * characteristics
 *
 * @param[in]   ptDef       pointer to the definition structure
 *
 * @return      pointer to the control or NULL
 *
 *****************************************************************************/
PTUARTHANDLE Uart_Configure( UARTDEF* ptDef )
{
  U16         wDivisor;
  U8          nTemp;
  PLCLCTL     ptCtl;
  
  // create the control entry
  if (( ptCtl = malloc( LCLCTL_SIZE )) != NULL )
  {
    // store the signature
    ptCtl->ptSignature = ptCtl;

    // cratre the transmit buffer
    if (( ptCtl->tTxBuf.pnBuffer = malloc( ptDef->wTxBufSize )) != NULL )
    {
      #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
        // create the receive buffer
        if (( ptCtl->tRxBuf.pnBuffer = malloc( ptDef->wRxBufSize )) != NULL )
        {
      #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_MINIMAL
          // we're good - perform the rest of the allocation
          GetRegisters( ptDef->eChan, ptCtl );
          
          // calculate the divisor/set the baud rate
          wDivisor = ( U16 )(( Clock_GetFreq( ) / ( 16 * ptDef->uBaudRate )) - 1 );
          *( ptCtl->pwUBRR ) = wDivisor;
          
          // get the UCSRC control value
          nTemp = BITS( ptDef->eMode, 6 );
          nTemp |= BITS( ptDef->eParity, 4 );
          nTemp |= BITS( ptDef->eStopBits, 3 );
          nTemp |= BITS(( ptDef->eWordLen & 0x03 ), 1 );
          *( ptCtl->pnUCSRC ) = nTemp;

          // set up the control registers
          // now enable the transmitter, receiver and receiver interrupts/
          *( ptCtl->pnUCSRB ) |= ( RXCIE_MASK | RXEN_MASK | TXEN_MASK );
          
          // clear the indices
          ptCtl->tTxBuf.wWrIdx = ptCtl->tTxBuf.wRdIdx = ptCtl->tTxBuf.wCount = 0;
          
          // store the callback
          ptCtl->pvCallback = ptDef->pvCallback;
          
          // store the buffer sizes
          ptCtl->tTxBuf.wSize = ptDef->wTxBufSize;
      #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
          ptCtl->tRxBuf.wWrIdx = ptCtl->tRxBuf.wRdIdx = ptCtl->tRxBuf.wCount = 0;
          ptCtl->tRxBuf.wSize = ptDef->wRxBufSize;
        }
        else
        {
          // unallocate the tx buffer
          free( ptCtl->tTxBuf.pnBuffer );
          free( ptCtl );
          
          // set to null
          ptCtl = NULL;
        }
      #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_MINIMAL
    }
    else
    {
      // free the control pointer
      free( ptCtl );
      ptCtl = NULL;
    }
  }
  
  // return the pointer to the control
  return(( PTUARTHANDLE )ptCtl );
}

#if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
/******************************************************************************
 * @function Uart_Read
 *
 * @brief read bytes from the UART
 *
 * This function will read bytes from the UART buffers
 *
 * @param[in]   ptHandle    device
 * @param[in]   pnData      pointer to the data storage
 * @param[in]   nLength     length of the data storage, number of bytes to read
 * @param[io]   pnBytesRead set to the number of actual bytes read
 *
 * @return      appropriate error
 *
 *****************************************************************************/
UARTERR Uart_Read( PTUARTHANDLE ptHandle, PU8 pnData, U16 wLength, PU16 pwBytesRead )
{
  UARTERR eError = UART_ERR_NONE;
  PLCLCTL ptCtl;
  U16     wIdx;
  
  // map handle to pointer
  ptCtl = MAP_HANDLE_TO_POINTER( ptHandle );
 
  // check for a valid UART
  if ( ptCtl->ptSignature == ptCtl )
  {
    // compute the number of bytes to read
    *pwBytesRead = MIN( ptCtl->tRxBuf.wCount, wLength );
    
    // disalbe interrupt
    Interrupt_Disable( );
    
    // copy the buffer
    for ( wIdx = 0; wIdx < *pwBytesRead; wIdx++ )
    {
      // stuff in buffer, adjust pointer/increemnt count
      *( pnData + wIdx ) = *( ptCtl->tRxBuf.pnBuffer + ptCtl->tRxBuf.wRdIdx++ );
      ptCtl->tRxBuf.wRdIdx %= ptCtl->tRxBuf.wSize;
      ptCtl->tRxBuf.wCount--;
    }
    
    // re-enable interrupts
    Interrupt_Enable( );
  }
  else
  {
    // set the error
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}
#endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_MINIMAL

/******************************************************************************
 * @function Uart_Write
 *
 * @brief write data to the UART
 *
 * This function 
 *
 * @param[in]   ptHandle    device
 * @param[in]   pnData      pointer to the data storage
 * @param[in]   wLength     length of the data storage, number of bytes to write
 * @param[io]   pwBytesRead set to the number of actual bytes written
 *
 * @return      appropriate error
 *
 *****************************************************************************/
UARTERR Uart_Write( PTUARTHANDLE ptHandle, PU8 pnData, U16 wLength, PU16 pwBytesWritten )
{
  UARTERR eError = UART_ERR_NONE;
  PLCLCTL ptCtl;
  U16     wBytesRemaining, wIdx;
  
  // map handle to pointer
  ptCtl = MAP_HANDLE_TO_POINTER( ptHandle );
 
  // check for a valid UART
  if ( ptCtl->ptSignature == ptCtl )
  {
    // compute the bytes written
    wBytesRemaining = ptCtl->tTxBuf.wSize - ptCtl->tTxBuf.wCount;
    *pwBytesWritten = MIN( wBytesRemaining, wLength );
    
    // disalb interrupt
    Interrupt_Disable( );
    
    // copy the buffer
    for ( wIdx = 0; wIdx < *pwBytesWritten; wIdx++ )
    {
      // stuff in buffer, adjust pointer/increemnt count
      *( ptCtl->tTxBuf.pnBuffer + ptCtl->tTxBuf.wWrIdx++ ) = *( pnData + wIdx );
      ptCtl->tTxBuf.wWrIdx %= ptCtl->tTxBuf.wSize;
      ptCtl->tTxBuf.wCount++;
    }
    
    // re-enable interrupts
    Interrupt_Enable( );
    
    if ( *( ptCtl->pnUCSRA ) & TXC_MASK )
    {
      // clear the mask
      *( ptCtl->pnUCSRA ) |= TXC_MASK;
      *( ptCtl->pnUCSRB ) |= TXCIE_MASK;
    }
    
    // always set the shift register empty
    *( ptCtl->pnUCSRB ) |= UDRE_MASK;
  }
  else
  {
    // set the error
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function Uart_CLose 
 *
 * @brief close the UART channel
 *
 * This function will close the UART channel
 *
 * @param[in]   ptHandle    device
 *
 * @return      appropriate error
 *
 *****************************************************************************/
UARTERR Uart_Close( PTUARTHANDLE ptHandle )
{
  UARTERR eError = UART_ERR_NONE;
  PLCLCTL ptCtl;
  
  // map handle to pointer
  ptCtl = MAP_HANDLE_TO_POINTER( ptHandle );
 
  // check for a valid UART
  if ( ptCtl->ptSignature == ptCtl )
  {
    // close the UART
    ptCtl->pnUCSRA = 0;
    ptCtl->pnUCSRB = 0;
    ptCtl->pnUCSRC = 0;
    ptCtl->pnUCSRD = 0;
  }
  else
  {
    // error
    eError = UART_ERR_ILLDEV;
  }
  
  // return the error
  return( eError );
}

#if defined ( UDR0 )
/******************************************************************************
 * @function USART0_RX_VECT
 *
 * @brief USART 0 RX vector
 *
 * This function handles the recieved character interrupt
 *
 *****************************************************************************/
ISR( USART0_RX_vect )
{
  // call the common handler
  IrqCommonHandler( UART_CHAN_0, UART_IRQ_EVENT_RXCHAR );
}
 
/******************************************************************************
 * @function USART0_UDRE_VECT
 *
 * @brief USART 0 UDRE vector
 *
 * This function handles the transmit shift register empty interrupt
 *
 *****************************************************************************/
ISR( USART0_UDRE_vect )
{
  // call the common handler
  IrqCommonHandler( UART_CHAN_0, UART_IRQ_EVENT_TXEMP );
}

/******************************************************************************
 * @function USART0_TX_VECT
 *
 * @brief USART 0 TX vector
 *
 * This function handles the transmit complete interrupt
 *
 *****************************************************************************/
ISR( USART0_TX_vect )
{
  // call the common handler
  IrqCommonHandler( UART_CHAN_0, UART_IRQ_EVENT_TXCMP );
}
#endif // UDDR0

#if defined ( UDR1 )
/******************************************************************************
 * @function USART1_RX_VECT
 *
 * @brief USART 1 RX vector
 *
 * This function handles the recieved character interrupt
 *
 *****************************************************************************/
ISR( USART1_RX_vect )
{
  // call the common handler
  IrqCommonHandler( UART_CHAN_1, UART_IRQ_EVENT_RXCHAR );
}
 
/******************************************************************************
 * @function USART1_UDRE_VECT
 *
 * @brief USART 1 UDRE vector
 *
 * This function handles the transmit shift register empty interrupt
 *
 *****************************************************************************/
ISR( USART1_UDRE_vect )
{
  // call the common handler
  IrqCommonHandler( UART_CHAN_1, UART_IRQ_EVENT_TXEMP );
}

/******************************************************************************
 * @function USART1_TX_VECT
 *
 * @brief USART 1 TX vector
 *
 * This function handles the transmit complete interrupt
 *
 *****************************************************************************/
ISR( USART1_TX_vect )
{
  // call the common handler
  IrqCommonHandler( UART_CHAN_1, UART_IRQ_EVENT_TXCMP );
}
#endif // UDDR1


/******************************************************************************
 * @function IrqCommonHandler
 *
 * @brief common interrupt handler
 *
 * This function handles the interrupts
 *
 * @param[in] ePhy    physical device
 * @param[in] eEvent  interrupt event
 *
 *****************************************************************************/
static void IrqCommonHandler( UARTCHAN eChan, UARTIRQEVENTS eEvent )
{
  PLCLCTL     ptCtl;
  U8          nChar, nSts;
  
  // get the definition/control
  ptCtl = aptLclCtls[ eChan ];
  
  // process the event
  switch( eEvent )
  {
    case UART_IRQ_EVENT_RXCHAR :
      // get the status and data
      nSts = *( ptCtl->pnUCSRA );
      nChar = *( ptCtl->pnUDR );
      
      // check for break detect
      if ( nSts & FRMERR_MASK )
      {
        // set the break detect event
        eEvent = UART_IRQ_EVENT_BRKDET;
      }
      else if ( nSts & PARERR_MASK )
      {
        // set the error event
        eEvent = UART_IRQ_EVENT_RXERR;
      }
      else
      {
        // if callback is not null
        if ( ptCtl->pvCallback == NULL )
        {
          #if (SYSTEMDEFINE_OS_SELECTION != SYSTEMDEFINE_OS_MINIMAL )
            // process the character
            if ( ptCtl->tRxBuf.wCount < ptCtl->tRxBuf.wSize )
            {
              // stuff the char
              *( ptCtl->tRxBuf.pnBuffer + ptCtl->tRxBuf.wWrIdx++ ) = nChar;
              ptCtl->tRxBuf.wWrIdx %= ptCtl->tRxBuf.wSize;
              ptCtl->tRxBuf.wCount++;
            }
          #endif // SYSTEMDEFINE_OS_SELECTON != SYSTEMDEFINE_OS_MINIMAL
        }
      }
      break;
      
    case UART_IRQ_EVENT_TXEMP :
      // check for characters available
      if ( ptCtl->tTxBuf.wCount != 0 )
      {
        // output the character
        nChar = *( ptCtl->tTxBuf.pnBuffer + ptCtl->tTxBuf.wRdIdx++ );
        *( ptCtl->pnUDR ) = nChar;
        ptCtl->tTxBuf.wRdIdx %= ptCtl->tTxBuf.wSize;
        ptCtl->tTxBuf.wCount--;
      }
      else
      {
        // turn off the interrupts
        *( ptCtl->pnUCSRB ) &= ~UDRE_MASK;
      }
      
      // clear the option
      nChar = 0;
      break;
      
    case UART_IRQ_EVENT_TXCMP :
      // clear the interrupt
      if ( ptCtl->tTxBuf.wCount == 0 )
      {
        // just clear the interrupt
        *( ptCtl->pnUCSRB ) &= ~TXCIE_MASK;
      }
      
      // clear the option
      nChar = 0;
      break;
      
    default :
      // clear the option
      nChar = 0;
      break;
  }
  
  // check for a valid callback
  if ( ptCtl->pvCallback != NULL )
  {
    // call it
    ptCtl->pvCallback( eEvent, nChar );
  }
}

static void GetRegisters( UARTCHAN eChan, PLCLCTL ptCtl )
{
  // set the registers
  switch( eChan )
  {
    case UART_CHAN_0 :
      #ifdef UDR0
        // set the registers
        ptCtl->pnUDR = ( PU8 )&UDR0;
        ptCtl->pnUCSRA = ( PU8 )&UCSR0A;
        ptCtl->pnUCSRB = ( PU8 )&UCSR0B;
        ptCtl->pnUCSRC = ( PU8 )&UCSR0C;
        #ifdef USCR1D
          ptCtl->pnUCSRD = ( PU8 )&UCSR0D;
        #endif
        ptCtl->pwUBRR = ( PU16 )&UBRR0;
      #endif
      break;
      
    case UART_CHAN_1 :
      #ifdef UDR1
        // set the registers
        ptCtl->pnUDR = ( PU8 )&UDR1;
        ptCtl->pnUCSRA = ( PU8 )&UCSR1A;
        ptCtl->pnUCSRB = ( PU8 )&UCSR1B;
        ptCtl->pnUCSRC = ( PU8 )&UCSR1C;
        #ifdef USCR1D
          ptCtl->pnUCSRD = ( PU8 )&UCSR1D;
        #endif
        ptCtl->pwUBRR = ( PU16 )&UBRR1;
      #endif
      break;
      
    default :
      break;
  }
  
  // store the pointer 
  aptLclCtls[ eChan ] = ptCtl;
}

/**@} EOF Uart.c */
