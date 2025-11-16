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


#ifndef _DEV_FPD983_H_
#define _DEV_FPD983_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "project.h"
#include "TypeDefine.h"

#ifdef EN_OTP_TPS55289
/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN TPS55289_SetI2cAddr(uint8_t DevAddr7b);
extern t_FUNC_RETURN TPS55289_WriteI2C(uint8_t reg, uint8_t wData);
extern uint8_t TPS55289_ReadI2C(uint8_t reg);
extern t_FUNC_RETURN TPS55289_SetOTPVolt(uint16_t mV);
extern t_FUNC_RETURN TPS55289_OE(uint8_t OnOff);
#endif

#endif
