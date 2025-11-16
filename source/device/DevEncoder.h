/**
 * \file
 *
 * \brief Brief description of the header file.
 *
 * Provide a detailed description of the header file.
 *
 * Project(s): PSOC4_Brifge_Board_App
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


#ifndef _DEV_ENCODER_H_
#define _DEV_ENCODER_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "project.h"
#include "TypeDefine.h"

#ifdef EN_DEV_ENCODER

/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN Encoder_PinInit(uint8_t EncAP1_Pin, uint8_t EncAP2_Pin, uint8_t EncBP1_Pin, uint8_t EncBP2_Pin);
extern t_FUNC_RETURN Encoder_GetPwm(uint8_t *PwmHz);
extern t_FUNC_RETURN Encoder_Gen1X(uint8_t EncNo, uint8_t dir, uint8_t Freq, uint8_t count);
extern t_FUNC_RETURN Encoder_Gen2X(uint8_t EncNo, uint8_t dir, uint8_t Freq, uint8_t count);
extern t_FUNC_RETURN Encoder_Gen4X(uint8_t EncNo, uint8_t dir, uint8_t Freq, uint8_t count);

#endif

#endif
