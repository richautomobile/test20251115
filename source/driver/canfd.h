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


#ifndef _CANFD_H_
#define _CANFD_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"
#include "project.h"

/* ==[ Macros ]============================================================= */
#define FIFO_SIZE       10
#define CANFD_MAX_DATA  64
/* ==[ Types ]============================================================== */
typedef struct
{
    uint32_t id;
    uint8_t FDF; /*0:CY_CANFD_FDF_STANDARD_FRAME,     1:CY_CANFD_FDF_CAN_FD_FRAME*/
    uint8_t data[CANFD_MAX_DATA];  
    uint8_t length;     /*data length */
} CanFdMsg;

typedef struct
{
    CanFdMsg buffer[FIFO_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} CanFdFifo;
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN CanFd_Init(void);
extern void CanFd_ISR(void);
extern t_FUNC_RETURN CanFd_TX(uint8 ExtFormat, uint32_t Id, uint8_t canfd_WData[], uint8_t Wlen);
extern t_FUNC_RETURN CanFd_RX(CanFdMsg *msg);
extern uint8_t CanFd_Get_Fifo_DataLen(void);
extern t_FUNC_RETURN CanFd_SetBitRate(uint16_t NormalBitRate_KHz, uint16_t FastBitRate_KHz);
#endif
