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
#include "gm_vcs.h"
#include "i2cm.h"
#include "crc.h"

#ifdef EN_GM_VCS_API
/* ==[ Macros ]============================================================= */
#define MCU_ADDR   0x12U   /*7bits address*/
#define NOVA_TCON_ADDR  0x60U   /*7bits address*/
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static t_FUNC_RETURN DiagnosticCRC8(uint16_t idx, uint8_t dat[]);
/* ==[ Definition of functions with internal linkage ]======================= */
static t_FUNC_RETURN DiagnosticCRC8(uint16_t idx, uint8_t dat[])
{
    t_FUNC_RETURN status = FUNC_RETURN_OK;
    uint8_t tmpD[8];
    uint32_t tmpCRC = 0;

    switch(idx)
    {
        case 1:
            tmpD[0] = dat[1];
            tmpD[1] = dat[2];
            tmpD[2] = dat[3];
            tmpD[3] = dat[4];
            tmpD[4] = dat[5];
            Crc_CalCrc8(tmpD, 5, &tmpCRC);

            if(((uint8_t)tmpCRC) == dat[0])
            {
                status = FUNC_RETURN_OK;
            }
            else
            {
                printf("Diag1 Error: Tran=0x%2X, Cal=0x%2X\r\n", dat[0], (uint8_t)tmpCRC);
                status = FUNC_RETURN_NG;
            }

            break;

        case 2:
            tmpD[0] = dat[1];
            tmpD[1] = dat[2];
            tmpD[2] = dat[3];
            Crc_CalCrc8(tmpD, 3, &tmpCRC);

            if(((uint8_t)tmpCRC) == dat[0])
            {
                status = FUNC_RETURN_OK;
            }
            else
            {
                printf("Diag2 Error: Tran=0x%2X, Cal=0x%2X\r\n", dat[0], (uint8_t)tmpCRC);
                status = FUNC_RETURN_NG;
            }

            break;

        default:
            break;
    }

    return status;
}

/* ==[ Definition of functions with external linkage ]====================== */
t_FUNC_RETURN GmVCS_GetGmCMD(uint8_t cmdID, uint8_t rLen, uint8_t rBuff[])
{
    t_FUNC_RETURN result = FUNC_RETURN_OK;
    uint8_t WriteBuf[2];
    uint8_t ReadBuf[10];

    WriteBuf[0] = cmdID;
    result = I2CMA_Read(MCU_ADDR, WriteBuf, 1U, ReadBuf, rLen);

    if(result == FUNC_RETURN_OK)
    {
        for(int i = 0; i < rLen; i++)
        {
            rBuff[i] = ReadBuf[i];
        }
    }

    return  result;
}

t_FUNC_RETURN GmVCS_MCURst(void)
{
    t_FUNC_RETURN status;
    uint8_t WriteBuf[2] = { 0x31, 0x02 };
    status = I2CMA_Write(MCU_ADDR, WriteBuf, 2U);
    return status;
}

/*
*
*    Display ID =>
*    VCS34" = 0x08
*/
t_FUNC_RETURN GmVCS_SetDimm(uint8_t DisplayID, uint16_t PWM)
{
    t_FUNC_RETURN status;
    uint32_t CRC;
    uint8_t WriteBuf[6];
    static uint8_t BlPwmCmdCnt = 0xF0U;

    WriteBuf[0] = 0x20U;
    WriteBuf[1] = 0U;
    WriteBuf[2] = DisplayID;
    WriteBuf[3] = (uint8_t)(PWM >> 8);
    WriteBuf[4] = (uint8_t)PWM;
    WriteBuf[5] = BlPwmCmdCnt;

    /*cal crc8*/
    Crc_CalCrc8(WriteBuf + 2U, 4, &CRC);
    WriteBuf[1] = (uint8_t)CRC;

    BlPwmCmdCnt++;

    if((BlPwmCmdCnt & 0x0FU) == 0x0FU)
    {
        BlPwmCmdCnt = 0xF0U;
    }

    status = I2CMA_Write(MCU_ADDR, WriteBuf, 6U);
    return status;
}

t_FUNC_RETURN GmVCS_GetDimm(uint8_t DisplayID, uint16_t *pwm)
{
    t_FUNC_RETURN STATUS = FUNC_RETURN_OK;
    uint8_t ReadBuf[5];
    uint32_t crc;

    STATUS = GmVCS_GetGmCMD(Display_Backlight_PWM, 5U, ReadBuf);

    if(STATUS == FUNC_RETURN_OK)
    {
        Crc_CalCrc8(ReadBuf + 1U, 4, &crc);

        if((crc == ReadBuf[0]) && (ReadBuf[1] == DisplayID))
        {
            *pwm = (uint16_t)(ReadBuf[3] + (ReadBuf[2] << 8));
            STATUS = FUNC_RETURN_OK;
        }
        else
        {
            *pwm = 0U;
            STATUS = FUNC_RETURN_NG;
        }
    }

    return STATUS;

}

t_FUNC_RETURN GmVCS_GetDiag(uint8_t *Diagnostic1, uint8_t *Diagnostic2)
{
    t_FUNC_RETURN result = FUNC_RETURN_OK;
    uint8_t Diag1[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    uint8_t Diag2[4] = { 0xff, 0xff, 0xff, 0xff };
    uint8_t irqReadCnt = 0U;

    while(irqReadCnt < 10)
    {
        irqReadCnt++;
        (void)GmVCS_GetGmCMD(NewDiagnostic_Status1, 6U, Diag1);
        (void)GmVCS_GetGmCMD(NewDiagnostic_Status2, 4U, Diag2);

        result =  DiagnosticCRC8(1, Diag1);
        result |=  DiagnosticCRC8(2, Diag2);
        printf("Diag1=0x%2X, Diag2=0x%2X\r\n", Diag1[3], Diag2[2]);

        if(result == FUNC_RETURN_NG)
        {
            *Diagnostic1 = 0xFFU;
            *Diagnostic2 = 0xFFU;
            
            printf("Diag CRC ERROR\r\n");
            break;
        }
        else
        {
            if((Diag1[3] == 0 && Diag2[2] == 0))
            {
                *Diagnostic1 = Diag1[3];
                *Diagnostic2 = Diag2[2];
                break;
            }
            else
            {
                Cy_SysLib_Delay(100U);
            }
        }
    }

    return result;
}


t_FUNC_RETURN GmVCS_ForceGetDiag(uint8_t *Diagnostic1, uint8_t *Diagnostic2)
{
    t_FUNC_RETURN result = FUNC_RETURN_OK;
    uint8_t Diag1[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    uint8_t Diag2[4] = { 0xff, 0xff, 0xff, 0xff };

    (void)GmVCS_GetGmCMD(NewDiagnostic_Status1, 6U, Diag1);
    (void)GmVCS_GetGmCMD(NewDiagnostic_Status2, 4U, Diag2);

    result =  DiagnosticCRC8(1, Diag1);
    result |= DiagnosticCRC8(2, Diag2);

    if(result == FUNC_RETURN_OK)
    {
        *Diagnostic1 = Diag1[3];
        *Diagnostic2 = Diag2[2];
        ////printf("Diag1=0x%2X, Diag2=0x%2X\r\n", Diag1[3], Diag2[2]);

    }
    else
    {
        *Diagnostic1 = 0xFFU;
        *Diagnostic2 = 0xFFU;

    }

    return result;
}

uint16_t GmVCS_GetResetCnt(void)
{
    uint8_t ReadBuf[3];
    GmVCS_GetGmCMD(ResetCntDAddr, 2, ReadBuf);
    return (uint16_t)(ReadBuf[0] + (ReadBuf[1] << 8));
}

uint16_t GmVCS_GetPwrCnt(void)
{
    uint8_t ReadBuf[3];
    GmVCS_GetGmCMD(PwrCntDAddr, 2, ReadBuf);
    return (uint16_t)(ReadBuf[0] + (ReadBuf[1] << 8));
}

/*===============Start eng. mode API================*/
/*******************************************************************************
 * \brief Function Name: GmVCS_EngMode
 *******************************************************************************
 *
 * Summary:
 *
 * Parameters:
 * uint8_t
 *
 * Return:
 * t_FUNC_RETURN -
 *
 ******************************************************************************/
t_FUNC_RETURN GmVCS_EngMode(uint8_t OnOff)
{
    t_FUNC_RETURN status;
    uint8_t WriteBuf[5];

    if(OnOff > 0U)
    {
        WriteBuf[0] = 0xFE;
        WriteBuf[1] = 0xFF;
    }
    else
    {
        WriteBuf[0] = 0xFE;
        WriteBuf[1] = 0xFE;
    }

    status = I2CMA_Write(MCU_ADDR, WriteBuf, 2U);

    return status;
}


t_FUNC_RETURN GmVCS_WriteFactory(uint8_t SAddr, uint8_t WLen, uint8_t WData[])
{
    t_FUNC_RETURN status;
    uint8_t WriteBuf[5];

    WriteBuf[0] = 0xFE;
    WriteBuf[1] = SAddr;

    for(int i = 0; i < WLen; i++)
    {
        WriteBuf[i + 2] = WData[i];

    }

    status = I2CMA_Write(MCU_ADDR, WriteBuf, (WLen + 2U));
    return status;
}

t_FUNC_RETURN GmVCS_ReadFactory(uint8_t SAddr, uint8_t RLen, uint8_t RData[])
{
    t_FUNC_RETURN status;

    I2CMA_WriteB(MCU_ADDR, 0xFEU, SAddr);

    Cy_SysLib_Delay(10);

    status = GmVCS_GetGmCMD(EngReadReg, RLen, RData);

    return status;
}

t_FUNC_RETURN GmVCS_GetNTC123(int *NTC1, int *NTC2, int *NTC3)
{
    t_FUNC_RETURN result = FUNC_RETURN_OK;
    uint8_t readNTC1[2];
    uint8_t readNTC23[4];

    /*ToDo:show HW and SW version*/
    result = GmVCS_ReadFactory(0x0A, 2, readNTC1);
    result |= GmVCS_ReadFactory(0x0B, 4, readNTC23);

    if(result == FUNC_RETURN_OK)
    {
        *NTC1 = (int)(readNTC1[0] | (readNTC1[1] << 8));
        *NTC2 = (int)(readNTC23[0] | (readNTC23[1] << 8));
        *NTC3 = (int)(readNTC23[2] | (readNTC23[3] << 8));
    }
    else
    {
        *NTC1 = 0;
        *NTC2 = 0;
        *NTC3 = 0;
    }

    return result;
}

/*******************************************************************************
* \brief Function Name: GmVCS_I2CBWrite
*******************************************************************************
*
* Summary:
* ENG mode I2CB Write =>
* I2CSEngWriteBuffer[2]: Address
* I2CSEngWriteBuffer[4]: Write Length
* I2CSEngWriteBuffer[3]: Data
* Example =>
* 0xFE 0x43 0x6c 0x03 0x06 0x80 0x22
* 0xFE 0x43 0x60 0x04 0x29 0x11 0x3F 0xFF  => to asic
*
* Parameters:
* uint8_t Addr7bs :  Tcon=0x60, PGamma=0x74
* uint8_t WLen:
* uint8_t WBuff[]:
*
* Return:
* t_FUNC_RETURN -
*
*/
t_FUNC_RETURN GmVCS_I2CBWrite(uint8_t Addr7bs, uint8_t WLen, uint8_t WBuff[])
{
    t_FUNC_RETURN status;
    uint8_t I2C_WriteBuf[10];

    I2C_WriteBuf[0] = 0xFEU;
    I2C_WriteBuf[1] = 0x43U;
    I2C_WriteBuf[2] = Addr7bs; //asic 7bit address 0x60
    I2C_WriteBuf[3] = WLen;
    memcpy((I2C_WriteBuf + 4U), WBuff, WLen);

    status = I2CMA_Write(MCU_ADDR, I2C_WriteBuf, (WLen + 4U));

    return status;
}

/*******************************************************************************
* \brief Function Name: GmVCS_I2CBRead
*******************************************************************************
*
* Summary:
* ENG mode I2CB Read =>
* I2CSEngWriteBuffer[2]: Address
* I2CSEngWriteBuffer[3]: Write length
* I2CSEngWriteBuffer[4]: Read length
* I2CSEngWriteBuffer[5]: WData
*
* Parameters:
* uint8_t Addr7bs:  Tcon=0x60, PGamma=0x74
* uint8_t WLen:
* uint8_t WBuff[]:
* uint8_t RLen:
* uint8_t RBuff[]:
*
* Return:
* t_FUNC_RETURN -
*
*/
t_FUNC_RETURN GmVCS_I2CBRead(uint8_t Addr7bs, uint8_t WLen, uint8_t WBuff[], uint8_t RLen, uint8_t RBuff[])
{
    t_FUNC_RETURN status;
    uint8_t I2C_ReadBuf[11];
    uint8_t I2C_WriteBuf[10];

    I2C_WriteBuf[0] = 0xFEU;
    I2C_WriteBuf[1] = 0x44U;
    I2C_WriteBuf[2] = Addr7bs; /*aisc device 7bits */
    I2C_WriteBuf[3] = WLen;    /*write len*/
    I2C_WriteBuf[4] = RLen;    /*read len*/
    memcpy((I2C_WriteBuf + 5U), WBuff, WLen);

    status = I2CMA_Write(MCU_ADDR, I2C_WriteBuf, (WLen + 5U));

    Cy_SysLib_Delay(10);

    status = GmVCS_GetGmCMD(EngReadReg, (RLen + 1U), I2C_ReadBuf);

    if((status == FUNC_RETURN_OK) && (I2C_ReadBuf[0] == 0U))
    {
        memcpy(RBuff, (I2C_ReadBuf + 1U), RLen);
    }
    else
    {
        RBuff[0] = 0xFFU;
    }

    return status;
}

/*******************************************************************************
* \brief Function Name: GmVCS_NovaTconWrite
*******************************************************************************
*
* Summary:
* ENG mode I2CB Write =>
* I2CSEngWriteBuffer[2]: Address          --Tcon=0x60
* I2CSEngWriteBuffer[4]: Write Length
* I2CSEngWriteBuffer[3]: Data
* Example =>
* 0xFE 0x43 0x6c 0x03 0x06 0x80 0x22
* 0xFE 0x43 0x60 0x04 0x29 0x11 0x3F 0xFF  => to asic
* Tcon=0x60, PGamma=0x74
* Parameters:
* uint8_t Page :
* uint8_t Reg:
* uint8_t WByte:
*
* Return:
* t_FUNC_RETURN -
*
*/
t_FUNC_RETURN GmVCS_NovaTconWrite(uint8_t Page, uint8_t Reg, uint8_t WByte)
{
    t_FUNC_RETURN status;
    uint8_t I2C_WriteBuf[4];

    I2C_WriteBuf[0] = Page;
    I2C_WriteBuf[1] = Reg;
    I2C_WriteBuf[2] = WByte;

    status = GmVCS_I2CBWrite(NOVA_TCON_ADDR, 3U, I2C_WriteBuf);

    return status;
}

/*******************************************************************************
* \brief Function Name: GmVCS_NovaTconRead
*******************************************************************************
*
* Summary:
* ENG mode I2CB Read =>
* I2CSEngWriteBuffer[2]: Address          --Tcon=0x60
* I2CSEngWriteBuffer[3]: Write length
* I2CSEngWriteBuffer[4]: Read length
* I2CSEngWriteBuffer[5]: WData
*
* Parameters:
* uint8_t Page :
* uint8_t Reg:
* uint8_t RLen:
* uint8_t RBuff[]:
*
* Return:
* t_FUNC_RETURN -
*
*/
t_FUNC_RETURN GmVCS_NovaTconRead(uint8_t Page, uint8_t Reg, uint8_t RLen, uint8_t RBuff[])
{
    t_FUNC_RETURN status;
    uint8_t I2C_WriteBuf[10];

    I2C_WriteBuf[0] = Page;
    I2C_WriteBuf[1] = Reg;

    status = GmVCS_I2CBRead(MCU_ADDR, 2U, I2C_WriteBuf, RLen, RBuff);

    return status;
}

#endif


