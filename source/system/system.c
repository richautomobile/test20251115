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
#include <stdarg.h>
#include "system.h"
#include "uart.h"
#include "gpio.h"
#include "flash.h"
#include "crc.h"
#include "I2cs.h"
#include "adc.h"
#include "i2cm.h"
#include "pwm_capture.h"
#include "SpiMaster.h"
#include "SoftwareTimer.h"
#include "user_flash.h"
#include "wdt.h"
#include "tcpwm_out.h"
#include "swpwm_out.h"
#include "breath_led.h"
#include "systick.h"
#include "button.h"
#include "canfd.h"
/*#include "timer.h"*/

/* ==[ Macros ]============================================================= */
#define ROW_SIZE (256)
/*user ram define =>   .userSec section */
#define RAM(addr) (*((volatile uint16 *) (addr)))
#define SessionKey RAM(0x200000C0UL)
#define AppFWVer   RAM(0x200000C2UL)
#define BootFWVer  RAM(0x200000C4UL)
#define HWVer      RAM(0x200000C6UL)
#define StatckOver RAM(0x200000C8UL)
#define UserRAM1   RAM(0x200000CAUL)
#define UserRAM2   RAM(0x200000CCUL)
#define UserRAM3   RAM(0x200000CEUL)
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
#if 0
/* Combine in the SetFlashLock_BootLock.hex. Don't open.
*  Address= 0x90400000
*  Boot size are 0x4000. flash protection is 1bit mapping 256Byts.
*  0x4000/256 = 64,  Total Flash size are (8*256*64)=131072=128KB
*/
#if defined(__GNUC__) || defined(__ARMCC_VERSION)
#ifndef CY_FLASH_PROT_SECTION
#define CY_FLASH_PROT_SECTION __attribute__ ((__section__(".cyflashprotect"), used))
#endif
CY_FLASH_PROT_SECTION
#elif defined(__ICCARM__)
#pragma  location=".cyflashprotect"
#else
#error "Unsupported toolchain"
#endif
/*Modify the flash protection structure as required*/
#if 1
const uint8_t cy_meta_flashprotect[] =
{
    0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u
};
#else
const uint8_t cy_meta_flashprotect[] =
{
    0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu
};
#endif
#endif


#if 0
/*  Combine in the SetFlashLock_BootLock.hex. Don't open.
* Warn : ***************************************************************
* Warn : * PSoC 4 device is in PROTECTED mode.
* Warn : * Press Erase Device to clear protection. All data will be lost!
* Warn : ***************************************************************
* Address= 0x90600000
*/
#if defined(__GNUC__) || defined(__ARMCC_VERSION)
#ifndef CY_CHIP_PROT_SECTION
#define CY_CHIP_PROT_SECTION __attribute__ ((__section__(".cychipprotect"), used))
#endif
CY_CHIP_PROT_SECTION
#elif defined(__ICCARM__)
#pragma  location=".cychipprotect"
#else
#error "Unsupported toolchain"
#endif

/*******************************************************************************
Open - 0x01  (factory default mode)
Protected - 0x02  (You can reset the mode to OPEN but only after you completely erase the flash.)
Kill - 0x03  (You may change the mode from OPEN to KILL.Note that the part cannot be taken out of KILL mode.)
*******************************************************************************/
const uint8_t cy_meta_chipprotect[] =
{
    0x02u
};
#endif
/* ==[ Variables with internal linkage ]==================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
static uint8_t SysPowerUpFlag;
static uint8_t SysWatchdogFlag;
static uint8_t SysExtResetFlag;
static uint8_t SysSwResetFlag;


/* ==[ Declaration of functions with internal linkage ]===================== */
static void Sys_GetRCAUSE(void);

/* ==[ Definition of functions with internal linkage ]====================== */
/*
*
*
* SRSS_RES_CAUSE(0x40030054)  =>  (((SRSSLT_Type *) SRSSLT)->RES_CAUSE)
* | Name                                        | Value
* |---------------------------|---------------------
* | CY_SYSLIB_RESET_HWWDT      | 0x00001 (bit0)
* | CY_SYSLIB_PROT_FAULT          | 0x00008 (bit3)
* | CY_SYSLIB_RESET_SOFT          | 0x00010 (bit4)
*
*
*  PWR_CONTROL(0x40030000)  => POWER_MODE [3:0]
*  0x0: RESET : RESET state
*  0x1: ACTIVE : ACTIVE state
*  0x2: SLEEP : SLEEP state
*  0x3: DEEP_SLEEP : DEEP_SLEEP state
*
*
*
*
*/
static void Sys_GetRCAUSE(void)
{
    uint32_t RCAUSE_RESET;
    /*uint32_t PWON_CAUSE*/
    /*uint16_t *RAM0 = (uint16_t *)0x200000C0UL*/

    /*Power on cause*/
    /*PWON_CAUSE = (((SRSSLT_Type *) SRSSLT)->PWR_DDFT_SELECT)*/
    /*PWON_CAUSE = SRSS_PWR_CONTROL & 0x0000000FU*/
    /* *RAM0 = (uint16_t)PWON_CAUSE*/
    /* Get Reset cause */
    RCAUSE_RESET = Cy_SysLib_GetResetReason();

    if((RCAUSE_RESET & CY_SYSLIB_RESET_HWWDT) > 0U)
    {
        SysWatchdogFlag = 1U;
    }
    else
    {
        SysWatchdogFlag = 0U;
    }

    if((RCAUSE_RESET & CY_SYSLIB_PROT_FAULT) > 0U)
    {
        SysExtResetFlag = 1U;
    }
    else
    {
        SysExtResetFlag = 0U;
    }

    if((RCAUSE_RESET & CY_SYSLIB_RESET_SOFT) > 0U)
    {
        SysSwResetFlag = 1U;
    }
    else
    {
        SysSwResetFlag = 0U;
    }

    if(RCAUSE_RESET == 0U)
    {
        SysPowerUpFlag = 1U;
    }
    else
    {
        SysPowerUpFlag = 0U;
    }

}


/* ==[ Definition of functions with external linkage ]======================= */
uint16_t Sys_GetAppFWVer(void)
{

    return AppFWVer;
}
uint16_t Sys_GetBootFWVer(void)
{
    return BootFWVer;
}
uint16_t Sys_GetHWVer(void)
{

    return HWVer;
}
uint16_t Sys_GetSessionKey(void)
{

    return SessionKey;
}

uint8_t Sys_GetBootReason(void)
{

    return (uint8_t)((SysPowerUpFlag << 4) | (SysWatchdogFlag << 2) | \
                     (SysExtResetFlag << 1) | SysSwResetFlag);
}

uint8_t Sys_GetUserRam(uint8_t RamData[], uint8_t RLen)
{
    int i;
    uint8_t realLen;
    volatile uint8_t *pRam = (uint8_t *)0x200000C0UL;

    if(RLen <= 16U)
    {
        realLen = RLen;

        for(i = 0; i < RLen; i++)
        {
            RamData[i] = *(pRam++);
        }
    }
    else
    {
        realLen = 0U;
    }

    return realLen;
}

t_FUNC_RETURN Sys_SetBootFWVer(uint16_t val)
{
    BootFWVer    = val;
    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Sys_SetSessionKey(uint16_t val)
{
    SessionKey   = val;
    return FUNC_RETURN_OK;
}



/*******************************************************************************
* \brief Function Name: Sys_VariableInit
********************************************************************************
* Summary:
* Global variable init.
*
*
* \Parameters [in] NONE
* \return          t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN Sys_VariableInit()
{
#if (LOT3_BOOT_ENABLE==1U)
    BootFWVer   = (uint16_t)((FW_BOOT_VER << 8U) | FW_BOOT_MINOR_VER);
#else
    AppFWVer    = (uint16_t)((FW_APP_VER << 8U) | FW_APP_MINOR_VER);
    HWVer       = (uint16_t)HW_VER;
    SessionKey  = (uint16_t)0U;


    if(StatckOver == 0xA55AU)
    {
        StatckOver = 0U;
    }

#endif
#if 0
    UserRAM1    = 0U;
    UserRAM2    = 0U;
    UserRAM3    = 0U;
    UserRAM4    = 0U;
#endif

    return FUNC_RETURN_OK;
}


/*******************************************************************************
* \brief Function Name: Sys_HwInit
********************************************************************************
* Summary:
* HW function init.
*
*
* \Parameters [in] NONE
* \return          t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN Sys_HwInit()
{
    (void)SWTimer_Init();    /*Don't move.*/

    /*(void)TCPWM5_1ms_Init()*/ /*Disable TCPWM5, and then enable Systick instead.*/
    (void)Systick_Init();
#if (SW_OR_HW_CRC == HW_CRC)
    (void)Crc_Init();
#endif
    (void)Gpio_Init();
    (void)I2CMA_Init();
    (void)I2cs_Init();

#if (LOT3_BOOT_ENABLE==0U)
    (void)Uart_Init();
    (void)Adc_Init();
    (void)I2CMB_Init();
    (void)SPIM_Init();
    (void)PWMCap_Init();
    (void)CanFd_Init();
    (void)UserFlash_Init(32U);

#if (SW_OR_HW_PWM == SEL_HW_PWM )
    /*HW PWM. The default frequence are 200Hz. */
    /*The pwm output pin is fix.*/
    (void)TCPWMO_Init(0, 200);
    (void)TCPWMO_Init(1, 200);
#elif (SW_OR_HW_PWM == SEL_SW_PWM )
    /*SW PWM. The default frequence are 200Hz. */
    /*The pwm output pin can user definable.*/
    (void)SWPWMO_FreqInit(200);
    (void)SWPWMO_PinInit(0, 0x64); /*port=6, pin=4*/
    (void)SWPWMO_SetDuty(0, 50);
#endif
#endif

    /*breath led init*/
    (void)BreathLED_Init(Breathe_LED);
    (void)Sys_GetRCAUSE();

    /* Enable global interrupts */
    __enable_irq();


    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    /*Cy_SCB_UART_PutString(mUART_HW, "Sys_HwInit\r\n")*/

    return FUNC_RETURN_OK;
}

/*******************************************************************************
* \brief Function Name: Sys_ProjectInit
********************************************************************************
* Summary:
* HW function init.
*
*
* \Parameters [in] NONE
* \return          t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN Sys_ProjectInit()
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
#if (LOT3_BOOT_ENABLE==1U)
    rtn_state = Boot_Init();
#elif (AGING_ENABLE==1U)
    rtn_state = Aging_Init();
#elif (LONGRUN_ENABLE==1U)
    rtn_state = LongRun_Init();
#elif (TESTING_ENABLE==1U)
    rtn_state = Testing_Init();
#elif (BRIDGE_ENABLE==1U) || (BRIDGE_UNIV_ENABLE==1U)
    (void)printf("IN APP MODE\r\n");
    rtn_state = Bridge_Init();
#endif

    return rtn_state;
}



/*******************************************************************************
* \brief Function Name: Sys_RoutineCall
********************************************************************************
* Summary:
* Routine call function.
*
*
* \Parameters [in] NONE
* \return          t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN Sys_RoutineCall()
{
    t_FUNC_RETURN rtn_state = FUNC_RETURN_OK;
#if (LOT3_BOOT_ENABLE==1U)
    rtn_state = Boot_RoutineCall();
#elif (AGING_ENABLE==1U)
    rtn_state = Aging_RoutineCall();
#elif (LONGRUN_ENABLE==1U)
    rtn_state = LongRun_RoutineCall();
#elif (TESTING_ENABLE==1U)
    rtn_state = Testing_RoutineCall();
#elif ((BRIDGE_ENABLE==1U) || (BRIDGE_UNIV_ENABLE==1U))
    rtn_state = Bridge_RoutineCall();
#endif

#if (LOT3_BOOT_ENABLE==0U)
    (void)I2cs_2StepRunProcess();
    (void)Button_RoutineCall(0x04U, 0xC0U); /*PORT4, MASK=0xC0*/
#endif
    return rtn_state;
}

/*******************************************************************************
* \brief Function Name: Sys_SoftReset
********************************************************************************
* Summary:
* MCU soft reset from CMSIS function.
*
*
* \Parameters [in] NONE
* \return          t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN Sys_SoftReset(void)
{
    uint8_t count = 0U;
    uint32_t i;

    (void)printf("Enter Reset\r\n");

    /* Disable global interrupts */
    __disable_irq();

    /* Check all interrupts disabled*/
    while(((__get_PRIMASK() & 0x01U) == 0U) && (count <= 10U))
    {
        count++;
    }

    /* Ensure all outstanding memory accesses included
        buffered write are completed before reset */
    __DSB();
    SCB->AIRCR = ((0x5FAUL << 16U) | (1UL << 2U));
    __DSB(); /* Ensure completion of memory access */


    for(i = 0U; i < 0xFFFFFFFFU; i++)                                                       /* wait until reset */
    {
        __NOP();
    }

    return FUNC_RETURN_OK;
}

/*******************************************************************************
* \brief Function Name: Sys_Uart_Print
********************************************************************************
*
* Summary:
*    pgo simu. uart.
*    9600,8,n,1
*    19200,8,n,1
* Parameters:
*  uint8_t *: debug string
*  uint32_t : string length
* Return:
*  None
*
*******************************************************************************/
void Sys_Uart_Print(char *str, uint32_t length)
{
    uint8_t char_temp;
    uint8_t debug_port_pin = (uint8_t)(DBG_UART);
    uint32_t i_str, i_char;
    uint32_t delay_us = 208u;  /*9600bps, 8,n,1,  4800=1/208u, 9600=1/104u, 19200=1/52u, 38400=1/26u*/

    for(i_str = 0u; i_str < length; i_str++)
    {
        char_temp = *str;

        /* idle */
#if 0
        Gpio_WritePin(debug_port, debug_pin, 1U);
        Cy_SysLib_DelayUs(delay_us);
#endif

        /* start bit */
        Gpio_WritePin(debug_port_pin, 0U);
        Cy_SysLib_DelayUs(delay_us);

        /* bit0~bit7 */
        for(i_char = 0u; i_char < 8u; i_char++)
        {
            if(((char_temp >> i_char) & 0x01u) == 0x01u)
            {
                Gpio_WritePin(debug_port_pin, 1U);
            }
            else
            {
                Gpio_WritePin(debug_port_pin, 0U);
            }

            Cy_SysLib_DelayUs(delay_us);
        }

        /* stop bit */
        Gpio_WritePin(debug_port_pin, 0U);
        Cy_SysLib_DelayUs(delay_us);

        /* idle */
        Gpio_WritePin(debug_port_pin, 1U);
        Cy_SysLib_DelayUs(delay_us);

        str++;
    }
}

/*******************************************************************************
* \brief Function Name: Sys_PsocSiliconID
********************************************************************************
* CPUSS_SYSARG register =>
* Bits [7:0] Silicon ID Lo
* Bits [15:8] Silicon ID Hi 2E00-2EFF
* Bits [19:16] Minor Revision Id See the CY8C4xxx, CYBLxxxx Programming Specifications
* Bits [23:20] Major Revision Id for these values.
* Bits [27:24] 0xXX Not used (don��t care)
* Bits [31:28] 0xA Success status code
*
* CPUSS_SYSREQ register =>
* Bits [11:0] Family ID Family ID is 0xBE for PSoC 4100S Max
* Bits [15:12] Chip Protection See the Device Security chapter on page 63.
* Bits [31:16] 0xXXXX Not used
*
* Parameters[inout]:  PsocID[]=>
* Return: t_FUNC_RETURN
*
*******************************************************************************/

t_FUNC_RETURN Sys_GetPsocSiliconID(uint8_t PsocID[])
{
    t_FUNC_RETURN rtn_status;
    uint32_t parameter[2];

    CPUSS_SYSARG = (uint32_t)(0xD3U << 8) | 0xB6U;
    CPUSS_SYSREQ = 0x80000000U;
    __NOP();
    __NOP();


    parameter[0] = CPUSS_SYSARG;
    parameter[1] = CPUSS_SYSREQ;

    if((parameter[0] & 0xA0000000U) == 0xA0000000U)
    {
        PsocID[0] = (uint8_t)parameter[0];      /*ID Lo*/
        PsocID[1] = (uint8_t)(parameter[0] >> 8); /*ID Hi*/
        PsocID[2] = (uint8_t)(parameter[0] >> 16); /*Major+Minor ID*/
        PsocID[3] = (uint8_t)(parameter[1]);    /*FamilyID=0xBE*/
        PsocID[4] = (uint8_t)(parameter[1] >> 12); /*Chip Protection state*/
        rtn_status = FUNC_RETURN_OK;
    }
    else
    {
        rtn_status = FUNC_RETURN_NG;
    }

    return rtn_status;
}


#if defined (__GNUC__)
/*******************************************************************************
 * \brief Function Name:
 *******************************************************************************
 * Summary:
 *  Redirect printf to UART
 *
 *
 *
 ******************************************************************************/
__attribute__((weak)) int32_t _write(int32_t file, char *ptr, int32_t len)
{
#if (EN_DEBUG==1U)
#if (LOT3_BOOT_ENABLE==1U)
    /*sw uart*/
    Sys_Uart_Print((char *)ptr, len);
#else
    /*hw uart*/
    (void)Uart_PutArray((uint8_t *)ptr, len);
#endif

    return len;
#else
    return -1;
#endif
}

#if 0
/*******************************************************************************
 * \brief Function Name: _read
 *******************************************************************************
 * Summary:
 *  Redirect scanf from UART
 *
 *
 *
 ******************************************************************************/
__attribute__((weak)) int32_t _read(int32_t file, char *ptr, int32_t len)
{    
    uint16_t length; 
    bool rtnState = Uart_GetArray((uint8_t *)ptr, &length);
    len = (int32_t)length;
    
    return (int32_t)rtnState;
}

#endif

/*
* \brief The callback function when stack overflow occurs.
*                     -fstack-protector-all
*   Rely on the function of gcc.
*   Store the state value in 0x200000C8UL(ram).
* \param[in] none.
* \return       none.
*/
#if (LOT3_BOOT_ENABLE==0U)
void *__stack_chk_guard = (void *)0xdeadbeefU;

__attribute__((optimize("O0"))) void __stack_chk_fail(void)
{
    StatckOver = 0xA55AU;
    Sys_SoftReset();
}
#endif

#endif

