/*******************************************************************************
 * File Name: cycfg_pins.c
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

#include "cycfg_pins.h"

const cy_stc_gpio_pin_config_t PWMCAP0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = PWMCAP0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t PWMCAP0_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = PWMCAP0_PORT_NUM,
    .channel_num = PWMCAP0_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t PWMCAP1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = PWMCAP1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t PWMCAP1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = PWMCAP1_PORT_NUM,
    .channel_num = PWMCAP1_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t CAN_RX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = CAN_RX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CAN_RX_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CAN_RX_PORT_NUM,
    .channel_num = CAN_RX_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t CAN_TX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CAN_TX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CAN_TX_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CAN_TX_PORT_NUM,
    .channel_num = CAN_TX_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_4_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = ioss_0_port_10_pin_4_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ioss_0_port_10_pin_4_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ioss_0_port_10_pin_4_PORT_NUM,
    .channel_num = ioss_0_port_10_pin_4_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ioss_0_port_10_pin_5_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = ioss_0_port_10_pin_5_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ioss_0_port_10_pin_5_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ioss_0_port_10_pin_5_PORT_NUM,
    .channel_num = ioss_0_port_10_pin_5_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t SPIM_MOSI_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPIM_MOSI_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t SPIM_MOSI_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = SPIM_MOSI_PORT_NUM,
    .channel_num = SPIM_MOSI_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t SPIM_MISO_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = SPIM_MISO_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t SPIM_MISO_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = SPIM_MISO_PORT_NUM,
    .channel_num = SPIM_MISO_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t SPIM_SCL_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPIM_SCL_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t SPIM_SCL_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = SPIM_SCL_PORT_NUM,
    .channel_num = SPIM_SCL_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t SPIM_CS0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPIM_CS0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t SPIM_CS0_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = SPIM_CS0_PORT_NUM,
    .channel_num = SPIM_CS0_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t SPIM_CS1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPIM_CS1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t SPIM_CS1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = SPIM_CS1_PORT_NUM,
    .channel_num = SPIM_CS1_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t CYBSP_BTN_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_BTN_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_BTN_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_BTN_PORT_NUM,
    .channel_num = CYBSP_BTN_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t UART_RX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = UART_RX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t UART_RX_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = UART_RX_PORT_NUM,
    .channel_num = UART_RX_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t UART_TX_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = UART_TX_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t UART_TX_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = UART_TX_PORT_NUM,
    .channel_num = UART_TX_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t PWMCAP2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = PWMCAP2_HSIOM,
    .intEdge = CY_GPIO_INTR_BOTH,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t PWMCAP2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = PWMCAP2_PORT_NUM,
    .channel_num = PWMCAP2_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t PWMCAP3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = PWMCAP3_HSIOM,
    .intEdge = CY_GPIO_INTR_BOTH,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t PWMCAP3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = PWMCAP3_PORT_NUM,
    .channel_num = PWMCAP3_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t I2CMB_SCL_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2CMB_SCL_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t I2CMB_SCL_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = I2CMB_SCL_PORT_NUM,
    .channel_num = I2CMB_SCL_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t I2CMB_SDA_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2CMB_SDA_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t I2CMB_SDA_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = I2CMB_SDA_PORT_NUM,
    .channel_num = I2CMB_SDA_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ADCOTP_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ADCOTP_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ADCOTP_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ADCOTP_PORT_NUM,
    .channel_num = ADCOTP_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ADCSilverVoltage_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ADCSilverVoltage_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ADCSilverVoltage_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ADCSilverVoltage_PORT_NUM,
    .channel_num = ADCSilverVoltage_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ADC983MODE0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ADC983MODE0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ADC983MODE0_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ADC983MODE0_PORT_NUM,
    .channel_num = ADC983MODE0_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ADCModeSel3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ADCModeSel3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ADCModeSel3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ADCModeSel3_PORT_NUM,
    .channel_num = ADCModeSel3_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ADCModeSel4_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ADCModeSel4_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ADCModeSel4_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ADCModeSel4_PORT_NUM,
    .channel_num = ADCModeSel4_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ADCModeSel5_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ADCModeSel5_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ADCModeSel5_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ADCModeSel5_PORT_NUM,
    .channel_num = ADCModeSel5_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ADCModeSel6_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ADCModeSel6_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ADCModeSel6_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ADCModeSel6_PORT_NUM,
    .channel_num = ADCModeSel6_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ADCModeSel7_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_ANALOG,
    .hsiom = ADCModeSel7_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ADCModeSel7_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ADCModeSel7_PORT_NUM,
    .channel_num = ADCModeSel7_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t CYBSP_SWDIO_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SWDIO_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_SWDIO_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SWDIO_PORT_NUM,
    .channel_num = CYBSP_SWDIO_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t CYBSP_SWDCK_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = CYBSP_SWDCK_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_SWDCK_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_SWDCK_PORT_NUM,
    .channel_num = CYBSP_SWDCK_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ioss_0_port_3_pin_4_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_HIGHZ,
    .hsiom = ioss_0_port_3_pin_4_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ioss_0_port_3_pin_4_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ioss_0_port_3_pin_4_PORT_NUM,
    .channel_num = ioss_0_port_3_pin_4_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t SPIM_CS3_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = SPIM_CS3_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t SPIM_CS3_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = SPIM_CS3_PORT_NUM,
    .channel_num = SPIM_CS3_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ioss_0_port_4_pin_6_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = ioss_0_port_4_pin_6_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ioss_0_port_4_pin_6_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ioss_0_port_4_pin_6_PORT_NUM,
    .channel_num = ioss_0_port_4_pin_6_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t I2CMA_SCL_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2CMA_SCL_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t I2CMA_SCL_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = I2CMA_SCL_PORT_NUM,
    .channel_num = I2CMA_SCL_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t I2CMA_SDA_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2CMA_SDA_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t I2CMA_SDA_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = I2CMA_SDA_PORT_NUM,
    .channel_num = I2CMA_SDA_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t PWMO0_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = PWMO0_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t PWMO0_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = PWMO0_PORT_NUM,
    .channel_num = PWMO0_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t PWMO1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = PWMO1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t PWMO1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = PWMO1_PORT_NUM,
    .channel_num = PWMO1_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ioss_0_port_6_pin_1_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_PULLUP,
    .hsiom = ioss_0_port_6_pin_1_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ioss_0_port_6_pin_1_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ioss_0_port_6_pin_1_PORT_NUM,
    .channel_num = ioss_0_port_6_pin_1_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t ioss_0_port_6_pin_2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG,
    .hsiom = ioss_0_port_6_pin_2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t ioss_0_port_6_pin_2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = ioss_0_port_6_pin_2_PORT_NUM,
    .channel_num = ioss_0_port_6_pin_2_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t CYBSP_LED2_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_STRONG_IN_OFF,
    .hsiom = CYBSP_LED2_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CYBSP_LED2_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = CYBSP_LED2_PORT_NUM,
    .channel_num = CYBSP_LED2_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t I2CS_SCL_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2CS_SCL_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t I2CS_SCL_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = I2CS_SCL_PORT_NUM,
    .channel_num = I2CS_SCL_PIN,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_gpio_pin_config_t I2CS_SDA_config =
{
    .outVal = 1,
    .driveMode = CY_GPIO_DM_OD_DRIVESLOW,
    .hsiom = I2CS_SDA_HSIOM,
    .intEdge = CY_GPIO_INTR_DISABLE,
    .vtrip = CY_GPIO_VTRIP_CMOS,
    .slewRate = CY_GPIO_SLEW_FAST,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t I2CS_SDA_obj =
{
    .type = CYHAL_RSC_GPIO,
    .block_num = I2CS_SDA_PORT_NUM,
    .channel_num = I2CS_SDA_PIN,
};
#endif /* defined (CY_USING_HAL) */

void init_cycfg_pins(void)
{
    Cy_GPIO_Pin_Init(PWMCAP0_PORT, PWMCAP0_PIN, &PWMCAP0_config);
    Cy_GPIO_Pin_Init(PWMCAP1_PORT, PWMCAP1_PIN, &PWMCAP1_config);
    Cy_GPIO_Pin_Init(CAN_RX_PORT, CAN_RX_PIN, &CAN_RX_config);
    Cy_GPIO_Pin_Init(CAN_TX_PORT, CAN_TX_PIN, &CAN_TX_config);
    Cy_GPIO_Pin_Init(ioss_0_port_10_pin_4_PORT, ioss_0_port_10_pin_4_PIN, &ioss_0_port_10_pin_4_config);
    Cy_GPIO_Pin_Init(ioss_0_port_10_pin_5_PORT, ioss_0_port_10_pin_5_PIN, &ioss_0_port_10_pin_5_config);
    Cy_GPIO_Pin_Init(SPIM_MOSI_PORT, SPIM_MOSI_PIN, &SPIM_MOSI_config);
    Cy_GPIO_Pin_Init(SPIM_MISO_PORT, SPIM_MISO_PIN, &SPIM_MISO_config);
    Cy_GPIO_Pin_Init(SPIM_SCL_PORT, SPIM_SCL_PIN, &SPIM_SCL_config);
    Cy_GPIO_Pin_Init(SPIM_CS0_PORT, SPIM_CS0_PIN, &SPIM_CS0_config);
    Cy_GPIO_Pin_Init(SPIM_CS1_PORT, SPIM_CS1_PIN, &SPIM_CS1_config);
    Cy_GPIO_Pin_Init(CYBSP_BTN_PORT, CYBSP_BTN_PIN, &CYBSP_BTN_config);
    Cy_GPIO_Pin_Init(UART_RX_PORT, UART_RX_PIN, &UART_RX_config);
    Cy_GPIO_Pin_Init(UART_TX_PORT, UART_TX_PIN, &UART_TX_config);
    Cy_GPIO_Pin_Init(PWMCAP2_PORT, PWMCAP2_PIN, &PWMCAP2_config);
    Cy_GPIO_Pin_Init(PWMCAP3_PORT, PWMCAP3_PIN, &PWMCAP3_config);
    Cy_GPIO_Pin_Init(I2CMB_SCL_PORT, I2CMB_SCL_PIN, &I2CMB_SCL_config);
    Cy_GPIO_Pin_Init(I2CMB_SDA_PORT, I2CMB_SDA_PIN, &I2CMB_SDA_config);
    Cy_GPIO_Pin_Init(ADCOTP_PORT, ADCOTP_PIN, &ADCOTP_config);
    Cy_GPIO_Pin_Init(ADCSilverVoltage_PORT, ADCSilverVoltage_PIN, &ADCSilverVoltage_config);
    Cy_GPIO_Pin_Init(ADC983MODE0_PORT, ADC983MODE0_PIN, &ADC983MODE0_config);
    Cy_GPIO_Pin_Init(ADCModeSel3_PORT, ADCModeSel3_PIN, &ADCModeSel3_config);
    Cy_GPIO_Pin_Init(ADCModeSel4_PORT, ADCModeSel4_PIN, &ADCModeSel4_config);
    Cy_GPIO_Pin_Init(ADCModeSel5_PORT, ADCModeSel5_PIN, &ADCModeSel5_config);
    Cy_GPIO_Pin_Init(ADCModeSel6_PORT, ADCModeSel6_PIN, &ADCModeSel6_config);
    Cy_GPIO_Pin_Init(ADCModeSel7_PORT, ADCModeSel7_PIN, &ADCModeSel7_config);
    Cy_GPIO_Pin_Init(CYBSP_SWDIO_PORT, CYBSP_SWDIO_PIN, &CYBSP_SWDIO_config);
    Cy_GPIO_Pin_Init(CYBSP_SWDCK_PORT, CYBSP_SWDCK_PIN, &CYBSP_SWDCK_config);
    Cy_GPIO_Pin_Init(ioss_0_port_3_pin_4_PORT, ioss_0_port_3_pin_4_PIN, &ioss_0_port_3_pin_4_config);
    Cy_GPIO_Pin_Init(SPIM_CS3_PORT, SPIM_CS3_PIN, &SPIM_CS3_config);
    Cy_GPIO_Pin_Init(ioss_0_port_4_pin_6_PORT, ioss_0_port_4_pin_6_PIN, &ioss_0_port_4_pin_6_config);
    Cy_GPIO_Pin_Init(I2CMA_SCL_PORT, I2CMA_SCL_PIN, &I2CMA_SCL_config);
    Cy_GPIO_Pin_Init(I2CMA_SDA_PORT, I2CMA_SDA_PIN, &I2CMA_SDA_config);
    Cy_GPIO_Pin_Init(PWMO0_PORT, PWMO0_PIN, &PWMO0_config);
    Cy_GPIO_Pin_Init(PWMO1_PORT, PWMO1_PIN, &PWMO1_config);
    Cy_GPIO_Pin_Init(ioss_0_port_6_pin_1_PORT, ioss_0_port_6_pin_1_PIN, &ioss_0_port_6_pin_1_config);
    Cy_GPIO_Pin_Init(ioss_0_port_6_pin_2_PORT, ioss_0_port_6_pin_2_PIN, &ioss_0_port_6_pin_2_config);
    Cy_GPIO_Pin_Init(CYBSP_LED2_PORT, CYBSP_LED2_PIN, &CYBSP_LED2_config);
    Cy_GPIO_Pin_Init(I2CS_SCL_PORT, I2CS_SCL_PIN, &I2CS_SCL_config);
    Cy_GPIO_Pin_Init(I2CS_SDA_PORT, I2CS_SDA_PIN, &I2CS_SDA_config);
}
void reserve_cycfg_pins(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&PWMCAP0_obj);
    cyhal_hwmgr_reserve(&PWMCAP1_obj);
    cyhal_hwmgr_reserve(&CAN_RX_obj);
    cyhal_hwmgr_reserve(&CAN_TX_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_10_pin_4_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_10_pin_5_obj);
    cyhal_hwmgr_reserve(&SPIM_MOSI_obj);
    cyhal_hwmgr_reserve(&SPIM_MISO_obj);
    cyhal_hwmgr_reserve(&SPIM_SCL_obj);
    cyhal_hwmgr_reserve(&SPIM_CS0_obj);
    cyhal_hwmgr_reserve(&SPIM_CS1_obj);
    cyhal_hwmgr_reserve(&CYBSP_BTN_obj);
    cyhal_hwmgr_reserve(&UART_RX_obj);
    cyhal_hwmgr_reserve(&UART_TX_obj);
    cyhal_hwmgr_reserve(&PWMCAP2_obj);
    cyhal_hwmgr_reserve(&PWMCAP3_obj);
    cyhal_hwmgr_reserve(&I2CMB_SCL_obj);
    cyhal_hwmgr_reserve(&I2CMB_SDA_obj);
    cyhal_hwmgr_reserve(&ADCOTP_obj);
    cyhal_hwmgr_reserve(&ADCSilverVoltage_obj);
    cyhal_hwmgr_reserve(&ADC983MODE0_obj);
    cyhal_hwmgr_reserve(&ADCModeSel3_obj);
    cyhal_hwmgr_reserve(&ADCModeSel4_obj);
    cyhal_hwmgr_reserve(&ADCModeSel5_obj);
    cyhal_hwmgr_reserve(&ADCModeSel6_obj);
    cyhal_hwmgr_reserve(&ADCModeSel7_obj);
    cyhal_hwmgr_reserve(&CYBSP_SWDIO_obj);
    cyhal_hwmgr_reserve(&CYBSP_SWDCK_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_3_pin_4_obj);
    cyhal_hwmgr_reserve(&SPIM_CS3_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_4_pin_6_obj);
    cyhal_hwmgr_reserve(&I2CMA_SCL_obj);
    cyhal_hwmgr_reserve(&I2CMA_SDA_obj);
    cyhal_hwmgr_reserve(&PWMO0_obj);
    cyhal_hwmgr_reserve(&PWMO1_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_6_pin_1_obj);
    cyhal_hwmgr_reserve(&ioss_0_port_6_pin_2_obj);
    cyhal_hwmgr_reserve(&CYBSP_LED2_obj);
    cyhal_hwmgr_reserve(&I2CS_SCL_obj);
    cyhal_hwmgr_reserve(&I2CS_SDA_obj);
#endif /* defined (CY_USING_HAL) */
}
