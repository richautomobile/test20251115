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


#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "project.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern uint16_t Sys_GetAppFWVer(void);
extern uint16_t Sys_GetBootFWVer(void);
extern uint16_t Sys_GetHWVer(void);
extern uint16_t Sys_GetSessionKey(void);
extern uint8_t Sys_GetBootReason(void);
extern uint8_t Sys_GetUserRam(uint8_t RamData[], uint8_t RLen);
extern t_FUNC_RETURN Sys_SetBootFWVer(uint16_t val);
extern t_FUNC_RETURN Sys_SetSessionKey(uint16_t val);
extern t_FUNC_RETURN Sys_VariableInit(void);
extern t_FUNC_RETURN Sys_HwInit(void);
extern t_FUNC_RETURN Sys_ProjectInit(void);
extern t_FUNC_RETURN Sys_RoutineCall(void);
extern t_FUNC_RETURN Sys_SoftReset(void);
extern void Sys_Uart_Print(char *str, uint32_t length);
extern t_FUNC_RETURN Sys_GetPsocSiliconID(uint8_t PsocID[]);
#endif
