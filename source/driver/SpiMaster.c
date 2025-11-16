/**
 * \file
 *
 * \brief Brief description of the source file.
 *
 * Provide a detailed description of the source file.
 *
 * Project(s): PSOC4_Brifge_Board_App xxx
 * default SPI Speed: 4MHz
 *
 * Target: independent
 *
 * \author walker.chiang <walker.chiang@carux.com>
 *
 * \copyright (C) 2024 Car-UX Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Car-UX
 * Corporation.
 */

/* ==[ Includes ]=========================================================== */
/*
#include "cy_pdl.h"
#include "cybsp.h"
*/
#include "SpiMaster.h"
#include "Gpio.h"

/* ==[ Macros ]============================================================= */
#define SpiDelayUs   10U
/****************************/
#define EnableAutoCS       0U  /*AutoCS can't work for NOVA tcon.*/

/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/*******************************************************************************
 * Global Variables
 ******************************************************************************/
static cy_stc_scb_spi_context_t mSPIM_context;
static uint8_t SPIM_CS_CH = 0xFFU;
static uint16_t SPIM_CLK_KHz = 2000U; /*default 4000KHz*/
static uint8_t CK_POL = 1U;
static uint8_t CK_PHA = 1U;

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
#if (EnableAutoCS==1U)
static uint8_t SPIM_CheckCSActive(uint8_t cs_ch);
#else
static t_FUNC_RETURN SPIM_SetCSLevel(uint8_t ch, uint8_t level);
#endif

/* ==[ Definition of functions with internal linkage ]======================= */
#if (EnableAutoCS==1U)
static uint8_t SPIM_CheckCSActive(uint8_t cs_ch)
{
    cy_en_scb_spi_slave_select_t sel_cs;

    if(4u > cs_ch)
    {
        if(SPIM_CS_CH != cs_ch)
        {

            Cy_SCB_SPI_Disable(mSPIM_HW, &mSPIM_context);
            SPIM_CS_CH = cs_ch;

            if(0U == cs_ch)
            {
                sel_cs = CY_SCB_SPI_SLAVE_SELECT0;
            }
            else if(1U == cs_ch)
            {
                sel_cs = CY_SCB_SPI_SLAVE_SELECT1;
            }
            else if(2U == cs_ch)
            {
                sel_cs = CY_SCB_SPI_SLAVE_SELECT2;
            }
            else
            {
                sel_cs = CY_SCB_SPI_SLAVE_SELECT3;
            }

            Cy_SCB_SPI_SetActiveSlaveSelect(mSPIM_HW, sel_cs);
            Cy_SCB_SPI_Enable(mSPIM_HW);
        }
    }

    return SPIM_CS_CH;
}
#else
t_FUNC_RETURN SPIM_SetCSLevel(uint8_t ch, uint8_t level)
{
    t_FUNC_RETURN rtn_state;

    if(ch < 4U)
    {
        if(ch == 0U)
        {
            Gpio_WritePin(SPIM_CS0, level);
        }
        else if(ch == 1U)
        {
            Gpio_WritePin(SPIM_CS1, level);
        }
        else if(ch == 2U)
        {
            Gpio_WritePin(SPIM_CS2, level);
        }
        else
        {
            Gpio_WritePin(SPIM_CS3, level);
        }

        rtn_state = FUNC_RETURN_OK;
    }
    else
    {
        rtn_state = FUNC_RETURN_NG;

    }


    return rtn_state;
}
#endif

void SPIM_Isr(void)
{
    Cy_SCB_SPI_Interrupt(mSPIM_HW, &mSPIM_context);
}

/* ==[ Definition of functions with external linkage ]====================== */
/*******************************************************************************
 * \brief Function Name: initMaster
 *******************************************************************************
 *
 * Summary:
 * This function initializes the SPI master based on the configuration done in
 * design.modus file.
 *
 * Parameters:
 * None
 *
 * Return:
 * uint32_t - Returns INIT_SUCCESS if the initialization is successful.
 * Otherwise it returns INIT_FAILURE
 *
 ******************************************************************************/
t_FUNC_RETURN SPIM_Init(void)
{
    cy_en_scb_spi_status_t result;

    SPIM_CS_CH = 0xFFu;

#if (EnableAutoCS==0U)
    /*CS GPIO init*/
    Gpio_PinConfig(SPIM_CS0, CY_GPIO_DM_STRONG, 1);
    Gpio_PinConfig(SPIM_CS1, CY_GPIO_DM_STRONG, 1);
    Gpio_PinConfig(SPIM_CS2, CY_GPIO_DM_STRONG, 1);
    Gpio_PinConfig(SPIM_CS3, CY_GPIO_DM_STRONG, 1);
#endif

    /* Configure the SPI block */
    result = Cy_SCB_SPI_Init(mSPIM_HW, &mSPIM_config, &mSPIM_context);

    if(result != CY_SCB_SPI_SUCCESS)
    {
        return(FUNC_RETURN_NG);
    }

    /*The interrupt must be configured when High-Level API will be used.*/
    /* Populate configuration structure */
    const cy_stc_sysint_t spiIntrConfig =
    {
        .intrSrc      = mSPIM_IRQ,
        .intrPriority = 3U
    };

    /* Hook interrupt service routine and enable interrupt */
    (void) Cy_SysInt_Init(&spiIntrConfig, &SPIM_Isr);
    NVIC_EnableIRQ(mSPIM_IRQ);
    /*******************************/

#if 0
    /* Set active slave select to line 0 */
    Cy_SCB_SPI_SetActiveSlaveSelect(mSPIM_HW, CY_SCB_SPI_SLAVE_SELECT0);
#endif


    /* Enable the SPI Master block */
    Cy_SCB_SPI_Enable(mSPIM_HW);


    /* Initialization completed */
    return(FUNC_RETURN_OK);
}

/*
 *
 *
 * vaild value : 4~16 step 1
 *
 */
t_FUNC_RETURN SPIM_SetCLK(uint16_t KHz)
{
    uint16_t SPI_CLK_BASE = 16000U;  /*8MHZ*/
    uint16_t new_spi_clk = (uint16_t)(SPI_CLK_BASE / KHz);
    cy_stc_scb_spi_config_t new_mSPIM_config = mSPIM_config;

    if(new_spi_clk >= 4U &&  new_spi_clk <= 16U)
    {
        new_mSPIM_config.oversample = new_spi_clk;
        (void)Cy_SCB_SPI_Init(mSPIM_HW, &new_mSPIM_config, &mSPIM_context);
        SPIM_CLK_KHz = new_spi_clk;
        return FUNC_RETURN_OK;
    }
    else
    {
        return FUNC_RETURN_NG;
    }
}

uint16_t SPIM_GetCLK(void)
{
    return SPIM_CLK_KHz;
}

/* \brief  SPIM_SetPOL_PHA
 *
 *
 * CKPOL  :  0:CLK Normal Low,  >0: CLK Normal Hi
 * CKPHA  :  0:First edge latch, >0:Second edge latch
 *
 */
t_FUNC_RETURN SPIM_SetPOL_PHA(uint8_t POL, uint8_t PHA)
{
    cy_stc_scb_spi_config_t new_mSPIM_config = mSPIM_config;

    if(POL > 0U)
    {
        CK_POL = 1U;
    }
    else
    {
        CK_POL = 0U;
    }

    if(PHA > 0U)
    {
        CK_PHA = 1U;
    }
    else
    {
        CK_PHA = 0U;
    }

    new_mSPIM_config.sclkMode=(CK_PHA<<1)|CK_POL;
    (void)Cy_SCB_SPI_Init(mSPIM_HW, &new_mSPIM_config, &mSPIM_context);
    
    return FUNC_RETURN_OK;    
}


/** \brief SPI Write
*
*   Simpe spi write. There is not reading RX buffer.
*
* \param [in] write_buffer is write buffer
*             read_buffer is read buffer
* \return t_FUNC_RETURN
*/
t_FUNC_RETURN SPIM_Write(uint8_t cs_ch, uint8_t WBuffer[], uint32_t WByteSize)
{
    /*Hi level*/
    t_FUNC_RETURN rtn_status;
    uint8_t RBuffer[WByteSize];
    /* Timeout 1 sec (one unit is us) */
    uint32_t timeout = 5000UL;

    /*Step 1: check cs channel.*/
#if (EnableAutoCS==1U)
    (void)SPIM_CheckCSActive(cs_ch);
#else
    SPIM_SetCSLevel(cs_ch, 0U);   /*En CS*/
#endif
    /* Master: start a transfer. Slave: prepare for a transfer. */
    (void)Cy_SCB_SPI_Transfer(mSPIM_HW, WBuffer, RBuffer, WByteSize, &mSPIM_context);

    /* Blocking wait for transfer completion */
    do
    {
        Cy_SysLib_DelayUs(SpiDelayUs);
        timeout--;
    }
    while((0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(mSPIM_HW, &mSPIM_context))) && (timeout > 0));

    if(timeout > 0)
    {
        rtn_status = FUNC_RETURN_OK;
    }
    else
    {
        rtn_status = FUNC_RETURN_NG;
    }

#if (EnableAutoCS==0U)
    /*set cs*/
    SPIM_SetCSLevel(cs_ch, 1U);  /*Dis CS*/
#endif
    return (rtn_status);


}

/** \brief SPI Read / Write
*
*  SPI write  =>  WRBuffer put in write data. WRByteLen = write buffer length.
*  SPI read   =>  WRBuffer put in write data and read dummy data. WRByteLen = write and read total length.
*                 RX Buffer will be recoded in the WRByteLen = write buffer length.
*
* \param [in] Write and Read buffer.
*             write and read buffer length
* \return t_FUNC_RETURN
*/
t_FUNC_RETURN SPIM_WriteRead(uint8_t cs_ch, uint8_t WBuffer[], uint32_t WByteLen, uint8_t RBuffer[], uint32_t RByteLen)
{
    /*Hi level*/
    uint32_t i;
    t_FUNC_RETURN rtn_status;
    /* Timeout 1 sec (one unit is us) */
    uint32_t timeout; /*timeout time = 5000*SpiDelayUs = 500000us */
    uint8_t WRBuff[WByteLen + RByteLen];
    uint8_t RtnBuff[WByteLen + RByteLen];

    for(i = 0; i < (WByteLen + RByteLen); i++)
    {
        if(i < WByteLen)
        {
            WRBuff[i] = WBuffer[i];
        }
        else
        {
            WRBuff[i] = 0xFFU;
        }
    }

#if (EnableAutoCS==1)
    /*Step 1: check cs channel.*/
    (void)SPIM_CheckCSActive(cs_ch);


    /*Step 2: SPI write data.*/
    /* Master: start a transfer. Slave: prepare for a transfer. */
    (void)Cy_SCB_SPI_Transfer(mSPIM_HW, WRBuff, RtnBuff, (WByteLen + RByteLen), &mSPIM_context);
    timeout = 5000UL;

    /* Blocking wait for transfer completion */
    do
    {
        Cy_SysLib_DelayUs(SpiDelayUs);
        timeout--;
    }
    while((0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(mSPIM_HW, &mSPIM_context))) && (timeout > 0));

    /*Step 3: copy read data.*/
    if(timeout > 0)
    {
        rtn_status = FUNC_RETURN_OK;

        for(i = 0; i < RByteLen; i++)
        {
            RBuffer[i] = RtnBuff[WByteLen + i];
        }
    }
    else
    {
        rtn_status = FUNC_RETURN_NG;
    }

#else
    /*Step 1: check cs channel.*/
    SPIM_SetCSLevel(cs_ch, 0U);   /*En CS*/

    /*Step 2: SPI write data.*/
    /* Master: start a transfer. Slave: prepare for a transfer. */
    (void)Cy_SCB_SPI_Transfer(mSPIM_HW, WRBuff, RtnBuff, WByteLen, &mSPIM_context);
    timeout = 5000UL;

    /* Blocking wait for transfer completion */
    do
    {
        Cy_SysLib_DelayUs(SpiDelayUs);
        timeout--;
    }
    while((0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(mSPIM_HW, &mSPIM_context))) && (timeout > 0));

    /*Step 3: SPI read data.*/
    (void)Cy_SCB_SPI_Transfer(mSPIM_HW, &WRBuff[WByteLen], &RtnBuff[WByteLen], RByteLen, &mSPIM_context);
    timeout = 5000UL;

    /* Blocking wait for transfer completion */
    do
    {
        Cy_SysLib_DelayUs(SpiDelayUs);
        timeout--;
    }
    while((0UL != (CY_SCB_SPI_TRANSFER_ACTIVE & Cy_SCB_SPI_GetTransferStatus(mSPIM_HW, &mSPIM_context))) && (timeout > 0));

    /*Step 4: check cs channel.*/
    SPIM_SetCSLevel(cs_ch, 1U);   /*Dis CS*/

    /*Step 5: copy read data.*/
    if(timeout > 0)
    {
        rtn_status = FUNC_RETURN_OK;

        for(i = 0; i < RByteLen; i++)
        {
            RBuffer[i] = RtnBuff[WByteLen + i];
        }
    }
    else
    {
        rtn_status = FUNC_RETURN_NG;
    }

#endif

    return (rtn_status);

}

/* [] END OF FILE */

