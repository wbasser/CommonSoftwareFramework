/******************************************************************************
 * @file Ac_cfg.c
 *
 * @brief aC configuration implementation 
 *
 * This file provides the implementation for the AC module
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
 * \addtogroup AC
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "AC/Ac_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------
const ACDEF  g_atAcDefs[ AC_ENUM_MAX ] =
{
  // add AC definitions here using one of the below macros
  // ACCHANDEFBLKM( chan, posinp, neginp, filtlen, speedsel, hystenb, singshot, inpevnenb, outevnenb, outmode, outedge )
  // ACCHANDEFNBLKM( chan, posinp, neginp, filtlen, speedsel, hystenb, singshot, inpevnenb, outevnenb, outmode, outedge, callback )

};

/**@} EOF Ac_cfg.c */
