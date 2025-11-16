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


#ifndef _QUAD_DEC_H_
#define _QUAD_DEC_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================= */
#define ENCODER_CW     CY_TCPWM_QUADDEC_STATUS_DOWN_COUNTING
#define ENCODER_CCW    CY_TCPWM_QUADDEC_STATUS_UP_COUNTING
#define ENCODER_NOCHANGE 0U

/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN QuadDec_Init(void);
extern uint32_t  QuadDec_GetCount(void);
extern uint8_t  QuadDec_GetDir(void);
extern t_FUNC_RETURN QuadDec_ReInit(void);
#endif
