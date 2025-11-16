/**
 * \file SoftwareTimer.c
 *
 * \brief
 *
 * Project(s): - iSchedule
 *
 * \author walker <walker.chiang@innolux.com>
 *
 * \date 2019/ 6/ 11
 *
 * Copyright (C) 2019 Innolux Display Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Innolux Display Corporation.
 */

/* ==[ Includes ]=========================================================== */
#include "SoftwareTimer.h"
#include "gpio.h"

/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static t_struct_SWTimer_def SWTimerQueue[MaxSWTimer];
static uint32_t Save_1ms_count = 0;
/* ==[ Declaration of functions with internal linkage ]===================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]======================= */
/**
 *  \brief SWTimer_Init
 *
 *  \param [in]  Name.
 *  \param [in]  task tpye
 *  \param [in]  tick time
 *  \param [in]  callback function
 *  \return  >0: ok     -1:fail
 *
 */
t_FUNC_RETURN SWTimer_Init(void)
{
    /*SWTimer CH0 : xxms for boot*/
    SWTimerQueue[0].ID   = 0U;
    SWTimerQueue[0].Type = SWTimer_OFF;
    SWTimerQueue[0].Tick = SWTimerCH0;
    SWTimerQueue[0].TimeoutTime = SWTimerCH0;
    SWTimerQueue[0].TimeoutFlag = 0U;
    /*SWTimer CH1 : 50ms  for WDT clear*/
    SWTimerQueue[1].ID   = 1U;
    SWTimerQueue[1].Type = SWTimer_OFF;
    SWTimerQueue[1].Tick = SWTimerCH1;
    SWTimerQueue[1].TimeoutTime = SWTimerCH1;
    SWTimerQueue[1].TimeoutFlag = 0U;
    /*SWTimer CH2 : 50ms for fix count base.*/
    SWTimerQueue[2].ID   = 2U;
    SWTimerQueue[2].Type = SWTimer_Routine;
    SWTimerQueue[2].Tick = SWTimerCH2;
    SWTimerQueue[2].TimeoutTime = SWTimerCH2;
    SWTimerQueue[2].TimeoutFlag = 0U;
    /*SWTimer CH3 : 1000ms*/
    SWTimerQueue[3].ID   = 3U;
    SWTimerQueue[3].Type = SWTimer_Routine;
    SWTimerQueue[3].Tick = SWTimerCH3;
    SWTimerQueue[3].TimeoutTime = SWTimerCH3;
    SWTimerQueue[3].TimeoutFlag = 0U;
    /*SWTimer CH4 : 2000ms*/
    SWTimerQueue[4].ID   = 4U;
    SWTimerQueue[4].Type = SWTimer_Routine;
    SWTimerQueue[4].Tick = SWTimerCH4;
    SWTimerQueue[4].TimeoutTime = SWTimerCH4;
    SWTimerQueue[4].TimeoutFlag = 0U;

    return FUNC_RETURN_OK;
}

/**
 *  \brief  SWTimer process.
 *
 *  \param [in]  void
 *  \return      void
 *
 */
void SWTimer_Process(uint32_t count_1msec)
{
    uint8_t i;

    Save_1ms_count = count_1msec;

    for(i = 0; i < MaxSWTimer; i++)
    {
        if(SWTimerQueue[i].Type != SWTimer_OFF)
        {
            if(SWTimerQueue[i].Tick ==  count_1msec)
            {
                if(SWTimerQueue[i].Type == SWTimer_Routine)
                {
                    SWTimerQueue[i].Tick = (uint32_t)(count_1msec + SWTimerQueue[i].TimeoutTime);
                }

                if(SWTimerQueue[i].Type == SWTimer_OneShort)
                {
                    SWTimerQueue[i].Type = SWTimer_OFF;
                }

                SWTimerQueue[i].TimeoutFlag = 1U;
            }
        }
    }
}

/**
 *  \brief  SWTimer_Reset.
 *
 *  \param [in]  ch: 0~4
 *  \param [in]  type:
 *  \param [in]  timeout_time: ms
 *  \return      t_FUNC_RETURN
 *
 */
t_FUNC_RETURN SWTimer_Reset(uint8_t ch, enum_SWTimerType type, uint32_t timeout_time)
{
    SWTimerQueue[ch].ID   = ch;
    SWTimerQueue[ch].Type = type;
    SWTimerQueue[ch].Tick = (uint32_t)(Save_1ms_count + timeout_time);
    SWTimerQueue[ch].TimeoutTime = timeout_time;
    SWTimerQueue[ch].TimeoutFlag = 0U;

    return FUNC_RETURN_OK;
}

/**
 *  \brief  SWTimer_Get1SecRoutine.
 *
 *  TimeoutFlag is cleared after reading.
 *  \param [in]  ch: 0~4
 *  \return      uint8_t: >0 timeout
 *
 */
uint8_t SWTimer_GetTimeoutState(uint8_t ch)
{
    uint8_t state = 0U;

    if(ch >= MaxSWTimer)
    {
        state = 0xFFU;
    }
    else
    {
        state = SWTimerQueue[ch].TimeoutFlag;

        if(state > 0U)
        {
            if(SWTimerQueue[ch].Type == SWTimer_Routine)
            {
                SWTimerQueue[ch].TimeoutFlag = 0U;
            }
        }
    }

    return state;
}

/**
 *  \brief  SWTimer_Get1SecRoutine.
 *
 *  \param [in]  void
 *  \return      uint8_t: 0 or 1:timeout
 *
 */
uint8_t SWTimer_Get1SecRoutine(void)
{
    return SWTimer_GetTimeoutState(3U);
}

/**
 *  \brief  SWTimer_Get50msRoutine.
 *             WDT clear tick
 *  \param [in]  void
 *  \return      uint8_t: 0 or 1:timeout
 *
 */
uint8_t SWTimer_Get50msRoutine(void)
{
    return SWTimer_GetTimeoutState(1U);
}

/**
 *  \brief  SWTimer_Get10msTimeBase.
 *             WDT clear tick
 *  \param [in]  void
 *  \return      uint8_t: 0 or 1:timeout
 *
 */
uint8_t SWTimer_Get10msTimeBase(void)
{
    return SWTimer_GetTimeoutState(2U);
}


