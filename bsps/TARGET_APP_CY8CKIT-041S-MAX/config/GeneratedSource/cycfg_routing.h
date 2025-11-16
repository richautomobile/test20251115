/*******************************************************************************
 * File Name: cycfg_routing.h
 *
 * Description:
 * Establishes all necessary connections between hardware elements.
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

#if !defined(CYCFG_ROUTING_H)
#define CYCFG_ROUTING_H

#include "cycfg_notices.h"

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define ioss_0_port_0_pin_0_HSIOM P0_0_TCPWM_TR_IN0
#define ioss_0_port_0_pin_1_HSIOM P0_1_TCPWM_TR_IN1
#define ioss_0_port_0_pin_2_HSIOM P0_2_CANFD_TTCAN_RX0
#define ioss_0_port_0_pin_3_HSIOM P0_3_CANFD_TTCAN_TX0
#define ioss_0_port_1_pin_0_HSIOM P1_0_SCB0_UART_RX
#define ioss_0_port_1_pin_1_HSIOM P1_1_SCB0_UART_TX
#define ioss_0_port_1_pin_2_HSIOM P1_2_TCPWM_TR_IN2
#define ioss_0_port_1_pin_3_HSIOM P1_3_TCPWM_TR_IN3
#define ioss_0_port_1_pin_4_HSIOM P1_4_SCB3_I2C_SCL
#define ioss_0_port_1_pin_5_HSIOM P1_5_SCB3_I2C_SDA
#define ioss_0_port_2_pin_0_ANALOG P2_0_PASS0_SARMUX_PADS0
#define ioss_0_port_2_pin_1_ANALOG P2_1_PASS0_SARMUX_PADS1
#define ioss_0_port_2_pin_2_ANALOG P2_2_PASS0_SARMUX_PADS2
#define ioss_0_port_2_pin_3_ANALOG P2_3_PASS0_SARMUX_PADS3
#define ioss_0_port_2_pin_4_ANALOG P2_4_PASS0_SARMUX_PADS4
#define ioss_0_port_2_pin_5_ANALOG P2_5_PASS0_SARMUX_PADS5
#define ioss_0_port_2_pin_6_ANALOG P2_6_PASS0_SARMUX_PADS6
#define ioss_0_port_2_pin_7_ANALOG P2_7_PASS0_SARMUX_PADS7
#define ioss_0_port_3_pin_2_HSIOM P3_2_CPUSS_SWD_DATA
#define ioss_0_port_3_pin_3_HSIOM P3_3_CPUSS_SWD_CLK
#define ioss_0_port_3_pin_6_HSIOM P3_6_SCB1_SPI_SELECT3
#define ioss_0_port_5_pin_0_HSIOM P5_0_SCB2_I2C_SCL
#define ioss_0_port_5_pin_1_HSIOM P5_1_SCB2_I2C_SDA
#define ioss_0_port_5_pin_4_HSIOM P5_4_TCPWM_LINE6
#define ioss_0_port_5_pin_6_HSIOM P5_6_TCPWM_LINE7
#define ioss_0_port_8_pin_0_HSIOM P8_0_SCB4_I2C_SCL
#define ioss_0_port_8_pin_1_HSIOM P8_1_SCB4_I2C_SDA
#define ioss_0_port_11_pin_0_HSIOM P11_0_SCB1_SPI_MOSI
#define ioss_0_port_11_pin_1_HSIOM P11_1_SCB1_SPI_MISO
#define ioss_0_port_11_pin_2_HSIOM P11_2_SCB1_SPI_CLK
#define ioss_0_port_11_pin_3_HSIOM P11_3_SCB1_SPI_SELECT0
#define ioss_0_port_11_pin_4_HSIOM P11_4_SCB1_SPI_SELECT1
#define ioss_0_port_11_pin_5_HSIOM P11_5_SCB1_SPI_SELECT2
#define mADC_tr_sar_out_0_TRIGGER_IN TRIG0_IN_PASS0_TR_SAR_OUT
#define mDMAADC_tr_in_0_TRIGGER_OUT TRIG0_OUT_CPUSS_DMAC_TR_IN0
#define mPWMCAP0_count_0_TRIGGER_OUT TRIG1_OUT_TCPWM_TR_IN13
#define mPWMCAP1_count_0_TRIGGER_OUT TRIG1_OUT_TCPWM_TR_IN13
#define mPWMCAP2_count_0_TRIGGER_OUT TRIG1_OUT_TCPWM_TR_IN13
#define mPWMCAP3_count_0_TRIGGER_OUT TRIG1_OUT_TCPWM_TR_IN13
#define mPWMCAP_TIMER100KHz_tr_overflow_0_TRIGGER_IN TRIG1_IN_TCPWM_TR_OVERFLOW4
#define SAR0_VPLUS0_PIN 0
#define SAR0_VPLUS0_PORT 0
#define SAR0_VPLUS1_PIN 1
#define SAR0_VPLUS1_PORT 0
#define SAR0_VPLUS2_PIN 2
#define SAR0_VPLUS2_PORT 0
#define SAR0_VPLUS3_PIN 3
#define SAR0_VPLUS3_PORT 0
#define SAR0_VPLUS4_PIN 4
#define SAR0_VPLUS4_PORT 0
#define SAR0_VPLUS5_PIN 5
#define SAR0_VPLUS5_PORT 0
#define SAR0_VPLUS6_PIN 6
#define SAR0_VPLUS6_PORT 0
#define SAR0_VPLUS7_PIN 7
#define SAR0_VPLUS7_PORT 0
#define SAR0_VPLUS8_PIN 0
#define SAR0_VPLUS8_PORT 7
#define TCPWM_CNT0_CAPTURE_VALUE 0x2
#define TCPWM_CNT0_COUNT_VALUE 0xf
#define TCPWM_CNT0_RELOAD_VALUE 0x2
#define TCPWM_CNT1_CAPTURE_VALUE 0x3
#define TCPWM_CNT1_COUNT_VALUE 0xf
#define TCPWM_CNT1_RELOAD_VALUE 0x3
#define TCPWM_CNT2_CAPTURE_VALUE 0x4
#define TCPWM_CNT2_COUNT_VALUE 0xf
#define TCPWM_CNT2_RELOAD_VALUE 0x4
#define TCPWM_CNT3_CAPTURE_VALUE 0x5
#define TCPWM_CNT3_COUNT_VALUE 0xf
#define TCPWM_CNT3_RELOAD_VALUE 0x5
#define TCPWM_CNT5_COUNT_VALUE 0x4
#define TCPWM_CNT5_START_VALUE 0x5

void init_cycfg_routing(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_ROUTING_H */
