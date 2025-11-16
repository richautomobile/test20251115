/*******************************************************************************
 * File Name: cycfg_dmas.c
 *
 * Description:
 * DMA configuration
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

#include "cycfg_dmas.h"

const cy_stc_dmac_channel_config_t mDMAADC_channel_config =
{
    .priority = 3UL,
    .enable = false,
    .descriptor = CY_DMAC_DESCRIPTOR_PING,
};
const cy_stc_dmac_descriptor_config_t mDMAADC_ping_config =
{
    .srcAddress = 0UL,
    .dstAddress = 0UL,
    .dataCount = 9UL,
    .dataSize = CY_DMAC_HALFWORD,
    .srcTransferSize = CY_DMAC_TRANSFER_SIZE_DATA,
    .srcAddrIncrement = true,
    .dstTransferSize = CY_DMAC_TRANSFER_SIZE_DATA,
    .dstAddrIncrement = true,
    .retrigger = CY_DMAC_RETRIG_IM,
    .cpltState = false,
    .interrupt = false,
    .preemptable = false,
    .flipping = false,
    .triggerType = CY_DMAC_SINGLE_ELEMENT,
};
const cy_stc_dmac_descriptor_config_t mDMAADC_pong_config =
{
    .srcAddress = 0UL,
    .dstAddress = 0UL,
    .dataCount = 1UL,
    .dataSize = CY_DMAC_WORD,
    .srcTransferSize = CY_DMAC_TRANSFER_SIZE_WORD,
    .srcAddrIncrement = true,
    .dstTransferSize = CY_DMAC_TRANSFER_SIZE_WORD,
    .dstAddrIncrement = true,
    .retrigger = CY_DMAC_RETRIG_IM,
    .cpltState = false,
    .interrupt = true,
    .preemptable = true,
    .flipping = true,
    .triggerType = CY_DMAC_SINGLE_ELEMENT,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mDMAADC_obj =
{
    .type = CYHAL_RSC_DMA,
    .block_num = 0UL,
    .channel_num = 0UL,
};
#endif /* defined (CY_USING_HAL) */

void reserve_cycfg_dmas(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&mDMAADC_obj);
#endif /* defined (CY_USING_HAL) */
}
