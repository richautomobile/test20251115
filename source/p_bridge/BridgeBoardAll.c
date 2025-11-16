/**
 * \file BridgeBoard.c
 *
 * \brief The main program of Bridge board.
 *
 * The main program of Bridge board is universal regardless of project.
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
#include "BridgeBoardAll.h"
#include "system.h"
#include "uart.h"
#include "gpio.h"
#include "i2cm.h"
#include "SpiMaster.h"
#include "SoftwareTimer.h"
#include "user_flash.h"
#include "tcpwm_out.h"
#include "swpwm_out.h"
#include "breath_led.h"
#include "wdt.h"
#include "ext_int.h"
#include "debounce.h"
#include "i2cs.h"
#include "adc.h"
#include "quad_dec.h"
#include "DevEncoder.h"
#include "button.h"

#if (BRIDGE_UNIV_ENABLE==1U)
/* ==[ Macros ]============================================================= */
#define BUTTON_PORT     4U
#define DIP_SWITCH_PORT 7U
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/*
UserDataArray
[0-1]: HW version     [2-3]:FW version
[4-5]:Reset counter   [6-7]: Power-on counter
[8-9]: Dig2.bit7(bit0) + rotation state(bit1) + Power OVP(bit2)
[10-11]: PWM1&2 multiplier  [12-13]: PWM3&4 multiplier
[14-15]: Display PWM multiplier(H) + rotation counter(L)
[16-17]: SW part number H   [18-19]: SW part number L
[20-21]: HW part number H   [22-23]: HW part number L
[24-25]: reserved  [26-27]: reserved
*/
static uint8_t UserDataArray[28];
static uint8_t Button_Val;
static uint8_t DipSwitch_Val;

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static void Bridge_UpdateUserData(void);
static void Bridge_BoardPower(uint8_t OnOff);
#if 0
static t_FUNC_RETURN NOVA_TCON_BypassSPI();
static t_FUNC_RETURN SPIM_Read_Test(void);
#endif
/* ==[ Definition of functions with internal linkage ]====================== */
/*
* UserDataPackageSize = 2bytes index + UserDataArray size + 2byte chk
* UserDataArray =>
* [0-1]: HW version     [2-3]:FW version
* [4-5]:Reset counter   [6-7]: Power-on counter
* [8-9]: Dig2.bit7(bit0) + rotation state(bit1) + Power OVP(bit2)
* [10-11]: PWM1&2 multiplier  [12-13]: PWM3&4 multiplier
* [14-15]: Display PWM multiplier(H) + rotation counter(L)
* [16-17]: SW part number H   [18-19]: SW part number L
* [20-21]: HW part number H   [22-23]: HW part number L
* [24-25]: reserved  [26-27]: reserved
*/
static void Bridge_UpdateUserData(void)
{
    uint8_t boot_reason = Sys_GetBootReason();
    uint16_t curr_data;
    t_FUNC_RETURN rtn_state;
    /*The default value of PSOC FLASH is 0x00.*/
    rtn_state = UserFlash_GetPureUserData(UserDataArray, 28U);

    if(rtn_state == FUNC_RETURN_NG)
    {
        (void)UserFlash_ClearAll();
    }

    if((UserDataArray[0] == 0x00U) && (UserDataArray[1] == 0x00U) && \
            (UserDataArray[2] == 0x00U) && (UserDataArray[3] == 0x00U))
    {
        /*Init default value.*/
        UserDataArray[0] = HW_VER;
        UserDataArray[1] = 0U;
        UserDataArray[2] = FW_APP_MINOR_VER;
        UserDataArray[3] = FW_APP_VER;
        UserDataArray[4] = 0U;
        UserDataArray[5] = 0U;
        UserDataArray[6] = 0U;
        UserDataArray[7] = 0U;
        UserDataArray[8] = 0U;
        UserDataArray[9] = 0U;
    }

    if((boot_reason & 0x0FU) > 0U)
    {
        /*reset count+1*/
        curr_data = UserDataArray[5];
        curr_data = (curr_data << 8) | UserDataArray[4];
        curr_data ++;
        UserDataArray[4] = (uint8_t)curr_data;
        UserDataArray[5] = (uint8_t)(curr_data >> 8);
    }

    if((boot_reason & 0x10U) > 0U)
    {
        /*power up count+1*/
        curr_data = UserDataArray[7];
        curr_data = (curr_data << 8) | UserDataArray[6];
        curr_data ++;
        UserDataArray[6] = (uint8_t)curr_data;
        UserDataArray[7] = (uint8_t)(curr_data >> 8);
    }

    (void)UserFlash_Write((uint8_t *)UserDataArray, 28, 1U);

}

#if 0
static t_FUNC_RETURN SPIM_Read_Test(void)
{
    uint8_t wData[50];
    uint8_t rData[50];

    for(int i = 0; i < 50 ; i++)
    {
        wData[i] = i;
    }

    SPIM_WriteRead(0, wData, 50, rData, 50);

    return FUNC_RETURN_OK;
}
#endif
static void Bridge_BoardPower(uint8_t OnOff)
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
#if 0
static t_FUNC_RETURN NOVA_TCON_BypassSPI()
{
    uint8_t wBuff[3] = {0x70U, 0xFFU, 0xA5};
    t_FUNC_RETURN rtn_state;

    rtn_state = SPIM_Write(0, wBuff, 3U);
    return rtn_state;
}
#endif
/* ==[ Definition of functions with external linkage ]======================= */
t_FUNC_RETURN Bridge_Init(void)
{
    Bridge_UpdateUserData();

#if 0
    /*INIT DBG*/
    /*CY_GPIO_INTR_RISING, CY_GPIO_INTR_FALLING, CY_GPIO_INTR_BOTH*/
    (void)ExtInt_Init(0xB5, CY_GPIO_INTR_BOTH);
#endif

#if 0  /*encoder test*/
    QuadDec_Init();
    Encoder_PinInit(0x16U, 0x17U, 0xFFU, 0xFFU);
#endif

    Bridge_BoardPower(1U);

    /*WDT Init*/
    SWTimer_Reset(1U, SWTimer_Routine, 50U); /*50ms WDT Period */
    Wdt_Init();

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Bridge_RoutineCall(void)
{
#if 0  /*For debug*/

    /*2sec routine call.*/
    if(SWTimer_GetTimeoutState(4U) > 0U) /*2sec*/
    {
        Gpio_Inv(SW_Debug_Led);

#if 0
        /*spi test*/
        SPIM_Read_Test();
        /*ADC test*/
        printf("Temp. = %i\r\n", Adc_GetAdcVal(8));
        printf("ADC0 = 0x%x\r\n", Adc_GetAdcVal(0));
#endif

#if 0
        /*ENCODER TEST*/
        static uint8_t test_dir = 0;

        if(test_dir == 0)
        {
            printf("Send Encoder CW =>\r\n");
            Encoder_Gen(0, 0, 100, 10);
            test_dir = 1;
        }
        else
        {
            printf("Send Encoder CCW =>\r\n");
            Encoder_Gen(0, 1, 100, 12);
            test_dir = 0;
        }

        printf("Encoder Cnt. = 0x%lx\r\n", QuadDec_GetCount());
        printf("Encoder Dir. = %i\r\n", QuadDec_GetDir());
#endif
    }

#if 0
    /*For PIONEER KIT*/
    DipSwitch_Val = Gpio_GetPort(DIP_SWITCH_PORT);
    Button_Val = Debounce_GetPort(11U, 0x20U);

    if(Button_Val > 0)
    {
        Gpio_WritePin(0x64, 1);
    }
    else
    {
        Gpio_WritePin(0x64, 0);
    }

#endif
#endif

    #if 0
    uint8_t data[100];
    uint8_t length;
    #endif

    Button_Val = Button_GetBtnState();
    DipSwitch_Val = Gpio_GetPort(DIP_SWITCH_PORT);

    /*WDT CLEAR*/
    if(SWTimer_Get50msRoutine() > 0U)
    {
        Wdt_Clear();
        #if 0
        /*usrt debug*/
        if(Uart_GetArray(data, &length) == TRUE)
        {
            if(data[length] != '\0')
            {
                data[length] = '\0';
            }

            printf("RX:%s\r\n", data);
        }
        #endif
    }

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
    Gpio_PinConfig(MUX_I2C_SEL, CY_GPIO_DM_STRONG, 0);   /*0:default FT4222 I2C*/
    Gpio_PinConfig(MUX_SPI_SEL, CY_GPIO_DM_STRONG, 0U);  /*1:default PSOC SPI*/
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

    Gpio_PinConfig(EN_5V0, CY_GPIO_DM_STRONG, 1);
    Gpio_PinConfig(DBGP_I2C_Bypass, CY_GPIO_DM_STRONG, 0);  /*dbg i2c close*/
    Gpio_PinConfig(DBGP_SPI_Bypass, CY_GPIO_DM_STRONG, 0);  /*dbg spi close*/
    Gpio_PinConfig(Breathe_LED, CY_GPIO_DM_STRONG, 1);
    Gpio_PinConfig(DBG_UART, CY_GPIO_DM_STRONG, 1);
    /*encoder default level low */
    Gpio_PinConfig(EncAP1, CY_GPIO_DM_STRONG, 0);
    Gpio_PinConfig(EncAP2, CY_GPIO_DM_STRONG, 0);
    Gpio_PinConfig(EncBP1, CY_GPIO_DM_STRONG, 0);
    Gpio_PinConfig(EncBP2, CY_GPIO_DM_STRONG, 0);
    
    Gpio_PinConfig(EX_FPGA1, CY_GPIO_DM_STRONG, 0);

    Gpio_PinConfig(CAN_STB, CY_GPIO_DM_STRONG, 0); /*0= working*/
    Gpio_PinConfig(EX_IO2, CY_GPIO_DM_STRONG, 1);

    Cy_SysLib_Delay(100U); /*wait for power stable */

    
    
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
    t_FUNC_RETURN rtnState = FUNC_RETURN_OK;

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
    t_FUNC_RETURN rtnState = FUNC_RETURN_OK;
    uint8_t len = 0;
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

#endif /*#if (PROJECT == BRIDGE_UNIV_ENABLE)*/
