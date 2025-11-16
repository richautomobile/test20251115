/**
 * \file
 *
 * \brief Brief description of the source file.
 *
 * Provide a detailed description of the source file.
 * However, it is not recommended to use the WDT for periodic interrupt generation.
 * The Watchdog Counter (WDC) can be used to generate periodic interrupts if such are presented in the device.
 * WDT_Reset_Period = ILO_Period * (2*2^(16-IgnoreBits) + MatchValue);
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
#include "wdt.h"

/* ==[ Macros ]============================================================= */
/* WDT demo options */
/* Select WDT_DEMO as either WDT_RESET_DEMO or WDT_INTERRUPT_DEMO */
#define WDT_RESET_MODE             (1U)
#define WDT_INTERRUPT_MODE         (2U)
#define WDT_MODE                   (WDT_RESET_MODE)

#if (WDT_MODE == WDT_INTERRUPT_MODE)
/* The number of WDT ticks for the 1s interrupt generation,
* at the ILO clock frequency of 40 kHz.
*/
#define WDT_TICKS_40000                         (40000U)
/*WDT_Reset_Period = ILO_Period * (2*2^(16-IgnoreBits) + MatchValue)*/
/*WDT_Reset_Period = 0.000025 * (2 * 2^16 + 40000) = 4.2768s.*/
/* WDT ignore bits */
#define WDT_IGNORE_BITS_0                       (0U)
/* Assign WDT interrupt number and priority */
#define WDT_INTR_NUM                            ((IRQn_Type) srss_interrupt_wdt_IRQn)
#define WDT_INTR_PRIORITY                       (3U)
#endif
#if (WDT_MODE == WDT_RESET_MODE)
/* The number of WDT ticks */
/*WDT_Reset_Period = ILO_Period * (2*2^(16-IgnoreBits) + MatchValue)*/
/*WDT_Reset_Period = 0,000025 * (2 * 2^14 + 16383) = 1.2287s.*/
#define WDT_TICKS_16383                         (16383U)
/* WDT ignore bits */
#define WDT_IGNORE_BITS_2                       (2U)
#endif
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/* Populate configuration structure */
#if (WDT_MODE == WDT_INTERRUPT_MODE)
cy_stc_sysint_t wdtIntrConfig =
{
    .intrSrc      = WDT_INTR_NUM,
    .intrPriority = WDT_INTR_PRIORITY,
};
#endif

static uint8_t WDT_ENABLE_STATE = 0U;
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
#if (WDT_MODE == WDT_INTERRUPT_MODE)
static void Wdt_Isr(void);
#endif
/* ==[ Definition of functions with internal linkage ]======================= */
/*******************************************************************************
* \brief Function Name: isr_button
********************************************************************************
* Summary:
* This is the callback function for button press
*
* Parameters:
*    None
*
*******************************************************************************/
#if (WDT_MODE == WDT_INTERRUPT_MODE)
static void Wdt_Isr(void)
{

    /* To adjust the WDT interrupt period using the match value, update
        * that match value in every WDT interrupt by assigning a new match value.
        * Calculate the new match value:
        * */
    uint16_t setValue = (uint16_t) Cy_WDT_GetCount() + (uint16_t) WDT_TICKS_40000;
    /* Update match value */
    Cy_WDT_SetMatch(setValue);
    /* Clear WDT interrupt */
    Cy_WDT_ClearInterrupt();
}
#endif
/* ==[ Definition of functions with external linkage ]====================== */
/*******************************************************************************
 * \brief Function Name: initMaster
 *******************************************************************************
 *
 * Summary:
 *
 * Parameters:
 * None
 *
 * Return:
 * uint32_t - Returns INIT_SUCCESS if the initialization is successful.
 * Otherwise it returns INIT_FAILURE
 *
 ******************************************************************************/
t_FUNC_RETURN Wdt_Init(void)
{
#if (WDT_MODE == WDT_INTERRUPT_MODE)
    /*However, it is not recommended to use the WDT for periodic interrupt generation.
        * The Watchdog Counter (WDC) can be used to generate periodic interrupts if such are presented in the device.
        */
    /* Disable the system reset generation. After a device reset, reset by WDT is
         * enabled by default.
       */
    Cy_WDT_Disable();
    /* Hook interrupt service routine and enable interrupt */
    (void) Cy_SysInt_Init(&wdtIntrConfig, &Wdt_Isr);
    NVIC_EnableIRQ(WDT_INTR_NUM);
    /* Config WDT: set the match value - 40000 and ignore bits - 0.*/
    Cy_WDT_SetMatch(WDT_TICKS_40000);
    Cy_WDT_SetIgnoreBits(WDT_IGNORE_BITS_0);
    /* Clear the interrupt */
    Cy_WDT_ClearInterrupt();
    /* Unmask WDT interrupt */
    Cy_WDT_UnmaskInterrupt();

    WDT_ENABLE_STATE = 1U;
#endif

#if (WDT_MODE == WDT_RESET_MODE)
    /*Configure the WDT for the system reset generation after a configurable interval.*/
    /* Disable the system reset generation. After a device reset, reset by WDT is
    * enabled by default.
    */
    Cy_WDT_Disable();
    /* Config WDT: set the match value to 16383 because this value
    * is equivalent to a timer period with 2 ignore bits.
    * Set ignore bits to 2. Based on these parameters,
    * the WDT_Reset_Period = 0,000025 * (2 * 2^14 + 16383) = 1.2287s.
    * WDT_Reset_Period = ILO_Period * (2*2^(16-IgnoreBits) + MatchValue);
    */
    Cy_WDT_SetMatch(WDT_TICKS_16383);
    Cy_WDT_SetIgnoreBits(WDT_IGNORE_BITS_2);
    /* Enables the watchdog timer reset generation. */
    Cy_WDT_Enable();

    WDT_ENABLE_STATE = 1U;
#endif

    return FUNC_RETURN_OK;
}

/*******************************************************************************
 * \brief Function Name: Wdt_Clear
 *******************************************************************************
 *
 * Summary:
 *
 * Parameters:
 * None
 *
 * Return:
 * t_FUNC_RETURN
 *
 ******************************************************************************/
t_FUNC_RETURN Wdt_Clear(void)
{
    t_FUNC_RETURN rtn_state;

    if(WDT_ENABLE_STATE == 1U)
    {
        Cy_WDT_ClearWatchdog();
        rtn_state = FUNC_RETURN_OK;
    }
    else
    {
        rtn_state = FUNC_RETURN_NG;
    }

    return rtn_state;

}

/*******************************************************************************
 * \brief Function Name: Wdt_OnOff
 *******************************************************************************
 *
 * Summary:
 *
 * Parameters:uint8_t OnOff   0:off, >0:On
 *
 * Return:
 * t_FUNC_RETURN
 *
 ******************************************************************************/
t_FUNC_RETURN Wdt_OnOff(uint8_t OnOff)
{

    if(OnOff == 0U)
    {
        Cy_WDT_Disable();
    }
    else
    {
        if(WDT_ENABLE_STATE == 1U)
        {
            Cy_WDT_Enable();
        }
    }

    return FUNC_RETURN_OK;

}



#if 0  /*Test code*/
#if(WDT_DEMO == WDT_INTERRUPT_DEMO)
/* Check if the WDT interrupt has been triggered */
if(interrupt_flag == true)
{
    /* Clear WDT Interrupt */
    Cy_WDT_ClearInterrupt();
    /* Unmask the WDT interrupt */
    Cy_WDT_UnmaskInterrupt();
    /* Clear the interrupt flag */
    interrupt_flag = false;
    /* Update the match count */
    Cy_WDT_SetMatch((uint16_t)(Cy_WDT_GetMatch() + \
                               ilo_compensated_counts));
    /* User Task- Invert the state of LED */
    Cy_GPIO_Inv(CYBSP_USER_LED1_PORT, CYBSP_USER_LED1_PIN);
}

/* Get the ILO compensated counts i.e. the actual counts for the
 desired ILO frequency. ILO default accuracy is +/- 60%.
 Note that DESIRED_WDT_INTERVAL should be less than the total
 count time */
if(CY_SYSCLK_SUCCESS == \
        Cy_SysClk_IloCompensate(DESIRED_WDT_INTERVAL, &temp_ilo_counts))
{
    ilo_compensated_counts = (uint32_t)temp_ilo_counts;
}

/* Stop ILO measurement before entering deep sleep mode */
Cy_SysClk_IloStopMeasurement();
/* Enter deep sleep mode */
Cy_SysPm_CpuEnterDeepSleep();
/* Start ILO measurement after wake up */
Cy_SysClk_IloStartMeasurement();
#endif
#endif



