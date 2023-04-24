/******************************************************************************
 * @file BinCommonCommands.c
 *
 * @brief 
 *
 * This file 
 *
 * @copyright Copyright (c) 2012 CyberIntegration
 * This document contains proprietary data and information of CyberIntegration 
 * LLC. It is the exclusive property of CyberIntegration, LLC and will not be 
 * disclosed in any form to any party without prior written permission of 
 * CyberIntegration, LLC. This document may not be reproduced or further used 
 * without the prior written permission of CyberIntegration, LLC.
 *
 * $Date: $
 *
 * Version History
 * ======
 * $Rev: $
 * 
 *
 * \addtogroup BinCommonCommands
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "BinCommonCommands/BinCommonCommands.h"

// library includes -----------------------------------------------------------
#include "BinaryCommandHandler/BinaryCommandHandler.h"
#include "BootLoader/BootLoader.h"
#include "ConfigManager/ConfigManager.h"
#include "LogHandler/LogHandler.h"
#include "ManufInfo/ManufInfo.h"
#include "RTCManager/RTCManager.h"
#include "SerialNumber/SerialNumber.h"
#include "SystemControlManager/SystemControlManager.h"
#include "SystemMOnitor/SystemMonitor.h"

// Macros and Defines ---------------------------------------------------------
#define BINCMN_ERR_NOTIMPLEMENTED       ( 0xFF )
#define BINCMN_ERR_ILLEGALPASSWORD      ( 0xFE )
#define BINCMD_ERR_ILLEGALLOGENTRY      ( 0xFD )

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// local slave command handlers 
static  BINPARSESTS CmdGetPng( U8 nCmdEnum );
static  BINPARSESTS CmdGetRev( U8 nCmdEnum );
static  BINPARSESTS CmdRstDfl( U8 nCmdEnum );
static  BINPARSESTS CmdQryMod( U8 nCmdEnum );
static  BINPARSESTS CmdSetMod( U8 nCmdEnum );
static  BINPARSESTS CmdQrySnm( U8 nCmdEnum );
static  BINPARSESTS CmdBootEn( U8 nCmdEnum );
static  BINPARSESTS CmdGetCrv( U8 nCmdEnum );

// constant parameter initializations -----------------------------------------
const CODE BINCMDSLVENTRY g_atBinCommonCommandsTable[ ] =
{
  BINCMD_SLV_INTENTRY( BINCMN_CMD_GETPNG, 0, -1, BINCOMP_FLAG_NONE, 0, CmdGetPng ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_GETREV, 0, -1, BINCOMP_FLAG_NONE, 0, CmdGetRev ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_RSTDFL, 0, -1, BINCOMP_FLAG_NONE, 0, CmdRstDfl ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_QRYMOD, 0, -1, BINCOMP_FLAG_NONE, 0, CmdQryMod ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_SETMOD, 0, -1, BINCOMP_FLAG_NONE, 0, CmdSetMod ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_QRYSNM, 0, -1, BINCOMP_FLAG_NONE, 0, CmdQrySnm ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_BOOTEN, 0, -1, BINCOMP_FLAG_NONE, 0, CmdBootEn ),
  BINCMD_SLV_INTENTRY( BINCMN_CMD_GETCRV, 0, -1, BINCOMP_FLAG_NONE, 0, CmdGetCrv ),

  // test for log handler
  #if ( LOGHANDLER_ENABLE_DBGBINCOMMANDS == ON )
    BINCMD_SLV_EXTENTRY( g_atLogHandlerBinCmdHandlerTable ),
  #endif // LOGHANDLER_ENABLE_DBGBINCOMMANDS

  // test for RTC manager
  #if ( RTCMANAGER_ENABLE_BINCTL_COMMANDS == ON )
    BINCMD_SLV_EXTENTRY( g_atRtcManagerBinCmdHandlerTable ),
  #endif // RTCMANAGER_ENABLE_BINCTL_COMMANDS

  // test for system monitor
  #if ( SYSTEMMONITOR_ENABLE_DBGBINCOMMANDS == ON )
    BINCMD_SLV_EXTENTRY( g_atSystemMonitorBinCmdHandlerTable ),
  #endif // SYSTEMMONITOR_ENABLE_DBGBINCOMMANDS

  // do not remove below entry
  BINCMD_SLV_ENDENTRY( ) // end of table
};

/******************************************************************************
 * @function CmdQryPng
 *
 * @brief ping
 *
 * This function will respond with an ACK
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdGetPng( U8 nCmdEnum )
{
  // send an ACK
  return( BINPARSE_STS_SND_ACK );
}

/******************************************************************************
 * @function CmdQryVer
 *
 * @brief respond with the software version
 *
 * This function respond with the software title and version
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdGetRev( U8 nCmdEnum )
{
  PC8   pszTemp;
  #if (( MANUFINFO_REV_BLT_VER_ENABLE == ON ) && ( SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS != 0 ))
    U32   uBootLoadRevAddress;
    U32UN tBootLoadRevision;
  #endif // SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS 

  #if (( MANUFINFO_REV_BLT_VER_ENABLE == ON ) && ( SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS != 0 ))
    // set the address of the boot loader
    uBootLoadRevAddress = SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS;
    tBootLoadRevision.uValue = *( PU32 )( uBootLoadRevAddress );
  #endif

  #if (( MANUFINFO_REV_BLT_VER_ENABLE == ON ) && ( SYSTEMDEFINE_BOOTLOADER_REV_ADDRESS != 0 ))
    // now set the bootloader major/minor
    BinaryCommandHandler_SetMessageByte( nCmdEnum, 'B' );
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tBootLoadRevision.anValue[ LE_U32_MS1_IDX ]);
    BinaryCommandHandler_SetMessageByte( nCmdEnum, tBootLoadRevision.anValue[ LE_U32_LSB_IDX ]);
  #endif
  
  // now set the hardware major/minor/
  BinaryCommandHandler_SetMessageByte( nCmdEnum, 'H' );
  BinaryCommandHandler_SetMessageByte( nCmdEnum, ManufInfo_GetHdwMajor( ));
  BinaryCommandHandler_SetMessageByte( nCmdEnum, ManufInfo_GetHdwMinor( ));

  // now set the software major/minor/
  BinaryCommandHandler_SetMessageByte( nCmdEnum, 'S' );
  BinaryCommandHandler_SetMessageByte( nCmdEnum, ManufInfo_GetSfwMajor( ));
  BinaryCommandHandler_SetMessageByte( nCmdEnum, ManufInfo_GetSfwMinor( ));
  #if ( MANUFINFO_REV_SFW_TST_ENABLE == 1 )
    BinaryCommandHandler_SetMessageByte( nCmdEnum, ManufInfo_GetSfwTest( ));
  #endif

  // copy the title
  pszTemp = ManufInfo_GetTitle( );
  BinaryCommandHandler_SetMessageBlock( nCmdEnum, ( PU8 )pszTemp, MANUFINFO_TITLE_LEN );
  BinaryCommandHandler_SetMessageByte( nCmdEnum, 0 );
  
  // copy the part number 2
  pszTemp = ManufInfo_GetPartNum2( );
  BinaryCommandHandler_SetMessageBlock( nCmdEnum, ( PU8 )pszTemp, MANUFINFO_PRTNUM2_LEN );
  BinaryCommandHandler_SetMessageByte( nCmdEnum, 0 );

  // now force a message to be sent
  return( BINPARSE_STS_SND_RESP );
}

/******************************************************************************
 * @function CmdRstDfl
 *
 * @brief     reset all configuration to their default value
 *
 * This function will reset all configurations to their defualt value
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdRstDfl( U8 nCmdEnum )
{
  BINPARSESTS eStatus = BINPARSE_STS_SND_ACK;
  U32         uPassword;
  
  // get the password/compare
  BinaryCommandHandler_GetRcvBufferPointer( nCmdEnum, (PU8*)&uPassword );
  if ( uPassword == SYSTEMDEFINE_CONFIG_RESET_DEFAULT )
  {
    // reset the configuration to defaults
    ConfigManager_ResetDefaults( );
  }
  else
  {
    // set the error
    BinaryCommandHandler_SetOption1( nCmdEnum, BINCMN_ERR_ILLEGALPASSWORD );
    eStatus = BINPARSE_STS_SND_NAK;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function CmdQryMod
 *
 * @brief     query the current mode
 *
 * This function will return the current mode
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdQryMod( U8 nCmdEnum )
{
  // get the curent mode
  BinaryCommandHandler_SetOption2( nCmdEnum, SystemControlManager_GetMode( )); 

  // now force a message to be sent
  return( BINPARSE_STS_SND_ACK );
}

/******************************************************************************
 * @function CmdSetMod
 *
 * @brief     set the current mode
 *
 * This function will set the current mode
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdSetMod( U8 nCmdEnum )
{
  BINPARSESTS     eStatus = BINPARSE_STS_SND_ACK;
  SYSCTRLMGRERR   eError;
  U8              nNewMode;

  // get the new mode
  BinaryCommandHandler_GetOption1( nCmdEnum, &nNewMode );

  // set the mode
  if (( eError = SystemControlManager_SetMode( nNewMode )) != SYSCTRLMNGR_ERROR_NONE )
  {
    // set the status to NAK/stuff error code in option2
    BinaryCommandHandler_SetOption2( nCmdEnum, eError );
    eStatus = BINPARSE_STS_SND_NAK;
  }
  
  // return the status
  return( eStatus );
}

/******************************************************************************
 * @function CmdQrySnm
 *
 * @brief     query the serial number
 *
 * This function will return the serial number
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdQrySnm( U8 nCmdEnum )
{
  SERIALNUMBER  tSerial;

  // get the serial number
  SerialNumber_Get( &tSerial );

  // stuff it
  BinaryCommandHandler_SetMessageBlock( nCmdEnum, ( PU8 )&tSerial.anBytes, SERIALNUMBER_SIZE );

  // now force a message to be sent
  return( BINPARSE_STS_SND_RESP );
}

/******************************************************************************
 * @function CmdBootEn
 *
 * @brief   enter the boot mode
 *
 * This function  will enter the boot mode
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdBootEn( U8 nCmdEnum )
{
  // call the boot loader
  BootLoader_Enter( );

  // return the status
  return( BINPARSE_STS_SND_ACK );
}

/******************************************************************************
 * @function CmdGetCrv
 *
 * @brief   get the configuration revision
 *
 * This function  will return the current configuration revision
 *
 * @return      appropriate protocol status/error
 *
 *****************************************************************************/
static BINPARSESTS CmdGetCrv( U8 nCmdEnum )
{
  CFGUSRVER tUserVersion;

  // get the configuration version
  ConfigManager_GetUserVersion( &tUserVersion );

  // stuff the major/minor/user
  BinaryCommandHandler_SetMessageByte( nCmdEnum, tUserVersion.anValue[ LE_U16_MSB_IDX ] );
  BinaryCommandHandler_SetMessageByte( nCmdEnum, tUserVersion.anValue[ LE_U16_LSB_IDX ] );

  // return the status
  return( BINPARSE_STS_SND_RESP );
}

/**@} EOF BinCommonCommands.c */
