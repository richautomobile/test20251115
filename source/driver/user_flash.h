/**
 * @file timer.h
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

#ifndef _USER_FLASH_H_
#define _USER_FLASH_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "project.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================== */
#define FlashInitValus   0x00U     /*Init. value for PSOC*/
/* ==[ Types ]=============================================================== */
/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with external linkage ]====================== */
extern uint16_t UserFlash_GetPackageSize(void);
extern t_FUNC_RETURN UserFlash_Init(uint16_t UserDefSize);
extern t_FUNC_RETURN UserFlash_GetUserData(uint8_t RBuff[], uint16_t rLen);
extern t_FUNC_RETURN UserFlash_GetPureUserData(uint8_t RBuff[], uint16_t rLen);
extern t_FUNC_RETURN UserFlash_GetUserDataByIdx(uint8_t RBuff[], uint16_t rLen, uint16_t Idx);
extern t_FUNC_RETURN UserFlash_GetUserDataByAddr(uint8_t RBuff[], uint32_t SAddr, uint16_t rLen);
extern t_FUNC_RETURN UserFlash_Write(uint8_t *WBuff, uint16_t WLen, uint8_t ForceUpdate);
extern t_FUNC_RETURN UserFlash_ClearAll(void);
extern t_FUNC_RETURN UserFlash_ForceSave(void);
#endif /* _USER_FLASH_H_ */
