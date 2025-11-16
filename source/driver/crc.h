/**
 * @file Gpio.h
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

#ifndef _CRC_H_
#define _CRC_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================== */
/* ==[ Types ]=============================================================== */
/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with external linkage ]====================== */
#if (SW_OR_HW_CRC == SW_CRC)
extern t_FUNC_RETURN Crc_CalCrc32(uint8_t *message, uint32_t dataLen, uint32_t *calculatedCrc);
extern t_FUNC_RETURN Crc_CalCrc16(uint8_t *message, uint32_t dataLen, uint32_t *calculatedCrc);
extern t_FUNC_RETURN Crc_CalCrc8(uint8_t *message, uint32_t dataLen, uint32_t *calculatedCrc);
#else
extern t_FUNC_RETURN Crc_Init(void);
extern t_FUNC_RETURN Crc_CalCrc32(uint8_t *message, uint32_t dataLen, uint32_t *calculatedCrc);
extern t_FUNC_RETURN Crc_CalCrc16(uint8_t *message, uint32_t dataLen, uint32_t *calculatedCrc);
extern t_FUNC_RETURN Crc_CalCrc8(uint8_t *message, uint32_t dataLen, uint32_t *calculatedCrc);
#endif
#endif /* CRC_H_ */
