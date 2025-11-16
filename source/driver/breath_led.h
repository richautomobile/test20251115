/**
 * @file Gpio.h
 *
 * @brief
 *
 * Copyright (C) 2022 Innolux Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Innolux Corporation
 *
 * MISRA-C:2012 Deviation Type definition
 *
 * MISRA-2:
 * Deviated Rule:
 *
 * @author BingHan (binghan.chou@innolux.com)
 * @date 2022-11-10
 */

#ifndef _BREATH_LED_H_
#define _BREATH_LED_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "system.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================== */
#define DEF_BREATH_LED_COUNT  31U //time tick 1ms  //55U //=>time tick 500us 
/* ==[ Types ]=============================================================== */
/* ==[ Variables with external linkage ]===================================== */

/* ==[ Declaration of functions with external linkage ]====================== */
extern t_FUNC_RETURN BreathLED_Init(uint8_t PortPin);
extern t_FUNC_RETURN BreathLED_RoutineCall(void);
#endif /* GPIO_H_ */
