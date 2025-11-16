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
#include "BootCode.h"
#include "system.h"
#include "uart.h"
#include "crc.h"
#include "flash.h"
#include "gpio.h"
#include "i2cm.h"
#include "SoftwareTimer.h"
#include "i2cs.h"

#if (LOT3_BOOT_ENABLE==1U)
/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static uint8_t PreCmd;
static uint8_t Update_flgBootkey;
static uint16_t Update_stCmd;
static uint16_t sessionKeyUser;
static uint32_t Expect_Address;
static uint8_t WPageBuff[256];

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static t_FUNC_RETURN Boot_CheckBootMode(void);
static t_FUNC_RETURN Boot_CheckCrcAndStartApp(void);
static t_FUNC_RETURN Boot_StartApplication(void);
static t_FUNC_RETURN Boot_CheckCrc(void);
static void Boot_BoardPower(uint8_t OnOff);

/* ==[ Definition of functions with internal linkage ]====================== */

/*
 *
 *
 * CY_SYSLIB_RESET_HWWDT 0x00001 (bit0)
 * CY_SYSLIB_PROT_FAULT     0x00008 (bit3)
 * CY_SYSLIB_RESET_SOFT     0x00010 (bit4)
 *
 *
 */
static t_FUNC_RETURN Boot_CheckBootMode(void)
{
    uint32_t RCAUSE_RESET;

    /* Get Reset cause */
    RCAUSE_RESET = Cy_SysLib_GetResetReason();

    if((RCAUSE_RESET & CY_SYSLIB_RESET_SOFT) == CY_SYSLIB_RESET_SOFT)
    {
        Sys_Uart_Print((char *)"Reset_On\r\n", 10U);
        /*soft reset*/
        sessionKeyUser = Sys_GetSessionKey();
        (void)Sys_SetSessionKey(0U);

        /*reset*/
        if(sessionKeyUser == 0U)
        {
            Boot_CheckCrcAndStartApp();
        }
        else
        {
            Sys_Uart_Print((char *)"IN BOOT MODE\r\n", 14U);
        }

        /*entery boot code*/
    }
    else
    {
        /*power on */
        Sys_Uart_Print((char *)"Power_On\r\n", 10U);
        Boot_CheckCrcAndStartApp();
    }

    return FUNC_RETURN_NG;
}

static t_FUNC_RETURN Boot_CheckCrcAndStartApp(void)
{
    t_FUNC_RETURN rtn_state;
    uint8_t count = 0U;
    rtn_state = Boot_CheckCrc();

    if(rtn_state == FUNC_RETURN_OK)  /*check CRC ok.*/
    {
        /*Sys_Uart_Print((char *)"Crc OK\r\n", 8U)*/
        /*(void)Sys_SetSessionKey(0U)*/ /*clear session key*/

        /* Disable global interrupts */
        __disable_irq();

        /* Check all interrupts disabled*/
        while(((__get_PRIMASK() & 0x01U) == 0U) && (count <= 10U))
        {
            count++;
        }

        Boot_StartApplication();
    }
    else
    {
        Sys_Uart_Print((char *)"APP Crc NG\r\n", 12U);
    }

    return rtn_state;
}

static t_FUNC_RETURN Boot_StartApplication(void)
{
    /* Pointer to the Application Section */
    void (*application_code_entry)(void);

    /* Rebase the Stack Pointer */
    __set_MSP(*(uint32_t *)APP_START_ADDRESS);

    /* Rebase the vector table base address */
    SCB->VTOR = (uint32_t)(APP_START_ADDRESS & 0xff000000U);

    /* Load the Reset Handler address of the application */
    application_code_entry = (void (*)(void))(uint32_t *)(*(uint32_t *)(APP_START_ADDRESS + 4U));

    /* Jump to user Reset Handler in the application */
    application_code_entry();

    return FUNC_RETURN_OK;
}


static t_FUNC_RETURN Boot_CheckCrc(void)
{
    t_FUNC_RETURN rtu_state;
    uint32_t CRC_32_TEMP;
    uint32_t APP_CRC;

    rtu_state = Crc_CalCrc32((uint8_t *)APP_START_ADDRESS, ((uint32_t)APP_SIZE - 4U), &CRC_32_TEMP);

    APP_CRC = *((const uint32_t *)APP_CRC_ADDRESS);

    if(APP_CRC == CRC_32_TEMP)
    {
        rtu_state = FUNC_RETURN_OK;
    }
    else
    {
#if 0
        char str[10];
        Sys_Uart_Print((char *)"APP_CRC = 0x", 12U);
        sprintf(str, "%08X", APP_CRC);
        Sys_Uart_Print(str, 8U);
        Sys_Uart_Print((char *)"\r\n", 2U);
        Sys_Uart_Print((char *)"CAL_CRC = 0x", 12U);
        sprintf(str, "%08X", CRC_32_TEMP);
        Sys_Uart_Print(str, 8U);
        Sys_Uart_Print((char *)"\r\n", 2U);
#endif
        rtu_state = FUNC_RETURN_NG;
    }

    return rtu_state;
}

static void Boot_BoardPower(uint8_t OnOff)
{
    if(OnOff > 0U)
    {
        /*Power On*/
        Gpio_WritePin(EN_5V0, 1U);
        Cy_SysLib_Delay(10U);
        Gpio_WritePin(EN_3V3, 1U);
        Cy_SysLib_Delay(10U);
        Gpio_WritePin(EN_1V15, 1U);
        Cy_SysLib_Delay(10U);
        Gpio_WritePin(EN_1V8, 1U);
        Cy_SysLib_Delay(10U);
        Gpio_WritePin(PDB_MCU, 1U);
    }    
    else
    {
        /*Power Off*/
        Gpio_WritePin(PDB_MCU, 0U);
        Cy_SysLib_Delay(10);
        Gpio_WritePin(EN_1V8, 0U);
        Cy_SysLib_Delay(10);
        Gpio_WritePin(EN_1V15, 0U);
        Cy_SysLib_Delay(10);
        Gpio_WritePin(EN_3V3, 0U);
        Cy_SysLib_Delay(10);
        Gpio_WritePin(EN_5V0, 0U);
    }
}

/* ==[ Definition of functions with external linkage ]======================= */
t_FUNC_RETURN Boot_Init(void)
{    
    /*(void)Sys_SetBootFWVer((uint16_t)((FW_BOOT_VER << 8U) | FW_BOOT_MINOR_VER))*/
    PreCmd = CMD_BOOT_IDLE;
    Update_flgBootkey = 0U;
    Update_stCmd = STATE_IN_BOOT_MODE;

    (void)Boot_CheckBootMode();
    Boot_BoardPower(1U);
    
    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Project_Gpio_Init(void)
{
#if (BOARD == BOARD_PSOC4)
        /*input define*/
        Gpio_PinConfig(CON1_Detetion, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(CON2_Detetion, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(Lock_MCU, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(IRQ_MCU, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(INTB_TP, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(MCU_BUTTON_1, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(MCU_BUTTON_2, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH1, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH2, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH3, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH4, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH5, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH6, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH7, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(DIP_SWITCH8, CY_GPIO_DM_HIGHZ, 0);
        Gpio_PinConfig(WAKE_CSM, CY_GPIO_DM_HIGHZ, 0);
    
        /*output define*/
        Gpio_PinConfig(PDB_MCU, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(Panel_Vin_ON_Norm, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(Panel_Vin_ON_Quie, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(LED_Crest_ON, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(Second_Panel_ON, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(STBYB, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(FPD_BD_I2C_Switch, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EN_3V3, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(WAKE_FIDM, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(MUX_I2C_SEL, CY_GPIO_DM_STRONG, 0U);
        Gpio_PinConfig(MUX_SPI_SEL, CY_GPIO_DM_STRONG, 0U);    
    
        Gpio_PinConfig(Commu_led, CY_GPIO_DM_STRONG, 1U);
        Gpio_PinConfig(EN_1V8, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EN_1V15, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SW_Debug_Led, CY_GPIO_DM_STRONG, 1U);
        Gpio_PinConfig(OTPPWR_EN, CY_GPIO_DM_STRONG, 1U);
    
        Gpio_PinConfig(SPButton_S1, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S2, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S3, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S4, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S5, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(SPButton_S6, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(WAKE_SP, CY_GPIO_DM_STRONG, 0);
    
        Gpio_PinConfig(CAN_STB, CY_GPIO_DM_STRONG, 0); /*0= working*/
        Gpio_PinConfig(EX_IO2, CY_GPIO_DM_STRONG, 1);
        Gpio_PinConfig(EN_5V0, CY_GPIO_DM_STRONG, 1);
        Gpio_PinConfig(DBGP_I2C_Bypass, CY_GPIO_DM_STRONG, 0);  /*dbg i2c close*/
        Gpio_PinConfig(DBGP_SPI_Bypass, CY_GPIO_DM_STRONG, 0);  /*dbg spi close*/
        Gpio_PinConfig(Breathe_LED, CY_GPIO_DM_STRONG, 1);
        Gpio_PinConfig(DBG_UART, CY_GPIO_DM_STRONG, 1);
    
        Gpio_PinConfig(EncAP1, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EncAP2, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EncBP1, CY_GPIO_DM_STRONG, 0);
        Gpio_PinConfig(EncBP2, CY_GPIO_DM_STRONG, 0);
    
        Gpio_PinConfig(EX_FPGA1, CY_GPIO_DM_STRONG, 0);
    
#endif

    return FUNC_RETURN_OK;
}


t_FUNC_RETURN Boot_RoutineCall(void)
{

    /*Session key function debug.*/
#if 0
    uint8_t wbyte1[3] = {0x34U, 0xAAU, 0x55U};
    uint8_t wbyte2[3] = {0x34U, 0xCCU, 0x33U};
    uint8_t wbyte3[3] = {0x34U, 0x34U, 0x12U};
    uint8_t rstBuff[2] = {0x31U, 0x01U};
    /*uint8_t rbyte[5] = {0U, 0U, 0U, 0U, 0U}*/
#endif

    /*1sec routine call.*/
    if(SWTimer_Get1SecRoutine() > 0U)
    {
                Gpio_Inv(SW_Debug_Led);  
    }

    return FUNC_RETURN_OK;
}


uint16_t Boot_GetSessionKeyUser(void)
{

    return sessionKeyUser;
}

t_FUNC_RETURN Boot_SetSessionKeyUser(uint16_t val)
{
    sessionKeyUser = val;
    return FUNC_RETURN_OK;
}



uint16_t Boot_UpdateStatus(uint16_t status)
{

    Update_stCmd |= status;

    return Update_stCmd;
}

uint16_t Boot_ClrUpdateStatus(uint16_t status)
{

    Update_stCmd &= (~status);

    return Update_stCmd;
}



t_FUNC_RETURN Boot_CmdStatusRegister(uint8_t *rtnBuff)
{
    uint32_t crc;
    t_FUNC_RETURN rtn_state;

    /* we are in boot loader mode update the status register at address 0x80 */
    rtnBuff[0] = (uint8_t)Update_stCmd;
    rtnBuff[1] = (uint8_t)(Update_stCmd >> 8);

    rtn_state = Crc_CalCrc16(&rtnBuff[0], 2, &crc);

    rtnBuff[2] = (uint8_t)crc;
    rtnBuff[3] = (uint8_t)(crc >> 8U);

    return rtn_state;
}

t_FUNC_RETURN Boot_CheckWCmdLength(uint8_t WCMD, uint8_t PackageLen)
{
    t_FUNC_RETURN rtnVal = 0U;

    switch(WCMD)
    {
        case BOOT_RW_BOOTKEY:

            if(PackageLen == CMD_WRITE_BOOTKEY_LEN)
            {
                rtnVal = FUNC_RETURN_OK;
            }

            break;

        case BOOT_MCU_RESET:
            if(PackageLen == CMD_BOOT_RESET_LEN)
            {
                rtnVal = FUNC_RETURN_OK;
            }

            break;

        case BOOT_APP_UNLOCK:
            if(PackageLen == CMD_BOOT_UNLOCK_LEN)
            {
                rtnVal = FUNC_RETURN_OK;
            }

            break;

        case BOOT_APP_ERASE:
            if(PackageLen == CMD_BOOT_ERASE_LEN)
            {
                rtnVal = FUNC_RETURN_OK;
            }

            break;

        case BOOT_APP_WRITE_PAGE:

            if(PackageLen == CMD_BOOT_WRITE_PAGE_LEN)
            {
                rtnVal = FUNC_RETURN_OK;
            }

            break;

        default:
            rtnVal = FUNC_RETURN_NG;   /*Fail CMD*/
            break;
    }

    return rtnVal;
}

t_FUNC_RETURN Boot_AppUnlock(uint8_t *Boot_aryRcvData)
{
    uint32_t crc;
    uint16_t rxData;
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;

    /* check for unlock command */

    /*
      #define APP_UNLOCKED        ((uint16_t)0x0002)
      #define APP_ERASED             ((uint16_t)0x0004)
      #define FLASH_WRITE_OK      ((uint16_t)0x0008)
      */
    Update_stCmd &= 0xFC81UL; /*&0xFC0X: clear all error. 0xXXXD:clear unlock flag*/


    /* calculate the CRC16 from the received data */
    (void)Crc_CalCrc16(&Boot_aryRcvData[0U], MSG_NBR_UNLOCK, &crc);

    /* read the CRC16 that was in the received data */
    rxData = (uint16_t)Boot_aryRcvData[2U] | ((uint16_t)Boot_aryRcvData[3U] << (uint16_t)8U);

    /* do the CRC's match ? */
    if(crc == rxData)
    {
        /* get the session key */
        rxData = (uint16_t) Boot_aryRcvData[0] | ((uint16_t)Boot_aryRcvData[1] << (uint16_t)8U);

        /* do we have a match to the stored session key? */
        if(sessionKeyUser == (uint16_t)rxData)
        {
            rtn_state = FUNC_RETURN_OK;
            (void)Boot_UpdateStatus(APP_UNLOCKED);
            /*TODO*/
        }
        else
        {
            /* session key access error */
            (void)Boot_UpdateStatus(SESSION_KEY_ERROR);
        }
    }
    else
    {
        /* CRC error */
        (void)Boot_UpdateStatus(CRC_ERROR);
    }

    return rtn_state;
}

t_FUNC_RETURN Boot_AppErase(uint8_t *Boot_aryRcvData)
{
    uint32_t crc;
    uint16_t rxData;
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;

    rxData = (Update_stCmd & APP_UNLOCKED);
    /* check for Erase Application command */

    /*
    #define APP_UNLOCKED        ((uint16_t)0x0002)
      #define APP_ERASED             ((uint16_t)0x0004)
      #define FLASH_WRITE_OK      ((uint16_t)0x0008)
      */
    Update_stCmd &= 0xFC83UL; /*&0xFC0X: clear all error. 0xXXXD:clear unlock flag*/

    if(rxData == APP_UNLOCKED)
    {
        /* calculate the CRC16 from the received data */
        (void)Crc_CalCrc16(&Boot_aryRcvData[0], MSG_NBR_ERASE, &crc);

        /* read the CRC16 that was in the received data */
        rxData = (uint16_t)Boot_aryRcvData[3] | ((uint16_t)Boot_aryRcvData[4] << 8U);

        /* do the CRC's match ? */
        if(crc == rxData)
        {
            /* get the session key */
            rxData = (uint16_t)Boot_aryRcvData[0] | ((uint16_t)Boot_aryRcvData[1] << 8U);

            /* do we have a match to the stored session key? */
            if(sessionKeyUser == (uint16_t)rxData)
            {

                if(Boot_aryRcvData[2] == 0xA5U)
                {
                    rtn_state = FUNC_RETURN_OK;
                    Expect_Address = APP_START_ADDRESS;
                    /* perform the command if the flash has been unlocked */
                    (void)Boot_UpdateStatus(APP_ERASED);

                }
                else
                {
                    (void)Boot_UpdateStatus(ERASE_KEY_ERROR);
                }
            }
            else
            {
                /* session key access error */
                (void)Boot_UpdateStatus(SESSION_KEY_ERROR);
            }
        }
        else
        {
            /* CRC error */
            (void)Boot_UpdateStatus(CRC_ERROR);
        }
    }
    else
    {
        (void)Boot_UpdateStatus(UNKNOWN_ERROR);
    }

    return rtn_state;
}

/*
*
*   PSOC4 4100s MAX       CY_FLASH_SIZEOF_ROW=256 bytes
*
*
*/
t_FUNC_RETURN Boot_AppWrite(uint8_t *Boot_aryRcvData, uint16_t DataLen)
{
    uint32_t crc;
    uint16_t rxData;
    uint32_t page_address;
    uint16_t buf_idx;
    uint16_t i;
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;


    Boot_ClrUpdateStatus(FLASH_WRITE_OK);

    rxData = (Update_stCmd & (APP_UNLOCKED | APP_ERASED));
    /* check for Write Flash Page command */
    Update_stCmd = Update_stCmd & 0xFC87UL; /*&0xFC0X: clear all error. 0xXXX7:write flasg flag*/

    if(rxData == (uint16_t)(APP_UNLOCKED | APP_ERASED))
    {
#if 1
        /*package size are 256byte*/
        /* calculate the CRC16 from the received data */
        (void)Crc_CalCrc16(&Boot_aryRcvData[0], (DataLen + 5U), &crc);

        /* read the CRC16 that was in the received data */
        rxData = (uint16_t)Boot_aryRcvData[(DataLen + 5U)] | ((uint16_t)Boot_aryRcvData[(DataLen + 6U)] << 8U);
#else
        /*package size are 64byte*/
        /* calculate the CRC16 from the received data */
#if (SW_OR_HW_CRC == SW_CRC)  
        (void)Crc_SwCalCrc16(&Boot_aryRcvData[0], MSG_NBR_PAGE_WRITE, &crc);
#else
        (void)Crc_CalCrc16(&Boot_aryRcvData[0], MSG_NBR_PAGE_WRITE, &crc);
#endif
        /* read the CRC16 that was in the received data */
        rxData = (uint16_t)Boot_aryRcvData[69] | ((uint16_t)Boot_aryRcvData[70] << 8U);
#endif

        /* do the CRC's match ? */
        if(crc == rxData)
        {
            /* get the session key */
            rxData = (uint16_t)Boot_aryRcvData[0] | ((uint16_t)Boot_aryRcvData[1] << 8U);

            /* do we have a match to the stored session key? */
            if(sessionKeyUser == (uint16_t) rxData)
            {
                /* perform the command if the flash has been unlocked */

                /* construct 24-bit address that was sent from the host */
                page_address = (uint32_t)Boot_aryRcvData[2] | ((uint32_t)Boot_aryRcvData[3] << 8U) | ((uint32_t)Boot_aryRcvData[4] << 16U);

                /* only write to flash at or above the application start address */
                if((page_address >= APP_START_ADDRESS) && (page_address <= APP_OVERFLOW_ADDRESS) && (page_address == Expect_Address))
                {
                    buf_idx = (uint8_t)page_address;

                    for(i = 0; i < DataLen; i++)
                    {
                        WPageBuff[buf_idx] = Boot_aryRcvData[i + 5];
                        buf_idx++;
                    }

                    Expect_Address = (page_address + DataLen);

                    if(0x100U == buf_idx)
                    {
                        rtn_state = Flash_WriteRow((page_address & 0xFFFFFF00U), WPageBuff);

                        if(rtn_state == FUNC_RETURN_OK)
                        {
                            /* Notify the host that the operation was run */
                            (void)Boot_UpdateStatus(FLASH_WRITE_OK);
                            /*TODO: add a verify operation of the flash page that was just programmed */
                        }
                        else
                        {
                            /* programming the flash page failed */
                            (void)Boot_UpdateStatus(PROGRAM_ERROR);
                        }
                    }
                    else if(0x100U < buf_idx)
                    {
                        rtn_state = FUNC_RETURN_NG;
                        (void)Boot_UpdateStatus(UNKNOWN_ERROR);
                    }
                    else
                    {
                        rtn_state = FUNC_RETURN_OK;
                        (void)Boot_UpdateStatus(FLASH_WRITE_OK);
                    }

                }
                else
                {
                    /* flash programming address is below the application start address  */
                    (void)Boot_UpdateStatus(PROGRAM_ERROR);
                }

            }
            else
            {
                /* session key access error */
                (void)Boot_UpdateStatus(SESSION_KEY_ERROR);
            }
        }
        else
        {
            /* CRC error */
            (void)Boot_UpdateStatus(CRC_ERROR);
        }
    }
    else
    {
        (void)Boot_UpdateStatus(UNKNOWN_ERROR);
    }

    return rtn_state;

}

t_FUNC_RETURN Project_ExtINT_Callback(uint8_t PortPin)
{
    if(PortPin == 0xB5)
    {
#if 0  /*DBG*/
                Gpio_Inv(SW_Debug_Led);  
#endif
    }

    return FUNC_RETURN_OK;
}

#endif
