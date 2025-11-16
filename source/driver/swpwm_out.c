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
#include "swpwm_out.h"
#include "gpio.h"

/* ==[ Macros ]============================================================= */
#define SWPWM_BASE_HZ (uint32_t)50000U
#define SWPWMO_CH       4U
#define mPWMO1_INPUT_DISABLED 0x7U

/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static uint32_t SwPWMFreq;
static uint32_t SwPWMPeriod;
static uint16_t SwPWMDuty[SWPWMO_CH];
static uint8_t  SwPWMEnable[SWPWMO_CH];
static uint8_t  SwPWMPinLevel[SWPWMO_CH];
static uint8_t  SwPWMHiDutyCompre[SWPWMO_CH];
static uint8_t  SwPWMNewHiDutyCompre[SWPWMO_CH];
static uint8_t  SetPwmPin[SWPWMO_CH];
static uint16_t Count_SwPwm;
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static void SWPWM_Interrupt(void);

/* ==[ Definition of functions with internal linkage ]====================== */
/*******************************************************************************
* \brief Function Name: SWPWM_Interrupt
********************************************************************************
* Summary:
*
*
* \Parameters [in] NONE
* \return          NONE
*
*******************************************************************************/
static void SWPWM_Interrupt(void)
{
    /*static uint16_t count_1sec = 0*/
    /* Clear the terminal count interrupt */
    Cy_TCPWM_ClearInterrupt(TCPWM, 7U, CY_TCPWM_INT_ON_TC);

    Count_SwPwm++;

    for(int i = 0; i < SWPWMO_CH; i++)
    {
        if(SwPWMEnable[i] == 0xAAU)
        {
            if(Count_SwPwm == 1U)
            {
                SwPWMHiDutyCompre[i] = SwPWMNewHiDutyCompre[i];
            }

            if(Count_SwPwm > SwPWMHiDutyCompre[i])
            {
                if(SwPWMPinLevel[i] == 1U)
                {
                    SwPWMPinLevel[i] = 0U;
                    Gpio_WritePin(SetPwmPin[i], 0U);
                }
            }
            else
            {
                if(SwPWMPinLevel[i] == 0U)
                {
                    SwPWMPinLevel[i] = 1U;
                    Gpio_WritePin(SetPwmPin[i], 1U);
                }
            }
        }
    }

    if(Count_SwPwm >= SwPWMPeriod)
    {
        Count_SwPwm = 0U;
    }
}

/* ==[ Definition of functions with external linkage ]======================= */
/*******************************************************************************
* \brief Function Name: PWMO_Init
********************************************************************************
*
* Summary:  Init base frequence 50KHz.
*   Freq. base  : 4MHz
*   perial         : 79    => (4M/50K)-1=79
*   timer          : TCPWM7  counter mode
*   PWM Freq  :  Fmax= 50K/100 = 500,   Fmin=50K/65535=0.6
*   PWM Duty  : default 0, renage:0~100
*   PWM PIN    : user define, max=8
*
* Parameter:
*   uint32_t Hz  :  0~500Hz
* Return:
*   Status of initialization
*
*******************************************************************************/
t_FUNC_RETURN SWPWMO_FreqInit(uint32_t Hz)
{
    cy_rslt_t result;
    cy_stc_tcpwm_counter_config_t mTIMER50K_config =  /*= mTIMER1MS_config;*/
    {
    .period = 3999,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 16384,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_ON_TC,
    .captureInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .captureInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
    };     

    /*interrupt config*/
    cy_stc_sysint_t intrCfg =
    {
        /*.intrSrc      =*/ mPWMO1_IRQ,    /* Interrupt source is Timer interrupt */
        /*.intrPriority =*/ 3UL          /* Interrupt priority is 3 */
    };

    SwPWMFreq = Hz;
    SwPWMPeriod = (SWPWM_BASE_HZ / Hz);
    mTIMER50K_config.period = 79U;  /*(4M/50K)-1=79*/  /*time base is 50KHZ*/
    
    /*Step1: timer hw init*/
    /*Initialize the TCPWM component in timer/counter mode. The return value of the
         * function indicates whether the arguments are valid or not.*/
    result = Cy_TCPWM_Counter_Init(mPWMO1_HW, mPWMO1_NUM, &mTIMER50K_config);

    if(result != CY_TCPWM_SUCCESS)
    {
        return FUNC_RETURN_NG;
    }

    /*Set the timer period in milliseconds. To count N cycles, period should be set to N-1.*/
    /*Cy_TCPWM_Counter_SetPeriod(mTIMER1MS_HW, mTIMER1MS_NUM, TIMER_PERIOD_MSEC - 1)*/


    /*Step2: interrupt handler */
    /*Interrupt handler define*/
    result = Cy_SysInt_Init(&intrCfg, &SWPWM_Interrupt);

    if(result != CY_SYSINT_SUCCESS)
    {
        return FUNC_RETURN_NG;
    }

    /*Step3: timer config setting.*/
    /* Check if the desired interrupt is enabled prior to triggering*/
    if(0UL != (CY_TCPWM_INT_ON_TC & Cy_TCPWM_GetInterruptMask(TCPWM, mPWMO1_NUM)))
    {
        Cy_TCPWM_SetInterrupt(mPWMO1_HW, mPWMO1_NUM, CY_TCPWM_INT_ON_TC);
    }

    /*Step4: timer enable*/
    NVIC_EnableIRQ(intrCfg.intrSrc);    /* Enable Interrupt */
    Cy_TCPWM_Counter_Enable(mPWMO1_HW, mPWMO1_NUM);

    /*Trigger a software start on the counter instance. This is required when
    * no other hardware input signal is connected to the component to act as
    * a trigger source.*/
    Cy_TCPWM_TriggerStart(mPWMO1_HW, mPWMO1_MASK);

    for(int i = 0; i < SWPWMO_CH; i++)
    {
        SwPWMEnable[i] = 0U;
    }


    return FUNC_RETURN_OK;

}

/*******************************************************************************
* \brief Function Name: PWMO_Init
********************************************************************************
*
* Summary:
*   Freq. base  : 4MHz
*   perial         : 79    => (4M/50K)-1=79
*   timer          : TCPWM7  counter mode
*   Duty           : default 0, renage:0~100
*   PWM PIN    : user define , max=8

* Return:
*   Status of initialization
*
*******************************************************************************/
t_FUNC_RETURN SWPWMO_PinInit(uint8_t ch, uint8_t PwmPin)
{
    if(ch < SWPWMO_CH)
    {
        SetPwmPin[ch] = PwmPin;
        SwPWMEnable[ch] = 0xAAU;
        SwPWMPinLevel[ch] = 0U;
        Gpio_PinConfig(PwmPin, CY_GPIO_DM_STRONG, 0);
        return FUNC_RETURN_OK;
    }
    else
    {
        return FUNC_RETURN_NG;
    }
}

/*******************************************************************************
* \brief Function Name: PWMO_SetFreq
********************************************************************************
*
* Summary:
*   Freq. base  : 4MHz
*   perial         : 79    => (4M/50K)-1=79
*   timer          : TCPWM7  counter mode
*   Duty           : default 0, renage:0~100
*   PWM PIN    : user define, max=8
*   PWM Freq  :  Fmax= 50K/100 = 500,   Fmin=50K/65535=0.6
*
* Return:
*   t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN SWPWMO_SetFreq(uint8_t ch,  uint16_t Hz)
{
    if(ch < SWPWMO_CH)
    {
        if(Hz != SwPWMFreq)
        {
            SwPWMFreq = Hz;
            SwPWMPeriod = (SWPWM_BASE_HZ / Hz);
            SwPWMNewHiDutyCompre[ch] = ((SwPWMPeriod * SwPWMDuty[ch]) / 100U);
        }

        return FUNC_RETURN_OK;
    }
    else
    {
        return FUNC_RETURN_NG;
    }
}



/*******************************************************************************
* \brief Function Name: PWMO_SetDuty
********************************************************************************
*
* Summary:
*   Freq. base  : 4MHz
*   perial         : 79    => (4M/50K)-1=79
*   timer          : TCPWM7  counter mode
*   Duty           : default 0, renage:0~100
*   PWM PIN    : user define, max=8

* Return:
*   t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN SWPWMO_SetDuty(uint8_t ch, uint8_t duty)
{
    if(ch < SWPWMO_CH)
    {
        if(duty >  100U)
        {
            duty =  100U;
        }

        if(SwPWMDuty[ch] !=  duty)
        {
            SwPWMDuty[ch] =  duty;
            /*SwPWMHiDutyCompre[ch] = ((SwPWMPeriod*duty)/100U)*/
            SwPWMNewHiDutyCompre[ch] = ((SwPWMPeriod * duty) / 100U);
        }

        return FUNC_RETURN_OK;
    }
    else
    {
        return FUNC_RETURN_NG;
    }
}

t_SWPWM_VALUE SWPWMO_GetPwmDuty(uint8_t ch)
{
    t_SWPWM_VALUE pwm_value;

    if((SWPWMO_CH > ch) && (SwPWMEnable[ch] == 0xAAU))
    {
        pwm_value.Frequency = SwPWMFreq;
        pwm_value.Duty = SwPWMDuty[ch];
    }
    else
    {
        pwm_value.Frequency = 0U;
        pwm_value.Duty = 0U;
    }

    return (pwm_value);
}
