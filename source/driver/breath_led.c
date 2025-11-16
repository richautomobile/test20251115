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
#include "breath_led.h"
#include "gpio.h"

/* ==[ Macros ]============================================================== */
/* ==[ Types ]=============================================================== */
/* ==[ Constants with external linkage ]===================================== */
/* ==[ Variables with external linkage ]===================================== */
static uint8_t SetPin = 0xFF;
static uint8_t BreathLedDir = 0U;  /*0:dark to light, 1:light to dark*/
static uint8_t PinOutState = 0U;
static uint16_t BreathLedCntL1 = 0U;
static uint16_t BreathLedCntL2 = 0U;
/* ==[ Declaration of functions with internal linkage ]====================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]====================== */
/* \brief  File Name: BreathLED_Init
*
*
*parpaeter:
*  uint8_t :  setting output pin
*
*return:
*   t_FUNC_RETURN
*/
t_FUNC_RETURN BreathLED_Init(uint8_t PortPin)
{
    SetPin = PortPin;
    PinOutState = 0U;
    BreathLedCntL1 = 0U;
    BreathLedCntL2 = 0U;
    BreathLedDir = 0U;
    return FUNC_RETURN_OK;
}


/*  \brief  File Name: BreathLED_RoutineCall
*
*
*parpaeter:
*  NONE
*
*return:
*   t_FUNC_RETURN
*/
t_FUNC_RETURN BreathLED_RoutineCall(void)
{
    t_FUNC_RETURN rtnState;
    uint8_t curr_out = 0U;

    if(SetPin != 0xFFU)
    {
        rtnState = FUNC_RETURN_OK;

        /*Step 1: set gpio output level.*/
        if(BreathLedCntL2 > BreathLedCntL1)
        {
            /*
                         BreathLedDir=0  =>   light to dark
                         BreathLedDir=1  =>   dark to light
                         */
            curr_out = (BreathLedDir ^ 1U);
        }
        else
        {
            /*
                         BreathLedDir=0  =>   dark to light
                         BreathLedDir=1  =>   light to dark
                         */
            curr_out = (BreathLedDir ^ 0U);
        }

        /*gpio output*/
        if(PinOutState != curr_out)
        {
            PinOutState = curr_out;
            Gpio_WritePin(SetPin, curr_out);
        }

        /*Step 2: set loop 1 and loop 2 count*/
        BreathLedCntL1++;
        
        if(BreathLedCntL1 >= DEF_BREATH_LED_COUNT)
        {
            BreathLedCntL1 = 0U;


            /*Step 2: set loop 2 count and direction*/
            BreathLedCntL2++;

            if(BreathLedCntL2 >= DEF_BREATH_LED_COUNT)
            {
                BreathLedCntL2 = 0U;
                BreathLedDir = BreathLedDir ^ 1U;
            }

        }
    }
    else
    {
        rtnState = FUNC_RETURN_NG;
    }

    return rtnState;
}

