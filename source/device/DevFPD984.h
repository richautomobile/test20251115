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


#ifndef _DEV_FPD984_H_
#define _DEV_FPD984_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "project.h"
#include "TypeDefine.h"

#ifdef EN_DEV_FPD984
/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN FPD984_SetI2cAddr(uint8_t DevAddr7b);
extern t_FUNC_RETURN FPD984_WriteI2C(uint8_t reg, uint8_t wData);
extern uint8_t FPD984_ReadI2C(uint8_t reg);
extern t_FUNC_RETURN FPD984_SubPageWrite(uint8_t page, uint8_t reg, uint8_t wData);
extern uint8_t FPD984_SubPageRead(uint8_t page, uint8_t reg);
extern uint32_t FPD984_APBRead(uint8_t vp_ch, uint16_t addr16b);
extern t_FUNC_RETURN FPD984_APBWrite(uint8_t vp_ch, uint16_t addr16b, uint32_t data32b);
extern t_FUNC_RETURN FPD984_SetBistEnable(bool bistEnable);
extern t_FUNC_RETURN FPD984_AutoScroll( uint8_t EnAutoScroll);
extern t_FUNC_RETURN FPD984_SetPattern(uint8_t VP0PatNo, uint8_t VP1PatNo);
extern t_FUNC_RETURN FPD984_SetPatternGrayLevel(uint16_t R, uint16_t G, uint16_t B);
extern t_FUNC_RETURN FPD984_I2c_1Mhz(void);
extern t_FUNC_RETURN FPD984_CLEAR_CRCECC(uint8_t STREAM);
extern t_FUNC_RETURN FPD984_FullRST(void);
#endif

#endif
