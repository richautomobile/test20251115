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
#include "ext_int.h"
#include "Gpio.h"

/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
typedef struct INTPinStruct
{
    uint8_t PortPin;
    GPIO_PRT_Type* Port_Base;
    uint32_t Pin_Num;
    uint32_t EdgeMode;
} t_INTPinStruct;

/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
uint8_t RealExtIntNum = 0U;
t_INTPinStruct ExtIntPinStruct[3];
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static void isr_ext_int0(void);
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
static void isr_ext_int0(void)
{
    uint8_t i;

    for(i = 0; i < RealExtIntNum ; i++)
    {
        /* Scenario: Clear stale (unmasked) port interrupts before reconfiguration */
        if(1UL == Cy_GPIO_GetInterruptStatus(ExtIntPinStruct[i].Port_Base, ExtIntPinStruct[i].Pin_Num))
        {
            /* Clear the P0.3 interrupt */
            Cy_GPIO_ClearInterrupt(ExtIntPinStruct[i].Port_Base, ExtIntPinStruct[i].Pin_Num);
            (void)Project_ExtINT_Callback(ExtIntPinStruct[i].PortPin);
        }
    }
}
/* ==[ Definition of functions with external linkage ]====================== */


/*******************************************************************************
* \brief Function Name: initMaster
*******************************************************************************
*
* Summary:
*  CY8C4149AZQ-S548 Peripheral Interrupt Numbers
* ioss_interrupts_gpio_0_IRQn       =   0,       [DeepSleep] GPIO P0
* ioss_interrupts_gpio_1_IRQn       =   1,       [DeepSleep] GPIO P1
* ioss_interrupts_gpio_2_IRQn       =   2,       [DeepSleep] GPIO P2
* ioss_interrupts_gpio_3_IRQn       =   3,       [DeepSleep] GPIO P3
* ioss_interrupt_gpio_IRQn             =   4,       [DeepSleep] GPIO All Ports
*
* Parameters:
* EdgeMode =>
* #define CY_GPIO_INTR_DISABLE   (0x00UL) Disable the pin interrupt generation.
* #define CY_GPIO_INTR_RISING   (0x01UL)  Rising - Edge interrupt.
* #define CY_GPIO_INTR_FALLING   (0x02UL) Falling - Edge interrupt.
* #define CY_GPIO_INTR_BOTH   (0x03UL)    Both - Edge interrupt.
*
*
* Return:
* uint32_t - Returns INIT_SUCCESS if the initialization is successful.
* Otherwise it returns INIT_FAILURE
*
******************************************************************************/
t_FUNC_RETURN ExtInt_Init(uint8_t PortPin, uint32_t EdgeMode)
{
#if 1
    /*Step 1: gpio init*/
    GPIO_PRT_Type* port_base = (&GPIO->PRT[PortPin >> 4U]);
    uint32_t pinNum = (PortPin & 0x0FUL);

    cy_stc_gpio_pin_config_t CYBSP_INT_GPI_Config =
    {
        .outVal = 1,
        .driveMode = CY_GPIO_DM_PULLUP,  /*Resistive Pull-Up.Input buffer on*/
        .hsiom = HSIOM_SEL_GPIO,
        .intEdge = EdgeMode,
        .vtrip = CY_GPIO_VTRIP_CMOS,
        .slewRate = CY_GPIO_SLEW_FAST,
    };

    Cy_GPIO_Pin_Init(port_base, pinNum, &CYBSP_INT_GPI_Config);
#else

    GPIO_PRT_Type* port_base = (&GPIO->PRT[PortPin >> 4U]);
    uint32_t pinNum = (PortPin & 0x0FUL);
    /* Individual pin configuration settings can also be changed at run time
     * using supplied driver API functions. An example of some of these
     * functions are provided below.
     */
    Cy_GPIO_SetHSIOM(port_base, pinNum, CYBSP_USER_BTN_HSIOM);
    Cy_GPIO_SetDrivemode(port_base, pinNum, CY_GPIO_DM_PULLUP);
    Cy_GPIO_SetVtrip(port_base, CY_GPIO_VTRIP_CMOS);
    Cy_GPIO_SetSlewRate(port_base, CY_GPIO_SLEW_FAST);
    Cy_GPIO_SetInterruptEdge(port_base, pinNum, IntMode);
#endif

    /*Step2:  Initialize and enable GPIO interrupt */
    cy_stc_sysint_t gpi_intr_config =
    {
        /* Source of interrupt signal */
        .intrSrc = ioss_interrupt_gpio_IRQn,   
        /* Interrupt priority */
        .intrPriority = 0U,
    };
    Cy_SysInt_Init(&gpi_intr_config, &isr_ext_int0);

    /*Step3: Clearing and enabling the GPIO interrupt in NVIC */
    NVIC_ClearPendingIRQ(gpi_intr_config.intrSrc);
    NVIC_EnableIRQ(gpi_intr_config.intrSrc);

    /*Step4: save INT parameter */
    ExtIntPinStruct[RealExtIntNum].PortPin = PortPin;
    ExtIntPinStruct[RealExtIntNum].Port_Base = port_base;
    ExtIntPinStruct[RealExtIntNum].Pin_Num = pinNum;
    ExtIntPinStruct[RealExtIntNum].EdgeMode = EdgeMode;
    RealExtIntNum++;

    /* Initialization completed */           /*TODO: define real gpi*/
    return FUNC_RETURN_OK;
}

t_FUNC_RETURN ExtInt_ClearAll(void)
{
    uint8_t i;
    t_FUNC_RETURN rtn_state;

    if(RealExtIntNum)
    {
        RealExtIntNum = 0U;
        
        for(i = 0; i < RealExtIntNum; i++)
        {
            Cy_GPIO_SetInterruptEdge(ExtIntPinStruct[i].Port_Base, \
                                     ExtIntPinStruct[i].Pin_Num , \
                                     CY_GPIO_INTR_DISABLE);
        } 

        NVIC_ClearPendingIRQ(ioss_interrupt_gpio_IRQn);
        NVIC_DisableIRQ(ioss_interrupt_gpio_IRQn);         
        rtn_state = FUNC_RETURN_OK;        
    }
    else
    {
        rtn_state = FUNC_RETURN_NG;
    }
    
    return rtn_state;
}


t_FUNC_RETURN ExtInt_ReEnable(uint8_t PortPin)
{
    uint8_t i;
    t_FUNC_RETURN rtn_state;

    if(RealExtIntNum)
    {
        for(i = 0; i < RealExtIntNum; i++)
        {
            if(PortPin == ExtIntPinStruct[i].PortPin)
            {
               Cy_GPIO_SetInterruptEdge(ExtIntPinStruct[i].Port_Base, \
                                        ExtIntPinStruct[i].Pin_Num , \
                                        ExtIntPinStruct[RealExtIntNum].EdgeMode);
               rtn_state = FUNC_RETURN_OK;
               break;
            }
            else
            {
                rtn_state = FUNC_RETURN_NG;
            }
        }
    }
    else
    {
        rtn_state = FUNC_RETURN_NG;
    }
    
    return rtn_state;

}


t_FUNC_RETURN ExtInt_Disable(uint8_t PortPin)
{
    uint8_t i;
    t_FUNC_RETURN rtn_state;

    if(RealExtIntNum)
    {
        for(i = 0; i < RealExtIntNum; i++)
        {
            if(PortPin == ExtIntPinStruct[i].PortPin)
            {
               Cy_GPIO_SetInterruptEdge(ExtIntPinStruct[i].Port_Base, \
                                        ExtIntPinStruct[i].Pin_Num , \
                                        CY_GPIO_INTR_DISABLE);
               rtn_state = FUNC_RETURN_OK;
               break;
            }
            else
            {
                rtn_state = FUNC_RETURN_NG;
            }
        }
    }
    else
    {
        rtn_state = FUNC_RETURN_NG;
    }
    
    return rtn_state;

}




