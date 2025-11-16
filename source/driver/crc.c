/**
 * @file crc.c
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
#include "crc.h"
#include "dma.h"
/* ==[ Macros ]============================================================== */
/* CRC parameters for some CRC algorithms:
 +---------------------+-----+------------+------------+------+------+-----+------------+------------+
 | CRC algorithm Name  | CRC | Polynomial |   Initial  | Data | Data | Rem |  Remainder |  Expected  |
 |                     | len |            |    seed    | REV  | XOR  | REV |    XOR     |    CRC     |
 | ------------------- | --- | ---------- |----------- | ---- | ---- | --- | ---------- | ---------- |
 | CRC-6  / CDMA2000-A |  6  |    0x27    |    0x3F    |   0  |   0  |  0  |    0x00    |    0x0D    |
 | CRC-6  / CDMA2000-B |  6  |    0x07    |    0x3F    |   0  |   0  |  0  |    0x00    |    0x3B    |
 | CRC-6  / DARC       |  6  |    0x19    |    0x00    |   1  |   0  |  1  |    0x00    |    0x26    |
 | CRC-6  / ITU        |  6  |    0x03    |    0x00    |   1  |   0  |  1  |    0x00    |    0x06    |
 | CRC-8  / ITU        |  8  |    0x07    |    0x00    |   0  |   0  |  0  |    0x55    |    0xA1    |
 | CRC-8  / MAXIM      |  8  |    0x31    |    0x00    |   1  |   0  |  1  |    0x00    |    0xA1    |
 | CRC-8  / ROHC       |  8  |    0x07    |    0xFF    |   1  |   0  |  1  |    0x00    |    0xD0    |
 | CRC-8  / WCDMA      |  8  |    0x9B    |    0x00    |   1  |   0  |  1  |    0x00    |    0x25    |
 | CRC-16 / CCITT-0    | 16  |   0x1021   |   0xFFFF   |   0  |   0  |  0  |   0x0000   |   0x29B1   |
 | CRC-16 / CDMA2000   | 16  |   0xC867   |   0xFFFF   |   0  |   0  |  0  |   0x0000   |   0x4C06   |
 | CRC-32              | 32  | 0x04C11DB7 | 0xFFFFFFFF |   1  |   0  |  1  | 0xFFFFFFFF | 0xCBF43926 |
 | CRC-32 / BZIP2      | 32  | 0x04C11DB7 | 0xFFFFFFFF |   0  |   0  |  0  | 0xFFFFFFFF | 0xFC891918 |
 +---------------------+-----+------------+------------+------+------+-----+------------+------------+
 */

/*  CRC-32   */
#define CRC32_WIDTH                (32u)
#define CRC32_POLYNOMIAL           (0x04C11DB7u)
#define CRC32_DATA_REVERSE         (1u)
#define CRC32_DATA_XOR             (0u)
#define CRC32_REM_REVERSE          (1u)
#define CRC32_REM_XOR              (0xFFFFFFFFu)
#define CRC32_LFSR_SEED            (0xFFFFFFFFu)

/* Use "CRC-16/CCITT-0" calculation */
#define CRC16CCITT_WIDTH           (16u)
#define CRC16CCITT_POLYNOMIAL      (0x1021u)
#define CRC16CCITT_DATA_REVERSE    (0u)
#define CRC16CCITT_DATA_XOR        (0u)
#define CRC16CCITT_REM_REVERSE     (0u)
#define CRC16CCITT_REM_XOR         (0x0000u)
#define CRC16CCITT_LFSR_SEED       (0x0000u)   /*INX:(0x0000u), CCITT-0: (0xffffu)*/

/* Use "CRC-8/EBU" calculation. for GM */
#define CRC8EBU_WIDTH           (8u)
#define CRC8EBU_POLYNOMIAL      (0x1Du)
#define CRC8EBU_DATA_REVERSE    (0u)     //(1u)
#define CRC8EBU_DATA_XOR        (0u)
#define CRC8EBU_REM_REVERSE     (0u)     //(1u)
#define CRC8EBU_REM_XOR         (0x00u)
#define CRC8EBU_LFSR_SEED       (0x00u)  //(0xFFu)

/* ==[ Types ]=============================================================== */
/* ==[ Constants with external linkage ]===================================== */
/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with internal linkage ]====================== */
/*static void Crc_DBG(void)*/
#if (SW_OR_HW_CRC == SW_CRC)
static uint8_t highAndLowShiftFor8Bits(uint8_t data);
/*static uint16_t highAndLowShiftFor16Bits(uint16_t data)*/
static uint32_t highAndLowShiftFor32Bits(uint32_t data);
#endif
/* ==[ Definition of functions with internal linkage ]====================== */
/*static void Crc_DBG()
{
    uint8_t data[2] = { 0xB3, 0x0F };
    uint32_t calculatedCrc;

    Crc_CalCrc32(data, 2, &calculatedCrc);
    Crc_CalCrc16(data, 2, &calculatedCrc);
    Crc_CalCrc8(data, 2, &calculatedCrc);
    Crc_CalCrc8(data, 2, &calculatedCrc );
}*/
#if (SW_OR_HW_CRC == SW_CRC)
static uint8_t highAndLowShiftFor8Bits(uint8_t data)
{
    data = (uint8_t)((data << 4) | (data >> 4));
    data = (uint8_t)(((data << 2) & 0xcc) | ((data >> 2) & 0x33));
    data = (uint8_t)(((data << 1) & 0xaa) | ((data >> 1) & 0x55));
    return data;
}
/*
static uint16_t highAndLowShiftFor16Bits(uint16_t data)
{
    data = (uint16_t)((data << 8) | (data >> 8));
    data = (uint16_t)(((data << 4) & 0xf0f0) | ((data >> 4) & 0x0f0f));
    data = (uint16_t)(((data << 2) & 0xcccc) | ((data >> 2) & 0x3333));
    data = (uint16_t)(((data << 1) & 0xaaaa) | ((data >> 1) & 0x5555));
    return data;
}
*/
static uint32_t highAndLowShiftFor32Bits(uint32_t data)
{
    data = ((data << 16) | (data >> 16));
    data = (((data << 8) & 0xff00ff00) | ((data >> 8) & 0x00ff00ff));
    data = (((data << 4) & 0xf0f0f0f0) | ((data >> 4) & 0x0f0f0f0f));
    data = (((data << 2) & 0xcccccccc) | ((data >> 2) & 0x33333333));
    data = (((data << 1) & 0xaaaaaaaa) | ((data >> 1) & 0x55555555));
    return data;
}
#endif

/* ==[ Definition of functions with external linkage ]====================== */
#if (SW_OR_HW_CRC == HW_CRC)
/*******************************************************************************
 * \brief Function Name: Crc_Init
 ********************************************************************************
 * Summary:
 * CRC HW function init.
 *
 *
 * \Parameters [in] NONE
 * \return          t_FUNC_RETURN
 *
 *******************************************************************************/
t_FUNC_RETURN Crc_Init()
{
    cy_en_crypto_status_t crypto_status = CY_CRYPTO_NOT_INITIALIZED;
    t_FUNC_RETURN rtu_state;

    /* Enable the Crypto block. */
    crypto_status = Cy_Crypto_Enable(CRYPTO);

    /* Crypto init failed. Stop program execution */
    if(crypto_status != CY_CRYPTO_SUCCESS)
    {
        /*CY_ASSERT(0)*/
        rtu_state = FUNC_RETURN_NG;
    }
    else
    {
        /*Crc_DBG()*/
        rtu_state = FUNC_RETURN_OK;
    }

    return rtu_state;
}

/*******************************************************************************
 * \brief Function Name: crc_CalCrc32
 ********************************************************************************
 * Summary:
 *  CRC-32                             => CRC32_WIDTH
 *  UInt16 polynomial = 0x04C11DB7;    => CRC32_POLYNOMIAL
 *  bool RefIn = true;                         => CRC32_DATA_REVERSE
 *  UInt16 XorOut = 0xFFFFFFFF         => CRC32_REM_XOR
 *  bool RefOut = true;                       => CRC32_REM_REVERSE
 *  UInt16 Init = 0xFFFFFFFF;              => CRC32_LFSR_SEED
 *
 *
 * \Parameters [in]     uint8_t *: Input array for calculating crc values
 * \Parameters [in]     uint32_t : array length
 * \Parameters [in\out] uint32_t : calculating crc32 values
 * \return          t_FUNC_RETURN
 *
 *******************************************************************************/
t_FUNC_RETURN Crc_CalCrc32(uint8_t *message, uint32_t dataLen,
                           uint32_t *calculatedCrc)
{
    cy_stc_crypto_crc_context_t crcContext;
    cy_en_crypto_status_t cryptoStatus;

    /* Initialize Crypto CRC operation */
    cryptoStatus = Cy_Crypto_Crc_CalcInit(
                       CRYPTO, /* Base address of the Crypto block registers */
                       CRC32_WIDTH, /* CRC width in bits*/
                       CRC32_POLYNOMIAL, /* CRC polynomial (specified using 32 bits) */
                       CRC32_DATA_REVERSE, /* Byte order for processing: 0 - MSB first; 1 - LSB first */
                       CRC32_DATA_XOR, /* Byte mask for XORing data */
                       CRC32_REM_REVERSE, /* Reverse remainder */
                       CRC32_REM_XOR, /* Byte mask for XORing LFSR32 register */
                       CRC32_LFSR_SEED, /* LFSR initial seed */
                       &crcContext); /* Pointer to CRC context structure */
    /* ... check for errors... */
    /* Calculate CRC */
    cryptoStatus = Cy_Crypto_Crc_Calc(
                       CRYPTO, /* Base address of the Crypto block registers */
                       calculatedCrc, /* Pointer to CRC result */
                       (void*)message, /* Pointer to message */
                       dataLen, /* Size of message */ /*sizeof(message)*/
                       &crcContext); /* Pointer to CRC context structure */

    /* ... check for errors... */
    /* Free the CRC context */
    cryptoStatus = Cy_Crypto_Crc_CalcFree(
                       CRYPTO, /* Base address of the Crypto block registers */
                       &crcContext); /* Pointer to CRC context structure */

    /* ... check for errors... */
    if(cryptoStatus == CY_CRYPTO_SUCCESS)
    {
        /*CY_ASSERT(0)*/
        return FUNC_RETURN_OK;
    }
    else
    {
        return FUNC_RETURN_NG;
    }
}

/*******************************************************************************
 * \brief Function Name: Crc_CalCrc16
 ********************************************************************************
 * Summary:
 *  CRC-16 / CCITT-0               => CRC16CCITT_WIDTH
 *  UInt16 polynomial = 0x1021;    => CRC16CCITT_POLYNOMIAL
 *  bool RefIn = false;            => CRC16CCITT_DATA_REVERSE
 *  UInt16 XorOut = 0x0000;        => CRC16CCITT_REM_XOR
 *  bool RefOut = false;           => CRC16CCITT_REM_REVERSE
 *  UInt16 Init = 0x0000;          => CRC16CCITT_LFSR_SEED
 *
 *
 * \Parameters [in]     uint8_t *: Input array for calculating crc values
 * \Parameters [in]     uint32_t : array length
 * \Parameters [in\out] uint32_t : calculating crc16 values
 * \return          t_FUNC_RETURN
 *
 *******************************************************************************/
t_FUNC_RETURN Crc_CalCrc16(uint8_t *message, uint32_t dataLen,
                           uint32_t *calculatedCrc)
{
    cy_stc_crypto_crc_context_t crcContext;
    cy_en_crypto_status_t cryptoStatus;
    /* Initialize Crypto CRC operation */
    cryptoStatus = Cy_Crypto_Crc_CalcInit(
                       CRYPTO, /* Base address of the Crypto block registers */
                       CRC16CCITT_WIDTH, /* CRC width in bits*/
                       CRC16CCITT_POLYNOMIAL, /* CRC polynomial (specified using 32 bits) */
                       CRC16CCITT_DATA_REVERSE, /* Byte order for processing: 0 - MSB first; 1 - LSB first */
                       CRC16CCITT_DATA_XOR, /* Byte mask for XORing data */
                       CRC16CCITT_REM_REVERSE, /* Reverse remainder */
                       CRC16CCITT_REM_XOR, /* Byte mask for XORing LFSR32 register */
                       CRC16CCITT_LFSR_SEED, /* LFSR initial seed */
                       &crcContext); /* Pointer to CRC context structure */
    /* ... check for errors... */
    /* Calculate CRC */
    cryptoStatus = Cy_Crypto_Crc_Calc(
                       CRYPTO, /* Base address of the Crypto block registers */
                       calculatedCrc, /* Pointer to CRC result */
                       (void*) message, /* Pointer to message */
                       dataLen, /* Size of message */ /*sizeof(message)*/
                       &crcContext); /* Pointer to CRC context structure */
    /* ... check for errors... */
    /* Free the CRC context */
    cryptoStatus = Cy_Crypto_Crc_CalcFree(
                       CRYPTO, /* Base address of the Crypto block registers */
                       &crcContext); /* Pointer to CRC context structure */

    /* ... check for errors... */
    if(cryptoStatus == CY_CRYPTO_SUCCESS)
    {
        /*CY_ASSERT(0)*/
        return FUNC_RETURN_OK;
    }
    else
    {
        return FUNC_RETURN_NG;
    }
}

/*******************************************************************************
 * \brief Function Name: Crc_CalCrc8
 ********************************************************************************
 * Summary:
 *  CRC-8 / EBU                     => CRC8EBU_WIDTH
 *  UInt16 polynomial = 0x1D;       => CRC8EBU_POLYNOMIAL
 *  bool RefIn = true ;             => CRC8EBU_DATA_REVERSE
 *  UInt16 XorOut = 0x00;           => CRC8EBU_REM_XOR
 *  bool RefOut = true;             => CRC8EBU_REM_REVERSE
 *  UInt16 Init = 0xFF;             => CRC8EBU_LFSR_SEED
 *
 *
 * \Parameters [in]     uint8_t *: Input array for calculating crc values
 * \Parameters [in]     uint32_t : array length
 * \Parameters [in\out] uint32_t : calculating crc8 values
 * \return          t_FUNC_RETURN
 *
 *******************************************************************************/
t_FUNC_RETURN Crc_CalCrc8(uint8_t *message, uint32_t dataLen,
                          uint32_t *calculatedCrc)
{
    cy_stc_crypto_crc_context_t crcContext;
    cy_en_crypto_status_t cryptoStatus;
    /* Initialize Crypto CRC operation */
    cryptoStatus = Cy_Crypto_Crc_CalcInit(
                       CRYPTO, /* Base address of the Crypto block registers */
                       CRC8EBU_WIDTH, /* CRC width in bits*/
                       CRC8EBU_POLYNOMIAL, /* CRC polynomial (specified using 32 bits) */
                       CRC8EBU_DATA_REVERSE, /* Byte order for processing: 0 - MSB first; 1 - LSB first */
                       CRC8EBU_DATA_XOR, /* Byte mask for XORing data */
                       CRC8EBU_REM_REVERSE, /* Reverse remainder */
                       CRC8EBU_REM_XOR, /* Byte mask for XORing LFSR32 register */
                       CRC8EBU_LFSR_SEED, /* LFSR initial seed */
                       &crcContext); /* Pointer to CRC context structure */
    /* ... check for errors... */
    /* Calculate CRC */
    cryptoStatus = Cy_Crypto_Crc_Calc(
                       CRYPTO, /* Base address of the Crypto block registers */
                       calculatedCrc, /* Pointer to CRC result */
                       (void*) message, /* Pointer to message */
                       dataLen, /* Size of message */ /*sizeof(message)*/
                       &crcContext); /* Pointer to CRC context structure */
    /* ... check for errors... */
    /* Free the CRC context */
    cryptoStatus = Cy_Crypto_Crc_CalcFree(
                       CRYPTO, /* Base address of the Crypto block registers */
                       &crcContext); /* Pointer to CRC context structure */

    /* ... check for errors... */
    if(cryptoStatus == CY_CRYPTO_SUCCESS)
    {
        /*CY_ASSERT(0)*/
        return FUNC_RETURN_OK;
    }
    else
    {
        return FUNC_RETURN_NG;
    }
}



#endif

#if (SW_OR_HW_CRC == SW_CRC)
t_FUNC_RETURN Crc_CalCrc32(uint8_t *message, uint32_t dataLen,
                             uint32_t *calculatedCrc)
{
    uint32_t Init = 0xFFFFFFFFUL;       /*CRC32_LFSR_SEED*/
    uint32_t polynomial = 0x04C11DB7UL; /*CRC32_POLYNOMIAL*/
    bool RefIn = true;                  /*CRC32_DATA_REVERSE*/
    bool RefOut = true;                 /*CRC32_REM_REVERSE*/
    uint32_t XorOut = 0xFFFFFFFFUL;     /*CRC32_REM_XOR*/
    uint32_t crc;
    uint8_t data;

    /*Init value*/
    crc = Init;

    for(uint32_t i = 0; i < dataLen; i++)
    {
        data = message[i];

        /*RefIn of every byte of data*/
        if(RefIn == true)
        {
            data = highAndLowShiftFor8Bits(data);
        }

        /*CRC polynomial calculation*/
        crc = crc ^ ((uint32_t)data << 24);

        for(uint8_t j = 0; j < 8; j++)
        {
            if((crc & 0x80000000UL) > 0)
                crc = (crc << 1) ^ polynomial;
            else
                crc = (crc << 1);
        }
    }

    /* RefOut of result of crc*/
    if(RefOut == true)
    {
        crc = highAndLowShiftFor32Bits(crc);
    }

    /*XorOut*/
    crc = crc ^ XorOut;
    *calculatedCrc = crc;

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Crc_CalCrc16(uint8_t *message, uint32_t dataLen,
                             uint32_t *calculatedCrc)
{
    uint16_t Init = 0x0000;       /*INX:(0x0000u), CCITT-0: (0xffffu)*/ /*CRC16CCITT_LFSR_SEED*/
    uint16_t polynomial = 0x1021; /*CRC16CCITT_POLYNOMIAL*/
    /*bool RefIn = false*/           /*CRC16CCITT_DATA_REVERSE*/
    /*bool RefOut = false*/          /*CRC16CCITT_REM_REVERSE*/
    uint16_t XorOut = 0x0000;     /*CRC16CCITT_REM_XOR*/
    uint16_t crc;
    uint8_t data;

    /*Init value*/
    crc = Init;

    for(uint32_t i = 0; i < dataLen; i++)
    {
        data = message[i];
#if 0

        /*RefIn of every byte of data*/
        if(RefIn == true)
        {
            data = highAndLowShiftFor8Bits(data);
        }

#endif
        /*CRC polynomial calculation*/
        crc = (uint16_t)(crc ^ ((uint16_t)data << 8));

        for(uint8_t j = 0; j < 8; j++)
        {
            if((crc & 0x8000U) > 0)
                crc = (uint16_t)((crc << 1) ^ polynomial);
            else
                crc = (uint16_t)(crc << 1);
        }
    }

#if 0

    /*RefOut of result of crc*/
    if(RefOut == true)
    {
        crc = highAndLowShiftFor16Bits(crc);
    }

#endif

    /*XorOut*/
    crc = (uint16_t)(crc ^ XorOut);
    *calculatedCrc = crc;

    return FUNC_RETURN_OK;
}

t_FUNC_RETURN Crc_CalCrc8(uint8_t *message, uint32_t dataLen,
                             uint32_t *calculatedCrc)
{
    /*For GM*/ 
    uint8_t Init = 0x00; //0xFF;       /**/
    uint8_t polynomial = 0x1D; /*CRC8EBU_POLYNOMIAL*/
    bool RefIn = false;//true;           /*CRC8EBU_DATA_REVERSE*/
    bool RefOut = false; //true;          /*CRC8EBU_REM_REVERSE*/
    uint8_t XorOut = 0x00U;     /*CRC8EBU_REM_XOR*/
    uint8_t crc;
    uint8_t data;

    /*Init value*/
    crc = Init;

    for(uint32_t i = 0; i < dataLen; i++)
    {
        data = message[i];

        /*RefIn of every byte of data*/
        if(RefIn == true)
        {
            data = highAndLowShiftFor8Bits(data);
        }

        /*CRC polynomial calculation*/
        crc = (uint8_t)(crc ^ data);

        for(uint8_t j = 0; j < 8; j++)
        {
            if((crc & 0x80U) > 0)
                crc = (uint8_t)((crc << 1) ^ polynomial);
            else
                crc = (uint8_t)(crc << 1);
        }
    }

    /*RefOut of result of crc*/
    if(RefOut == true)
    {
        crc = highAndLowShiftFor8Bits(crc);
    }

    /*XorOut*/
    crc = (uint8_t)(crc ^ XorOut);
    *calculatedCrc = crc;

    return FUNC_RETURN_OK;
}

#endif


