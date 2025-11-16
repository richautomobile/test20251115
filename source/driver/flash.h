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

#ifndef _FLASH_H_
#define _FLASH_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================== */
/* ==[ Types ]=============================================================== */
/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with external linkage ]====================== */
extern t_FUNC_RETURN Flash_WriteRow(uint32_t SAddr, const uint8_t *WBuff);
#endif /* FLASH_H_ */
