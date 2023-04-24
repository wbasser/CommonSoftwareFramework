/******************************************************************************
 * @file RadNRF24L_cfg.h
 *
 * @brief Radio NRF24L radio configuraiton definitions 
 *
 * This file provides the configuration definitions for the NRF24L radio
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
#ifndef _RADNRF24L_CFG_H
#define _RADNRF24L_CFG_H

// system includes ------------------------------------------------------------
#include "Types/TYpes.h

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  RadNRF24L_LocalInitialize( void );
extern  BOOL  RadNRF24L_WriteData( U8 nRegister, PU8 pnData, U8 nLength );
extern  BOOL  RadNRF24L_ReadData( U8 nRegister, PU8 pnData, U8 nLength );
extern  BOOL  RadNRF24L_InterruptCallback( U8 nIrq, U8 nEvent, BOOL bState );

/**@} EOF RadNRF24L_cfg.h */

#endif  // _RADNRF24L_CFG_H