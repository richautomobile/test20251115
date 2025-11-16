/*******************************************************************************
 * File Name: cycfg_pins.h
 *
 * Description:
 * Pin configuration
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

#if !defined(CYCFG_PINS_H)
#define CYCFG_PINS_H

#include "cycfg_notices.h"
#include "cy_gpio.h"
#include "cycfg_routing.h"

#if defined (CY_USING_HAL)
#include "cyhal_hwmgr.h"
#endif /* defined (CY_USING_HAL) */

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define PWMCAP0_ENABLED 1U
#define PWMCAP0_PORT GPIO_PRT0
#define PWMCAP0_PORT_NUM 0U
#define PWMCAP0_PIN 0U
#define PWMCAP0_NUM 0U
#define PWMCAP0_DRIVEMODE CY_GPIO_DM_HIGHZ
#define PWMCAP0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_0_HSIOM
    #define ioss_0_port_0_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define PWMCAP0_HSIOM ioss_0_port_0_pin_0_HSIOM
#define PWMCAP0_IRQ ioss_interrupts_gpio_0_IRQn

#if defined (CY_USING_HAL)
#define PWMCAP0_HAL_PORT_PIN P0_0
#define PWMCAP0 P0_0
#define PWMCAP0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define PWMCAP0_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define PWMCAP0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif /* defined (CY_USING_HAL) */

#define PWMCAP1_ENABLED 1U
#define PWMCAP1_PORT GPIO_PRT0
#define PWMCAP1_PORT_NUM 0U
#define PWMCAP1_PIN 1U
#define PWMCAP1_NUM 1U
#define PWMCAP1_DRIVEMODE CY_GPIO_DM_HIGHZ
#define PWMCAP1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_1_HSIOM
    #define ioss_0_port_0_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define PWMCAP1_HSIOM ioss_0_port_0_pin_1_HSIOM
#define PWMCAP1_IRQ ioss_interrupts_gpio_0_IRQn

#if defined (CY_USING_HAL)
#define PWMCAP1_HAL_PORT_PIN P0_1
#define PWMCAP1 P0_1
#define PWMCAP1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define PWMCAP1_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define PWMCAP1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif /* defined (CY_USING_HAL) */

#define CAN_RX_ENABLED 1U
#define CAN_RX_PORT GPIO_PRT0
#define CAN_RX_PORT_NUM 0U
#define CAN_RX_PIN 2U
#define CAN_RX_NUM 2U
#define CAN_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define CAN_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_2_HSIOM
    #define ioss_0_port_0_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CAN_RX_HSIOM ioss_0_port_0_pin_2_HSIOM
#define CAN_RX_IRQ ioss_interrupts_gpio_0_IRQn

#if defined (CY_USING_HAL)
#define CAN_RX_HAL_PORT_PIN P0_2
#define CAN_RX P0_2
#define CAN_RX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define CAN_RX_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define CAN_RX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif /* defined (CY_USING_HAL) */

#define CAN_TX_ENABLED 1U
#define CAN_TX_PORT GPIO_PRT0
#define CAN_TX_PORT_NUM 0U
#define CAN_TX_PIN 3U
#define CAN_TX_NUM 3U
#define CAN_TX_DRIVEMODE CY_GPIO_DM_STRONG
#define CAN_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_0_pin_3_HSIOM
    #define ioss_0_port_0_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CAN_TX_HSIOM ioss_0_port_0_pin_3_HSIOM
#define CAN_TX_IRQ ioss_interrupts_gpio_0_IRQn

#if defined (CY_USING_HAL)
#define CAN_TX_HAL_PORT_PIN P0_3
#define CAN_TX P0_3
#define CAN_TX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define CAN_TX_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define CAN_TX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define ioss_0_port_10_pin_4_ENABLED 1U
#define ioss_0_port_10_pin_4_PORT GPIO_PRT10
#define ioss_0_port_10_pin_4_PORT_NUM 10U
#define ioss_0_port_10_pin_4_PIN 4U
#define ioss_0_port_10_pin_4_NUM 4U
#define ioss_0_port_10_pin_4_DRIVEMODE CY_GPIO_DM_PULLUP
#define ioss_0_port_10_pin_4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_4_HSIOM
    #define ioss_0_port_10_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_10_pin_4_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define ioss_0_port_10_pin_4_HAL_PORT_PIN P10_4
#define ioss_0_port_10_pin_4 P10_4
#define ioss_0_port_10_pin_4_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ioss_0_port_10_pin_4_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define ioss_0_port_10_pin_4_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLUP
#endif /* defined (CY_USING_HAL) */

#define ioss_0_port_10_pin_5_ENABLED 1U
#define ioss_0_port_10_pin_5_PORT GPIO_PRT10
#define ioss_0_port_10_pin_5_PORT_NUM 10U
#define ioss_0_port_10_pin_5_PIN 5U
#define ioss_0_port_10_pin_5_NUM 5U
#define ioss_0_port_10_pin_5_DRIVEMODE CY_GPIO_DM_STRONG
#define ioss_0_port_10_pin_5_INIT_DRIVESTATE 1
#ifndef ioss_0_port_10_pin_5_HSIOM
    #define ioss_0_port_10_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_10_pin_5_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define ioss_0_port_10_pin_5_HAL_PORT_PIN P10_5
#define ioss_0_port_10_pin_5 P10_5
#define ioss_0_port_10_pin_5_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ioss_0_port_10_pin_5_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define ioss_0_port_10_pin_5_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define SPIM_MOSI_ENABLED 1U
#define SPIM_MOSI_PORT GPIO_PRT11
#define SPIM_MOSI_PORT_NUM 11U
#define SPIM_MOSI_PIN 0U
#define SPIM_MOSI_NUM 0U
#define SPIM_MOSI_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPIM_MOSI_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_0_HSIOM
    #define ioss_0_port_11_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define SPIM_MOSI_HSIOM ioss_0_port_11_pin_0_HSIOM
#define SPIM_MOSI_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define SPIM_MOSI_HAL_PORT_PIN P11_0
#define SPIM_MOSI P11_0
#define SPIM_MOSI_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define SPIM_MOSI_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define SPIM_MOSI_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define SPIM_MISO_ENABLED 1U
#define SPIM_MISO_PORT GPIO_PRT11
#define SPIM_MISO_PORT_NUM 11U
#define SPIM_MISO_PIN 1U
#define SPIM_MISO_NUM 1U
#define SPIM_MISO_DRIVEMODE CY_GPIO_DM_HIGHZ
#define SPIM_MISO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_1_HSIOM
    #define ioss_0_port_11_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define SPIM_MISO_HSIOM ioss_0_port_11_pin_1_HSIOM
#define SPIM_MISO_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define SPIM_MISO_HAL_PORT_PIN P11_1
#define SPIM_MISO P11_1
#define SPIM_MISO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define SPIM_MISO_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define SPIM_MISO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif /* defined (CY_USING_HAL) */

#define SPIM_SCL_ENABLED 1U
#define SPIM_SCL_PORT GPIO_PRT11
#define SPIM_SCL_PORT_NUM 11U
#define SPIM_SCL_PIN 2U
#define SPIM_SCL_NUM 2U
#define SPIM_SCL_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPIM_SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_2_HSIOM
    #define ioss_0_port_11_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define SPIM_SCL_HSIOM ioss_0_port_11_pin_2_HSIOM
#define SPIM_SCL_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define SPIM_SCL_HAL_PORT_PIN P11_2
#define SPIM_SCL P11_2
#define SPIM_SCL_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define SPIM_SCL_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define SPIM_SCL_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define SPIM_CS0_ENABLED 1U
#define SPIM_CS0_PORT GPIO_PRT11
#define SPIM_CS0_PORT_NUM 11U
#define SPIM_CS0_PIN 3U
#define SPIM_CS0_NUM 3U
#define SPIM_CS0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPIM_CS0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_3_HSIOM
    #define ioss_0_port_11_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define SPIM_CS0_HSIOM ioss_0_port_11_pin_3_HSIOM
#define SPIM_CS0_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define SPIM_CS0_HAL_PORT_PIN P11_3
#define SPIM_CS0 P11_3
#define SPIM_CS0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define SPIM_CS0_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define SPIM_CS0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define SPIM_CS1_ENABLED 1U
#define SPIM_CS1_PORT GPIO_PRT11
#define SPIM_CS1_PORT_NUM 11U
#define SPIM_CS1_PIN 4U
#define SPIM_CS1_NUM 4U
#define SPIM_CS1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPIM_CS1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_4_HSIOM
    #define ioss_0_port_11_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define SPIM_CS1_HSIOM ioss_0_port_11_pin_4_HSIOM
#define SPIM_CS1_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define SPIM_CS1_HAL_PORT_PIN P11_4
#define SPIM_CS1 P11_4
#define SPIM_CS1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define SPIM_CS1_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define SPIM_CS1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define CYBSP_BTN_ENABLED 1U
#define CYBSP_BTN_PORT GPIO_PRT11
#define CYBSP_BTN_PORT_NUM 11U
#define CYBSP_BTN_PIN 5U
#define CYBSP_BTN_NUM 5U
#define CYBSP_BTN_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_BTN_INIT_DRIVESTATE 1
#ifndef ioss_0_port_11_pin_5_HSIOM
    #define ioss_0_port_11_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_BTN_HSIOM ioss_0_port_11_pin_5_HSIOM
#define CYBSP_BTN_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define CYBSP_BTN_HAL_PORT_PIN P11_5
#define CYBSP_BTN P11_5
#define CYBSP_BTN_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define CYBSP_BTN_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define CYBSP_BTN_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#define CYBSP_LED3 (P12_0)
#endif /* defined (CY_USING_HAL) */

#define UART_RX_ENABLED 1U
#define UART_RX_PORT GPIO_PRT1
#define UART_RX_PORT_NUM 1U
#define UART_RX_PIN 0U
#define UART_RX_NUM 0U
#define UART_RX_DRIVEMODE CY_GPIO_DM_HIGHZ
#define UART_RX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_0_HSIOM
    #define ioss_0_port_1_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define UART_RX_HSIOM ioss_0_port_1_pin_0_HSIOM
#define UART_RX_IRQ ioss_interrupts_gpio_1_IRQn

#if defined (CY_USING_HAL)
#define UART_RX_HAL_PORT_PIN P1_0
#define UART_RX P1_0
#define UART_RX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define UART_RX_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define UART_RX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif /* defined (CY_USING_HAL) */

#define UART_TX_ENABLED 1U
#define UART_TX_PORT GPIO_PRT1
#define UART_TX_PORT_NUM 1U
#define UART_TX_PIN 1U
#define UART_TX_NUM 1U
#define UART_TX_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define UART_TX_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_1_HSIOM
    #define ioss_0_port_1_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define UART_TX_HSIOM ioss_0_port_1_pin_1_HSIOM
#define UART_TX_IRQ ioss_interrupts_gpio_1_IRQn

#if defined (CY_USING_HAL)
#define UART_TX_HAL_PORT_PIN P1_1
#define UART_TX P1_1
#define UART_TX_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define UART_TX_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define UART_TX_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define PWMCAP2_ENABLED 1U
#define PWMCAP2_PORT GPIO_PRT1
#define PWMCAP2_PORT_NUM 1U
#define PWMCAP2_PIN 2U
#define PWMCAP2_NUM 2U
#define PWMCAP2_DRIVEMODE CY_GPIO_DM_HIGHZ
#define PWMCAP2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_2_HSIOM
    #define ioss_0_port_1_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define PWMCAP2_HSIOM ioss_0_port_1_pin_2_HSIOM
#define PWMCAP2_IRQ ioss_interrupts_gpio_1_IRQn

#if defined (CY_USING_HAL)
#define PWMCAP2_HAL_PORT_PIN P1_2
#define PWMCAP2 P1_2
#define PWMCAP2_HAL_IRQ CYHAL_GPIO_IRQ_BOTH
#define PWMCAP2_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define PWMCAP2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif /* defined (CY_USING_HAL) */

#define PWMCAP3_ENABLED 1U
#define PWMCAP3_PORT GPIO_PRT1
#define PWMCAP3_PORT_NUM 1U
#define PWMCAP3_PIN 3U
#define PWMCAP3_NUM 3U
#define PWMCAP3_DRIVEMODE CY_GPIO_DM_HIGHZ
#define PWMCAP3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_3_HSIOM
    #define ioss_0_port_1_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define PWMCAP3_HSIOM ioss_0_port_1_pin_3_HSIOM
#define PWMCAP3_IRQ ioss_interrupts_gpio_1_IRQn

#if defined (CY_USING_HAL)
#define PWMCAP3_HAL_PORT_PIN P1_3
#define PWMCAP3 P1_3
#define PWMCAP3_HAL_IRQ CYHAL_GPIO_IRQ_BOTH
#define PWMCAP3_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define PWMCAP3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif /* defined (CY_USING_HAL) */

#define I2CMB_SCL_ENABLED 1U
#define I2CMB_SCL_PORT GPIO_PRT1
#define I2CMB_SCL_PORT_NUM 1U
#define I2CMB_SCL_PIN 4U
#define I2CMB_SCL_NUM 4U
#define I2CMB_SCL_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define I2CMB_SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_4_HSIOM
    #define ioss_0_port_1_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define I2CMB_SCL_HSIOM ioss_0_port_1_pin_4_HSIOM
#define I2CMB_SCL_IRQ ioss_interrupts_gpio_1_IRQn

#if defined (CY_USING_HAL)
#define I2CMB_SCL_HAL_PORT_PIN P1_4
#define I2CMB_SCL P1_4
#define I2CMB_SCL_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define I2CMB_SCL_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define I2CMB_SCL_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
#endif /* defined (CY_USING_HAL) */

#define I2CMB_SDA_ENABLED 1U
#define I2CMB_SDA_PORT GPIO_PRT1
#define I2CMB_SDA_PORT_NUM 1U
#define I2CMB_SDA_PIN 5U
#define I2CMB_SDA_NUM 5U
#define I2CMB_SDA_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define I2CMB_SDA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_1_pin_5_HSIOM
    #define ioss_0_port_1_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define I2CMB_SDA_HSIOM ioss_0_port_1_pin_5_HSIOM
#define I2CMB_SDA_IRQ ioss_interrupts_gpio_1_IRQn

#if defined (CY_USING_HAL)
#define I2CMB_SDA_HAL_PORT_PIN P1_5
#define I2CMB_SDA P1_5
#define I2CMB_SDA_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define I2CMB_SDA_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define I2CMB_SDA_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
#define VREF (P1_7)
#endif /* defined (CY_USING_HAL) */

#define ADCOTP_ENABLED 1U
#define ADCOTP_PORT GPIO_PRT2
#define ADCOTP_PORT_NUM 2U
#define ADCOTP_PIN 0U
#define ADCOTP_NUM 0U
#define ADCOTP_DRIVEMODE CY_GPIO_DM_ANALOG
#define ADCOTP_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_0_HSIOM
    #define ioss_0_port_2_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define ADCOTP_HSIOM ioss_0_port_2_pin_0_HSIOM
#define ADCOTP_IRQ ioss_interrupts_gpio_2_IRQn

#if defined (CY_USING_HAL)
#define ADCOTP_HAL_PORT_PIN P2_0
#define ADCOTP P2_0
#define ADCOTP_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ADCOTP_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define ADCOTP_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif /* defined (CY_USING_HAL) */

#define ADCSilverVoltage_ENABLED 1U
#define ADCSilverVoltage_PORT GPIO_PRT2
#define ADCSilverVoltage_PORT_NUM 2U
#define ADCSilverVoltage_PIN 1U
#define ADCSilverVoltage_NUM 1U
#define ADCSilverVoltage_DRIVEMODE CY_GPIO_DM_ANALOG
#define ADCSilverVoltage_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_1_HSIOM
    #define ioss_0_port_2_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ADCSilverVoltage_HSIOM ioss_0_port_2_pin_1_HSIOM
#define ADCSilverVoltage_IRQ ioss_interrupts_gpio_2_IRQn

#if defined (CY_USING_HAL)
#define ADCSilverVoltage_HAL_PORT_PIN P2_1
#define ADCSilverVoltage P2_1
#define ADCSilverVoltage_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ADCSilverVoltage_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define ADCSilverVoltage_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif /* defined (CY_USING_HAL) */

#define ADC983MODE0_ENABLED 1U
#define ADC983MODE0_PORT GPIO_PRT2
#define ADC983MODE0_PORT_NUM 2U
#define ADC983MODE0_PIN 2U
#define ADC983MODE0_NUM 2U
#define ADC983MODE0_DRIVEMODE CY_GPIO_DM_ANALOG
#define ADC983MODE0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_2_HSIOM
    #define ioss_0_port_2_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define ADC983MODE0_HSIOM ioss_0_port_2_pin_2_HSIOM
#define ADC983MODE0_IRQ ioss_interrupts_gpio_2_IRQn

#if defined (CY_USING_HAL)
#define ADC983MODE0_HAL_PORT_PIN P2_2
#define ADC983MODE0 P2_2
#define ADC983MODE0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ADC983MODE0_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define ADC983MODE0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif /* defined (CY_USING_HAL) */

#define ADCModeSel3_ENABLED 1U
#define ADCModeSel3_PORT GPIO_PRT2
#define ADCModeSel3_PORT_NUM 2U
#define ADCModeSel3_PIN 3U
#define ADCModeSel3_NUM 3U
#define ADCModeSel3_DRIVEMODE CY_GPIO_DM_ANALOG
#define ADCModeSel3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_3_HSIOM
    #define ioss_0_port_2_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define ADCModeSel3_HSIOM ioss_0_port_2_pin_3_HSIOM
#define ADCModeSel3_IRQ ioss_interrupts_gpio_2_IRQn

#if defined (CY_USING_HAL)
#define ADCModeSel3_HAL_PORT_PIN P2_3
#define ADCModeSel3 P2_3
#define ADCModeSel3_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ADCModeSel3_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define ADCModeSel3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif /* defined (CY_USING_HAL) */

#define ADCModeSel4_ENABLED 1U
#define ADCModeSel4_PORT GPIO_PRT2
#define ADCModeSel4_PORT_NUM 2U
#define ADCModeSel4_PIN 4U
#define ADCModeSel4_NUM 4U
#define ADCModeSel4_DRIVEMODE CY_GPIO_DM_ANALOG
#define ADCModeSel4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_4_HSIOM
    #define ioss_0_port_2_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define ADCModeSel4_HSIOM ioss_0_port_2_pin_4_HSIOM
#define ADCModeSel4_IRQ ioss_interrupts_gpio_2_IRQn

#if defined (CY_USING_HAL)
#define ADCModeSel4_HAL_PORT_PIN P2_4
#define ADCModeSel4 P2_4
#define ADCModeSel4_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ADCModeSel4_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define ADCModeSel4_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif /* defined (CY_USING_HAL) */

#define ADCModeSel5_ENABLED 1U
#define ADCModeSel5_PORT GPIO_PRT2
#define ADCModeSel5_PORT_NUM 2U
#define ADCModeSel5_PIN 5U
#define ADCModeSel5_NUM 5U
#define ADCModeSel5_DRIVEMODE CY_GPIO_DM_ANALOG
#define ADCModeSel5_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_5_HSIOM
    #define ioss_0_port_2_pin_5_HSIOM HSIOM_SEL_GPIO
#endif
#define ADCModeSel5_HSIOM ioss_0_port_2_pin_5_HSIOM
#define ADCModeSel5_IRQ ioss_interrupts_gpio_2_IRQn

#if defined (CY_USING_HAL)
#define ADCModeSel5_HAL_PORT_PIN P2_5
#define ADCModeSel5 P2_5
#define ADCModeSel5_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ADCModeSel5_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define ADCModeSel5_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif /* defined (CY_USING_HAL) */

#define ADCModeSel6_ENABLED 1U
#define ADCModeSel6_PORT GPIO_PRT2
#define ADCModeSel6_PORT_NUM 2U
#define ADCModeSel6_PIN 6U
#define ADCModeSel6_NUM 6U
#define ADCModeSel6_DRIVEMODE CY_GPIO_DM_ANALOG
#define ADCModeSel6_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_6_HSIOM
    #define ioss_0_port_2_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define ADCModeSel6_HSIOM ioss_0_port_2_pin_6_HSIOM
#define ADCModeSel6_IRQ ioss_interrupts_gpio_2_IRQn

#if defined (CY_USING_HAL)
#define ADCModeSel6_HAL_PORT_PIN P2_6
#define ADCModeSel6 P2_6
#define ADCModeSel6_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ADCModeSel6_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define ADCModeSel6_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif /* defined (CY_USING_HAL) */

#define ADCModeSel7_ENABLED 1U
#define ADCModeSel7_PORT GPIO_PRT2
#define ADCModeSel7_PORT_NUM 2U
#define ADCModeSel7_PIN 7U
#define ADCModeSel7_NUM 7U
#define ADCModeSel7_DRIVEMODE CY_GPIO_DM_ANALOG
#define ADCModeSel7_INIT_DRIVESTATE 1
#ifndef ioss_0_port_2_pin_7_HSIOM
    #define ioss_0_port_2_pin_7_HSIOM HSIOM_SEL_GPIO
#endif
#define ADCModeSel7_HSIOM ioss_0_port_2_pin_7_HSIOM
#define ADCModeSel7_IRQ ioss_interrupts_gpio_2_IRQn

#if defined (CY_USING_HAL)
#define ADCModeSel7_HAL_PORT_PIN P2_7
#define ADCModeSel7 P2_7
#define ADCModeSel7_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ADCModeSel7_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define ADCModeSel7_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_ANALOG
#endif /* defined (CY_USING_HAL) */

#define CYBSP_SWDIO_ENABLED 1U
#define CYBSP_SWDIO_PORT GPIO_PRT3
#define CYBSP_SWDIO_PORT_NUM 3U
#define CYBSP_SWDIO_PIN 2U
#define CYBSP_SWDIO_NUM 2U
#define CYBSP_SWDIO_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SWDIO_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_2_HSIOM
    #define ioss_0_port_3_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDIO_HSIOM ioss_0_port_3_pin_2_HSIOM
#define CYBSP_SWDIO_IRQ ioss_interrupts_gpio_3_IRQn

#if defined (CY_USING_HAL)
#define CYBSP_SWDIO_HAL_PORT_PIN P3_2
#define CYBSP_SWDIO P3_2
#define CYBSP_SWDIO_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define CYBSP_SWDIO_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define CYBSP_SWDIO_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define CYBSP_SWDCK_ENABLED 1U
#define CYBSP_SWDCK_PORT GPIO_PRT3
#define CYBSP_SWDCK_PORT_NUM 3U
#define CYBSP_SWDCK_PIN 3U
#define CYBSP_SWDCK_NUM 3U
#define CYBSP_SWDCK_DRIVEMODE CY_GPIO_DM_STRONG
#define CYBSP_SWDCK_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_3_HSIOM
    #define ioss_0_port_3_pin_3_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_SWDCK_HSIOM ioss_0_port_3_pin_3_HSIOM
#define CYBSP_SWDCK_IRQ ioss_interrupts_gpio_3_IRQn

#if defined (CY_USING_HAL)
#define CYBSP_SWDCK_HAL_PORT_PIN P3_3
#define CYBSP_SWDCK P3_3
#define CYBSP_SWDCK_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define CYBSP_SWDCK_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define CYBSP_SWDCK_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define ioss_0_port_3_pin_4_ENABLED 1U
#define ioss_0_port_3_pin_4_PORT GPIO_PRT3
#define ioss_0_port_3_pin_4_PORT_NUM 3U
#define ioss_0_port_3_pin_4_PIN 4U
#define ioss_0_port_3_pin_4_NUM 4U
#define ioss_0_port_3_pin_4_DRIVEMODE CY_GPIO_DM_HIGHZ
#define ioss_0_port_3_pin_4_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_4_HSIOM
    #define ioss_0_port_3_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_3_pin_4_IRQ ioss_interrupts_gpio_3_IRQn

#if defined (CY_USING_HAL)
#define ioss_0_port_3_pin_4_HAL_PORT_PIN P3_4
#define ioss_0_port_3_pin_4 P3_4
#define ioss_0_port_3_pin_4_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ioss_0_port_3_pin_4_HAL_DIR CYHAL_GPIO_DIR_INPUT 
#define ioss_0_port_3_pin_4_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_NONE
#endif /* defined (CY_USING_HAL) */

#define SPIM_CS3_ENABLED 1U
#define SPIM_CS3_PORT GPIO_PRT3
#define SPIM_CS3_PORT_NUM 3U
#define SPIM_CS3_PIN 6U
#define SPIM_CS3_NUM 6U
#define SPIM_CS3_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define SPIM_CS3_INIT_DRIVESTATE 1
#ifndef ioss_0_port_3_pin_6_HSIOM
    #define ioss_0_port_3_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define SPIM_CS3_HSIOM ioss_0_port_3_pin_6_HSIOM
#define SPIM_CS3_IRQ ioss_interrupts_gpio_3_IRQn

#if defined (CY_USING_HAL)
#define SPIM_CS3_HAL_PORT_PIN P3_6
#define SPIM_CS3 P3_6
#define SPIM_CS3_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define SPIM_CS3_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define SPIM_CS3_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define ioss_0_port_4_pin_6_ENABLED 1U
#define ioss_0_port_4_pin_6_PORT GPIO_PRT4
#define ioss_0_port_4_pin_6_PORT_NUM 4U
#define ioss_0_port_4_pin_6_PIN 6U
#define ioss_0_port_4_pin_6_NUM 6U
#define ioss_0_port_4_pin_6_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define ioss_0_port_4_pin_6_INIT_DRIVESTATE 1
#ifndef ioss_0_port_4_pin_6_HSIOM
    #define ioss_0_port_4_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_4_pin_6_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define ioss_0_port_4_pin_6_HAL_PORT_PIN P4_6
#define ioss_0_port_4_pin_6 P4_6
#define ioss_0_port_4_pin_6_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ioss_0_port_4_pin_6_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define ioss_0_port_4_pin_6_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define I2CMA_SCL_ENABLED 1U
#define I2CMA_SCL_PORT GPIO_PRT5
#define I2CMA_SCL_PORT_NUM 5U
#define I2CMA_SCL_PIN 0U
#define I2CMA_SCL_NUM 0U
#define I2CMA_SCL_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define I2CMA_SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_0_HSIOM
    #define ioss_0_port_5_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define I2CMA_SCL_HSIOM ioss_0_port_5_pin_0_HSIOM
#define I2CMA_SCL_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define I2CMA_SCL_HAL_PORT_PIN P5_0
#define I2CMA_SCL P5_0
#define I2CMA_SCL_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define I2CMA_SCL_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define I2CMA_SCL_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
#endif /* defined (CY_USING_HAL) */

#define I2CMA_SDA_ENABLED 1U
#define I2CMA_SDA_PORT GPIO_PRT5
#define I2CMA_SDA_PORT_NUM 5U
#define I2CMA_SDA_PIN 1U
#define I2CMA_SDA_NUM 1U
#define I2CMA_SDA_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define I2CMA_SDA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_1_HSIOM
    #define ioss_0_port_5_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define I2CMA_SDA_HSIOM ioss_0_port_5_pin_1_HSIOM
#define I2CMA_SDA_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define I2CMA_SDA_HAL_PORT_PIN P5_1
#define I2CMA_SDA P5_1
#define I2CMA_SDA_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define I2CMA_SDA_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define I2CMA_SDA_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
#endif /* defined (CY_USING_HAL) */

#define PWMO0_ENABLED 1U
#define PWMO0_PORT GPIO_PRT5
#define PWMO0_PORT_NUM 5U
#define PWMO0_PIN 4U
#define PWMO0_NUM 4U
#define PWMO0_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define PWMO0_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_4_HSIOM
    #define ioss_0_port_5_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define PWMO0_HSIOM ioss_0_port_5_pin_4_HSIOM
#define PWMO0_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define PWMO0_HAL_PORT_PIN P5_4
#define PWMO0 P5_4
#define PWMO0_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define PWMO0_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define PWMO0_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define PWMO1_ENABLED 1U
#define PWMO1_PORT GPIO_PRT5
#define PWMO1_PORT_NUM 5U
#define PWMO1_PIN 6U
#define PWMO1_NUM 6U
#define PWMO1_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define PWMO1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_5_pin_6_HSIOM
    #define ioss_0_port_5_pin_6_HSIOM HSIOM_SEL_GPIO
#endif
#define PWMO1_HSIOM ioss_0_port_5_pin_6_HSIOM
#define PWMO1_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define PWMO1_HAL_PORT_PIN P5_6
#define PWMO1 P5_6
#define PWMO1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define PWMO1_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define PWMO1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define ioss_0_port_6_pin_1_ENABLED 1U
#define ioss_0_port_6_pin_1_PORT GPIO_PRT6
#define ioss_0_port_6_pin_1_PORT_NUM 6U
#define ioss_0_port_6_pin_1_PIN 1U
#define ioss_0_port_6_pin_1_NUM 1U
#define ioss_0_port_6_pin_1_DRIVEMODE CY_GPIO_DM_PULLUP
#define ioss_0_port_6_pin_1_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_1_HSIOM
    #define ioss_0_port_6_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_6_pin_1_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define ioss_0_port_6_pin_1_HAL_PORT_PIN P6_1
#define ioss_0_port_6_pin_1 P6_1
#define ioss_0_port_6_pin_1_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ioss_0_port_6_pin_1_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define ioss_0_port_6_pin_1_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_PULLUP
#endif /* defined (CY_USING_HAL) */

#define ioss_0_port_6_pin_2_ENABLED 1U
#define ioss_0_port_6_pin_2_PORT GPIO_PRT6
#define ioss_0_port_6_pin_2_PORT_NUM 6U
#define ioss_0_port_6_pin_2_PIN 2U
#define ioss_0_port_6_pin_2_NUM 2U
#define ioss_0_port_6_pin_2_DRIVEMODE CY_GPIO_DM_STRONG
#define ioss_0_port_6_pin_2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_2_HSIOM
    #define ioss_0_port_6_pin_2_HSIOM HSIOM_SEL_GPIO
#endif
#define ioss_0_port_6_pin_2_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define ioss_0_port_6_pin_2_HAL_PORT_PIN P6_2
#define ioss_0_port_6_pin_2 P6_2
#define ioss_0_port_6_pin_2_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define ioss_0_port_6_pin_2_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define ioss_0_port_6_pin_2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#endif /* defined (CY_USING_HAL) */

#define CYBSP_LED2_ENABLED 1U
#define CYBSP_LED2_PORT GPIO_PRT6
#define CYBSP_LED2_PORT_NUM 6U
#define CYBSP_LED2_PIN 4U
#define CYBSP_LED2_NUM 4U
#define CYBSP_LED2_DRIVEMODE CY_GPIO_DM_STRONG_IN_OFF
#define CYBSP_LED2_INIT_DRIVESTATE 1
#ifndef ioss_0_port_6_pin_4_HSIOM
    #define ioss_0_port_6_pin_4_HSIOM HSIOM_SEL_GPIO
#endif
#define CYBSP_LED2_HSIOM ioss_0_port_6_pin_4_HSIOM
#define CYBSP_LED2_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define CYBSP_LED2_HAL_PORT_PIN P6_4
#define CYBSP_LED2 P6_4
#define CYBSP_LED2_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define CYBSP_LED2_HAL_DIR CYHAL_GPIO_DIR_OUTPUT 
#define CYBSP_LED2_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_STRONG
#define CYBSP_LED1 (P7_3)
#endif /* defined (CY_USING_HAL) */

#define I2CS_SCL_ENABLED 1U
#define I2CS_SCL_PORT GPIO_PRT8
#define I2CS_SCL_PORT_NUM 8U
#define I2CS_SCL_PIN 0U
#define I2CS_SCL_NUM 0U
#define I2CS_SCL_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define I2CS_SCL_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_0_HSIOM
    #define ioss_0_port_8_pin_0_HSIOM HSIOM_SEL_GPIO
#endif
#define I2CS_SCL_HSIOM ioss_0_port_8_pin_0_HSIOM
#define I2CS_SCL_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define I2CS_SCL_HAL_PORT_PIN P8_0
#define I2CS_SCL P8_0
#define I2CS_SCL_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define I2CS_SCL_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define I2CS_SCL_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
#endif /* defined (CY_USING_HAL) */

#define I2CS_SDA_ENABLED 1U
#define I2CS_SDA_PORT GPIO_PRT8
#define I2CS_SDA_PORT_NUM 8U
#define I2CS_SDA_PIN 1U
#define I2CS_SDA_NUM 1U
#define I2CS_SDA_DRIVEMODE CY_GPIO_DM_OD_DRIVESLOW
#define I2CS_SDA_INIT_DRIVESTATE 1
#ifndef ioss_0_port_8_pin_1_HSIOM
    #define ioss_0_port_8_pin_1_HSIOM HSIOM_SEL_GPIO
#endif
#define I2CS_SDA_HSIOM ioss_0_port_8_pin_1_HSIOM
#define I2CS_SDA_IRQ ioss_interrupt_gpio_IRQn

#if defined (CY_USING_HAL)
#define I2CS_SDA_HAL_PORT_PIN P8_1
#define I2CS_SDA P8_1
#define I2CS_SDA_HAL_IRQ CYHAL_GPIO_IRQ_NONE
#define I2CS_SDA_HAL_DIR CYHAL_GPIO_DIR_BIDIRECTIONAL 
#define I2CS_SDA_HAL_DRIVEMODE CYHAL_GPIO_DRIVE_OPENDRAINDRIVESLOW
#define DBG_UART (P8_3)
#define Enc1_AP (P9_0)
#define Enc1_BP (P9_1)
#define Enc2_AP (P9_2)
#define Enc2_BP (P9_3)
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t PWMCAP0_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t PWMCAP0_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t PWMCAP1_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t PWMCAP1_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t CAN_RX_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CAN_RX_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t CAN_TX_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CAN_TX_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_4_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ioss_0_port_10_pin_4_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_5_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ioss_0_port_10_pin_5_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t SPIM_MOSI_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t SPIM_MOSI_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t SPIM_MISO_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t SPIM_MISO_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t SPIM_SCL_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t SPIM_SCL_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t SPIM_CS0_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t SPIM_CS0_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t SPIM_CS1_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t SPIM_CS1_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t CYBSP_BTN_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_BTN_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t UART_RX_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t UART_RX_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t UART_TX_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t UART_TX_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t PWMCAP2_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t PWMCAP2_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t PWMCAP3_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t PWMCAP3_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t I2CMB_SCL_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t I2CMB_SCL_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t I2CMB_SDA_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t I2CMB_SDA_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ADCOTP_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ADCOTP_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ADCSilverVoltage_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ADCSilverVoltage_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ADC983MODE0_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ADC983MODE0_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ADCModeSel3_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ADCModeSel3_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ADCModeSel4_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ADCModeSel4_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ADCModeSel5_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ADCModeSel5_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ADCModeSel6_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ADCModeSel6_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ADCModeSel7_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ADCModeSel7_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_SWDIO_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t CYBSP_SWDCK_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_SWDCK_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ioss_0_port_3_pin_4_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ioss_0_port_3_pin_4_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t SPIM_CS3_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t SPIM_CS3_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ioss_0_port_4_pin_6_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ioss_0_port_4_pin_6_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t I2CMA_SCL_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t I2CMA_SCL_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t I2CMA_SDA_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t I2CMA_SDA_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t PWMO0_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t PWMO0_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t PWMO1_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t PWMO1_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ioss_0_port_6_pin_1_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ioss_0_port_6_pin_1_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t ioss_0_port_6_pin_2_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t ioss_0_port_6_pin_2_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t CYBSP_LED2_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CYBSP_LED2_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t I2CS_SCL_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t I2CS_SCL_obj;
#endif /* defined (CY_USING_HAL) */

extern const cy_stc_gpio_pin_config_t I2CS_SDA_config;

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t I2CS_SDA_obj;
#endif /* defined (CY_USING_HAL) */

void init_cycfg_pins(void);
void reserve_cycfg_pins(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_PINS_H */
