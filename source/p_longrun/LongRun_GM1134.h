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


#ifndef _LONGRUN_GM1134_H_
#define _LONGRUN_GM1134_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "project.h"
#include "TypeDefine.h"
#include "i2cs.h"

#if (LONGRUN_ENABLE==1U) && (PROJECT == GM_VCS_1134)
/* ==[ Macros ]============================================================= */
/**PROJECT DEFINE**/
#define BOARD               BOARD_PSOC4
#define SW_OR_HW_CRC        HW_CRC
#define SW_OR_HW_PWM        SEL_HW_PWM
/***PROJECT DEFINE END***/

/*Device select. */
/*#define EN_DEV_ENCODER   */
#define EN_DEV_INA236
#define EN_GM_VCS_API
#define EN_DEV_FPD983      
#define EN_DEV_FPD984   
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN LongRun_Init(void);
extern t_FUNC_RETURN LongRun_RoutineCall(void);
extern t_FUNC_RETURN Project_Gpio_Init(void);
extern t_FUNC_RETURN Project_ExtINT_Callback(uint8_t PortPin);
extern t_FUNC_RETURN Project_I2CSW_Callback(uint8_t i2csWriteBuff[]);
extern t_FUNC_RETURN Project_I2CSR_Callback(uint8_t i2csReadBuff[]);
#endif  /*(GM1134==1U)*/
#endif
