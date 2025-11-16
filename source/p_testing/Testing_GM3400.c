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
#include "Testing_GM3400.h"

#if (TESTING_ENABLE==1U) && (PROJECT == GM_VCS_3400)
/* ==[ Macros ]============================================================= */
#define BUTTON_PORT     4U
#define DIP_SWITCH_PORT 7U

#define serAddr 0x0C    //7bits   // 0x18      //8bits
#define desAddr0 0x2C   //7bits   // 0x58     //8bits
#define desAlias0 0x2C  //7bits   //0x58    //8bits

#define EN_UH983  0U
#define EN_HH983  1U

/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static void Testing_BoardPower(uint8_t OnOff);
/* ==[ Definition of functions with internal linkage ]====================== */
/*  \Brief Aging_BoardPower
*
*     LOT3 board power on or off control.
*    
* \Parameters [in]   uint8_t  0:off, >0:on 
* \return         void
*/
static void Testing_BoardPower(uint8_t OnOff)
{
    if(OnOff > 0U)
    {
        /*Power On*/
        Gpio_WritePin(EN_5V0, 1U);
        Cy_SysLib_Delay(10U);
        Gpio_WritePin(EN_3V3, 1U);
        Cy_SysLib_Delay(10U);
        Gpio_WritePin(EN_1V15, 1U);
        Cy_SysLib_Delay(10U);
        Gpio_WritePin(EN_1V8, 1U);
        Cy_SysLib_Delay(10U);
        Gpio_WritePin(PDB_MCU, 1U);
    }    
    else
    {
        /*Power Off*/
        Gpio_WritePin(PDB_MCU, 0U);
        Cy_SysLib_Delay(10);
        Gpio_WritePin(EN_1V8, 0U);
        Cy_SysLib_Delay(10);
        Gpio_WritePin(EN_1V15, 0U);
        Cy_SysLib_Delay(10);
        Gpio_WritePin(EN_3V3, 0U);
        Cy_SysLib_Delay(10);
        Gpio_WritePin(EN_5V0, 0U);
    }
}

/* ==[ Definition of functions with external linkage ]======================= */
t_FUNC_RETURN Testing_Init(void)
{
    Testing_BoardPower(1U);
    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Testing_RoutineCall(void)
{

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Project_Gpio_Init(void)
{
#if (BOARD == BOARD_PSOC4)
        /*input define*/
        Gpio_PinConfig(CON1_Detetion, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(CON2_Detetion, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(Lock_MCU, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(IRQ_MCU, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(INTB_TP, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(MCU_BUTTON_1, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(MCU_BUTTON_2, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH1, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH2, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH3, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH4, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH5, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH6, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH7, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH8, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(WAKE_CSM, CY_GPIO_DM_HIGHZ, 0);
        
        /*output define*/
        Gpio_PinConfig(PDB_MCU, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(Panel_Vin_ON_Norm, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(Panel_Vin_ON_Quie, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(LED_Crest_ON, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(Second_Panel_ON, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(STBYB, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(FPD_BD_I2C_Switch, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EN_3V3, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(WAKE_FIDM, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(MUX_I2C_SEL, CY_GPIO_DM_STRONG, 1U); /*1:PSOC I2C*/
        Gpio_PinConfig(MUX_SPI_SEL, CY_GPIO_DM_STRONG, 1U); /*1:PSOC SPI*/  
    
        Gpio_PinConfig(Commu_led, CY_GPIO_DM_STRONG, 1U);
        Gpio_PinConfig(EN_1V8, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EN_1V15, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SW_Debug_Led, CY_GPIO_DM_STRONG, 1U);
        Gpio_PinConfig(OTPPWR_EN, CY_GPIO_DM_STRONG, 1U);
    
        Gpio_PinConfig(SPButton_S1, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S2, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S3, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S4, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S5, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S6, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(WAKE_SP, CY_GPIO_DM_STRONG, 0);
    
        Gpio_PinConfig(CAN_STB, CY_GPIO_DM_STRONG, 0); /*0= working*/
        Gpio_PinConfig(EX_IO2, CY_GPIO_DM_STRONG, 1);
        Gpio_PinConfig(EN_5V0, CY_GPIO_DM_STRONG, 1);
        Gpio_PinConfig(DBGP_I2C_Bypass, CY_GPIO_DM_STRONG, 0);  /*dbg i2c close*/
        Gpio_PinConfig(DBGP_SPI_Bypass, CY_GPIO_DM_STRONG, 0);  /*dbg spi close*/
        Gpio_PinConfig(Breathe_LED, CY_GPIO_DM_STRONG, 1);
        Gpio_PinConfig(DBG_UART, CY_GPIO_DM_STRONG, 1);
    
        Gpio_PinConfig(EncAP1, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EncAP2, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EncBP1, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EncBP2, CY_GPIO_DM_STRONG, 0);
    
        Gpio_PinConfig(EX_FPGA1, CY_GPIO_DM_STRONG, 0);
    
#endif

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Project_ExtINT_Callback(uint8_t PortPin)
{
    if(PortPin == 0xB5)
    {
    #if 0  /*DBG*/
       Gpio_Inv(SW_Debug_Led);  
    #endif
    }

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Project_I2CSW_Callback(uint8_t i2csWriteBuff[])
{
    t_FUNC_RETURN rtnState=FUNC_RETURN_OK;
    
    switch(i2csWriteBuff[1])
    {
       default:
        rtnState = FUNC_RETURN_NG;
        break;
    }

    return rtnState;
}

t_FUNC_RETURN Project_I2CSR_Callback(uint8_t i2csReadBuff[])
{
    t_FUNC_RETURN rtnState=FUNC_RETURN_OK;
    uint8_t len=0;
    uint8_t rtnBuff[10];
    
    switch(i2csReadBuff[1])
    {
       default:
        rtnState = FUNC_RETURN_NG;
        break;
    }

    if(rtnState == FUNC_RETURN_OK)
    {
       (void)I2cs_UpdateReadBuff(rtnBuff, len);
    }

    return rtnState;
}

#endif
