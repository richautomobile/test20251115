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
#include "i2cm.h"

/* ==[ Macros ]============================================================= */
#define I2cDelayUs   100U
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/** The instance-specific context structure.
 * It is used by the driver for internal configuration and
 * data keeping for the I2C. Do not modify anything in this structure.
 */
static cy_stc_scb_i2c_context_t mI2CMA_context;
static cy_stc_scb_i2c_context_t mI2CMB_context;
static uint16_t I2CMA_CLK_KHz = 1000U;  /*default 1000KHz*/

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]======================= */

/*******************************************************************************
* \brief Function Name: I2CMA_Init
********************************************************************************
*
* Summary:
*   This function initiates and enables master SCB
*   1MHz
*
* Return:
*   Status of initialization
*
*******************************************************************************/
t_FUNC_RETURN I2CMA_Init(void)
{
    cy_en_scb_i2c_status_t initStatus;

    /*Cy_SysClk_PeriphAssignDivider(PCLK_SCB3_CLOCK, CY_SYSCLK_DIV_16_BIT, 5U)*/

    /*Step 1: HW INIT*/
    /*Initialize and enable the I2C in master mode*/
    initStatus = Cy_SCB_I2C_Init(mI2CMA_HW, &mI2CMA_config, &mI2CMA_context);

    if(initStatus != CY_SCB_I2C_SUCCESS)
    {
        return FUNC_RETURN_NG;
    }

    /*Step 2: INT init and HOOK setting.*/

    /*Step 3: INT and HW enable.*/
    Cy_SCB_I2C_Enable(mI2CMA_HW, &mI2CMA_context);

    return FUNC_RETURN_OK;
}


/*******************************************************************************
* \brief Function Name: I2CMB_Init
********************************************************************************
*
* Summary:
*   This function initiates and enables master SCB
*   400KHZ
*
* Return:
*   Status of initialization
*
*******************************************************************************/
t_FUNC_RETURN I2CMB_Init(void)
{
    cy_en_scb_i2c_status_t initStatus;

    /*Cy_SysClk_PeriphAssignDivider(PCLK_SCB2_CLOCK, CY_SYSCLK_DIV_16_BIT, 1U)*/

    /*Step 1: HW INIT*/
    /*Initialize and enable the I2C in master mode*/
    initStatus = Cy_SCB_I2C_Init(mI2CMB_HW, &mI2CMB_config, &mI2CMB_context);

    if(initStatus != CY_SCB_I2C_SUCCESS)
    {
        return FUNC_RETURN_NG;
    }

    /*Step 2: INT init and HOOK setting.*/

    /*Step 3: INT and HW enable.*/
    Cy_SCB_I2C_Enable(mI2CMB_HW, &mI2CMB_context);

    return FUNC_RETURN_OK;
}

/*
 * \brief Function Name:I2CMA_SetCLK
 *
 *  I2CMA Normal Frequency=1MHz, high_duty_cycle=7, low_duty_cycle=9
 *  I2CMA      max Frequency=8MHz, high_duty_cycle=1, low_duty_cycle=1
 *  I2CMA      min  Frequency=551KHz, high_duty_cycle=(29*0.45)=13, low_duty_cycle=16
 *
 * Low duty vaild value : 1~16 step 1
 * Hi  duty vaild value : 1~16 step 1
 *
 * Parameters:
 * uint16_t KHz
 *
 * Return:
 * Status of initialization
 *
 */
t_FUNC_RETURN I2CMA_SetCLK(uint16_t KHz)
{
    cy_stc_scb_i2c_config_t new_mI2CMA_config = mI2CMA_config;  /**/
    uint16_t I2C_CLK_BASE = 16000U;  /*16MHZ, KHz*/
    uint16_t new_i2c_clk = (uint16_t)(I2C_CLK_BASE / KHz);
    uint16_t high_duty_cycle = (uint16_t)((new_i2c_clk * 45) / 100); /*Hi duty=45%, Low duty=55%*/
    uint16_t low_duty_cycle;

    if(new_i2c_clk >= 2U &&  new_i2c_clk <= 16U)
    {
        if(high_duty_cycle <= 1U)
        {
            high_duty_cycle = 1U;
        }

        low_duty_cycle = new_i2c_clk - high_duty_cycle;

        new_mI2CMA_config.highPhaseDutyCycle = high_duty_cycle;
        new_mI2CMA_config.lowPhaseDutyCycle = low_duty_cycle;
        (void)Cy_SCB_I2C_Init(mI2CMA_HW, &new_mI2CMA_config, &mI2CMA_context);
        I2CMA_CLK_KHz = new_i2c_clk;
        return FUNC_RETURN_OK;
    }
    else
    {
        return FUNC_RETURN_NG;
    }
}

uint16_t I2CMA_GetCLK(void)
{
    return I2CMA_CLK_KHz;
}

/*******************************************************************************
* \brief Function Name: WritePacketToEzI2C
******************************************************************************
*
* Summary:
*   Buffer is assigned with data to be sent to slave.
*   high level PDL library function is used to control I2C SCB to send data to
*   EzI2C slave. Errors are handled depend on the return value from the
*   appropriate function.
*
* Parameters:
*   SlaveAddress: 7bits device address
*   writebuffer: Command packet buffer pointer
*   bufferSize: Size of the packet buffer
*
* Return:
*   Status after command is written to slave.
*   FUNC_RETURN_NG is returned if any error occurs.
*   FUNC_RETURN_OK is returned if write is successful.
*
*******************************************************************************/
t_FUNC_RETURN I2CMA_Write(uint8_t SlaveAddress, uint8_t WBuffer[], uint32_t WByteLen)
{
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;
    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(mI2CMA_HW, SlaveAddress, CY_SCB_I2C_WRITE_XFER, timeout, &mI2CMA_context);

    if(CY_SCB_I2C_SUCCESS == status)
    {
        uint32_t cnt = 0UL;

        /* Write data into the slave from the buffer */
        do
        {
            /* Write byte and receive ACK/NACK response */
            status = Cy_SCB_I2C_MasterWriteByte(mI2CMA_HW, WBuffer[cnt], timeout, &mI2CMA_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < WByteLen));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {
        /* Send Stop condition on the bus */
        status = Cy_SCB_I2C_MasterSendStop(mI2CMA_HW, timeout, &mI2CMA_context);

        if(status == CY_SCB_I2C_SUCCESS)
        {
            /* Data has been written into the slave */
            rtn_status = FUNC_RETURN_OK;
        }
    }
    else
    {
        /* Other statuses do not require any actions */
        rtn_status = FUNC_RETURN_NG;
    }

    return rtn_status;
}

/*******************************************************************************
* \brief Function Name: I2CMA_WriteB
******************************************************************************
*
* Summary:
*   Buffer is assigned with data to be sent to slave.
*   high level PDL library function is used to control I2C SCB to send data to
*   EzI2C slave. Errors are handled depend on the return value from the
*   appropriate function.
*
* Parameters:
*   SlaveAddress: 7bits device address
*   writebuffer: Command packet buffer pointer
*   bufferSize: Size of the packet buffer
*
* Return:
*   Status after command is written to slave.
*   FUNC_RETURN_NG is returned if any error occurs.
*   FUNC_RETURN_OK is returned if write is successful.
*
*******************************************************************************/
t_FUNC_RETURN I2CMA_WriteB(uint8_t SlaveAddress, uint8_t WReg, uint8_t WData)
{
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    uint32_t cnt = 0UL;
    uint8_t WBuffer[2] = {WReg, WData};
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;
    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(mI2CMA_HW, SlaveAddress, CY_SCB_I2C_WRITE_XFER, timeout, &mI2CMA_context);

    if(CY_SCB_I2C_SUCCESS == status)
    {
        /* Write data into the slave from the buffer */
        do
        {
            /* Write byte and receive ACK/NACK response */
            status = Cy_SCB_I2C_MasterWriteByte(mI2CMA_HW, WBuffer[cnt], timeout, &mI2CMA_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < 2U));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {
        /* Send Stop condition on the bus */
        status = Cy_SCB_I2C_MasterSendStop(mI2CMA_HW, timeout, &mI2CMA_context);

        if(status == CY_SCB_I2C_SUCCESS)
        {
            /* Data has been written into the slave */
            rtn_status = FUNC_RETURN_OK;
        }
    }
    else
    {
        /* Other statuses do not require any actions */
        rtn_status = FUNC_RETURN_NG;
        printf("I2CMA_WriteB Fail!\r\n");
    }

    return rtn_status;
}

/*******************************************************************************
* \brief Function Name: ReadStatusPacketFromEzI2C
******************************************************************************
*
* Summary:
*   Master initiates the read from EzI2C buffer.
*   The status of the transfer is returned by comparing the data in EzI2C buffer.
*
* Return:
*   Status of the transfer by checking packets read.
*   Note that if the status packet read is correct function returns TRANSFER_CMPLT
*   and if status packet is incorrect function returns TRANSFER_ERROR.
*
*******************************************************************************/
t_FUNC_RETURN I2CMA_DirRead(uint8_t SlaveAddress, uint8_t RBuffer[], uint32_t RByteLen)
{
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;

    /*
    * Check if the condition is normal start or Re-start
    */
    if(mI2CMA_context.state == CY_SCB_I2C_IDLE)
    {
        /* IDLE condition, this means Master should send normal start*/
        /* Send Start condition with Slave address and receive ACK/NACK response from slave */
        status = Cy_SCB_I2C_MasterSendStart(mI2CMA_HW, SlaveAddress, CY_SCB_I2C_READ_XFER, timeout, &mI2CMA_context);
    }
    else
    {
        status = Cy_SCB_I2C_MasterSendReStart(mI2CMA_HW, SlaveAddress, CY_SCB_I2C_READ_XFER, timeout, &mI2CMA_context);
    }

    if(CY_SCB_I2C_SUCCESS == status)
    {
        uint32_t cnt = 0UL;
        cy_en_scb_i2c_command_t cmd = CY_SCB_I2C_ACK;

        /* Read data from the slave into the buffer */
        do
        {
            if(cnt == (RByteLen - 1UL))
            {
                /* The last byte must be NACKed */
                cmd = CY_SCB_I2C_NAK;
            }

            /* Read byte and generate ACK / or prepare for NACK */
            status = Cy_SCB_I2C_MasterReadByte(mI2CMA_HW, cmd, &RBuffer[cnt], timeout, &mI2CMA_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < RByteLen));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {
        /* Send Stop condition on the bus */
        status = Cy_SCB_I2C_MasterSendStop(mI2CMA_HW, timeout, &mI2CMA_context);

        if(status == CY_SCB_I2C_SUCCESS)
        {
            /* Process received data */
            rtn_status = FUNC_RETURN_OK;
        }
    }
    else
    {
        /* Other statuses do not require any actions.
        * The received data should dropped.
        */
        rtn_status = FUNC_RETURN_NG;
    }

    return rtn_status;
}

/*******************************************************************************
* \brief Function Name: ReadStatusPacketFromEzI2C
******************************************************************************
*
* Summary:
*   Low level I2C read.
*
* Return:
*   Status of the transfer by checking packets read.
*   Note that if the status packet read is correct function returns TRANSFER_CMPLT
*   and if status packet is incorrect function returns TRANSFER_ERROR.
*
*******************************************************************************/
t_FUNC_RETURN I2CMA_Read(uint8_t SlaveAddress, uint8_t WBuffer[],  uint32_t WByteLen, uint8_t RBuffer[], uint32_t RByteLen)
{
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;

    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(mI2CMA_HW, SlaveAddress, CY_SCB_I2C_WRITE_XFER, timeout, &mI2CMA_context);

    if(CY_SCB_I2C_SUCCESS == status)
    {
        uint32_t cnt = 0UL;

        /* Write data into the slave from the buffer */
        do
        {
            /* Write byte and receive ACK/NACK response */
            status = Cy_SCB_I2C_MasterWriteByte(mI2CMA_HW, WBuffer[cnt], timeout, &mI2CMA_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < WByteLen));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {

        rtn_status = I2CMA_DirRead(SlaveAddress, RBuffer, RByteLen);
    }

    return rtn_status;
}

/*******************************************************************************
* \brief Function Name: I2CMA_ReadB
******************************************************************************
*
* Summary:
*   Low level I2C read.
*
* Return:
*   Status of the transfer by checking packets read.
*   Note that if the status packet read is correct function returns TRANSFER_CMPLT
*   and if status packet is incorrect function returns TRANSFER_ERROR.
*
*******************************************************************************/
uint8_t I2CMA_ReadB(uint8_t SlaveAddress, uint8_t RReg)
{
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    uint32_t cnt = 0UL;
    uint8_t WBuffer[1] = {RReg};
    uint8_t RBuffer[1];
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;

    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(mI2CMA_HW, SlaveAddress, CY_SCB_I2C_WRITE_XFER, timeout, &mI2CMA_context);

    if(CY_SCB_I2C_SUCCESS == status)
    {
        /* Write data into the slave from the buffer */
        do
        {
            /* Write byte and receive ACK/NACK response */
            status = Cy_SCB_I2C_MasterWriteByte(mI2CMA_HW, WBuffer[cnt], timeout, &mI2CMA_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < 1U));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {

        rtn_status = I2CMA_DirRead(SlaveAddress, RBuffer, 1);
    }

    if(FUNC_RETURN_OK == rtn_status)
    {
        return RBuffer[0];
    }
    else
    {
        return 0xFF;
    }
}


/*******************************************************************************
* \brief Function Name: I2CMB_Write
******************************************************************************
*
* Summary:
*   Buffer is assigned with data to be sent to slave.
*   high level PDL library function is used to control I2C SCB to send data to
*   EzI2C slave. Errors are handled depend on the return value from the
*   appropriate function.
*
* Parameters:
*   SlaveAddress: 7bits device address
*   writebuffer: Command packet buffer pointer
*   bufferSize: Size of the packet buffer
*
* Return:
*   Status after command is written to slave.
*   FUNC_RETURN_NG is returned if any error occurs.
*   FUNC_RETURN_OK is returned if write is successful.
*
*******************************************************************************/
t_FUNC_RETURN I2CMB_Write(uint8_t SlaveAddress, uint8_t WBuffer[], uint32_t WByteLen)
{
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;
    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(mI2CMB_HW, SlaveAddress, CY_SCB_I2C_WRITE_XFER, timeout, &mI2CMB_context);

    if(CY_SCB_I2C_SUCCESS == status)
    {
        uint32_t cnt = 0UL;

        /* Write data into the slave from the buffer */
        do
        {
            /* Write byte and receive ACK/NACK response */
            status = Cy_SCB_I2C_MasterWriteByte(mI2CMB_HW, WBuffer[cnt], timeout, &mI2CMB_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < WByteLen));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {
        /* Send Stop condition on the bus */
        status = Cy_SCB_I2C_MasterSendStop(mI2CMB_HW, timeout, &mI2CMB_context);

        if(status == CY_SCB_I2C_SUCCESS)
        {
            /* Data has been written into the slave */
            rtn_status = FUNC_RETURN_OK;
        }
    }
    else
    {
        /* Other statuses do not require any actions */
        rtn_status = FUNC_RETURN_NG;
    }

    return rtn_status;
}

/*******************************************************************************
* \brief Function Name: I2CMB_WriteB
******************************************************************************
*
* Summary:
*   Buffer is assigned with data to be sent to slave.
*   high level PDL library function is used to control I2C SCB to send data to
*   EzI2C slave. Errors are handled depend on the return value from the
*   appropriate function.
*
* Parameters:
*   SlaveAddress: 7bits device address
*   writebuffer: Command packet buffer pointer
*   bufferSize: Size of the packet buffer
*
* Return:
*   Status after command is written to slave.
*   FUNC_RETURN_NG is returned if any error occurs.
*   FUNC_RETURN_OK is returned if write is successful.
*
*******************************************************************************/
t_FUNC_RETURN I2CMB_WriteB(uint8_t SlaveAddress, uint8_t WReg, uint8_t WData)
{
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    uint32_t cnt = 0UL;
    uint8_t WBuffer[2] = {WReg, WData};
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;
    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(mI2CMB_HW, SlaveAddress, CY_SCB_I2C_WRITE_XFER, timeout, &mI2CMB_context);

    if(CY_SCB_I2C_SUCCESS == status)
    {
        /* Write data into the slave from the buffer */
        do
        {
            /* Write byte and receive ACK/NACK response */
            status = Cy_SCB_I2C_MasterWriteByte(mI2CMB_HW, WBuffer[cnt], timeout, &mI2CMB_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < 2U));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {
        /* Send Stop condition on the bus */
        status = Cy_SCB_I2C_MasterSendStop(mI2CMB_HW, timeout, &mI2CMB_context);

        if(status == CY_SCB_I2C_SUCCESS)
        {
            /* Data has been written into the slave */
            rtn_status = FUNC_RETURN_OK;
        }
    }
    else
    {
        /* Other statuses do not require any actions */
        rtn_status = FUNC_RETURN_NG;
    }

    return rtn_status;
}



/*******************************************************************************
* \brief Function Name: ReadStatusPacketFromEzI2C
******************************************************************************
*
* Summary:
*   Master initiates the read from EzI2C buffer.
*   The status of the transfer is returned by comparing the data in EzI2C buffer.
*
* Return:
*   Status of the transfer by checking packets read.
*   Note that if the status packet read is correct function returns TRANSFER_CMPLT
*   and if status packet is incorrect function returns TRANSFER_ERROR.
*
*******************************************************************************/
t_FUNC_RETURN I2CMB_DirRead(uint8_t SlaveAddress, uint8_t RBuffer[], uint32_t RByteLen)
{
    uint32_t cnt = 0UL;
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;


    /*
    * Check if the condition is normal start or Re-start
    */
    if(mI2CMB_context.state == CY_SCB_I2C_IDLE)
    {
        /* IDLE condition, this means Master should send normal start*/
        /* Send Start condition with Slave address and receive ACK/NACK response from slave */
        status = Cy_SCB_I2C_MasterSendStart(mI2CMB_HW, SlaveAddress, CY_SCB_I2C_READ_XFER, timeout, &mI2CMB_context);
    }
    else
    {
        status = Cy_SCB_I2C_MasterSendReStart(mI2CMB_HW, SlaveAddress, CY_SCB_I2C_READ_XFER, timeout, &mI2CMB_context);
    }

    if(CY_SCB_I2C_SUCCESS == status)
    {
        cy_en_scb_i2c_command_t cmd = CY_SCB_I2C_ACK;

        /* Read data from the slave into the buffer */
        do
        {
            if(cnt == (RByteLen - 1UL))
            {
                /* The last byte must be NACKed */
                cmd = CY_SCB_I2C_NAK;
            }

            /* Read byte and generate ACK / or prepare for NACK */
            status = Cy_SCB_I2C_MasterReadByte(mI2CMB_HW, cmd, &RBuffer[cnt], timeout, &mI2CMB_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < RByteLen));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {
        /* Send Stop condition on the bus */
        status = Cy_SCB_I2C_MasterSendStop(mI2CMB_HW, timeout, &mI2CMB_context);

        if(status == CY_SCB_I2C_SUCCESS)
        {
            /* Process received data */
            rtn_status = FUNC_RETURN_OK;
        }
    }
    else
    {
        /* Other statuses do not require any actions.
                * The received data should dropped.
                */
        rtn_status = FUNC_RETURN_NG;
    }

    return rtn_status;
}

/*******************************************************************************
* \brief Function Name: ReadStatusPacketFromEzI2C
******************************************************************************
*
* Summary:
*   Low level I2C read.
*
* Return:
*   Status of the transfer by checking packets read.
*   Note that if the status packet read is correct function returns TRANSFER_CMPLT
*   and if status packet is incorrect function returns TRANSFER_ERROR.
*
*******************************************************************************/
t_FUNC_RETURN I2CMB_Read(uint8_t SlaveAddress, uint8_t WBuffer[],  uint32_t WByteLen, uint8_t RBuffer[], uint32_t RByteLen)
{
    uint32_t cnt = 0UL;
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;
    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(mI2CMB_HW, SlaveAddress, CY_SCB_I2C_WRITE_XFER, timeout, &mI2CMB_context);

    if(CY_SCB_I2C_SUCCESS == status)
    {
        /* Write data into the slave from the buffer */
        do
        {
            /* Write byte and receive ACK/NACK response */
            status = Cy_SCB_I2C_MasterWriteByte(mI2CMB_HW, WBuffer[cnt], timeout, &mI2CMB_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < WByteLen));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {

        rtn_status = I2CMB_DirRead(SlaveAddress, RBuffer, RByteLen);
    }

    return rtn_status;
}

/*******************************************************************************
* \brief Function Name: I2CMB_ReadB
******************************************************************************
*
* Summary:
*   Low level I2C read.
*
* Return:
*   Status of the transfer by checking packets read.
*   Note that if the status packet read is correct function returns TRANSFER_CMPLT
*   and if status packet is incorrect function returns TRANSFER_ERROR.
*
*******************************************************************************/
uint8_t I2CMB_ReadB(uint8_t SlaveAddress, uint8_t RReg)
{
    t_FUNC_RETURN rtn_status = FUNC_RETURN_NG;
    cy_en_scb_i2c_status_t status;
    uint32_t cnt = 0UL;
    uint8_t WBuffer[1] = {RReg};
    uint8_t RBuffer[1];
    /* Wait 100 ms until operation completion */
    uint32_t timeout = 100UL;

    /* Send Start condition, address and receive ACK/NACK response from slave */
    status = Cy_SCB_I2C_MasterSendStart(mI2CMB_HW, SlaveAddress, CY_SCB_I2C_WRITE_XFER, timeout, &mI2CMB_context);

    if(CY_SCB_I2C_SUCCESS == status)
    {
        /* Write data into the slave from the buffer */
        do
        {
            /* Write byte and receive ACK/NACK response */
            status = Cy_SCB_I2C_MasterWriteByte(mI2CMB_HW, WBuffer[cnt], timeout, &mI2CMB_context);
            ++cnt;
        }
        while((status == CY_SCB_I2C_SUCCESS) && (cnt < 1U));
    }

    /* Check status of transaction */
    if((status == CY_SCB_I2C_SUCCESS)           ||
            (status == CY_SCB_I2C_MASTER_MANUAL_NAK) ||
            (status == CY_SCB_I2C_MASTER_MANUAL_ADDR_NAK))
    {

        rtn_status = I2CMB_DirRead(SlaveAddress, RBuffer, 1);
    }

    if(FUNC_RETURN_OK == rtn_status)
    {
        return RBuffer[0];
    }
    else
    {
        return 0xFF;
    }
}

