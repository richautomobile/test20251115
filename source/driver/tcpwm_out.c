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
/*
#include "cy_pdl.h"
#include "cybsp.h"
*/
#include "tcpwm_out.h"
#include "Gpio.h"

/* ==[ Macros ]============================================================= */
#define HWPWM_BASE_HZ (uint32_t)4000000U
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static uint32_t TCPWMFreq[2];
static uint32_t TCPWMPeriod[2];
static uint16_t TCPWMHiDuty[2];
static uint16_t TCPWMHiPeriod[2];
static uint16_t TCPWMNewHiPeriod[2];
static uint8_t TCPWMEnable[2];
static uint8_t PwmShadowPin[2];


/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
void TCPWMO_ISR0(void);
void TCPWMO_ISR1(void);
/* ==[ Definition of functions with internal linkage ]====================== */
/*******************************************************************************
* \brief Function Name: mPWMO0_Interrupt
********************************************************************************
* Summary:
*
* CY_TCPWM_INT_ON_TC  :  Interrupt on Terminal count
*
*
* \Parameters [in] NONE
* \return          NONE
*
*******************************************************************************/
void TCPWMO_ISR0(void)
{
    uint32_t intrMask = Cy_TCPWM_GetInterruptStatusMasked(mPWMO0_HW, mPWMO0_NUM);
    uint8_t Port = (PwmShadowPin[0] >> 4U);    
    uint8_t Pin = (PwmShadowPin[0] & 0x0FU);
    
    if(0UL != (CY_TCPWM_INT_ON_TC & intrMask))
    {
        if(PwmShadowPin[0] != 0xFFU)
        {
            Cy_GPIO_Write(((GPIO_PRT_Type*) &GPIO->PRT[Port]), Pin, 1U);
        }

        if(TCPWMHiPeriod[0] != TCPWMNewHiPeriod[0])
        {
            TCPWMHiPeriod[0] = TCPWMNewHiPeriod[0];
            /* Modify the compare value here */
            Cy_TCPWM_PWM_SetCompare0(mPWMO0_HW, mPWMO0_NUM, TCPWMHiPeriod[0]);

        }
    }
    else
    {
        if(PwmShadowPin[0] != 0xFFU)
        {            
            Cy_GPIO_Write(((GPIO_PRT_Type*) &GPIO->PRT[Port]), Pin, 0U);
        }
    }

    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(mPWMO0_HW, mPWMO0_NUM, intrMask);

}

/*******************************************************************************
* \brief Function Name: mPWMO1_Interrupt
********************************************************************************
* Summary:
* 1ms Timer_Interrupt_Handler
* Handler function for the timer interrupt that simply toggles the LED.
*
*
* \Parameters [in] NONE
* \return          NONE
*
*******************************************************************************/
void TCPWMO_ISR1(void)
{
    uint32_t intrMask = Cy_TCPWM_GetInterruptStatusMasked(mPWMO1_HW, mPWMO1_NUM);
    uint8_t Port = (PwmShadowPin[1] >> 4U);    
    uint8_t Pin = (PwmShadowPin[1] & 0x0FU);
    
    if(0UL != (CY_TCPWM_INT_ON_TC & intrMask))
    {
        if(PwmShadowPin[1] != 0xFFU)
        {
            Cy_GPIO_Write(((GPIO_PRT_Type*) &GPIO->PRT[Port]), Pin, 1U);
        }

        if(TCPWMHiPeriod[1] != TCPWMNewHiPeriod[1])
        {
            TCPWMHiPeriod[1] = TCPWMNewHiPeriod[1];
            /* Modify the compare value here */
            Cy_TCPWM_PWM_SetCompare0(mPWMO1_HW, mPWMO1_NUM, TCPWMHiPeriod[1]);

        }
    }
    else
    {
        if(PwmShadowPin[1] != 0xFFU)
        {
            Cy_GPIO_Write(((GPIO_PRT_Type*) &GPIO->PRT[Port]), Pin, 0U);
        }
    }

    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(mPWMO1_HW, mPWMO1_NUM, intrMask);

}


/* ==[ Definition of functions with external linkage ]======================= */
/*******************************************************************************
* \brief Function Name: PWMO_Init
********************************************************************************
*
* Summary:
*   Freq. base : 4MHz
*   perial     : 1~65535    => Output Freq= 61HZ(4M/65535)~40KHz(4M/100)
*   Duty       : default 0, range:0~100
*   PWM PIN    : CH 0:P5.4  CH1:P5.6
* \Parameters [in] NONE
* \return
*
*
*******************************************************************************/
t_FUNC_RETURN TCPWMO_Init(uint8_t ch, uint32_t Hz)
{
    cy_stc_tcpwm_pwm_config_t myPWMO_config;

    cy_stc_sysint_t intrCfg;

    /* Initialize, enable, and start the PWM component.
     * To see all pre-defined parameters such as <Instance_Name>_Mask/Num
     * or PWM_config see cycfg_peripherals.h under generated sources.
     */
    if(ch == 0)
    {
        /*init variable*/
        TCPWMFreq[0] = Hz;
        TCPWMEnable[0] = 0U;
        TCPWMHiDuty[0] = 0U;
        TCPWMHiPeriod[0] = 0U;
        TCPWMNewHiPeriod[0] = 0U;
        TCPWMPeriod[0] = (HWPWM_BASE_HZ / Hz);
        PwmShadowPin[0] = 0xFFU;  /*No Accompany Pin*/

        /*Step1: timer hw init*/
        myPWMO_config = mPWMO0_config;
        myPWMO_config.period0 = TCPWMPeriod[0] - 1;
        myPWMO_config.compare0 = 0U;

        (void)Cy_TCPWM_PWM_Init(mPWMO0_HW, mPWMO0_NUM, &myPWMO_config);
        /*Step2: interrupt handler */
        intrCfg.intrSrc = mPWMO0_IRQ;
        intrCfg.intrPriority = 3UL;
        Cy_SysInt_Init(&intrCfg, &TCPWMO_ISR0);
        /*Step3: timer enable*/
        NVIC_EnableIRQ(intrCfg.intrSrc);    /* Enable Interrupt */
        /* Enable the initialized PWM */
        Cy_TCPWM_PWM_Enable(mPWMO0_HW, mPWMO0_NUM);
        /* Then start the PWM */
        /* Triggers a software start on the selected TCPWMs */
        Cy_TCPWM_TriggerStart(mPWMO0_HW, mPWMO0_MASK);
        /*Cy_TCPWM_TriggerReloadOrIndex(mPWMO0_HW, mPWMO0_MASK)*/


    }
    else
    {
        /*init variable*/
        TCPWMFreq[1] = Hz;
        TCPWMEnable[1] = 0U;
        TCPWMHiDuty[1] = 0U;
        TCPWMHiPeriod[1] = 0U;
        TCPWMNewHiPeriod[1] = 0U;
        TCPWMPeriod[1] = (HWPWM_BASE_HZ / Hz);
        PwmShadowPin[1] = 0xFFU; /*No Accompany Pin*/

        /*Step1: timer hw init*/
        myPWMO_config = mPWMO1_config;
        myPWMO_config.period0 = TCPWMPeriod[1U] - 1;
        myPWMO_config.compare0 = 0U;
        (void)Cy_TCPWM_PWM_Init(mPWMO1_HW, mPWMO1_NUM, &myPWMO_config);
        /*Step2: interrupt handler */
        intrCfg.intrSrc = mPWMO1_IRQ;
        intrCfg.intrPriority = 3UL;
        Cy_SysInt_Init(&intrCfg, &TCPWMO_ISR1);
        /*Step3: timer enable*/
        NVIC_EnableIRQ(intrCfg.intrSrc);    /* Enable Interrupt */
        /* Enable the initialized PWM */
        Cy_TCPWM_PWM_Enable(mPWMO1_HW, mPWMO1_NUM);
        /* Triggers a software start on the selected TCPWMs */
        Cy_TCPWM_TriggerStart(mPWMO1_HW, mPWMO1_MASK);

    }

    return FUNC_RETURN_OK;
}

/*******************************************************************************
* \brief Function Name: TCPWMO_SetShadowPin
********************************************************************************
*
* Summary:
*   Freq. base : 4MHz
*   perial     : 1~65535    => Output Freq= 61HZ(4M/65535)~40KHz(4M/100)
*   Duty       : default 0, range:0~100
*   PWM PIN    : CH 0:P5.4  CH1:P5.6 hw pin. These pin are defined in the device config.
*   ShadowPin : 0xFF: empty pin.  other: GPO
*
* \Parameters [in] ch
* \Parameters [in] pin:  port|pin
* \return          t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN TCPWMO_SetShadowPin(uint8_t ch, uint8 pin)
{
    if(ch > 1U)
    {
        ch = 1U;
    }

    PwmShadowPin[ch] = pin;

    return FUNC_RETURN_OK;
}


/*******************************************************************************
* \brief Function Name: TCPWMO_SetFreq
********************************************************************************
*
* Summary:
*   Freq. base : 4MHz
*   perial     : 1~65535    => Output Freq= 61HZ(4M/65535)~40KHz(4M/100)
*   Duty       : default 0, range:0~100
*   PWM PIN    : CH 0:P5.4  CH1:P5.6 hw pin. These pin are defined in the device config.
*   ShadowPin : 0xFF: empty pin.  other: GPO
*
* \Parameters [in] ch
* \Parameters [in] Hz
* \return          t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN TCPWMO_SetFreq(uint8_t ch, uint16_t Hz)
{
    cy_stc_tcpwm_pwm_config_t myPWMO_config;

    if(ch > 1U)
    {
        ch = 1U;
    }

    /*Step1: timer hw init*/
    if(ch == 0U)
    {
        /*init variable*/
        TCPWMFreq[0] = Hz;
        TCPWMPeriod[0] = (HWPWM_BASE_HZ / Hz);
        TCPWMNewHiPeriod[0] = (TCPWMPeriod[0] * TCPWMHiDuty[0]) / 100U;

        /*Step1: timer hw init*/
        myPWMO_config = mPWMO0_config;
        myPWMO_config.period0 = TCPWMPeriod[0] - 1;
        myPWMO_config.compare0 = TCPWMNewHiPeriod[0];
        (void)Cy_TCPWM_PWM_Init(mPWMO0_HW, mPWMO0_NUM, &myPWMO_config);
    }
    else
    {
        /*init variable*/
        TCPWMFreq[1] = Hz;
        TCPWMPeriod[1] = (HWPWM_BASE_HZ / Hz);
        TCPWMNewHiPeriod[1] = (TCPWMPeriod[1] * TCPWMHiDuty[1]) / 100U;

        /*Step1: timer hw init*/
        myPWMO_config = mPWMO1_config;
        myPWMO_config.period0 = TCPWMPeriod[1U] - 1;
        myPWMO_config.compare0 = TCPWMNewHiPeriod[1];
        (void)Cy_TCPWM_PWM_Init(mPWMO1_HW, mPWMO1_NUM, &myPWMO_config);
    }

    return FUNC_RETURN_OK;
}



/*******************************************************************************
* \brief Function Name: TCPWMO_SetDuty
********************************************************************************
*
* Summary:
*   Freq. base : 4MHz
*   perial     : 1~65535    => Output Freq= 61HZ(4M/65535)~40KHz(4M/100)
*   Duty       : default 0, range:0~100
*   PWM PIN    : CH 0:P5.4  CH1:P5.6

* \Parameters [in] ch
* \Parameters [in] duty
* \return          t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN TCPWMO_SetDuty(uint8_t ch, uint8_t duty)
{
    if(ch > 1U)
    {
        ch = 1U;
    }

    TCPWMEnable[ch] = 0xAAU;

    if(TCPWMHiDuty[ch] !=  duty)
    {
        TCPWMHiDuty[ch] =  duty;
        TCPWMNewHiPeriod[ch] = (TCPWMPeriod[ch] * duty) / 100U;
    }

    return FUNC_RETURN_OK;
}

t_TCPWM_VALUE TCPWMO_GetPwmDuty(uint8_t ch)
{
    t_TCPWM_VALUE pwm_value;

    if((2U > ch) && (TCPWMEnable[ch] == 0xAAU))
    {
        pwm_value.Frequency = TCPWMFreq[ch];
        pwm_value.Duty = TCPWMHiDuty[ch];
    }
    else
    {
        pwm_value.Frequency = 0U;
        pwm_value.Duty = 0U;
    }

    return (pwm_value);
}
