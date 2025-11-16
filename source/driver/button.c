/**
 * @file Button.c
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
#include "button.h"
#include "Gpio.h"
#include "systick.h"

/* ==[ Macros ]============================================================== */
#define DEBOUNCE_DELAY_MS    20U
#define LONG_PRESS_TIME_MS   1000U
#define ButtomNormState      NormalHiLevel
/* ==[ Types ]=============================================================== */
/* ==[ Constants with external linkage ]===================================== */
/* ==[ Variables with external linkage ]===================================== */
static uint8_t BtnHoldState = 0U;
static uint8_t BtnLastState = 0xFFU;
static uint8_t BtnStableState = 0xFFU;
static uint32_t BtnLastDebounceTime = 0U;

/* ==[ Declaration of functions with internal linkage ]====================== */
static t_FUNC_RETURN Button_BtnHoldStateDet(uint8_t PortVal);

/* ==[ Definition of functions with internal linkage ]====================== */
/*
*  \brief  Button_BtnHoldStateDet
*
*     loop time = 10ms
*     1 times get 1 bit state. P4.6 P4.7
*
* uint8_t PortVal : detect port
* uint8_t bitNo    : 0~7
* uint8_t TimeTick10ms
*/
static t_FUNC_RETURN Button_BtnHoldStateDet(uint8_t PortVal)
{
    static uint16_t TimeCount[8] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};
    uint8_t check_val[8] = {0x01U, 0x02U, 0x04U, 0x08U, 0x10U, 0x20U, 0x40U, 0x80U};

    for(int i = 0; i < 8; i++)
    {
        if((PortVal & check_val[i]) == 0U)
        {
            TimeCount[i] = 0U;
            BtnHoldState = BtnHoldState & (~check_val[i]) ;
        }
        else
        {
            if((BtnHoldState & check_val[i]) == 0U)
            {
                if(TimeCount[i] >= LONG_PRESS_TIME_MS)
                {
                    TimeCount[i] = LONG_PRESS_TIME_MS;
                    BtnHoldState = (BtnHoldState | check_val[i]);
                }
                TimeCount[i] = TimeCount[i] + DEBOUNCE_DELAY_MS;
            }
        }
    }

    return FUNC_RETURN_OK;
}

/* ==[ Definition of functions with external linkage ]====================== */
/*
*  \brief  Button_RoutineCall
*
*     loop time = 1ms
*
*
* uint8_t PortVal : detect port
* uint8_t Mask    : Hi bit=enable, Low bit=disable
* return:
* uint8_t : stable port value
*/
uint8_t Button_RoutineCall(uint8_t Port, uint8_t Mask)
{
    uint32_t currentMsCnt = Systick_GetMsCnt(); /*TCPWM5_GetMsCnt()*/
#if (ButtomNormState == NormalHiLevel)
    uint8_t currentState = ((Gpio_GetPort(Port) & Mask)^Mask);
#else
    uint8_t currentState = (Gpio_GetPort(Port) & Mask);
#endif
    uint8_t checkPortValue;
    static bool checkTimeout = false;
    static uint32_t lastHoldCheckTime=0U;

    /*Step1: */
    if(currentState != BtnLastState)
    {
        BtnLastDebounceTime = currentMsCnt;
        checkTimeout = true;
    }

    if(checkTimeout == true)
    {
        if((currentMsCnt - BtnLastDebounceTime) >= DEBOUNCE_DELAY_MS)
        {
            checkTimeout = false;
            if(currentState != BtnStableState)
            {
                BtnStableState = currentState;
            }
        }
    }

    if((currentMsCnt - lastHoldCheckTime) >= DEBOUNCE_DELAY_MS)
    {
        lastHoldCheckTime = currentMsCnt;       
        checkPortValue = BtnStableState;        
        Button_BtnHoldStateDet(checkPortValue);
    }

    BtnLastState = currentState;
    return BtnStableState;
}

uint8_t Button_GetBtnState(void)
{
    return BtnStableState;
}

uint8_t Button_GetBtnHoldState(void)
{
    return BtnHoldState;
}



