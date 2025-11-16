/**
 * \file
 *
 * \brief Brief description of the source file.
 *
 * Provide a detailed description of the source file.
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


/* ==[ Includes ]=========================================================== */
/*
#include "cy_pdl.h"
#include "cybsp.h"
*/
#include <stdio.h>
#include "quad_dec.h"
#include "gpio.h"

/* ==[ Macros ]============================================================= */
#define MID_COUNT           (0x8000u)   /* for 16 bit counter*/
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static uint32_t Dec_count_prev;
static uint8_t  Dec_CWCCW = ENCODER_NOCHANGE;

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]======================= */
/*******************************************************************************
* \brief Function Name: QuadDec_Init
********************************************************************************
*
* Summary:
*   Freq. base  : 1MHz
*   Quadrature Mode: x1
*   mQuadDec_HW_NUM  : 5 . The Counter instance number in the selected TCPWM.
*   mQuadDec_HW_MASK : (1<<5)= (1<<mQuadDec_HW_NUM)
*
*   PhaA Signal : P1[2]
*   PhaB Signal : P1[3]
*
* Parameter:
*     NONE
* Return:
*      t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN QuadDec_Init(void)
{
    cy_en_tcpwm_status_t result;

    /*Variable init.*/
    Dec_count_prev = MID_COUNT;
    Dec_CWCCW = ENCODER_NOCHANGE;
    
    /* Initialize with config set in peripheral and enable QuadDec */
    result = Cy_TCPWM_QuadDec_Init(mQuadDec_HW, mQuadDec_NUM, &mQuadDec_config);

    if(result != CY_TCPWM_SUCCESS)
    {
        return FUNC_RETURN_NG;
    }
        
    Cy_TCPWM_Enable_Multiple(mQuadDec_HW, mQuadDec_MASK);
    /* Start QuadDec */
    Cy_TCPWM_TriggerReloadOrIndex(mQuadDec_HW, mQuadDec_MASK);

    return FUNC_RETURN_OK;

}

/*******************************************************************************
* \brief Function Name: QuadDec_GetCount
********************************************************************************
*
*      Dir:  CW=0x80000002, CCW=0x80000001
* Parameter:
*     NONE
* Return:
*      t_FUNC_RETURN
*
*******************************************************************************/
uint32_t  QuadDec_GetCount(void)
{
    uint32_t count;
    uint32_t delta_count=0UL;
    uint32_t dir;

    /*Get count value.*/
    count = Cy_TCPWM_QuadDec_GetCounter(mQuadDec_HW, mQuadDec_NUM);
    dir = Cy_TCPWM_QuadDec_GetStatus(mQuadDec_HW, mQuadDec_NUM);

    /*Update direction state.*/
    if((dir & CY_TCPWM_QUADDEC_STATUS_UP_COUNTING) != 0UL)
    {
        Dec_CWCCW = CY_TCPWM_QUADDEC_STATUS_UP_COUNTING;
        delta_count = (uint32_t)(count - Dec_count_prev);
    }
    if((dir & CY_TCPWM_QUADDEC_STATUS_DOWN_COUNTING) != 0UL)
    {
        Dec_CWCCW = CY_TCPWM_QUADDEC_STATUS_DOWN_COUNTING;
        delta_count = (uint32_t)(Dec_count_prev - count);
    }
    if( delta_count == 0UL )        
    {
        Dec_CWCCW = ENCODER_NOCHANGE;
    }

    /*Update count value.*/
    Dec_count_prev = count;

    return delta_count;
}

/*******************************************************************************
* \brief Function Name: QuadDec_GetDir
********************************************************************************
*
*#define CY_TCPWM_QUADDEC_STATUS_DOWN_COUNTING   (0x1UL)
*#define CY_TCPWM_QUADDEC_STATUS_UP_COUNTING   (0x2UL)
*#define CY_TCPWM_QUADDEC_STATUS_COUNTER_RUNNING   (TCPWM_CNT_STATUS_RUNNING_Msk)
*   ==>
*   ==QuadDec_GetCount must be executed first to obtain the correct direction.==
*
* Parameter:
*     NONE
* Return:
*      t_FUNC_RETURN
*
*******************************************************************************/
uint8_t  QuadDec_GetDir(void)
{
#if 0
    uint32_t state = Cy_TCPWM_QuadDec_GetStatus(mQuadDec_HW, mQuadDec_NUM);
    Dec_CWCCW = state & (CY_TCPWM_QUADDEC_STATUS_DOWN_COUNTING | CY_TCPWM_QUADDEC_STATUS_UP_COUNTING);
#endif
    return Dec_CWCCW;
}

/*******************************************************************************
* \brief Function Name: QuadDec_ReInit
********************************************************************************
*
*
*
* Parameter:
*     NONE
* Return:
*      t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN QuadDec_ReInit(void)
{
    Cy_TCPWM_TriggerStopOrKill(mQuadDec_HW, mQuadDec_MASK);
    Cy_TCPWM_QuadDec_SetCounter(mQuadDec_HW, mQuadDec_NUM, MID_COUNT);
    Cy_TCPWM_TriggerStart(mQuadDec_HW, mQuadDec_MASK);
    Dec_count_prev = MID_COUNT;
    Dec_CWCCW = ENCODER_NOCHANGE;

    return FUNC_RETURN_OK;
}


