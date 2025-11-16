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
#include "pwm_capture.h"

/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static cy_stc_sysint_t mPWMCAP0_cfg =
{
    /*.intrSrc      =*/ mPWMCAP0_IRQ,   /* Interrupt source */
    /*.intrPriority =*/ 3UL       /* Interrupt priority is 3 */
};
static cy_stc_sysint_t mPWMCAP1_cfg =
{
    /*.intrSrc      =*/ mPWMCAP1_IRQ,   /* Interrupt source */
    /*.intrPriority =*/ 3UL       /* Interrupt priority is 3 */
};
static cy_stc_sysint_t mPWMCAP2_cfg =
{
    /*.intrSrc      =*/ mPWMCAP2_IRQ,   /* Interrupt source */
    /*.intrPriority =*/ 3UL       /* Interrupt priority is 3 */
};
static cy_stc_sysint_t mPWMCAP3_cfg =
{
    /*.intrSrc      =*/ mPWMCAP3_IRQ,   /* Interrupt source */
    /*.intrPriority =*/ 3UL       /* Interrupt priority is 3 */
};

static uint32_t DutyHiCNT[4];
/*static uint32_t DutyLowCNT[4];*/
static uint32_t Freq[4];
static uint8_t  Duty[4];

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]======================= */
/*******************************************************************************
* \brief Function Name: PWMCap_Init
********************************************************************************
*
* Summary:
*
*
* Return:
*   Status of initialization
*
*******************************************************************************/
t_FUNC_RETURN PWMCap_Init(void)
{
    /*t_FUNC_RETURN status = FUNC_RETURN_NG*/
    cy_en_tcpwm_status_t result;
    cy_en_sysint_status_t int_result;

    /*Step 1: */
    /* Initialize Counter with config set in Peripheral */
    result  = Cy_TCPWM_Counter_Init(mPWMCAP0_HW, mPWMCAP0_NUM, &mPWMCAP0_config);
    result |= Cy_TCPWM_Counter_Init(mPWMCAP1_HW, mPWMCAP1_NUM, &mPWMCAP1_config);
    result |= Cy_TCPWM_Counter_Init(mPWMCAP2_HW, mPWMCAP2_NUM, &mPWMCAP2_config);
    result |= Cy_TCPWM_Counter_Init(mPWMCAP3_HW, mPWMCAP3_NUM, &mPWMCAP3_config);

    if(CY_TCPWM_SUCCESS != result)
    {
        /*CY_ASSERT (CY_ASSERT_FAILED)*/
        return FUNC_RETURN_NG;
    }

    /* Hook up and enable interrupt */
    int_result  = Cy_SysInt_Init(&mPWMCAP0_cfg, &mPWMCAP0_Interrupt);
    int_result |= Cy_SysInt_Init(&mPWMCAP1_cfg, &mPWMCAP1_Interrupt);
    int_result |= Cy_SysInt_Init(&mPWMCAP2_cfg, &mPWMCAP2_Interrupt);
    int_result |= Cy_SysInt_Init(&mPWMCAP3_cfg, &mPWMCAP3_Interrupt);

    if(int_result != CY_SYSINT_SUCCESS)
    {
        return FUNC_RETURN_NG; /*CY_ASSERT(CY_ASSERT_FAILED)*/
    }

    /*enable interrupt*/
    NVIC_ClearPendingIRQ(mPWMCAP0_cfg.intrSrc);
    NVIC_EnableIRQ(mPWMCAP0_cfg.intrSrc);
    NVIC_ClearPendingIRQ(mPWMCAP1_cfg.intrSrc);
    NVIC_EnableIRQ(mPWMCAP1_cfg.intrSrc);
    NVIC_ClearPendingIRQ(mPWMCAP2_cfg.intrSrc);
    NVIC_EnableIRQ(mPWMCAP2_cfg.intrSrc);
    NVIC_ClearPendingIRQ(mPWMCAP3_cfg.intrSrc);
    NVIC_EnableIRQ(mPWMCAP3_cfg.intrSrc);

    /*Step2:*/
    /* Initialize 100KHZ timer for capture count base. */
    result  = Cy_TCPWM_Counter_Init(mPWMCAP_TIMER100KHz_HW, mPWMCAP_TIMER100KHz_NUM, &mPWMCAP_TIMER100KHz_config);

    if(CY_TCPWM_SUCCESS != result)
    {
        return FUNC_RETURN_NG;  /*CY_ASSERT(CY_ASSERT_FAILED)*/
    }

    /*Step3: enable capture*/
    /* Enable all Peripherals */
    Cy_TCPWM_Counter_Enable(mPWMCAP0_HW, mPWMCAP0_NUM);
    Cy_TCPWM_Counter_Enable(mPWMCAP1_HW, mPWMCAP1_NUM);
    Cy_TCPWM_Counter_Enable(mPWMCAP2_HW, mPWMCAP2_NUM);
    Cy_TCPWM_Counter_Enable(mPWMCAP3_HW, mPWMCAP3_NUM);
    Cy_TCPWM_Counter_Enable(mPWMCAP_TIMER100KHz_HW, mPWMCAP_TIMER100KHz_NUM);

    /*Step4: start trigger*/
    /* Triggers a software start on the selected TCPWMs */
    Cy_TCPWM_TriggerStart(mPWMCAP0_HW, mPWMCAP0_MASK);
    Cy_TCPWM_TriggerStart(mPWMCAP1_HW, mPWMCAP1_MASK);
    Cy_TCPWM_TriggerStart(mPWMCAP2_HW, mPWMCAP2_MASK);
    Cy_TCPWM_TriggerStart(mPWMCAP3_HW, mPWMCAP3_MASK);
    Cy_TCPWM_TriggerStart(mPWMCAP_TIMER100KHz_HW, mPWMCAP_TIMER100KHz_MASK);

    return FUNC_RETURN_OK;
}

/*******************************************************************************
* \brief Function Name: mPWMCAP0_Interrupt
******************************************************************************
*
* Summary:
*   Invokes the Cy_SCB_I2C_MasterInterrupt() PDL driver function.
*   PWM rising will reload count.
*
*#define     CY_TCPWM_INT_NONE   (0U)   :No Interrupt.
*#define     CY_TCPWM_INT_ON_TC  (1U)   :Interrupt on Terminal count(TC)
*#define     CY_TCPWM_INT_ON_CC  (2U)   :Interrupt on Compare/Capture(CC)
*#define     CY_TCPWM_INT_ON_CC_OR_TC   (3U) :Interrupt on TC or CC.
*
*   P0[0]: rising edge reload
*   Counter base: 100KHz
*      ____      ____
*   __|    |____|    |___
*     |CAP1|
*     |  CAP2   |
*
*     Duty= CAP1/CAP2,  Freq= 100K/CAP2
*
*
*******************************************************************************/
void mPWMCAP0_Interrupt(void)
{
    /* Get which event triggered the interrupt */
    uint32_t cap_value = Cy_TCPWM_Counter_GetCapture(mPWMCAP0_HW, mPWMCAP0_NUM);
    uint32_t pin_value = Cy_GPIO_Read(PWMCAP0_PORT, PWMCAP0_NUM);
    uint32_t int_state = Cy_TCPWM_GetInterruptStatus(mPWMCAP0_HW, mPWMCAP0_NUM);
    uint32_t tmpVal;
    /* Clear interrupt */
    Cy_TCPWM_ClearInterrupt(mPWMCAP0_HW, mPWMCAP0_NUM, CY_TCPWM_INT_ON_CC_OR_TC);

    /* If the interrupt is triggered by capture event then set the flag for
     * frequency calculation
     */
    if(int_state == CY_TCPWM_INT_ON_CC)   /*normal capture.*/
    {
        if(0U < pin_value)
        {
            /*DutyLowCNT[0] = cap_value - DutyHiCNT[0];*/
            tmpVal = (DutyHiCNT[0] * 1000U) / cap_value; /*rounding*/
            Duty[0] = (uint8_t)((tmpVal + 5U) / 10);
            Freq[0] = 100000 / cap_value;
        }
        else
        {
            DutyHiCNT[0] = cap_value;
        }
    }

    /* If the interrupt is triggered by an overflow event, then counting how
     * many times counter overflow happened in one second
     */
    if(int_state == CY_TCPWM_INT_ON_TC)  /*overflow*/
    {
        if(0U < pin_value)
        {
            Duty[0] = 100U;
        }
        else
        {
            Duty[0] = 0U;
        }
    }
}

/*******************************************************************************
* \brief Function Name: mPWMCAP2_Interrupt
******************************************************************************
*
* Summary:
*   Invokes the Cy_SCB_I2C_MasterInterrupt() PDL driver function.
*   P0[1]: rising edge reload
*   Counter base: 100KHz
*      ____      ____
*   __|    |____|    |___
*     |CAP1|
*     |  CAP2   |
*
*     Duty= CAP1/CAP2,  Freq= 100K/CAP2
*
*******************************************************************************/
void mPWMCAP1_Interrupt(void)
{
    /* Get which event triggered the interrupt */
    uint32_t cap_value = Cy_TCPWM_Counter_GetCapture(mPWMCAP1_HW, mPWMCAP1_NUM);
    uint32_t pin_value = Cy_GPIO_Read(PWMCAP1_PORT, PWMCAP1_NUM);
    uint32_t int_state = Cy_TCPWM_GetInterruptStatus(mPWMCAP1_HW, mPWMCAP1_NUM);
    uint32_t tmpVal;
    /* Clear interrupt */
    Cy_TCPWM_ClearInterrupt(mPWMCAP1_HW, mPWMCAP1_NUM, CY_TCPWM_INT_ON_CC_OR_TC);

    /* If the interrupt is triggered by capture event then set the flag for
     * frequency calculation
     */
    if(int_state == CY_TCPWM_INT_ON_CC)   /*normal capture.*/
    {
        if(0U < pin_value)
        {
            /*DutyLowCNT[1] = cap_value - DutyHiCNT[1];*/
            tmpVal = (DutyHiCNT[1] * 1000U) / cap_value; /*rounding*/
            Duty[1] = (uint8_t)((tmpVal + 5U) / 10);
            Freq[1] = 100000 / cap_value;
        }
        else
        {
            DutyHiCNT[1] = cap_value;
        }
    }

    /* If the interrupt is triggered by an overflow event, then counting how
     * many times counter overflow happened in one second
     */
    if(int_state == CY_TCPWM_INT_ON_TC)  /*overflow*/
    {
        if(0U < pin_value)
        {
            Duty[1] = 100U;
        }
        else
        {
            Duty[1] = 0U;
        }
    }
}

/*******************************************************************************
* \brief Function Name: mPWMCAP2_Interrupt
******************************************************************************
*
* Summary:
*   Invokes the Cy_SCB_I2C_MasterInterrupt() PDL driver function.
*   P1[2]: rising edge reload
*   Counter base: 100KHz
*      ____      ____
*   __|    |____|    |___
*     |CAP1|
*     |  CAP2   |
*
*     Duty= CAP1/CAP2,  Freq= 100K/CAP2
*
*******************************************************************************/
void mPWMCAP2_Interrupt(void)
{
    /* Get which event triggered the interrupt */
    uint32_t cap_value = Cy_TCPWM_Counter_GetCapture(mPWMCAP2_HW, mPWMCAP2_NUM);
    uint32_t pin_value = Cy_GPIO_Read(PWMCAP2_PORT, PWMCAP2_NUM);
    uint32_t int_state = Cy_TCPWM_GetInterruptStatus(mPWMCAP2_HW, mPWMCAP2_NUM);
    uint32_t tmpVal;

    /* Clear interrupt */
    Cy_TCPWM_ClearInterrupt(mPWMCAP2_HW, mPWMCAP2_NUM, CY_TCPWM_INT_ON_CC_OR_TC);

    /* If the interrupt is triggered by capture event then set the flag for
     * frequency calculation
     */
    if(int_state == CY_TCPWM_INT_ON_CC)   /*normal capture.*/
    {
        if(0U < pin_value)
        {
            /*DutyLowCNT[2] = cap_value - DutyHiCNT[2];*/
            tmpVal = (DutyHiCNT[2] * 1000U) / cap_value; /*rounding*/
            Duty[2] = (uint8_t)((tmpVal + 5U) / 10);
            Freq[2] = 100000 / cap_value;
        }
        else
        {
            DutyHiCNT[2] = cap_value;
        }
    }

    /* If the interrupt is triggered by an overflow event, then counting how
     * many times counter overflow happened in one second
     */
    if(int_state == CY_TCPWM_INT_ON_TC)  /*overflow*/
    {
        if(0U < pin_value)
        {
            Duty[2] = 100U;
        }
        else
        {
            Duty[2] = 0U;
        }
    }
}

/*******************************************************************************
* \brief Function Name: mPWMCAP3_Interrupt
******************************************************************************
*
* Summary:
*   Invokes the Cy_SCB_I2C_MasterInterrupt() PDL driver function.
*   P1[3]: rising edge reload
*   Counter base: 100KHz
*      ____      ____
*   __|    |____|    |___
*     |CAP1|
*     |  CAP2   |
*
*     Duty= CAP1/CAP2,  Freq= 100K/CAP2
*
*******************************************************************************/
void mPWMCAP3_Interrupt(void)
{
    /* Get which event triggered the interrupt */
    uint32_t cap_value = Cy_TCPWM_Counter_GetCapture(mPWMCAP3_HW, mPWMCAP3_NUM);
    uint32_t pin_value = Cy_GPIO_Read(PWMCAP3_PORT, PWMCAP3_NUM);
    uint32_t int_state = Cy_TCPWM_GetInterruptStatus(mPWMCAP3_HW, mPWMCAP3_NUM);
    uint32_t tmpVal;
    /* Clear interrupt */
    Cy_TCPWM_ClearInterrupt(mPWMCAP3_HW, mPWMCAP3_NUM, CY_TCPWM_INT_ON_CC_OR_TC);

    /* If the interrupt is triggered by capture event then set the flag for
     * frequency calculation
     */
    if(int_state == CY_TCPWM_INT_ON_CC)   /*normal capture.*/
    {
        if(0U < pin_value)
        {
            /*DutyLowCNT[3] = cap_value - DutyHiCNT[3];*/
            tmpVal = (DutyHiCNT[3] * 1000U) / cap_value; /*rounding*/
            Duty[3] = (uint8_t)((tmpVal + 5U) / 10);
            Freq[3] = 100000 / cap_value;
        }
        else
        {
            DutyHiCNT[3] = cap_value; /*CAP1*/
        }
    }

    /* If the interrupt is triggered by an overflow event, then counting how
     * many times counter overflow happened in one second
     */
    if(int_state == CY_TCPWM_INT_ON_TC)  /*overflow*/
    {
        if(0U < pin_value)
        {
            Duty[3] = 100U;
        }
        else
        {
            Duty[3] = 0U;
        }
    }
}


t_PWM_VALUE PWMCap_GetPWMVal(uint8_t ch)
{
    t_PWM_VALUE pwm_value;

    if(4U > ch)
    {
        pwm_value.Frequency = Freq[ch];
        pwm_value.Duty = Duty[ch];
    }
    else
    {
        pwm_value.Frequency = 0U;
        pwm_value.Duty = 0U;
    }

    return (pwm_value);
}
