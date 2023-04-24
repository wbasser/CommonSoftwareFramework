/******************************************************************************
 * @file Dsu.c
 *
 * @brief Device Service Unit (DSU) implementaion
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
 * $Rev: $
 * 
 *
 * \addtogroup Dsu
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DSU/Dsu.h"

// library includes -----------------------------------------------------------
#include "Interrupt/Interrupt.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function Dsu_CalculateCRC
 *
 * @brief calculate a CRC
 *
 * This function will calculate the CRC of a given range
 *
 * @param[in]   uStartAddress     start address
 * @param[in]   uLength           length of block
 * @param[io]   puCrc             pointer to store the CRC value
 *
 * @return      TRUE if OK, FALSE if errors
 *
 *****************************************************************************/
BOOL Dsu_CalculateCRC( U32 uStartAddress, U32 uLength, PU32 puCrc )
{
  BOOL bResult = FALSE;

  // disable IRQ
  Interrupt_Disable( );

  // un-write-protect it
  PAC1->WPCLR.reg = 0x02;

  // clear the status
  DSU->STATUSA.bit.DONE = ON;
  DSU->STATUSA.bit.BERR = ON;

  // calculate the CRC
  DSU->DATA.reg = 0xFFFFFFFF;
  DSU->ADDR.reg = uStartAddress;
  DSU->LENGTH.reg = uLength;
  DSU->CTRL.bit.CRC = ON;

  // now wait for done
  while( !DSU->STATUSA.bit.DONE );

  // check for no errors
  if ( !DSU->STATUSA.bit.BERR )
  {
    // no errors - store it
    *( puCrc ) = ~DSU->DATA.reg;

    // set good status
    bResult = TRUE;
  }
 
  // protect it
  PAC1->WPSET.reg = 0x02;

  // enable IRQ
  Interrupt_Enable( );

  // return the result
  return( bResult );
}

/**@} EOF Dsu.c */
