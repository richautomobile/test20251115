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
#include "flash.h"


/* ==[ Macros ]============================================================== */
/*******************************************************************************
 * Macros and Constants
 ********************************************************************************/
#define CY_SRAM_BASE                    0x20000000UL
#define CY_SRAM_SIZE                    0x00008000UL
#define CY_FLASH_BASE                   0x00000000UL
#define CY_FLASH_SIZE                   0x00060000UL
#define CY_SFLASH_BASE                  0x0FFFE000UL
#define CY_SFLASH_SIZE                  0x00001800UL
#define CY_ROM_BASE                     0x10000000UL
#define CY_ROM_SIZE                     0x00002000UL
#define CY_CAN0MRAM_BASE                0x40410000UL
#define CY_CAN0MRAM_SIZE                0x00010000UL
/* ==[ Types ]=============================================================== */
/* ==[ Constants with external linkage ]===================================== */
/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with internal linkage ]====================== */
/* ==[ Definition of functions with internal linkage ]====================== */
/* ==[ Definition of functions with external linkage ]====================== */
/** \brief Write page date in the code flash
*
*   PSOC4 4100s MAX       CY_FLASH_SIZEOF_ROW=256 bytes
*
*   Erases a row of Flash or Supervisory Flash and programs it with the new data.
*   The size of the array must be equal to the Flash row size defined by CY_FLASH_SIZEOF_ROW.
*
* \param [in] SAddr : write start address
*             WBuff : write data buffer. The size of the array must be equal to the Flash row size defined by CY_FLASH_SIZEOF_ROW.
*
* \return t_FUNC_RETURN
*/
t_FUNC_RETURN Flash_WriteRow(uint32_t SAddr, const uint8_t *WBuff)
{
    cy_en_flashdrv_status_t flash_write_status;
    bool error_flag = false;
    /*const uint8_t *flash_data = (uint8_t *)(SAddr);*/

#if 1
    /* blocking flash write */
    flash_write_status = Cy_Flash_WriteRow(SAddr, (const uint32_t *)WBuff);
#else
    /*non-blocking flash write*/
    flash_write_status = Cy_Flash_StartWrite(SAddr, (const uint32_t *)WBuff);
#endif


    if(flash_write_status == CY_FLASH_DRV_SUCCESS)
    {
#if 0
        /* The non-blocking write row API Cy_Flash_StartWrite() requires that
         * Cy_Flash_ResumeWrite() function be called 3 times to complete the write.
         * It is advised not to prolong calling this API for more than 25 ms.
         */
        for(int i = 0; i < 3; i++)
        {
            Cy_SysLib_Delay(25); //FLASH_WRITE_DELAY_MS);
            (void)Cy_Flash_ResumeWrite();
            Cy_SysLib_Delay(25); //FLASH_WRITE_DELAY_MS);
        }
#endif

        /* Wait for the successful flash write */
        if(Cy_Flash_IsOperationComplete() != CY_FLASH_DRV_SUCCESS)
        {
            error_flag = true;
        }
#if 0        
        else
        {
            /* Verify the data written into flash by comparing it with the RAM data */
            if(memcmp(WBuff, flash_data, CY_FLASH_SIZEOF_ROW) != 0u)
            {
                error_flag = true;
            }
        }
#endif        
    }
    else  /*flash write operation did not start*/
    {
        /* Flag error if the Cy_Flash_StartWrite API status is not as expected*/
        error_flag = true;
    }

    return (t_FUNC_RETURN)error_flag;
}


