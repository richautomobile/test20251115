/**
 * \file
 *
 * \brief Brief description of the source file.
 *
 * Provide a detailed description of the source file.
 *
 * Project(s): PSOC4_Brifge_Board_App PSOC4_BRIDGE_BOARD
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
#include "DevINA236.h"
#include "i2cm.h"


#ifdef EN_DEV_INA236
/*
 *
Address Register, Name, Register Size (bits), Reset Value
0h,  Configuration Register, 16, 4127h
1h,  Shunt Voltage Register, 16, 0000h
2h,  Bus Voltage Register, 16, 0000h
3h,  Power Register, 16, 0000h
4h,  Current Register, 16, 0000h
5h,  Calibration Register, 16, 0000h
6h,  Mask/Enable Register, 16, 0000h
7h,  Alert Limit Register, 16, 0000h
3Eh, Manufacturer ID Register, 16, 5449h
3Fh, Device ID Register, 16, A080h
 */

/* ==[ Macros ]============================================================= */
/*FAST MODE:400KHZ,  HIGH-SPEED MODE:2940KHz*/

/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/*
INA236_VoutNormal        =>  0x40, 5m ohm
INA236_VoutQuie            =>  0x41, 499 ohm
INA236_VoutCrest           =>  0x42, 50m ohm
INA236_Vout2Normal      =>  0x43, 5m ohm
*/
uint8_t DevAddr7b[4] = {0x40, 0x41, 0x42, 0x43}; /*INA236A:0x40, INA236B:0x48*/
uint16_t ShuntOhm[4] = {5, 499, 50, 5};          /*mohm(mA), ohm(uA), mohm(mA), mohm(mA)*/
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]======================= */
/* \brief
 *
 * \Parameters [in] 
 * \Parameters [in] 
 * \return          t_FUNC_RETURN:
 *
 */
t_FUNC_RETURN INA236_SetI2cAddr(uint8_t cp_idx, uint8_t setDevAddr7b)
{
    DevAddr7b[cp_idx] = setDevAddr7b;

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN INA236_WriteI2C(uint8_t cp_idx, uint8_t reg, uint16_t wData)
{
    uint8_t WBuff[3] = {reg, (uint8_t)(wData >> 8U), (uint8_t)wData};

    return I2CMB_Write(DevAddr7b[cp_idx], WBuff, 3);
}

t_FUNC_RETURN INA236_ReadI2C(uint8_t cp_idx, uint8_t reg, uint16 *readData)
{
    uint8_t WBuff[1] = {reg};
    uint8_t RBuff[2];
    t_FUNC_RETURN rtn_State;

    rtn_State = I2CMB_Read(DevAddr7b[cp_idx], WBuff, 1, RBuff, 2);

    if(rtn_State == FUNC_RETURN_OK)
    {
        *readData = (uint16_t)(RBuff[1] | (RBuff[0] << 8U));
    }
    else
    {
        *readData = 0xFFFFU;
    }

    return rtn_State;
}

t_MODULE_POWER INA236_GetCurrent(uint8_t cp_idx)
{
    t_FUNC_RETURN rtn_State;
    uint16_t configVal;
    uint16_t rawCurrent;
    uint16_t rawVoltage;
    t_MODULE_POWER modulePower;

    /* configurartion register 0x00*/
    rtn_State = INA236_ReadI2C(cp_idx, 0x03U, &configVal);
    if( rtn_State == FUNC_RETURN_OK )
    {
        modulePower.dataExisted = true;
        if( configVal != 0x4727U )
        {
            /*average 64*/
            rtn_State = INA236_WriteI2C(cp_idx, 0x00U, 0x4727U);        
        }
        
        /* step 6 : Get Shunt Voltage Register to cal current. (ADCRANGE = 0), 2.5 uV/LSB*/
        rtn_State = INA236_ReadI2C(cp_idx, 0x01U, &rawCurrent);
    
        if((rawCurrent & 0x8000U) > 0U) /*ABS*/
        {
            rawCurrent = ~rawCurrent;
            rawCurrent += 1U;
        }
        /*2.5uV/LSB*/
        modulePower.processedCurrent = (uint16_t)((rawCurrent * 25)/10/ ShuntOhm[cp_idx]); /*normal(0,2,3)=mA, Quie(1)=uA */
    
        /*Get Bus Voltage Register. 1.6 mV/LSB*/
        rtn_State = INA236_ReadI2C(cp_idx, 0x02U, &rawVoltage);
        modulePower.processedVoltage = (uint16_t)((rawVoltage * 16)/1000); /*unit=0.1V*/
    }
    else
    {
        modulePower.dataExisted = false;
        modulePower.processedCurrent = 0xFFFFU;
        modulePower.processedVoltage = 0xFFFFU; 
    } 
    return modulePower;
}

#endif
