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
#include <stdio.h>
#include "LongRun_GM3400.h"
#include "i2cm.h"
#include "i2cs.h"
#include "gpio.h"
#include "wdt.h"
#include "debounce.h"
#include "DevFPD983.h"
#include "DevFPD984.h"
#include "SoftwareTimer.h"
#include "gm_vcs.h"

#if (LONGRUN_ENABLE==1U) && (PROJECT == GM_VCS_3400)
/* ==[ Macros ]============================================================= */
#define BUTTON_PORT     4U
#define DIP_SWITCH_PORT 7U

#define serAddr 0x0C    //7bits   // 0x18      //8bits
#define desAddr0 0x2C   //7bits   // 0x58     //8bits
#define desAlias0 0x2C  //7bits   //0x58    //8bits

#define EN_UH983  0U
#define EN_HH983  1U
/* ==[ Types ]============================================================== */
/* ==[ Constants with internal linkage ]==================================== */
/* ==[ Variables with internal linkage ]==================================== */
static uint8_t ItemSwitch = 0U;
static uint16_t TestDimmPWM = 0xFFFFU;
static uint8_t TimeCtrlCnt = 0U;

static t_FUNC_RETURN INIT_FINSHED = FUNC_RETURN_OK;
#if 0
static t_FUNC_RETURN MCU_BUTTON1_HOLD = FUNC_RETURN_OK;
static t_FUNC_RETURN MCU_BUTTON2_HOLD = FUNC_RETURN_OK;
#endif

/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static void LongRun_BoardPower(uint8_t OnOff);
static void LongRun_ModulePower(uint8_t OnOff);
static void LongRun_ModuleWakeUp(uint8_t OnOff);
static t_FUNC_RETURN LongRun_FPDLink_Script(void);
static t_FUNC_RETURN LongRun_TestProcess(void);
/* ==[ Definition of functions with internal linkage ]====================== */
/*  \Brief Aging_BoardPower
*
*     LOT3 board power on or off control.
*
* \Parameters [in]   uint8_t  0:off, >0:on
* \return         void
*/
static void LongRun_BoardPower(uint8_t OnOff)
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
        Cy_SysLib_Delay(50U);
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

static void LongRun_ModulePower(uint8_t OnOff)
{
    //t_FUNC_RETURN rtn_state;
    if(OnOff > 0U)
    {
        /*Module Power On*/
        Gpio_WritePin(Panel_Vin_ON_Norm, 1U);
        Gpio_WritePin(Panel_Vin_ON_Quie, 0U);
        
        /*983+984 script*/
        (void)FPD983_FullRST();

#if 0

        if(rtn_state == FUNC_RETURN_OK)
        {
            LongRun_FPDLink_Script();
        }

#endif
    }
    else
    {
        /*Module Power Off*/
        Gpio_WritePin(Panel_Vin_ON_Norm, 0U);
    }

}

static void LongRun_ModuleWakeUp(uint8_t OnOff)
{
    if(OnOff > 0U)
    {
        /*Module wake On*/
        Gpio_WritePin(Panel_Vin_ON_Norm, 1U);
        Gpio_WritePin(Panel_Vin_ON_Quie, 0U);
        Gpio_WritePin(WAKE_FIDM, 1U);
    }
    else
    {
        /*Module wake Off*/
        Gpio_WritePin(WAKE_FIDM, 0U);
        Gpio_WritePin(Panel_Vin_ON_Quie, 1U);
        Gpio_WritePin(Panel_Vin_ON_Norm, 0U);
    }
}


/*
*
*     VCS34" FPDLINK 983 + 984 BIST Script.  VCS_34_CS3_983BIST_V5p3
*    Copy from C# script ==>
*    I2CMA_WriteB(A,B,C)   => I2CMA_WriteB(A,B,C)
*    I2CMA_ReadB(A,B)       => I2CMA_ReadB(A,B)
*
*/
static t_FUNC_RETURN LongRun_FPDLink_Script(void)
{
#if (EN_UH983==1)
    /*V2P7 983 BIST*/
    //// *********************************************
    //// Set up Variables
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x70, (desAddr0 << 1));
    I2CMA_WriteB(serAddr, 0x78, (desAlias0 << 1));
    I2CMA_WriteB(serAddr, 0x88, 0x0);
    I2CMA_WriteB(serAddr, 0x71, 0x24);  /*walker add to bypass mcu i2c.*/
    I2CMA_WriteB(serAddr, 0x79, 0x24);  /*walker add to bypass mcu i2c.*/
    I2CMA_WriteB(serAddr, 0x89, 0x0);   /*walker add to bypass mcu i2c.*/
    I2CMA_WriteB(serAddr, 0x72, 0x48);  /*walker add to bypass touch i2c.*/
    I2CMA_WriteB(serAddr, 0x7A, 0x48);  /*walker add to bypass touch i2c.*/
    I2CMA_WriteB(serAddr, 0x8A, 0x0);   /*walker add to bypass touch i2c.*/

    I2CMA_WriteB(serAddr, 0x3a, 0x88); //Disable remote controller on FPD port 0 and port 1 - comment out if (there is an I2C controller on the remote side

    //// *********************************************
    //// Enable I2C Passthrough
    //// *********************************************
    uint8_t I2C_PASS_THROUGH = I2CMA_ReadB(serAddr, 0x7);
    uint8_t I2C_PASS_THROUGH_MASK = 0x18;//I2C_PASS_ALL + I2C_PASS_THROUGH   //0x08; //I2C_PASS_THROUGH
    uint8_t I2C_PASS_THROUGH_REG = (uint8_t)(I2C_PASS_THROUGH | I2C_PASS_THROUGH_MASK);
    I2CMA_WriteB(serAddr, 0x07, I2C_PASS_THROUGH_REG); //Enable I2C Passthrough

    //// *********************************************
    //// Program SER to FPD-Link IV mode - Ensure 983 is strapped to FPD IV dual mode (MODE_SEL0 = 1, 3, or 5);
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x5b, 0x23); //Disable FPD3 FIFO pass through
    I2CMA_WriteB(serAddr, 0x5, 0x28); //Force FPD4_TX dual mode

    //// *********************************************
    //// Set up FPD IV PLL Settings - This section can be commented out to improve bringup time if 983/981 MODE_SEL0 and MODE_SEL2 are strapped to the correct FPD IV speed
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x40, 0x8); //Select PLL reg page
    I2CMA_WriteB(serAddr, 0x41, 0x1b);
    I2CMA_WriteB(serAddr, 0x42, 0x8); //Disable PLL0
    I2CMA_WriteB(serAddr, 0x41, 0x5b);
    I2CMA_WriteB(serAddr, 0x42, 0x8); //Disable PLL1
    I2CMA_WriteB(serAddr, 0x2, 0xd1); //Enable mode overwrite
    I2CMA_WriteB(serAddr, 0x2d, 0x1);
    I2CMA_WriteB(serAddr, 0x40, 0x8);   //Select PLL reg page
    I2CMA_WriteB(serAddr, 0x41, 0x5); //Select Ncount Reg
    I2CMA_WriteB(serAddr, 0x42, 0x74); //Set Ncount
    I2CMA_WriteB(serAddr, 0x41, 0x13); //Select post div reg
    I2CMA_WriteB(serAddr, 0x42, 0x80); //Set post div for 12.528 Gbps
    I2CMA_WriteB(serAddr, 0x2d, 0x1); //Select write reg to port 0
    I2CMA_WriteB(serAddr, 0x6a, 0x4a); //set BC sampling rate
    I2CMA_WriteB(serAddr, 0x6e, 0x80); //set BC fractional sampling
    I2CMA_WriteB(serAddr, 0x40, 0x4); //Select FPD page and set BC settings for FPD IV port 0
    I2CMA_WriteB(serAddr, 0x41, 0x6);
    I2CMA_WriteB(serAddr, 0x42, 0x0);
    I2CMA_WriteB(serAddr, 0x41, 0xd);
    I2CMA_WriteB(serAddr, 0x42, 0x34);
    I2CMA_WriteB(serAddr, 0x41, 0xe);
    I2CMA_WriteB(serAddr, 0x42, 0x53);
    I2CMA_WriteB(serAddr, 0x40, 0x8); //Select PLL reg page
    I2CMA_WriteB(serAddr, 0x41, 0x45); //Select Ncount Reg
    I2CMA_WriteB(serAddr, 0x42, 0x74); //Set Ncount
    I2CMA_WriteB(serAddr, 0x41, 0x53); //Select post div reg
    I2CMA_WriteB(serAddr, 0x42, 0x80); //Set post div for 12.528 Gbps
    I2CMA_WriteB(serAddr, 0x2d, 0x12); //Select write reg to port 1
    I2CMA_WriteB(serAddr, 0x6a, 0x4a); //set BC sampling rate
    I2CMA_WriteB(serAddr, 0x6e, 0x80); //set BC fractional sampling
    I2CMA_WriteB(serAddr, 0x40, 0x4);    //Select FPD page and set BC settings for FPD IV port 1
    I2CMA_WriteB(serAddr, 0x41, 0x26);
    I2CMA_WriteB(serAddr, 0x42, 0x0);
    I2CMA_WriteB(serAddr, 0x41, 0x2d);
    I2CMA_WriteB(serAddr, 0x42, 0x34);
    I2CMA_WriteB(serAddr, 0x41, 0x2e);
    I2CMA_WriteB(serAddr, 0x42, 0x53);
    I2CMA_WriteB(serAddr, 0x2, 0xd1); //Set HALFRATE_MODE
    I2CMA_WriteB(serAddr, 0x2d, 0x1);
    //// *********************************************
    //// Zero out PLL fractional - This section can be commented out to improve bringup time if 983/981 MODE_SEL0 and MODE_SEL2 are strapped to the correct FPD IV speed
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x40, 0x8); //Select PLL reg page  //copy from 35"m
    I2CMA_WriteB(serAddr, 0x41, 0x4);
    I2CMA_WriteB(serAddr, 0x42, 0x1);
    I2CMA_WriteB(serAddr, 0x41, 0x1e);
    I2CMA_WriteB(serAddr, 0x42, 0x0);
    I2CMA_WriteB(serAddr, 0x41, 0x1f);
    I2CMA_WriteB(serAddr, 0x42, 0x0);
    I2CMA_WriteB(serAddr, 0x41, 0x20);
    I2CMA_WriteB(serAddr, 0x42, 0x0);
    I2CMA_WriteB(serAddr, 0x41, 0x44);
    I2CMA_WriteB(serAddr, 0x42, 0x1);
    I2CMA_WriteB(serAddr, 0x41, 0x5e);
    I2CMA_WriteB(serAddr, 0x42, 0x0);
    I2CMA_WriteB(serAddr, 0x41, 0x5f);
    I2CMA_WriteB(serAddr, 0x42, 0x0);
    I2CMA_WriteB(serAddr, 0x41, 0x60);
    I2CMA_WriteB(serAddr, 0x42, 0x0);
    //// *********************************************
    //// Configure and Enable PLLs - This section can be commented out to improve bringup time if 983/981 MODE_SEL0 and MODE_SEL2 are strapped to the correct FPD IV speed
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x41, 0xe); //Select VCO reg
    I2CMA_WriteB(serAddr, 0x42, 0xc7); //Set VCO
    I2CMA_WriteB(serAddr, 0x41, 0x4e); //Select VCO reg
    I2CMA_WriteB(serAddr, 0x42, 0xc7); //Set VCO
    I2CMA_WriteB(serAddr, 0x40, 0x8); //Select PLL page
    I2CMA_WriteB(serAddr, 0x41, 0x1b);
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Enable PLL0
    I2CMA_WriteB(serAddr, 0x41, 0x5b);
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Enable PLL1
    ////I2CMA_WriteB(serAddr, 0x1, 0x1); //Soft reset Ser
    I2CMA_WriteB(serAddr, 0x1, 0x30); //Soft reset Ser PLLs /**Don't open.Unexpected black screen,**/
    Cy_SysLib_Delay(60);  //Cy_SysLib_Delay(20);


    //*********************************************
    // Configure Serializer TX Link Layer
    // *********************************************
    I2CMA_WriteB(serAddr, 0x40, 0x2e);  //Indirect TX Link layer Reg page
    I2CMA_WriteB(serAddr, 0x41, 0x1);  //Link layer 0 stream enable
    I2CMA_WriteB(serAddr, 0x42, 0x3);  //Link layer 0 stream enable
    I2CMA_WriteB(serAddr, 0x41, 0x6);  //Link layer 0 time slot 0
    I2CMA_WriteB(serAddr, 0x42, 0x20);  //Link layer 0 time slot 0
    I2CMA_WriteB(serAddr, 0x41, 0x7);  //Link layer 0 time slot 1
    I2CMA_WriteB(serAddr, 0x42, 0x20);  //Link layer 0 time slot 1
    I2CMA_WriteB(serAddr, 0x41, 0x20);  //Set Link layer vp bpp
    I2CMA_WriteB(serAddr, 0x42, 0x5a);  //Set Link layer vp bpp according to VP Bit per pixel
    I2CMA_WriteB(serAddr, 0x41, 0x0);  //Link layer 0 enable
    I2CMA_WriteB(serAddr, 0x42, 0x3);  //Link layer 0 enable

    //// *********************************************
    //// Program VP Configs
    //// *********************************************
    // Configure VP 0
    I2CMA_WriteB(serAddr, 0x40, 0x32); //Page12
    I2CMA_WriteB(serAddr, 0x41, 0x1);
    I2CMA_WriteB(serAddr, 0x42, 0xa8); //Set VP_SRC_SELECT to Stream 0 for SST Mode
    I2CMA_WriteB(serAddr, 0x41, 0x2);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //VID H Active
    I2CMA_WriteB(serAddr, 0x42, 0x11); //VID H Active
    I2CMA_WriteB(serAddr, 0x41, 0x8);
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start X
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start X
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start Y
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start Y
    I2CMA_WriteB(serAddr, 0x42, 0x7f); //Crop Stop X
    I2CMA_WriteB(serAddr, 0x42, 0x11); //Crop Stop X
    I2CMA_WriteB(serAddr, 0x42, 0x27); //Crop Stop Y
    I2CMA_WriteB(serAddr, 0x42, 0x5); //Crop Stop Y
    I2CMA_WriteB(serAddr, 0x41, 0x10);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //Horizontal Active
    I2CMA_WriteB(serAddr, 0x42, 0x11); //Horizontal Active
    I2CMA_WriteB(serAddr, 0x42, 0x50); //Horizontal Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Horizontal Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Horizontal Sync
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Horizontal Sync
    I2CMA_WriteB(serAddr, 0x42, 0x48); //Horizontal Total
    I2CMA_WriteB(serAddr, 0x42, 0x12); //Horizontal Total
    //I2CMA_WriteB(serAddr, 0x42, 0x2e); //Horizontal Total  //for test
    //I2CMA_WriteB(serAddr, 0x42, 0x1e); //Horizontal Total  //for test
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Active
    I2CMA_WriteB(serAddr, 0x42, 0x5); //Vertical Active
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x8); //Vertical Sync
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Sync
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Front Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Front Porch
    I2CMA_WriteB(serAddr, 0x41, 0x27);
    I2CMA_WriteB(serAddr, 0x42, 0x0); //HSYNC Polarity = +, VSYNC Polarity = +
    I2CMA_WriteB(serAddr, 0x41, 0x0);
    I2CMA_WriteB(serAddr, 0x42, 0x4); //Enable Cropping
    I2CMA_WriteB(serAddr, 0x41, 0x23); //M/N Register
    I2CMA_WriteB(serAddr, 0x42, 0x65); //M value
    I2CMA_WriteB(serAddr, 0x42, 0x28); //M value
    I2CMA_WriteB(serAddr, 0x42, 0xf); //N value

    // Configure VP 1
    I2CMA_WriteB(serAddr, 0x40, 0x32); //Page12
    I2CMA_WriteB(serAddr, 0x41, 0x41);
    I2CMA_WriteB(serAddr, 0x42, 0xa8); //Set VP_SRC_SELECT to Stream 0 for SST Mode
    I2CMA_WriteB(serAddr, 0x41, 0x42);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //VID H Active
    I2CMA_WriteB(serAddr, 0x42, 0x11); //VID H Active
    I2CMA_WriteB(serAddr, 0x41, 0x48);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //Crop Start X
    I2CMA_WriteB(serAddr, 0x42, 0x11); //Crop Start X
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start Y
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start Y
    I2CMA_WriteB(serAddr, 0x42, 0xff); //Crop Stop X
    I2CMA_WriteB(serAddr, 0x42, 0x22); //Crop Stop X
    I2CMA_WriteB(serAddr, 0x42, 0x27); //Crop Stop Y
    I2CMA_WriteB(serAddr, 0x42, 0x5); //Crop Stop Y
    I2CMA_WriteB(serAddr, 0x41, 0x50);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //Horizontal Active
    I2CMA_WriteB(serAddr, 0x42, 0x11); //Horizontal Active
    I2CMA_WriteB(serAddr, 0x42, 0x50); //Horizontal Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Horizontal Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Horizontal Sync
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Horizontal Sync
    I2CMA_WriteB(serAddr, 0x42, 0x48); //Horizontal Total
    I2CMA_WriteB(serAddr, 0x42, 0x12); //Horizontal Total
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Active
    I2CMA_WriteB(serAddr, 0x42, 0x5); //Vertical Active
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x8); //Vertical Sync
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Sync
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Front Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Front Porch
    I2CMA_WriteB(serAddr, 0x41, 0x67);
    I2CMA_WriteB(serAddr, 0x42, 0x0); //HSYNC Polarity = +, VSYNC Polarity = +
    I2CMA_WriteB(serAddr, 0x41, 0x40);
    I2CMA_WriteB(serAddr, 0x42, 0x4); //Enable Cropping
    I2CMA_WriteB(serAddr, 0x41, 0x63); //M/N Register
    I2CMA_WriteB(serAddr, 0x42, 0x65); //M value
    I2CMA_WriteB(serAddr, 0x42, 0x28); //M value
    I2CMA_WriteB(serAddr, 0x42, 0xf); //N value

    //// *********************************************
    //// Enable PATGEN
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x40, 0x30);
    I2CMA_WriteB(serAddr, 0x41, 0x29);
    I2CMA_WriteB(serAddr, 0x42, 0x10); //Set PATGEN Color Depth to 30bpp for VP0
    I2CMA_WriteB(serAddr, 0x41, 0x28);
    I2CMA_WriteB(serAddr, 0x42, 0x95); //Enable PATGEN on VP0 - Comment out this line to disable PATGEN and enable end to end video
    I2CMA_WriteB(serAddr, 0x41, 0x69);
    I2CMA_WriteB(serAddr, 0x42, 0x10); //Set PATGEN Color Depth to 30bpp for VP1
    I2CMA_WriteB(serAddr, 0x41, 0x68);

    ////FPD_LINK983.SetGrayLevel(serAddr, 0, 1023,1023);  //walker change color.

    I2CMA_WriteB(serAddr, 0x42, 0x95); //Enable PATGEN on VP1 - Comment out this line to disable PATGEN and enable end to end video
    I2CMA_WriteB(serAddr, 0x1, 0x30); //Reset Ser TX-PLLs
    Cy_SysLib_Delay(60);

    //// *********************************************
    //// Enable VPs
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x43, 0x1); //Set number of VPs used = 2
    I2CMA_WriteB(serAddr, 0x44, 0x3); //Enable video processors

    // *************************************************
    // Confirm that 984 is alive
    // *************************************************
    //check 984 ID  //Confirm that 984 is alive
    uint8_t readData = 0, ErrCnt = 0;

    while(readData != 0x58)
    {
        ErrCnt++;
        Cy_SysLib_Delay(40);
        readData = I2CMA_ReadB(desAlias0, 0x0);

        if(readData != 0x58)
        {
            //Debuggings.MainFormShowMessage("Waiting for 984 connection count =" + ErrCnt.ToString(), Debuggings.HIGH);
            if(ErrCnt >= 30)
            {
                //Debuggings.MainFormShowMessage("984 Connection failed.", Debuggings.HIGH);
                return FUNC_RETURN_NG;
            }
        }
    }


    //// *********************************************
    //// Set DP Sink in sleep mode - DPTX Port 1
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x9); //Enable DPTX1 APB interface to Write//walker modify
    I2CMA_WriteB(desAlias0, 0x49, 0x8); //Set aux address to 0x600
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x6);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0x4); //Set auxWriteFIFO data to 0x002(sleep command)
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x2);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    ////Cy_SysLib_Delay(100);                //LOGAN agrees to delete this line.

    I2CMA_WriteB(desAlias0, 0x49, 0x0); //Set auxWriteFIFO data to 0x002
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x8);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    //// *********************************************
    //// Set DP Sink in sleep mode - DPTX Port 0
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x1); //Enable DPTX0 APB interface to Write
    I2CMA_WriteB(desAlias0, 0x49, 0x8); //Set aux address to 0x600
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x6);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0x4); //Set auxWriteFIFO data to 0x002(sleep command)
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x2);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    ////Cy_SysLib_Delay(100);                //LOGAN agrees to delete this line.

    I2CMA_WriteB(desAlias0, 0x49, 0x0); //Set auxWriteFIFO data to 0x002
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x8);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    // *************************************************
    //
    // *************************************************
    ////full reset
    I2CMA_WriteB(desAlias0, 0x1, 0x2); //984 all reset  //*************************************//

    //check 984 ID  //Confirm that 984 is alive
    readData = 0;
    ErrCnt = 0;

    while(readData != 0x58)
    {
        ErrCnt++;
        Cy_SysLib_Delay(40);
        readData = I2CMA_ReadB(desAlias0, 0x0);

        if(readData != 0x58)
        {
            //Debuggings.MainFormShowMessage("Waiting for 984 connection count =" + ErrCnt.ToString(), Debuggings.HIGH);
            if(ErrCnt >= 30)
            {
                //Debuggings.MainFormShowMessage("984 Connection failed.", Debuggings.HIGH);
                return FUNC_RETURN_NG;
            }
        }
    }

    /**/
    //I2CMA_WriteB(desAlias0, 0x04, 0xDD);  /*I2C bypass all.*/


    //// *********************************************
    //// Read Deserializer 0 Temp
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x40, 0x6c);
    I2CMA_WriteB(desAlias0, 0x41, 0xd);
    I2CMA_WriteB(desAlias0, 0x42, 0x0);
    I2CMA_WriteB(desAlias0, 0x41, 0x13);
    uint8_t TEMP_FINAL = I2CMA_ReadB(desAlias0, 0x42);
    int TEMP_FINAL_C = (2 * TEMP_FINAL - 273);
    ////Debuggings.MainFormShowMessage("TEMP_FINAL_C = " + TEMP_FINAL_C.ToString(), Debuggings.HIGH);

    //// *********************************************
    //// Set up Deserializer 0 Temp Ramp Optimizations
    //// *********************************************
    int Efuse_TS_CODE = 2;
    int Ramp_UP_Range_CODES_Needed = (int)((150 - TEMP_FINAL_C) / (190 / 11)) + 1;
    int Ramp_DN_Range_CODES_Needed = (int)((TEMP_FINAL_C - 30) / (190 / 11)) + 1;
    int Ramp_UP_CAP_DELTA = Ramp_UP_Range_CODES_Needed - 4;
    int Ramp_DN_CAP_DELTA = Ramp_DN_Range_CODES_Needed - 7;
    uint8_t rb = 0;
    int TS_CODE_UP = 0;
    int TS_CODE_DN = 0;

    I2CMA_WriteB(desAlias0, 0x40, 0x3c);
    I2CMA_WriteB(desAlias0, 0x41, 0xf5);
    I2CMA_WriteB(desAlias0, 0x42, (uint8_t)((Efuse_TS_CODE << 4) + 1)); // Override TS_CODE Efuse Code

    if(Ramp_UP_CAP_DELTA > 0)
    {
        TS_CODE_UP = Efuse_TS_CODE - Ramp_UP_CAP_DELTA;

        if(TS_CODE_UP < 0)
        {
            TS_CODE_UP = 0;
        }

        I2CMA_WriteB(desAlias0, 0x41, 0xf5);
        rb = I2CMA_ReadB(desAlias0, 0x42);
        rb &= 0x8F;
        rb |= (uint8_t)(TS_CODE_UP << 4);
        I2CMA_WriteB(desAlias0, 0x42, rb);
        rb = I2CMA_ReadB(desAlias0, 0x42);
        rb &= 0xFE;
        rb |= 0x01;
        I2CMA_WriteB(desAlias0, 0x42, rb);

        I2CMA_WriteB(desAlias0, 0x1, 0x1);  //*************************************//
        Cy_SysLib_Delay(60);                     //*************************************//
    }

    //Debuggings.MainFormShowMessage("Ramp_UP_CAP_DELTA=" + Ramp_UP_CAP_DELTA.ToString() + ", TS_CODE_UP=" + TS_CODE_UP.ToString() + ", rb=" + rb.ToString(), Debuggings.HIGH);

    if(Ramp_DN_CAP_DELTA > 0)   /*Ramp_DN_CAP_DELTA will not execute*/
    {
        TS_CODE_DN = Efuse_TS_CODE + Ramp_DN_CAP_DELTA;

        if(TS_CODE_DN >= 7)
        {
            TS_CODE_DN = 7;
        }

        I2CMA_WriteB(desAlias0, 0x41, 0xf5);
        rb = I2CMA_ReadB(desAlias0, 0x42);
        rb &= 0x8F;
        rb |= (uint8_t)(TS_CODE_DN << 4);
        I2CMA_WriteB(desAlias0, 0x42, rb);
        rb = I2CMA_ReadB(desAlias0, 0x42);
        rb &= 0xFE;
        rb |= 0x01;
        I2CMA_WriteB(desAlias0, 0x42, rb);
        I2CMA_WriteB(desAlias0, 0x1, 0x1);  //*************************************//
        Cy_SysLib_Delay(60);                     //*************************************//
    }

    //Debuggings.MainFormShowMessage("Ramp_DN_CAP_DELTA=" + Ramp_DN_CAP_DELTA.ToString() + ", TS_CODE_DN=" + TS_CODE_DN.ToString() + ", rb=" + rb.ToString(), Debuggings.HIGH);

    //// *********************************************
    //// Clear CRC errors from initial link process
    //// *********************************************
    uint8_t Reg_value = I2CMA_ReadB(serAddr, 0x2);
    Reg_value = (uint8_t)(Reg_value | 0x20);
    I2CMA_WriteB(serAddr, 0x2, Reg_value); //CRC Error Reset

    Reg_value = I2CMA_ReadB(serAddr, 0x2);
    Reg_value = (uint8_t)(Reg_value & 0xdf);
    I2CMA_WriteB(serAddr, 0x2, Reg_value); //CRC Error Reset Clear

    //// *********************************************
    //// Hold Des DTG in reset
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x40, 0x50); //Select DTG Page
    I2CMA_WriteB(desAlias0, 0x41, 0x32);
    I2CMA_WriteB(desAlias0, 0x42, 0x6); //Hold Port 0 DTG in reset
    I2CMA_WriteB(desAlias0, 0x41, 0x62);
    I2CMA_WriteB(desAlias0, 0x42, 0x6); //Hold Port 1 DTG in reset

    // *********************************************
    // Disable Stream Mapping
    // *********************************************
    I2CMA_WriteB(desAlias0, 0xe, 0x3); //Select both Output Ports
    I2CMA_WriteB(desAlias0, 0xd0, 0x0); //Disable FPD4 video forward to Output Port
    I2CMA_WriteB(desAlias0, 0xd7, 0x0); //Disable FPD3 video forward to Output Port

    // *********************************************
    // Force DP Rate
    // *********************************************
    I2CMA_WriteB(desAlias0, 0x40, 0x2c); //Select DP Page
    I2CMA_WriteB(desAlias0, 0x41, 0x81);
    I2CMA_WriteB(desAlias0, 0x42, 0xc0); //Set DP Rate to 5.4Gbps
    I2CMA_WriteB(desAlias0, 0x41, 0x82);
    I2CMA_WriteB(desAlias0, 0x42, 0x3); //Enable force DP rate & disable calibration of DP port
    I2CMA_WriteB(desAlias0, 0x41, 0x91);
    I2CMA_WriteB(desAlias0, 0x42, 0xc); //Force 4 lanes
    I2CMA_WriteB(desAlias0, 0x40, 0x30); //Disable DP SSCG
    I2CMA_WriteB(desAlias0, 0x41, 0xf);
    I2CMA_WriteB(desAlias0, 0x42, 0x1);
    I2CMA_WriteB(desAlias0, 0x1, 0x40);  //DPTX-PLLs reset  //*************************************//

    // *********************************************
    // Setup DP ports
    // *********************************************
    // Both Ports enabled


    // *********************************************
    // Program quad pixel clock for DP port 0
    // *********************************************
    I2CMA_WriteB(desAlias0, 0xe, 0x1); //Select Port0 registers
    I2CMA_WriteB(desAlias0, 0xb1, 0x1); //Enable clock divider
    I2CMA_WriteB(desAlias0, 0xb2, 0x67); //Program M value lower uint8_t
    I2CMA_WriteB(desAlias0, 0xb3, 0x8); //Program M value middle uint8_t
    I2CMA_WriteB(desAlias0, 0xb4, 0x6); //Program M value upper uint8_t
    I2CMA_WriteB(desAlias0, 0xb5, 0x80); //Program N value lower uint8_t
    I2CMA_WriteB(desAlias0, 0xb6, 0xf5); //Program N value middle uint8_t
    I2CMA_WriteB(desAlias0, 0xb7, 0x20); //Program N value upper uint8_t
    I2CMA_WriteB(desAlias0, 0xe, 0x1); //Select Port 0 registers

    // *********************************************
    // Setup DTG for port 0
    // *********************************************
    I2CMA_WriteB(desAlias0, 0x40, 0x50); //Select DTG Page
    I2CMA_WriteB(desAlias0, 0x41, 0x20);
    I2CMA_WriteB(desAlias0, 0x42, 0xa3); //Set up DTG BPP, Sync Polarities, and Measurement Type
    I2CMA_WriteB(desAlias0, 0x41, 0x29); //Set Hstart
    I2CMA_WriteB(desAlias0, 0x42, 0x80); //Hstart upper uint8_t
    I2CMA_WriteB(desAlias0, 0x41, 0x2a);
    I2CMA_WriteB(desAlias0, 0x42, 0x78); //Hstart lower uint8_t
    I2CMA_WriteB(desAlias0, 0x41, 0x2f); //Set HSW
    I2CMA_WriteB(desAlias0, 0x42, 0x40); //HSW upper uint8_t
    I2CMA_WriteB(desAlias0, 0x41, 0x30);
    I2CMA_WriteB(desAlias0, 0x42, 0x28); //HSW lower uint8_t


    //// *********************************************
    //// Program DPTX for DP port 0
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x1); //Enable DPTX0 APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0xa4); //Set bit per color
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x40);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb8); //Set pixel width
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x4);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xac); //Set DP Mvid
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0xb7);
    I2CMA_WriteB(desAlias0, 0x4c, 0x5d);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb4); //Set DP Nvid
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x80);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc8); //Set TU Mode
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb0); //Set TU Size
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x40);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x2c);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc8); //Set FIFO Size
    I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    I2CMA_WriteB(desAlias0, 0x4b, 0x6);
    I2CMA_WriteB(desAlias0, 0x4c, 0x40);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xbc); //Set data count
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x68);
    I2CMA_WriteB(desAlias0, 0x4c, 0x10);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc0); //Disable STREAM INTERLACED
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc4); //Set SYNC polarity
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0xc);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);


    // *********************************************
    // Program quad pixel clock for DP port 1
    // *********************************************
    I2CMA_WriteB(desAlias0, 0xe, 0x12); //Select Port1 registers
    I2CMA_WriteB(desAlias0, 0xb1, 0x1); //Enable clock divider
    I2CMA_WriteB(desAlias0, 0xb2, 0x67); //Program M value lower uint8_t
    I2CMA_WriteB(desAlias0, 0xb3, 0x8); //Program M value middle uint8_t
    I2CMA_WriteB(desAlias0, 0xb4, 0x6); //Program M value upper uint8_t
    I2CMA_WriteB(desAlias0, 0xb5, 0x80); //Program N value lower uint8_t
    I2CMA_WriteB(desAlias0, 0xb6, 0xf5); //Program N value middle uint8_t
    I2CMA_WriteB(desAlias0, 0xb7, 0x20); //Program N value upper uint8_t
    I2CMA_WriteB(desAlias0, 0xe, 0x1); //Select Port 0 registers

    //// *********************************************
    //// Setup DTG port 1 on DES0
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x40, 0x50); //Select DTG Page
    I2CMA_WriteB(desAlias0, 0x41, 0x50);
    I2CMA_WriteB(desAlias0, 0x42, 0xa3); //Set up DTG BPP, Sync Polarities, and Measurement Type
    I2CMA_WriteB(desAlias0, 0x41, 0x59); //Set Hstart
    I2CMA_WriteB(desAlias0, 0x42, 0x80); //Hstart upper uint8_t
    I2CMA_WriteB(desAlias0, 0x41, 0x5a);
    I2CMA_WriteB(desAlias0, 0x42, 0x78); //Hstart lower uint8_t
    I2CMA_WriteB(desAlias0, 0x41, 0x5f); //Set HSW
    I2CMA_WriteB(desAlias0, 0x42, 0x40); //HSW upper uint8_t
    I2CMA_WriteB(desAlias0, 0x41, 0x60);
    I2CMA_WriteB(desAlias0, 0x42, 0x28); //HSW lower uint8_t

    //// *********************************************
    //// Program DPTX for DP port 1
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x9); //Enable DPTX1 APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0xa4); //Set bit per color
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x40); //10bits dp
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb8); //Set pixel width
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x4);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xac); //Set DP Mvid
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0xb7);
    I2CMA_WriteB(desAlias0, 0x4c, 0x5d);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb4); //Set DP Nvid
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x80);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc8); //Set TU Mode
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb0); //Set TU Size
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x40); //10bits dp
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x2c);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc8); //Set FIFO Size
    I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    I2CMA_WriteB(desAlias0, 0x4b, 0x6);
    I2CMA_WriteB(desAlias0, 0x4c, 0x40);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xbc); //Set data count
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x68);
    I2CMA_WriteB(desAlias0, 0x4c, 0x10);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc0); //Disable STREAM INTERLACED
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc4); //Set SYNC polarity
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0xc);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    // *********************************************
    // Map video to display output
    // *********************************************
    I2CMA_WriteB(desAlias0, 0xe, 0x3); //Select both Output Ports
    I2CMA_WriteB(desAlias0, 0xd0, 0xc); //Enable FPD_RX video forward to Output Port
    I2CMA_WriteB(desAlias0, 0xd1, 0xf); //Every stream forwarded on DC
    //I2CMA_WriteB(desAlias0, 0xd6, 0x8); //Send Stream 0 to Output Port 0 and Send Stream 1 to Output Port 1
    I2CMA_WriteB(desAlias0, 0xd6, 0x1); //Send Stream 1 to Output Port 0 and Send Stream 0 to Output Port 1
    I2CMA_WriteB(desAlias0, 0xd7, 0x0); //FPD3 mapping disabled
    I2CMA_WriteB(desAlias0, 0xe, 0x01); //Select Port 0

    //// *********************************************
    //// Send AUX Wake Command to DP Sink - DPTX Port 1
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x9); //Enable DPTX1 APB interface to Write//walker modify
    I2CMA_WriteB(desAlias0, 0x49, 0x8); //Set aux address to 0x600(Setup aux_addr)
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x6);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0x4); //Set auxWriteFIFO data to 0x001(wakeup command)
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x1);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    ////Cy_SysLib_Delay(100);                //LOGAN agrees to delete this line.

    I2CMA_WriteB(desAlias0, 0x49, 0x0); //Set auxCommand to write(0x800)
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x8);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    //// *********************************************
    //// Send AUX Wake Command to DP Sink - DPTX Port 0
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x1); //Enable DPTX0 APB interface to Write
    I2CMA_WriteB(desAlias0, 0x49, 0x8); //Set aux address to 0x600(Setup aux_addr)
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x6);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0x4); //Set auxWriteFIFO data to 0x001(wakeup command)
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x1);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    ////Cy_SysLib_Delay(100);                //LOGAN agrees to delete this line.

    I2CMA_WriteB(desAlias0, 0x49, 0x0); //Set auxCommand to write(0x800)
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x8);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    ////I2CMA_WriteB(desAlias0, 0x1, 0x40); //Renew Link Training  //*************************************//

    //// *********************************************
    //// Release Des DTG reset
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x40, 0x50); //Select DTG Page
    I2CMA_WriteB(desAlias0, 0x41, 0x32);
    I2CMA_WriteB(desAlias0, 0x42, 0x4); //Release Port 0 DTG
    I2CMA_WriteB(desAlias0, 0x41, 0x62);
    I2CMA_WriteB(desAlias0, 0x42, 0x4); //Release Port 1 DTG

    I2CMA_WriteB(desAlias0, 0x48, 0x1); //Enable DPTX0 APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0x80); //Set DPTX Port0 Htotal
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x48);
    I2CMA_WriteB(desAlias0, 0x4c, 0x12);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    //mark to sync Alex's V2p7 script
    ////I2CMA_WriteB(desAlias0, 0x49, 0x90); //DPTX Port0 Core Reset
    ////I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    ////I2CMA_WriteB(desAlias0, 0x4b, 0x1);
    ////I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    ////I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    ////I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x48, 0x9);  //Enable DPTX1 APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0x80); //Set DPTX Port1 Htotal
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x48);
    I2CMA_WriteB(desAlias0, 0x4c, 0x12);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    //mark to sync Alex's V2p7 script
    ////I2CMA_WriteB(desAlias0, 0x49, 0x90); //DPTX Port1 Core Reset
    ////I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    ////I2CMA_WriteB(desAlias0, 0x4b, 0x1);
    ////I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    ////I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    ////I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x1, 0x1);  //*************************************//
    ////Cy_SysLib_Delay(60);  //Add delay to avoid FIFO-OV //85degree >=60
    readData = 0;
    ErrCnt = 0;

    while(readData != 0x58)
    {
        ErrCnt++;
        Cy_SysLib_Delay(40);
        readData = I2CMA_ReadB(desAlias0, 0x0);

        if(readData != 0x58)
        {
            //Debuggings.MainFormShowMessage("Waiting for 984 connection count =" + ErrCnt.ToString(), Debuggings.HIGH);
            if(ErrCnt >= 30)
            {
                //Debuggings.MainFormShowMessage("984 Connection failed.", Debuggings.HIGH);
                return FUNC_RETURN_NG;
            }
        }
    }

    // *********************************************
    // Enable DP 0 output
    // *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x1); //Enable DPTX0 APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0x84); //Enable DP output
    I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    I2CMA_WriteB(desAlias0, 0x4b, 0x1);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    // *********************************************
    // Enable DP 1 output
    // *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x9); //Enable DPTX1 APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0x84); //Enable DP output
    I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    I2CMA_WriteB(desAlias0, 0x4b, 0x1);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    /*===CarUx setting====== */
    /* setting I2C speed*/
    FPD984_I2c_1Mhz();

    /*setting 983 gpio init*/
    I2CMA_WriteB(serAddr, 0x17, 0x80); //GPIO0_PIN_CTL Register ////0000000b = Received PORT0 BC_GPIO0
    I2CMA_WriteB(serAddr, 0x18, 0x81); //GPIO1_PIN_CTL Register ////0000001b = Received PORT0 BC_GPIO1
    I2CMA_WriteB(serAddr, 0x28, 0x00); //GPIO_EN_BC Register    //BC enable 4 gpio
    I2CMA_WriteB(serAddr, 0x1d, 0x89); //GPIO6_PIN_CTL          //PORT0 RX_LOCK_DET
    /*setting 984 gpio init*/
    //GPIO0(IN):TP_IN,  GPIOI(IN):IRQ, GPIO6:LOCK
    I2CMA_WriteB(desAlias0, 0x13, 0x0F); //GPIO_IN_EN_LOW //GPIO0,1,2,3 are input.
    I2CMA_WriteB(desAlias0, 0x15, 0x00);  //GPIO0_PIN_CTL //input
    I2CMA_WriteB(desAlias0, 0x16, 0x00);  //GPIO1_PIN_CTL //input
    I2CMA_WriteB(desAlias0, 0x1B, 0xC2);  //GPIO6_PIN_CTL //output

    /*Determines the data sent on GPIO0 and GPIO1 for the selected port back channel.*/
    /*GPIO0 and GPIO1:RX input. TX output.*/
    I2CMA_WriteB(desAlias0, 0x40, 0x04); //select PAGE1(main page2)
    I2CMA_WriteB(desAlias0, 0x41, 0x10); //BC_GPIO_CTL0 Register
    I2CMA_WriteB(desAlias0, 0x42, 0x00);
    I2CMA_WriteB(desAlias0, 0x41, 0x11); //BC_GPIO_CTL1 Register
    I2CMA_WriteB(desAlias0, 0x42, 0x01);
    I2CMA_WriteB(desAlias0, 0x41, 0x20); //BC_CONFIG Register
    I2CMA_WriteB(desAlias0, 0x42, 0x00);

    /*clear 984 CRC and ECC error count*/
    FPD984_CLEAR_CRCECC(0);
    FPD984_CLEAR_CRCECC(1);

#endif
#if (EN_HH983==1)
    printf("EN_HH983: Script\r\n");
    
    //// *********************************************
    //// Set up Variables
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x70, (desAddr0 << 1));
    I2CMA_WriteB(serAddr, 0x78, (desAlias0 << 1));
    I2CMA_WriteB(serAddr, 0x88, 0x0);
    I2CMA_WriteB(serAddr, 0x71, 0x24);  /*walker add to bypass mcu i2c.*/
    I2CMA_WriteB(serAddr, 0x79, 0x24);  /*walker add to bypass mcu i2c.*/
    I2CMA_WriteB(serAddr, 0x89, 0x0);   /*walker add to bypass mcu i2c.*/
    I2CMA_WriteB(serAddr, 0x72, 0x48);  /*walker add to bypass touch i2c.*/
    I2CMA_WriteB(serAddr, 0x7A, 0x48);  /*walker add to bypass touch i2c.*/
    I2CMA_WriteB(serAddr, 0x8A, 0x0);   /*walker add to bypass touch i2c.*/
    I2CMA_WriteB(serAddr, 0x3a, 0x88); //Disable remote contorller on FPD port 0 and port 1 - comment out if( there is an I2C controller on the remote side

    
    //// *********************************************
    //// Enable I2C Passthrough
    //// *********************************************
    //print("Enable I2C Passthrough");
    uint8_t I2C_PASS_THROUGH = 0;
    uint8_t I2C_PASS_THROUGH_MASK = 0x08;
    uint8_t I2C_PASS_THROUGH_REG = 0;
    I2C_PASS_THROUGH = I2CMA_ReadB(serAddr, 0x7);
    I2C_PASS_THROUGH_MASK = 0x08;
    I2C_PASS_THROUGH_REG = (uint8_t)(I2C_PASS_THROUGH | I2C_PASS_THROUGH_MASK);
    I2CMA_WriteB(serAddr, 0x07, I2C_PASS_THROUGH_REG); //Enable I2C Passthrough

    //// *********************************************
    //// Program VP Configs
    //// *********************************************
    //print("Configure Video Processors");
    // Configure VP 0
    I2CMA_WriteB(serAddr, 0x40, 0x32);
    I2CMA_WriteB(serAddr, 0x41, 0x1);
    I2CMA_WriteB(serAddr, 0x42, 0xa8); //Set VP_SRC_SELECT to Stream 0 for SST Mode
    I2CMA_WriteB(serAddr, 0x41, 0x2);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //VID H Active
    I2CMA_WriteB(serAddr, 0x42, 0x11); //VID H Active
    I2CMA_WriteB(serAddr, 0x41, 0x8);
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start X
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start X
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start Y
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start Y
    I2CMA_WriteB(serAddr, 0x42, 0x7f); //Crop Stop X
    I2CMA_WriteB(serAddr, 0x42, 0x11); //Crop Stop X
    I2CMA_WriteB(serAddr, 0x42, 0x27); //Crop Stop Y
    I2CMA_WriteB(serAddr, 0x42, 0x5); //Crop Stop Y
    I2CMA_WriteB(serAddr, 0x41, 0x10);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //Horizontal Active
    I2CMA_WriteB(serAddr, 0x42, 0x11); //Horizontal Active
    I2CMA_WriteB(serAddr, 0x42, 0x50); //Horizontal Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Horizontal Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Horizontal Sync
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Horizontal Sync
    I2CMA_WriteB(serAddr, 0x42, 0x48); //Horizontal Total
    I2CMA_WriteB(serAddr, 0x42, 0x12); //Horizontal Total
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Active
    I2CMA_WriteB(serAddr, 0x42, 0x5); //Vertical Active
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x8); //Vertical Sync
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Sync
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Front Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Front Porch
    I2CMA_WriteB(serAddr, 0x41, 0x27);
    I2CMA_WriteB(serAddr, 0x42, 0x0); //HSYNC Polarity = +, VSYNC Polarity = +
    I2CMA_WriteB(serAddr, 0x41, 0x0);
    I2CMA_WriteB(serAddr, 0x42, 0x4); //Enable Cropping
    I2CMA_WriteB(serAddr, 0x41, 0x23); //M/N Register
    I2CMA_WriteB(serAddr, 0x42, 0x65); //M value
    I2CMA_WriteB(serAddr, 0x42, 0x28); //M value
    I2CMA_WriteB(serAddr, 0x42, 0xf); //N value

    // Configure VP 1
    I2CMA_WriteB(serAddr, 0x40, 0x32);
    I2CMA_WriteB(serAddr, 0x41, 0x41);
    I2CMA_WriteB(serAddr, 0x42, 0xa8); //Set VP_SRC_SELECT to Stream 0 for SST Mode
    I2CMA_WriteB(serAddr, 0x41, 0x42);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //VID H Active
    I2CMA_WriteB(serAddr, 0x42, 0x11); //VID H Active
    I2CMA_WriteB(serAddr, 0x41, 0x48);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //Crop Start X
    I2CMA_WriteB(serAddr, 0x42, 0x11); //Crop Start X
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start Y
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Crop Start Y
    I2CMA_WriteB(serAddr, 0x42, 0xff); //Crop Stop X
    I2CMA_WriteB(serAddr, 0x42, 0x22); //Crop Stop X
    I2CMA_WriteB(serAddr, 0x42, 0x27); //Crop Stop Y
    I2CMA_WriteB(serAddr, 0x42, 0x5); //Crop Stop Y
    I2CMA_WriteB(serAddr, 0x41, 0x50);
    I2CMA_WriteB(serAddr, 0x42, 0x80); //Horizontal Active
    I2CMA_WriteB(serAddr, 0x42, 0x11); //Horizontal Active
    I2CMA_WriteB(serAddr, 0x42, 0x50); //Horizontal Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Horizontal Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Horizontal Sync
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Horizontal Sync
    I2CMA_WriteB(serAddr, 0x42, 0x48); //Horizontal Total
    I2CMA_WriteB(serAddr, 0x42, 0x12); //Horizontal Total
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Active
    I2CMA_WriteB(serAddr, 0x42, 0x5); //Vertical Active
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Back Porch
    I2CMA_WriteB(serAddr, 0x42, 0x8); //Vertical Sync
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Sync
    I2CMA_WriteB(serAddr, 0x42, 0x28); //Vertical Front Porch
    I2CMA_WriteB(serAddr, 0x42, 0x0); //Vertical Front Porch
    I2CMA_WriteB(serAddr, 0x41, 0x67);
    I2CMA_WriteB(serAddr, 0x42, 0x0); //HSYNC Polarity = +, VSYNC Polarity = +
    I2CMA_WriteB(serAddr, 0x41, 0x40);
    I2CMA_WriteB(serAddr, 0x42, 0x4); //Enable Cropping
    I2CMA_WriteB(serAddr, 0x41, 0x63); //M/N Register
    I2CMA_WriteB(serAddr, 0x42, 0x65); //M value
    I2CMA_WriteB(serAddr, 0x42, 0x28); //M value
    I2CMA_WriteB(serAddr, 0x42, 0xf); //N value
    
    //// *********************************************
    //// Enable PATGEN
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x40, 0x30);
    I2CMA_WriteB(serAddr, 0x41, 0x29);
    I2CMA_WriteB(serAddr, 0x42, 0x10); //Set PATGEN Color Depth to 30bpp for VP0
    I2CMA_WriteB(serAddr, 0x41, 0x28);
    //print("Enable PATGEN on VP0");
    I2CMA_WriteB(serAddr, 0x42, 0x95); //Enable Color Bar PATGEN on VP0 - Comment out this line to disable PATGEN and enable end to end video

    I2CMA_WriteB(serAddr, 0x41, 0x69);
    I2CMA_WriteB(serAddr, 0x42, 0x10); //Set PATGEN Color Depth to 30bpp for VP1
    I2CMA_WriteB(serAddr, 0x41, 0x68);
    //print("Enable PATGEN on VP1");
    I2CMA_WriteB(serAddr, 0x42, 0x95); //Enable Color Bar PATGEN on VP1 - Comment out this line to disable PATGEN and enable end to end video

    //// *********************************************
    //// Initialize CPU
    //// *********************************************
    I2CMA_WriteB(serAddr, 0x48, 0x9);
    I2CMA_WriteB(serAddr, 0x49, 0x4);
    I2CMA_WriteB(serAddr, 0x4a, 0x0);
    I2CMA_WriteB(serAddr, 0x4b, 0x88);
    I2CMA_WriteB(serAddr, 0x4c, 0x0);
    I2CMA_WriteB(serAddr, 0x4d, 0x0);
    I2CMA_WriteB(serAddr, 0x4e, 0x0);
    I2CMA_WriteB(serAddr, 0x49, 0x8);
    I2CMA_WriteB(serAddr, 0x4a, 0x0);
    I2CMA_WriteB(serAddr, 0x4b, 0x8c);
    I2CMA_WriteB(serAddr, 0x4c, 0x0);
    I2CMA_WriteB(serAddr, 0x4d, 0x0);
    I2CMA_WriteB(serAddr, 0x4e, 0x0);
    I2CMA_WriteB(serAddr, 0x49, 0xc);
    I2CMA_WriteB(serAddr, 0x4a, 0x0);
    I2CMA_WriteB(serAddr, 0x4b, 0x90);
    I2CMA_WriteB(serAddr, 0x4c, 0x0);
    I2CMA_WriteB(serAddr, 0x4d, 0x0);
    I2CMA_WriteB(serAddr, 0x4e, 0x0);
    I2CMA_WriteB(serAddr, 0x49, 0x10);
    I2CMA_WriteB(serAddr, 0x4a, 0x0);
    I2CMA_WriteB(serAddr, 0x4b, 0x94);
    I2CMA_WriteB(serAddr, 0x4c, 0x0);
    I2CMA_WriteB(serAddr, 0x4d, 0x0);
    I2CMA_WriteB(serAddr, 0x4e, 0x0);
    I2CMA_WriteB(serAddr, 0x49, 0x14);
    I2CMA_WriteB(serAddr, 0x4a, 0x0);
    I2CMA_WriteB(serAddr, 0x4b, 0x98);
    I2CMA_WriteB(serAddr, 0x4c, 0x0);
    I2CMA_WriteB(serAddr, 0x4d, 0x0);
    I2CMA_WriteB(serAddr, 0x4e, 0x0);
    I2CMA_WriteB(serAddr, 0x49, 0x18);
    I2CMA_WriteB(serAddr, 0x4a, 0x0);
    I2CMA_WriteB(serAddr, 0x4b, 0x9c);
    I2CMA_WriteB(serAddr, 0x4c, 0x0);
    I2CMA_WriteB(serAddr, 0x4d, 0x0);
    I2CMA_WriteB(serAddr, 0x4e, 0x0);
    I2CMA_WriteB(serAddr, 0x49, 0x1c);
    I2CMA_WriteB(serAddr, 0x4a, 0x0);
    I2CMA_WriteB(serAddr, 0x4b, 0x5);
    I2CMA_WriteB(serAddr, 0x4c, 0x0);
    I2CMA_WriteB(serAddr, 0x4d, 0x0);
    I2CMA_WriteB(serAddr, 0x4e, 0x0);

    I2CMA_WriteB(serAddr, 0x49, 0xb8);
    I2CMA_WriteB(serAddr, 0x4a, 0x2);
    I2CMA_WriteB(serAddr, 0x48, 0xb);

    //// *********************************************
    //// Configure Serializer TX Link Layer
    //// *********************************************
    //print("Configure serializer TX link layer");
    I2CMA_WriteB(serAddr, 0x40, 0x2e); //Link layer Reg page
    I2CMA_WriteB(serAddr, 0x41, 0x1); //Link layer 0 stream enable
    I2CMA_WriteB(serAddr, 0x42, 0x3); //Link layer 0 stream enable
    I2CMA_WriteB(serAddr, 0x41, 0x6); //Link layer 0 time slot 0
    I2CMA_WriteB(serAddr, 0x42, 0x20); //Link layer 0 time slot 0 minimum calculation = 32. Assigned slots = 32
    I2CMA_WriteB(serAddr, 0x41, 0x7); //Link layer 0 time slot 1
    I2CMA_WriteB(serAddr, 0x42, 0x20); //Link layer 0 time slot 1 minimum calculation = 32. Assigned slots = 32
    I2CMA_WriteB(serAddr, 0x41, 0x20); //Set Link layer vp bpp
    I2CMA_WriteB(serAddr, 0x42, 0x5a); //Set Link layer vp bpp according to VP Bit per pixel
    I2CMA_WriteB(serAddr, 0x41, 0x0); //Link layer 0 enable
    I2CMA_WriteB(serAddr, 0x42, 0x3); //Link layer 0 enable

    //// *********************************************
    //// Enable VPs
    //// *********************************************
    //print("Enable Video Processors");
    I2CMA_WriteB(serAddr, 0x43, 0x1); //Set number of VPs used = 2
    I2CMA_WriteB(serAddr, 0x44, 0x3); //Enable video processors


    //// *********************************************
    //// Read Deserializer 0 Temp
    //// *********************************************
    //print("Configure deserializer 0 temp ramp optimizations");
    I2CMA_WriteB(desAlias0, 0x40, 0x6c);
    I2CMA_WriteB(desAlias0, 0x41, 0xd);
    I2CMA_WriteB(desAlias0, 0x42, 0x0);
    I2CMA_WriteB(desAlias0, 0x41, 0x13);
    uint8_t TEMP_FINAL = I2CMA_ReadB(desAlias0, 0x42);
    uint8_t TEMP_FINAL_C = 0;

    if(TEMP_FINAL != 0)
    {
        TEMP_FINAL_C = (uint8_t)(2 * TEMP_FINAL - 273);
        //print("Deserializer 0 starting temp =", TEMP_FINAL_C, "C");
    }
    else
    {
        //print("DES Temp Invalid");
    }

    //// *********************************************
    //// Set up Deserializer 0 Temp Ramp Optimizations
    //// *********************************************
    uint8_t Efuse_TS_CODE = 2;
    uint8_t Ramp_UP_Range_CODES_Needed = (uint8_t)(((150 - TEMP_FINAL_C) / (190 / 11)) + 1);
    uint8_t Ramp_DN_Range_CODES_Needed = (uint8_t)(((TEMP_FINAL_C - 30) / (190 / 11)) + 1);
    uint8_t Ramp_UP_CAP_DELTA = (uint8_t)(Ramp_UP_Range_CODES_Needed - 4);
    uint8_t Ramp_DN_CAP_DELTA = (uint8_t)(Ramp_DN_Range_CODES_Needed - 7);

    I2CMA_WriteB(desAlias0, 0x40, 0x3c);
    I2CMA_WriteB(desAlias0, 0x41, 0xf5);
    I2CMA_WriteB(desAlias0, 0x42, (uint8_t)((Efuse_TS_CODE << 4) + 1)); // Override TS_CODE Efuse Code

    if(Ramp_UP_CAP_DELTA > 0)
    {
        //print("Adjusting ramp up and resetting DES");
        int TS_CODE_UP = (Efuse_TS_CODE - Ramp_UP_CAP_DELTA);

        if(TS_CODE_UP < 0)
            TS_CODE_UP = 0;

        I2CMA_WriteB(desAlias0, 0x41, 0xf5);
        uint8_t rb = I2CMA_ReadB(desAlias0, 0x42);
        rb &= 0x8F;
        rb |= (uint8_t)(TS_CODE_UP << 4);
        I2CMA_WriteB(desAlias0, 0x42, rb);
        rb = I2CMA_ReadB(desAlias0, 0x42);
        rb &= 0xFE;
        rb |= 0x01;
        I2CMA_WriteB(desAlias0, 0x42, rb);
        I2CMA_WriteB(desAlias0, 0x1, 0x1);
        Cy_SysLib_Delay(40);
    }

    if(Ramp_DN_CAP_DELTA > 0)
    {
        //print("Adjusting ramp down and resetting DES");
        int TS_CODE_DN = (Efuse_TS_CODE + Ramp_DN_CAP_DELTA);

        if(TS_CODE_DN >= 7)
            TS_CODE_DN = 7;

        I2CMA_WriteB(desAlias0, 0x41, 0xf5);
        uint8_t rb = I2CMA_ReadB(desAlias0, 0x42);
        rb &= 0x8F;
        rb |= (uint8_t)(TS_CODE_DN << 4);
        I2CMA_WriteB(desAlias0, 0x42, rb);
        rb = I2CMA_ReadB(desAlias0, 0x42);
        rb &= 0xFE;
        rb |= 0x01;
        I2CMA_WriteB(desAlias0, 0x42, rb);
        I2CMA_WriteB(desAlias0, 0x1, 0x1);
        Cy_SysLib_Delay(40);
    }

    //// *********************************************
    //// Wait for DES0 ready and accessible
    //// *********************************************
    uint8_t retry = 0;
    uint8_t DES_check = I2CMA_ReadB(desAlias0, 0x0);

    while((DES_check != 0x58) && (retry < 10))
    {
        //print("Can't read DES - Delaying 20ms. Retry =", retry);
        Cy_SysLib_Delay(20);
        DES_check = I2CMA_ReadB(desAlias0, 0x0);
        retry += 1;
    }

    //if( (DES_check == 0x58))
    //print("DES0 Linked!");
    //else
    //print("Delay retry timeout! Unable to access DES0.");

    //// *********************************************
    //// Clear CRC errors from initial link process
    //// *********************************************
    //print("Clear CRC errors from initial link process");
    uint8_t Reg_value = I2CMA_ReadB(serAddr, 0x2);
    Reg_value = (uint8_t)(Reg_value | 0x20);
    I2CMA_WriteB(serAddr, 0x2, Reg_value); //CRC Error Reset
    Reg_value = I2CMA_ReadB(serAddr, 0x2);
    Reg_value = (uint8_t)(Reg_value & 0xdf);
    I2CMA_WriteB(serAddr, 0x2, Reg_value); //CRC Error Reset Clear
    I2CMA_WriteB(serAddr, 0x2d, 0x1);

    //// *********************************************
    //// Hold DES0 DTG in reset
    //// *********************************************
    //print("Hold Des 0 DTG in reset and configure video settings");
    I2CMA_WriteB(desAlias0, 0x40, 0x50); //Select DTG Page
    I2CMA_WriteB(desAlias0, 0x41, 0x32);
    I2CMA_WriteB(desAlias0, 0x42, 0x6); //Hold Local Display Output Port 0 DTG in Reset
    I2CMA_WriteB(desAlias0, 0x41, 0x62);
    I2CMA_WriteB(desAlias0, 0x42, 0x6); //Hold Local Display Output Port 1 DTG in Reset
    I2CMA_WriteB(desAlias0, 0xa8, 0x2); //Disable DES Daisy output

    //// *********************************************
    //// Disable Stream Mapping
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0xe, 0x3); //Select both Output Ports
    I2CMA_WriteB(desAlias0, 0xd0, 0x0); //Disable FPD4 video forward to Output Port
    I2CMA_WriteB(desAlias0, 0xd7, 0x0); //Disable FPD3 video forward to Output Port

    //// *********************************************
    //// Force DP Rate
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x40, 0x2e); //Select DP Page
    I2CMA_WriteB(desAlias0, 0x41, 0x81);
    I2CMA_WriteB(desAlias0, 0x42, 0xc0); //Set DP Rate to 5.4Gbps
    I2CMA_WriteB(desAlias0, 0x42, 0x3); //Enable force DP rate with calibration disabled
    I2CMA_WriteB(desAlias0, 0x41, 0x91);
    I2CMA_WriteB(desAlias0, 0x42, 0xc); //Force 4 lanes on DP port 0
    I2CMA_WriteB(desAlias0, 0x41, 0xb1);
    I2CMA_WriteB(desAlias0, 0x42, 0xc); //Force 4 lanes on DP port 1
    I2CMA_WriteB(desAlias0, 0x40, 0x30); //Disable DP SSCG
    I2CMA_WriteB(desAlias0, 0x41, 0xf);
    I2CMA_WriteB(desAlias0, 0x42, 0x1);

    //// *********************************************
    //// Setup DP ports
    //// *********************************************
    //// Both Ports enabled
    I2CMA_WriteB(desAlias0, 0x1, 0x01); //SW RESET Applied  
    //I2CMA_WriteB(desAlias0, 0x1, 0x40); //DP-TX-PLL RESET Applied
    Cy_SysLib_Delay(60);
    
    //// *********************************************
    //// Program quad pixel clock for DP port 0
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0xe, 0x1); //Select Port0 registers
    I2CMA_WriteB(desAlias0, 0xb1, 0x1); //Enable clock divider
    I2CMA_WriteB(desAlias0, 0xb2, 0x67); //Program M value lower byte
    I2CMA_WriteB(desAlias0, 0xb3, 0x8); //Program M value middle byte
    I2CMA_WriteB(desAlias0, 0xb4, 0x6); //Program M value upper byte
    I2CMA_WriteB(desAlias0, 0xb5, 0x80); //Program N value lower byte
    I2CMA_WriteB(desAlias0, 0xb6, 0xf5); //Program N value middle byte
    I2CMA_WriteB(desAlias0, 0xb7, 0x20); //Program N value upper byte
    I2CMA_WriteB(desAlias0, 0xe, 0x1); //Select Port 0 registers

    //// *********************************************
    //// Setup DES0 DTG port 0
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x40, 0x52); //Select DTG Page w/ auto-increment
    I2CMA_WriteB(desAlias0, 0x41, 0x20);
    I2CMA_WriteB(desAlias0, 0x42, 0xa3); //Set up Local Display DTG BPP, Sync Polarities, and Measurement Type
    I2CMA_WriteB(desAlias0, 0x41, 0x29); //Set Hstart
    I2CMA_WriteB(desAlias0, 0x42, 0x80); //Hstart upper byte
    I2CMA_WriteB(desAlias0, 0x42, 0x78); //Hstart lower byte
    I2CMA_WriteB(desAlias0, 0x41, 0x2f); //Set HSW
    I2CMA_WriteB(desAlias0, 0x42, 0x40); //HSW upper byte
    I2CMA_WriteB(desAlias0, 0x42, 0x28); //HSW lower byte

    //// *********************************************
    //// Program DPTX for DP port 0
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x1); //Enable APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0xa4); //Set bit per color
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x40);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb8); //Set pixel width
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x4);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xac); //Set DP Mvid
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0xb7);
    I2CMA_WriteB(desAlias0, 0x4c, 0x5d);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb4); //Set DP Nvid
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x80);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc8); //Set TU Mode
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb0); //Set TU Size
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x40);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x2c);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc8); //Set Fif(O Size
    I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    I2CMA_WriteB(desAlias0, 0x4b, 0x6);
    I2CMA_WriteB(desAlias0, 0x4c, 0x40);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xbc); //Set data count
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x68);
    I2CMA_WriteB(desAlias0, 0x4c, 0x10);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc0); //Disable STREAM INTERLACED
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc4); //Set SYNC polarity
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0xc);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    //// *********************************************
    //// Program quad pixel clock for DP port 1
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0xe, 0x12); //Select Port1 registers
    I2CMA_WriteB(desAlias0, 0xb1, 0x1); //Enable clock divider
    I2CMA_WriteB(desAlias0, 0xb2, 0x67); //Program M value lower byte
    I2CMA_WriteB(desAlias0, 0xb3, 0x8); //Program M value middle byte
    I2CMA_WriteB(desAlias0, 0xb4, 0x6); //Program M value upper byte
    I2CMA_WriteB(desAlias0, 0xb5, 0x80); //Program N value lower byte
    I2CMA_WriteB(desAlias0, 0xb6, 0xf5); //Program N value middle byte
    I2CMA_WriteB(desAlias0, 0xb7, 0x20); //Program N value upper byte
    I2CMA_WriteB(desAlias0, 0xe, 0x1); //Select Port 0 registers

    //// *********************************************
    //// Setup DES0 DTG port 1
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x40, 0x52); //Select DTG Page w/ auto-increment
    I2CMA_WriteB(desAlias0, 0x41, 0x50);
    I2CMA_WriteB(desAlias0, 0x42, 0xa3); //Set up Local Display DTG BPP, Sync Polarities, and Measurement Type
    I2CMA_WriteB(desAlias0, 0x41, 0x59); //Set Hstart
    I2CMA_WriteB(desAlias0, 0x42, 0x80); //Hstart upper byte
    I2CMA_WriteB(desAlias0, 0x42, 0x78); //Hstart lower byte
    I2CMA_WriteB(desAlias0, 0x41, 0x5f); //Set HSW
    I2CMA_WriteB(desAlias0, 0x42, 0x40); //HSW upper byte
    I2CMA_WriteB(desAlias0, 0x42, 0x28); //HSW lower byte

    //// *********************************************
    //// Program DPTX for DP port 1
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x9); //Enable APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0xa4); //Set bit per color
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x40);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb8); //Set pixel width
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x4);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xac); //Set DP Mvid
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0xb7);
    I2CMA_WriteB(desAlias0, 0x4c, 0x5d);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb4); //Set DP Nvid
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x80);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc8); //Set TU Mode
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xb0); //Set TU Size
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x40);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x2c);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc8); //Set Fif(O Size
    I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    I2CMA_WriteB(desAlias0, 0x4b, 0x6);
    I2CMA_WriteB(desAlias0, 0x4c, 0x40);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xbc); //Set data count
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x68);
    I2CMA_WriteB(desAlias0, 0x4c, 0x10);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc0); //Disable STREAM INTERLACED
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x0);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x49, 0xc4); //Set SYNC polarity
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0xc);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    //// *********************************************
    //// Map video to display output
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0xe, 0x3); //Select both Output Ports
    I2CMA_WriteB(desAlias0, 0xd0, 0xc); //Enable FPD_RX video forward to Output Port
    I2CMA_WriteB(desAlias0, 0xd1, 0xf); //Every stream forwarded on DC
    I2CMA_WriteB(desAlias0, 0xd6, 0x8); //Send Stream 0 to Output Port 0 and Send Stream 1 to Output Port 1
    I2CMA_WriteB(desAlias0, 0xd7, 0x0); //FPD3 to local display output mapping disabled
    I2CMA_WriteB(desAlias0, 0xe, 0x1); //Select Port 0

    //// *********************************************
    //// Release DES0 DTG from reset
    //// *********************************************
    //print("Release Des 0 DTG reset and enable video output");
    I2CMA_WriteB(desAlias0, 0x40, 0x50); //Select DTG Page
    I2CMA_WriteB(desAlias0, 0x41, 0x32);
    I2CMA_WriteB(desAlias0, 0x42, 0x5); //Release Local Display Output Port 0 DTG with WDT Enabled
    I2CMA_WriteB(desAlias0, 0x41, 0x62);
    I2CMA_WriteB(desAlias0, 0x42, 0x5); //Release Local Display Output Port 1 DTG with WDT Enabled

    I2CMA_WriteB(desAlias0, 0x48, 0x1); //Enable APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0x80); //Set DPTX Port0 Htotal
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x48);
    I2CMA_WriteB(desAlias0, 0x4c, 0x12);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    I2CMA_WriteB(desAlias0, 0x48, 0x9); //Enable APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0x80); //Set DPTX Port1 Htotal
    I2CMA_WriteB(desAlias0, 0x4a, 0x1);
    I2CMA_WriteB(desAlias0, 0x4b, 0x48);
    I2CMA_WriteB(desAlias0, 0x4c, 0x12);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    Cy_SysLib_Delay(40); //Add delay to avoid Fif(O-OV

    //// *********************************************
    //// Enable DP 0 output
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x1); //Enable APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0x84); //Enable DP output
    I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    I2CMA_WriteB(desAlias0, 0x4b, 0x1);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    //// *********************************************
    //// Enable DP 1 output
    //// *********************************************
    I2CMA_WriteB(desAlias0, 0x48, 0x9); //Enable APB interface
    I2CMA_WriteB(desAlias0, 0x49, 0x84); //Enable DP output
    I2CMA_WriteB(desAlias0, 0x4a, 0x0);
    I2CMA_WriteB(desAlias0, 0x4b, 0x1);
    I2CMA_WriteB(desAlias0, 0x4c, 0x0);
    I2CMA_WriteB(desAlias0, 0x4d, 0x0);
    I2CMA_WriteB(desAlias0, 0x4e, 0x0);

    /////INX
    FPD984_I2c_1Mhz();
   
    /*setting 983 gpio init*/
    I2CMA_WriteB(serAddr, 0x17, 0x80); //GPIO0_PIN_CTL Register ////0000000b = Received PORT0 BC_GPIO0
    I2CMA_WriteB(serAddr, 0x18, 0x81); //GPIO1_PIN_CTL Register ////0000001b = Received PORT0 BC_GPIO1
    I2CMA_WriteB(serAddr, 0x28, 0x00); //GPIO_EN_BC Register    //BC enable 4 gpio
    I2CMA_WriteB(serAddr, 0x1d, 0x89); //GPIO6_PIN_CTL          //PORT0 RX_LOCK_DET
    /*setting 984 gpio init*/
    //GPIO0(IN):TP_IN,  GPIOI(IN):IRQ, GPIO6:LOCK
    I2CMA_WriteB(desAlias0, 0x13, 0x0F); //GPIO_IN_EN_LOW //GPIO0,1,2,3 are input.
    I2CMA_WriteB(desAlias0, 0x15, 0x00);  //GPIO0_PIN_CTL //input
    I2CMA_WriteB(desAlias0, 0x16, 0x00);  //GPIO1_PIN_CTL //input
    I2CMA_WriteB(desAlias0, 0x1B, 0xC2);  //GPIO6_PIN_CTL //output

    /*Determines the data sent on GPIO0 and GPIO1 for the selected port back channel.*/
    /*GPIO0 and GPIO1:RX input. TX output.*/
    I2CMA_WriteB(desAlias0, 0x40, 0x04); //select PAGE1(main page2)
    I2CMA_WriteB(desAlias0, 0x41, 0x10); //BC_GPIO_CTL0 Register
    I2CMA_WriteB(desAlias0, 0x42, 0x00);
    I2CMA_WriteB(desAlias0, 0x41, 0x11); //BC_GPIO_CTL1 Register
    I2CMA_WriteB(desAlias0, 0x42, 0x01);
    I2CMA_WriteB(desAlias0, 0x41, 0x20); //BC_CONFIG Register
    I2CMA_WriteB(desAlias0, 0x42, 0x00);

    /////*clear 984 CRC and ECC error count*/
    //FPD984_CLEAR_CRCECC(0);
    //FPD984_CLEAR_CRCECC(1);
#endif

    return FUNC_RETURN_OK;
}

static t_FUNC_RETURN LongRun_TestProcess(void)
{
    t_FUNC_RETURN rtnState = FUNC_RETURN_OK;
    uint8_t Diag1;
    uint8_t Diag2;
    int ntc1;
    int ntc2;
    int ntc3;
    int deltaNtc;
    uint16_t getVal;
    uint16_t OResetCnt;
    uint16_t PowerOnCnt;
    static uint16_t PrePowerOnCnt;
    
    switch(ItemSwitch)
    {
        case 0U:  /*init state*/ 
            printf("Case0: Init State\r\n");             
            (void)LongRun_ModulePower(1U);
            (void)LongRun_ModuleWakeUp(1U);            
            Cy_SysLib_Delay(400U);
            (void)LongRun_FPDLink_Script();
            rtnState = GmVCS_GetDiag(&Diag1, &Diag2);
            PrePowerOnCnt = GmVCS_GetPwrCnt(); 
            (void)LongRun_ModulePower(0U);
            TimeCtrlCnt = 0U;
            ItemSwitch = 2U;
            break;
            
        case 2U:  /*power on*/
            if(TimeCtrlCnt == 0U)
            {
                printf("Case2: Power On\r\n");                
                (void)LongRun_ModulePower(1U);
                (void)LongRun_ModuleWakeUp(1U);                
                Cy_SysLib_Delay(400U);
                (void)LongRun_FPDLink_Script();

                FPD983_SetPattern(White, White);
                GmVCS_SetDimm(8U, 0xFFFFU);
                rtnState = GmVCS_GetDiag(&Diag1, &Diag2);
                rtnState |= GmVCS_EngMode(1U);
                PowerOnCnt = GmVCS_GetPwrCnt();
                PrePowerOnCnt = PowerOnCnt - 1U; //for test                

                if(rtnState == FUNC_RETURN_OK)
                {
                    if((Diag1 == 0x00U) && (Diag2 == 0x00U) &&
                            (PowerOnCnt == (PrePowerOnCnt + 1U)))
                    {
                        TimeCtrlCnt = 1U;
                        PrePowerOnCnt = PowerOnCnt;
                    }
                    else
                    {
                        printf("Case0-1: Diag error\r\n");
                        ItemSwitch = 0xFF;
                    }
                }
                else
                {
                    printf("Case0-2: I2CA Bus error\r\n");
                    ItemSwitch = 0xFF;
                }
            }
            else
            {
                rtnState = GmVCS_ForceGetDiag(&Diag1, &Diag2);

                if(rtnState == FUNC_RETURN_OK)
                {
                    if((Diag1 == 0x00U) && (Diag2 == 0x00U))
                    {
                        if(TimeCtrlCnt >= 10U)////80) /*80sec*/
                        {
                            TimeCtrlCnt = 0U;
                            ItemSwitch = 4U;
                        }
                        else
                        {
                            GmVCS_SetDimm(8U, 0xFFFFU);                        
                            TimeCtrlCnt ++;
                        }
                    }
                    else
                    {
                        printf("Case0-3: Diag1 error\r\n");
                        ItemSwitch = 0xFFU;
                    }
                }
                else
                {
                    printf("Case0-4: Diag1 error\r\n");
                    ItemSwitch = 0xFFU;
                }
            }

            if(ItemSwitch == 0xFFU)
            {
                TimeCtrlCnt = 0U;
                FPD983_SetPattern(HorBlackToWhite, HorBlackToWhite);
            }

            break;

        case 4: /*Brightness control*/
            if(TimeCtrlCnt == 0)
            {
                printf("Case4: Brightness control\r\n");
                FPD983_SetPattern(Green, Green);
                TestDimmPWM = 0xFFFFU;
                TimeCtrlCnt = 1U;
            }
            else
            {
                rtnState = GmVCS_GetDimm(8, &getVal);

                if(rtnState == FUNC_RETURN_OK)
                {
                    if(getVal == TestDimmPWM)
                    {
                        if(TimeCtrlCnt >= 10U)////80U)    /*80sec*/
                        {
                            TimeCtrlCnt = 0U;
                            ItemSwitch = 6;
                        }
                        else
                        {
                            TestDimmPWM -= 0x300U;
                            (void)GmVCS_SetDimm(8U, TestDimmPWM);
                            TimeCtrlCnt++;
                        }
                    }
                    else
                    {
                        printf("Case4-1: TestDimmPWM error\r\n");
                        printf("getVal=0x%2X, TestDimmPWM=0x%2X\r\n",getVal, TestDimmPWM);
                        ItemSwitch = 0xFFU;
                    }
                }
                else
                {
                    printf("Case4-2: bus error\r\n");
                    ItemSwitch = 0xFFU;
                }
            }

            if(ItemSwitch == 0xFFU)
            {
                TimeCtrlCnt = 0U;
                FPD983_SetPattern(HorBlackToGreen, HorBlackToGreen);
            }

            break;

        case 6:  /*Mcu reset test.*/
            if(TimeCtrlCnt == 0)
            {
                printf("Case6: Mcu reset test.\r\n");
                OResetCnt = GmVCS_GetResetCnt();
                rtnState = GmVCS_MCURst();
                Cy_SysLib_Delay(400U);
                (void)LongRun_FPDLink_Script();
                FPD983_SetPattern(Red, Red);
                GmVCS_SetDimm(8U, 0xFFFFU);
                getVal = GmVCS_GetResetCnt();
                rtnState |= GmVCS_GetDiag(&Diag1, &Diag2);
                rtnState |= GmVCS_EngMode(1U);

                if(rtnState == FUNC_RETURN_OK)
                {
                    if((Diag1 == 0x00U) && (Diag2 == 0x00U) &&
                            (getVal == (OResetCnt + 1U)))
                    {
                        TimeCtrlCnt = 1U;
                    }
                    else
                    {
                        printf("Case6-1: check value error\r\n");                        
                        ItemSwitch = 0xFFU;
                    }
                }
                else
                {
                    printf("Case6-2: bus error\r\n");  
                    ItemSwitch = 0xFFU;
                }
            }
            else
            {
                rtnState = GmVCS_ForceGetDiag(&Diag1, &Diag2);

                if(rtnState == FUNC_RETURN_OK)
                {
                    if((Diag1 == 0x00U) && (Diag2 == 0x00U))
                    {
                        if(TimeCtrlCnt >= 10U)////60U)    /*60sec*/
                        {
                            TimeCtrlCnt = 0U;
                            ItemSwitch = 8;
                        }
                        else
                        {
                            GmVCS_SetDimm(8U, 0xFFFFU);
                            TimeCtrlCnt ++;
                        }
                    }
                    else
                    {
                        printf("Case6-1: check value error\r\n");
                        ItemSwitch = 0xFFU;
                    }
                }
                else
                {
                    printf("Case6-4: bus error\r\n");  
                    ItemSwitch = 0xFFU;
                }
            }

            if(ItemSwitch == 0xFFU)
            {
                TimeCtrlCnt = 0U;
                FPD983_SetPattern(HorBlackToRed, HorBlackToRed);
            }

            break;

        case 8: /*NTC Test:  Blue pattern*/
            if(TimeCtrlCnt == 0U)
            {
                printf("Case8: NTC Test\r\n");
                FPD983_SetPattern(Blue, Blue);
                GmVCS_SetDimm(8U, 0xFFFFU);
            }

            rtnState = GmVCS_GetNTC123(&ntc1, &ntc2, &ntc3);

            if(ntc2 > ntc3)
                deltaNtc = (ntc2 - ntc3);
            else
                deltaNtc = (ntc3 - ntc2);

            if((rtnState == FUNC_RETURN_OK) &&
                    (ntc2 >= 50) && (ntc2 <= 650) &&
                    (ntc3 >= 50) && (ntc3 <= 650) && (deltaNtc <= 350))
            {
                if(TimeCtrlCnt >= 10)////50)  /*50sec*/
                {
                    TimeCtrlCnt = 0U;
                    ItemSwitch = 10U;
                    printf("Case8: ntc2=%d, ntc3=%d\r\n",ntc2,ntc3);
                
                }
                else
                {
                    GmVCS_SetDimm(8U, 0xFFFFU);
                    TimeCtrlCnt++;
                }    
            }
            else
            {
                printf("Case8: NTC Test Fail. ntc2=%d, ntc3=%d\r\n",ntc2,ntc3);
                TimeCtrlCnt = 0U;
                ItemSwitch = 0xFF;   /*Stop State*/
                FPD983_SetPattern(HorBlackToBlue, HorBlackToBlue);
            }

            break;

        case 10U: /*power off/on2*/
            if(TimeCtrlCnt == 0U)
            {
                printf("Case10: power off\r\n");
                (void)LongRun_ModuleWakeUp(0U);
                (void)LongRun_ModulePower(0U);
            }

            if(TimeCtrlCnt >= 10)////300) /*300esc*/
            {
                TimeCtrlCnt = 0U;
                ItemSwitch = 0x02U;
            }
            else
            {
                TimeCtrlCnt++;
            }
            break;

        default:
            break;
    }

    if(ItemSwitch == 0xFF)
    {
        return FUNC_RETURN_NG;
    }
    else
    {
        return FUNC_RETURN_OK;
    }

}

/* ==[ Definition of functions with external linkage ]======================= */
/*******************************************************************************
* \brief Function Name: LongRun_Init
*******************************************************************************
*
* Summary:
*   project callback function.
*
* Parameters:
* NONE
*
* Return:
* t_FUNC_RETURN :
*
*/
t_FUNC_RETURN LongRun_Init(void)
{
    LongRun_BoardPower(1U);
    (void)FPD983_SetI2cAddr(serAddr);
    (void)FPD984_SetI2cAddr(desAddr0);
    //LongRun_ModulePower(1U);  /*Module On*/

    /*WDT Init*/
    (void)SWTimer_Reset(1U, SWTimer_Routine, 50U); /*50ms WDT Period */
    //(void)Wdt_Init();  //stop wdt

    INIT_FINSHED = FUNC_RETURN_OK;
    return INIT_FINSHED;
}


/*******************************************************************************
* \brief Function Name: LongRun_RoutineCall
*******************************************************************************
*
* Summary:
*   project RoutineCall function.
*
* Parameters:
* NONE
*
* Return:
* t_FUNC_RETURN :
*
*/
t_FUNC_RETURN LongRun_RoutineCall(void)
{
#if 0
    static uint8_t Button_Val;
    static uint8_t DipSwitch_Val;
    uint8_t butHoldState = 0U;
#endif
    static uint8_t CheckFPDLive = 0U;


    //uint8_t T10msTick = 0U;

    /*Get 10ms time base flag.*/
    //T10msBase = SWTimer_Get10msTimeBase();
#if 0
    Button_Val = Debounce_GetPort(BUTTON_PORT, 0xC0U);  /*use debounce function to get buttom value. */
    DipSwitch_Val = Gpio_GetPort(DIP_SWITCH_PORT);
#endif
    /*buttom long press detect.  Port4.6 P4.7 */

    if(SWTimer_Get1SecRoutine() > 0U)
    {
        /*butHoldState = Button_GetBtnHoldState();*/

        if(CheckFPDLive == 0U)
        {
            if(I2CMA_ReadB(serAddr, 0x0) == 0x18U)
            {
                CheckFPDLive = 1U;
            }
        }

        /*ToDo: LongRun Loop*/
        if((INIT_FINSHED == FUNC_RETURN_OK) && (CheckFPDLive > 0U))
        {
            LongRun_TestProcess();
        }
    }

#if 0

    /*2sec routine call.*/
    if(SWTimer_GetTimeoutState(4U) > 0U) /*2sec*/
    {
        Gpio_Inv(SW_Debug_Led);
    }

#endif

    /*WDT CLEAR*/
    if(SWTimer_Get50msRoutine() > 0U)
    {
        Wdt_Clear();
    }

    return FUNC_RETURN_OK;
}

/*******************************************************************************
* \brief Function Name: Project_Gpio_Init
*******************************************************************************
*
* Summary:
*   Gpio pins callback function.
*
* Parameters:
* NONE
*
* Return:
* t_FUNC_RETURN :
*
*/
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
    Gpio_PinConfig(MUX_I2C_SEL, CY_GPIO_DM_STRONG, 1U); /*1:PSOC I2C*/
    Gpio_PinConfig(MUX_SPI_SEL, CY_GPIO_DM_STRONG, 1U); /*1:PSOC SPI*/  
    
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
    Gpio_PinConfig(EX_IO2, CY_GPIO_DM_STRONG, 0U);
    Gpio_PinConfig(EN_5V0, CY_GPIO_DM_STRONG, 0U);
    Gpio_PinConfig(DBGP_I2C_Bypass, CY_GPIO_DM_STRONG, 0);  /*dbg i2c close*/
    Gpio_PinConfig(DBGP_SPI_Bypass, CY_GPIO_DM_STRONG, 0);  /*dbg spi close*/
    Gpio_PinConfig(Breathe_LED, CY_GPIO_DM_STRONG, 0U);
    Gpio_PinConfig(DBG_UART, CY_GPIO_DM_STRONG, 0U);

    Gpio_PinConfig(EncAP1, CY_GPIO_DM_STRONG, 0U);
    Gpio_PinConfig(EncAP2, CY_GPIO_DM_STRONG, 0U);
    Gpio_PinConfig(EncBP1, CY_GPIO_DM_STRONG, 0U);
    Gpio_PinConfig(EncBP2, CY_GPIO_DM_STRONG, 0U);

    Gpio_PinConfig(EX_FPGA1, CY_GPIO_DM_STRONG, 0U);

#endif

    return FUNC_RETURN_OK;
}

/*============================================*/
/*******************************************************************************
* \brief Function Name: Project_ExtINT_Callback
*******************************************************************************
*
* Summary:
*   External interrupt callback function.
*
* Parameters:
* uint8_t PortPin : External interrupt pin name.
*
* Return:
* t_FUNC_RETURN :
*
*/
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

/*******************************************************************************
* \brief Function Name: Project_I2CSW_Callback
*******************************************************************************
*
* Summary:
*   Project-customized i2cs commands.
*
* Parameters:
* uint8_t i2csWriteBuff[] :
*
* Return:
* t_FUNC_RETURN :
*
*/
t_FUNC_RETURN Project_I2CSW_Callback(uint8_t i2csWriteBuff[])
{
    t_FUNC_RETURN rtnState = FUNC_RETURN_OK;

    switch(i2csWriteBuff[1])
    {
        default:
            rtnState = FUNC_RETURN_NG;
            break;
    }

    return rtnState;
}

/*******************************************************************************
* \brief Function Name: Project_I2CSR_Callback
*******************************************************************************
*
* Summary:
*   Project-customized i2cs commands.
*
* Parameters:
* uint8_t i2csReadBuff[] :
*
* Return:
* t_FUNC_RETURN :
*
*/
t_FUNC_RETURN Project_I2CSR_Callback(uint8_t i2csReadBuff[])
{
    t_FUNC_RETURN rtnState = FUNC_RETURN_OK;
    uint8_t len = 0;
    uint8_t rtnBuff[10];

    switch(i2csReadBuff[1])
    {
        default:
            rtnState = FUNC_RETURN_NG;
            break;
    }

    if(rtnState == FUNC_RETURN_OK)
    {
        (void)I2cs_UpdateReadBuff(rtnBuff, len);
    }

    return rtnState;
}

#endif
