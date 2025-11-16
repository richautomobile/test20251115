/*******************************************************************************
 * File Name: cycfg_clocks.c
 *
 * Description:
 * Clock configuration
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

#include "cycfg_clocks.h"

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t CLOCK_48M_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CLOCK_48M_HW,
    .channel_num = CLOCK_48M_NUM,
};
const cyhal_resource_inst_t CLOCK_24M_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CLOCK_24M_HW,
    .channel_num = CLOCK_24M_NUM,
};
const cyhal_resource_inst_t CLOCK_16M_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CLOCK_16M_HW,
    .channel_num = CLOCK_16M_NUM,
};
const cyhal_resource_inst_t CLOCK_8M_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CLOCK_8M_HW,
    .channel_num = CLOCK_8M_NUM,
};
const cyhal_resource_inst_t CLOCK_4M_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CLOCK_4M_HW,
    .channel_num = CLOCK_4M_NUM,
};
const cyhal_resource_inst_t CLOCK_UART_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CLOCK_UART_HW,
    .channel_num = CLOCK_UART_NUM,
};
const cyhal_resource_inst_t CLOCK_20K_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CLOCK_20K_HW,
    .channel_num = CLOCK_20K_NUM,
};
const cyhal_resource_inst_t CLOCK_ADC_1_6M_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CLOCK_ADC_1_6M_HW,
    .channel_num = CLOCK_ADC_1_6M_NUM,
};
const cyhal_resource_inst_t CLOCK_QuadDec_1M_obj =
{
    .type = CYHAL_RSC_CLOCK,
    .block_num = CLOCK_QuadDec_1M_HW,
    .channel_num = CLOCK_QuadDec_1M_NUM,
};
#endif /* defined (CY_USING_HAL) */

void init_cycfg_clocks(void)
{
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 0U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 0U, 0U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 0U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 1U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 1U, 1U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 1U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 2U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 2U, 2U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 2U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 3U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 3U, 5U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 3U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 4U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 4U, 11U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 4U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 5U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 5U, 51U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 5U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 6U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 6U, 2399U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 6U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 7U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 7U, 29U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 7U);
    Cy_SysClk_PeriphDisableDivider(CY_SYSCLK_DIV_16_BIT, 8U);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_16_BIT, 8U, 47U);
    Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_16_BIT, 8U);
}
void reserve_cycfg_clocks(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&CLOCK_48M_obj);
    cyhal_hwmgr_reserve(&CLOCK_24M_obj);
    cyhal_hwmgr_reserve(&CLOCK_16M_obj);
    cyhal_hwmgr_reserve(&CLOCK_8M_obj);
    cyhal_hwmgr_reserve(&CLOCK_4M_obj);
    cyhal_hwmgr_reserve(&CLOCK_UART_obj);
    cyhal_hwmgr_reserve(&CLOCK_20K_obj);
    cyhal_hwmgr_reserve(&CLOCK_ADC_1_6M_obj);
    cyhal_hwmgr_reserve(&CLOCK_QuadDec_1M_obj);
#endif /* defined (CY_USING_HAL) */
}
