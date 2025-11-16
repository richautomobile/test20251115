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
/*#include "cy_retarget_io.h"*/

#include "uart.h"
#include "Gpio.h"

/* ==[ Macros ]============================================================= */
#define UARD_CLOCK_BASE 923077U
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
static uint16_t uartRxBufferIndex = 0U;
static UartFifo_t uartRxFifo;

/*******************************************************************************
* Global Variables
*******************************************************************************/
static cy_stc_scb_uart_context_t mUART_context;
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static t_FUNC_RETURN Uart_Fifo_Push(uint8_t data[], uint16_t length);
static t_FUNC_RETURN Uart_Fifo_Pop(uint8_t buffer[], uint16_t *length);
/* ==[ Definition of functions with internal linkage ]======================= */
static t_FUNC_RETURN Uart_Fifo_Push(uint8_t data[], uint16_t length)
{

#if 1
    memcpy(uartRxFifo.buffer[uartRxFifo.head].data, data, length);
    uartRxFifo.buffer[uartRxFifo.head].length = length;
#else
    uint8_t copy_len;

    copy_len = (length < UART_RX_BUFFER_SIZE) ? length : (UART_RX_BUFFER_SIZE - 1U);
    memcpy(uartRxFifo.buffer[uartRxFifo.head].data, data, length);
    uartRxFifo.buffer[uartRxFifo.head].data[length] = '\0';
    uartRxFifo.buffer[uartRxFifo.head].length = copy_len;
#endif

    /*update uartRxFifo.head*/
    if(uartRxFifo.count < FIFO_BUFFER_SIZE)
    {
        uartRxFifo.head = (uartRxFifo.head + 1U) % FIFO_BUFFER_SIZE;
        uartRxFifo.count++;
        return FUNC_RETURN_OK;
    }
    else
    {
        uartRxFifo.head = uartRxFifo.tail;
        uartRxFifo.tail = (uartRxFifo.tail + 1U) % FIFO_BUFFER_SIZE;
        return FUNC_RETURN_NG;
    }

}

static t_FUNC_RETURN Uart_Fifo_Pop(uint8_t buffer[], uint16_t *length)
{
    uint8_t copy_len;

    if(uartRxFifo.count > 0U)
    {
        copy_len = uartRxFifo.buffer[uartRxFifo.tail].length;
        memcpy(buffer, uartRxFifo.buffer[uartRxFifo.tail].data, copy_len);
        *length = copy_len;
        /*update uartRxFifo.tail*/
        uartRxFifo.tail = (uartRxFifo.tail + 1) % FIFO_BUFFER_SIZE;
        uartRxFifo.count--;
        return FUNC_RETURN_OK;
    }
    else
    {
        *length = 0U;
        return FUNC_RETURN_NG;
    }

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
t_FUNC_RETURN Uart_Init(void)
{
    cy_en_scb_uart_status_t initstatus;


    /* Step1: Initialize the UART */
    initstatus = Cy_SCB_UART_Init(mUART_HW, &mUART_config, &mUART_context);

    /* Initialization failed. Handle error */
    if(initstatus != CY_SCB_UART_SUCCESS)
    {
        return FUNC_RETURN_NG;
    }

    /* Step 2: UART interrupt initialization structure  */
    cy_stc_sysint_t mUART_INT_cfg =
    {
        /*.intrSrc      =*/ mUART_IRQ,
        /*.intrPriority =*/ 3u
    };
    /* Step3: Initialize and enable the UART interrupt */
    Cy_SysInt_Init(&mUART_INT_cfg, &Uart_Interrupt);
    NVIC_EnableIRQ(mUART_INT_cfg.intrSrc);

    Cy_SCB_UART_Enable(mUART_HW);


    /* Step4: variable init*/
    uartRxFifo.head = 0U;
    uartRxFifo.tail = 0U;
    uartRxFifo.count = 0U;
    uartRxBufferIndex = 0U;

    /* Initialization completed */
    return FUNC_RETURN_OK;
}

/*******************************************************************************
* \brief Function Name: Isr_UART
********************************************************************************
*
* Summary:
* Handles UART Rx underflow and overflow conditions. This conditions must never
* occur.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Uart_Interrupt(void)
{
    uint32_t rx_intr_src;
    uint32_t tx_intr_src;
    uint32_t read_data;
    uint32_t number_rx_fifo;

    /* Step 1: Get RX interrupt sources */
    rx_intr_src =  Cy_SCB_UART_GetRxFifoStatus(mUART_HW);

    if(rx_intr_src > 0U)
    {
        Cy_SCB_UART_ClearRxFifoStatus(mUART_HW, rx_intr_src);
        number_rx_fifo = Cy_SCB_UART_GetNumInRxFifo(mUART_HW);

        /* Check if there is a received character from user console */
        if(0UL != number_rx_fifo)
        {
            /* Re-transmit whatever the user types on the console */
            read_data = Cy_SCB_UART_Get(mUART_HW);

            if(uartRxBufferIndex < (UART_RX_BUFFER_SIZE - 1))
            {
                uartRxBuffer[uartRxBufferIndex] = (uint8_t)read_data;
                uartRxBufferIndex++;
#if 1

                /*EOF=\r. \r=0x0D,*/
                if(uartRxBuffer[uartRxBufferIndex - 1U] == '\r')
                {
                    (void)Uart_Fifo_Push(uartRxBuffer, uartRxBufferIndex);
                    uartRxBufferIndex = 0U;
                }

#else

                /*EOF=\r\n.  \r=0x0D, \n=0x0A*/
                if((uartRxBufferIndex >= 2U))
                {

                    if((uartRxBuffer[uartRxBufferIndex - 2U] == '\r') &&
                            (uartRxBuffer[uartRxBufferIndex - 1U] == '\n'))
                    {
                        (void)Uart_Fifo_Push(uartRxBuffer, uartRxBufferIndex);
                        uartRxBufferIndex = 0U;
                    }
                }

#endif
            }
            else
            {
                uartRxBufferIndex = 0U;

            }

        }
    }

    /* Step 2: Get TX interrupt sources. */
    tx_intr_src =  Cy_SCB_UART_GetTxFifoStatus(mUART_HW);

    if(tx_intr_src > 0U)
    {
        Cy_SCB_UART_ClearTxFifoStatus(mUART_HW, tx_intr_src);
    }

    /* RX overflow or RX underflow or TX overflow occurred */
    /*uart_error = 1*/
}

/*******************************************************************************
* \brief Function Name: Uart_PutArray
********************************************************************************
*
* Summary:
* uart write API.   115200. 8.n.1
*
* Parameters:
*  uint8_t [] :  array buffer
*  uint32_t   :  array bytes
* Return:
*  t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN Uart_PutArray(uint8_t WArray[], uint32_t wLen)
{
    Cy_SCB_UART_PutArrayBlocking(mUART_HW, WArray, wLen);

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Uart_GetArray(uint8_t RArray[], uint16_t *rLen)
{

    return Uart_Fifo_Pop(RArray, rLen);
}

/* \brief Uart_SetBaudRate
 *
 *   uart clock base = 48M/52 = 923077Hz
 *   default oversample = 8
 *   over sample range = 8~16 => max baud rate = 923077/8 = 115384
 *                                             => min baud rate = 923077/16=  57692
 *   parameter:
 *      bps:  Max:115200, Min:57692  
 *   return:
 *      t_FUNC_RETURN:  TRUE or FALSE
 */
t_FUNC_RETURN Uart_SetBaudRate(uint32_t bps)
{

    cy_en_scb_uart_status_t initstatus;

    uint32_t oversample;

    oversample = (bps == 0) ? 8U : (UARD_CLOCK_BASE / bps);

    if(oversample < 8U)
    {
        oversample = 8U;
    }

    if(oversample > 16U)
    {
        oversample = 16U;
    }

    /* Step1: Initialize the UART */
    Cy_SCB_UART_Disable(mUART_HW, &mUART_context);
    initstatus = Cy_SCB_UART_SetOverSample(mUART_HW, oversample, &mUART_context);

    /* Initialization failed. Handle error */
    if(initstatus != CY_SCB_UART_SUCCESS)
    {
        return FUNC_RETURN_NG;
    }
    else
    {
        Cy_SCB_UART_Enable(mUART_HW);
        return FUNC_RETURN_OK;
    }
}

uint8_t Uart_Get_Fifo_DataLen(void)
{
    uint8_t dataLen;

    if(uartRxFifo.count > 0U)
    {
        dataLen = uartRxFifo.buffer[uartRxFifo.tail].length;
    }
    else
    {
        dataLen = 0xFFU;  /*empty*/
    }

    return dataLen;
}


