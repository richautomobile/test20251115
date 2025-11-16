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
#include <stdio.h>
#include "i2cs.h"
#include "system.h"
#include "adc.h"
#include "pwm_capture.h"
#include "gpio.h"
#include "DevEncoder.h"
#include "I2cm.h"
#include "Spimaster.h"
#include "SoftwareTimer.h"
#include "user_flash.h"
#include "wdt.h"
#if (LOT3_BOOT_ENABLE==1U)
#include "BootCode.h"
#endif
#include "tcpwm_out.h"
#include "swpwm_out.h"
#include "DevINA236.h"
#include "button.h"
#include "flash.h"
#include "canfd.h"
#include "uart.h"


/* ==[ Macros ]============================================================= */
/* Valid command packet size of three bytes */
#define PACKET_SIZE          (280u) /*(0x03u)*/

/* Master write and read buffer of size three bytes */
#define SL_RD_BUFFER_SIZE    (PACKET_SIZE)
#define SL_WR_BUFFER_SIZE    (PACKET_SIZE)

/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
/** The instance-specific context structure.
 * It is used by the driver for internal configuration and
 * data keeping for the I2C. Do not modify anything in this structure.
 */
static uint32_t I2cs_WriteLen;
static cy_stc_scb_i2c_context_t mI2CS_context;
static uint8_t UpdataAPP_Stage = 0;
/* CYBSP_I2C_SCB_IRQ */
static cy_stc_sysint_t mI2CS_IRQ_config =
{
    .intrSrc      = mI2CS_IRQ,
    .intrPriority = 3U
};
/* I2C read and write buffers */
static uint8_t i2csReadBuffer[SL_RD_BUFFER_SIZE];
static uint8_t i2csWriteBuffer[SL_WR_BUFFER_SIZE];
#if (LOT3_BOOT_ENABLE==0U)
static struct StrI2csReceiveData I2cs2StepRunBuff;
static struct StrRtnData ReqCmdRtnBuff;
static uint8_t fakePageBuff[256];
static uint32_t CurrFakeFlashPageAddr = 0xFFFFFFU;
#endif

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static void I2cs_EvenCallback(uint32_t event);
static t_FUNC_RETURN I2cs_WriteBufProcess(uint32_t wLen);
static t_FUNC_RETURN I2cs_ReadBufProcess(uint32_t wLen);
#if (LOT3_BOOT_ENABLE==1U)
/*
*       I2cs_WriteBufProcess   -->   I2cs_BootWBufProc
*
*       I2cs_ReadBufProcess   -->   I2cs_BootRBufProc
*/
static t_FUNC_RETURN I2cs_BootWBufProc(uint32_t wLen);
static t_FUNC_RETURN I2cs_BootRBufProc(uint32_t wLen);
#else
/*
*       I2cs_WriteBufProcess   -->   I2cs_AppWBufProc
*                                           |
*       I2cs_ReadBufProcess   -->   I2cs_AppRBufProc
*/
static t_FUNC_RETURN I2cs_AppWBufProc(uint32_t wLen);
static t_FUNC_RETURN I2cs_AppRBufProc(uint32_t wLen);
static t_FUNC_RETURN I2cs_CMD0_AppWBufProc(uint32_t wLen);
static t_FUNC_RETURN I2cs_CMD0_AppRBufProc(uint32_t wLen);
static t_FUNC_RETURN I2cs_WBuffVerifyCHKSUM(uint32_t wLen);
static uint8_t I2cs_RBuffCalCHKSUM(uint32_t rLen);
#endif
/*command file*/
static t_FUNC_RETURN I2cs_CheckAppEnterBoot(uint8_t Key0, uint8_t Key1);

/* ==[ Definition of functions with internal linkage ]======================= */
/*******************************************************************************
 * \brief Function Name: I2cs_EvenCallback
 *******************************************************************************
 *
 * Summary:
 *  Handles slave events write and read completion events.
 *
 * Parameters:
 *  event:  Reports slave events.
 *
 ******************************************************************************/
static void I2cs_EvenCallback(uint32_t event)
{
    /* Check write complete event or restart */
    if(0UL != (CY_SCB_I2C_SLAVE_WR_CMPLT_EVENT & event))
    {
        /* Check for errors */
        if(0UL == (CY_SCB_I2C_SLAVE_ERR_EVENT & event))
        {
            /* Check packet length */
            I2cs_WriteLen = Cy_SCB_I2C_SlaveGetWriteTransferCount(mI2CS_HW, &mI2CS_context);

            if(I2cs_WriteLen > 0U)
            {
                /* Execute command and update reply status for received
                                * command
                                */
                if(0UL != (mI2CS_HW->INTR_S & 0x10000UL))  /*bit16: get I2C_RESTART state*/
                {
                    (void)I2cs_ReadBufProcess(I2cs_WriteLen);
                    mI2CS_HW->INTR_S = 0x10000UL;  /*clear I2C_RESTART state.*/
                }
                else
                {
                    (void)I2cs_WriteBufProcess(I2cs_WriteLen);
                }
            }
        }

        /* Configure write buffer for the next write */
        Cy_SCB_I2C_SlaveConfigWriteBuf(mI2CS_HW, i2csWriteBuffer,
                                       SL_WR_BUFFER_SIZE, &mI2CS_context);
    }

    /* Check read complete event */
    if(0UL != (CY_SCB_I2C_SLAVE_RD_CMPLT_EVENT & event))
    {
        /* Configure read buffer for the next read */
        Cy_SCB_I2C_SlaveConfigReadBuf(mI2CS_HW, i2csReadBuffer,
                                      SL_RD_BUFFER_SIZE, &mI2CS_context);
    }
}


/*******************************************************************************
 *  \brief Function Name: I2cs_WriteBufProcess
 *******************************************************************************
 *
 * I2Cs write buffer process
 *
 *
 * \Parameters [in] uint32: master write buffer length
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN I2cs_WriteBufProcess(uint32_t wLen)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
#if (LOT3_BOOT_ENABLE==1U)

    if(FUNC_RETURN_OK == Boot_CheckWCmdLength(i2csWriteBuffer[0], wLen))
    {
        rtn_state = I2cs_BootWBufProc(wLen);
    }
    else
    {
        (void)Boot_UpdateStatus(UNKNOWN_ERROR);
        rtn_state = FUNC_RETURN_NG;
    }

#else

    if((i2csWriteBuffer[0] == BOOT_MCU_RESET) || (i2csWriteBuffer[0] == BOOT_RW_BOOTKEY))
    {
        rtn_state = I2cs_AppWBufProc(wLen);
    }
    else
    {
        /*CMD0*/
        if(i2csWriteBuffer[0] == 0U)
        {
            rtn_state = I2cs_CMD0_AppWBufProc(wLen);
#if 0

            if(FUNC_RETURN_NG == rtn_state)
            {
                rtn_state = I2cs_CMD0_AppRBufProc(wLen);
            }

#endif
        }

        /*For direct read */
        if(i2csWriteBuffer[0] == 0xF0U)
        {
            rtn_state = I2cs_CMD0_AppRBufProc(wLen);
        }
    }

#endif

    return rtn_state;
}

/*******************************************************************************
 *  \brief Function Name: I2cs_ReadBufProcess
 *******************************************************************************
 *
 * Set read buffer process for I2CS
 *
 *
 * \Parameters [in] uint32: master write buffer length
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN I2cs_ReadBufProcess(uint32_t wLen)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;

#if (LOT3_BOOT_ENABLE==1U)
    rtn_state = I2cs_BootRBufProc(wLen);
    return rtn_state;

#else

    if((i2csWriteBuffer[0] == BOOT_HW_VER) || (i2csWriteBuffer[0] == BOOT_FW_VER) || \
            (i2csWriteBuffer[0] == BOOT_RW_BOOTKEY) || (i2csWriteBuffer[0] == BOOT_READ_STATE))
    {
        /*Suport Boot command in the App mode.*/
        rtn_state = I2cs_AppRBufProc(wLen);
    }
    else
    {
        /*CMD0*/
        if(i2csWriteBuffer[0] == 0xF0U)
        {
            rtn_state = I2cs_CMD0_AppRBufProc(wLen);
        }
    }

    return rtn_state;

#endif

}

#if (LOT3_BOOT_ENABLE==1U)
/*******************************************************************************
 *  \brief Function Name: I2cs_BootWBufProc
 *******************************************************************************
 *
 * Both App mode and Boot mode are required.
 * Suport Boot command in the App mode.
 *
 * \Parameters [in] uint32: master write buffer length
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN I2cs_BootWBufProc(uint32_t wLen)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;

    switch(i2csWriteBuffer[0])
    {
        case BOOT_MCU_RESET:
            rtn_state = FUNC_RETURN_OK;

            if((i2csWriteBuffer[1] & 0x02U) > 0U)
            {
                (void)Sys_SoftReset();
            }

            break;

        case BOOT_RW_BOOTKEY:
            rtn_state = I2cs_CheckAppEnterBoot(i2csWriteBuffer[1], i2csWriteBuffer[2]);

            if(rtn_state == FUNC_RETURN_OK)
            {
                (void)Boot_SetSessionKeyUser(((uint16_t)i2csWriteBuffer[2] << 8U) | i2csWriteBuffer[1]);
                (void)Sys_SetSessionKey(0U);
            }

            break;
#if 1

        /*Run bootloader process in the interrupt .*/
        case BOOT_APP_UNLOCK:  /*4 bytes*/
            rtn_state = Boot_AppUnlock(&i2csWriteBuffer[1]);
            break;

        case BOOT_APP_ERASE:  /*5 bytes*/
            rtn_state = Boot_AppErase(&i2csWriteBuffer[1]);
            break;

        case BOOT_APP_WRITE_PAGE: /*71 bytes*/
            /*0x8D, Session1, Session2, Addr0, Addr1, Addr2, D1~Dn, CRC0, CRC1*/
            rtn_state = Boot_AppWrite(&i2csWriteBuffer[1], (wLen - 8U));
            break;
#else

        /*Run bootloader process in main loop.*/
        case BOOT_APP_UNLOCK:  /*4 bytes*/
        case BOOT_APP_ERASE:  /*5 bytes*/
        case BOOT_APP_WRITE_PAGE: /*71 bytes*/
            rtn_state = FUNC_RETURN_OK;
            I2cs2StepRunBuff.enable = 1U;
            I2cs2StepRunBuff.WLen = wLen;

            for(int i = 0; i < wLen; i++)
            {
                I2cs2StepRunBuff.WBuff[i] = i2csWriteBuffer[0];
            }

            break;
#endif

        default:
            rtn_state = FUNC_RETURN_NG;
            (void)Boot_UpdateStatus(UNKNOWN_ERROR);
            break;
    }

    return rtn_state;

}

/*******************************************************************************
 *  \brief Function Name: I2cs_BootRBufProc
 *******************************************************************************
 *
 * Both App mode and Boot mode are required.
 * Suport Boot command in the App mode.
 *
 * \Parameters [in] uint32: master write buffer length
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN I2cs_BootRBufProc(uint32_t wLen)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
    uint16_t get_data = 0U;

    switch(i2csWriteBuffer[0])
    {
        case BOOT_HW_VER:  /*This command is not supported in APP mode.*/
            rtn_state = FUNC_RETURN_OK;
            get_data = Sys_GetHWVer();
            i2csReadBuffer[0] = (uint8_t)get_data;
            i2csReadBuffer[1] = (uint8_t)(get_data >> 8U);
            break;

        case BOOT_FW_VER:
            rtn_state = FUNC_RETURN_OK;
            i2csReadBuffer[0] = (uint8_t)FW_APP_MINOR_VER;
            i2csReadBuffer[1] = (uint8_t)FW_APP_VER;
            break;

        case BOOT_RW_BOOTKEY:
            rtn_state = FUNC_RETURN_OK;
            get_data = Boot_GetSessionKeyUser();
            i2csReadBuffer[0] = (uint8_t)get_data;
            i2csReadBuffer[1] = (uint8_t)(get_data >> 8U);
            break;


        case BOOT_READ_STATE:
            rtn_state = Boot_CmdStatusRegister(&i2csReadBuffer[0]);
            break;

        default:
            rtn_state = FUNC_RETURN_NG;
            break;
    }

    return rtn_state;
}
#endif


#if (LOT3_BOOT_ENABLE==0U)
/*******************************************************************************
 *  \brief Function Name: I2cs_AppWBufProc
 *******************************************************************************
 *
 * Both App mode and Boot mode are required.
 * Suport Boot command in the App mode.
 *
 * \Parameters [in] uint32: master write buffer length
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN I2cs_AppWBufProc(uint32_t wLen)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;

    switch(i2csWriteBuffer[0])
    {
        case BOOT_MCU_RESET:
            rtn_state = FUNC_RETURN_OK;

            if((i2csWriteBuffer[1] & 0x02U) > 0U)
            {
                (void)Sys_SoftReset();
            }

            break;

        case BOOT_RW_BOOTKEY:
            rtn_state = I2cs_CheckAppEnterBoot(i2csWriteBuffer[1], i2csWriteBuffer[2]);

            break;

        default:
            rtn_state = FUNC_RETURN_NG;
            break;
    }

    return rtn_state;

}

/*******************************************************************************
 *  \brief Function Name: I2cs_AppRBufProc
 *******************************************************************************
 *
 * Both App mode and Boot mode are required.
 * Suport Boot command in the App mode.
 *
 * \Parameters [in] uint32: master write buffer length
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN I2cs_AppRBufProc(uint32_t wLen)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
    uint16_t get_data = 0U;

    switch(i2csWriteBuffer[0])
    {
        case BOOT_HW_VER:  /*This command is not supported in APP mode.*/
            rtn_state = FUNC_RETURN_OK;
            get_data = Sys_GetHWVer();
            i2csReadBuffer[0] = (uint8_t)get_data;
            i2csReadBuffer[1] = (uint8_t)(get_data >> 8U);
            break;

        case BOOT_FW_VER:
            rtn_state = FUNC_RETURN_OK;
            i2csReadBuffer[0] = (uint8_t)FW_APP_MINOR_VER;
            i2csReadBuffer[1] = (uint8_t)FW_APP_VER;
            break;

        case BOOT_RW_BOOTKEY:
            rtn_state = FUNC_RETURN_OK;
            get_data = Sys_GetSessionKey();
            i2csReadBuffer[0] = (uint8_t)get_data;
            i2csReadBuffer[1] = (uint8_t)(get_data >> 8U);
            break;


        case BOOT_READ_STATE:
            /*return app state*/
            i2csReadBuffer[0] = 0x00U;
            i2csReadBuffer[1] = 0x00U;
            i2csReadBuffer[2] = 0x00U; /*CRC16 LSB*/
            i2csReadBuffer[3] = 0x00U; /*CRC16 MSB*/
            rtn_state = FUNC_RETURN_OK;
            break;

        default:
            rtn_state = FUNC_RETURN_NG;
            break;
    }

    return rtn_state;
}

/*******************************************************************************
 *  \brief Function Name: I2cs_AppWBufProc
 *******************************************************************************
 *
 * App mode only.
 * CMD0 I2Cs writing process.
 *
 * \Parameters [in] uint32: master write buffer length
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN I2cs_CMD0_AppWBufProc(uint32_t wLen)
{
    uint16_t i;
    uint16_t c_wLen;
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;

    /*===========================*/
    rtn_state = I2cs_WBuffVerifyCHKSUM(wLen);

    if(rtn_state == FUNC_RETURN_OK)
    {
        /*Tooling board CMD.*/
        switch(i2csWriteBuffer[1])
        {
            case CMD_SET_LOT3_IO:
                rtn_state = Gpio_WritePin(((i2csWriteBuffer[2] << 4U) | i2csWriteBuffer[3]), i2csWriteBuffer[4]);
                break;

            case CMD_SET_LOT3_PORT:
                rtn_state = Gpio_WritePort(i2csWriteBuffer[2], i2csWriteBuffer[3]);
                break;

            case CMD_PSOC_RESET:
#if 1
                (void)Sys_SoftReset();
#else /*Wdt test*/
                Cy_SysLib_Delay(2000U);
                Gpio_WritePin(EVM_LED1, 0);
                Gpio_WritePin(EVM_LED2, 0);
                Gpio_WritePin(EVM_LED3, 0);
#endif
                break;

            case CMD_SET_PSOC_IICA_CLOCK:
                rtn_state = I2CMA_SetCLK((uint16_t)(i2csWriteBuffer[2] | (i2csWriteBuffer[3] << 8U)));
                /*set return buffer.*/
                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], 0U, rtn_state);
                break;

            case CMD_SET_USER_FLASH_CLEAR_ALL:
                rtn_state = UserFlash_ClearAll();
                break;

            case CMD_SPI_WRITE: /*Mode ID, CMD, ch, WData~WData,CHK */
                c_wLen = wLen - 4U;

                if(i2csWriteBuffer[2] == 0U)
                {
                    rtn_state = SPIM_Write(i2csWriteBuffer[2], &i2csWriteBuffer[3], c_wLen);
                }
                else
                {
                    rtn_state = SPIM_Write(i2csWriteBuffer[2], &i2csWriteBuffer[3], c_wLen);
                }

                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], 0U, rtn_state);
                break;

            case CMD_SET_PSOC_SPI_CKPOL_CKPHA:
                rtn_state = SPIM_SetPOL_PHA(i2csWriteBuffer[2], i2csWriteBuffer[3]);
                /*set return buffer.*/
                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], 0U, rtn_state);
                break;

            case CMD_SET_PSOC_SPI_CLOCK:
                rtn_state = SPIM_SetCLK((uint16_t)(i2csWriteBuffer[2] | (i2csWriteBuffer[3] << 8U)));
                /*set return buffer.*/
                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], 0U, rtn_state);
                break;

            case CMD_SET_PWM_PIN: /*Mode ID, 0x75, ch, Pin|Port, CHK */
#if (SW_OR_HW_PWM == SEL_HW_PWM )
                /*HW PWM*/
                TCPWMO_SetShadowPin(i2csWriteBuffer[2], i2csWriteBuffer[3]);
#elif (SW_OR_HW_PWM == SEL_SW_PWM )
                /*SW PWM*/
                SWPWMO_PinInit(i2csWriteBuffer[2], i2csWriteBuffer[3]);
#endif
                break;

            case CMD_SET_PWM_FREQ: /*Mode ID, 0x76, ch, Freq_LB, Freq_HB. , CHK */
#if (SW_OR_HW_PWM == SEL_HW_PWM )
                /*HW PWM*/
                TCPWMO_SetFreq(i2csWriteBuffer[2], (uint16_t)(i2csWriteBuffer[4] << 8 | i2csWriteBuffer[3]));
#elif (SW_OR_HW_PWM == SEL_SW_PWM )
                /*SW PWM*/
                SWPWMO_SetFreq(i2csWriteBuffer[2], (uint16_t)(i2csWriteBuffer[4] << 8 | i2csWriteBuffer[3]));
#endif
                break;

            case CMD_SET_PWM_DUTY: /*Mode ID, 0x77, ch, Duty, CHK */
#if (SW_OR_HW_PWM == SEL_HW_PWM )
                /*HW PWM*/
                TCPWMO_SetDuty(i2csWriteBuffer[2], i2csWriteBuffer[3]);
#elif (SW_OR_HW_PWM == SEL_SW_PWM )
                /*SW PWM*/
                SWPWMO_SetDuty(i2csWriteBuffer[2], i2csWriteBuffer[3]);
#endif
                break;

            case CMD_IIC_WRITE: /*Mode ID, CMD, ch, dev addr, WData~WData,CHK */
                c_wLen = wLen - 5U;

                if(i2csWriteBuffer[2] == 0U)
                {
                    rtn_state = I2CMA_Write(i2csWriteBuffer[3], &i2csWriteBuffer[4], c_wLen);
                }
                else
                {
                    rtn_state = I2CMB_Write(i2csWriteBuffer[3], &i2csWriteBuffer[4], c_wLen);
                }

                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], 0U, rtn_state);
                break;

            case CMD_SPI_2STEP_WRITE:
            case CMD_PSOC_IIC_2STEP_WRITE:
            case CMD_SPI_2STEP_READ:
            case CMD_PSOC_IIC_2STEP_READ:
            case CMD_2STEP_FAKE_DATA_FLASH_WRITE:
            case CMD_2STEP_FAKE_DATA_FLASH_READ:
            case CMD_2STEP_CANFD_WRITE:
            case CMD_2STEP_UART_WRITE:
            case CMD_SET_ENCODER:
            case CMD_GPO_TOGGLE:/*Mode ID, CMD, port|pin, DelayMs(H), DelayMs(L), CHK */
                I2cs2StepRunBuff.enable = 1U;
                I2cs2StepRunBuff.WLen = wLen;

                for(i = 0; i < wLen; i++)
                {
                    I2cs2StepRunBuff.WBuff[i] = i2csWriteBuffer[i];
                }

                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], i2csWriteBuffer[2], 0xA5U);

                /*printf("AppWBufProc_2StepRun: CMD=0x%2X\r\n", i2csWriteBuffer[1]);*/
                rtn_state = FUNC_RETURN_OK;
                break;

            case CMD_CAN_SET_BitRate:
                rtn_state = CanFd_SetBitRate((uint16_t)(i2csWriteBuffer[2] | (i2csWriteBuffer[3] << 8U)),
                                             (uint16_t)(i2csWriteBuffer[4] | (i2csWriteBuffer[5] << 8U)));
                /*set return buffer.*/
                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], 0U, rtn_state);
                break;

            case CMD_SET_UART_BAUD:
                rtn_state = Uart_SetBaudRate((uint32_t)(i2csWriteBuffer[2] | (i2csWriteBuffer[3] << 8U) |
                                                        (i2csWriteBuffer[4] << 16U) | (i2csWriteBuffer[5] << 24U))
                                            );
                /*set return buffer.*/
                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], 0U, rtn_state);
                break;

            case CMD_SWD_ONOFF_CONTROL:
                (void)Wdt_OnOff(i2csWriteBuffer[2]);
                break;

            default:
                rtn_state = Project_I2CSW_Callback(i2csWriteBuffer);
                break;
        }
    }

    return rtn_state;
}

/*******************************************************************************
 *  \brief Function Name: I2cs_AppRBufProc
 *******************************************************************************
 *
 * App mode only.
 * CMD0 I2Cs reading process.
 *
 * \Parameters [in] uint32: master write buffer length
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN I2cs_CMD0_AppRBufProc(uint32_t wLen)
{
    uint16_t get_val;
    uint32_t i;
    uint32_t c_rLen, c_wLen;
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;

    /*For APP*/
    rtn_state = I2cs_WBuffVerifyCHKSUM(wLen);

    /*printf("I2CR: 0=0x%x, 1=0x%x, 2=0x%x, CHK_state =%d\r\n", i2csWriteBuffer[0], i2csWriteBuffer[1], i2csWriteBuffer[2], rtn_state)*/
    if(rtn_state == FUNC_RETURN_OK)
    {
        /*For tooling board CMD.*/
        switch(i2csWriteBuffer[1])
        {
            case CMD_GET_FW_VERSION:
                i2csReadBuffer[0] = (uint8_t)HW_VER;
                i2csReadBuffer[1] = (uint8_t)FW_APP_VER;
                i2csReadBuffer[2] = (uint8_t)BOARD;
                i2csReadBuffer[3] = (uint8_t)FW_APP_MINOR_VER;
                i2csReadBuffer[4] = I2cs_RBuffCalCHKSUM(4);
                break;

            case CMD_GET_USER_RAM_DATA:
                Sys_GetUserRam(i2csReadBuffer, 16U);
                i2csReadBuffer[16U] = I2cs_RBuffCalCHKSUM(16U);
                break;

            case CMD_GET_PSOC_IICA_CLOCK:
                get_val = I2CMA_GetCLK();
                i2csReadBuffer[0] = (uint8_t)get_val;
                i2csReadBuffer[1] = (uint8_t)(get_val >> 8);
                i2csReadBuffer[2] = I2cs_RBuffCalCHKSUM(2);
                break;

            case CMD_GET_LOT3_IO:
                i2csReadBuffer[0] = Gpio_GetPin(((i2csWriteBuffer[2] << 4U) | i2csWriteBuffer[3]));
                i2csReadBuffer[1] = I2cs_RBuffCalCHKSUM(1);
                break;

            case CMD_GET_LOT3_PORT:
                i2csReadBuffer[0] = Gpio_GetPort(i2csWriteBuffer[2]);
                i2csReadBuffer[1] = I2cs_RBuffCalCHKSUM(1);
                break;

            case CMD_GET_ADC_VAL:
                int16_t adc = Adc_GetAdcVal(i2csWriteBuffer[2]);
                i2csReadBuffer[0] = (uint8_t)adc;
                i2csReadBuffer[1] = (uint8_t)(adc >> 8U);
                i2csReadBuffer[2] = I2cs_RBuffCalCHKSUM(2);
                break;

            case CMD_GET_BUTTON:  /*0x17U*/
                //i2csReadBuffer[0] = Gpio_GetPort(4U);
                i2csReadBuffer[0] = Button_GetBtnState();
                i2csReadBuffer[1] = Button_GetBtnHoldState();
                i2csReadBuffer[2] = I2cs_RBuffCalCHKSUM(2);
                break;

            case CMD_GET_DIP_SWITCH: /*0x18U*/
                i2csReadBuffer[0] = ~Gpio_GetPort(7U);
                i2csReadBuffer[1] = I2cs_RBuffCalCHKSUM(1);
                break;
#ifdef EN_DEV_INA236

            case CMD_GET_MODULE_CURRENT: /*0x19U*/
                t_MODULE_POWER module_power = INA236_GetCurrent(i2csWriteBuffer[2]);
                i2csReadBuffer[0] = (uint8_t)module_power.processedCurrent;
                i2csReadBuffer[1] = (uint8_t)(module_power.processedCurrent >> 8U);
                i2csReadBuffer[2] = (uint8_t)module_power.processedVoltage;
                i2csReadBuffer[3] = (uint8_t)(module_power.processedVoltage >> 8U);
                i2csReadBuffer[4] = I2cs_RBuffCalCHKSUM(4);
                break;
#endif

            case CMD_GET_USER_FLASH_PARAMETER:
                get_val = UserFlash_GetPackageSize();
                i2csReadBuffer[0] = (uint8_t)get_val;
                i2csReadBuffer[1] = (uint8_t)(get_val >> 8U);
                get_val = (uint16_t)(UFLASH_TOTAL_SIZE / get_val);
                i2csReadBuffer[2] = (uint8_t)get_val;
                i2csReadBuffer[3] = (uint8_t)(get_val >> 8U);
                i2csReadBuffer[4] = (uint8_t)UFLASH_TOTAL_SIZE;
                i2csReadBuffer[5] = (uint8_t)(UFLASH_TOTAL_SIZE >> 8U);
                i2csReadBuffer[6] = I2cs_RBuffCalCHKSUM(6);
                break;

            case CMD_GET_USER_FLASH_NEWEST:
                get_val = UserFlash_GetPackageSize();
                (void)UserFlash_GetUserData(i2csReadBuffer, get_val);
                i2csReadBuffer[get_val] = I2cs_RBuffCalCHKSUM(get_val);
                break;

            case CMD_GET_USER_FLASH_BY_ADDR:
                i =  i2csWriteBuffer[2];
                i |= (uint32_t)(i2csWriteBuffer[3] << 8);
                i |= (uint32_t)(i2csWriteBuffer[4] << 16);
                i |= (uint32_t)(i2csWriteBuffer[5] << 24); /*Start address*/
                get_val = (uint16_t)(i2csWriteBuffer[7] << 8) | i2csWriteBuffer[6];
                (void)UserFlash_GetUserDataByAddr(i2csReadBuffer, i, get_val);
                i2csReadBuffer[get_val] = I2cs_RBuffCalCHKSUM(get_val);
                break;

            case CMD_GET_SP_PWM_DUTY:
                t_PWM_VALUE t_pwm = PWMCap_GetPWMVal(i2csWriteBuffer[2]);
                i2csReadBuffer[0] = (uint8_t)(t_pwm.Frequency);  /*Freq*/
                i2csReadBuffer[1] = (t_pwm.Duty);                /*Duty*/
                i2csReadBuffer[2] = I2cs_RBuffCalCHKSUM(2);
                break;

            case CMD_SPI_READ: /*Mode ID, CMD, ch,  R_Len(LByte),  R_Len(HByte), WData~WData,CHK */

                c_wLen = wLen - 6U;
                c_rLen = i2csWriteBuffer[3] | (i2csWriteBuffer[4] << 8);
                rtn_state = SPIM_WriteRead(i2csWriteBuffer[2], &i2csWriteBuffer[5], c_wLen, &i2csReadBuffer[0], c_rLen);
                i2csReadBuffer[c_rLen] = I2cs_RBuffCalCHKSUM(c_rLen);
                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], 0U, rtn_state);
                break;

            case CMD_GET_PSOC_SPI_CLOCK:
                get_val = SPIM_GetCLK();
                i2csReadBuffer[0] = (uint8_t)get_val;
                i2csReadBuffer[1] = (uint8_t)(get_val >> 8);
                i2csReadBuffer[2] = I2cs_RBuffCalCHKSUM(2);
                break;

            case CMD_IIC_READ: /*Mode ID, CMD, ch, RLB, RHB, dev addr, WData~WData,CHK */
                c_wLen = wLen - 7U;
                c_rLen = i2csWriteBuffer[3] | (i2csWriteBuffer[4] << 8);

                if(i2csWriteBuffer[2] == 0U)
                {
                    rtn_state = I2CMA_Read(i2csWriteBuffer[5], &i2csWriteBuffer[6], c_wLen, &i2csReadBuffer[0], c_rLen);
                }
                else
                {
                    rtn_state = I2CMB_Read(i2csWriteBuffer[5], &i2csWriteBuffer[6], c_wLen, &i2csReadBuffer[0], c_rLen);
                }

                i2csReadBuffer[c_rLen] = I2cs_RBuffCalCHKSUM(c_rLen);
                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], 0U, rtn_state);
                break;

            case CMD_GET_PSOCID:
                (void)Sys_GetPsocSiliconID(&i2csReadBuffer[0]);
                i2csReadBuffer[5] = I2cs_RBuffCalCHKSUM(5U);
                break;

            case CMD_2STEP_CANFD_READ:
                i2csReadBuffer[0] = CanFd_Get_Fifo_DataLen();
                i2csReadBuffer[1] = I2cs_RBuffCalCHKSUM(1);

                /*copy data to 2step buffer*/
                if(i2csReadBuffer[0] != 0xFFU)
                {
                    I2cs2StepRunBuff.enable = 1U;
                    I2cs2StepRunBuff.WLen = wLen;

                    for(i = 0; i < wLen; i++)
                    {
                        I2cs2StepRunBuff.WBuff[i] = i2csWriteBuffer[i];
                    }
                }

                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], i2csWriteBuffer[2], 0xA5U);

                break;

            case CMD_2STEP_UART_READ:
                i2csReadBuffer[0] = Uart_Get_Fifo_DataLen();
                i2csReadBuffer[1] = I2cs_RBuffCalCHKSUM(1);

                /*copy data to 2step buffer*/
                if(i2csReadBuffer[0] != 0xFFU)
                {
                    I2cs2StepRunBuff.enable = 1U;
                    I2cs2StepRunBuff.WLen = wLen;

                    for(i = 0; i < wLen; i++)
                    {
                        I2cs2StepRunBuff.WBuff[i] = i2csWriteBuffer[i];
                    }
                }

                (void)I2cs_SetRTNState(i2csWriteBuffer[0], i2csWriteBuffer[1], i2csWriteBuffer[2], 0xA5U);

                break;

            case CMD_2STEP_READ_RETURN_DATA: /*Mode ID, CMD, MagicNo, ack/nack, len_LB(N), len_HB(N), rtnData1~rtnDataN,CHK */
                i2csReadBuffer[3] = 0xA5U;

                if(ReqCmdRtnBuff.AckNack != 0xA5U)
                {
                    i2csReadBuffer[0] = ReqCmdRtnBuff.RtnMode;
                    i2csReadBuffer[1] = ReqCmdRtnBuff.RtnID;
                    i2csReadBuffer[2] = ReqCmdRtnBuff.MagicNo;
                    i2csReadBuffer[4] = (uint8_t)ReqCmdRtnBuff.RtnLen;
                    i2csReadBuffer[5] = (uint8_t)(ReqCmdRtnBuff.RtnLen >> 8);

                    for(i = 0; i < ReqCmdRtnBuff.RtnLen; i++)
                    {
                        i2csReadBuffer[6 + i] = ReqCmdRtnBuff.RtnBuff[i];
                    }

                    i2csReadBuffer[3] = ReqCmdRtnBuff.AckNack;
                    i2csReadBuffer[ReqCmdRtnBuff.RtnLen + 6] = I2cs_RBuffCalCHKSUM(ReqCmdRtnBuff.RtnLen + 6);
                }

                break;

            case CMD_GET_RETURN_ACK_NACK: /*Mode ID, CMD, MagicNo, AckNackBusy,CHK */
                i2csReadBuffer[3] = 0xA5U;

                if(ReqCmdRtnBuff.AckNack != 0xA5U)
                {
                    i2csReadBuffer[0] = ReqCmdRtnBuff.RtnMode;
                    i2csReadBuffer[1] = ReqCmdRtnBuff.RtnID;
                    i2csReadBuffer[2] = ReqCmdRtnBuff.MagicNo;
                    i2csReadBuffer[3] = ReqCmdRtnBuff.AckNack;
                    i2csReadBuffer[4] = I2cs_RBuffCalCHKSUM(4);
                }

                break;

            default:
                rtn_state = Project_I2CSR_Callback(i2csWriteBuffer);
                break;
        }
    }

    return rtn_state;
}

/*******************************************************************************
 *  \brief Function Name: I2cs_WBuffVerifyCHKSUM
 *******************************************************************************
 *
 * Set read buffer process for I2CS
 *
 *
 * \Parameters [in] uint32: master write buffer length include checksum.
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN I2cs_WBuffVerifyCHKSUM(uint32_t wLen)
{
    uint8_t xor = 0U;
    uint32_t i;

    for(i = 0U; i < (wLen - 1); i++)
    {
        xor = xor ^ i2csWriteBuffer[i];
    }

    xor = xor + 1U;

    if(xor == i2csWriteBuffer[wLen - 1])
    {
        return FUNC_RETURN_OK;
    }
    else
    {
        return FUNC_RETURN_NG;
    }
}

t_FUNC_RETURN I2cs_UpdateReadBuff(uint8_t updateBuff[], uint8_t len)
{
    t_FUNC_RETURN rtnState;

    if(len > 0U)
    {
        rtnState = FUNC_RETURN_OK;

        for(int i = 0; i < len; i++)
        {
            i2csReadBuffer[i] = updateBuff[i];
        }

        i2csReadBuffer[len] = I2cs_RBuffCalCHKSUM(len);
    }
    else
    {
        rtnState = FUNC_RETURN_NG;
    }

    return rtnState;
}

/*******************************************************************************
 *  \brief Function Name: I2cs_RBuffCalCHKSUM
 *******************************************************************************
 *
 * Set read buffer process for I2CS
 *
 *
 * \Parameters [in] rLen: master read buffer length.
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static uint8_t I2cs_RBuffCalCHKSUM(uint32_t rLen)
{
    uint8_t xor = 0U;
    uint32_t i;

    for(i = 0U; i < (rLen); i++)
    {
        xor = xor ^ i2csReadBuffer[i];
    }

    xor = xor + 1U;

    return xor;
}

t_FUNC_RETURN I2cs_SetRTNState(uint8_t mode, uint8_t id, uint8_t magic_no, uint8_t state)
{
    ReqCmdRtnBuff.RtnMode = mode;
    ReqCmdRtnBuff.RtnID = id;
    ReqCmdRtnBuff.MagicNo = magic_no;
    ReqCmdRtnBuff.AckNack = state;

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN I2cs_SetRtnData(struct StrRtnData RtnStrBuff)
{
    //ReqCmdRtnBuff = RtnStrBuff;
    /*printf("I2cs_SetRtnData: RtnID=0x%2X\r\n", ReqCmdRtnBuff.RtnID);*/

    ReqCmdRtnBuff.RtnMode = RtnStrBuff.RtnMode; /*0*/
    ReqCmdRtnBuff.RtnID = RtnStrBuff.RtnID; /*ID*/
    ReqCmdRtnBuff.MagicNo = RtnStrBuff.MagicNo;/*MagicNo*/
    ReqCmdRtnBuff.RtnLen = RtnStrBuff.RtnLen;

    for(int i = 0; i < RtnStrBuff.RtnLen; i++)
    {
        ReqCmdRtnBuff.RtnBuff[i] = RtnStrBuff.RtnBuff[i];
    }

    ReqCmdRtnBuff.AckNack = RtnStrBuff.AckNack;/*A!*/

    return FUNC_RETURN_OK;
}

#endif

static t_FUNC_RETURN I2cs_CheckAppEnterBoot(uint8_t Key0, uint8_t Key1)
{
    uint8_t TimeOut_flag;
    t_FUNC_RETURN rtn_state;

    switch(UpdataAPP_Stage)
    {
        case 0U:
            if((Key0 == 0xAAU) && (Key1 == 0x55U))
            {
                UpdataAPP_Stage = 1U;
                rtn_state = FUNC_RETURN_OK;
                SWTimer_Reset(0U, SWTimer_OneShort, 100U); /*100ms*/
            }
            else
            {
                UpdataAPP_Stage = 0U;
                rtn_state = FUNC_RETURN_NG;
            }

            break;

        case 1U:
            TimeOut_flag = SWTimer_GetTimeoutState(0);

            if((TimeOut_flag == 0U) && (Key0 == 0xccU) && (Key1 == 0x33U))
            {
                UpdataAPP_Stage = 2U;
                rtn_state = FUNC_RETURN_OK;
            }
            else
            {
                UpdataAPP_Stage = 0U;
                rtn_state = FUNC_RETURN_NG;
            }

            break;

        case 2U:
            TimeOut_flag = SWTimer_GetTimeoutState(0);

            if((TimeOut_flag == 0U) && ((Key0 != 0U) || (Key1 != 0U)))
            {

                Sys_SetSessionKey(((uint16_t)Key1 << 8U) | Key0);
                UpdataAPP_Stage = 0U;
                rtn_state = FUNC_RETURN_OK;
            }
            else
            {
                UpdataAPP_Stage = 0U;
                rtn_state = FUNC_RETURN_NG;
            }


            break;

        default:
            UpdataAPP_Stage = 0U;
            rtn_state = FUNC_RETURN_NG;
            break;
    }

    return rtn_state;
}


/* ==[ Definition of functions with external linkage ]====================== */

/*******************************************************************************
 * \brief Function Name: sI2C_InterruptHandler
 *******************************************************************************
 * Summary:
 *  I2CS init. Device address(7bits) =0x0A define in the 'device config'.
 *
 *
 * \Parameters [in] NULL
 * \return                t_FUNC_RETURN
 ******************************************************************************/

t_FUNC_RETURN I2cs_Init()
{
    cy_en_scb_i2c_status_t result;
    cy_en_sysint_status_t  sysint_status;

    /*Step1:*/
    /* Initialize and enable I2C Component in slave mode.
     * If initialization fails process error
     */
    result = Cy_SCB_I2C_Init(mI2CS_HW, &mI2CS_config,
                             &mI2CS_context);

    if(result != CY_SCB_I2C_SUCCESS)
    {
        /*CY_ASSERT(0)*/
        return FUNC_RETURN_NG;
    }

    /*I2CS interrupt define*/
    sysint_status = Cy_SysInt_Init(&mI2CS_IRQ_config, &I2CS_InterruptHandler);

    if(sysint_status != CY_SYSINT_SUCCESS)
    {
        /*CY_ASSERT(0)*/
        return FUNC_RETURN_NG;
    }

    /* Configure read buffer */
    Cy_SCB_I2C_SlaveConfigReadBuf(mI2CS_HW, i2csReadBuffer,
                                  SL_RD_BUFFER_SIZE, &mI2CS_context);

    /* Configure write buffer */
    Cy_SCB_I2C_SlaveConfigWriteBuf(mI2CS_HW, i2csWriteBuffer,
                                   SL_WR_BUFFER_SIZE, &mI2CS_context);

    /*Step2:*/
    /* Register Callback function for interrupt */
    Cy_SCB_I2C_RegisterEventCallback(mI2CS_HW, \
                                     (cy_cb_scb_i2c_handle_events_t)I2cs_EvenCallback, \
                                     &mI2CS_context);

    /*Step3:*/
    /*  Enable interrupt and I2C block */
    NVIC_EnableIRQ((IRQn_Type)mI2CS_IRQ_config.intrSrc);
    Cy_SCB_I2C_Enable(mI2CS_HW, &mI2CS_context);

    return FUNC_RETURN_OK;
}

/*******************************************************************************
 * \brief Function Name: sI2C_InterruptHandler
 *******************************************************************************
 * Summary:
 *  This function executes interrupt service routine.
 *
 ******************************************************************************/
void I2CS_InterruptHandler(void)
{
    /* ISR implementation for I2C */
    Cy_SCB_I2C_SlaveInterrupt(mI2CS_HW, &mI2CS_context);
}

/*******************************************************************************
 * \brief Function Name: sI2C_InterruptHandler
 *******************************************************************************
 * Summary:
 *  This function executes interrupt service routine.
 *
 ******************************************************************************/
#if (LOT3_BOOT_ENABLE==0U)
t_FUNC_RETURN I2cs_2StepRunProcess(void)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
    struct StrRtnData RtnStrBuff;
    uint32_t c_wLen, c_rLen;
    uint32_t SAddr;
    uint8_t rwPageIdx;
    uint8_t *p_real_addr;
    CanFdMsg canfdMsg;

    if(0U == I2cs2StepRunBuff.enable)
    {
        return FUNC_RETURN_OK;
    }
    else
    {
        I2cs2StepRunBuff.enable = 0U;

        switch(I2cs2StepRunBuff.WBuff[1])
        {
            case CMD_SPI_2STEP_WRITE: /*Mode ID, CMD, MagicNo, ch, WData~WData,CHK */
                c_wLen = I2cs2StepRunBuff.WLen - 5U;
                rtn_state = SPIM_Write(I2cs2StepRunBuff.WBuff[3], &I2cs2StepRunBuff.WBuff[4], c_wLen);
                /*set return buffer.*/
                (void)I2cs_SetRTNState(I2cs2StepRunBuff.WBuff[0], I2cs2StepRunBuff.WBuff[1], I2cs2StepRunBuff.WBuff[2], rtn_state);
                break;

            case CMD_PSOC_IIC_2STEP_WRITE: /*Mode ID, CMD, MagicNo, ch, dev addr, WData~WData,CHK */
                c_wLen = I2cs2StepRunBuff.WLen - 6U;

                if(I2cs2StepRunBuff.WBuff[3] == 0U)
                {
                    rtn_state = I2CMA_Write(I2cs2StepRunBuff.WBuff[4], &I2cs2StepRunBuff.WBuff[5], c_wLen);
                }
                else
                {
                    rtn_state = I2CMB_Write(I2cs2StepRunBuff.WBuff[4], &I2cs2StepRunBuff.WBuff[5], c_wLen);
                }

                /*set return buffer.*/
                (void)I2cs_SetRTNState(I2cs2StepRunBuff.WBuff[0], I2cs2StepRunBuff.WBuff[1], I2cs2StepRunBuff.WBuff[2], rtn_state);
                break;

            case CMD_SPI_2STEP_READ: /*Mode ID, CMD, MagicNo, ch, RLB, RHB, WData~WData,CHK */
                c_wLen = I2cs2StepRunBuff.WLen - 7U;
                c_rLen = I2cs2StepRunBuff.WBuff[4] | (I2cs2StepRunBuff.WBuff[5] << 8);
                rtn_state = SPIM_WriteRead(I2cs2StepRunBuff.WBuff[3], &I2cs2StepRunBuff.WBuff[6], c_wLen, &RtnStrBuff.RtnBuff[0], c_rLen);

                /*set return buffer.*/
                RtnStrBuff.RtnMode = I2cs2StepRunBuff.WBuff[0];
                RtnStrBuff.RtnID = I2cs2StepRunBuff.WBuff[1];
                RtnStrBuff.MagicNo = I2cs2StepRunBuff.WBuff[2];
                RtnStrBuff.AckNack = (uint8_t)rtn_state;
                RtnStrBuff.RtnLen = (uint8_t)(c_rLen);

                (void)I2cs_SetRtnData(RtnStrBuff);
                break;

            case CMD_PSOC_IIC_2STEP_READ: /*Mode ID, CMD, MagicNo, ch, RLB, RHB, dev addr, WData~WData,CHK */
                c_wLen = I2cs2StepRunBuff.WLen - 8U;
                c_rLen = I2cs2StepRunBuff.WBuff[4] | (I2cs2StepRunBuff.WBuff[5] << 8);

                if(I2cs2StepRunBuff.WBuff[3] == 0U)
                {
                    rtn_state = I2CMA_Read(I2cs2StepRunBuff.WBuff[6], &I2cs2StepRunBuff.WBuff[7], c_wLen, &RtnStrBuff.RtnBuff[0], c_rLen);
                }
                else
                {
                    rtn_state = I2CMB_Read(I2cs2StepRunBuff.WBuff[6], &I2cs2StepRunBuff.WBuff[7], c_wLen, &RtnStrBuff.RtnBuff[0], c_rLen);
                }

                /*set return buffer.*/
                RtnStrBuff.RtnMode = I2cs2StepRunBuff.WBuff[0];
                RtnStrBuff.RtnID = I2cs2StepRunBuff.WBuff[1];
                RtnStrBuff.MagicNo = I2cs2StepRunBuff.WBuff[2];
                RtnStrBuff.AckNack = (uint8_t)rtn_state;
                RtnStrBuff.RtnLen = (uint8_t)(c_rLen);

                (void)I2cs_SetRtnData(RtnStrBuff);
                break;

            case CMD_2STEP_FAKE_DATA_FLASH_WRITE:
                /*0x00,0xC0,MagicNo,ADDR(LB),ADDR(MLB),ADDR(MHB),ADDR(HB),WData1~WDataN,CHKSUM*/
                SAddr = (uint32_t)(I2cs2StepRunBuff.WBuff[6] << 24 | I2cs2StepRunBuff.WBuff[5] << 16 | I2cs2StepRunBuff.WBuff[4] << 8 | I2cs2StepRunBuff.WBuff[3]);
                c_wLen = I2cs2StepRunBuff.WLen - 8U;

                /*Step1: save page data in the fakePageBuff.*/
                if(CurrFakeFlashPageAddr != (SAddr & 0xFFFFFF00) && (c_wLen < 256U))
                {
                    p_real_addr = (uint8_t *)(CurrFakeFlashPageAddr);

                    for(int i = 0; i < 256; i++)
                    {
                        fakePageBuff[i] = *(p_real_addr + i);
                    }
                }

                /*Step2: write flash*/
                rtn_state = FUNC_RETURN_NG;
                rwPageIdx = (uint8_t)(SAddr);
                CurrFakeFlashPageAddr = (SAddr & 0xFFFFFF00); /*Don't move*/

                if((rwPageIdx + c_wLen) <= 256U)
                {
                    for(uint32_t i = 0; i < c_wLen; i++)
                    {
                        fakePageBuff[rwPageIdx + i] = I2cs2StepRunBuff.WBuff[i + 7];
                    }

                    rtn_state = Flash_WriteRow(CurrFakeFlashPageAddr, fakePageBuff);
#if 0
                    printf("SAddr=0x%8lX, c_wLen=0x%8lX\r\n", SAddr, c_wLen);
                    printf("W[250]=0x%2X, [251]=0x%2X, [252]=0x%2X, [253]=0x%2X\r\n", fakePageBuff[250], fakePageBuff[251], fakePageBuff[252], fakePageBuff[253]);
                    printf("rtn_state=0x%2X\r\n", rtn_state);
#endif
                }

                /*set return buffer.*/
                (void)I2cs_SetRTNState(I2cs2StepRunBuff.WBuff[0], I2cs2StepRunBuff.WBuff[1], I2cs2StepRunBuff.WBuff[2], rtn_state);
                break;

            case CMD_2STEP_FAKE_DATA_FLASH_READ:
                /*0xF0,0xC1,MagicNo,ADDR(LB),ADDR(MLB),ADDR(MHB),ADDR(HB),Rlen_N(LB),Rlen_N(HB),CHKSUM*/
                SAddr = (uint32_t)(I2cs2StepRunBuff.WBuff[6] << 24 | I2cs2StepRunBuff.WBuff[5] << 16 | I2cs2StepRunBuff.WBuff[4] << 8 | I2cs2StepRunBuff.WBuff[3]);
                c_rLen = (uint32_t)(I2cs2StepRunBuff.WBuff[8] << 8 | I2cs2StepRunBuff.WBuff[7]);

                /*set return buffer.*/
                RtnStrBuff.RtnMode = I2cs2StepRunBuff.WBuff[0];
                RtnStrBuff.RtnID = I2cs2StepRunBuff.WBuff[1];
                RtnStrBuff.MagicNo = I2cs2StepRunBuff.WBuff[2];
                RtnStrBuff.RtnLen = (uint16_t)(c_rLen);

                if(c_rLen <= 256)
                {
                    RtnStrBuff.AckNack = FUNC_RETURN_OK;
                    p_real_addr = (uint8_t *)(SAddr);

                    for(uint32_t i = 0; i < c_rLen; i++)
                    {
                        RtnStrBuff.RtnBuff[i] = *(p_real_addr + i);
                    }
                }
                else
                {
                    RtnStrBuff.AckNack = FUNC_RETURN_NG;
                }

                (void)I2cs_SetRtnData(RtnStrBuff);
                break;

            case CMD_2STEP_CANFD_WRITE:
                /*0x00,0xC2,MagicNo, CanFD Format,ID(LB),ID(MLB),ID(MHB),ID(HB),WData1~WDataN,CHKSUM*/
                c_wLen = I2cs2StepRunBuff.WLen - 9U;
                /*cal ID*/
                SAddr = (uint32_t)(I2cs2StepRunBuff.WBuff[4] + (I2cs2StepRunBuff.WBuff[5] << 8) + \
                                   (I2cs2StepRunBuff.WBuff[6] << 16) + (I2cs2StepRunBuff.WBuff[7] << 24));
                rtn_state = CanFd_TX(I2cs2StepRunBuff.WBuff[3], SAddr, &I2cs2StepRunBuff.WBuff[8], c_wLen);

                /*set return buffer.*/
                (void)I2cs_SetRTNState(I2cs2StepRunBuff.WBuff[0], I2cs2StepRunBuff.WBuff[1], I2cs2StepRunBuff.WBuff[2], rtn_state);

                break;

            case CMD_2STEP_CANFD_READ:
                /*CMD MODE,CMD ID,MagicNo,ACK(0)/NACK(1)/BUSY(0xA5),RTN LEN(N)_LB,RTN LEN(N)_HB,Data1~DataN,CHKSUM*/
                canfdMsg.length = 0U;
                canfdMsg.id = 0U;
                rtn_state = CanFd_RX(&canfdMsg);
#if 0
                printf("Can[]=%x %x %x %x %x %x %x %x\r\n",
                       canfdMsg.data[0], canfdMsg.data[1], canfdMsg.data[2], canfdMsg.data[3], canfdMsg.data[4], canfdMsg.data[5], canfdMsg.data[6], canfdMsg.data[7]
                      );
#endif
                /*set return buffer.*/
                ReqCmdRtnBuff.RtnMode = I2cs2StepRunBuff.WBuff[0];
                ReqCmdRtnBuff.RtnID = I2cs2StepRunBuff.WBuff[1];
                ReqCmdRtnBuff.MagicNo = I2cs2StepRunBuff.WBuff[2];
                ReqCmdRtnBuff.RtnLen = (uint8_t)(canfdMsg.length + 4U); /*4bytes ID+DataLen*/
                ReqCmdRtnBuff.RtnBuff[0] = (uint8_t)canfdMsg.id;
                ReqCmdRtnBuff.RtnBuff[1] = (uint8_t)(canfdMsg.id >> 8);
                ReqCmdRtnBuff.RtnBuff[2] = (uint8_t)(canfdMsg.id >> 16);
                ReqCmdRtnBuff.RtnBuff[3] = (uint8_t)(canfdMsg.id >> 24);

                for(int i = 0; i < canfdMsg.length; i++)
                {
                    ReqCmdRtnBuff.RtnBuff[i + 4] = canfdMsg.data[i]; //  RtnStrBuff.RtnBuff[i];
                }

                ReqCmdRtnBuff.AckNack = (uint8_t)rtn_state;

                break;

            case CMD_2STEP_UART_WRITE:

                /*0x00,0xC7,MagicNo, WData1~WDataN,CHKSUM*/
                c_wLen = I2cs2StepRunBuff.WLen - 4U;
                /*cal ID*/
                rtn_state = Uart_PutArray(&I2cs2StepRunBuff.WBuff[3], c_wLen);

                /*set return buffer.*/
                (void)I2cs_SetRTNState(I2cs2StepRunBuff.WBuff[0], I2cs2StepRunBuff.WBuff[1], I2cs2StepRunBuff.WBuff[2], rtn_state);

                break;

            case CMD_2STEP_UART_READ:
                /*CMD MODE,CMD ID,MagicNo,ACK(0)/NACK(1)/BUSY(0xA5),RTN LEN(N)_LB,RTN LEN(N)_HB,Data1~DataN,CHKSUM*/
                rtn_state = Uart_GetArray(ReqCmdRtnBuff.RtnBuff, &ReqCmdRtnBuff.RtnLen);

                /*set return buffer.*/
                ReqCmdRtnBuff.RtnMode = I2cs2StepRunBuff.WBuff[0];
                ReqCmdRtnBuff.RtnID = I2cs2StepRunBuff.WBuff[1];
                ReqCmdRtnBuff.MagicNo = I2cs2StepRunBuff.WBuff[2];
                ReqCmdRtnBuff.AckNack = (uint8_t)rtn_state;

                break;

            case CMD_GPO_TOGGLE:/*Mode ID, CMD, port|pin, DelayMs(H), DelayMs(L), CHK */
                Gpio_Inv(I2cs2StepRunBuff.WBuff[2]);
                Cy_SysLib_Delay((uint32_t)(I2cs2StepRunBuff.WBuff[3] << 8 | I2cs2StepRunBuff.WBuff[4]));
                Gpio_Inv(I2cs2StepRunBuff.WBuff[2]);
                /*set return buffer.*/
                (void)I2cs_SetRTNState(I2cs2StepRunBuff.WBuff[0], I2cs2StepRunBuff.WBuff[1], 0U, FUNC_RETURN_OK);
                break;

            case CMD_SET_ENCODER:
                rtn_state = Encoder_Gen2X(I2cs2StepRunBuff.WBuff[2], I2cs2StepRunBuff.WBuff[3], \
                                          I2cs2StepRunBuff.WBuff[4], I2cs2StepRunBuff.WBuff[5]);
                /*set return buffer.*/
                (void)I2cs_SetRTNState(I2cs2StepRunBuff.WBuff[0], I2cs2StepRunBuff.WBuff[1], 0U, rtn_state);
                break;

            default:
                rtn_state = FUNC_RETURN_NG;
                /*rtn_state = Project_I2CS_2StepRun_Callback(I2cs2StepRunBuff.WBuff, I2cs2StepRunBuff.WLen);*/
                break;
        }

        return rtn_state;
    }
}
#endif


