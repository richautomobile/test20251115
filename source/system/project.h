/*
 * \file project.h
 * \brief
 *  Created on: Dec 21, 2023
 *
 *
 *
 * \author walker.chiang <walker.chiang@carux.com>
 *
 * \copyright (C) 2023 Car-UX Display Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Cax-UX Display
 * Corporation.
 */

#ifndef _PROJECT_H_
#define _PROJECT_H_

/* ==[ Includes ]=========================================================== */
/* ==[ Macros ]============================================================= */
/*===============================================*/
/*return version value: HW_VER.FW_APP_VER.BOARD.FW_APP_MINOR_VER */
/*Modify1.: Setting fw version.*/
#define HW_VER              0x01U
#define FW_APP_VER          0x01U
#define FW_APP_MINOR_VER    0x0AU /*0x0 for test fw release*/

/*===============================================*/
/*Modify2. : PCBA board selection*/
#define BOARD_PSOC4         0x01U
#define BOARD_AGING         0x02U
/*#define BOARD               BOARD_PSOC4*/ //Move to each project's .h file

/*===============================================*/
/*Modify3. : Build fw mode selection. */
/*BRIDGE_ENABLE, AGING_ENABLE, LONGRUN_ENABLE, TESTING_ENABLE and BOOT_ENABLE should only be turned on when necessary.
For the version used on the production line, just turn on BRIDGE_UNIV_ENABLE.*/
#define LOT3_BOOT_ENABLE    0U
#define BRIDGE_UNIV_ENABLE  1U
#define BRIDGE_ENABLE       0U
#define AGING_ENABLE        0U
#define LONGRUN_ENABLE      0U
#define TESTING_ENABLE      0U

/*====================================================*/
/*Modify4. : project selection.  BRIDGE_UNIV_ENABLE and LOT3_BOOT_ENABLE don't care abour the PROJECT definition.*/
/*project name define*/
#define NULL_PRJ            0U
#define GM_VCS_1134         1U
#define GM_VCS_3400         2U
#define GM_VCS_3500         3U
#define PROJECT             GM_VCS_3400

/*=================================================*/
/*Modify5. : HW or SW CRC selection.*/
#define HW_CRC              0U
#define SW_CRC              1U
/*#define SW_OR_HW_CRC         HW_CRC*/ //Move to each project's .h file

/*=================================================*/
/*Modify6.: HW or SW PWM selection.*/
#define SEL_SW_PWM          0U
#define SEL_HW_PWM          1U
#define SEL_NO_PWM          0xFFU
/*#define SW_OR_HW_PWM        SEL_NO_PWM*/ //Move to each project's .h file  

/*=================================================*/
/*Modify7.: Debug setting*/
#define EN_DEBUG    1U

/*=================================================*/
/*Modify8.: Normally no modification is required*/
/*   ALL_Flash_Range=0x00000~0x3FFFF(256KBytes) */
/*   USER_Flash_Data=0x3E000~0x3FFFF(8KBytes) */
#define UFLASH_START_ADDR   0x3E000UL
#define UFLASH_END_ADDR     0x3FFFFUL
/****UFLASH_PACKAGE_SIZE = 2bytes index + user data + 1byte checksum****/
/*Limit input value :8, 16, 32, 64, 128, 256 bytes. */
#define UFLASH_DEFAULT_PACKAGE_SIZE 32U
#define UFLASH_TOTAL_SIZE   ((UFLASH_END_ADDR-UFLASH_START_ADDR)+1)
#define UFLASH_MAX_NO       0x8000U
/*Modify8.1: Fake data flash by code flash*/
#define FAKE_DATA_FLASH_START_ADDR   0x3A000UL
#define FAKE_DATA_FLASH_END_ADDR     0x3DFFFUL



/*=================================================*/
/*Modify9.: Normally no modification is required*/
/*Boot code section define */
#define BOOT_SIZE            0x4000U
#define APP_START_ADDRESS    0x4000U
#define APP_END_ADDRESS      0xFFFFU
#define APP_SIZE             0xC000U
#define APP_OVERFLOW_ADDRESS (APP_END_ADDRESS+1U)
/*CRC SART : 32bits crc32*/
#define APP_CRC_ADDRESS      (APP_END_ADDRESS-3U)

/*=================================================*/
/*Modify10.: Add the .h file of the new project.*/
/*#include "gitinfo.h"*/
#if (LOT3_BOOT_ENABLE==1U)
#include "BootCode.h"
#elif (AGING_ENABLE==1U)
#include "Aging_GM1134.h"
#include "Aging_GM3400.h"
#include "Aging_GM3500.h"
#elif (LONGRUN_ENABLE==1U)
#include "LongRun_GM1134.h"
#include "LongRun_GM3400.h"
#elif (TESTING_ENABLE==1U)
#include "Testing_GM1134.h"
#include "Testing_GM3400.h"
#elif (BRIDGE_ENABLE==1U)
#elif (BRIDGE_UNIV_ENABLE==1U) 
#include "BridgeBoardAll.h"
#endif

/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */

#endif /* PROJECT_H_ */
