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


#ifndef _UART_H_
#define _UART_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"
#include "project.h"

/* ==[ Macros ]============================================================= */
#define UART_RX_BUFFER_SIZE 64U
#define FIFO_BUFFER_SIZE 10U


/* ==[ Types ]============================================================== */
typedef struct
{
    uint8_t data[UART_RX_BUFFER_SIZE];
    uint8_t length;
} UartMessage_t;

typedef struct
{
    UartMessage_t buffer[FIFO_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} UartFifo_t;

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN Uart_Init(void);
extern void Uart_Interrupt(void);
extern t_FUNC_RETURN Uart_PutArray(uint8_t WArray[], uint32_t wLen);
extern t_FUNC_RETURN Uart_GetArray(uint8_t RArray[], uint16_t *rLen);
extern t_FUNC_RETURN Uart_SetBaudRate(uint32_t bps);
extern uint8_t Uart_Get_Fifo_DataLen(void);
#endif
