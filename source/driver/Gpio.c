/**
 * @file Gpio.c
 *
 * @brief
 *
 * Copyright (C) 2022 Innolux Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Innolux Corporation
 *
 * MISRA-C:2012 Deviation List
 *
 * MISRA-2:
 * Deviated Rule:
 *
 * @author BingHan (binghan.chou@innolux.com)
 * @date 2022-11-10
 */


/* ==[ Includes ]============================================================ */
/*******************************************************************************
 * Include header files
 ******************************************************************************/
/*
#include "cy_pdl.h"
#include "cybsp.h"
*/
#include "Gpio.h"

/* ==[ Macros ]============================================================== */
#define GPIO_PTR_BASE(Port) ((GPIO_PRT_Type*)&GPIO->PRT[Port]) //((GPIO_PRT_Type*) &GPIO->PRT[Port])
/* ==[ Types ]=============================================================== */
/* ==[ Constants with external linkage ]===================================== */
/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with internal linkage ]====================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]====================== */
t_FUNC_RETURN Gpio_Init(void)
{

    (void)Project_Gpio_Init();

    return FUNC_RETURN_OK;
}

uint8_t Gpio_GetPin(uint8_t PortPin)
{
    uint8_t Port = (PortPin >> 4U);
    uint8_t Pin = (PortPin & 0x0FU);

    return (uint8_t)Cy_GPIO_Read(GPIO_PTR_BASE(Port), Pin);
}

uint8_t Gpio_GetPort(uint8_t Port)
{
    uint8_t portValue = GPIO_PRT_IN(GPIO_PTR_BASE(Port));

    return portValue;
}

t_FUNC_RETURN Gpio_WritePin(uint8_t PortPin, uint8_t Level)
{
    uint8_t Port = (PortPin >> 4U);
    uint8_t Pin = (PortPin & 0x0FU);

    if(Level > 0U)
        Level = 1U;

    Cy_GPIO_Write(GPIO_PTR_BASE(Port), Pin, Level);
    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Gpio_WritePort(uint8_t Port, uint8_t value)
{
    GPIO_PRT_DR(GPIO_PTR_BASE(Port)) = (uint32_t)value;
    return FUNC_RETURN_OK;
}


t_FUNC_RETURN Gpio_Inv(uint8_t PortPin)
{
    uint8_t Port = (PortPin >> 4U);
    uint8_t Pin = (PortPin & 0x0FU);

    Cy_GPIO_Inv(GPIO_PTR_BASE(Port), Pin);
    return FUNC_RETURN_OK;
}

/*
*
*
*Pin drive mode=>
CY_GPIO_DM_HIGHZ   (0x01UL)                                  Digital High-Z.Input buffer on
CY_GPIO_DM_PULLUP   (0x02UL)                                 Resistive Pull-Up.Input buffer on
CY_GPIO_DM_PULLDOWN   (0x03UL)                           Resistive Pull-Down.Input buffer on
CY_GPIO_DM_OD_DRIVESLOW   (0x04UL)                    Open Drain, Drives Low.Input buffer on
CY_GPIO_DM_OD_DRIVESHIGH   (0x05UL)                   Open Drain, Drives High.Input buffer on
CY_GPIO_DM_STRONG   (0x06UL)                                Strong Drive.Input buffer on
CY_GPIO_DM_PULLUP_DOWN   (0x07UL)                      Resistive Pull-Up/Down.Input buffer on
CY_GPIO_DM_ANALOG   (0x09UL)                                Analog High-Z.Input buffer off
CY_GPIO_DM_PULLUP_IN_OFF   (0x0AUL)                    Resistive Pull-Up.Input buffer off
CY_GPIO_DM_PULLDOWN_IN_OFF   (0x0BUL)              Resistive Pull-Down.Input buffer off
CY_GPIO_DM_OD_DRIVESLOW_IN_OFF   (0x0CUL)      Open Drain, Drives Low.Input buffer off
CY_GPIO_DM_OD_DRIVESHIGH_IN_OFF   (0x0DUL)     Open Drain, Drives High.Input buffer off
CY_GPIO_DM_STRONG_IN_OFF   (0x0EUL)                  Strong Drive.Input buffer off
CY_GPIO_DM_PULLUP_DOWN_IN_OFF   (0x0FUL)        Resistive Pull-Up/Down.Input buffer off

Cy_GPIO_Pin_FastInit    Parameters:
base             Pointer to the pin's port register base address
pinNum         Position of the pin bit-field within the port register
driveMode     Pin drive mode. Options are detailed in Pin drive mode macros
outVal          Logic state of the output buffer driven to the pin (1 or 0)
hsiom           HSIOM input selection. set to 0 for giop.

*parpaeter:
*uint8_t  gpio_pin:  (port<<4)|(pin)
*uint32_t PinDriverMode:
*uint8_t  defVal:    1 or 0
*
*return:
*t_FUNC_RETURN
*/
t_FUNC_RETURN Gpio_PinConfig(uint8_t PortPin, uint32_t PinDriverMode, uint8_t defVal)
{
    uint8_t Port = (PortPin >> 4U);
    uint8_t pinNum = (PortPin & 0x0FU);

    /* Quickly initialize pin P0.3 (e.g. quickly set up a test LED) */
    /*Cy_GPIO_Pin_FastInit(P0_0_PORT, P0_0_NUM, CY_GPIO_DM_PULLUP, 1UL, P0_0_GPIO)*/
    Cy_GPIO_Pin_FastInit(GPIO_PTR_BASE(Port), pinNum, PinDriverMode, defVal, 0);
    return FUNC_RETURN_OK;
}

