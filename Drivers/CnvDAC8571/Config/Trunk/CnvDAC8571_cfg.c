/******************************************************************************
 * @file CnvDAC7581_cfg.c
 *
 * @brief converter DAC7581 configuration implementation
 *
 * This file provides the implementation for the convert DAC7581
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

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "CnvDAC7581/CnvDAC7581_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function CnvDAC7581_WriteRegisters
 *
 * @brief write regiters
 *
 * This function will write the caommand and data to the device
 *
 * @param[in]   nCmd      command to send
 * @param[in]   wValue    output value
 *
 *****************************************************************************/
void CnvDAC7581_WriteRegisters( U8 nCmd, U16 wValue )
{

}

/**@} EOF CnvDAC7581_cfg.c */
