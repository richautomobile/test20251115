/******************************************************************************
* File Name:   SpiMaster.c
*
* Description: This file contains all the function prototypes required for
*              SPI Master implemented using Serial Communication Block (SCB)
*
*
*******************************************************************************
* Copyright 2020-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#ifndef _SPIMASTER_H_
#define _SPIMASTER_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================= */
/* ==[ Types ]============================================================== */
/* ==[ Constants with external linkage ]==================================== */
/* ==[ Variables with external linkage ]==================================== */
/* ==[ Declaration of functions with external linkage ]===================== */

/***************************************
*         Function Prototypes
****************************************/
extern void SPIM_Isr(void);
extern t_FUNC_RETURN SPIM_Init(void);
extern t_FUNC_RETURN SPIM_SetCLK(uint16_t KHz);
extern uint16_t SPIM_GetCLK(void);
extern t_FUNC_RETURN SPIM_SetPOL_PHA(uint8_t POL, uint8_t PHA);
extern void SPIM_Interrupt(void);
extern t_FUNC_RETURN SPIM_Write(uint8_t cs_ch, uint8_t WBuffer[], uint32_t WByteSize);
extern t_FUNC_RETURN SPIM_WriteRead(uint8_t cs_ch, uint8_t WRBuffer[], uint32_t WRByteLen, uint8_t RBuffer[], uint32_t RByteLen);

#endif /* SOURCE_SPIMASTER_H_ */

/* [] END OF FILE */
