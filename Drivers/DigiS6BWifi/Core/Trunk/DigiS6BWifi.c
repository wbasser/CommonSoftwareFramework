/******************************************************************************
 * @file DigiS6BWifi.c
 *
 * @brief DIGI S6B wifi driver implementation
 *
 * This file provides the implementation for the DIGI S6B wifi module
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
 * \addtogroup DigiS6BWifi
 * @{
 *****************************************************************************/

// system includes ------------------------------------------------------------

// local includes -------------------------------------------------------------
#include "DigiS6BWifi/DigiS6BWifi.h"

// library includes -----------------------------------------------------------

// Macros and Defines ---------------------------------------------------------

// enumerations ---------------------------------------------------------------

// structures -----------------------------------------------------------------

// global parameter declarations ----------------------------------------------

// local parameter declarations -----------------------------------------------

// local function prototypes --------------------------------------------------

// constant parameter initializations -----------------------------------------

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void DigiS6BWifi_Initialize( void )
{
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
BOOL DigiS6BWifi_ProcessChar( U8 nChar )
{
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
void DigiS6BWifi_Connect( PC8 pcSSID, PC8 pcPassword )
{
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
BOOL DigiS6BWifi_GetConnectionState( void )
{
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
PVDIGIWIFIHANDLE DigiS6BWifi_OpenSocket( IPADDR ptIpAddr, U16 wPort, DIGIPROTOCOLTYPE eProtoType )
{
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
DIGIS6BWIFIERR DigiS6BWifi_WriteSocket( PVDIGIWIFIHANDLE pvHandler, PU8 pnData, U16 wLength )
{
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
DIGIS6BWIFIERR DigiS6BWifi_ReadSocket( PVDIGIWIFIHANDLE pvHandler, PU8 pnData, U16 wLength )
{
}

/******************************************************************************
 * @function 
 *
 * @brief 
 *
 * This function 
 *
 * @param[in]   
 *
 * @return      
 *
 *****************************************************************************/
DIGIS6BWIFIERR DigiS6BWifi_CloseSocket( PVDIGIWIFIHANDLE pvHandler )
{
}

/**@} EOF DigiS6BWifi.c */
