/******************************************************************************
 * @file AgilionTagComm.h
 *
 * @brief Agilion Tag Communication protocol handler
 *
 * This file provides the declarations for the Agilion Tag Communication handler
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
 * \addtogroup AgilionTagComm
 * @{
 *****************************************************************************/
 
// ensure only one instantiation
#ifndef _AGILIONTAGCOMM_H
#define _AGILIONTAGCOMM_H

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------

// library includes -----------------------------------------------------------
#include "Types/Types.h"

// Macros and Defines ---------------------------------------------------------
/// define the maximum data size
#define AGILION_MAX_DATA_SIZE                     ( 512 )

// enumerations ---------------------------------------------------------------
/// enumerate the decode status
typedef enum _AGILIONSTS
{
  AGILION_STS_NONE = 0,           ///< no error
  AGILION_STS_DECINPRG,           ///< decode in progress
  AGILION_STS_VALIDMSG,           ///< message received - valid
  AGILION_STS_CHECKERR = 0xFF,    ///< message received - checksum error
} AGILIONSTS;

/// enumerate the message IDS
typedef enum _AGILIONMSGID
{
  AGILION_MSGID_ERROR = 0,
  AGILION_MSGID_TAGCHRS,
  AGILION_MSGID_RESET,
  AGILION_MSGID_ECHO,
  AGILION_MSGID_STATUS,
  AGILION_MSGID_PRESSURE,
  AGILION_MSGID_TEMPERATURE,
  AGILION_MSGID_REQGPS,
  AGILION_MSGID_FREEFALLSET,
  AGILION_MSGID_FREEFALLDET,
  AGILION_MSGID_ACCELSET,
  AGILION_MSGID_ACCELDET,
  AGILION_MSGID_IMUDATA,
  AGILION_MSGID_REQWLS,
  AGILION_MSGID_REQNETTIME,
  AGILION_MSGID_REQDEVADDR,
  AGILION_MSGID_UICTOKYRA,
  AGILION_MSGID_KYRATOUIC,
} AGILIONMSGID;

/// enumerate the error codes
typedef enum _AGILIONERRCODE
{
  AGILION_ERRCODE_ILLCMD = 2,
  AGILION_ERRCODE_ILLPRM,
  AGILILN_ERRCODE_NODATA,  
} AGILIONERRCODE;

// structures -----------------------------------------------------------------
/// define the error
typedef struct _AGILIONMSGERROR
{
  AGILIONSTS  eStatus;
} AGILIONMSGERROR, *PAGILIONMSGERROR;
#define AGILIONMSGERROR_SIZE                    sizeof( AGILIONMSGERROR )

/// define the characteristics of the tag
typedef struct _AGILIONMSGTAGCHRS
{
  C8  acHardware[ 4 ];          ///< hardware
  C8  acSoftware[ 3 ][ 3 ];     ///< software
  C8  acSerNum[ 6 ];            ///< serial number    
} AGILIONMSGTAGCHRS, *PAGILIONMSGTAGCHRS;
#define AGILIONTAGMSGCHRS_SIZE                  sizeof( AGILIONMSGTAGCHRS )

/// define the echo command
typedef struct _AGILIONMSGECHO
{
  U8  anContent[ 40 ];
} AGILIONMSGECHO, *PAGILIONMSGECHO;
#define AGILIONMSGECHO_SIZE                     sizeof( AGILIONMSGECHO )

/// define the status message
typedef struct _AGILIONMSGSTS
{
  struct   
  { 
    BOOL  bReady          : 1;  ///< ready, tag is up and running
    BOOL  bConnected      : 1;  ///< connected to server
    BOOL  bGPSAvailable   : 1;  ///< GPS data available
    BOOL  bPressAvailable : 1;  ///< pressure available
    BOOL  bAccelAvailable : 1;  ///< acceleration available
    BOOL  bGyroAvailable  : 1;  ///< gyro available
    BOOL  bMagnAvailable  : 1;  ///< magnetometer available
    BOOL  bTempAvailable  : 1;  ///< temperature available
  } tStatus1;
  U8 nStatus2;
} AGILIONMSGSTS, *PAGILIONMSGSTS;
#define AGILIONMSGSTS_SIZE                      sizeof( AGILIONMSGSTS)

/// define the pressure message
typedef struct _AGILIONMSGPRES
{
  U8    nPresMsb;            ///< pressure MSB
  U8    nPresMs1;            ///< pressure MS1
  U8    nPresMs2;            ///< pressure MS2
  U8    nPresLsb;            ///< pressure LSB
} AGILIONMSGPRES, *PAGILIONMSGPRES;
#define AGILIONMSGPRES_SIZE                     sizeof( AGILIONMSGPRES )

/// define the temperatrue message
typedef struct _AGILIONMSGTEMP
{
  U8    nTempMsb;             ///< temp MSB
  U8    nTempLsb;             ///< temp LSB
} AGILIONMSGTEMP, *PAGILIONMSGTEMP;
#define AGILIONMSGTEMP_SIZE                     sizeof( AGILIONMSGTEMP )

/// define the GPS message
typedef struct _AGILIOMMSGGPS
{
  U8    nAccuracy;          ///< accuracy
  U8    nLatMsb;            ///< latitude MSB
  U8    nLatMs1;            ///< latitude MS1
  U8    nLatMs2;            ///< latitude MS2
  U8    nLatLsb;            ///< latitude LSB
  U8    nLonMsb;            ///< longitude MSB
  U8    nLonMs1;            ///< longitude MS1
  U8    nLonMs2;            ///< longitude MS2
  U8    nLonLsb;            ///< longitude LSB
  U8    nAltMsb;            ///< altitude MSB
  U8    nAltMs1;            ///< altitude MS1
  U8    nAltMs2;            ///< altitude MS2
  U8    nAltLsb;            ///< altitude LSB
} AGILIOMMSGGPS, *PAGILIOMMSGGPS;
#define AGILIOMMSGGPS_SIZE                    sizeof( AGILIOMMSGGPS )

/// define the freefall/accel threshold
typedef struct _AGILIONMSGFFACLT
{
  U8    nAccelMsb;          ///< acceleration MSB
  U8    nAccelLsb;          ///< acceleration LSB
  U8    nTmoMsb;            ///< timeout MSB
  U8    nTmoLsb;            ///< timeout LSB
} AGILIONMSGFFACLT, *PAGILIONMSGFFACLT;
#define AGILIONMSGFFACLT_SIZE                    sizeof( AGILIONMSGFFACLT )

/// define the freefall/accel detec
typedef struct _AGILIONMSGFFACLD
{
  U8    nDetect;            ///< detect
} AGILIONMSGFFACLD, *PAGILIONMSGFFACLD;
#define AGILIONMSGFFACLD_SIZE                    sizeof( AGILIONMSGFFACLD )

/// define the WLS message
typedef struct _AGILIOMMSGWLS
{
  U8    nReserved;          ///< reserved
  U8    nLatMsb;            ///< latitude MSB
  U8    nLatMs1;            ///< latitude MS1
  U8    nLatMs2;            ///< latitude MS2
  U8    nLatLsb;            ///< latitude LSB
  U8    nLonMsb;            ///< longitude MSB
  U8    nLonMs1;            ///< longitude MS1
  U8    nLonMs2;            ///< longitude MS2
  U8    nLonLsb;            ///< longitude LSB
  U8    nAltMsb;            ///< altitude MSB
  U8    nAltMs1;            ///< altitude MS1
  U8    nAltMs2;            ///< altitude MS2
  U8    nAltLsb;            ///< altitude LSB
} AGILIOMMSGWLS, *PAGILIOMMSGWLS;
#define AGILIOMMSGWLS_SIZE                      sizeof( AGILIOMMSGWLS )

/// define the IMU data message
typedef struct _AGILIONMSGIMU
{
  U8    nAccelXMsb;         ///< acceleration X MSB
  U8    nAccelXMs1;         ///< acceleration X MS1
  U8    nAccelXMs2;         ///< acceleration X MS2
  U8    nAccelXLsb;         ///< acceleration X LSB
  U8    nAccelYMsb;         ///< acceleration Y MSB
  U8    nAccelYMs1;         ///< acceleration Y MS1
  U8    nAccelYMs2;         ///< acceleration Y MS2
  U8    nAccelYLsb;         ///< acceleration Y LSB
  U8    nAccelZMsb;         ///< acceleration Z MSB
  U8    nAccelZMs1;         ///< acceleration Z MS1
  U8    nAccelZMs2;         ///< acceleration Z MS2
  U8    nAccelZLsb;         ///< acceleration Z LSB
  U8    nGyroXMsb;          ///< gyro X MSB
  U8    nGyroXMs1;          ///< gyro X MS1
  U8    nGyroXMs2;          ///< gyro X MS2
  U8    nGyroXLsb;          ///< gyro X LSB
  U8    nGyroYMsb;          ///< gyro Y MSB
  U8    nGyroYMs1;          ///< gyro Y MS1
  U8    nGyroYMs2;          ///< gyro Y MS2
  U8    nGyroYLsb;          ///< gyro Y LSB
  U8    nGyroZMsb;          ///< gyro Z MSB
  U8    nGyroZMs1;          ///< gyro Z MS1
  U8    nGyroZMs2;          ///< gyro Z MS2
  U8    nGyroZLsb;          ///< gyro Z LSB
  U8    nMagnXMsb;          ///< magnetic X MSB
  U8    nMagnXMs1;          ///< magnetic X MS1
  U8    nMagnXMs2;          ///< magnetic X MS2
  U8    nMagnXLsb;          ///< magnetic X LSB
  U8    nMagnYMsb;          ///< magnetic Y MSB
  U8    nMagnYMs1;          ///< magnetic Y MS1
  U8    nMagnYMs2;          ///< magnetic Y MS2
  U8    nMagnYLsb;          ///< magnetic Y LSB
  U8    nMagnZMsb;          ///< magnetic Z MSB
  U8    nMagnZMs1;          ///< magnetic Z MS1
  U8    nMagnZMs2;          ///< magnetic Z MS2
  U8    nMagnZLsb;          ///< magnetic Z LSB
} AGILIONMSGIMU, *PAGILIONMSGIMU;
#define AGILIONMSGIMU_SIZE                      sizeof( AGILIONMSGIMU )

/// define the union of all the structures
typedef struct _AGILIONMESSAGES
{
  AGILIONMSGID          eMsgId;
  union
  {
    union
    {
      AGILIONMSGERROR     tError;
      AGILIONMSGTAGCHRS   tTagChrs;
      AGILIONMSGECHO      tEcho;
      AGILIONMSGSTS       tStatus;
      AGILIONMSGPRES      tPress;
      AGILIONMSGTEMP      tTemp;
      AGILIOMMSGGPS       tGps;
      AGILIONMSGFFACLT    tFreefallThres;
      AGILIONMSGFFACLD    tFreefallDet;
      AGILIONMSGFFACLT    tAccelThres;
      AGILIONMSGFFACLD    tAccelDet;
      AGILIONMSGIMU       tImu;
      AGILIOMMSGWLS       tWls;
    } tMessages;
    U8  anByte[ AGILION_MAX_DATA_SIZE + 1 ];
  } tContent;
} AGILIONMESSAGES, *PAGILIONMESSAGES;
#define AGILIONMESSAGES_SIZE                    sizeof( AGILIONMESSAGES )

// global parameter declarations -----------------------------------------------

// global function prototypes --------------------------------------------------
extern  void        AgilionTagComm_Initialize( void );
extern  AGILIONSTS  AgilionTagComm_ProcessChar( PAGILIONMESSAGES ptRcvMessage, U8 nChar );
extern  U16         AgilionTagComm_FormatBuffer( PAGILIONMESSAGES pMessage, PU8 pnXmitBuffer, U16 wBufLength, U16 wMsgLength  );

/**@} EOF AgilionTagComm.h */

#endif  // _AGILIONTAGCOMM_H