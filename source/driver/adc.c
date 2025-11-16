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
#include "adc.h"
#include "gpio.h"
#include "dma.h"
/* ==[ Macros ]============================================================= */
#define EN_DMA_ADC
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static uint16_t adcResult[9] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U}; /* Results storage */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
/* ==[ Definition of functions with internal linkage ]======================= */
/* ==[ Definition of functions with external linkage ]====================== */
/*******************************************************************************
 * \brief Function Name: Adc_Init
 *******************************************************************************
 * Summary:
 *  This function executes interrupt service routine.
 *  8 ch SCAN time : 576us
 *  ref voltage pin : P1[7]
 *  Differential Code Range : 0x000~0x7FF
 *
 *
 ******************************************************************************/
t_FUNC_RETURN Adc_Init()
{
    cy_en_sar_status_t result;
    /* Scenario: Configure and enable the SAR interrupt. */
    cy_stc_sysint_t mADC_HW_IrqConfig =
    {
        /*.intrSrc         =*/ mADC_IRQ,   /* Interrupt source is the SAR interrupt */
        /*.intrPriority    =*/ 3UL         /* Interrupt priority is 3 */
    };

    /* Initialize the SAR ADC with the device configurator generated structure*/
    result = Cy_SAR_Init(mADC_HW, &mADC_config);

    if(result != CY_SAR_SUCCESS)
    {
        /*CY_ASSERT(0)*/
        return FUNC_RETURN_NG;
    }

    /* Enable the SAR ADC */
    Cy_SAR_Enable(mADC_HW);


    /* Configure the interrupt with vector at mADC_HW_Isr(). */
    cy_en_sysint_status_t sysintStatus = Cy_SysInt_Init(&mADC_HW_IrqConfig, &Adc_InterruptHandler);

    if(CY_SYSINT_SUCCESS != sysintStatus)
    {
        /* insert error handling here */
        return FUNC_RETURN_NG;
    }

    /* Clear possible interrupt erroneously raised during block enabling */
    Cy_SAR_ClearInterrupt(mADC_HW, CY_SAR_INTR);
    Cy_SAR_SetInterruptMask(mADC_HW, CY_SAR_INTR_EOS); /*EOS: End of scan.*/
    /* Enable the interrupt. */
    NVIC_EnableIRQ(mADC_IRQ);

#ifdef EN_DMA_ADC
    DMA_AdcToMem_Init(adcResult, 9U);
#endif
    /* Start the continuous conversion */
    Cy_SAR_StartConvert(mADC_HW, CY_SAR_START_CONVERT_CONTINUOUS); /* CY_SAR_START_CONVERT_SINGLE_SHOT, CY_SAR_START_CONVERT_CONTINUOUS */
    //Cy_SAR_StartConvert(mADC_HW, CY_SAR_START_CONVERT_SINGLE_SHOT);

    return FUNC_RETURN_OK;
}

/*******************************************************************************
 * \brief Function Name: sI2C_InterruptHandler
 *******************************************************************************
 * Summary:
 *  This function executes interrupt service routine.
 *  12bits ADC:  0x000~0xFFF.
 *  V = (RawCounts/AvgDivider - Offset)*TEN_VOLT/Gain
 *  Warning:
 *  Noise and offset can cause unsigned results to underflow(wrap-around to +ve full scale) when Vneg is set to Vssa or External. [CY8C4149AZI-S598: PASS 0 12-bit SAR ADC 0 (mADC)]
 *
 ******************************************************************************/
void Adc_InterruptHandler(void)
{
    /*Cy_GPIO_Inv(CYBSP_LED2_PORT, CYBSP_LED2_NUM)*/
    uint32_t intrMask = Cy_SAR_GetInterruptStatusMasked(mADC_HW);
#ifndef EN_DMA_ADC
    int16_t adcTmp;

    if(0UL != (CY_SAR_INTR_EOS & intrMask))
    {
        adcResult[0] = Cy_SAR_GetResult16(mADC_HW, 0);
        adcResult[1] = Cy_SAR_GetResult16(mADC_HW, 1);
        adcResult[2] = Cy_SAR_GetResult16(mADC_HW, 2);
        adcResult[3] = Cy_SAR_GetResult16(mADC_HW, 3);
        adcResult[4] = Cy_SAR_GetResult16(mADC_HW, 4);
        adcResult[5] = Cy_SAR_GetResult16(mADC_HW, 5);
        adcResult[6] = Cy_SAR_GetResult16(mADC_HW, 6); /*NG: 3.3=2066*/
        adcResult[7] = Cy_SAR_GetResult16(mADC_HW, 7);
        /*count to degree*/
        adcTmp = Cy_SAR_GetResult16(mADC_HW, 8);
        adcResult[8] = Cy_SAR_CountsTo_degreeC(mADC_HW, 8, adcTmp);
    }
#endif
    Cy_SAR_ClearInterrupt(mADC_HW, intrMask);

}

/*******************************************************************************
 * \brief Function Name: Adc_GetAdcVal
 *******************************************************************************
 * Summary:
 *  get adc value.
 *
 *  CH0:  otp adc(分壓比=53.6/(300+53.6))
 *  CH1:  Silver_voltage(Vin=12V => CH1=0xFFF ) 
 *  CH2:  983_MODE0_Voltage(沒有分壓比)
 *  CH3 ~ CH7: backup adc
 *  CH8:  MCU temperture(degree) 
 *  V = (RawCounts/AvgDivider - Offset)*TEN_VOLT/Gain
 *
 *
 * \Parameters [in] uint8_t: get adc channel 0~8.  >=9 is error and return -1.
 * \return          int16_t: returnn adc value
 ******************************************************************************/
int16_t Adc_GetAdcVal(uint8_t ch)
{
    int16_t rtnAdc = -1;

    if(9U > ch)
    {
#ifdef EN_DMA_ADC

        if(8U == ch)
        {
            rtnAdc = Cy_SAR_CountsTo_degreeC(mADC_HW, 8, adcResult[8]);
        }
        else
        {
            rtnAdc = adcResult[ch];
        }

#else
        rtnAdc = adcResult[ch];
#endif
    }

    return rtnAdc;
}

