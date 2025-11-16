/**
 * \file
 *
 * \brief Brief description of the source file.
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
#include "dma.h"

/* ==[ Macros ]============================================================= */
/* Scenario: Initialize the single ping descriptor */
/*#define DATA_CNT (8U)*/
/*#define CHANN_NUM (0U)*/
#define DMA_IRQ               (cpuss_interrupt_dma_IRQn)
#define DMA_INT_PRIORITY      (3u)
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static bool DMA_DONE_STATE[16];
static bool DMA_ERR_STATE[16];
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static void DMA_Isr(void);
/* ==[ Definition of functions with internal linkage ]======================= */
/*******************************************************************************
 * \brief Function Name: Isr_DMA
 ********************************************************************************
 *
 * Summary:
 *  Handles Dma descriptor
 *
 * Parameters:
 *  None
 *
 * Return:
 *  None
 *
 *******************************************************************************/
static void DMA_Isr(void)
{
    uint8_t dma_ch = 0U;
    /*cy_en_dmac_descriptor_t descriptor*/
    cy_en_dmac_response_t dmac_response;
    /* Scenario: Inside the interrupt service routine for DMAC channel*/
    uint32_t interrupt = Cy_DMAC_GetInterruptStatusMasked(DMAC);
    uint32_t check_interrupt = interrupt;

    while(check_interrupt > 0U)
    {
        if((check_interrupt & 0x0001U) > 0U)
        {
            dmac_response = Cy_DMAC_Descriptor_GetResponse(DMAC, dma_ch,
                            CY_DMAC_DESCRIPTOR_PING);
            Cy_DMAC_Channel_Disable(DMAC, dma_ch);

            if(dmac_response == CY_DMAC_DONE)
            {
                DMA_DONE_STATE[dma_ch] = true;
                /*TODO: Finish callback.*/
            }
            else if((dmac_response != CY_DMAC_DONE) &&
                    (dmac_response != CY_DMAC_NO_ERROR))
            {
                DMA_ERR_STATE[dma_ch] = true;
                /*TODO: DMA error callback.*/
            }
        }

        /*Step2: Update flag*/
        check_interrupt = check_interrupt >> 1U;
        dma_ch ++;
    }

    /* Clear TxDma channel interrupt */
    Cy_DMAC_ClearInterrupt(DMAC, interrupt);
}


/* ==[ Definition of functions with external linkage ]====================== */
/*******************************************************************************
 * \brief Function Name: DMA_MemToPeripheral_Init
 *******************************************************************************
 *
 * Summary:
 *                   Initialize DMAC DMAC Ping config from memory to peripheral.
 * Parameters:
 * uint8_t                              : CHANN_NUM=0~15
 * uint8_t *src                       : source buffer point
 * void *dst                           : dst buffer point
 * uint32_t DATA_CNT           : buffer length
 *
 * Return:
 * uint32_t - Returns INIT_SUCCESS if the initialization is successful.
 * Otherwise it returns INIT_FAILURE
 *
 ******************************************************************************/
t_FUNC_RETURN DMA_MemToPeripheral_Init(uint8_t CHANN_NUM, uint8_t *src, uint32_t *dst, uint32_t DATA_CNT)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
    uint16_t DmaIntMask;
    cy_stc_sysint_t DMA_INT_cfg =
    {
        .intrSrc      = (IRQn_Type)DMA_IRQ,
        .intrPriority = DMA_INT_PRIORITY,
    };

    /*Step1 : Descriptor ping configuration structure */
    cy_stc_dmac_descriptor_config_t descriptorPingCfg =
    {
        .srcAddress       = src,
        .dstAddress       = dst,
        .dataCount        = DATA_CNT,
        .dataSize         = CY_DMAC_BYTE,   /*CY_DMAC_HALFWORD, CY_DMAC_WORD,*/
        .srcTransferSize  = CY_DMAC_TRANSFER_SIZE_DATA,
        .srcAddrIncrement = true,   /*true,*/
        .dstTransferSize  = CY_DMAC_TRANSFER_SIZE_WORD,
        .dstAddrIncrement = false,    /*false,  true,*/
        .retrigger        = CY_DMAC_RETRIG_16CYC, //CY_DMAC_RETRIG_4CYC,
        .cpltState        = false,
        .interrupt        = true,
        .preemptable      = true,
        .flipping         = false,
        .triggerType      = CY_DMAC_SINGLE_ELEMENT /*CY_DMAC_SINGLE_DESCR*/
    };
    /* Channel configuration structure */
    cy_stc_dmac_channel_config_t channelConfig =
    {
        .priority = 3U,
        .enable = false,
        .descriptor = CY_DMAC_DESCRIPTOR_PING
    };

    /* Initialize the ping descriptor for channel 0 */
    if(CY_DMAC_SUCCESS != Cy_DMAC_Descriptor_Init(DMAC, CHANN_NUM, CY_DMAC_DESCRIPTOR_PING, &descriptorPingCfg))
    {
        rtn_state = FUNC_RETURN_NG;
    }

    /* Scenario: Setup and enable DMAC channel 0 */
    if(CY_DMAC_SUCCESS != Cy_DMAC_Channel_Init(DMAC, CHANN_NUM, &channelConfig))
    {
        rtn_state = FUNC_RETURN_NG;
    }

    /*Step2:  Enable interrupt for Dma channel */
    DmaIntMask = Cy_DMAC_GetInterruptMask(DMAC);
    DmaIntMask |= (0x01 << CHANN_NUM);
    Cy_DMAC_SetInterruptMask(DMAC, DmaIntMask);

    /* Initialize and enable the DMA interrupt */
    Cy_SysInt_Init(&DMA_INT_cfg, &DMA_Isr);
    NVIC_EnableIRQ(DMA_INT_cfg.intrSrc);

    /*Step3:  Enable DMA */
    Cy_DMAC_Channel_Enable(DMAC, CHANN_NUM);
    Cy_DMAC_Enable(DMAC);

    return rtn_state;
}


/*******************************************************************************
 * \brief Function Name: DMA_PeripheralToMem_Init
 *******************************************************************************
 *
 * Summary:
 *                   Initialize DMAC Ping config from peripheral to memory.
 * Parameters:
 * uint8_t                              : CHANN_NUM=0~15
 * void *src                           : source buffer point
 * uint8_t *dst                       : dst buffer point
 * uint32_t DATA_CNT           : buffer length
 *
 * Return:
 * uint32_t - Returns INIT_SUCCESS if the initialization is successful.
 * Otherwise it returns INIT_FAILURE
 *
 ******************************************************************************/
t_FUNC_RETURN DMA_PeripheralToMem_Init(uint8_t CHANN_NUM, uint32_t *src, uint8_t *dst, uint32_t DATA_CNT)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
    uint16_t DmaIntMask;
    cy_stc_sysint_t DMA_INT_cfg =
    {
        .intrSrc      = (IRQn_Type)DMA_IRQ,
        .intrPriority = DMA_INT_PRIORITY,
    };

    /*Step1 : Descriptor ping configuration structure */
    cy_stc_dmac_descriptor_config_t descriptorPingCfg =
    {
        .srcAddress       = src,
        .dstAddress       = dst,
        .dataCount        = DATA_CNT,
        .dataSize         = CY_DMAC_BYTE,   /*CY_DMAC_HALFWORD, CY_DMAC_WORD,*/
        .srcTransferSize  = CY_DMAC_TRANSFER_SIZE_WORD,
        .srcAddrIncrement = false,   /*true,*/
        .dstTransferSize  = CY_DMAC_TRANSFER_SIZE_DATA,
        .dstAddrIncrement = true,    /*false,  true,*/
        .retrigger        = CY_DMAC_RETRIG_16CYC, //CY_DMAC_RETRIG_4CYC,
        .cpltState        = false,
        .interrupt        = true,
        .preemptable      = true,
        .flipping         = false,
        .triggerType      = CY_DMAC_SINGLE_ELEMENT /*CY_DMAC_SINGLE_DESCR*/
    };
    /* Channel configuration structure */
    cy_stc_dmac_channel_config_t channelConfig =
    {
        .priority = 3U,
        .enable = false,
        .descriptor = CY_DMAC_DESCRIPTOR_PING
    };

    /* Initialize the ping descriptor for channel 0 */
    if(CY_DMAC_SUCCESS != Cy_DMAC_Descriptor_Init(DMAC, CHANN_NUM, CY_DMAC_DESCRIPTOR_PING, &descriptorPingCfg))
    {
        rtn_state = FUNC_RETURN_NG;
    }

    /* Scenario: Setup and enable DMAC channel 0 */
    if(CY_DMAC_SUCCESS != Cy_DMAC_Channel_Init(DMAC, CHANN_NUM, &channelConfig))
    {
        rtn_state = FUNC_RETURN_NG;
    }

    /*Step2:  Enable interrupt for Dma channel */
    DmaIntMask = Cy_DMAC_GetInterruptMask(DMAC);
    DmaIntMask |= (0x01 << CHANN_NUM);
    Cy_DMAC_SetInterruptMask(DMAC, DmaIntMask);

    /* Initialize and enable the DMA interrupt */
    Cy_SysInt_Init(&DMA_INT_cfg, &DMA_Isr);
    NVIC_EnableIRQ(DMA_INT_cfg.intrSrc);

    /*Step3:  Enable DMA */
    Cy_DMAC_Channel_Enable(DMAC, CHANN_NUM);
    Cy_DMAC_Enable(DMAC);

    return rtn_state;
}

/*******************************************************************************
 * \brief Function Name: DMA_AdcToMem_Init
 *******************************************************************************
 *
 * Summary:
 *                   Initialize DMA_C0 Ping config from ADC to memory.
 * Parameters:
 * uint8_t *dst                       : dst buffer point
 * uint32_t DATA_CNT           : buffer length
 * NO open interrupt.  Not verified yet.
 *
 * Return:
 * uint32_t - Returns INIT_SUCCESS if the initialization is successful.
 * Otherwise it returns INIT_FAILURE
 *
 ******************************************************************************/
t_FUNC_RETURN DMA_AdcToMem_Init(uint16_t *dst, uint32_t DATA_CNT)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
    uint16_t DmaIntMask;
    /* The device configurator has this config, but the config is const can not be modify. */
    const cy_stc_dmac_descriptor_config_t descriptorPingCfg =
    {
        .srcAddress = &SAR0->CHAN_RESULT[0],
        .dstAddress = dst,
        .dataCount = DATA_CNT,
        .dataSize = CY_DMAC_HALFWORD, /*16bits*/
        .srcTransferSize = CY_DMAC_TRANSFER_SIZE_WORD,
        .srcAddrIncrement = true,
        .dstTransferSize = CY_DMAC_TRANSFER_SIZE_DATA,
        .dstAddrIncrement = true,
        .retrigger = CY_DMAC_RETRIG_IM,
        .cpltState = false,
        .interrupt = false,   /*Not enable interrupt. Cyclin adc detect.*/
        .preemptable = true,
        .flipping = false,
        .triggerType = CY_DMAC_SINGLE_ELEMENT
    };

    /* Channel configuration structure */
    cy_stc_dmac_channel_config_t channelConfig =
    {
        .priority = 3U,
        .enable = false,
        .descriptor = CY_DMAC_DESCRIPTOR_PING
    };

    /*cycfg_routing*/
    /*Cy_TrigMux_Connect(TRIG0_IN_PASS0_TR_SAR_OUT, (TRIG0_OUT_CPUSS_DMAC_TR_IN0 | CHANN_NUM)); */
        
    /* Initialize the ping descriptor for channel 0 */
    if(CY_DMAC_SUCCESS != Cy_DMAC_Descriptor_Init(DMAC, 0U, CY_DMAC_DESCRIPTOR_PING, &descriptorPingCfg))
    {
        rtn_state = FUNC_RETURN_NG;
    }
    
    /* Scenario: Setup and enable DMAC channel 0 */
    if(CY_DMAC_SUCCESS != Cy_DMAC_Channel_Init(DMAC, 0U, &channelConfig))
    {
        rtn_state = FUNC_RETURN_NG;
    }

    /*Step2:  Disable interrupt for Dma channel */
    DmaIntMask = Cy_DMAC_GetInterruptMask(DMAC);
    DmaIntMask &= (~(0x0001));
    Cy_DMAC_SetInterruptMask(DMAC, DmaIntMask);

    /*Step3:  Enable DMA */
    Cy_DMAC_Channel_Enable(DMAC, 0);
    Cy_DMAC_Enable(DMAC);

    return rtn_state;
}



/*******************************************************************************
 * \brief Function Name: DMA_ChannelEnable
 *******************************************************************************
 *
 * Summary:
 *
 * Parameters:
 * uint8_t : CHANN_NUM=0~15
 *
 * Return:
 * t_FUNC_RETURN
 *
 ******************************************************************************/
t_FUNC_RETURN DMA_ChannelEnable(uint8_t CHANN_NUM)
{
    Cy_DMAC_Channel_Enable(DMAC, CHANN_NUM);
    /*Cy_DMAC_Enable(DMAC)*/

    return FUNC_RETURN_OK;
}


/*******************************************************************************
 * \brief Function Name: DMA_ChannelClose
 *******************************************************************************
 *
 * Summary:
 *
 * Parameters:
 * uint8_t : CHANN_NUM=0~15
 *
 * Return:
 * t_FUNC_RETURN
 *
 ******************************************************************************/
t_FUNC_RETURN DMA_ChannelClose(uint8_t CHANN_NUM)
{
    Cy_DMAC_Channel_Disable(DMAC, CHANN_NUM);
    /*Cy_DMAC_Channel_DeInit(DMAC, CHANN_NUM);*/

    return FUNC_RETURN_OK;
}

/*******************************************************************************
 * \brief Function Name: DMA_Disable
 *******************************************************************************
 *
 * Summary:
 *
 * Parameters:
 * NONE
 *
 * Return:
 * t_FUNC_RETURN
 *
 ******************************************************************************/
t_FUNC_RETURN DMA_Disable(void)
{
    Cy_DMAC_Disable(DMAC);
    return FUNC_RETURN_OK;
}

bool DMA_GetDoneState(uint8_t ch)
{
    return DMA_DONE_STATE[ch];
}
bool DMA_GetErrorState(uint8_t ch)
{
    return DMA_ERR_STATE[ch];
}
void DMA_ClrDoneState(uint8_t ch)
{
    DMA_DONE_STATE[ch] = false;
}
void DMA_ClrErrorState(uint8_t ch)
{
    DMA_ERR_STATE[ch] = false;
}


