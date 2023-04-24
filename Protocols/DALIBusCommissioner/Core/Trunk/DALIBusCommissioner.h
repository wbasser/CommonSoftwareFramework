/******************************************************************************
 * @file DALIBusCommissioner.h
 *
 * @brief DALI bus commisioner declarations 
 *
 * This file provides the bus commissioning for the DALI bus
 *
 * @copyright Copyright (c) 2012CyberIntegration
 * This document contains proprietary data and information ofCyberIntegration 
 * LLC. It is the exclusive property ofCyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 *CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission ofCyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup DALIBusCommissioner
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _DALIBUSCOMMISSIONER_H
#define _DALIBUSCOMMISSIONER_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DALIBusCommissioner/DALIBusCommissioner_cfg.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  BOOL  DALIBusCommissioner_Initialize( void );
extern  void  DALIBusCommissioner_EventProcess( DALIBUSCOMMARG xArg );
extern  void  DALIBusCommissioner_Control( BOOL bState );

/**@} EOF DALIBusCommissioner.h */

#endif  // _DALIBUSCOMMISSIONER_H