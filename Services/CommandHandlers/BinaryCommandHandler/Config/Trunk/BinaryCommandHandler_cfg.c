/******************************************************************************
 * @file BinaryCommandHandler_cfg.c
 *
 * @brief binary command handler configuration implementation
 *
 * This file provides the implementation for the binary command handler
 * configuration
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
 * $Log: $
 * 
 *
 * \addtogroup BinaryCommandHandler
 * @{
 *****************************************************************************/


// local includes -------------------------------------------------------------
#include "BinaryCommandHandler/BinaryCommandHandler.h"

// library includes'

// local parameter declarations -----------------------------------------------
/// declare the buffers here using the below mactros
// BINCMD_SNGBUF( name, bufsize );
// BINCMD_DBLBUF( name, xmtsize, rcvsize );
// BINCMD_MSVBUF( name, mstsize, slvsize )

// constant parameter initializations -----------------------------------------
const CODE  BINCMDDEF   g_atBinCmdDefs[ BINCMD_ENUM_MAX ] =
{
  // add entries to this table using the below macros
  // BINCMD_SLV_DEFSBPP( name, bufsize, chkmode, seqenb, writefunc, cmdtbl )
  // BINCMD_SLV_DEFDBPP( name, xmtsize, rcvsize, chkmode, seqenb, writefunc, cmdtbl )
  // BINCMD_SLV_DEFSBMD( name, bufsize, chkmode, seqenb, devadr, writefunc, cmdtbl )
  // BINCMD_SLV_DEFDBMD( name, xmtsize, rcvsize, chkmode, seqenb, devadr, writefunc, cmdtbl )
  #if ( BINCMDHAND_ENABLE_MASTERMODE == 1 )
  // BINCMD_MST_DEFSBPP( name, bufsize, chkmode, seqenb, numretry, writefunc, tbllen, cmdtbl )
  // BINCMD_MST_DEFDBPP( name, xmtsize, rcvsize, chkmode, seqenb, numretry, writefunc, tbllen, cmdtbl )
  // BINCMD_MST_DEFSBMD( name, bufsize, chkmode, seqenb, devadr, numretry, writefunc, tbllen, cmdtbl )
  // BINCMD_MST_DEFDBPP( name, xmtsize, rcvsize, chkmode, seqenb, devadr, numretry, writefunc, tbllen, cmdtbl )
  // BINCMD_MST_DEFDBMD( name, xmtsize, rcvsize, chkmode, seqenb, devadr, numretry, writefunc, tbllen, cmdtbl )
  // BINCMD_MSV_DEFDBPP( name, mstsize, slvsize, chkmode, seqenb, mstadr, numretry, writefunc, msttbllen, mstcmdtbl, slvcmdtbl )
  #endif // BINCMDHAND_ENABLE_MASTERMODE // add entries to this table using the below macros
 
};

/******************************************************************************
 * @function BinaryCommandHandler_GetInitialValue
 *
 * @brief get initial value
 *
 * This function will return the initial value for a CRC 16 calculation
 *
 * @return      initial value for CRC calculation
 *
 *****************************************************************************/
U16 BinaryCommandHandler_GetInitialValue(void)
{

}

/******************************************************************************
 * @function BinaryCommandHandler_ComputeCrcByte
 *
 * @brief compute CRC byte
 *
 * This function will compute a CRC byte
 *
 * @param[in]   wCurrentCrc     current CRC
 * @param[in]   nData           new data byte
 *
 * @return      computed crc
 *
 *****************************************************************************/
U16 BinaryCommandHandler_ComputeCrcByte(U16 wCurrentCrc, U8 nData)
{
  U16 wNewCrc;

  // return the new CRC
  return( wNewCrc );
}

/**@} EOF BinaryCommandHandler_cfg.h.c */
