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
#include <stdio.h>
#include "DevFPD983.h"
#include "i2cm.h"


#ifdef EN_DEV_FPD983
/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
static uint8_t Device_TX = 0x0C;
/* ==[ Declaration of functions with internal linkage ]===================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]======================= */
/* \brief
 *
 * ==================================
 * \Parameters [in] EncNo: Encoder Number. Encoder A=0,  Encoder B=1
 * \Parameters [in] dir:      direction.           CW=0,  CCW=1
 * \Parameters [in] Freq:    Encoder frequence. 20~1.25KHz
 * \Parameters [in] count:  gen. plus number
 * \return          t_FUNC_RETURN:
 * ===================================
 *
 */
t_FUNC_RETURN FPD983_SetI2cAddr(uint8_t DevAddr7b)
{
    Device_TX = DevAddr7b;

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN FPD983_WriteI2C(uint8_t reg, uint8_t wData)
{
    uint8_t WBuff[2] = {reg, wData};

    return I2CMA_Write(Device_TX, WBuff, 2);
}

uint8_t FPD983_ReadI2C(uint8_t reg)
{
    uint8_t WBuff[1] = {reg};
    uint8_t RBuff[1];
    t_FUNC_RETURN rtn_State;

    rtn_State = I2CMA_Read(Device_TX, WBuff, 1, RBuff, 1);

    if(rtn_State == FUNC_RETURN_OK)
    {
        return RBuff[0];
    }
    else
    {
        return 0xFFU;
    }
}

t_FUNC_RETURN FPD983_SubPageWrite(uint8_t page, uint8_t reg, uint8_t wData)
{
    t_FUNC_RETURN rtn_State;
    uint8_t reg_page = (uint8_t)(1 + 2 + page * 4);
    rtn_State = FPD983_WriteI2C(0x40U, reg_page);
    rtn_State |= FPD983_WriteI2C(0x41U, reg);
    rtn_State |= FPD983_WriteI2C(0x41U, wData);
    return rtn_State;
}

uint8_t FPD983_SubPageRead(uint8_t page, uint8_t reg)
{
    uint8_t reg_page = (uint8_t)(1 + 2 + page * 4);
    (void)FPD983_WriteI2C(0x40U, reg_page);
    (void)FPD983_WriteI2C(0x41U, reg);
    return FPD983_ReadI2C(0x42U);
}

uint32_t FPD983_APBRead(uint8_t vp_ch, uint16_t addr16b)
{
    uint8_t page = 0;
    uint8_t APB_CTL = 0x48;
    /*APB_AUTO_INC = { 'pos':2, 'wid':1}*/
    /*APB_READ = { 'pos':1, 'wid':1}*/
    /*APB_ENABLE = { 'pos':0, 'wid':1}*/
    uint8_t APB_ADR0 = 0x49;
    uint8_t APB_ADR1 = 0x4A;
    uint8_t APB_DATA0 = 0x4B;
    uint8_t APB_DATA1 = 0x4C;
    uint8_t APB_DATA2 = 0x4D;
    uint8_t APB_DATA3 = 0x4E;
    uint8_t apbData0;  /*low uint8_t*/
    uint8_t apbData1;
    uint8_t apbData2;
    uint8_t apbData3;

    /*983 DP RX is only one port.*/
    /*7:3 Selects target for register access: 0x0 = DisplayPort RX 0 APB registers*/
    if(vp_ch == 0)
    {
        page = 0x03;
    }
    else
    {
        page = 0x0B;
    }

    (void)FPD983_WriteI2C(APB_ADR0, (uint8_t)addr16b);
    (void)FPD983_WriteI2C(APB_ADR1, (uint8_t)(addr16b >> 8U));
    (void)FPD983_WriteI2C(APB_CTL, page);

    apbData0 = FPD983_ReadI2C(APB_DATA0);  /*low uint8_t*/
    apbData1 = FPD983_ReadI2C(APB_DATA1);
    apbData2 = FPD983_ReadI2C(APB_DATA2);
    apbData3 = FPD983_ReadI2C(APB_DATA3);

    return (((uint32_t)apbData3 << 24) | ((uint32_t)apbData2 << 16) | ((uint32_t)apbData1 << 8) | ((uint32_t)apbData0));
}

t_FUNC_RETURN FPD983_APBWrite(uint8_t vp_ch, uint16_t addr16b, uint32_t data32b)
{
    t_FUNC_RETURN rtn_State;
    uint8_t APB_CTL = 0x48;
    uint8_t APB_ADR0 = 0x49;
    uint8_t APB_ADR1 = 0x4A;
    uint8_t APB_DATA0 = 0x4B;
    uint8_t APB_DATA1 = 0x4C;
    uint8_t APB_DATA2 = 0x4D;
    uint8_t APB_DATA3 = 0x4E;
    uint8_t page = 0;

    /*983 DP RX is only one port.*/
    /*7:3 Selects target for register access: 0x0 = DisplayPort RX 0 APB registers*/
    if(vp_ch == 0)
    {
        page = 0x01;
    }
    else
    {
        page = 0x09;
    }

    rtn_State = FPD983_WriteI2C(APB_CTL, page);  /*switch channel*/

    rtn_State |= FPD983_WriteI2C(APB_ADR0, (uint8_t)addr16b);
    rtn_State |= FPD983_WriteI2C(APB_ADR1, (uint8_t)(addr16b >> 8));

    rtn_State |= FPD983_WriteI2C(APB_DATA0, (uint8_t)data32b);   /*low uint8_t*/
    rtn_State |= FPD983_WriteI2C(APB_DATA1, (uint8_t)(data32b >> 8));
    rtn_State |= FPD983_WriteI2C(APB_DATA2, (uint8_t)(data32b >> 16));
    rtn_State |= FPD983_WriteI2C(APB_DATA3, (uint8_t)(data32b >> 24)); /*hi uint8_t*/

    return rtn_State;
}


t_FUNC_RETURN FPD983_SetBistEnable(bool bistEnable)
{
    t_FUNC_RETURN rtn_State;

    if(bistEnable == false)
    {
        rtn_State = FPD983_WriteI2C(0x40, 0x30);
        rtn_State |= FPD983_WriteI2C(0x41, 0x28);
        rtn_State |= FPD983_WriteI2C(0x42, 0x00);
        /*this is for 34"*/
        rtn_State |= FPD983_WriteI2C(0x40, 0x30);
        rtn_State |= FPD983_WriteI2C(0x41, 0x68);
        rtn_State |= FPD983_WriteI2C(0x42, 0x00);
    }
    else
    {
        rtn_State = FPD983_WriteI2C(0x40, 0x30);
        rtn_State |= FPD983_WriteI2C(0x41, 0x28);
        rtn_State |= FPD983_WriteI2C(0x42, 0x95);
        /*this is for 34"*/
        rtn_State |= FPD983_WriteI2C(0x40, 0x30);
        rtn_State |= FPD983_WriteI2C(0x41, 0x68);
        rtn_State |= FPD983_WriteI2C(0x42, 0x95);
    }

    return rtn_State;
}

t_FUNC_RETURN FPD983_AutoScroll(uint8_t EnAutoScroll)
{
    t_FUNC_RETURN rtn_State;
    uint8_t rData;

    if(EnAutoScroll > 0U)
    {
        rtn_State =  FPD983_WriteI2C(0x40, 0x31);/*select VP0/1/2/3. page 0xC(0x30=(0x0C<<2))*/
        rtn_State |= FPD983_WriteI2C(0x41, 0x29);
        rData = FPD983_ReadI2C(0x42);
        rtn_State |= FPD983_WriteI2C(0x40, 0x30);
        rtn_State |= FPD983_WriteI2C(0x42, (uint8_t)(rData | 0x01));
        rtn_State |= FPD983_WriteI2C(0x40, 0x31);/*select VP0/1/2/3. page 0xC(0x30=(0x0C<<2))*/
        rtn_State |= FPD983_WriteI2C(0x41, 0x69);
        rData = FPD983_ReadI2C(0x42);
        rtn_State |= FPD983_WriteI2C(0x40, 0x30);
        rtn_State |= FPD983_WriteI2C(0x42, (uint8_t)(rData | 0x01));
    }
    else
    {
        rtn_State =  FPD983_WriteI2C(0x40, 0x31);/*select VP0/1/2/3. page 0xC(0x30=(0x0C<<2))*/
        rtn_State |= FPD983_WriteI2C(0x41, 0x29);
        rData = FPD983_ReadI2C(0x42);
        rtn_State |= FPD983_WriteI2C(0x40, 0x30);
        rtn_State |= FPD983_WriteI2C(0x42, (uint8_t)(rData & 0xFE));
        rtn_State |= FPD983_WriteI2C(0x40, 0x31);/*select VP0/1/2/3. page 0xC(0x30=(0x0C<<2))*/
        rtn_State |= FPD983_WriteI2C(0x41, 0x69);
        rData = FPD983_ReadI2C(0x42);
        rtn_State |= FPD983_WriteI2C(0x40, 0x30);
        rtn_State |= FPD983_WriteI2C(0x42, (uint8_t)(rData & 0xFE));
    }

    return rtn_State;
}

/* b7:b2
* 00000b= Checkerboard (White/Black)
* 00001b= White
* 00010b= Black
* 00011b= Red
* 00100b= Green
* 00101b= Blue
*
*/
t_FUNC_RETURN FPD983_SetPattern(uint8_t VP0PatNo, uint8_t VP1PatNo)
{
    t_FUNC_RETURN rtn_State;

    if((VP0PatNo > MIPITestPattern) || (VP1PatNo > MIPITestPattern))
    {
        rtn_State = FUNC_RETURN_NG;
    }
    else
    {
        rtn_State =  FPD983_WriteI2C(0x40, 0x30);
        rtn_State |= FPD983_WriteI2C(0x41, 0x28);
        rtn_State |= FPD983_WriteI2C(0x42, (uint8_t)((VP0PatNo << 3U) | 0x05));
        rtn_State |= FPD983_WriteI2C(0x41, 0x68);
        rtn_State |= FPD983_WriteI2C(0x42, (uint8_t)((VP1PatNo << 3U) | 0x05));
    }

    return rtn_State;
}

/*
*   10bits color : 0~1023
*
*/
t_FUNC_RETURN FPD983_SetPatternGrayLevel(uint16_t R, uint16_t G, uint16_t B)
{
    t_FUNC_RETURN rtn_State;

    uint8_t red0 = (uint8_t)(R);/* 0xFC #6-LSBs of red color (bits 7:2 used for 24-bit color)*/
    uint8_t red1 = (uint8_t)(R >> 8);/* 0x01 #2-MSBs of red color*/
    uint8_t green0 = (uint8_t)(G);/*0xFC #6-LSBs of green color (bits 7:2 used for 24-bit color)*/
    uint8_t green1 = (uint8_t)(G >> 8);/*0x01 #2-MSBs of green color*/
    uint8_t blue0 = (uint8_t)(B);/*0xFC#6-LSBs of blue color (bits 7:2 used for 24-bit color)*/
    uint8_t blue1 = (uint8_t)(B >> 8);/*0x01 #2-MSBs of blue color*/

    rtn_State =  FPD983_WriteI2C(0x41, 0x28);
    rtn_State |= FPD983_WriteI2C(0x42, 0x75); /*custom color*/

    rtn_State |= FPD983_WriteI2C(0x40, 0x30); /*select Page 12*/
    rtn_State |= FPD983_WriteI2C(0x41, 0x2A); /*PGIA on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, 0x80); /*PGIA on Page 12, set to 0x00 and auto-increment*/
    rtn_State |= FPD983_WriteI2C(0x41, 0x2B); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, red0); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, red1); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, green0); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, green1); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, blue0); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, blue1); /*PGID on Page 12*/
    /*================================================================*/
    rtn_State |= FPD983_WriteI2C(0x41, 0x68);
    rtn_State |= FPD983_WriteI2C(0x42, 0x75); /*custom color*/
    rtn_State |= FPD983_WriteI2C(0x41, 0x6A); /*PGIA on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, 0x80); /*PGIA on Page 12, set to 0x00 and auto-increment*/
    rtn_State |= FPD983_WriteI2C(0x41, 0x6B); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, red0); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, red1); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, green0); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, green1); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, blue0); /*PGID on Page 12*/
    rtn_State |= FPD983_WriteI2C(0x42, blue1); /*PGID on Page 12*/


    return rtn_State;

}

t_FUNC_RETURN FPD983_FullRST(void)
{
    uint8_t readData;
    uint8_t outCnt = 0;
    t_FUNC_RETURN rtn_state;

    rtn_state = FPD983_WriteI2C(0x1, 0x2); //full reset Ser

    if(rtn_state == FUNC_RETURN_NG)
    {
        return FUNC_RETURN_NG;
    }

    do
    {
        Cy_SysLib_Delay(10U);
        readData = FPD983_ReadI2C(0x01);
        outCnt++;
    }
    while(readData != 0 && outCnt < 100);

    if(outCnt >= 100)
    {
        return FUNC_RETURN_NG;
    }
    else
    {
        return FUNC_RETURN_OK;
    }

}

#endif
