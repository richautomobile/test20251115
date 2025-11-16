/*******************************************************************************
 * File Name: cycfg_clocks.h
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

#if !defined(CYCFG_CLOCKS_H)
#define CYCFG_CLOCKS_H

#include "cycfg_notices.h"
#include "cy_sysclk.h"

#if defined (CY_USING_HAL)
#include "cyhal_hwmgr.h"
#endif /* defined (CY_USING_HAL) */

#if defined(__cplusplus)
extern "C" {
#endif /* defined(__cplusplus) */

#define CLOCK_48M_ENABLED 1U
#define CLOCK_48M_HW CY_SYSCLK_DIV_16_BIT
#define CLOCK_48M_NUM 0U
#define CLOCK_24M_ENABLED 1U
#define CLOCK_24M_HW CY_SYSCLK_DIV_16_BIT
#define CLOCK_24M_NUM 1U
#define CLOCK_16M_ENABLED 1U
#define CLOCK_16M_HW CY_SYSCLK_DIV_16_BIT
#define CLOCK_16M_NUM 2U
#define CLOCK_8M_ENABLED 1U
#define CLOCK_8M_HW CY_SYSCLK_DIV_16_BIT
#define CLOCK_8M_NUM 3U
#define CLOCK_4M_ENABLED 1U
#define CLOCK_4M_HW CY_SYSCLK_DIV_16_BIT
#define CLOCK_4M_NUM 4U
#define CLOCK_UART_ENABLED 1U
#define CLOCK_UART_HW CY_SYSCLK_DIV_16_BIT
#define CLOCK_UART_NUM 5U
#define CLOCK_20K_ENABLED 1U
#define CLOCK_20K_HW CY_SYSCLK_DIV_16_BIT
#define CLOCK_20K_NUM 6U
#define CLOCK_ADC_1_6M_ENABLED 1U
#define CLOCK_ADC_1_6M_HW CY_SYSCLK_DIV_16_BIT
#define CLOCK_ADC_1_6M_NUM 7U
#define CLOCK_QuadDec_1M_ENABLED 1U
#define CLOCK_QuadDec_1M_HW CY_SYSCLK_DIV_16_BIT
#define CLOCK_QuadDec_1M_NUM 8U

#if defined (CY_USING_HAL)
extern const cyhal_resource_inst_t CLOCK_48M_obj;
extern const cyhal_resource_inst_t CLOCK_24M_obj;
extern const cyhal_resource_inst_t CLOCK_16M_obj;
extern const cyhal_resource_inst_t CLOCK_8M_obj;
extern const cyhal_resource_inst_t CLOCK_4M_obj;
extern const cyhal_resource_inst_t CLOCK_UART_obj;
extern const cyhal_resource_inst_t CLOCK_20K_obj;
extern const cyhal_resource_inst_t CLOCK_ADC_1_6M_obj;
extern const cyhal_resource_inst_t CLOCK_QuadDec_1M_obj;
#endif /* defined (CY_USING_HAL) */

void init_cycfg_clocks(void);
void reserve_cycfg_clocks(void);

#if defined(__cplusplus)
}
#endif /* defined(__cplusplus) */

#endif /* CYCFG_CLOCKS_H */
