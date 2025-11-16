/*******************************************************************************
 * File Name: cycfg_peripherals.c
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

#include "cycfg_peripherals.h"

#define STD_ID_FILTER_0 \
{\
    .sfid2 = 2047U, \
    .sfid1 = 0x1U, \
    .sfec = CY_CANFD_SFEC_STORE_RX_FIFO_0, \
    .sft = CY_CANFD_SFT_CLASSIC_FILTER, \
 }
#define STD_ID_FILTER_1 \
{\
    .sfid2 = 2047U, \
    .sfid1 = 0x10U, \
    .sfec = CY_CANFD_SFEC_STORE_RX_FIFO_1, \
    .sft = CY_CANFD_SFT_CLASSIC_FILTER, \
 }
#define EXT_ID_FILTER_0 \
{\
    .f0_f = &mCANFD_extIdFilterF0Config_0, \
    .f1_f = &mCANFD_extIdFilterF1Config_0, \
 }
#define EXT_ID_FILTER_1 \
{\
    .f0_f = &mCANFD_extIdFilterF0Config_1, \
    .f1_f = &mCANFD_extIdFilterF1Config_1, \
 }
#define mPWMCAP0_INPUT_DISABLED 0x7U
#define mPWMCAP1_INPUT_DISABLED 0x7U
#define mPWMCAP2_INPUT_DISABLED 0x7U
#define mPWMCAP3_INPUT_DISABLED 0x7U
#define mPWMCAP_TIMER100KHz_INPUT_DISABLED 0x7U
#define mQuadDec_INPUT_DISABLED 0x7U
#define mPWMO0_INPUT_DISABLED 0x7U
#define mPWMO1_INPUT_DISABLED 0x7U

void CanFd_RxCallback(bool rxFIFOMsg, uint8_t msgBufOrRxFIFONum, cy_stc_canfd_rx_buffer_t* basemsg);
const cy_stc_canfd_bitrate_t mCANFD_nominalBitrateConfig =
{
    .prescaler = 12U - 1U,
    .timeSegment1 = 5U - 1U,
    .timeSegment2 = 2U - 1U,
    .syncJumpWidth = 2U - 1U,
};
const cy_stc_canfd_bitrate_t mCANFD_dataBitrateConfig =
{
    .prescaler = 6U - 1U,
    .timeSegment1 = 5U - 1U,
    .timeSegment2 = 2U - 1U,
    .syncJumpWidth = 2U - 1U,
};
const cy_stc_canfd_transceiver_delay_compensation_t mCANFD_tdcConfig =
{
    .tdcEnabled = false,
    .tdcOffset = 0U,
    .tdcFilterWindow = 0U,
};
const cy_stc_id_filter_t mCANFD_stdIdFilter_0 =
{
    .sfid2 = 2047U,
    .sfid1 = 0x1U,
    .sfec = CY_CANFD_SFEC_STORE_RX_FIFO_0,
    .sft = CY_CANFD_SFT_CLASSIC_FILTER,
};
const cy_stc_id_filter_t mCANFD_stdIdFilter_1 =
{
    .sfid2 = 2047U,
    .sfid1 = 0x10U,
    .sfec = CY_CANFD_SFEC_STORE_RX_FIFO_1,
    .sft = CY_CANFD_SFT_CLASSIC_FILTER,
};
const cy_stc_id_filter_t mCANFD_stdIdFilters[] =
{
    [0] = STD_ID_FILTER_0,
    [1] = STD_ID_FILTER_1,
};
const cy_stc_canfd_sid_filter_config_t mCANFD_sidFiltersConfig =
{
    .numberOfSIDFilters = 2U,
    .sidFilter = mCANFD_stdIdFilters,
};
const cy_stc_canfd_f0_t mCANFD_extIdFilterF0Config_0 =
{
    .efid1 = 0x10010U,
    .efec = CY_CANFD_EFEC_STORE_RX_BUFFER,
};
const cy_stc_canfd_f0_t mCANFD_extIdFilterF0Config_1 =
{
    .efid1 = 0x10020U,
    .efec = CY_CANFD_EFEC_STORE_RX_BUFFER,
};
const cy_stc_canfd_f1_t mCANFD_extIdFilterF1Config_0 =
{
    .efid2 = 2U,
    .eft = CY_CANFD_EFT_RANGE_EFID1_EFID2_NO_MSK,
};
const cy_stc_canfd_f1_t mCANFD_extIdFilterF1Config_1 =
{
    .efid2 = 3U,
    .eft = CY_CANFD_EFT_RANGE_EFID1_EFID2,
};
const cy_stc_extid_filter_t mCANFD_extIdFilter_0 =
{
    .f0_f = &mCANFD_extIdFilterF0Config_0,
    .f1_f = &mCANFD_extIdFilterF1Config_0,
};
const cy_stc_extid_filter_t mCANFD_extIdFilter_1 =
{
    .f0_f = &mCANFD_extIdFilterF0Config_1,
    .f1_f = &mCANFD_extIdFilterF1Config_1,
};
const cy_stc_extid_filter_t mCANFD_extIdFilters[] =
{
    [0] = EXT_ID_FILTER_0,
    [1] = EXT_ID_FILTER_1,
};
const cy_stc_canfd_extid_filter_config_t mCANFD_extIdFiltersConfig =
{
    .numberOfEXTIDFilters = 2U,
    .extidFilter = (cy_stc_extid_filter_t*)&mCANFD_extIdFilters,
    .extIDANDMask = 536870911UL,
};
const cy_stc_canfd_global_filter_config_t mCANFD_globalFilterConfig =
{
    .nonMatchingFramesStandard = CY_CANFD_ACCEPT_IN_RXFIFO_0,
    .nonMatchingFramesExtended = CY_CANFD_ACCEPT_IN_RXFIFO_1,
    .rejectRemoteFramesStandard = true,
    .rejectRemoteFramesExtended = true,
};
const cy_en_canfd_fifo_config_t mCANFD_rxFifo0Config =
{
    .mode = CY_CANFD_FIFO_MODE_BLOCKING,
    .watermark = 1U,
    .numberOfFIFOElements = 2U,
    .topPointerLogicEnabled = false,
};
const cy_en_canfd_fifo_config_t mCANFD_rxFifo1Config =
{
    .mode = CY_CANFD_FIFO_MODE_BLOCKING,
    .watermark = 4U,
    .numberOfFIFOElements = 5U,
    .topPointerLogicEnabled = false,
};
const cy_stc_canfd_config_t mCANFD_config =
{
    .txCallback = NULL,
    .rxCallback = CanFd_RxCallback,
    .errorCallback = NULL,
    .canFDMode = true,
    .bitrate = &mCANFD_nominalBitrateConfig,
    .fastBitrate = &mCANFD_dataBitrateConfig,
    .tdcConfig = &mCANFD_tdcConfig,
    .sidFilterConfig = &mCANFD_sidFiltersConfig,
    .extidFilterConfig = &mCANFD_extIdFiltersConfig,
    .globalFilterConfig = &mCANFD_globalFilterConfig,
    .rxBufferDataSize = CY_CANFD_BUFFER_DATA_SIZE_64,
    .rxFIFO1DataSize = CY_CANFD_BUFFER_DATA_SIZE_64,
    .rxFIFO0DataSize = CY_CANFD_BUFFER_DATA_SIZE_64,
    .txBufferDataSize = CY_CANFD_BUFFER_DATA_SIZE_64,
    .rxFIFO0Config = &mCANFD_rxFifo0Config,
    .rxFIFO1Config = &mCANFD_rxFifo1Config,
    .noOfRxBuffers = 4U,
    .noOfTxBuffers = 1U,
    .messageRAMaddress = CY_CAN0MRAM_BASE + 0U,
    .messageRAMsize = 4096U,
};
cy_stc_canfd_t0_t mCANFD_T0RegisterBuffer_0 =
{
    .id = 0x110U,
    .rtr = CY_CANFD_RTR_DATA_FRAME,
    .xtd = CY_CANFD_XTD_STANDARD_ID,
    .esi = CY_CANFD_ESI_ERROR_PASSIVE,
};
cy_stc_canfd_t1_t mCANFD_T1RegisterBuffer_0 =
{
    .dlc = 0U,
    .brs = false,
    .fdf = CY_CANFD_FDF_STANDARD_FRAME,
    .efc = false,
    .mm = 0U,
};
uint32_t mCANFD_dataBuffer_0[] =
{
    [mCANFD_DATA_0] = 0U,
    [mCANFD_DATA_1] = 0U,
    [mCANFD_DATA_2] = 0U,
    [mCANFD_DATA_3] = 0U,
    [mCANFD_DATA_4] = 0U,
    [mCANFD_DATA_5] = 0U,
    [mCANFD_DATA_6] = 0U,
    [mCANFD_DATA_7] = 0U,
    [mCANFD_DATA_8] = 0U,
    [mCANFD_DATA_9] = 0U,
    [mCANFD_DATA_10] = 0U,
    [mCANFD_DATA_11] = 0U,
    [mCANFD_DATA_12] = 0U,
    [mCANFD_DATA_13] = 0U,
    [mCANFD_DATA_14] = 0U,
    [mCANFD_DATA_15] = 0U,
};
cy_stc_canfd_tx_buffer_t mCANFD_txBuffer_0 =
{
    .t0_f = &mCANFD_T0RegisterBuffer_0,
    .t1_f = &mCANFD_T1RegisterBuffer_0,
    .data_area_f = mCANFD_dataBuffer_0,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mCANFD_obj =
{
    .type = CYHAL_RSC_CAN,
    .block_num = 0U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_sar_channel_config_t mADC_channel_0_config =
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS0_PIN | (SAR0_VPLUS0_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_0,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t mADC_channel_1_config =
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS1_PIN | (SAR0_VPLUS1_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_1,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t mADC_channel_2_config =
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS2_PIN | (SAR0_VPLUS2_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_1,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t mADC_channel_3_config =
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS3_PIN | (SAR0_VPLUS3_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_1,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t mADC_channel_4_config =
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS4_PIN | (SAR0_VPLUS4_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_1,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t mADC_channel_5_config =
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS5_PIN | (SAR0_VPLUS5_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_1,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t mADC_channel_6_config =
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS6_PIN | (SAR0_VPLUS6_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_1,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t mADC_channel_7_config =
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS7_PIN | (SAR0_VPLUS7_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_1,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_channel_config_t mADC_channel_8_config =
{
    .addr = (cy_en_sar_chan_config_port_pin_addr_t)(SAR0_VPLUS8_PIN | (SAR0_VPLUS8_PORT << SAR_CHAN_CONFIG_PORT_ADDR_Pos)),
    .differential = false,
    .resolution = CY_SAR_MAX_RES,
    .avgEn = true,
    .sampleTimeSel = CY_SAR_SAMPLE_TIME_2,
    .rangeIntrEn = false,
    .satIntrEn = false,
};
const cy_stc_sar_config_t mADC_config =
{
    .vrefSel = CY_SAR_VREF_SEL_VDDA_DIV_2,
    .vrefBypCapEn = false,
    .negSel = CY_SAR_NEG_SEL_VREF,
    .negVref = CY_SAR_NEGVREF_HW,
    .boostPump = true,
    .power = CY_SAR_QUARTER_PWR,
    .sarMuxDsEn = false,
    .switchDisable = false,
    .subResolution = CY_SAR_SUB_RESOLUTION_10B,
    .leftAlign = false,
    .singleEndedSigned = false,
    .differentialSigned = true,
    .avgCnt = CY_SAR_AVG_CNT_32,
    .avgShift = true,
    .trigMode = CY_SAR_TRIGGER_MODE_FW_ONLY,
    .eosEn = true,
    .sampleTime0 = 2,
    .sampleTime1 = 2,
    .sampleTime2 = 3,
    .sampleTime3 = 2,
    .rangeThresLow = 0UL,
    .rangeThresHigh = 0UL,
    .rangeCond = CY_SAR_RANGE_COND_BELOW,
    .chanEn = 511UL,
    .channelConfig = {&mADC_channel_0_config, &mADC_channel_1_config, &mADC_channel_2_config, &mADC_channel_3_config, &mADC_channel_4_config, &mADC_channel_5_config, &mADC_channel_6_config, &mADC_channel_7_config, &mADC_channel_8_config, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    .routingConfig = NULL,
    .vrefMvValue = mADC_VREF_MV,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mADC_obj =
{
    .type = CYHAL_RSC_ADC,
    .block_num = 0,
    .channel_num = 0,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_scb_uart_config_t mUART_config =
{
    .uartMode = CY_SCB_UART_STANDARD,
    .enableMutliProcessorMode = false,
    .smartCardRetryOnNack = false,
    .irdaInvertRx = false,
    .irdaEnableLowPowerReceiver = false,
    .enableLinMode = false,
    .oversample = 8,
    .enableMsbFirst = false,
    .dataWidth = 8UL,
    .parity = CY_SCB_UART_PARITY_NONE,
    .stopBits = CY_SCB_UART_STOP_BITS_1,
    .enableInputFilter = true,
    .breakWidth = 11UL,
    .breakLevel = false,
    .dropOnFrameError = false,
    .dropOnParityError = false,
    .receiverAddress = 0x0UL,
    .receiverAddressMask = 0x0UL,
    .acceptAddrInFifo = false,
    .enableCts = false,
    .ctsPolarity = CY_SCB_UART_ACTIVE_LOW,
    .rtsRxFifoLevel = 0UL,
    .rtsPolarity = CY_SCB_UART_ACTIVE_LOW,
    .rxFifoTriggerLevel = 0UL,
    .rxFifoIntEnableMask = 4UL,
    .txFifoTriggerLevel = 0UL,
    .txFifoIntEnableMask = 0UL,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mUART_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 0U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_scb_spi_config_t mSPIM_config =
{
    .spiMode = CY_SCB_SPI_MASTER,
    .subMode = CY_SCB_SPI_MOTOROLA,
    .sclkMode = CY_SCB_SPI_CPHA1_CPOL1,
    .parity = CY_SCB_SPI_PARITY_NONE,
    .dropOnParityError = false,
    .oversample = 8,
    .rxDataWidth = 8UL,
    .txDataWidth = 8UL,
    .enableMsbFirst = true,
    .enableInputFilter = true,
    .enableFreeRunSclk = false,
    .enableMisoLateSample = true,
    .enableTransferSeperation = false,
    .ssPolarity = ((CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT0) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT1) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT2) | \
                                         (CY_SCB_SPI_ACTIVE_LOW << CY_SCB_SPI_SLAVE_SELECT3)),
    .ssSetupDelay = CY_SCB_SPI_SS_SETUP_DELAY_0_75_CYCLES,
    .ssHoldDelay = CY_SCB_SPI_SS_HOLD_DELAY_0_75_CYCLES,
    .ssInterDataframeDelay = CY_SCB_SPI_SS_INTERFRAME_DELAY_1_5_CYCLES,
    .enableWakeFromSleep = false,
    .rxFifoTriggerLevel = 7UL,
    .rxFifoIntEnableMask = 0UL,
    .txFifoTriggerLevel = 0UL,
    .txFifoIntEnableMask = 0UL,
    .masterSlaveIntEnableMask = 0UL,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mSPIM_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 1U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_scb_i2c_config_t mI2CMA_config =
{
    .i2cMode = CY_SCB_I2C_MASTER,
    .useRxFifo = true,
    .useTxFifo = true,
    .slaveAddress = 0U,
    .slaveAddressMask = 0U,
    .acceptAddrInFifo = false,
    .ackGeneralAddr = false,
    .hsEnable = false,
    .enableWakeFromSleep = false,
    .enableDigitalFilter = true,
    .lowPhaseDutyCycle = 9,
    .highPhaseDutyCycle = 7,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mI2CMA_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 2U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_scb_i2c_config_t mI2CMB_config =
{
    .i2cMode = CY_SCB_I2C_MASTER,
    .useRxFifo = true,
    .useTxFifo = true,
    .slaveAddress = 0U,
    .slaveAddressMask = 0U,
    .acceptAddrInFifo = false,
    .ackGeneralAddr = false,
    .hsEnable = false,
    .enableWakeFromSleep = false,
    .enableDigitalFilter = true,
    .lowPhaseDutyCycle = 11,
    .highPhaseDutyCycle = 9,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mI2CMB_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 3U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_scb_i2c_config_t mI2CS_config =
{
    .i2cMode = CY_SCB_I2C_SLAVE,
    .useRxFifo = true,
    .useTxFifo = true,
    .slaveAddress = 10,
    .slaveAddressMask = 254,
    .acceptAddrInFifo = false,
    .ackGeneralAddr = false,
    .hsEnable = false,
    .enableWakeFromSleep = false,
    .enableDigitalFilter = false,
    .lowPhaseDutyCycle = 0,
    .highPhaseDutyCycle = 0,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mI2CS_obj =
{
    .type = CYHAL_RSC_SCB,
    .block_num = 4U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_tcpwm_counter_config_t mPWMCAP0_config =
{
    .period = 50000,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 16384,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_ON_CC_OR_TC,
    .captureInputMode = CY_TCPWM_INPUT_EITHEREDGE,
    .captureInput = TCPWM_CNT0_CAPTURE_VALUE,
    .reloadInputMode = CY_TCPWM_INPUT_RISINGEDGE,
    .reloadInput = TCPWM_CNT0_RELOAD_VALUE,
    .startInputMode = mPWMCAP0_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = mPWMCAP0_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = CY_TCPWM_INPUT_RISINGEDGE,
    .countInput = TCPWM_CNT0_COUNT_VALUE,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mPWMCAP0_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 0U,
    .channel_num = 0U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_tcpwm_counter_config_t mPWMCAP1_config =
{
    .period = 50000,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 16384,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_ON_CC_OR_TC,
    .captureInputMode = CY_TCPWM_INPUT_EITHEREDGE,
    .captureInput = TCPWM_CNT1_CAPTURE_VALUE,
    .reloadInputMode = CY_TCPWM_INPUT_RISINGEDGE,
    .reloadInput = TCPWM_CNT1_RELOAD_VALUE,
    .startInputMode = mPWMCAP1_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = mPWMCAP1_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = CY_TCPWM_INPUT_RISINGEDGE,
    .countInput = TCPWM_CNT1_COUNT_VALUE,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mPWMCAP1_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 0U,
    .channel_num = 1U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_tcpwm_counter_config_t mPWMCAP2_config =
{
    .period = 50000,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 16384,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_ON_CC_OR_TC,
    .captureInputMode = CY_TCPWM_INPUT_EITHEREDGE,
    .captureInput = TCPWM_CNT2_CAPTURE_VALUE,
    .reloadInputMode = CY_TCPWM_INPUT_RISINGEDGE,
    .reloadInput = TCPWM_CNT2_RELOAD_VALUE,
    .startInputMode = mPWMCAP2_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = mPWMCAP2_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = CY_TCPWM_INPUT_RISINGEDGE,
    .countInput = TCPWM_CNT2_COUNT_VALUE,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mPWMCAP2_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 0U,
    .channel_num = 2U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_tcpwm_counter_config_t mPWMCAP3_config =
{
    .period = 50000,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 16384,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_ON_CC_OR_TC,
    .captureInputMode = CY_TCPWM_INPUT_EITHEREDGE,
    .captureInput = TCPWM_CNT3_CAPTURE_VALUE,
    .reloadInputMode = CY_TCPWM_INPUT_RISINGEDGE,
    .reloadInput = TCPWM_CNT3_RELOAD_VALUE,
    .startInputMode = mPWMCAP3_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = mPWMCAP3_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = CY_TCPWM_INPUT_RISINGEDGE,
    .countInput = TCPWM_CNT3_COUNT_VALUE,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mPWMCAP3_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 0U,
    .channel_num = 3U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_tcpwm_counter_config_t mPWMCAP_TIMER100KHz_config =
{
    .period = 39,
    .clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
    .runMode = CY_TCPWM_COUNTER_CONTINUOUS,
    .countDirection = CY_TCPWM_COUNTER_COUNT_UP,
    .compareOrCapture = CY_TCPWM_COUNTER_MODE_CAPTURE,
    .compare0 = 16384,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_NONE,
    .captureInputMode = mPWMCAP_TIMER100KHz_INPUT_DISABLED & 0x3U,
    .captureInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = mPWMCAP_TIMER100KHz_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = mPWMCAP_TIMER100KHz_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .stopInputMode = mPWMCAP_TIMER100KHz_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .countInputMode = mPWMCAP_TIMER100KHz_INPUT_DISABLED & 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mPWMCAP_TIMER100KHz_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 0U,
    .channel_num = 4U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_tcpwm_quaddec_config_t mQuadDec_config =
{
    .resolution = CY_TCPWM_QUADDEC_X1,
    .interruptSources = CY_TCPWM_INT_NONE,
    .indexInputMode = mQuadDec_INPUT_DISABLED & 0x3U,
    .indexInput = CY_TCPWM_INPUT_0,
    .stopInputMode = mQuadDec_INPUT_DISABLED & 0x3U,
    .stopInput = CY_TCPWM_INPUT_0,
    .phiAInput = TCPWM_CNT5_COUNT_VALUE,
    .phiBInput = TCPWM_CNT5_START_VALUE,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mQuadDec_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 0U,
    .channel_num = 5U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_tcpwm_pwm_config_t mPWMO0_config =
{
    .pwmMode = CY_TCPWM_PWM_MODE_PWM,
    .clockPrescaler = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
    .pwmAlignment = CY_TCPWM_PWM_LEFT_ALIGN,
    .deadTimeClocks = 0,
    .runMode = CY_TCPWM_PWM_CONTINUOUS,
    .period0 = 32767,
    .period1 = 32768,
    .enablePeriodSwap = false,
    .compare0 = 16383,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_ON_CC_OR_TC,
    .invertPWMOut = CY_TCPWM_PWM_INVERT_DISABLE,
    .invertPWMOutN = CY_TCPWM_PWM_INVERT_DISABLE,
    .killMode = CY_TCPWM_PWM_STOP_ON_KILL,
    .swapInputMode = mPWMO0_INPUT_DISABLED & 0x3U,
    .swapInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = mPWMO0_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = mPWMO0_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .killInputMode = mPWMO0_INPUT_DISABLED & 0x3U,
    .killInput = CY_TCPWM_INPUT_0,
    .countInputMode = mPWMO0_INPUT_DISABLED & 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mPWMO0_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 0U,
    .channel_num = 6U,
};
#endif /* defined (CY_USING_HAL) */

const cy_stc_tcpwm_pwm_config_t mPWMO1_config =
{
    .pwmMode = CY_TCPWM_PWM_MODE_PWM,
    .clockPrescaler = CY_TCPWM_PWM_PRESCALER_DIVBY_1,
    .pwmAlignment = CY_TCPWM_PWM_LEFT_ALIGN,
    .deadTimeClocks = 0,
    .runMode = CY_TCPWM_PWM_CONTINUOUS,
    .period0 = 32767,
    .period1 = 32768,
    .enablePeriodSwap = false,
    .compare0 = 16383,
    .compare1 = 16384,
    .enableCompareSwap = false,
    .interruptSources = CY_TCPWM_INT_ON_CC_OR_TC,
    .invertPWMOut = CY_TCPWM_PWM_INVERT_DISABLE,
    .invertPWMOutN = CY_TCPWM_PWM_INVERT_DISABLE,
    .killMode = CY_TCPWM_PWM_STOP_ON_KILL,
    .swapInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .swapInput = CY_TCPWM_INPUT_0,
    .reloadInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .reloadInput = CY_TCPWM_INPUT_0,
    .startInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .startInput = CY_TCPWM_INPUT_0,
    .killInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .killInput = CY_TCPWM_INPUT_0,
    .countInputMode = mPWMO1_INPUT_DISABLED & 0x3U,
    .countInput = CY_TCPWM_INPUT_1,
};

#if defined (CY_USING_HAL)
const cyhal_resource_inst_t mPWMO1_obj =
{
    .type = CYHAL_RSC_TCPWM,
    .block_num = 0U,
    .channel_num = 7U,
};
#endif /* defined (CY_USING_HAL) */

void init_cycfg_peripherals(void)
{
    Cy_SysClk_PeriphAssignDivider(PCLK_CANFD_CLOCK_CAN0, CY_SYSCLK_DIV_16_BIT, 0U);
    Cy_CTB_SetPumpClkSource(CTBM0, CY_CTB_CLK_PUMP_HF_DIV_2);
    Cy_SysClk_PeriphAssignDivider(PCLK_PASS0_CLOCK_SAR, CY_SYSCLK_DIV_16_BIT, 7U);
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB0_CLOCK, CY_SYSCLK_DIV_16_BIT, 5U);
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB1_CLOCK, CY_SYSCLK_DIV_16_BIT, 2U);
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB2_CLOCK, CY_SYSCLK_DIV_16_BIT, 2U);
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB3_CLOCK, CY_SYSCLK_DIV_16_BIT, 3U);
    Cy_SysClk_PeriphAssignDivider(PCLK_SCB4_CLOCK, CY_SYSCLK_DIV_16_BIT, 1U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS0, CY_SYSCLK_DIV_16_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS1, CY_SYSCLK_DIV_16_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS2, CY_SYSCLK_DIV_16_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS3, CY_SYSCLK_DIV_16_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS4, CY_SYSCLK_DIV_16_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS5, CY_SYSCLK_DIV_16_BIT, 8U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS6, CY_SYSCLK_DIV_16_BIT, 4U);
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM_CLOCKS7, CY_SYSCLK_DIV_16_BIT, 4U);
}
void reserve_cycfg_peripherals(void)
{
#if defined (CY_USING_HAL)
    cyhal_hwmgr_reserve(&mCANFD_obj);
    cyhal_hwmgr_reserve(&mADC_obj);
    cyhal_hwmgr_reserve(&mUART_obj);
    cyhal_hwmgr_reserve(&mSPIM_obj);
    cyhal_hwmgr_reserve(&mI2CMA_obj);
    cyhal_hwmgr_reserve(&mI2CMB_obj);
    cyhal_hwmgr_reserve(&mI2CS_obj);
    cyhal_hwmgr_reserve(&mPWMCAP0_obj);
    cyhal_hwmgr_reserve(&mPWMCAP1_obj);
    cyhal_hwmgr_reserve(&mPWMCAP2_obj);
    cyhal_hwmgr_reserve(&mPWMCAP3_obj);
    cyhal_hwmgr_reserve(&mPWMCAP_TIMER100KHz_obj);
    cyhal_hwmgr_reserve(&mQuadDec_obj);
    cyhal_hwmgr_reserve(&mPWMO0_obj);
    cyhal_hwmgr_reserve(&mPWMO1_obj);
#endif /* defined (CY_USING_HAL) */
}
