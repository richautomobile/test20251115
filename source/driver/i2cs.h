/**
 * \file
 *
 * \brief Brief description of the header file.
 *
 * Provide a detailed description of the header file.
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


#ifndef _I2CS_H_
#define _I2CS_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"
#include "project.h"

/* ==[ Macros ]============================================================= */
#define EN_DEV_ENCODER
/*CMD_Mode=0, For APP*/
enum I2cs_ComMode0
{
    CMD_GET_FW_VERSION = 0x01U,
    CMD_PSOC_RESET = 0x02U,
    CMD_GET_USER_RAM_DATA = 0x03U,
    CMD_SET_PSOC_IICA_CLOCK = 0x06U,
    CMD_GET_PSOC_IICA_CLOCK = 0x07U,
    CMD_SET_LOT3_IO = 0x12U,
    CMD_GET_LOT3_IO = 0x13U,
    CMD_SET_LOT3_PORT = 0x14U,
    CMD_GET_LOT3_PORT = 0x15U,    
    CMD_GET_ADC_VAL = 0x16U,
    CMD_GET_BUTTON  = 0x17U,
    CMD_GET_DIP_SWITCH = 0x18U,   
    CMD_GET_MODULE_CURRENT = 0x19U,
    CMD_GET_USER_FLASH_PARAMETER = 0x20U,
    CMD_GET_USER_FLASH_NEWEST = 0x21U,
    CMD_GET_USER_FLASH_BY_ADDR = 0x22U,
    CMD_SET_USER_FLASH_CLEAR_ALL = 0x23U,
    CMD_SPI_WRITE = 0x28U,
    CMD_SPI_READ = 0x29U,
    CMD_SET_PSOC_SPI_CKPOL_CKPHA = 0x2FU, 
    CMD_SET_PSOC_SPI_CLOCK = 0x30U,
    CMD_GET_PSOC_SPI_CLOCK = 0x31U,
    CMD_SET_PWM_PIN = 0x75U,
    CMD_SET_PWM_FREQ = 0x76U,
    CMD_SET_PWM_DUTY = 0x77U,
    CMD_IIC_WRITE  = 0x7CU,
    CMD_SET_ENCODER = 0x82U,
    CMD_GPO_TOGGLE = 0x88U,
    CMD_IIC_READ = 0xBBU,
    CMD_SPI_2STEP_WRITE = 0xBCU,
    CMD_SPI_2STEP_READ = 0xBDU,
    CMD_PSOC_IIC_2STEP_WRITE = 0xBEU,
    CMD_PSOC_IIC_2STEP_READ = 0xBFU,
    CMD_2STEP_FAKE_DATA_FLASH_WRITE = 0xC0U,
    CMD_2STEP_FAKE_DATA_FLASH_READ = 0xC1U,
    CMD_2STEP_CANFD_WRITE = 0xC2U,
    CMD_2STEP_CANFD_READ = 0xC3U,    
    CMD_CAN_SET_BitRate=0xC4,
    
    CMD_GET_SP_PWM_DUTY = 0xC6U,
    CMD_2STEP_UART_WRITE = 0xC7U,
    CMD_2STEP_UART_READ = 0xC8U,
    CMD_SET_UART_BAUD = 0xC9U,
    
    CMD_GET_MCU_IO_LEVEL = 0xCBU,
    CMD_SWD_ONOFF_CONTROL = 0xE0U,
    CMD_GET_PSOCID = 0xE1U,
    CMD_2STEP_READ_RETURN_DATA = 0xF0U,
    CMD_GET_RETURN_ACK_NACK = 0xF1U
    
};

/*Host command*/
/*For BOOT*/
enum I2cs_BootCom
{
    BOOT_HW_VER = 0x00U,   /*2 bytes*/
    BOOT_FW_VER = 0x02U,   /*2 bytes*/
    BOOT_MCU_RESET = 0x31U,   /*1 bytes*/
    BOOT_RW_BOOTKEY = 0x34U,  /*2 bytes*/
    BOOT_READ_STATE = 0x80U,  /*4 bytes*/
    BOOT_APP_UNLOCK = 0x84U,  /*4 bytes*/
    BOOT_APP_ERASE  = 0x88U,  /*5 bytes*/
    BOOT_APP_WRITE_PAGE = 0x8DU  /*71 bytes*/
};


struct StrRtnData  /*0x00F1*/
{
    uint8_t RtnMode; /*0*/
    uint8_t RtnID;   /*ID*/
    uint8_t MagicNo; /*MagicNo*/
    uint8_t AckNack; /*A!*/
    uint16_t RtnLen;
    uint8_t RtnBuff[256]; /*A!read data array*/ /*ack + read data array*/
};

/*long time execute buffer of I2cs.*/
struct StrI2csReceiveData
{
    uint8_t enable;
    uint16_t WLen;
    uint8_t WBuff[264];
};
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN I2cs_Init(void);
extern void I2CS_InterruptHandler(void);
#if (LOT3_BOOT_ENABLE==0U)
extern t_FUNC_RETURN I2cs_2StepRunProcess(void);
extern t_FUNC_RETURN I2cs_SetRTNState(uint8_t mode, uint8_t id, uint8_t magic_no, uint8_t state);
extern t_FUNC_RETURN I2cs_SetRtnData(struct StrRtnData RtnStrBuff);
extern t_FUNC_RETURN I2cs_UpdateReadBuff(uint8_t updateBuff[], uint8_t len);
#endif
#endif
