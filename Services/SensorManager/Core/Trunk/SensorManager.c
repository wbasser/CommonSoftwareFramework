/******************************************************************************
 * @file Sensormanager.c
 *
 * @brief sensor manager implementation
 *
 * This file provides the sensor manager implementations
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
 * \addtogroup Sensormanager
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "SensorManager/SensorManager.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------
#if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == ON )
  /// define the FIR filter control structure
  typedef struct _FIRFILTCTL
  {
  } FIRFILTCTL, *PFIRFILTCTL;
  #define FIRFILTCTL_SIZE   sizeof ( FIRFILTCTL )
#endif

#if ( SENSORMANAGER_IIR_FILTER_ENABLE == ON )
  /// define the IIR filter control structure
  typedef struct _IIRFILTCTL
  {
  } IIRFILTCTL, *PIIRFILTCTL;
  #define IIRFILTCTL_SIZE   sizeof ( IIRFILTCTL )
#endif

/// define the overall control structure
typedef struct _SENMANCTL
{
  U32         uDesExecCount;      ///< desired execuiton counts
  U32         uCurExecCount;      ///< current execution counts
  SENMANARG   xCurrentValue;      ///< current value
  BOOL        bValid;             ///< valid
  union
  {
    #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == ON )
      FILTAVERAGECTL  tAverage;   ///< average control
    #endif
    #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == ON )
      FILTLDLGCTL     tLeadLag;   ///< lead/lag control
    #endif
    #if ( SENSORMANAGER_FIR_FILTER_ENABLE == ON )
      FIRFILTCTL      tFir;       ///< FIR control
    #endif
    #if ( SENSORMANAGER_IIR_FILTER_ENABLE == ON )
      IIRFILTCTL      tIir;       ///< IIR control
    #endif
  } tFilters;
} SENMANCTL, *PSENMANCTL;
#define SENMANCTL_SIZE    sizeof( SENMANCTL )

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
static  SENMANCTL   atCtls[ SENMAN_ENUM_MAX ];
static  SENMANENUM  eCurSensor = 0;
static  BOOL        bScanWaitEnabled;

// local function prototypes --------------------------------------------------
static  void ProcessFilter( PSENMANDEF ptDef, PSENMANCTL ptCtl, S32 lValue );

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function SensorManager_Initialize
 *
 * @brief sensor manager initialization
 *
 * This function will initialize the sensor manager
 *
 * @return  TRUE if erros, FALSE otherwise
 *
 *****************************************************************************/
BOOL SensorManager_Initialize( void )
{
  PSENMANDEF  ptDef;
  PSENMANCTL  ptCtl;
  SENMANENUM  eSensor;
  
  // call the local initialization
  SensorManager_LocalInitialize( );

  // clear the scan wait
  bScanWaitEnabled = FALSE;
  
  // for each sensor
  for ( eSensor = 0; eSensor < SENMAN_ENUM_MAX; eSensor++ )
  {
    // get a pointer to the definition/control
    ptDef = ( PSENMANDEF )&g_atSensorMngrDefs[ eSensor ];
    ptCtl = ( PSENMANCTL )&atCtls[ eSensor ];
    
    // now calculate the sample time
    ptCtl->uDesExecCount = PGM_RDWORD( ptDef->wSampleRate ) / SENSORMANAGER_EXECUTION_RATE_MSEC;
    ptCtl->uCurExecCount = 0;
    ptCtl->bValid = FALSE;
    
    // determine the type of filter
    switch( PGM_RDBYTE( ptDef->eFiltType ))
    {
      #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_AVERAGE :
        // initialize it
        ptCtl->tFilters.tAverage.tDef.nNumberSamples = ptDef->tFilters.tAverage.nNumberSamples;
        ptCtl->tFilters.tAverage.tDef.pxBuffer = ptDef->tFilters.tAverage.pxBuffer;
        FilterAveraging_Initialize( &ptCtl->tFilters.tAverage );
        break;
    #endif // SENSORMANAGER_AVERAGE_FILTER_ENABLE
        
    #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_LEADLAG :
	  	// clear the values
	  	ptCtl->tFilters.tLeadLag.fPrvValue  = 0.0;
	  	ptCtl->tFilters.tLeadLag.fPrvOutput = 0.0;
	  	ptCtl->tFilters.tLeadLag.fOutput    = 0.0;
        break;
    #endif
        
    #if ( SENSORMANAGER_FIR_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_FIR :
        break;
    #endif
        
    #if ( SENSORMANAGER_IIR_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_IIR :
        break;
    #endif
        
    #if ( SENSORMANAGER_USR_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_USER :
        break;
    #endif

      case SENMAN_FILTTYPE_NONE :
      default :
        break;
    }
  }
  
  // reset the current sensor
  eCurSensor = 0;
  
  // return false
  return( FALSE );
}

/******************************************************************************
 * @function SensorManager_ProcessScan
 *
 * @brief scan all configured sensors
 *
 * This function will scan and process all configured sensors
 *
 *****************************************************************************/
void SensorManager_ProcessScan( void )
{
  PSENMANDEF      ptDef;
  PSENMANCTL      ptCtl;
  S32             lSenValue;
  PVSENMANGEREXT  pvExtGet;
  PVSENMANGERSPC  pvSpcGet;
  PVSENMANGERECB  pvExtCb;
  
  // if no sensors - skip
  if ( SENMAN_ENUM_MAX != 0 )
  {
    // get a pointer to the definition/control
    ptDef = ( PSENMANDEF )&g_atSensorMngrDefs[ eCurSensor ];
    ptCtl = ( PSENMANCTL )&atCtls[ eCurSensor ];

    // test for wait
    if ( !bScanWaitEnabled )
    {
      // now increment the sensor time/check for ready
      if ( ++ptCtl->uCurExecCount >= ptCtl->uDesExecCount )
      {
        // reset the time
        ptCtl->uCurExecCount = 0;
    
        // get a value based on type
        switch( PGM_RDBYTE( ptDef->eInpType ))
        {
          case SENMAN_INPTYPE_INTANA :
            lSenValue = ( S32 )SensorManager_InternalAdcConvertChannel( PGM_RDBYTE( ptDef->nChannel ));
            break;
        
          case SENMAN_INPTYPE_EXTANA :
          case SENMAN_INPTYPE_EXTANACMP :
            pvExtGet = PGM_RDWORD( ptDef->tGetFuncs.pvExt );
            lSenValue = pvExtGet( PGM_RDBYTE( ptDef->nChannel ));
            break;

          case SENMAN_INPTYPE_EXTANACB :
            pvExtGet = PGM_RDWORD( ptDef->tGetFuncs.pvExt );
            pvExtGet( PGM_RDBYTE( ptDef->nChannel ));
            bScanWaitEnabled = TRUE;
            break;
        
          case SENMAN_INPTYPE_SPCANA :
            pvSpcGet = PGM_RDWORD( ptDef->tGetFuncs.pvSpc );
            break;
        
          default :
            lSenValue = 0;
            break;
        }
 
        // if not wwait, call filter
        if ( !bScanWaitEnabled )
        {
          // process the filter
          ProcessFilter( ptDef, ptCtl, lSenValue );
        }
      }
    }
    else
    {
      // call the data ready function
      pvExtCb = PGM_RDWORD( ptDef->pvExtCallback );
      if ( pvExtCb != NULL )
      {
        if ( pvExtCb( ptDef->nChannel, &lSenValue ))
        {
          // process the filter
          ProcessFilter( ptDef, ptCtl, lSenValue );

          // clear the wait flag
          bScanWaitEnabled = FALSE;
        }
      }
    }
  
    // increment current sensor/adjust for max
    if ( !bScanWaitEnabled )
    {
      eCurSensor++;
      eCurSensor %= SENMAN_ENUM_MAX;
    }
  }
}

/******************************************************************************
 * @function SensorManager_GetValue
 *
 * @brief get a value
 *
 * This function will return the last good value of a given sensor
 *
 * @param[in]   eSenNum   sensor enumeration
 * @param[io]   pxValue   pointer to the storage to return the value
 *
 * @return      appropriate error
 *
 *****************************************************************************/
SENMANERROR SensorManager_GetValue( SENMANENUM eSenEnum, PSENMANARG pxValue )
{
  SENMANERROR eError = SENMAN_ERROR_NONE;
  PSENMANDEF      ptDef;
  PSENMANCTL      ptCtl;
  SENMANARG       xMinRawValue, xMaxRawValue, xMinEguValue, xMaxEguValue;
  
  // clear the return value
  *( pxValue ) = 0;
  
  // check for a valid sensor enum
  if ( eSenEnum < SENMAN_ENUM_MAX )
  {
    // get a pointer to the definition/control
    ptDef = ( PSENMANDEF )&g_atSensorMngrDefs[ eSenEnum ];
    ptCtl = ( PSENMANCTL )&atCtls[ eSenEnum ];
    
    // check for valid
    if ( ptCtl->bValid )
    {
      // Determine the filter type
      switch( PGM_RDBYTE( ptDef->eFiltType ))
      {
        // get the value
      #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == ON )
        case SENMAN_FILTTYPE_AVERAGE :
          ptCtl->xCurrentValue = FilterAveraging_GetValue( &ptCtl->tFilters.tAverage );
          break;
      #endif

      #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == ON )
        case SENMAN_FILTTYPE_LEADLAG :
          ptCtl->xCurrentValue = FilterLeadLag_GetValue( &ptCtl->tFilters.tLeadLag );
          break;
      #endif
        
        default :
          break;
      }
      
      // scale only if parameters are not zero
      if (( xMinRawValue != 0 ) && ( xMaxRawValue != 0 ) && ( xMinEguValue != 0 ) && ( xMaxEguValue != 0 ))
      {
        // apply the scaling correction 
        xMinRawValue = PGM_RDWORD( ptDef->xMinRawValue );
        xMaxRawValue = PGM_RDWORD( ptDef->xMaxRawValue );
        xMinEguValue = PGM_RDWORD( ptDef->xMinEguValue );
        xMaxEguValue = PGM_RDWORD( ptDef->xMaxEguValue );
      }
      else
      {
        // return the current value
        *( pxValue )= ptCtl->xCurrentValue;
      }
      
      // check for lineralization
      if ( ptDef->pvLinearize != NULL )
      {
        // call the linerazation function
        *( pxValue ) = ptDef->pvLinearize( ptCtl->xCurrentValue, 0 );
      }
      else if ( ptDef->pvCompensate != NULL )
      {
        // call the compensation function
        *( pxValue ) = ptDef->pvCompensate( ptDef->nChannel, ptCtl->xCurrentValue );
      }
    }
    else
    {
      // return the invalid value error
      eError = SENMAN_ERROR_VALNOTVALID;
    }
  }
  else
  {
    // illegal enumeration
    eError = SENMAN_ERROR_ILLENUM;
  }
  
  // return the error
  return( eError );
}

/******************************************************************************
 * @function ProcessFilter
 *
 * @brief process the filter type
 *
 * This function will process a filter on a completin of scan
 *
 * @param[in]   ptDef       pointer to the definition 
 * @param[in]   ptCtl       pointer to the control structure
 * @param[in]   lSenValue   latest value
 *
 *****************************************************************************/
static  void ProcessFilter( PSENMANDEF ptDef, PSENMANCTL ptCtl, S32 lValue )
{
  // determine the type of filter
  switch( PGM_RDBYTE( ptDef->eFiltType ))
  {
    #if ( SENSORMANAGER_AVERAGE_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_AVERAGE :
        ptCtl->bValid = FilterAveraging_Execute( &ptCtl->tFilters.tAverage, lValue );
        break;
    #endif
  
    #if ( SENSORMANAGER_LEADLAG_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_LEADLAG :
        ptCtl->bValid = FilterLeadLag_Execute( &ptCtl->tFilters.tLeadLag, lValue, ( U32 )TASK_TIME_MSECS( PGM_RDWORD( ptDef->wSampleRate )));
        break;
    #endif
  
    #if ( SENSORMANAGER_FIR_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_FIR :
        break;
    #endif
  
    #if ( SENSORMANAGER_IIR_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_IIR :
        break;
    #endif
  
    #if ( SENSORMANAGER_USR_FILTER_ENABLE == ON )
      case SENMAN_FILTTYPE_USER :
        break;
    #endif

    case SENMAN_FILTTYPE_NONE :
      ptCtl->bValid = TRUE;
      ptCtl->xCurrentValue = ( SENMANARG )lValue;
      break;
  
    default :
      break;
  }
}

/**@} EOF Sensormanager.c */
