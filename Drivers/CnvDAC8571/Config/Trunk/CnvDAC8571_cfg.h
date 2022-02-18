/******************************************************************************
 * @file CnvDAC7581_cfg.h
 *
 * @brief  converter DAC7581 configuration declarations
 *
 * This file provides the configuration declarations for the convert DAC7581
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
 * \addtogroup CnvDAC7581
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _CNVDAC7581_CFG_H
#define _CNVDAC7581_CFG_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------
/// define the full scale voltage
#define CNVDAC7581_FULLSCALE_VOLTS              ( 5.0f )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void  CnvDAC7581_WriteRegisters( U8 nCmd, U16 wValue );

/**@} EOF CnvDAC7581_cfg.h */

#endif  // _CNVDAC7581_CFG_H