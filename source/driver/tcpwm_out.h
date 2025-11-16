/**
 * \file
 *
 * \brief Brief description of the header file.
 *
 * Provide a detailed description of the header file.
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


#ifndef _TCPWM_OUT_H_
#define _TCPWM_OUT_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
typedef struct {
    uint32_t  Frequency;
    uint8 Duty;
}t_TCPWM_VALUE;
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN TCPWMO_Init(uint8_t ch, uint32_t Hz);
extern t_FUNC_RETURN TCPWMO_SetShadowPin(uint8_t ch, uint8 pin);
extern t_FUNC_RETURN TCPWMO_SetFreq(uint8_t ch, uint16_t Hz);
extern t_FUNC_RETURN TCPWMO_SetDuty(uint8_t ch, uint8_t duty);
extern t_TCPWM_VALUE TCPWMO_GetPwmDuty(uint8_t ch);

#endif
