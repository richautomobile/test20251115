/**
 * @file Flash.c
 *
 * @brief
 *
 * Copyright (C) 2022 Innolux Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Innolux Corporation
 *
 * MISRA-C:2012 Deviation List
 *
 * MISRA-2:
 * Deviated Rule:
 *
 * @author BingHan (binghan.chou@innolux.com)
 * @date 2022-11-10
 */


/* ==[ Includes ]============================================================ */
/*******************************************************************************
 * Include header files
 ******************************************************************************/
/*
#include "cy_pdl.h"
#include "cybsp.h"
*/
#include <stdio.h>
#include "user_flash.h"
#include "flash.h"
#include "string.h"
#include "wdt.h"


/* ==[ Macros ]============================================================== */
/* ==[ Types ]=============================================================== */
/* ==[ Constants with external linkage ]===================================== */
/* ==[ Variables with external linkage ]===================================== */
/*UserDataPackageSize = 2bytes index + UserDataArray size + 2byte chk*/
static uint16_t UserDataPackageSize = UFLASH_DEFAULT_PACKAGE_SIZE;
static uint16_t UserDataCurrentIdxNo;  /*init value 0*/
static uint32_t UserDataCurrentIdxAddr;
static uint8_t UserDataCurrentRowBuf[256];
/*
UserDataArray
[0]: Index      [1]: HW version     [2]:FW version
[3]:Reset counter   [4]: Power-on counter
[5]: Dig2.bit7(bit0) + rotation state(bit1) + Power OVP(bit2)
[6]: PWM1&2 multiplier  [7]: PWM3&4 multiplier  [8]: Display PWM multiplier(H) + rotation counter(L)
[9]: SW part number H   [10]: SW part number L  [11]: HW part number H   [12]: HW part number L  [13][14]: reserved  [15]: checksum
*/

/* ==[ Declaration of functions with internal linkage ]====================== */
static uint8_t UserFlash_CalCHKSUM(uint8_t Buff[], uint32_t rLen);
static t_FUNC_RETURN UserFlash_CompareCHKSUM(uint8_t Buff[], uint32_t rLen);
/* ==[ Definition of functions with internal linkage ]====================== */
/*******************************************************************************
 *  \brief Function Name: UserFlash_CalCHKSUM
 *******************************************************************************
 *
 * Set read buffer process for I2CS
 *
 *
 * \Parameters [in] rLen: master read buffer length.
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static uint8_t UserFlash_CalCHKSUM(uint8_t Buff[], uint32_t rLen)
{
    uint8_t xor = 0U;
    uint32_t i;

    for(i = 0U; i < (rLen); i++)
    {
        xor = xor ^ Buff[i];
    }

    xor = xor + 1U;

    return xor;
}
/*******************************************************************************
 *  \brief Function Name: I2cs_CompareCHKSUM
 *******************************************************************************
 *
 * Set read buffer process for I2CS
 *
 *
 * \Parameters [in] rLen: master read buffer length.
 * \return          t_FUNC_RETURN
 ******************************************************************************/
static t_FUNC_RETURN UserFlash_CompareCHKSUM(uint8_t Buff[], uint32_t rLen)
{
    uint8_t xor = 0U;
    uint32_t i;
    t_FUNC_RETURN rtn_state;

    for(i = 0U; i < (rLen); i++)
    {
        xor = xor ^ Buff[i];
    }

    xor = xor + 1U;

    if(xor == Buff[rLen])
    {
        rtn_state = FUNC_RETURN_OK;

    }
    else
    {
        rtn_state = FUNC_RETURN_NG;
    }

    return rtn_state;
}

/* ==[ Definition of functions with external linkage ]====================== */
/*******************************************************************************
* \brief Function Name: UserFlash_GetPackageSize
********************************************************************************
* Summary:
*
*
*
* \Parameters [in] NONE
* \return          t_FUNC_RETURN
*
*******************************************************************************/
uint16_t UserFlash_GetPackageSize(void)
{
    return UserDataPackageSize;
}

/*******************************************************************************
* \brief Function Name: UserFlash_Init
********************************************************************************
* Summary:
* UserFlash init. Initialization is executed once
*
*
* \Parameters [in] UserDefSize : 256, 128, 64, 32, 16, 8
* \return                t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN UserFlash_Init(uint16_t UserDefSize)
{
    uint32_t i;
    volatile uint16_t *addr_idx;
    uint16_t IdxNoDelta;
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;


    if(UserDefSize > 128)
    {
        UserDataPackageSize = 256;
    }
    else if(UserDefSize > 64)
    {
        UserDataPackageSize = 128;
    }
    else if(UserDefSize > 32)
    {
        UserDataPackageSize = 64;
    }
    else if(UserDefSize > 16)
    {
        UserDataPackageSize = 32;
    }
    else if(UserDefSize > 8)
    {
        UserDataPackageSize = 16;
    }
    else
    {
        UserDataPackageSize = 8;
    }

    /*Step1: search the last user data.*/
    UserDataCurrentIdxNo = 0U;
    UserDataCurrentIdxAddr = UFLASH_START_ADDR;

    for(i = UFLASH_START_ADDR; i < UFLASH_END_ADDR; i = (i + UserDataPackageSize))
    {
        addr_idx = (uint16_t *)i;

        if((*addr_idx > UFLASH_MAX_NO) || (*addr_idx == 0U))
        {
            /*Empty or Stop search*/
            rtn_state = FUNC_RETURN_NG;
            break;
        }
        else
        {

            if(*addr_idx >= UserDataCurrentIdxNo)
            {
                /*small -> big*/
                IdxNoDelta = *addr_idx - UserDataCurrentIdxNo;

                /*delta =1 or The first item*/
                if((IdxNoDelta == 1U) || (UserDataCurrentIdxNo == 0U))
                {
                    if(UserFlash_CompareCHKSUM((uint8_t *)addr_idx, (UserDataPackageSize - 2U)) == FUNC_RETURN_OK)
                    {
                        UserDataCurrentIdxNo = *addr_idx;
                        UserDataCurrentIdxAddr = i;
                    }
                    else
                    {
                        printf("Small to Big CHK error.\r\n");
                        /*checksum error*/
                        rtn_state = FUNC_RETURN_NG;
                        break;
                    }
                }
                else
                {
                    /*Stop search*/
                    rtn_state = FUNC_RETURN_OK;
                    break;
                }
            }
            else
            {
                /*big -> small. */
                if((UserDataCurrentIdxNo == UFLASH_MAX_NO) && (*addr_idx == 1U))
                {
                    if(UserFlash_CompareCHKSUM((uint8_t *)addr_idx, (UserDataPackageSize - 2U)) == FUNC_RETURN_OK)
                    {
                        UserDataCurrentIdxNo = *addr_idx;
                        UserDataCurrentIdxAddr = i;
                    }
                    else
                    {
                        /*checksum error*/
                        printf("Big to small CHK error.\r\n");

                        rtn_state = FUNC_RETURN_NG;
                        break;
                    }
                }
                else
                {
                    /*Stop search*/
                    rtn_state = FUNC_RETURN_OK;
                    break;
                }
            }

        }
    }

    //printf("UserDataCurrentIdxAddr = 0x%4x\r\n", (uint16_t)(UserDataCurrentIdxAddr>>16));
    //printf("UserDataCurrentIdxAddr = 0x%08lx\r\n", UserDataCurrentIdxAddr);

    /*Step2: copy flash data in the local buffer. The default value of PSOC FLASH is 0x00.*/
    memcpy((void *)UserDataCurrentRowBuf, (const void *)(UserDataCurrentIdxAddr & 0xFFFFFF00U), 256);

    return rtn_state;
}

/*******************************************************************************
* \brief Function Name: UserFlash_GetUserData
********************************************************************************
* Summary:
* return user data.
*
*
* \Parameters [in] NONE
* \return          uint8_t *: return (index+ user data + checksum)
*
*******************************************************************************/
t_FUNC_RETURN UserFlash_GetUserData(uint8_t RBuff[], uint16_t rLen)
{
    memcpy(RBuff, &UserDataCurrentRowBuf[(UserDataCurrentIdxAddr & 0x000000FFU)], rLen);

    return FUNC_RETURN_OK;
}

/*******************************************************************************
* \brief Function Name: UserFlash_GetPureUserData
********************************************************************************
* Summary:
* return user data bypassing idx and checksum.
*
*
* \Parameters [in] NONE
* \return          uint8_t *: only return user data
*
*******************************************************************************/
t_FUNC_RETURN UserFlash_GetPureUserData(uint8_t RBuff[], uint16_t rLen)
{
    t_FUNC_RETURN rtn_state;

    if(UserDataPackageSize < (rLen + 4U))  /*2bytes index + 2byte chk*/
    {
        rtn_state = FUNC_RETURN_NG;
    }
    else
    {
        /*check checksum.*/
        rtn_state = UserFlash_CompareCHKSUM((&UserDataCurrentRowBuf[(UserDataCurrentIdxAddr & 0x000000FFU)]), (UserDataPackageSize - 2U));
    }

    if(rtn_state == FUNC_RETURN_OK)
    {
        memcpy(RBuff, (&UserDataCurrentRowBuf[(UserDataCurrentIdxAddr & 0x000000FFU)]) + 2U, rLen);
    }
    else
    {
        memset(RBuff, 0x00U, rLen);
    }

    return rtn_state;
}


/*******************************************************************************
* \brief Function Name: UserFlash_GetUserDataByIdx
********************************************************************************
* Summary:
* return user data.
*
*
* \Parameters [in] NONE
* \return          uint8_t *: return 32 bytes data
*
*******************************************************************************/
t_FUNC_RETURN UserFlash_GetUserDataByIdx(uint8_t RBuff[], uint16_t rLen, uint16_t Idx)
{
    volatile uint8_t *p_real_addr;
    uint32_t offset;

    offset = (uint32_t)(UserDataPackageSize * Idx);
    p_real_addr = (uint8_t *)(UFLASH_START_ADDR + offset);
    memcpy(RBuff, (const uint8_t *)p_real_addr, rLen);

    return FUNC_RETURN_OK;
}

/*******************************************************************************
* \brief Function Name: UserFlash_GetUserDataByAddr
********************************************************************************
* Summary:
* return user data.
*
*
* \Parameters [in] uint32_t SAddr
* \Parameters [in] uint16_t rLen
* \return          uint8_t *: return array
*
*******************************************************************************/
t_FUNC_RETURN UserFlash_GetUserDataByAddr(uint8_t RBuff[], uint32_t SAddr, uint16_t rLen)
{
    uint32_t interruptState;
    volatile uint8_t *p_real_addr = (uint8_t *)SAddr;
    interruptState = Cy_SysLib_EnterCriticalSection();

    memcpy(RBuff, (const uint8_t *)p_real_addr, rLen);

    Cy_SysLib_ExitCriticalSection(interruptState);
    return FUNC_RETURN_OK;
}


/*******************************************************************************
* \brief Function Name: UserFlash_Write
********************************************************************************
* Summary:
* return user data.
*
*
* \param [in] WBuff : write data buffer. max buffer size are 32BYTES.
*             ForceUpdate : >0 force update, 0:
* \return t_FUNC_RETURN
*/
t_FUNC_RETURN UserFlash_Write(uint8_t *WBuff, uint16_t WLen, uint8_t ForceUpdate)
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
    uint8_t NewWBuff[UserDataPackageSize];
    uint32_t new_addr;
    uint16_t offset;

    /*Step1: check write data length.*/
    if(UserDataPackageSize < (WLen + 4U))  /*2bytes index + 2byte chk*/
    {
        return FUNC_RETURN_NG;
    }

    /*Step2: check if over flash saving range.*/
    if((UserDataCurrentIdxAddr == UFLASH_START_ADDR) && \
            (UserDataCurrentIdxNo == 0U))
    {
        new_addr = UserDataCurrentIdxAddr;
    }
    else
    {
        new_addr = UserDataCurrentIdxAddr + UserDataPackageSize;
    }

    if(new_addr > UFLASH_END_ADDR)
    {
        UserDataCurrentIdxAddr = UFLASH_START_ADDR;
    }
    else
    {
        UserDataCurrentIdxAddr = new_addr;
    }

    if(UserDataCurrentIdxNo == 0U)
    {
        UserDataCurrentIdxNo = 1U;
    }
    else
    {
        UserDataCurrentIdxNo++;
    }

    if(UserDataCurrentIdxNo > UFLASH_MAX_NO)
    {
        UserDataCurrentIdxNo = 1U;
    }

    /*Step3: copy new row in the Row buffer.*/
    offset = (uint16_t)(UserDataCurrentIdxAddr & 0x000000FFU);

    if(offset == 0U) /*get new row data*/
    {
        memcpy(UserDataCurrentRowBuf, (void *)UserDataCurrentIdxAddr, 256U);
    }

    /*Step4: save data in the buffer*/
    memset(NewWBuff, 0xFFU, UserDataPackageSize);  /*Temp buffer fill init value.*/
    NewWBuff[0] = (uint8_t)(UserDataCurrentIdxNo); /*change to new index number.*/
    NewWBuff[1] = (uint8_t)(UserDataCurrentIdxNo >> 8U);
    memcpy(&NewWBuff[2], WBuff, WLen);
    /*cal checksum*/
    NewWBuff[(UserDataPackageSize - 2U)] = UserFlash_CalCHKSUM(NewWBuff, (UserDataPackageSize - 2U));
    NewWBuff[(UserDataPackageSize - 1U)] = 0U;
    memcpy(&UserDataCurrentRowBuf[offset], NewWBuff, UserDataPackageSize);

    /*Step5: write data in the code flash*/
    new_addr = UserDataCurrentIdxAddr + UserDataPackageSize;

    if(((new_addr & 0xFFFFFF00U) == 0U) || (ForceUpdate > 0U))
    {
        rtn_state = Flash_WriteRow((UserDataCurrentIdxAddr & 0xFFFFFF00U), UserDataCurrentRowBuf);
    }

    return rtn_state;
}

/*******************************************************************************
* \brief Function Name: UserFlash_ClearAll
********************************************************************************
* Summary:
* Clear all user data buffer. USER_Flash_Data=0x3F000~0x3FFFF(4KBytes)
*
*
* \param [in] NONE
*
* \return t_FUNC_RETURN
*/
t_FUNC_RETURN UserFlash_ClearAll(void)
{
    t_FUNC_RETURN rtn_state;
    uint16_t i;
    uint32_t addr_idx = UFLASH_START_ADDR;

    for(i = 0; i < 256; i++)
    {
        UserDataCurrentRowBuf[i] = (uint8_t)FlashInitValus;
    }

    for(i = 0; i < 16; i++)  /*TODO*/
    {
        rtn_state = Flash_WriteRow(addr_idx, UserDataCurrentRowBuf);
        addr_idx += 256U;

        Wdt_Clear();
    }

    /*init user flash variable*/
    UserDataCurrentIdxNo = 0U;
    UserDataCurrentIdxAddr = (uint32_t)UFLASH_START_ADDR;
    return rtn_state;
}

/*******************************************************************************
* \brief Function Name: UserFlash_ForceSave
********************************************************************************
* Summary:
* Force the current buffer data to be stored in flash
*
*
* \param [in] NONE
*
* \return t_FUNC_RETURN
*/
t_FUNC_RETURN UserFlash_ForceSave(void)
{
    return Flash_WriteRow((UserDataCurrentIdxAddr & 0xFFFFFF00U), UserDataCurrentRowBuf);
}


