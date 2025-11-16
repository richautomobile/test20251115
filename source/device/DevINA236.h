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


#ifndef _DEV_INA236_H_
#define _DEV_INA236_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "project.h"
#include "TypeDefine.h"

#ifdef EN_DEV_INA236
/* ==[ Macros ]============================================================= */
#define  INA236_VoutNormal          /*0x40, 5m ohm*/
#define  INA236_VoutQuie            /*0x41, 499 ohm*/
#define  INA236_VoutCrest           /*0x42, 50m ohm*/
#define  INA236_Vout2Normal         /*0x43, 5m ohm*/

/* ==[ Types ]============================================================== */
typedef struct ModulePowerStruct
{
    bool dataExisted;
    uint16_t processedCurrent;
    uint16_t processedVoltage;
} t_MODULE_POWER;
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN INA236_SetI2cAddr(uint8_t cp_idx, uint8_t DevAddr7b);
extern t_FUNC_RETURN INA236_WriteI2C(uint8_t cp_idx, uint8_t reg, uint16_t wData);
extern t_FUNC_RETURN INA236_ReadI2C(uint8_t cp_idx, uint8_t reg, uint16 *readData);
extern t_MODULE_POWER INA236_GetCurrent(uint8_t cp_idx);
#endif

#endif
