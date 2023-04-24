/******************************************************************************
 * @file RadNRF24L.h   
 *
 * @brief Radio NRF24L driver declarations
 *
 * This file provides the declarations for the NRF24 radio
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
 * \addtogroup RadNRF24L
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _RADNRF24L_H
#define _RADNRF24L_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "RadNRF24L/RadNRF24L_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------
/// enumerate the return values for callback
typedef enum _RADRF24LSTS
{
  RADRF24L_STS_OK = 0,
  RADRF24L_STS_RCVDONE,
  RADRF24L_STS_XMTDONE,
  RADRF24L_STS_ERR,
} RADRF24LSTS;

// structures -----------------------------------------------------------------
/// define the callback function
#define RADRRF24LSTS ( PVRADRNF24LCALLBACK )( void );

// global parameter declarations -----------------------------------------------

// global function prototypes ---------------------- ?> vj\lkjcx
----------------------------
extern  BOOL  RadNRF24L_Initialize( void );
extern  U8    RadNRF24L_ReadData( PU8 pnData, U8 nMaxLength, PVRADRNF24LCALLBACK );
extern  U8    RadNRF24L_SendData( U8 nDestination, PU8 pnData, U8 nLength, BOOL bRequireAck, PVRADRNF24LCALLBACK );
extern  U8    RadNRF24L_ScanChannel( U8 nChannel, U8 nNumScans );
extern  U8    RadNRF24L_AddAckData( PU8 pnData, U8 nLength, BOOL bRemoveExistingAcks );

/**@} EOF RadNRF24L.h */

#endif  // _RADNRF24L_H