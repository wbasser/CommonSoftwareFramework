/*****************************************************************************
 * @file LedManager_cfg.c
 *
 * @brief LED manager configuration implementation 
 *
 * This file provides the implementation for the LED manager configuration
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
 * $Log: $
 * 
 *
 * \addtogroup LedManager
 * @{
 *****************************************************************************/

// local includes -------------------------------------------------------------
#include "LedManager/LedManager.h"

// library includes -----------------------------------------------------------
#include "GPIO/Gpio.h"

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  // OS includes
  #include "FreeRtos.h"
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  #include <zephyr.h>
#endif // SYSTEMDEFINE_OS_SELECTION

// Macros and Defines ---------------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  #define LED_ANIMATE_TASK_PRIORITY             ( tskIDLE_PRIORITY + 4 )
  #define LED_SCAN_TASK_PRIORITY                ( tskIDLE_PRIORITY + 4 )
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  #define LED_ANIMATE_STACK_SIZE                ( 384 )
  #define LED_SCAN_STACK_SIZE                   ( 256 )
  #define LED_ANIMATE_TASK_PRIORITY             ( SYSTEMDEFINE_PRIORITY_LEDANIMATE )
  #define LED_SCAN_TASK_PRIORITY                ( -2 )
#endif // SYSTEMDEFINE_OS_SELECTION

// local parameter declarations
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  // define the trhead stack
  K_THREAD_STACK_DEFINE( tAnimateThreadStack, LED_ANIMATE_STACK_SIZE );
  static struct k_thread tAnimateThreadData;
  static k_tid_t tAnimateThreadId;
  #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
  #endif  // MATRIX DEFS
#endif // SYSTEMDEFINE_OS_SELECTION

// local function prototypes --------------------------------------------------
#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  static  void  AnimateTask( PVOID pvParameters );
  #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
    static  void  ScanTask( PVOID pvParameters );
  #endif  // MATRIX DEFS
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  static  void  AnimateThread( PVOID pvParam1, PVOID pvParam2, PVOID pvParam3 );
  #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
    static  void  ScanTask( PVOID pvParam1, PVOID pvParam2, PVOID pvParam3 );
  #endif  // MATRIX DEFS
#endif // SYSTEMDEFINE_OS_SELECTION

// constant parameter initializations -----------------------------------------
/// declare the led definitions
const CODE LEDDEF  g_atLedDefs[ LEDMANAGER_ENUM_MAX ] = 
{
  // populate using one or both of the helper macros
  // LEDDEF_DIRECT( pin )
  // LEDDEF_MATRIX( row, col )
  // LEDDEF_SPECIAL( func, col )
  // LEDDEF_DIMMED( pwm )
  LEDDEF_DIRECT( GPIO_PIN_ENUM_STSLED ),
  //LEDDEF_DIRECT( GPIO_PIN_ENUM_WIFLED ),
};

#if ( LEDMANAGER_RGB_LEDS_ENABLED == ON )
  const CODE LEDRGBDEF  g_atLedRgbDefs[ LEDMANAGER_RGBENUM_MAX ] =
  {
    // popuate usinig the macro below
    // LEDMNGRRGBDEF( red, grn, blu )
    //LEDMNGRRGBDEF( LEDMANAGER_ENUM_LD1RED, LEDMANAGER_ENUM_LD1GRN, LEDMANAGER_ENUM_LD1BLU ),
    //LEDMNGRRGBDEF( LEDMANAGER_ENUM_LD2RED, LEDMANAGER_ENUM_LD2GRN, LEDMANAGER_ENUM_LD2BLU ),
  };
#endif  // LEDMANAGER_RGB_LEDS_ENABLED

/// fill the matrix row columns 
#if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
  const CODE GPIOPINENUM g_aeLedMatrixRows[ LEDMANAGER_MATRIX_MAX_NUM_ROWS ] =
  {
  };

  const CODE GPIOPINENUM g_aeLedMatrixCols[ LEDMANAGER_MATRIX_MAX_NUM_COLS ] =
  {
  };
#endif  // MATRIX DEFS

/// define each sequence here
/// LEDMNGRDEFSTART( name )
/// LEDMNGRDEFSNGSENTRY,
/// LEDMNGRDEFSNGSTEP( action, led, duration, nextevent, option )
/// LEDMNGRDEFRGBSTEP( action, led, color, duration, nextevent, optred, optblu, optgrn )

#if ( LEDMANAGER_7SEG_LEDS_ENABLED == ON )
  const CODE LED7SEGDEF g_atLeds7SegDef[ LEDMNGR_7SEGENUM_MAX ] =
  {
    // populate using the macro below
    // LEDMNGR7SEGDEF( sega, segb, segc, segd, sege, segf, segg, segh, segdp )
  };
#endif // LEDMANAGER_7SEG_LEDS_ENABLED

/// declare the animation definitions
const CODE PLEDSEQENTRY g_apLedAnimationsDef[ LEDMNGR_ANIMATION_MAX ] =
{
  /// fill animations here using the below helper
  /// LEDMNGRANIMATIONDEF( name )]]
};

/******************************************************************************
 * @function LedManager_LocalInitialize
 *
 * @brief local initialization
 *
 * This function will perform any custom initialization
 *
 * @return  TRUE if errors, FALSE if none
 *
 *****************************************************************************/
BOOL LedManager_LocalInitialize( void )
{
  // create tasks
  #if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
    // create the animation task
    xTaskCreate( AnimateTask, "LedAnimation", configMINIMAL_STACK_SIZE, NULL, LED_ANIMATE_TASK_PRIORITY, NULL );
    #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
      xTaskCreate( ScanTask, "LedMatrixScan", configMINIMAL_STACK_SIZE, NULL, LED_SCAN_TASK_PRIORITY, NULL );
    #endif  // MATRIX DEFS
  #elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
    // create the animation task
    tAnimateThreadId = k_thread_create( &tAnimateThreadData, 
                                        tAnimateThreadStack, 
                                        K_THREAD_STACK_SIZEOF( tAnimateThreadStack), 
                                        AnimateThread, 
                                        NULL, NULL, NULL, 
                                        LED_ANIMATE_TASK_PRIORITY, 0, K_NO_WAIT
                                      );
  #endif // SYSTEMDEFINE_OS_SELECTION
}

/******************************************************************************
 * @function LedManager_GpioSet
 *
 * @brief set a GPIO pin
 *
 * This function will set the state of a GPIO pin
 *
 * @param[in]   nPinEnum    pin enumeratin
 * @param[in]   bState      state of the pin
 *
 *****************************************************************************/
void LedManager_GpioSet( U8 nPinEnum, BOOL bState )
{
  // call the GpIO handler
  Gpio_Set( nPinEnum, bState );
}

#if ( LEDMANAGER_ENABLE_DIMMABLE_LEDS == 1 )
  /******************************************************************************
   * @function LedManager_PwmSet
   *
   * @brief set a PWM width
   *
   * This function will set the PWM to its approrpiate value
   *
   * @param[in]   nPwmEnum    pwm enumeratin
   * @param[in]   nIntnesity  pwm intensity
   *
   *****************************************************************************/
  void LedManager_PwmSet( U8 nPwmEnum, U8 nIntensity )
  {
  }
#endif // LEDMANAGER_ENABLE_DIMMABLE_LEDS

/******************************************************************************
 * @function LedManager_NotifyDone
 *
 * @brief animation done notification
 *
 * This function will be called to notify when an animation is done
 *
 *****************************************************************************/
void LedManager_NotifyDone( void )
{
}

#if ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_FREERTOS )
  /******************************************************************************
   * @function AnimateTask
   *
   * @brief animation task
   *
   * This function is the animation task
   *
   * @param[in]   pvParameters  not used
   *
   *****************************************************************************/
  static void AnimateTask( PVOID pvParameters )
  {
    // main loop
    FOREVER
    {
      // call the default handler
      LedManager_ProcessAnimation( );

      // sleep for the animate rate
      vTaskDelay( LEDMANAGER_ANIMATE_RATE_MSECS / portTICK_RATE_MS );
    }
  }

  #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
    /******************************************************************************
     * @function ScanTask
     *
     * @brief scan task
     *
     * This function is the animation task
     *
     * @param[in]   pvParameters  not used
     *
     *****************************************************************************/
    static void ScanTask( PVOID pvParameters )
    {
      // main loop
      FOREVER
      {
        // call the default handler
        LedManager_ProcessScan( );

        // sleep for the animate rate
        vTaskDelay( LEDMANAGER_SCAN_RATE_MSEC / portTICK_RATE_MS );
      }
    }
  #endif  // MATRIX DEFS
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_TASKMANAGER )

/******************************************************************************
 * @function LedManager_AnimationTask
 *
 * @brief animation task
 *
 * This function will call the animation process
 *
 * @param[in]   xArg      task argument
 *
 * @return      TRUE      flush event
 *
 *****************************************************************************/
BOOL LedManager_AnimationTask( TASKARG xArg )
{
  // call the animation process
  LedManager_ProcessAnimation( );
  
  // return true
  return( TRUE );
}

  #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
    /******************************************************************************
     * @function LedManager_ScanTask
     *
     * @brief scan task
     *
     * This function will be call the scan task
     *
     * @param[in]   xArg      task argument
     *
     * @return      TRUE      flush event
     *
     *****************************************************************************/
    BOOL LedManager_ScanTask( TASKARG xArg )
    {
      // call the animation process
      LedManager_ProcessScan( );
      
      // return true
      return( TRUE );
    }
  #endif  // MATRIX DEFS
#elif ( SYSTEMDEFINE_OS_SELECTION == SYSTEMDEFINE_OS_ZEPHYR )
  /******************************************************************************
   * @function AnimateThread
   *
   * @brief animation thread
   *
   * This function is the animation task
   *
   * @param[in]   pvParam1      pointer to parameter #1
   * @param[in]   pvParam2      pointer to parameter #2
   * @param[in]   pvParam3      pointer to parameter #3
   *
   *****************************************************************************/
  static void AnimateThread( PVOID pvParam1, PVOID pvParam2, PVOID pvParam3 )
  {
    // main loop
    FOREVER
    {
      // call the default handler
      LedManager_ProcessAnimation( );

      // sleep for the animate rate
      k_msleep( LEDMANAGER_ANIMATE_RATE_MSECS );
    }
  }

  #if (( LEDMANAGER_MATRIX_MAX_NUM_ROWS != 0 ) && ( LEDMANAGER_MATRIX_MAX_NUM_COLS != 0 ))
    /******************************************************************************
     * @function ScanTask
     *
     * @brief scan task
     *
     * This function is the animation task
     *
     * @param[in]   pvParam1      pointer to parameter #1
     * @param[in]   pvParam2      pointer to parameter #2
     * @param[in]   pvParam3      pointer to parameter #3
     *
     *****************************************************************************/
    static void ScanTask( PVOID pvParam1, PVOID pvParam2, PVOID pvParam3 )
    {
      // main loop
      FOREVER
      {
        // call the default handler
        LedManager_ProcessScan( );

        // sleep for the animate rate
        vTaskDelay( LEDMANAGER_SCAN_RATE_MSEC / portTICK_RATE_MS );
      }
    }
  #endif  // MATRIX DEFS
#endif  // SYSTEMDEFINE_OS_SELECTION

/**@} EOF LedManager_cfg.c */
