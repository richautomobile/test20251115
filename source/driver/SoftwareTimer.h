/**
 * \file os_SoftwareTimer.h
 *
 * \brief main include file.
 *
 * Project(s): 
 *
 * Target: independent
 *
 * \author walker.chiang <walker.chiang@innolux.com>
 *
 * \date 2019/0715
 *
 * Copyright (C) 2016 Innolux Display Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Innolux Display Corporation.
 */

#ifndef _SOFTWARE_TIMER_H_
#define _SOFTWARE_TIMER_H_
/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================= */
#define MaxSWTimer   5
#define SWTimerCH0   50U    /*50ms: 50ms for Buttom Hold detect*/
#define SWTimerCH1   50U    /*50ms: for WDT clear*/
#define SWTimerCH2   10U    /*50ms: for fix count base in the main loop.*/
#define SWTimerCH3   1000U  /*1000ms*/
#define SWTimerCH4   2000U  /*2000ms*/


/*For OS define*/
typedef enum
{
    SWTimer_OFF = 0U,
    SWTimer_Routine,
    SWTimer_OneShort /*1ms base*/      
} enum_SWTimerType;

typedef struct
{
    uint8_t ID;
    enum_SWTimerType Type;
    uint32_t Tick;
    uint32_t TimeoutTime;
    uint8_t  TimeoutFlag; 
} t_struct_SWTimer_def;

/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN SWTimer_Init(void);
extern void SWTimer_Process(uint32_t count_1msec);
extern t_FUNC_RETURN SWTimer_Reset(uint8_t ch, enum_SWTimerType type, uint32_t timeout_time);
extern uint8_t SWTimer_GetTimeoutState(uint8_t ch);
extern uint8_t SWTimer_Get1SecRoutine(void);
extern uint8_t SWTimer_Get50msRoutine(void);
extern uint8_t SWTimer_Get10msTimeBase(void);
#endif
