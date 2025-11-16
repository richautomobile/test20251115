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
#include "debounce.h"
#include "Gpio.h"
#include "systick.h"

/* ==[ Macros ]============================================================== */
#define DEBOUNCE_DELAY_MS    20U   /*20ms*/
#define LONG_PRESS_TIME_MS   100U  /*10ms*100*/

/* ==[ Types ]=============================================================== */
/* ==[ Constants with external linkage ]===================================== */
/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with internal linkage ]====================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]====================== */
/*
*  \brief  Debounce_GetPort
*
*     loop time = 1ms
*
*
* uint8_t PortVal : detect port
* uint8_t Mask    : Hi bit=enable, Low bit=disable
* return:
* uint8_t : stable port value
*/
uint8_t Debounce_GetPort(uint8_t Port, uint8_t Mask)
{
    static uint8_t lastState = 0xFFU;
    static uint8_t stableState = 0xFFU;
    static uint32_t lastDebounceTime = 0U;
    static bool checkTimeout = false;
    uint32_t currentSystickCnt = Systick_GetMsCnt(); /*TCPWM5_GetMsCnt()*/
    uint8_t currentState = (Gpio_GetPort(Port) & Mask);

    if(currentState != lastState)
    {
        lastDebounceTime = currentSystickCnt;
        checkTimeout = true;
    }

    if(checkTimeout == true)
    {
    	if((currentSystickCnt - lastDebounceTime) > DEBOUNCE_DELAY_MS)
        {

            checkTimeout = false;
            if(currentState != stableState)
            {
                stableState = currentState;
            }
        }
    }

    lastState = currentState;
    return stableState;
}

