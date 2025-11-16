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


#ifndef _GM_VCS_H_
#define _GM_VCS_H_

/* =[ Includes ]============================= */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"
#include "project.h"

#ifdef EN_GM_VCS_API
/* =[ Macros ]============================== */
/*region GM VCS Communication table*/
#define HwVerDAddr  0x00U
#define FwVerDAddr  0x02U
#define TouchIdDAddr  0x04U
#define DisIdDAddr  0x05U
#define SwitchPackDAddr  0x08U
#define SwitchPackButtonStatus  0x0AU
#define Diagnostic_Status1  0x1AU
#define Diagnostic_Status2  0x1BU
#define Display_Backlight_PWM  0x20U
#define Display_Button_PWM1  0x25U
#define Display_Button_PWM2  0x27U
#define Display_Button_PWM3  0x29U
#define Display_Button_PWM4  0x2BU
#define ResetCntDAddr  0x2DU
#define PwrCntDAddr  0x2FU
#define ForceResetAddr  0x31U
#define SessionKeyAddr  0x34U
#define Display_Button_PWMMul1  0x36U
#define Display_Button_PWMMul2  0x37U
#define Display_Button_PWMMul3  0x38U
#define Display_Button_PWMMul4  0x39U
#define ReqLedPwmDimLevel  0x56U
#define SwPartNumAddr  0x60U
#define HwPartNumAddr  0x68U
#define BootStatusAdddr  0x80U
#define NewDiagnostic_Status1  0x16U
#define NewDiagnostic_Status2  0x1CU

#define EngReadReg            0x70U 
#define UnlockAppMemoryAdddr  0x84U
#define EraseppMemoryAdddr  0x88U
#define WriteFlashPageAdddr  0x8DU

/* =[ Types ]=============================== */
/* =[ Constants with external linkage ]================== */
/* =[ Variables with external linkage ]================== */
/* =[ Declaration of functions with external linkage ]========== */
extern t_FUNC_RETURN GmVCS_EngMode(uint8_t OnOff);
extern t_FUNC_RETURN GmVCS_WriteFactory(uint8_t SAddr, uint8_t WLen, uint8_t WData[]);
extern t_FUNC_RETURN GmVCS_ReadFactory(uint8_t SAddr, uint8_t RLen, uint8_t RData[]);
extern t_FUNC_RETURN GmVCS_MCURst(void);
extern t_FUNC_RETURN GmVCS_SetDimm(uint8_t DisplayID, uint16_t PWM);
extern t_FUNC_RETURN GmVCS_GetDimm(uint8_t DisplayID, uint16_t *pwm);
extern t_FUNC_RETURN GmVCS_GetDiag(uint8_t *Diagnostic1, uint8_t *Diagnostic2);
extern t_FUNC_RETURN GmVCS_ForceGetDiag(uint8_t *Diagnostic1, uint8_t *Diagnostic2);
extern t_FUNC_RETURN GmVCS_GetNTC123(int *NTC1, int *NTC2, int *NTC3);
extern uint16_t GmVCS_GetResetCnt(void);
extern uint16_t GmVCS_GetPwrCnt(void);
extern t_FUNC_RETURN GmVCS_GetGmCMD(uint8_t cmdID, uint8_t rLen, uint8_t rBuff[]);
extern t_FUNC_RETURN GmVCS_I2CBWrite(uint8_t Addr7bs, uint8_t WLen, uint8_t WBuff[]);
extern t_FUNC_RETURN GmVCS_I2CBRead(uint8_t Addr7bs, uint8_t WLen, uint8_t WBuff[], uint8_t RLen, uint8_t RBuff[]);
#endif
#endif
