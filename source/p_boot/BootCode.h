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


#ifndef _BOOT_CODE_H_
#define _BOOT_CODE_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "project.h"
#include "TypeDefine.h"

#if (LOT3_BOOT_ENABLE==1U)
/* ==[ Macros ]============================================================= */
/**PROJECT DEFINE**/
#define FW_BOOT_VER         0x01U
#define FW_BOOT_MINOR_VER   0x01U

#define BOARD               BOARD_PSOC4
#define SW_OR_HW_CRC        HW_CRC
#define SW_OR_HW_PWM        SEL_NO_PWM

/***PROJECT DEFINE END***/


/* Boot loader Status */
#define STATE_IN_BOOT_MODE  ((uint16_t)0x0001U)
#define APP_UNLOCKED        ((uint16_t)0x0002U)
#define APP_ERASED          ((uint16_t)0x0004U)
#define FLASH_WRITE_OK      ((uint16_t)0x0008U)
#define SESSION_KEY_ERROR   ((uint16_t)0x0010U)
#define ERASE_KEY_ERROR     ((uint16_t)0x0020U)
#define CRC_ERROR           ((uint16_t)0x0040U)
#define VOLTAGE_ERROR       ((uint16_t)0x0080U)
#define PROGRAM_ERROR       ((uint16_t)0x0100U)
#define UNKNOWN_ERROR       ((uint16_t)0x0200U)

#if 0
/*Host command. Move to i2cs.h*/ 
/*For BOOT*/
enum I2cs_ComMode
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
#endif


/**/
#define CMD_BOOT_IDLE            0U
#define CMD_BOOT_RESET           1U
#define CMD_BOOT_UNLOCK          2U
#define CMD_BOOT_ERASE           4U
#define CMD_BOOT_WRITE_PAGE      8U
#define CMD_WRITE_BOOTKEY        16U
#define CMD_BOOT_WRITE_TEST      32U
#define CMD_DISPLAY_BACKLIGHT    64U

#define CMD_WRITE_BOOTKEY_LEN            ((uint8_t)3U)  /*CMD(34)+55+AA->CMD(34)+33+CC->CMD(34)+Key0+Key1*/
#define CMD_BOOT_RESET_LEN               ((uint8_t)2U)  /*CMD(31)+02*/
#define CMD_BOOT_UNLOCK_LEN              ((uint8_t)5U)  /*CMD(84)+2Keys+2CRC*/
#define CMD_BOOT_ERASE_LEN               ((uint8_t)6U)  /*CMD(88)+2Keys+A5+2CRC*/
#define CMD_BOOT_WRITE_PAGE_LEN          ((uint8_t)72U) /*CMD(8D)+2Keys+3Addr+64Data+2CRC*/
#define CMD_DISPLAY_BACKLIGHT_LEN        ((uint8_t)6U)  /*CMD(20)+CRC8+DisplayID+PWM0+PWM1+Counter*/

/*for CRC16 index. Don't include CMD.*/
#define MSG_NBR_STATUS ((uint32_t)0x02U)
#define MSG_NBR_UNLOCK ((uint32_t)0x02U)
#define MSG_NBR_ERASE ((uint32_t)0x03U)
#define MSG_NBR_PAGE_WRITE ((uint32_t)69U) /*package=64bytes, 2+3+64=69*/
                                           /*package=256bytes, 2+3+256=261*/


/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */
extern t_FUNC_RETURN Boot_Init(void);
extern t_FUNC_RETURN Project_Gpio_Init(void);
extern uint16_t Boot_GetSessionKeyUser(void);
extern t_FUNC_RETURN Boot_SetSessionKeyUser(uint16_t val);
extern t_FUNC_RETURN Boot_CmdStatusRegister(uint8_t *rtnBuff);
extern uint16_t Boot_UpdateStatus(uint16_t status);
extern uint16_t Boot_ClrUpdateStatus(uint16_t status);
extern t_FUNC_RETURN Boot_CheckWCmdLength(uint8_t WCMD, uint8_t PackageLen);
extern t_FUNC_RETURN Boot_RoutineCall(void);
extern t_FUNC_RETURN Boot_AppUnlock(uint8_t *Boot_aryRcvData);
extern t_FUNC_RETURN Boot_AppErase(uint8_t *Boot_aryRcvData);
extern t_FUNC_RETURN Boot_AppWrite(uint8_t *Boot_aryRcvData, uint16_t DataLen);
extern t_FUNC_RETURN Project_ExtINT_Callback(uint8_t PortPin);
#endif
#endif
