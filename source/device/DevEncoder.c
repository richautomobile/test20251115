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
#include "DevEncoder.h"
#include "gpio.h"
#include "wdt.h"

#ifdef EN_DEV_ENCODER
/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
uint8_t Set_EncAP1 = EncAP1;
uint8_t Set_EncAP2 = EncAP2;
uint8_t Set_EncBP1 = EncBP1;
uint8_t Set_EncBP2 = EncBP2;
uint8_t CurrEncAStep = 3U;  /*init (0,0)=>Step=3*/
uint8_t CurrEncBStep = 3U;  /*init (0,0)=>Step=3*/

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
static uint16_t EncoderPWM;
/* ==[ Declaration of functions with internal linkage ]===================== */
static void Encoder_Output(uint8_t EncNo, uint8_t dir, uint8_t step);
/* ==[ Definition of functions with internal linkage ]====================== */
/* \brief Encoder_Output
 *
 * Default state 0,0
 * CW => 0,0 => 1,0 => 1,1 => 0,1 => 0,0
 * CCW=> 0,0 => 0,1 => 1,1 => 1,0 => 0,0
 * ==================================
 * \Parameters [in] EncNo: Encoder Number. Encoder A=0,  Encoder B=1
 * \Parameters [in] dir:      direction.           CW=0,  CCW=1
 * \Parameters [in] step:  gen. plus number
 * \return          NONE
 * ===================================
 *
 */
static void Encoder_Output(uint8_t EncNo, uint8_t dir, uint8_t step)
{
    /*Encoder0 => P0_6, P0_7*/
    /*Encoder1=>  P1_0, P2_0*/
    /*CW   => dir=0, CCW => dir=1*/

    if(EncNo == 0)   /*Encoder0*/
    {
        CurrEncAStep = step;

        if(dir == 0)   /*CW*/
        {
            switch(step)
            {
                case 0U:
                    /*En0_A(1);  En0_B(0)*/
                    Gpio_WritePin(Set_EncAP1, 1U);
                    Gpio_WritePin(Set_EncAP2, 0U);
                    break;

                case 1:
                    /*En0_A(1);  En0_B(1)*/
                    Gpio_WritePin(Set_EncAP1, 1U);
                    Gpio_WritePin(Set_EncAP2, 1U);
                    break;

                case 2:
                    /*En0_A(0);  En0_B(1)*/
                    Gpio_WritePin(Set_EncAP1, 0U);
                    Gpio_WritePin(Set_EncAP2, 1U);
                    break;

                case 3:
                    /*En0_A(0);  En0_B(0)*/
                    Gpio_WritePin(Set_EncAP1, 0U);
                    Gpio_WritePin(Set_EncAP2, 0U);
                    break;

                default:
                    break;
            }
        }
        else            /*CCW*/
        {
            switch(step)
            {
                case 0:
                    /*En0_A(0);  En0_B(1)*/
                    Gpio_WritePin(Set_EncAP1, 0U);
                    Gpio_WritePin(Set_EncAP2, 1U);
                    break;

                case 1:
                    /*En0_A(1);  En0_B(1)*/
                    Gpio_WritePin(Set_EncAP1, 1U);
                    Gpio_WritePin(Set_EncAP2, 1U);
                    break;

                case 2:
                    /*En0_A(1);  En0_B(0)*/
                    Gpio_WritePin(Set_EncAP1, 1U);
                    Gpio_WritePin(Set_EncAP2, 0U);
                    break;

                case 3:
                    /*En0_A(0);  En0_B(0)*/
                    Gpio_WritePin(Set_EncAP1, 0U);
                    Gpio_WritePin(Set_EncAP2, 0U);
                    break;

                default:
                    break;
            }
        }
    }
    else
    {
        CurrEncBStep = step;

        if(dir == 0)   /*CW*/
        {
            switch(step)
            {
                case 0:
                    /*En1_A(1);  En1_B(0)*/
                    Gpio_WritePin(Set_EncBP1, 1U);
                    Gpio_WritePin(Set_EncBP2, 0U);
                    break;

                case 1:
                    /*En1_A(1);  En1_B(1)*/
                    Gpio_WritePin(Set_EncBP1, 1U);
                    Gpio_WritePin(Set_EncBP2, 1U);
                    break;

                case 2:
                    /*En1_A(0);  En1_B(1)*/
                    Gpio_WritePin(Set_EncBP1, 0U);
                    Gpio_WritePin(Set_EncBP2, 1U);
                    break;

                case 3:
                    /*En1_A(0);  En1_B(0)*/
                    Gpio_WritePin(Set_EncBP1, 0U);
                    Gpio_WritePin(Set_EncBP2, 0U);
                    break;

                default:
                    break;
            }
        }
        else            /*CCW*/
        {
            switch(step)
            {
                case 0:
                    /*En1_A(0);  En1_B(1)*/
                    Gpio_WritePin(Set_EncBP1, 0U);
                    Gpio_WritePin(Set_EncBP2, 1U);
                    break;

                case 1:
                    /*En1_A(1);  En1_B(1)*/
                    Gpio_WritePin(Set_EncBP1, 1U);
                    Gpio_WritePin(Set_EncBP2, 1U);
                    break;

                case 2:
                    /*En1_A(1);  En1_B(0)*/
                    Gpio_WritePin(Set_EncBP1, 1U);
                    Gpio_WritePin(Set_EncBP2, 0U);
                    break;

                case 3:
                    /*En1_A(0);  En1_B(0)*/
                    Gpio_WritePin(Set_EncBP1, 0U);
                    Gpio_WritePin(Set_EncBP2, 0U);
                    break;

                default:
                    break;
            }
        }
    }

}
/* ==[ Definition of functions with external linkage ]======================= */
/* \brief Encoder_PinInit
 *
 * The user redefines the encoding hardware pins.
 * There are already preset values and there is no need to set them again.
 * ==================================
 * \Parameters [in] EncAP1_Pin:
 * \Parameters [in] EncAP2_Pin:
 * \Parameters [in] EncBP1_Pin:
 * \Parameters [in] EncBP2_Pin:
 * \return          t_FUNC_RETURN:
 * ===================================
 *
 */
t_FUNC_RETURN Encoder_PinInit(uint8_t EncAP1_Pin, uint8_t EncAP2_Pin, uint8_t EncBP1_Pin, uint8_t EncBP2_Pin)
{
    /*output define*/
    if(EncAP1_Pin != 0xFFU)
    {
        Gpio_PinConfig(EncAP1_Pin, CY_GPIO_DM_STRONG, 0);
        Set_EncAP1 = EncAP1_Pin;
        CurrEncAStep = 3U;
    }

    if(EncAP1_Pin != 0xFFU)
    {
        Gpio_PinConfig(EncAP2_Pin, CY_GPIO_DM_STRONG, 0);
        Set_EncAP2 = EncAP2_Pin;
        CurrEncAStep = 3U;
    }

    if(EncAP1_Pin != 0xFFU)
    {
        Gpio_PinConfig(EncBP1_Pin, CY_GPIO_DM_STRONG, 0);
        Set_EncBP1 = EncBP1_Pin;
        CurrEncBStep = 3U;
    }

    if(EncAP1_Pin != 0xFFU)
    {
        Gpio_PinConfig(EncBP2_Pin, CY_GPIO_DM_STRONG, 0);
        Set_EncBP2 = EncBP2_Pin;
        CurrEncBStep = 3U;
    }

    return FUNC_RETURN_OK;
}

/* \brief Encoder_Gen1X
 *   1 phase clock(Freq) => 1 output count,  1 output count = 4 Quadrature counts
 * Default state 0,0(Step3)
 * CW => 0,0 => 1,0(Step0)=> 1,1(Step1) => 0,1(Step2) => 0,0(Step3)
 * CCW=> 0,0 => 0,1(Step0) => 1,1(Step1) => 1,0(Step2) => 0,0(Step3)
 * ==================================
 * \Parameters [in] EncNo: Encoder Number. Encoder A=0,  Encoder B=1
 * \Parameters [in] dir:      direction.           CW=0,  CCW=1
 * \Parameters [in] Freq:    Encoder frequence. 1~255Hz
 * \Parameters [in] count:  gen. plus number
 * \return          t_FUNC_RETURN:
 * ===================================
 *
 */
t_FUNC_RETURN Encoder_Gen1X(uint8_t EncNo, uint8_t dir, uint8_t Freq, uint8_t count)
{
    /*Encoder0 => P0_6, P0_7*/
    /*Encoder1=>  P1_0, P2_0*/
    /*CW   => dir=0, CCW => dir=1*/
    uint8_t i;
    uint8_t StartInd;
    uint8_t longDelay;
    uint16_t DelayCnt;
    uint16_t count4X;

    if((EncNo >= 2u) || (count == 0u) || (Freq == 0u))
    {
        return FUNC_RETURN_NG;
    }

    EncoderPWM = Freq;

    if(Freq > 5U)
    {
        DelayCnt = (uint16_t)(250000 / Freq); /* 1000000/Freq/4 */
        longDelay = 0U;
    }
    else
    {
        DelayCnt = (uint16_t)(250 / Freq); /* 1000/Freq/4 */
        longDelay = 1U;
    }

    /*Step 3: 1x mode output*/
    count4X = count;
    count4X <<= 2;

    if(EncNo == 0U)
    {
        StartInd = CurrEncAStep + 1U;
    }
    else
    {
        StartInd = CurrEncBStep + 1U;
    }

    StartInd = (StartInd % 4);

    Wdt_OnOff(0U);  /*WDT OFF*/

    for(i = 0; i < count4X; i++)
    {
        Encoder_Output(EncNo, dir, StartInd);
        (longDelay > 0U) ? Cy_SysLib_Delay(DelayCnt) : Cy_SysLib_DelayUs(DelayCnt);
        StartInd ++;
        StartInd = (StartInd % 4);
    }

    Wdt_OnOff(1U);  /*WDT ON*/

    return FUNC_RETURN_OK;
}

/* \brief Encoder_Gen2X
 *   1 phase clock(Freq) => 2 output counts, 1 output count = 2 Quadrature counts
 * Default state 0,0(Step3)
 * CW => 0,0 => 1,0(Step0)=> 1,1(Step1) => 0,1(Step2) => 0,0(Step3)
 * CCW=> 0,0 => 0,1(Step0) => 1,1(Step1) => 1,0(Step2) => 0,0(Step3)
 * ==================================
 * \Parameters [in] EncNo: Encoder Number. Encoder A=0,  Encoder B=1
 * \Parameters [in] dir:      direction.           CW=0,  CCW=1
 * \Parameters [in] Freq:    Encoder frequence. 1~255Hz
 * \Parameters [in] count:  gen. plus number
 * \return          t_FUNC_RETURN:
 * ===================================
 *
 */
t_FUNC_RETURN Encoder_Gen2X(uint8_t EncNo, uint8_t dir, uint8_t Freq, uint8_t count)
{
    /*Encoder0 => P0_6, P0_7*/
    /*Encoder1=>  P1_0, P2_0*/
    /*CW   => dir=0, CCW => dir=1*/
    uint8_t i;
    uint8_t StartInd;
    uint8_t longDelay;
    uint16_t DelayCnt;
    uint16_t count2X;

    /*Step 1:*/
    if((EncNo >= 2U) || (count == 0U) || (Freq == 0U))
    {
        return FUNC_RETURN_NG;
    }

    /*Step 2: cal delay time to map freq*/
    EncoderPWM = Freq;

    if(Freq > 5U)
    {
        DelayCnt = (uint16_t)(250000 / Freq); /* 1000000/Freq/4. us delay */
        longDelay = 0U;
    }
    else
    {
        DelayCnt = (uint16_t)(250 / Freq); /* 1000/Freq/4. ms delay */
        longDelay = 1U;
    }

    /*Step 3: 2x mode output*/
    count2X = count;
    count2X <<= 1;

    if(EncNo == 0U)
    {
        StartInd = CurrEncAStep + 1U;
    }
    else
    {
        StartInd = CurrEncBStep + 1U;
    }

    StartInd = (StartInd % 4);

    Wdt_OnOff(0U); /*WDT OFF*/

    for(i = 0; i < count2X; i++)
    {
        Encoder_Output(EncNo, dir, StartInd);
        (longDelay > 0U) ? Cy_SysLib_Delay(DelayCnt) : Cy_SysLib_DelayUs(DelayCnt);

        StartInd ++;
        StartInd = (StartInd % 4);
    }

    Wdt_OnOff(1U); /*WDT ON*/
    return FUNC_RETURN_OK;
}

/* \brief Encoder_Gen4X
 *    1 phase clock(Freq) => 4 output counts, 1 output count = 1 Quadrature count
 * Default state 0,0(Step3)
 * CW => 0,0 => 1,0(Step0)=> 1,1(Step1) => 0,1(Step2) => 0,0(Step3)
 * CCW=> 0,0 => 0,1(Step0) => 1,1(Step1) => 1,0(Step2) => 0,0(Step3)
 * ==================================
 * \Parameters [in] EncNo: Encoder Number. Encoder A=0,  Encoder B=1
 * \Parameters [in] dir:      direction.           CW=0,  CCW=1
 * \Parameters [in] Freq:    Encoder frequence. 1~255Hz
 * \Parameters [in] count:  gen. plus number
 * \return          t_FUNC_RETURN:
 * ===================================
 *
 */
t_FUNC_RETURN Encoder_Gen4X(uint8_t EncNo, uint8_t dir, uint8_t Freq, uint8_t count)
{
    /*Encoder0 => P0_6, P0_7*/
    /*Encoder1=>  P1_0, P2_0*/
    /*CW   => dir=0, CCW => dir=1*/
    uint8_t i;
    uint8_t StartInd;
    uint8_t longDelay;
    uint16_t DelayCnt;

    /*Step 1:*/
    if((EncNo >= 2U) || (count == 0U) || (Freq == 0U))
    {
        return FUNC_RETURN_NG;
    }

    /*Step 2: cal delay time to map freq*/
    EncoderPWM = Freq;

    if(Freq > 5U)
    {
        DelayCnt = (uint16_t)(250000 / Freq); /* 1000000/Freq/4. us delay */
        longDelay = 0U;
    }
    else
    {
        DelayCnt = (uint16_t)(250 / Freq); /* 1000/Freq/4. ms delay */
        longDelay = 1U;
    }

    /*Step 3: 2x mode output*/
    if(EncNo == 0U)
    {
        StartInd = CurrEncAStep + 1U;
    }
    else
    {
        StartInd = CurrEncBStep + 1U;
    }

    StartInd = (StartInd % 4);

    Wdt_OnOff(0U);  /*WDT OFF*/

    for(i = 0; i < count; i++)
    {
        Encoder_Output(EncNo, dir, StartInd);
        (longDelay > 0U) ? Cy_SysLib_Delay(DelayCnt) : Cy_SysLib_DelayUs(DelayCnt);

        StartInd ++;
        StartInd = (StartInd % 4);
    }

    Wdt_OnOff(1U); /*WDT ON*/
    return FUNC_RETURN_OK;
}


t_FUNC_RETURN Encoder_GetPwm(uint8_t *PwmHz)
{
    *PwmHz = EncoderPWM;
    return FUNC_RETURN_OK;
}

#endif
