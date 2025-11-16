/**
 * \file
 *
 * \brief Brief description of the source file.
 *
 * Provide a detailed description of the source file.
 *
 * Project(s): PSOC4_Brifge_Board_App xxx
 *
 *
 * Target: independent
 *
 * \author Firstname Lastname <firstname.lastname@innolux.com>
 *
 * \copyright (C) 2016 Innolux Display Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Innolux Display
 * Corporation.
 */


/** MISRA-C:2004 Deviation List
 *
 *  MISRA-1:
 *  Deviated Rule:
 *
 *  Reason:
 *
 */

/* ==[ Includes ]=========================================================== */
#include "DevTPS55289.h"
#include "i2cm.h"


#ifdef EN_OTP_TPS55289
/* ==[ Macros ]============================================================= */
#define OTP_Vmin  598U
#define OTP_Vmax  15597U
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
static uint8_t DeviceAddr_7bit = 0x75U;
/* ==[ Declaration of functions with internal linkage ]===================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]======================= */
/* \brief TPS55289_SetI2cAddr
 *
 * ==================================
 * \Parameters [in] DevAddr7b: default 0x75U
 * \return          t_FUNC_RETURN:
 * ===================================
 */
t_FUNC_RETURN TPS55289_SetI2cAddr(uint8_t DevAddr7b)
{
    DeviceAddr_7bit = DevAddr7b;

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN TPS55289_WriteI2C(uint8_t reg, uint8_t wData)
{
    uint8_t WBuff[2] = {reg, wData};

    return I2CMA_Write(DeviceAddr_7bit, WBuff, 2);
}

uint8_t TPS55289_ReadI2C(uint8_t reg)
{
    uint8_t WBuff[1] = {reg};
    uint8_t RBuff[1];
    t_FUNC_RETURN rtn_State;

    rtn_State = I2CMA_Read(DeviceAddr_7bit, WBuff, 1, RBuff, 1);

    if(rtn_State == FUNC_RETURN_OK)
    {
        return RBuff[0];
    }
    else
    {
        return 0xFFU;
    }
}

/* \brief TPS55289_SetOTPVolt
 *
 *  INTFB = 0.0752, VREF = (45mV+0x564mV*Reg(0:1))  =>  Vout= VREF/INTFB
 *  Vstep = 0.5645mV/0.0752 = 7.5mV, Vstart=45mV/0.0752=598mV 
 *  Vmax_out=15.957, Vmin_out=598mV
 * ==================================
 * \Parameters [in] void
 * \return          t_FUNC_RETURN:
 * ===================================
 */
t_FUNC_RETURN TPS55289_SetOTPVolt(uint16_t mV)
{
    uint8_t RegVal;
    uint16_t vref;
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
    
    /*Step1: Cehck INTFB=0x0752*/
    RegVal=TPS55289_ReadI2C(0x04U);
    if( RegVal != 0x02U )
    {
        TPS55289_WriteI2C(0x04U, 0x02U);
    }
    
    /*Step2: Setting voltage and output enable.*/
    if( (mV>=OTP_Vmin) && (mV<OTP_Vmax) )
    {   
      /*Vstep = 7.5mV*/
      vref = (((mV - OTP_Vmin)*10U)/75U);    
      /*The output voltage are more than 0.19v. 0.19/7.5mV=25 */ 
      if( vref >= 25 )
          vref = (vref - 25U);      
      TPS55289_WriteI2C(0x00U, (uint8_t)vref);
      TPS55289_WriteI2C(0x01U, (uint8_t)(vref>>8));  

      /**/
      TPS55289_OE(1U);
      rtn_state = FUNC_RETURN_OK;
    }

    return rtn_state;
}

t_FUNC_RETURN TPS55289_OE(uint8_t OnOff)
{
    if( OnOff > 0U )
    {
      TPS55289_WriteI2C(0x06U, 0xA0U);      
    }
    else
    {
       TPS55289_WriteI2C(0x06U, 0x20U);    
    }

    return FUNC_RETURN_OK;
}

#endif
