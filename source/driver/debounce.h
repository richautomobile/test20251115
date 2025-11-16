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

#ifndef _DEBOUNCE_H_
#define _DEBOUNCE_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "system.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================== */
/* ==[ Types ]=============================================================== */
/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with external linkage ]====================== */
extern uint8_t Debounce_GetPort(uint8_t Port, uint8_t Mask);
#endif /* _DEBOUNCE_H_ */
