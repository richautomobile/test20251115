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


#ifndef _I2CM_H_
#define _I2CM_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN I2CMA_Init(void);
extern t_FUNC_RETURN I2CMB_Init(void);
extern t_FUNC_RETURN I2CMA_SetCLK(uint16_t KHz);
extern uint16_t I2CMA_GetCLK(void);
extern t_FUNC_RETURN I2CMA_Write(uint8_t SlaveAddress, uint8_t WBuffer[], uint32_t WByteLen);
extern t_FUNC_RETURN I2CMA_WriteB(uint8_t SlaveAddress, uint8_t WReg, uint8_t WData);
extern t_FUNC_RETURN I2CMA_DirRead(uint8_t SlaveAddress, uint8_t RBuffer[], uint32_t RByteLen);
extern t_FUNC_RETURN I2CMA_Read(uint8_t SlaveAddress, uint8_t WBuffer[],  uint32_t WByteLen, uint8_t RBuffer[], uint32_t RByteLen);
extern uint8_t I2CMA_ReadB(uint8_t SlaveAddress, uint8_t RReg);
extern t_FUNC_RETURN I2CMB_Write(uint8_t SlaveAddress, uint8_t WBuffer[], uint32_t WByteLen);
extern t_FUNC_RETURN I2CMB_WriteB(uint8_t SlaveAddress, uint8_t WReg, uint8_t WData);
extern t_FUNC_RETURN I2CMB_DirRead(uint8_t SlaveAddress, uint8_t RBuffer[], uint32_t RByteLen);
extern t_FUNC_RETURN I2CMB_Read(uint8_t SlaveAddress, uint8_t WBuffer[],  uint32_t WByteLen, uint8_t RBuffer[], uint32_t RByteLen);
extern uint8_t I2CMB_ReadB(uint8_t SlaveAddress, uint8_t RReg);

#endif
