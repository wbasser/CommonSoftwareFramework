/******************************************************************************
 * @file HardwareDebug.c
 *
 * @brief hardware debug implementation
 *
 * This file provides the implementation for the hardware debug
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
 * \addtogroup HardwareDebug
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "HardwareDebug/HardwareDebug.h"

// library includes -----------------------------------------------------------
#if ( HARDWAREDEBUG_ENABLE_ADC == ON )
  #include "ADC/Adc.h"
#endif
#if ( HARDWAREDEBUG_ENABLE_GPIO == ON )
  #include "GPIO/Gpio.h"
#endif
#if ( HARDWAREDEBUG_ENABLE_I2C == ON )
  #include "I2C/I2c.h"
#endif
#include "SystemControlManager/SystemControlManager.h"

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------
/// command handlers
#if ( HARDWAREDEBUG_ENABLE_ADC == ON )
  static ASCCMDSTS CmdAdcCnv( U8 nCmdEnum );
#endif
#if ( HARDWAREDEBUG_ENABLE_GPIO == ON )
  static ASCCMDSTS CmdGpioRd( U8 nCmdEnum );
  static ASCCMDSTS CmdGpioWr( U8 nCmdEnum );
  static ASCCMDSTS CmdGpioRdIrq( U8 nCmdEnum );
#endif
#if ( HARDWAREDEBUG_ENABLE_I2C == ON )
  static ASCCMDSTS CmdI2cRd( U8 nCmdEnum );
  static ASCCMDSTS CmdI2cWr( U8 nCmdEnum );
  static ASCCMDSTS CmdI2cDmp( U8 nCmdEnum );
  static ASCCMDSTS CmdI2cPoll( U8 nCmdENum ); 
#endif

// local function prototypes --------------------------------------------------

// localconstant parameter initializations ------------------------------------
/// command constants
#if ( HARDWAREDEBUG_ENABLE_ADC == ON )
  static  const CODE C8 szCmdAdcCnv[ ]    = { "ADCCNV" };
#endif
#if ( HARDWAREDEBUG_ENABLE_GPIO == ON )
  static  const CODE C8 szCmdGpioRd[ ]    = { "GPIORD" };
  static  const CODE C8 szCmdGpioWr[ ]    = { "GPIOWR" };
//  static  const CODE C8 szCmdGpioRdIrq[ ] = { "GPIORDIRQ" };
#endif
#if ( HARDWAREDEBUG_ENABLE_I2C == ON )
  static  const CODE C8 szCmdI2cRd[ ]     = { "I2CRD" };
  static  const CODE C8 szCmdI2cWr[ ]     = { "I2CWR" };
  static  const CODE C8 szCmdI2cDmp[ ]    = { "I2CDMP" };
  static  const CODE C8 szCmdI2cPoll[ ]   = { "I2CPOLL" };
#endif

// reponse strings
#if ( HARDWAREDEBUG_ENABLE_ADC == ON )
  static  const CODE C8 szRspAdcCvn[ ]    = { "RADCCvn, ENUM: %02X, VAL: %04X\n\r" };
#endif
#if ( HARDWAREDEBUG_ENABLE_GPIO == ON )
  static  const CODE C8 szRspRdGpio[ ]    = { "RGPIORD, ENUM: %02X, VAL: %02X\n\r" };
#endif
#if ( HARDWAREDEBUG_ENABLE_I2C == ON )
  static  const CODE C8 szRspRdI2c[ ]     = { "RI2CRD, DEV: %02X, ADDR: %02X, VAL: %02X\n\r" };
#endif

// global constant parameter initializations ----------------------------------
/// command table
const CODE ASCCMDENTRY g_atHardwareDebugCommandsTable[ ] =
{
#if ( HARDWAREDEBUG_ENABLE_ADC == ON )
  ASCCMD_ENTRY( szCmdAdcCnv, 6, 1, ASCFLAG_COMPARE_GE,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdAdcCnv ),
#endif
#if ( HARDWAREDEBUG_ENABLE_GPIO == ON )
  ASCCMD_ENTRY( szCmdGpioRd,    6, 1, ASCFLAG_COMPARE_GE,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdGpioRd ),
  ASCCMD_ENTRY( szCmdGpioWr,    6, 2, ASCFLAG_COMPARE_GE,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdGpioWr ),
//  ASCCMD_ENTRY( szCmdGpioRdIrq, 6, 1, ASCFLAG_COMPARE_GE,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdGpioRdIrq ),
#endif
#if ( HARDWAREDEBUG_ENABLE_I2C == ON )
  ASCCMD_ENTRY( szCmdI2cRd,   5, 3, ASCFLAG_COMPARE_GE,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdI2cRd  ),
  ASCCMD_ENTRY( szCmdI2cWr,   5, 4, ASCFLAG_COMPARE_GE,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdI2cWr  ),
  ASCCMD_ENTRY( szCmdI2cDmp,  6, 4, ASCFLAG_COMPARE_GE,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdI2cDmp ),
  ASCCMD_ENTRY( szCmdI2cPoll, 7, 2, ASCFLAG_COMPARE_GE,   SYSCTRLMNGR_LCLMODE_DIAGNOSTICS, CmdI2cPoll ),
#endif
   
  // the entry below must be here
  ASCCMD_ENDTBL( )
};

#if ( HARDWAREDEBUG_ENABLE_ADC == ON )
  /******************************************************************************
   * @function CmdAdcCnv
   *
   * @brief convert an ADC channel
   *
   * This function will convert an ADC channel
   *
   * @param[in]   nCmdEnum    command enumeration
   *
   * @return      appropriate status
   *
   *****************************************************************************/
  static ASCCMDSTS CmdAdcCnv( U8 nCmdEnum )
  {
    PC8     pcBuffer;
    ADCERRS eError;
    U32UN   tValue;

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // get the arguments
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );
    
    // check for valid
    if (( Adc_ConvertChannel( tValue.anValue[ LE_U32_LSB_IDX ], &tValue.awValue[ LE_U32_MSW_IDX ])) == ADC_ERR_NONE )
    {
      // output the read value
      SPRINTF_P( pcBuffer, ( PCC8 )szRspAdcCvn, tValue.anValue[ LE_U32_LSB_IDX ], tValue.awValue[ LE_U32_MSW_IDX ] );
    }
    else
    {
      // report error
      SPRINTF_P( pcBuffer, ( PCC8 )g_szAsciiErrStrn, eError, eError );
    }

    // return the status
    return( ASCCMD_STS_OUTPUTBUFFER );
  }
#endif

#if ( HARDWAREDEBUG_ENABLE_GPIO == ON )
  /******************************************************************************
   * @function CmdGpioRd
   *
   * @brief read a byte from the GPIO
   *
   * This function will read a GPIO pin
   *
   * @param[in]   nCmdEnum    command enumeration
   *
   * @return      appropriate status
   *
   *****************************************************************************/
  static ASCCMDSTS CmdGpioRd( U8 nCmdEnum )
  {
    PC8       pcBuffer;
    U32UN     tValue;
    BOOL      bState;
    GPIOERR   eError;

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // get the arguments
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );

    // now do it
    if (( eError = Gpio_Get( tValue.anValue[ LE_U32_LSB_IDX ], &bState )) != GPIO_ERR_NONE )
    {
      // report error
      SPRINTF_P( pcBuffer, ( PCC8 )g_szAsciiErrStrn, eError, eError );
    }
    else
    {
      // output the read value
      SPRINTF_P( pcBuffer, ( PCC8 )szRspRdGpio, tValue.anValue[ LE_U32_LSB_IDX ], bState );
    }

    // return the status
    return( ASCCMD_STS_OUTPUTBUFFER );
  }
   
  /******************************************************************************
   * @function CmdGpioWr
   *
   * @brief write the gpio
   *
   * This function will write the GPIO
   *
   * @param[in]   nCmdEnum    command enumerator
   *
   * @return      appropriate status
   *
   *****************************************************************************/
  static ASCCMDSTS CmdGpioWr( U8 nCmdEnum )
  {
    ASCCMDSTS   eStatus = ASCCMD_STS_NONE;
    PC8         pcBuffer;
    U32UN       tValue;
    GPIOERR     eError;
    GPIOPINENUM ePin;

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // get the arguments
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );
    ePin = ( GPIOPINENUM )tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 1, &tValue.uValue );

    // now do it
    if (( eError = Gpio_Set( ePin, tValue.anValue[ LE_U32_LSB_IDX ] )) != GPIO_ERR_NONE )
    {
      // report error
      SPRINTF_P( pcBuffer, ( PCC8 )g_szAsciiErrStrn, eError, eError );
      eStatus = ASCCMD_STS_OUTPUTBUFFER;
    }

    // return the status
    return( eStatus );
  }

  ///******************************************************************************
  // * @function CmdGpioRdIrq
  // *
  // * @brief read a byte from the GPIO IRQ
  // *
  // * This function will read a GPIO pin
  // *
  // * @param[in]   nCmdEnum    command enumeration
  // *
  // * @return      appropriate status
  // *
  // *****************************************************************************/
  //static ASCCMDSTS CmdGpioRdIrq( U8 nCmdEnum )
  //{
  //  PC8       pcBuffer;
  //  U32UN     tValue;
  //  BOOL      bState;
  //  GPIOERR   eError;

  //  // get a pointer to the buffer
  //  AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

  //  // get the arguments
  //  AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );

  //  // now do it
  //  if (( eError = Gpio_GetIrq( tValue.anValue[ LE_U32_LSB_IDX ], &bState )) != GPIO_ERR_NONE )
  //  {
  //    // report error
  //    SPRINTF_P( pcBuffer, ( PCC8 )g_szAsciiErrStrn, eError, eError );
  //  }
  //  else
  //  {
  //    // output the read value
  //    SPRINTF_P( pcBuffer, ( PCC8 )szRspRdGpio, tValue.anValue[ LE_U32_LSB_IDX ], bState );
  //  }

  //  // return the status
  //  return( ASCCMD_STS_OUTPUTBUFFER );
  //}
#endif

#if ( HARDWAREDEBUG_ENABLE_I2C == ON )
  /******************************************************************************
   * @function CmdI2cRd
   *
   * @brief read a byte from the I2c
   *
   * This function will read a byte from an I2c device at a given location
   *
   * @param[in]   nCmdEnum    command enumeration
   *
   * @return      appropriate status
   *
   *****************************************************************************/
  static ASCCMDSTS CmdI2cRd( U8 nCmdEnum )
  {
    PC8         pcBuffer;
    U32UN       tValue;
    U8          nDevice, nAddress, nData;
    I2CDEVENUM  eDevEnum;
    I2CXFRCTL   tXfrCtl;
    I2CERROR    eI2cErr;

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // get the arguments
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );
    eDevEnum = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 1, &tValue.uValue );
    nDevice = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 2, &tValue.uValue );
    nAddress = tValue.anValue[ LE_U32_LSB_IDX ];

    // now read it
    tXfrCtl.nDevAddr        = nDevice;
    tXfrCtl.nAddrLen        = 1;
    tXfrCtl.tAddress.uValue = nAddress;
    tXfrCtl.pnData          = &nData;
    tXfrCtl.wDataLen        = 1;
    tXfrCtl.uTimeout        = 100;

    // now do it
    if (( eI2cErr = I2c_Read( eDevEnum, &tXfrCtl )) != I2C_ERROR_NONE )
    {
      // report error
      SPRINTF_P( pcBuffer, ( PCC8 )g_szAsciiErrStrn, eI2cErr, eI2cErr );
    }
    else
    {
      // output the read value
      SPRINTF_P( pcBuffer, ( PCC8 )szRspRdI2c, nDevice, nAddress, nData );
    }

    // return the status
    return( ASCCMD_STS_OUTPUTBUFFER );
  }
   
  /******************************************************************************
   * @function CmdI2cWr
   *
   * @brief write a byte to the I2c device
   *
   * This function will write a byte to an I2C device at a given location
   *
   * @param[in]   nCmdEnum    command enumeration
   *
   * @return      appropriate status
   *
   *****************************************************************************/
  static ASCCMDSTS CmdI2cWr( U8 nCmdEnum )
  {
    ASCCMDSTS   eStatus = ASCCMD_STS_NONE;
    PC8         pcBuffer;
    U32UN       tValue;
    U8          nDevice, nAddress, nData;
    I2CDEVENUM  eDevEnum;
    I2CXFRCTL   tXfrCtl;
    I2CERROR    eI2cErr;

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // get the arguments
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );
    eDevEnum = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 1, &tValue.uValue );
    nDevice = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 2, &tValue.uValue );
    nAddress = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 3, &tValue.uValue );
    nData = tValue.anValue[ LE_U32_LSB_IDX ];

    // now read it
    tXfrCtl.nDevAddr        = nDevice;
    tXfrCtl.nAddrLen        = 1;
    tXfrCtl.tAddress.uValue = nAddress;
    tXfrCtl.pnData          = &nData;
    tXfrCtl.wDataLen        = 1;
    tXfrCtl.uTimeout        = 100;

    // now do it
    if (( eI2cErr = I2c_Write( eDevEnum, &tXfrCtl )) != I2C_ERROR_NONE )
    {
      // report error
      SPRINTF_P( pcBuffer, ( PCC8 )g_szAsciiErrStrn, eI2cErr, eI2cErr );
      eStatus = ASCCMD_STS_OUTPUTBUFFER;
    }

    // return the status
    return( eStatus );
  }
   
  /******************************************************************************
   * @function CmdI2cDmp
   *
   * @brief dump an I2c device
   *
   * This function will dump a block of registers read from an I2C device
   *
   * @param[in]   nCmdEnum    command enumeration
   *
   * @return      appropriate status
   *
   *****************************************************************************/
  static ASCCMDSTS CmdI2cDmp( U8 nCmdEnum )
  {
    ASCCMDSTS   eStatus = ASCCMD_STS_OUTPUTBUFFER;
    PC8         pcBuffer;
    U32UN       tValue;
    U8          nDevice, nAddress, nLength;
    PU8         pnData;
    I2CDEVENUM  eDevEnum;
    I2CXFRCTL   tXfrCtl;
    I2CERROR    eI2cErr;

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // get the arguments
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );
    eDevEnum = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 1, &tValue.uValue );
    nDevice = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 2, &tValue.uValue );
    nAddress = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 3, &tValue.uValue );
    nLength = tValue.anValue[ LE_U32_LSB_IDX ];

    // allocate the data block
    if (( pnData = malloc( nLength )) != NULL )
    {
      // now read it
      tXfrCtl.nDevAddr        = nDevice;
      tXfrCtl.nAddrLen        = 1;
      tXfrCtl.tAddress.uValue = nAddress;
      tXfrCtl.pnData          = pnData;
      tXfrCtl.wDataLen        = nLength;
      tXfrCtl.uTimeout        = 1000;

      // now do it
      if (( eI2cErr = I2c_Read( eDevEnum, &tXfrCtl )) != I2C_ERROR_NONE )
      {
        // report error
        SPRINTF_P( pcBuffer, ( PCC8 )g_szAsciiErrStrn, eI2cErr, eI2cErr );
      }
      else
      {
        // display the block
        AsciiCommandHandler_DisplayBlock( nCmdEnum, nAddress, pnData, nLength );
        eStatus = ASCCMD_STS_NONE;
      }

      // free the data buffer
      free( pnData );
    }
    else
    {
      // report error
      SPRINTF_P( pcBuffer, ( PCC8 )g_szAsciiErrStrn, 0xBA, 0xBA );
    }

    // return the status
    return( eStatus );
  }

  /******************************************************************************
   * @function CmdI2cPoll
   *
   * @brief poll an I2c device
   *
   * This function will poll an I2C device
   *
   * @param[in]   nCmdEnum    command enumeration
   *
   * @return      appropriate status
   *
   *****************************************************************************/
  static ASCCMDSTS CmdI2cPoll( U8 nCmdEnum )
  {
    ASCCMDSTS   eStatus = ASCCMD_STS_NONE;
    PC8         pcBuffer;
    U32UN       tValue;
    U8          nDevice;
    I2CDEVENUM  eDevEnum;
    I2CERROR    eI2cErr;
    I2CCHKBSY   tChkBusy;

    // get a pointer to the buffer
    AsciiCommandHandler_GetBuffer( nCmdEnum, &pcBuffer );

    // get the arguments
    AsciiCommandHandler_GetValue( nCmdEnum, 0, &tValue.uValue );
    eDevEnum = tValue.anValue[ LE_U32_LSB_IDX ];
    AsciiCommandHandler_GetValue( nCmdEnum, 1, &tValue.uValue );
    nDevice = tValue.anValue[ LE_U32_LSB_IDX ];

    // check for device presence
    tChkBusy.nDevAddr = nDevice;
    tChkBusy.bReadMode = FALSE;
    if (( eI2cErr = I2c_Ioctl( eDevEnum, I2C_ACTION_POLL_DEVICE, ( PVOID )&tChkBusy )) != I2C_ERROR_NONE )
    {
      // report error
      SPRINTF_P( pcBuffer, ( PCC8 )g_szAsciiErrStrn, eI2cErr, eI2cErr );
      eStatus = ASCCMD_STS_OUTPUTBUFFER;
    }

    // return the status
    return( eStatus );
  }
#endif
 
 
/**@} EOF HardwareDebug.c */
