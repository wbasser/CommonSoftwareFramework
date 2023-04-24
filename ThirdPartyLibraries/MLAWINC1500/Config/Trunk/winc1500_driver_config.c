/******************************************************************************
 * @file winc1500_driver_config.c
 *
 * @brief WINC1500 driver config implementation
 *
 * This file provides the confriguration for the driver
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
 * \addtogroup winc1500_driver_config
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "winc1500_driver_config.h"
#include "winc1500_api.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"
#include "SPI/Spi.h"
#include "SystemTick/SystemTick.h"
#include "MLAWINC1500Handler/MLAWINC1500Handler.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function m2m_wifi_handle_event
 *
 * @brief wifi event handler
 *
 * This function will call the abstracted WIFI event handler
 *
 * @param[in]   eEventCode    event codoe
 * @param[in]   ptEventData   event data
 *
 *****************************************************************************/
void m2m_wifi_handle_events( t_m2mWifiEventType eEventCode, t_wifiEventData* ptEventData )
{
  // call the handler
  MLAWINC1500Handler_WifiCallback( eEventCode, ptEventData );
}

/******************************************************************************
 * @function m2m_socket_handle_event
 *
 * @brief socket event handler
 *
 * This function will call the abstracted socket event handler
 *
 * @param[in]   iSock         socket 
 * @param[in]   eEventCode    event codoe
 * @param[in]   ptEventData   event data
 *
 *****************************************************************************/
void m2m_socket_handle_events( SOCKET iSock, t_m2mSocketEventType eEventCode, t_socketEventData* ptEventData )
{
  // call the handler
  MLAWINC1500Handler_SocketCallback( iSock, eEventCode, ptEventData );
}

/******************************************************************************
 * @function m2m_ota_handle_events
 *
 * @brief ota event handler
 *
 * This function will call the abstracted ota event handler
 *
 * @param[in]   eEventCode    event codoe
 * @param[in]   ptEventData   event data
 *
 *****************************************************************************/
void m2m_ota_handle_events( t_m2mOtaEventType eEventCode, t_m2mOtaEventData* ptEventData )
{
}

/******************************************************************************
 * @function m2m_error_handle_events
 *
 * @brief error event handler
 *
 * This function will call the abstracted error event handler
 *
 * @param[in]   uErrorCode    error code
 *
 *****************************************************************************/
void m2m_error_handle_events( uint32_t uErrorCode )
{
  // call the error handler
  MLAWINC1500Handler_ErrorCallback( uErrorCode );
}

/******************************************************************************
* @function m2mStub_PinSet_CE
*
* @brief set the state of the Chip Enable
*
* This function will set the steate of the chip enable
*
* @param[in]   eAction   action of pin
*
*****************************************************************************/
void m2mStub_PinSet_CE( t_m2mWifiPinAction eAction )
{
  // set the CE pin
  Gpio_Set( GPIO_PIN_ENUM_WIFICEN, eAction );
}

/******************************************************************************
 * @function m2mStub_PinSet_RESET
 *
 * @brief set the state of the RESET
 *
 * This function will set the steate of the reset
 *
 * @param[in]   eAction   action of pin
 *
 *****************************************************************************/
void m2mStub_PinSet_RESET( t_m2mWifiPinAction eAction )
{
  // set the reset pin
  Gpio_Set( GPIO_PIN_ENUM_WIFIRST, eAction );
}

/******************************************************************************
 * @function m2mStub_PinSet_SPI_SS
 *
 * @brief set the state of the slave select
 *
 * This function will set the steate of the slave select
 *
 * @param[in]   eAction   action of pin
 *
 *****************************************************************************/
void m2mStub_PinSet_SPI_SS( t_m2mWifiPinAction eAction )
{
  // set the select pin
  Gpio_Set( GPIO_PIN_ENUM_WIFISEL, eAction );
}

/******************************************************************************
 * @function m2mStub_EintDisable
 *
 * @brief disable the interrupt
 *
 * This function will disable the interrup
 *
 *****************************************************************************/
void m2mStub_EintDisable( void )
{
  BOOL  bAction = OFF;

  // disable
  Gpio_Ioctl( GPIO_IRQ_ENUM_WIFIIRQ, GPIO_ACT_ENBDSBIRQ, ( PVOID )&bAction );
}

/******************************************************************************
 * @function m2mStub_EintEnable
 *
 * @brief enable the interrupt
 *
 * This function will enable the interrup
 *
 *****************************************************************************/
void m2mStub_EintEnable( void )
{
  BOOL  bAction = ON;

  // disable
  Gpio_Ioctl( GPIO_IRQ_ENUM_WIFIIRQ, GPIO_ACT_ENBDSBIRQ, ( PVOID )&bAction );
}

/******************************************************************************
 * @function m2mStub_GetOneMsTimer
 *
 * @brief get the current 1 milliseond time value
 *
 * This function will return the urrent count of the 1 millisecond time
 *
 * @return    current value of the timer
 *
 *****************************************************************************/
U32 m2mStub_GetOneMsTimer( void )
{
  // return the current system tick count
  return( SystemTick_GetTimeMsec( ));
}

/******************************************************************************
 * @function m2mStub_SpiTxRx
 *
 * @brief SPI transmit/receive
 *
 * This function will writeand read from the SPI bus
 *
 * @param[in]   pnTxBuf     pointer to the transmit buffer
 * @param[in]   wTxLen      transmit length
 * @param[io]   pnRxBuf     pointer to the receive buffer
 * @param[in]   wRxLen      receive length
 *
 *****************************************************************************/
void m2mStub_SpiTxRx( PU8 pnTxBuf, U16 wTxLen, PU8 pnRxBuf, U16 wRxLen )
{
  U8  nTxData, nRxData;
  U16 wTotalLength;

  // get the total loop length
  wTotalLength = MAX( wTxLen, wRxLen );

  // execute loop
  while( wTotalLength != 0 )
  {
    // get the transmit byte
    if ( wTxLen != 0 )
    {
      // get the data/decrement the length
      nTxData = *( pnTxBuf++ );
      wTxLen--;
    }
    else
    {
      // set to zero
      nTxData = 0;
    }

    // read/write it
    Spi_Read( SPI_DEV_ENUM_WIFI, nTxData, &nRxData );

    // check for receive stuff
    if ( wRxLen != 0 )
    {
      // stuff the receive byte/decrement the length
      *( pnRxBuf++ ) = nRxData;
      wRxLen--;
    }

    // decrement the total length
    wTotalLength--;
  }
}

/**@} EOF winc1500_driver_config.c */
