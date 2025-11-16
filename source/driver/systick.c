/**
 * @file Gpio.c
 *
 * @brief
 *
 * Copyright (C) 2022 Innolux Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Innolux Corporation
 *
 * MISRA-C:2012 Deviation List
 *
 * MISRA-2:
 * Deviated Rule:
 *
 * @author BingHan (binghan.chou@innolux.com)
 * @date 2022-11-10
 */


/* ==[ Includes ]============================================================ */
/*******************************************************************************
 * Include header files
 ******************************************************************************/
/*
#include "cy_pdl.h"
#include "cybsp.h"
*/
#include "systick.h"
#include "Gpio.h"
#include "SoftwareTimer.h"
#include "breath_led.h"

/* ==[ Macros ]============================================================== */
/* ==[ Types ]=============================================================== */
/* ==[ Constants with external linkage ]===================================== */
/* ==[ Variables with external linkage ]===================================== */
static uint32_t systick_ms_cnt;
//static uint16_t cnt_500us;
/* ==[ Declaration of functions with internal linkage ]====================== */
static void Systick_Handler(void);
/* ==[ Definition of functions with internal linkage ]====================== */
/* \brief System tick 1ms interrupt handler.
 *
 *  1ms Routine Call
 *  The "Cy_SysTick_GetValue()" function cannot be called here
 *
 */
static void Systick_Handler(void)
{
#if 0
    cnt_500us = cnt_500us ^ 0x0001U;
    if(cnt_500us == 0U)
    {
       SWTimer_Process(++systick_ms_cnt);
    }
#else    
    SWTimer_Process(++systick_ms_cnt);    
#endif
#if 1   /*Breath LED*/
    (void)BreathLED_RoutineCall();
#endif

}
/* ==[ Definition of functions with external linkage ]====================== */
/*
 * 1ms systemtick
 * CY_SYSTICK_CLOCK_SOURCE_CLK_LF = 40000Hz(50%). Not precise. The time error is large.
 * CY_SYSTICK_CLOCK_SOURCE_CLK_CPU = 48MHz(2%)
 * interval = (48000-1) =>  48000/48M = 1ms
 * interval = (24000-1) =>  24000/48M = 500us
 * interval = (12000-1) =>  12000/48M = 250us
 * interval = (4800-1)   =>  4800/48M = 100us
 */
t_FUNC_RETURN Systick_Init(void)
{
    systick_ms_cnt = 0U;
    //cnt_500us = 0U;
    /*tick = 1ms*/
    Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU, 47999U);
    /*tick = 100us*/
    //Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU, 4799U);
    /*tick = 500us*/
    //Cy_SysTick_Init(CY_SYSTICK_CLOCK_SOURCE_CLK_CPU, 23999U);
    Cy_SysTick_SetCallback(0, Systick_Handler);
    NVIC_SetPriority(SysTick_IRQn, 2); /*priority highest:0~3*/
    Cy_SysTick_Enable();

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Systick_Disable(void)
{
    Cy_SysTick_Disable();

    return FUNC_RETURN_OK;
}

uint32_t Systick_GetMsCnt(void)
{
    return systick_ms_cnt;
}

