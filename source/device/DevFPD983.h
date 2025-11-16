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

#ifdef EN_DEV_FPD983
/* ==[ Macros ]============================================================= */
#define Checkerboard 0
#define White 1 
#define Black 2 
#define Red 3 
#define Green 4 
#define Blue 5 
#define HorBlackToWhite 6 
#define HorBlackToRed 7 
#define HorBlackToGreen 8 
#define HorBlackToBlue 9 
#define VerBlackToWhite 10 
#define VerBlackToRed 11 
#define VerBlackToGreen 12 
#define VerBlackToBlue 13 
#define CustomColor 14 
#define VCOM 15 
#define AlternateVCOM 16 
#define CustomColorCheckerboard 17 
#define ColorBars 18 
#define MIPITestPattern 19 
#define NoPattern   0xFFU
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN FPD983_SetI2cAddr(uint8_t DevAddr7b);
extern t_FUNC_RETURN FPD983_WriteI2C(uint8_t reg, uint8_t wData);
extern uint8_t FPD983_ReadI2C(uint8_t reg);
extern t_FUNC_RETURN FPD983_SubPageWrite(uint8_t page, uint8_t reg, uint8_t wData);
extern uint8_t FPD983_SubPageRead(uint8_t page, uint8_t reg);
extern uint32_t FPD983_APBRead(uint8_t vp_ch, uint16_t addr16b);
extern t_FUNC_RETURN FPD983_APBWrite(uint8_t vp_ch, uint16_t addr16b, uint32_t data32b);
extern t_FUNC_RETURN FPD983_SetBistEnable(bool bistEnable);
extern t_FUNC_RETURN FPD983_AutoScroll( uint8_t EnAutoScroll);
extern t_FUNC_RETURN FPD983_SetPattern(uint8_t VP0PatNo, uint8_t VP1PatNo);
extern t_FUNC_RETURN FPD983_SetPatternGrayLevel(uint16_t R, uint16_t G, uint16_t B);
extern t_FUNC_RETURN FPD983_FullRST(void);
#endif

#endif
