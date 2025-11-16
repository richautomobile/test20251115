/**
 * \file
 *
 * \brief Brief description of the header file.
 *
 * Provide a detailed description of the header file.
 *
 * Project(s): PSOC4_Brifge_Board_App PSOC4_BRIDGE_BOARD
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


#ifndef _DMA_H_
#define _DMA_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"
#include "project.h"

/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN DMA_MemToPeripheral_Init(uint8_t CHANN_NUM, uint8_t *src, uint32_t *dst, uint32_t DATA_CNT);
extern t_FUNC_RETURN DMA_PeripheralToMem_Init(uint8_t CHANN_NUM, uint32_t *src, uint8_t *dst, uint32_t DATA_CNT);
extern t_FUNC_RETURN DMA_AdcToMem_Init(uint16_t *dst, uint32_t DATA_CNT);
extern t_FUNC_RETURN DMA_ChannelEnable(uint8_t CHANN_NUM);
extern t_FUNC_RETURN DMA_ChannelClose(uint8_t CHANN_NUM);
extern t_FUNC_RETURN DMA_Disable(void);
extern bool DMA_GetDoneState(uint8_t ch);
extern bool DMA_GetErrorState(uint8_t ch);
extern void DMA_ClrDoneState(uint8_t ch);
extern void DMA_ClrErrorState(uint8_t ch);
#endif
