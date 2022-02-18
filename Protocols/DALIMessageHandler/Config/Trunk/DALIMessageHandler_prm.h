/****************************************7**************************************
 * @file  DALIMessageHandler_prm.h
 *
 * @brief  DALI message handler declarations
 *
 * This file provides the declarations for the DALI message handler
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
 * $Rev: $
 *
 *
 * \addtogroup  DALIMessageHandler
 * @{
 *****************************************************************************/

// ensure only one instantiation
#ifndef _DALIMESSAGEHANDLER_PRM_H
#define _DALIMESSAGEHANDLER_PRM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "ManchesterCodec/ManchesterCodec.h"
#include "ParameterManager/ParameterManager.h"
#include "TaskManager/TaskManager.h"

// Macros and Defines ---------------------------------------------------------
/// define the DALI version number
#define	DALI_VERSION_NUMBER               ( 1 )

/// define the physical minimum level
#define	DALI_PHYS_MINLEVEL                ( 25 )

/// define the DALI Debug level
#define DALI_DEBUG_LEVEL_NONE             ( 0 )
#define DALI_DEBUG_LEVEL_XMTONLY          ( 1 )
#define DALI_DEBUG_LEVEL_RCVONLY          ( 2 )
#define DALI_DEBUG_LEVEL_BOTH             ( 3 )

/// define the enable debug macro
#define DALI_DEBUG_LEVEL                  ( DALI_DEBUG_LEVEL_XMTONLY )

/// define the delay time between forward frame/backward frame
#define DALI_FORWARD_BACKWARD_DELAY_TIME  ( 4 )

/// define the manchester receive done task argument
#define DALI_MANCHESTER_RCV_DONE_EVENT    ( MANCHESTERCODEC_RECV_DONE )
#define DALI_MANCHESTER_RCV_EROR_EVENT    ( MANCHESTERCODEC_RECV_EROR )
#define DALI_MANCHESTER_XMT_DONE_EVENT    ( MANCHESTERCODEC_XMIT_DONE )

/// define the parameter enumerations
#define DALI_PARAM_SHORT_ADDR             ( PARAMSEL_ENUM_OPS_SHRTADRR )
#define DALI_PARAM_POWERON_LEVEL          ( PARAMSEL_ENUM_OPS_PWRONLVL )
#define DALI_PARAM_SYSFAIL_LEVEL          ( PARAMSEL_ENUM_OPS_SYSFLVL )
#define DALI_PARAM_MIN_LEVEL              ( PARAMSEL_ENUM_OPS_MINLVL )
#define DALI_PARAM_MAX_LEVEL              ( PARAMSEL_ENUM_OPS_MAXLVL )
#define DALI_PARAM_LASTREQ_LEVEL          ( PARAMSEL_ENUM_OPS_LREQVAL )
#define DALI_PARAM_FADE_RATE              ( PARAMSEL_ENUM_OPS_FADERATE )
#define DALI_PARAM_FADE_TIME              ( PARAMSEL_ENUM_OPS_FADETIME )
#define DALI_PARAM_RANDOM_ADDR_H          ( PARAMSEL_ENUM_OPS_RANADRH )
#define DALI_PARAM_RANDOM_ADDR_M          ( PARAMSEL_ENUM_OPS_RANADRM )
#define DALI_PARAM_RANDOM_ADDR_L          ( PARAMSEL_ENUM_OPS_RANADRL )
#define DALI_PARAM_GROUP0007              ( PARAMSEL_ENUM_OPS_GRP0007 )
#define DALI_PARAM_GROUP0815              ( PARAMSEL_ENUM_OPS_GRP0815 )
#define DALI_PARAM_SCENE00                ( PARAMSEL_ENUM_OPS_SCENE00 )
#define DALI_PARAM_SCENE01                ( PARAMSEL_ENUM_OPS_SCENE01 )
#define DALI_PARAM_SCENE02                ( PARAMSEL_ENUM_OPS_SCENE02 )
#define DALI_PARAM_SCENE03                ( PARAMSEL_ENUM_OPS_SCENE03 )
#define DALI_PARAM_SCENE04                ( PARAMSEL_ENUM_OPS_SCENE04 )
#define DALI_PARAM_SCENE05                ( PARAMSEL_ENUM_OPS_SCENE05 )
#define DALI_PARAM_SCENE06                ( PARAMSEL_ENUM_OPS_SCENE06 )
#define DALI_PARAM_SCENE07                ( PARAMSEL_ENUM_OPS_SCENE07 )
#define DALI_PARAM_SCENE08                ( PARAMSEL_ENUM_OPS_SCENE08 )
#define DALI_PARAM_SCENE09                ( PARAMSEL_ENUM_OPS_SCENE09 )
#define DALI_PARAM_SCENE10                ( PARAMSEL_ENUM_OPS_SCENE10 )
#define DALI_PARAM_SCENE11                ( PARAMSEL_ENUM_OPS_SCENE11 )
#define DALI_PARAM_SCENE12                ( PARAMSEL_ENUM_OPS_SCENE12 )
#define DALI_PARAM_SCENE13                ( PARAMSEL_ENUM_OPS_SCENE13 )
#define DALI_PARAM_SCENE14                ( PARAMSEL_ENUM_OPS_SCENE14 )
#define DALI_PARAM_SCENE15                ( PARAMSEL_ENUM_OPS_SCENE15 )
#define DALI_PARAM_FAILURESTATUS          ( PARAMSEL_ENUM_OPS_FAILSTS )
#define DALI_PARAM_OPMODE                 ( PARAMSEL_ENUM_OPS_OPMODE )
#define DALI_PARAM_FASTFADETIME           ( PARAMSEL_ENUM_OPS_FFADETIM )
#define DALI_PARAM_MB0_MINFASTFADETIME    ( PARAMSEL_ENUM_MB0_MINFFTIME )
#define DALI_PARAM_MB0_GEARTYPE           ( PARAMSEL_ENUM_MB0_GEARTYPE )
#define DALI_PARAM_MB0_POSSOPTYPES        ( PARAMSEL_ENUM_MB0_POPTYPES )
#define DALI_PARAM_MB0_FEATURES           ( PARAMSEL_ENUM_MB0_FEATURES )
#define DALI_PARAM_MB0_EXTVERNUM          ( PARAMSEL_ENUM_MB0_EXTVERNUM )
#define DALI_PARAM_MB0_DEVTYPE            ( PARAMSEL_ENUM_MB0_DEVTYPE )
#define DALI_PARAM_MB0_PHYSICALMINLVL     ( PARAMSEL_ENUM_MB0_PHYMNLVL )

/// define the memory block baqse parameter enums
#define DALI_PARAM_MB0_BASE_ENUM          ( PARAMSEL_ENUM_MB0_LASTADDR )
#define DALI_PARAM_MB1_BASE_ENUM          ( PARAMSEL_ENUM_MB1_LASTADDR )

/**@} EOF DALIMessageHandler_prm.h */

#endif  // _DALIMESSAGEHANDLER_PRM_H
