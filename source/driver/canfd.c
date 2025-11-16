/**
 * \file
 *
 * \brief Brief description of the source file.
 *
 * Provide a detailed description of the source file.
 *
 * Project(s): PSOC4_Brifge_Board_App xxx
 *
 * Target: independent
 *  CANFD Formate:   SOF, Arbitration field(ID), Control field, data field, CRC field,EOF
 *  CANFD Spec.:
 *                 1. 500K+1M
 *                 2. default:Base ID(11bites), Base ID(11bites)+ID extension(18bits)=29bits,
 *                 3. format: classic CAN, CanFD(default)
 *                 4. DLC:0~15, 8(default)
 *                 5. MAX read/write data length: 64bytes
 *                 6. CRC: 15, 17, 21
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
#include "canfd.h"

/* ==[ Macros ]============================================================= */
/*#define CANFD_DLC               8*/   /*write 8bytes. DLC=8*/
#define CANFD_HW_CHANNEL        0
#define CANFD_BUFFER_INDEX      0

/*
|-DLC-|-CAN(BYTES)-|-CAN FD(BYTES)-|-CAN CRC-|-CAN FD CRC-|
|-----|------------|---------------|----------|------------|
|-0    -|-0                 -|-0                      -|-15           -|-17               -|
|-1    -|-1                 -|-1                      -|-15           -|-17               -|
|-2    -|-2                 -|-2                      -|-15           -|-17               -|
|-3    -|-3                 -|-3                      -|-15           -|-17               -|
|-4    -|-4                 -|-4                      -|-15           -|-17               -|
|-5    -|-5                 -|-5                      -|-15           -|-17               -|
|-6    -|-6                 -|-6                      -|-15           -|-17               -|
|-7    -|-7                 -|-7                      -|-15           -|-17               -|
|-8    -|-8                 -|-8                      -|-15           -|-17               -|
|-9    -|-8                 -|-12                    -|-15           -|-17               -|
|-10  -|-8                 -|-16                    -|-15           -|-17               -|
|-11  -|-8                 -|-20                    -|-15           -|-21               -|
|-12  -|-8                 -|-24                    -|-15           -|-21               -|
|-13  -|-8                 -|-32                    -|-15           -|-21               -|
|-14  -|-8                 -|-48                    -|-15           -|-21               -|
|-15  -|-8                 -|-64                    -|-15           -|-21               -|
|-----|------------|---------------|----------|------------|
*/
/* ==[ Types ]============================================================== */

/* ==[ Constants with internal linkage ]==================================== */
/* Array to store the data bytes of the CANFD frame. Max 16*4=64bytes*/
static CanFdFifo canfd_rx_fifo;
static cy_stc_canfd_context_t canfd_context;
/*500KHZ:{ .prescaler, .timeSegment1,  .timeSegment2, .syncJumpWidth} */
static cy_stc_canfd_bitrate_t CanFD_Normal_BitRate;
/*1000KHz: { .prescaler, .timeSegment1,  .timeSegment2, .syncJumpWidth} */
static cy_stc_canfd_bitrate_t CanFD_Fast_BitRate;

/* ==[ Variables with internal linkage ]==================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with internal linkage ]===================== */
static void CanFd_Fifo_Init(void);
/*static bool CanFd_Fifo_IsFull(void)*/
static bool CanFd_Fifo_IsEmpty(void);
static bool CanFd_Fifo_Push(CanFdMsg msg);
static bool CanFd_Fifo_Pop(CanFdMsg *msg);
static uint8_t CanFd_DLCToLength(uint8_t DLC);
static uint8_t CanFd_LengthToDLC(uint8_t Wlen);
////static void CanFd_ReSetBitRate(void);

/* ==[ Definition of functions with internal linkage ]======================= */
static void CanFd_Fifo_Init(void)
{
    canfd_rx_fifo.head = 0U;
    canfd_rx_fifo.tail = 0U;
    canfd_rx_fifo.count = 0U;
    canfd_rx_fifo.buffer[0].length = 0U;
}
#if 0
static bool CanFd_Fifo_IsFull(void)
{
    return (canfd_rx_fifo.count >= FIFO_SIZE);
}
#endif
static bool CanFd_Fifo_IsEmpty(void)
{
    return (canfd_rx_fifo.count == 0U);
}

/*
*
*
*     -->tail     : Data1
*                     : Data2
*                     :     :
*                     : Data(N-1)
*     -->head  : Empty
*/
static bool CanFd_Fifo_Push(CanFdMsg msg)
{
    /*ring fifo*/
    //canfd_rx_fifo.buffer[canfd_rx_fifo.head] = msg;  /*OK: low level copy*/
    memcpy(&canfd_rx_fifo.buffer[canfd_rx_fifo.head], &msg, sizeof(CanFdMsg));

    #if 0
    printf("Can[]=%x %x %x %x %x %x %x %x\r\n",
                   msg.data[0],msg.data[1],msg.data[2],msg.data[3],msg.data[4],msg.data[5],msg.data[6],msg.data[7]
                   );
    #endif
    if(canfd_rx_fifo.count < FIFO_SIZE)
    {
        canfd_rx_fifo.head = (canfd_rx_fifo.head + 1U) % FIFO_SIZE;
        canfd_rx_fifo.count++;
    }
    else
    {
        canfd_rx_fifo.head = (canfd_rx_fifo.head + 1U) % FIFO_SIZE;
        canfd_rx_fifo.tail = (canfd_rx_fifo.tail + 1U) % FIFO_SIZE;

    }

    return true;
}

/*
*
*
*     x-> tail       : Data1
*     -->             : Data2
*                        :     :
*                        : Data(N-1)
*     -->head      : Empty
*/

static bool CanFd_Fifo_Pop(CanFdMsg *msg)
{
    if(CanFd_Fifo_IsEmpty())
    {
        return false;
    }

    /* *msg = canfd_rx_fifo.buffer[canfd_rx_fifo.tail]; */ /*NG: low level copy*/
    memcpy(msg, &canfd_rx_fifo.buffer[canfd_rx_fifo.tail], sizeof(CanFdMsg));

    canfd_rx_fifo.tail = (canfd_rx_fifo.tail + 1U) % FIFO_SIZE;
    canfd_rx_fifo.count--;

    return true;
}

static uint8_t CanFd_DLCToLength(uint8_t DLC)
{
    uint8_t rtnLen;

    if(DLC == 15U)
    {
        rtnLen = 64U;
    }
    else if(DLC == 14U)
    {
        rtnLen = 48U;
    }
    else if(DLC == 13U)
    {
        rtnLen = 32U;
    }
    else if(DLC == 12U)
    {
        rtnLen = 24U;
    }
    else if(DLC == 11U)
    {
        rtnLen = 20U;
    }
    else if(DLC == 10U)
    {
        rtnLen = 16U;
    }
    else if(DLC == 9U)
    {
        rtnLen = 12U;
    }
    else
    {
        rtnLen = DLC;
    }

    return rtnLen;

}

static uint8_t CanFd_LengthToDLC(uint8_t Wlen)
{
    uint8_t DLC;

    if(Wlen > 48U)
    {
        DLC = 15U;
    }
    else if(Wlen > 36U)
    {
        DLC = 14U;
    }
    else if(Wlen > 24U)
    {
        DLC = 13U;
    }
    else if(Wlen > 20U)
    {
        DLC = 12U;
    }
    else if(Wlen > 16U)
    {
        DLC = 11U;
    }
    else if(Wlen > 12U)
    {
        DLC = 10U;
    }
    else if(Wlen > 8U)
    {
        DLC = 9U;
    }
    else
    {
        DLC = Wlen;
    }

    return DLC;

}

/* ==[ Definition of functions with external linkage ]====================== */
/*******************************************************************************
 * \brief Function Name: initMaster
 *******************************************************************************
 *
 * Summary:
 * This function initializes the SPI master based on the configuration done in
 * design.modus file.
 *
 * Parameters:
 * None
 *
 * Return:
 * uint32_t - Returns INIT_SUCCESS if the initialization is successful.
 * Otherwise it returns INIT_FAILURE
 *
 ******************************************************************************/
t_FUNC_RETURN CanFd_Init(void)
{
    cy_en_canfd_status_t status;


    /*Step1: CanFD Init*/
    CanFD_Normal_BitRate = mCANFD_nominalBitrateConfig;
    CanFD_Fast_BitRate = mCANFD_dataBitrateConfig;

    /*If the channel was disabled, call Cy_CANFD_Enable before calling Cy_CANFD_Init.*/
    (void)Cy_CANFD_Enable(mCANFD_HW, 0x00000001UL);/* Enable the first channel */
    Cy_SysLib_Delay(100U); /*wait for power stable */
    status = Cy_CANFD_Init(mCANFD_HW, CANFD_HW_CHANNEL, &mCANFD_config, &canfd_context);

    if(status != CY_CANFD_SUCCESS)
    {
        printf("CanFd_Init Fail!");
        return FUNC_RETURN_NG;
    }
 
    /*Step2: set interrupt*/
    /* Populate the configuration structure for CANFD Interrupt */
    cy_stc_sysint_t canfd_irq_cfg =
    {
        /* Source of interrupt signal */
        .intrSrc      = canfd_interrupts0_0_IRQn,
        /* Interrupt priority */
        .intrPriority = 3U
    };
    /* Hook the interrupt service routine and enable the interrupt */
    (void)Cy_SysInt_Init(&canfd_irq_cfg, CanFd_ISR);
    NVIC_EnableIRQ(canfd_irq_cfg.intrSrc);

#if 1
    /*Step 3: Disable Auto reTransmission*/
    /* Enables the configuration changes to set Test mode */
    Cy_CANFD_ConfigChangesEnable(mCANFD_HW, 0);
    /* Scenario:  Configures Test mode to use the external loop back to receive the sent data back.*/
    Cy_CANFD_TestModeConfig(mCANFD_HW, CANFD_HW_CHANNEL, CY_CANFD_TEST_MODE_DISABLE); //CY_CANFD_TEST_MODE_DISABLE);
    /* Disables the configuration changes */
    Cy_CANFD_ConfigChangesDisable(mCANFD_HW, 0);
#endif
    
    (void)CanFd_Fifo_Init();

    return FUNC_RETURN_OK;

}

/*******************************************************************************
* Function Name: CanFd_ISR
********************************************************************************
* Summary:
* This is the interrupt handler function for the canfd interrupt.
*
* Parameters:
*  none
*
*
*
******************************************************************************/
void CanFd_ISR(void)
{
    /*printf("CanFd_ISR\r\n");*/

    /* Just call the IRQ handler with the current channel number and context */
    Cy_CANFD_IrqHandler(mCANFD_HW, CANFD_HW_CHANNEL, &canfd_context);
}

/*******************************************************************************
* Function Name: canfd_rx_callback
********************************************************************************
* Summary:
* This is the callback function for canfd reception. This function is declared in the "Device Configration".
* RX max 64 bytes
*
* typedef struct
* {
*     cy_stc_canfd_r0_t *r0_f;              // Rx buffer element for R0. See \ref cy_stc_canfd_r0_t
*     cy_stc_canfd_r1_t *r1_f;              // Rx buffer element for R1. See \ref cy_stc_canfd_r1_t
*     uint32_t          *data_area_f;       //Rx buffer element for Rn
* } cy_stc_canfd_rx_buffer_t;
* typedef struct
* {
*     volatile uint32_t id;                       // Identifier
*     volatile cy_en_canfd_rtr_t    rtr;   // Remote transmission request
*     volatile cy_en_canfd_xtd_t    xtd; // Extended identifier
*     volatile cy_en_canfd_esi_t    esi;  // Error state indicator
* } cy_stc_canfd_r0_t;
*
* typedef struct
* {
*     volatile uint32_t rxts;                   // Rx timestamp
*     volatile uint32_t dlc;                    // Data length code
*     volatile bool     brs;                      // Bit rate switch
*     volatile cy_en_canfd_fdf_t    fdf;   // Extended data length
*     volatile uint32_t fidx;                   // Filter index
*     volatile cy_en_canfd_anmf_t   anmf;  // Accepted non-matching frame
* } cy_stc_canfd_r1_t;
*
* typedef struct
* {
*     uint32_t id;
*     uint8_t data[CANFD_MAX_DATA];
*     uint8_t length;
*     bool    ext_id;
*     bool    fd_frame;
*     bool    trt;
* } CanFdMsg;
*
*
*
* Parameters:
*    msg_valid                     Message received properly or not
*    msg_buf_fifo_num         RxFIFO number of the received message
*    canfd_rx_buf                 Message buffer
*
*******************************************************************************/
void CanFd_RxCallback(bool msg_valid,
                      uint8_t    msg_buf_fifo_num,
                      cy_stc_canfd_rx_buffer_t*   canfd_rx_buf)
{

    /* Array to hold the data bytes of the CANFD frame */
    CanFdMsg canfd_msg;

    if(true == msg_valid)
    {
        /* Checking whether the frame received is a data frame */
        if(CY_CANFD_RTR_DATA_FRAME == canfd_rx_buf->r0_f->rtr)
        {
            /* Variable to hold the data length code of the CANFD frame */
            canfd_msg.length = CanFd_DLCToLength(canfd_rx_buf->r1_f->dlc);
            /* Variable to hold the Identifier of the CANFD frame */
            canfd_msg.id = canfd_rx_buf->r0_f->id;
            canfd_msg.FDF = (uint8_t)canfd_rx_buf->r1_f->fdf;

            memcpy(canfd_msg.data, canfd_rx_buf->data_area_f, canfd_msg.length);
            CanFd_Fifo_Push(canfd_msg);

#if 0
            printf("%x bytes received from identifier %lx\r\n",
                   canfd_msg.length,
                   canfd_msg.id);
#endif
#if 0
            printf("Rx Data : ");

            for(uint8_t msg_idx = 0U; msg_idx < canfd_msg.length ; msg_idx++)
            {
                printf(" %d ", canfd_msg.data[msg_idx]);
            }

            printf("\r\n");
#endif
        }
    }

}

/*******************************************************************************
* Function Name: canfd_Tx
********************************************************************************
* Summary:
*   The maximum transmitted data is 8 bytes and is stored in 2 uint32_t arrays.
*   TX DLC is fixed at 8 bytes in the "Device Config"
*   TX max 64 bytes.  Current DCL=8.
*
* cy_en_canfd_status_t Cy_CANFD_UpdateAndTransmitMsgBuffer(
*                                 CANFD_Type *base,
*                                 uint32_t chan,
*                                 const cy_stc_canfd_tx_buffer_t *txBuffer,
*                                 uint8_t index,
*                                 cy_stc_canfd_context_t const *context
*                                 )
* cy_stc_canfd_t0_t CANFD_T0RegisterBuffer_0 =
* {
*     .id = 0x22U,
*     .rtr = CY_CANFD_RTR_DATA_FRAME,      //0:data frame(fix), 1:remote frame(canfd no support)
*     .xtd = CY_CANFD_XTD_STANDARD_ID,   //0:11-bit standard identifier, 1:29-bit extended identifier
*     .esi = CY_CANFD_ESI_ERROR_ACTIVE,  //Error state indicator.
* };
* cy_stc_canfd_t1_t CANFD_T1RegisterBuffer_0 =
* {
*     .dlc = 8U,                                               //Data length code.
*     .brs = true,                                            //Bit rate switching
*     .fdf = CY_CANFD_FDF_CAN_FD_FRAME, //clase CAN or CAN FD Format. //other:CY_CANFD_FDF_STANDARD_FRAME
*     .efc = false,                                           //false: Do not store Tx events, true: Store Tx events
*     .mm = 0U,                                             //Message marker.
* };
* cy_stc_canfd_tx_buffer_t CANFD_txBuffer_0 =
* {
*     .t0_f = &CANFD_T0RegisterBuffer_0,
*     .t1_f = &CANFD_T1RegisterBuffer_0,
*     .data_area_f = CANFD_dataBuffer_0,
* };
*
* Parameters:
*    uint8 ExtFormat : bit0:     0: canfd base format(11bits), 1:canfd extended format(29bits)
*                                bit7:     0: calssic can,                         1:canfd
*    uint32_t Id         :
*    uint32_t canfd_WData[] : The length are 8bytes(2*uint32_t).
*    uint8_t Wlen      :   <=64bytes
* Return:
*    t_FUNC_RETURN
*******************************************************************************/
t_FUNC_RETURN CanFd_TX(uint8 ExtFormat, uint32_t Id, uint8_t canfd_WData[], uint8_t Wlen)
{
    t_FUNC_RETURN rtn_Status = FUNC_RETURN_NG;
    cy_en_canfd_status_t tx_status = CY_CANFD_SUCCESS;

    if(Wlen <= 64U)
    {
        /*Step1: Set ID, xtd, fdf and  DLC*/
        if((ExtFormat & 0x01U) > 0U)
        {
            /*29bits: extended mode*/
            mCANFD_T0RegisterBuffer_0.id = Id;
            mCANFD_T0RegisterBuffer_0.xtd = CY_CANFD_XTD_EXTENDED_ID; /*IDE*/
        }
        else
        {
            /*11-bit standard identifier*/
            mCANFD_T0RegisterBuffer_0.id = Id;
            mCANFD_T0RegisterBuffer_0.xtd = CY_CANFD_XTD_STANDARD_ID; /*IDE*/
        }

        if((ExtFormat & 0x80U) > 0U)
        {
            mCANFD_T1RegisterBuffer_0.fdf = CY_CANFD_FDF_CAN_FD_FRAME;
            mCANFD_T1RegisterBuffer_0.brs = true;
        }
        else
        {
            mCANFD_T1RegisterBuffer_0.fdf = CY_CANFD_FDF_STANDARD_FRAME;
            mCANFD_T1RegisterBuffer_0.brs = false;
        }

        mCANFD_T1RegisterBuffer_0.dlc = CanFd_LengthToDLC(Wlen);
#if 0
        printf("ExtFormat=%x, Id=%4lx, DLC=%x\r\n",
               ExtFormat,
               Id,
               (uint16_t)mCANFD_T1RegisterBuffer_0.dlc);
#endif
#if 0
        printf("Id=%lx, rtr=%x, xtd=%x, esi=%x\r\n",
               mCANFD_T0RegisterBuffer_0.id,
               (uint16_t)mCANFD_T0RegisterBuffer_0.rtr,
               (uint16_t)mCANFD_T0RegisterBuffer_0.xtd,
               (uint16_t)mCANFD_T0RegisterBuffer_0.esi);
        printf("DLC=%x, brs=%x, fdf=%x, efc=%x, mm=%x\r\n",
               (uint16_t)mCANFD_T1RegisterBuffer_0.dlc,
               (uint16_t)mCANFD_T1RegisterBuffer_0.brs,
               (uint16_t)mCANFD_T1RegisterBuffer_0.fdf,
               (uint16_t)mCANFD_T1RegisterBuffer_0.efc,
               (uint16_t)mCANFD_T1RegisterBuffer_0.mm);
#endif

        /*Step2: wtire data in the buffer and than tx output.*/
        /* Assign the user defined data buffer to CANFD data area */
        memcpy(mCANFD_txBuffer_0.data_area_f, canfd_WData, Wlen);
        tx_status = Cy_CANFD_UpdateAndTransmitMsgBuffer(
                        mCANFD_HW,
                        CANFD_HW_CHANNEL,
                        &mCANFD_txBuffer_0,
                        CANFD_BUFFER_INDEX,
                        &canfd_context);

#if 0
        printf("tx_status=%x, D[0]=%lx, D[1]=%lx\r\n",
               tx_status,
               (uint32_t)mCANFD_txBuffer_0.data_area_f[0],
               (uint32_t)mCANFD_txBuffer_0.data_area_f[1]
              );
#endif
    }

    if(CY_CANFD_SUCCESS == tx_status)
    {
        rtn_Status = FUNC_RETURN_OK;
    }
    else
    {
        rtn_Status = FUNC_RETURN_NG;
    }

    return rtn_Status;

}

/*******************************************************************************
* Function Name: CanFd_RX
********************************************************************************
* Summary:
*   RX max 64 bytes
*
*
* Parameters:
*    CanFdMsg  *msg
* return :
*    bool
*
*******************************************************************************/
t_FUNC_RETURN CanFd_RX(CanFdMsg *msg)
{
    t_FUNC_RETURN rtn_Status;
    bool pop_status = CanFd_Fifo_Pop(msg);

    if(pop_status == true)
    {
        rtn_Status = FUNC_RETURN_OK;
    }
    else
    {
        rtn_Status = FUNC_RETURN_NG;
    }

    return rtn_Status;

}

/*******************************************************************************
* Function Name: CanFd_Get_Fifo_DataLen
********************************************************************************
* Summary:
*   Only get rx data length
*
*
* Parameters:
*    void
* return :
*    uint8_t:  >64: empty
*
*******************************************************************************/
uint8_t CanFd_Get_Fifo_DataLen(void)
{
    uint8_t dataLen;

    if(CanFd_Fifo_IsEmpty() == false)
    {
        dataLen = canfd_rx_fifo.buffer[canfd_rx_fifo.tail].length + 4U; /*4 bytes ID + Data*/
    }
    else
    {
        dataLen = 0xFFU;  /*empty*/
    }

    return dataLen;
}

/*******************************************************************************
* Function Name: CanFd_Sett_FUNC_RETURNBitRate
********************************************************************************
* Summary:
* default Bit Rate :  NormalBitRate_KHz = 500KHz,  FastBitRate_KHz = 1000KHz
* fix parameter=> tseg1 = 5 tq, tseg2 = 2 tq, sjw = 2 tq
* Can Freq Base :  48MHz
* Prescaler = 48 MHz / 500 kbps / 8 tq = 12
* Prescaler = 48 MHz / 1000 kbps / 8 tq = 6
* Prescaler = 48 MHz / 2000 kbps / 8 tq = 3
*
* Parameters:
*    uint16_t NormalBitRate_KHz
*    uint16_t FastBitRate_KHz
* return :
*    t_FUNC_RETURN
*
*******************************************************************************/
t_FUNC_RETURN CanFd_SetBitRate(uint16_t NormalBitRate_KHz, uint16_t FastBitRate_KHz)
{
    t_FUNC_RETURN rtn_Status = FUNC_RETURN_NG;

    if((NormalBitRate_KHz >= 500U) && (FastBitRate_KHz >= 500U))
    {
        Cy_CANFD_ConfigChangesEnable(mCANFD_HW, 0UL);
        CanFD_Normal_BitRate.prescaler = (48000 / NormalBitRate_KHz / 8) - 1U;
        Cy_CANFD_SetBitrate(mCANFD_HW, 0UL, &CanFD_Normal_BitRate);

        CanFD_Fast_BitRate.prescaler = (48000 / FastBitRate_KHz / 8) - 1U;
        Cy_CANFD_SetFastBitrate(mCANFD_HW, 0UL, &CanFD_Fast_BitRate);
        Cy_CANFD_ConfigChangesDisable(mCANFD_HW, 0UL);

        rtn_Status = FUNC_RETURN_OK;

#if 0
        printf("NormalBitRate_KHz=%d, FastBitRate_KHz=%d\r\n",
               NormalBitRate_KHz,
               FastBitRate_KHz
              );
#endif
    }

    return rtn_Status;

}



