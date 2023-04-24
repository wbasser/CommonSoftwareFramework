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

// local includes -------------------------------------------------------------
#include "UART/Uart.h"

// library includes -----------------------------------------------------------
#include "Clock/Clock.h"
#include "GPIO/Gpio.h"
#include "PowerManager/PowerManager.h"

// macros and defines ---------------------------------------------------------
/// define the macro to map the control pointer
#define MAP_HANDLE_TO_POINTER( handle )   (( SercomUsart* )handle )

/// define the sample rate
#define UART_SAMPLE_RATE                  ( 16 )

// structures -----------------------------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------
static  SercomUsart*  GetSercomChannel( UARTCHAN eChan );
static  U16           ComputeBaudrate( PUARTDEF ptDef );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Uart_Configure
 *
 * @brief UART configure
 *
 * This function will initialize a given device 
 *
 * @param[in]   ptDef     pointer to the definition structure
 *
 *****************************************************************************/
PVUARTHANDLE Uart_Configure( PUARTDEF ptDef )
{
  U16           wBaudRate;
  SercomUsart*  ptUsart = NULL;
  
  // get the pointer to the USART channel
  ptUsart = GetSercomChannel( ptDef->eChan );

  // now compute the baud rate
  if (( wBaudRate = ComputeBaudrate( ptDef )) != 0 )
  {
    // configure the GPIO pins
    Gpio_Configure( ptDef->eDevPort, ptDef->nTxdPin, GPIO_MODE_OUTPUT_INPDSB, OFF, ptDef->eDevMux, OFF );
    Gpio_Configure( ptDef->eDevPort, ptDef->nRxdPin, GPIO_MODE_INPUT,         OFF, ptDef->eDevMux, OFF );

    // clear the two registers
    ptUsart->CTRLA.reg = 0;
    ptUsart->CTRLB.reg = 0;

    // disable the control adn wait for done
    ptUsart->CTRLA.bit.SWRST = 1;
    while( ptUsart->SYNCBUSY.bit.SWRST );
  
    // valid baud rate
    ptUsart->BAUD.reg = wBaudRate;
    
    // now set the modes
    switch( ptDef->eMode )
    {
      case UART_MODE_ASYNCINT :
        ptUsart->CTRLA.bit.MODE = 1;
        ptUsart->CTRLA.bit.CMODE = 0;
        break;
        
      case UART_MODE_SYNCEXT :
        ptUsart->CTRLA.bit.MODE = 1;
        ptUsart->CTRLA.bit.CMODE = 1;
        break;
        
      case UART_MODE_SYNCINT :
        ptUsart->CTRLA.bit.MODE = 1;
        ptUsart->CTRLA.bit.CMODE = 1;
        break;
        
      case UART_MODE_ASYNCEXT :
      default :
        ptUsart->CTRLA.bit.MODE = 0;
        ptUsart->CTRLA.bit.CMODE = 0;
        break;
    }
    
    // set the run in standby/sample rate/TX pin/RX pin
    ptUsart->CTRLA.bit.RUNSTDBY = ptDef->bRunStandby;
    ptUsart->CTRLA.bit.TXPO = ptDef->eTxdPad;
    ptUsart->CTRLA.bit.RXPO = ptDef->eRxdPad;
    ptUsart->CTRLA.bit.DORD = UART_DATAORDER_LSB;
    
    // set the frame/parity
    ptUsart->CTRLA.bit.FORM = ( ptDef->eParity != UART_PARITY_NONE ) ? 1 : 0;
    
    // set the word length/stop bits/parity/TX enable/RX enable
    switch( ptDef->eWrdLen )
    {
      case UART_WRDLEN_5 : 
        ptUsart->CTRLB.reg |= SERCOM_USART_CTRLB_CHSIZE( 5 );
        break;
        
      case UART_WRDLEN_6 :
        ptUsart->CTRLB.reg |= SERCOM_USART_CTRLB_CHSIZE( 6 );
        break;
        
      case UART_WRDLEN_7 :
        ptUsart->CTRLB.reg |= SERCOM_USART_CTRLB_CHSIZE( 7 );
        break;
        
      case UART_WRDLEN_8 :
        ptUsart->CTRLB.bit.CHSIZE = 0;
        break;
        
      case UART_WRDLEN_9 :
        ptUsart->CTRLB.bit.CHSIZE = 1;
        break;
        
      default :
        ptUsart->CTRLB.bit.CHSIZE = 0;
        break;
    }
    ptUsart->CTRLB.bit.SBMODE = ptDef->eStopBit;
    ptUsart->CTRLB.bit.PMODE = ( ptDef->eParity == UART_PARITY_ODD ) ? ON : 0;
    ptUsart->CTRLB.bit.RXEN = ON;
    ptUsart->CTRLB.bit.TXEN = ON;
    
    // now write CTRLA/CTRLB
    while( ptUsart->SYNCBUSY.bit.CTRLB & SERCOM_USART_SYNCBUSY_CTRLB );

    // now enable the device
    ptUsart->CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
    while( ptUsart->SYNCBUSY.bit.CTRLB & SERCOM_USART_SYNCBUSY_ENABLE );
  }

  // return the handle as a void
  return(( PVUARTHANDLE )ptUsart ); 
}

/******************************************************************************
 * @function Uart_Write
 *
 * @brief write some characters to the UART
 *
 * This function will write some characters to the UART buffer if room
 *
 * @param[in]   eChan       channel
 * @param[in]   nData       data to write
 *
 *****************************************************************************/
void Uart_Write( PVUARTHANDLE pvUart, U8 nData )
{
  SercomUsart* ptUsart;

  // map handle to pointer
  ptUsart = MAP_HANDLE_TO_POINTER( pvUart );

  // wait for done
  while( !ptUsart->INTFLAG.bit.DRE );

  // output the byte
  ptUsart->DATA.reg = nData;
}

/******************************************************************************
 * @function Uart_Read
 *
 * @brief read some characters from the UART
 *
 * This function will read bytes from the buffer
 *
 * @param[in]   eChan       channel
 * @param[in]   pnData      pointer to the data buffer
 *
 * @return    TRUE if characters read, FALSE otherwise
 *
 *****************************************************************************/
BOOL Uart_Read( PVUARTHANDLE pvUart, PU8 pnData )
{
  SercomUsart*  ptUsart;
  BOOL          bStatus;

  // map handle to pointer
  ptUsart = MAP_HANDLE_TO_POINTER( pvUart );

  if (( bStatus = ptUsart->INTFLAG.bit.RXC ) == TRUE )
  {
    *pnData = ptUsart->DATA.reg;
  }

  // return status
  return( bStatus );
}

/******************************************************************************
 * @function Uart_CharsAvailable
 *
 * @brief check to see if characters are available
 *
 * This function will check to see if characters are available
 *
 * @param[in]   eChan       channel
 *
 * @return      TRUE if characters are avialable
 *
 *****************************************************************************/
BOOL Uart_CharsAvailable( PVUARTHANDLE pvUart )
{
  SercomUsart* ptUsart;

  // map handle to pointer
  ptUsart = MAP_HANDLE_TO_POINTER( pvUart );

  // check if there are characters are available
  return( ptUsart->INTFLAG.bit.RXC );
}

/******************************************************************************
 * @function Uart_Close
 *
 * @brief close the uart
 *
 * This function will close the uart
 *
 * @param[in]   eChan       channel
 *
 *****************************************************************************/
void Uart_Close( PVUARTHANDLE pvUart )
{
  SercomUsart* ptUsart;

  // map handle to pointer
  ptUsart = MAP_HANDLE_TO_POINTER( pvUart );

  // close the UART
  ptUsart->CTRLB.reg = 0;
  ptUsart->CTRLA.reg = 0;
  while( ptUsart->SYNCBUSY.bit.CTRLB & SERCOM_USART_SYNCBUSY_ENABLE );
}

/******************************************************************************
 * @function GetSercomChannel
 *
 * @brief gets a pointer to the control structure
 *
 * This function will return a pointer to the correct control structure
 * for a given channel
 *
 * @param[in]   eChan			SERCOM channel
 *
 * @return      pointer to a SercomUsart or NULL if illegal channel
 *
 *****************************************************************************/
static SercomUsart* GetSercomChannel( UARTCHAN eChan )
{
  Sercom*     ptSercom = NULL;
  CLOCKMUXID  eClockId;
  U32         uPeriphId;
  
  switch( eChan )
  {
    case UART_CHAN_0 :
      ptSercom = SERCOM0;
      eClockId = CLOCK_MUXID_SERCOM_0;
      uPeriphId = PM_APBCMASK_SERCOM0;
      break;
      
    case UART_CHAN_1 :
      ptSercom = SERCOM1;
      eClockId = CLOCK_MUXID_SERCOM_1;
      uPeriphId = PM_APBCMASK_SERCOM1;
      break;
    
    #ifdef SERCOM2
    case UART_CHAN_2:
      ptSercom = SERCOM2;
      eClockId = CLOCK_MUXID_SERCOM_2;
      uPeriphId = PM_APBCMASK_SERCOM2;
      break;
    #endif // SERCOM2
    
    #ifdef SERCOM3
    case UART_CHAN_3 :
      ptSercom = SERCOM3;
      eClockId = CLOCK_MUXID_SERCOM_3;
      uPeriphId = PM_APBCMASK_SERCOM3;
      break;
    #endif // SERCOM3
    
    #ifdef SERCOM4
    case UART_CHAN_4 :
      ptSercom = SERCOM4;
      eClockId = CLOCK_MUXID_SERCOM_4;
      uPeriphId = PM_APBCMASK_SERCOM4;
      break;
    #endif // SERCOM4
    
    #ifdef SERCOM5
    case UART_CHAN_5 :
      ptSercom = SERCOM5;
      eClockId = CLOCK_MUXID_SERCOM_5;
      uPeriphId = PM_APBCMASK_SERCOM5;
      break;
    #endif // SERCOM5
    
    default :
      eClockId = CLOCK_MUXID_SERCOM_0;
      uPeriphId = PM_APBCMASK_SERCOM0;
      break;
  }
  
  // now enable the clock and power mask
  if ( ptSercom != NULL )
  {
    // only initialize if valid SERCOM
    Clock_PeriphEnable( eClockId, CLOCK_GENID_0 );
    PowerManager_DisableEnablePeriphC( uPeriphId, ON );
  }

  // return the pointer to the channlel
  return( &ptSercom->USART );
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
  U32   uPeripheralClock;
  U16   wBaudRate = 0;
  
  // get the system clock value
  uPeripheralClock = Clock_GetGenClock( CLOCK_GENID_0 );
  
  // check for baud rate outside of normal range
  if (( ptDef->uBaudrate * UART_SAMPLE_RATE ) <= uPeripheralClock )
  {
    // calculate the baud rate
    wBaudRate = 65536 - (( U64 ) 65536 * UART_SAMPLE_RATE * ptDef->uBaudrate ) / uPeripheralClock;
  }
  
  // return the baud rate
  return( wBaudRate );
}

/**@} EOF Uart.c */
