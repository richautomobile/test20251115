/**
 * \file
 *
 * \brief Brief description of the source file.
 *
 * Provide a detailed description of the source file.
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


/* ==[ Includes ]=========================================================== */
/*
#include "cy_pdl.h"
#include "cybsp.h"
*/
#include "deep_sleep.h"
#include "Gpio.h"

/* ==[ Macros ]============================================================= */


/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static cy_en_syspm_status_t DeepSleep_Callback(  
            cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode);
/* ==[ Definition of functions with internal linkage ]======================= */
/*******************************************************************************
* \brief Function Name: deep_sleep_callback
********************************************************************************
*
* Summary:
* Deep Sleep callback implementation. It turns the LED off before going to deep
* sleep power mode. After waking up, LED turns ON (100%).
*
* Parameters:
*  callbackParams: The pointer to the callback parameters structure cy_stc_syspm_callback_params_t.
*  mode: Callback mode, see cy_en_syspm_callback_mode_t
*
* Return:
*  Entered status, see cy_en_syspm_status_t.
*
*******************************************************************************/
cy_en_syspm_status_t DeepSleep_Callback(
        cy_stc_syspm_callback_params_t *callbackParams, cy_en_syspm_callback_mode_t mode)
{
    cy_en_syspm_status_t ret_val = CY_SYSPM_FAIL;

    switch (mode)
    {
        case CY_SYSPM_CHECK_READY:
#if 0
            while(Cy_SCB_UART_IsTxComplete(CYBSP_UART_HW) == 0u)
                {
                    /* Wait until the TX FIFO
                     * and Shifter are empty and there is no more data to send. */
                }

            /* Disable the UART */
            Cy_SCB_UART_Disable(CYBSP_UART_HW, &CYBSP_UART_context);
#endif
            ret_val = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_CHECK_FAIL:
#if 0
            /* Enable the UART */
            Cy_SCB_UART_Enable(CYBSP_UART_HW);
            /* Send a string over serial terminal */
            Cy_SCB_UART_PutString(CYBSP_UART_HW, "Device failed to enter Deep Sleep mode\r\n");
#endif
            ret_val = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_BEFORE_TRANSITION:
#if 0
            /* Disable the PWM */
            Cy_TCPWM_PWM_Disable(USER_PWM_HW, USER_PWM_NUM);
#endif
            ret_val = CY_SYSPM_SUCCESS;
            break;

        case CY_SYSPM_AFTER_TRANSITION:
#if 0
            /* Enable and Start the PWM */
            Cy_TCPWM_PWM_Enable(USER_PWM_HW, USER_PWM_NUM);
            Cy_TCPWM_TriggerStart(USER_PWM_HW, USER_PWM_MASK);

            /* Enable the UART */
            Cy_SCB_UART_Enable(CYBSP_UART_HW);
            /* Send a string over serial terminal */
            Cy_SCB_UART_PutString(CYBSP_UART_HW, "Wake-up from Deep Sleep mode and enters Active mode\r\n");

            /* Turn ON LED */
            PWM_LED_DIM(LED_ON);
#endif
            ret_val = CY_SYSPM_SUCCESS;
            break;

        default:
            /* Don't do anything in the other modes */
            ret_val = CY_SYSPM_SUCCESS;
            break;
    }
    return ret_val;
}


/* ==[ Definition of functions with external linkage ]====================== */
/*******************************************************************************
 * \brief Function Name: DeepSleep_Init
 *******************************************************************************
 *
 * Summary:
 *
 *
 * Parameters:
 * None
 *
 * Return:
 * uint32_t - Returns INIT_SUCCESS if the initialization is successful.
 * Otherwise it returns INIT_FAILURE
 *
 ******************************************************************************/
t_FUNC_RETURN DeepSleep_Init(void)
{
    /* SysPm callback params */
    cy_stc_syspm_callback_params_t callbackParams =
    {
        /*.base       =*/ NULL,
        /*.context    =*/ NULL
    };

    /* Callback declaration for Deep Sleep mode */
    cy_stc_syspm_callback_t deep_sleep_cb =
    {
        DeepSleep_Callback,         /* Callback function*/
        CY_SYSPM_DEEPSLEEP,         /* Callback type: CY_SYSPM_SLEEP */
        0,                          /* Skip mode */
        &callbackParams,            /* Callback params */
        NULL, NULL,                 /* For internal usage */
        0                           /* order */
    };
    /* Register Deep Sleep callback */
    Cy_SysPm_RegisterCallback(&deep_sleep_cb);

#if 0
    /*Use I2C communication to wake up. */ 
    /* i2c context structure */
    cy_stc_scb_i2c_context_t i2cContext;

    /* SysPm callback params */
    cy_stc_syspm_callback_params_t i2cCallbackParams =
    {
        .base       = mI2CS_HW,
        .context    = (void*)&i2cContext
    };

    /* Callback declaration for Deep Sleep mode */
    cy_stc_syspm_callback_t i2c_deep_sleep_cb =
    {
        &Cy_SCB_I2C_DeepSleepCallback, /* Callback function*/
        CY_SYSPM_DEEPSLEEP,            /* Callback type: CY_SYSPM_SLEEP */
        0,                             /* Skip mode */
        &i2cCallbackParams,            /* Callback params */
        NULL, NULL,                    /* For internal usage */
        0                              /* order */
    };
    
    /* Register Deep Sleep callback */
    Cy_SysPm_RegisterCallback(&i2c_deep_sleep_cb);
#endif

    return FUNC_RETURN_OK;
}

void DeepSleep_Enter(void)
{

    /* Go to Deep Sleep */
    Cy_SysPm_CpuEnterDeepSleep();   /*wait here*/

    
}




