/*******************************************************************************
 * File Name: cycfg_peripherals.h
 *
 * Description:
 * Peripheral Hardware Block configuration
 * This file was automatically generated and should not be modified.
 * Configurator Backend 3.30.0
 * device-db 4.11.1.5194
 * mtb-pdl-cat2 2.8.0.11364
 *
 *******************************************************************************
 * Copyright 2025 Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/

#if !defined(CYCFG_PERIPHERALS_H)
#define CYCFG_PERIPHERALS_H

#include "cycfg_notices.h"
#include "cy_canfd.h"
#include "cy_sysclk.h"
#include "cy_ctb.h"
#include "cy_sar.h"
#include "cycfg_routing.h"
#include "cy_scb_uart.h"
#include "cy_scb_spi.h"
#include "cy_scb_i2c.h"
#include "cy_tcpwm_counter.h"
#include "cy_tcpwm_quaddec.h"
#include "cy_tcpwm_pwm.h"

#if defined (CY_USING_HAL)
#include "cyhal_hwmgr.h"
#endif /* defined (CY_USING_HAL) */

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define mCANFD_ENABLED 1U
#define mCANFD_HW CANFD0
#define mCANFD_CHANNEL CANFD0_CH0
#define mCANFD_STD_ID_FILTER_ID_0 0
#define mCANFD_STD_ID_FILTER_ID_1 1
#define mCANFD_EXT_ID_FILTER_ID_0 0
#define mCANFD_EXT_ID_FILTER_ID_1 1
#define mCANFD_DATA_0 0
#define mCANFD_DATA_1 1
#define mCANFD_DATA_2 2
#define mCANFD_DATA_3 3
#define mCANFD_DATA_4 4
#define mCANFD_DATA_5 5
#define mCANFD_DATA_6 6
#define mCANFD_DATA_7 7
#define mCANFD_DATA_8 8
#define mCANFD_DATA_9 9
#define mCANFD_DATA_10 10
#define mCANFD_DATA_11 11
#define mCANFD_DATA_12 12
#define mCANFD_DATA_13 13
#define mCANFD_DATA_14 14
#define mCANFD_DATA_15 15
#define mCANFD_IRQ_0 canfd_interrupts0_0_IRQn
#define mCANFD_IRQ_1 canfd_interrupts1_0_IRQn
#define mPASS0_ENABLED 1U
#define mADC_ENABLED 1U
#define mADC_HW SAR0
#define mADC_IRQ pass_0_interrupt_sar_IRQn
#define mADC_VREF_MV 1650UL
#define mMcuTemp_ENABLED 1U
#define mUART_ENABLED 1U
#define mUART_HW SCB0
#define mUART_IRQ scb_0_interrupt_IRQn
#define mSPIM_ENABLED 1U
#define mSPIM_HW SCB1
#define mSPIM_IRQ scb_1_interrupt_IRQn
#define mI2CMA_ENABLED 1U
#define mI2CMA_HW SCB2
#define mI2CMA_IRQ scb_2_interrupt_IRQn
#define mI2CMB_ENABLED 1U
#define mI2CMB_HW SCB3
#define mI2CMB_IRQ scb_3_interrupt_IRQn
#define mI2CS_ENABLED 1U
#define mI2CS_HW SCB4
#define mI2CS_IRQ scb_4_interrupt_IRQn
#define mPWMCAP0_ENABLED 1U
#define mPWMCAP0_HW TCPWM
#define mPWMCAP0_NUM 0UL
#define mPWMCAP0_MASK (1UL << 0)
#define mPWMCAP0_IRQ tcpwm_interrupts_0_IRQn
#define mPWMCAP1_ENABLED 1U
#define mPWMCAP1_HW TCPWM
#define mPWMCAP1_NUM 1UL
#define mPWMCAP1_MASK (1UL << 1)
#define mPWMCAP1_IRQ tcpwm_interrupts_1_IRQn
#define mPWMCAP2_ENABLED 1U
#define mPWMCAP2_HW TCPWM
#define mPWMCAP2_NUM 2UL
#define mPWMCAP2_MASK (1UL << 2)
#define mPWMCAP2_IRQ tcpwm_interrupts_2_IRQn
#define mPWMCAP3_ENABLED 1U
#define mPWMCAP3_HW TCPWM
#define mPWMCAP3_NUM 3UL
#define mPWMCAP3_MASK (1UL << 3)
#define mPWMCAP3_IRQ tcpwm_interrupts_3_IRQn
#define mPWMCAP_TIMER100KHz_ENABLED 1U
#define mPWMCAP_TIMER100KHz_HW TCPWM
#define mPWMCAP_TIMER100KHz_NUM 4UL
#define mPWMCAP_TIMER100KHz_MASK (1UL << 4)
#define mQuadDec_ENABLED 1U
#define mQuadDec_HW TCPWM
#define mQuadDec_NUM 5UL
#define mQuadDec_MASK (1UL << 5)
#define mPWMO0_ENABLED 1U
#define mPWMO0_HW TCPWM
#define mPWMO0_NUM 6UL
#define mPWMO0_MASK (1UL << 6)
#define mPWMO0_IRQ tcpwm_interrupts_6_IRQn
#define mPWMO1_ENABLED 1U
#define mPWMO1_HW TCPWM
#define mPWMO1_NUM 7UL
#define mPWMO1_MASK (1UL << 7)
#define mPWMO1_IRQ tcpwm_interrupts_7_IRQn

extern void CanFd_RxCallback(bool rxFIFOMsg, uint8_t msgBufOrRxFIFONum, cy_stc_canfd_rx_buffer_t* basemsg);
extern const cy_stc_canfd_bitrate_t mCANFD_nominalBitrateConfig;
extern const cy_stc_canfd_bitrate_t mCANFD_dataBitrateConfig;
extern const cy_stc_canfd_transceiver_delay_compensation_t mCANFD_tdcConfig;
extern const cy_stc_id_filter_t mCANFD_stdIdFilter_0;
extern const cy_stc_id_filter_t mCANFD_stdIdFilter_1;
extern const cy_stc_id_filter_t mCANFD_stdIdFilters[];
extern const cy_stc_canfd_sid_filter_config_t mCANFD_sidFiltersConfig;
extern const cy_stc_canfd_f0_t mCANFD_extIdFilterF0Config_0;
extern const cy_stc_canfd_f0_t mCANFD_extIdFilterF0Config_1;
extern const cy_stc_canfd_f1_t mCANFD_extIdFilterF1Config_0;
extern const cy_stc_canfd_f1_t mCANFD_extIdFilterF1Config_1;
extern const cy_stc_extid_filter_t mCANFD_extIdFilter_0;
extern const cy_stc_extid_filter_t mCANFD_extIdFilter_1;
extern const cy_stc_extid_filter_t mCANFD_extIdFilters[];
extern const cy_stc_canfd_extid_filter_config_t mCANFD_extIdFiltersConfig;
extern const cy_stc_canfd_global_filter_config_t mCANFD_globalFilterConfig;
extern const cy_en_canfd_fifo_config_t mCANFD_rxFifo0Config;
extern const cy_en_canfd_fifo_config_t mCANFD_rxFifo1Config;
extern const cy_stc_canfd_config_t mCANFD_config;
extern cy_stc_canfd_t0_t mCANFD_T0RegisterBuffer_0;
extern cy_stc_canfd_t1_t mCANFD_T1RegisterBuffer_0;
extern uint32_t mCANFD_dataBuffer_0[];
extern cy_stc_canfd_tx_buffer_t mCANFD_txBuffer_0;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mCANFD_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_sar_channel_config_t mADC_channel_0_config;
extern const cy_stc_sar_channel_config_t mADC_channel_1_config;
extern const cy_stc_sar_channel_config_t mADC_channel_2_config;
extern const cy_stc_sar_channel_config_t mADC_channel_3_config;
extern const cy_stc_sar_channel_config_t mADC_channel_4_config;
extern const cy_stc_sar_channel_config_t mADC_channel_5_config;
extern const cy_stc_sar_channel_config_t mADC_channel_6_config;
extern const cy_stc_sar_channel_config_t mADC_channel_7_config;
extern const cy_stc_sar_channel_config_t mADC_channel_8_config;
extern const cy_stc_sar_config_t mADC_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mADC_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_scb_uart_config_t mUART_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mUART_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_scb_spi_config_t mSPIM_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mSPIM_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_scb_i2c_config_t mI2CMA_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mI2CMA_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_scb_i2c_config_t mI2CMB_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mI2CMB_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_scb_i2c_config_t mI2CS_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mI2CS_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_tcpwm_counter_config_t mPWMCAP0_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mPWMCAP0_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_tcpwm_counter_config_t mPWMCAP1_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mPWMCAP1_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_tcpwm_counter_config_t mPWMCAP2_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mPWMCAP2_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_tcpwm_counter_config_t mPWMCAP3_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mPWMCAP3_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_tcpwm_counter_config_t mPWMCAP_TIMER100KHz_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mPWMCAP_TIMER100KHz_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_tcpwm_quaddec_config_t mQuadDec_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mQuadDec_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_tcpwm_pwm_config_t mPWMO0_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mPWMO0_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_tcpwm_pwm_config_t mPWMO1_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t mPWMO1_obj;
#endif /* defined (CY_USING_HAL) */

void init_cycfg_peripherals(void);
void reserve_cycfg_peripherals(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_PERIPHERALS_H */
