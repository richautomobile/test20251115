/**
 * @file Gpio.h
 *
 * @brief
 *
 * Copyright (C) 2022 Innolux Corporation
 * ALL RIGHTS RESERVED. No part of this publication may be copied in any form,
 * by photocopy, microfilm, retrieval system, or by any other means now known or
 * hereafter invented without the prior written permission of Innolux Corporation
 *
 * MISRA-C:2012 Deviation Type definition
 *
 * MISRA-2:
 * Deviated Rule:
 *
 * @author BingHan (binghan.chou@innolux.com)
 * @date 2022-11-10
 */

#ifndef _GPIO_H_
#define _GPIO_H_

/* ==[ Includes ]=========================================================== */
#include "cy_pdl.h"
#include "cybsp.h"
#include "system.h"
#include "TypeDefine.h"

/* ==[ Macros ]============================================================== */
#define PORT0       (0U<<4)
#define PORT1       (1U<<4)
#define PORT2       (2U<<4)
#define PORT3       (3U<<4)
#define PORT4       (4U<<4)
#define PORT5       (5U<<4)
#define PORT6       (6U<<4)
#define PORT7       (7U<<4)
#define PORT8       (8U<<4)
#define PORT9       (9U<<4)
#define PORT10      (10U<<4)
#define PORT11      (11U<<4)
#define PORT12      (12U<<4)
#define Pin0        0U
#define Pin1        1U
#define Pin2        2U
#define Pin3        3U
#define Pin4        4U
#define Pin5        5U
#define Pin6        6U
#define Pin7        7U

#define PIN_SET(PortPin, Level) Gpio_WritePin(PortPin,  Level)
#define PIN_TOGGLE(PortPin)     Gpio_Inv(PortPin)
#define PIN_READ(PortPin)       Gpio_GetPin(PortPin)

#if (BOARD == BOARD_PSOC4)
/*GPIO define by project.*/
/*===============Input pin==============*/
#define CON1_Detetion (PORT1  | Pin6)
#define CON2_Detetion (PORT1  | Pin7)

#define MCU_BUTTON_1  (PORT4  | Pin6)
#define MCU_BUTTON_2  (PORT4  | Pin7)

#define WAKE_CSM      (PORT6  | Pin0)

#define DIP_SWITCH1   (PORT7  | Pin0)
#define DIP_SWITCH2   (PORT7  | Pin1)
#define DIP_SWITCH3   (PORT7  | Pin2)
#define DIP_SWITCH4   (PORT7  | Pin3)
#define DIP_SWITCH5   (PORT7  | Pin4)
#define DIP_SWITCH6   (PORT7  | Pin5)
#define DIP_SWITCH7   (PORT7  | Pin6)
#define DIP_SWITCH8   (PORT7  | Pin7)

#define INTB_TP       (PORT10 | Pin2)
#define IRQ_MCU       (PORT10 | Pin4)
#define Lock_MCU      (PORT10 | Pin5)

/*=========Output pin====================*/
#define CAN_RX        (PORT0  | Pin2)
#define CAN_TX        (PORT0  | Pin3)
#define EncAP1        (PORT0  | Pin4)
#define EncAP2        (PORT0  | Pin5)
#define EncBP1        (PORT0  | Pin6)
#define EncBP2        (PORT0  | Pin7)

#define STBYB         (PORT3  | Pin0)
#define OTPPWR_EN     (PORT3  | Pin1)
#define TestPin3      (PORT3  | Pin4)
#define TestPin4      (PORT3  | Pin5)
#define MUX_SPI_SEL   (PORT3  | Pin4)  /*0:FT4222 spi*/
#define MUX_I2C_SEL   (PORT3  | Pin5)  /*0:FT4222 i2c*/
#define Commu_led     (PORT3  | Pin7)

#define SPButton_S1   (PORT4 | Pin0)
#define SPButton_S2   (PORT4 | Pin1)
#define SPButton_S3   (PORT4 | Pin2)
#define SPButton_S4   (PORT4 | Pin3)
#define SPButton_S5   (PORT4 | Pin4)
#define SPButton_S6   (PORT4 | Pin5)

#define EN_5V0             (PORT5 | Pin2)
#define DBGP_I2C_Bypass    (PORT5 | Pin3)  /*0:dbg i2c Isolation*/
#define DBGP_SPI_Bypass    (PORT5 | Pin5)  /*0:dbg spi Isolation*/
#define WAKE_SP            (PORT5 | Pin7)

#define WAKE_FIDM          (PORT6 | Pin1)
#define EN_3V3             (PORT6 | Pin2)
#define EN_1V8             (PORT6 | Pin3)
#define EN_1V15            (PORT6 | Pin4)
#define SW_Debug_Led      (PORT6 | Pin5)

#define Breathe_LED        (PORT8 | Pin2)
#define DBG_UART           (PORT8 | Pin3)

#define Second_Panel_ON    (PORT9  | Pin0)
#define Panel_Vin_ON_Quie  (PORT9  | Pin1)
#define LED_Crest_ON       (PORT9  | Pin2)
#define Panel_Vin_ON_Norm  (PORT9  | Pin3)

#define EX_FPGA1           (PORT10 | Pin0)
#define FPD_BD_I2C_Switch  (PORT10 | Pin1)
#define PDB_MCU            (PORT10 | Pin3)

#define CAN_STB            (PORT12  | Pin0)
//#define EX_IO1             (PORT12  | Pin0)
#define EX_IO2             (PORT12  | Pin1)

/*===Peripherals pin(Fix define in the Modus Device Config)===*/
#define PWM3                (PORT0 | Pin0)  /*pwm capture*/
#define PWM4                (PORT0 | Pin1)  /*pwm capture*/
#define UART_RX             (PORT1 | Pin0)
#define UART_TX             (PORT1 | Pin1)
#define PWM2                (PORT1 | Pin2)  /*pwm capture*/
#define PWM1                (PORT1 | Pin3)  /*pwm capture*/
#define I2CMB_SCL           (PORT1 | Pin4)
#define I2CMB_SDA           (PORT1 | Pin5)
#define OTPPWR_Detetion     (PORT2 | Pin0)
#define Silver_voltage      (PORT2 | Pin1)
#define TI983_MODE0_Voltage (PORT2 | Pin2)
#define ADCModeSel3         (PORT2 | Pin3)
#define ADCModeSel4         (PORT2 | Pin4)
#define ADCModeSel5         (PORT2 | Pin5)
#define ADCModeSel6         (PORT2 | Pin6)
#define ADCModeSel7         (PORT2 | Pin7)
#define SPIM_CS3            (PORT3 | Pin6)
#define I2CMA_SCL           (PORT5 | Pin0)
#define I2CMA_SDA           (PORT5 | Pin1)
#define PWMO0               (PORT5 | Pin4)  
#define PWMO1               (PORT5 | Pin6)
#define I2CS_SCL            (PORT8 | Pin0)
#define I2CS_SDA            (PORT8 | Pin1)
#define SPIM_MOSI           (PORT11 | Pin0)
#define SPIM_MISO           (PORT11 | Pin1)
#define SPIM_SCL            (PORT11 | Pin2)
#define SPIM_CS0            (PORT11 | Pin3)
#define SPIM_CS1            (PORT11 | Pin4)
#define SPIM_CS2            (PORT11 | Pin5)

#endif

#if (BOARD == BOARD_AGING)
/*GPIO define by project.*/
/*===============Input pin==============*/
#define Lock_MCU    (PORT3 | Pin4)
#define IRQ_MCU     (PORT3 | Pin5)
#define INTB_TP     (PORT3 | Pin7)

#define MCU_BUTTON_1 (PORT4 | Pin6)
#define MCU_BUTTON_2 (PORT4 | Pin7)

#define DIP_SWITCH1 (PORT7 | Pin0)
#define DIP_SWITCH2 (PORT7 | Pin1)
#define DIP_SWITCH3 (PORT7 | Pin2)
#define DIP_SWITCH4 (PORT7 | Pin3)
#define DIP_SWITCH5 (PORT7 | Pin4)
#define DIP_SWITCH6 (PORT7 | Pin5)
#define DIP_SWITCH7 (PORT7 | Pin6)
#define DIP_SWITCH8 (PORT7 | Pin7)

#define WAKE_CSM      (PORT10 | Pin0)

/*=========Output pin====================*/
#define EN_1V15            (PORT0 | Pin4)
#define EN_1V8             (PORT0 | Pin5)
#define EN_3V3             (PORT0 | Pin6)

#define LED1               (PORT4 | Pin0)
#define LED2               (PORT4 | Pin1)
#define LED3               (PORT4 | Pin2)
#define LED4               (PORT4 | Pin3)
#define LED5               (PORT4 | Pin4)
#define LED6               (PORT4 | Pin5)

#define WAKE_SP            (PORT5 | Pin7)

#define WAKE_FIDM          (PORT6 | Pin3)
#define SW_Debug_Led      (PORT6 | Pin5)

#define Breathe_LED        (PORT8 | Pin2)
#define DBG_UART           (PORT8 | Pin3)

#define PDB_MCU            (PORT10 | Pin1)
#define Second_Panel_ON    (PORT10 | Pin2)

/*===Peripherals pin(Fix define in the Modus Device Config)===*/
#define PWM3                (PORT0 | Pin0)  /*pwm capture*/
#define PWM4                (PORT0 | Pin1)  /*pwm capture*/
#define UART_RX             (PORT1 | Pin0)
#define UART_TX             (PORT1 | Pin1)
#define PWM2                (PORT1 | Pin2)  /*pwm capture*/
#define PWM1                (PORT1 | Pin3)  /*pwm capture*/
#define I2CMB_SCL           (PORT1 | Pin4)
#define I2CMB_SDA           (PORT1 | Pin5)
#define OTPPWR_Detetion     (PORT2 | Pin0)
#define Silver_voltage      (PORT2 | Pin1)
#define TI983_MODE0_Voltage (PORT2 | Pin2)
#define ADCModeSel3         (PORT2 | Pin3)
#define ADCModeSel4         (PORT2 | Pin4)
#define ADCModeSel5         (PORT2 | Pin5)
#define ADCModeSel6         (PORT2 | Pin6)
#define ADCModeSel7         (PORT2 | Pin7)
#define SPIM_CS3            (PORT3 | Pin6)
#define I2CMA_SCL           (PORT5 | Pin0)
#define I2CMA_SDA           (PORT5 | Pin1)
#define PWMO0               (PORT5 | Pin4)  
#define PWMO1               (PORT5 | Pin6)
#define I2CS_SCL            (PORT8 | Pin0)
#define I2CS_SDA            (PORT8 | Pin1)
#define SPIM_MOSI           (PORT11 | Pin0)
#define SPIM_MISO           (PORT11 | Pin1)
#define SPIM_SCL            (PORT11 | Pin2)
#define SPIM_CS0            (PORT11 | Pin3)
#define SPIM_CS1            (PORT11 | Pin4)
#define SPIM_CS2            (PORT11 | Pin5)
#endif

/* ==[ Types ]=============================================================== */
/** \brief Define GPIO high level and low level */
typedef enum GPIO_PIN_LEVEL
{
    GPIO_LEVEL_LOW,
    GPIO_LEVEL_HIGH
} t_GPIO_PIN_LEVEL;

/* ==[ Variables with external linkage ]===================================== */
/* ==[ Declaration of functions with external linkage ]====================== */
extern t_FUNC_RETURN Gpio_Init(void);
extern uint8_t Gpio_GetPin(uint8_t PortPin);
extern uint8_t Gpio_GetPort(uint8_t Port);
extern t_FUNC_RETURN Gpio_WritePin(uint8_t PortPin, uint8_t Level);
extern t_FUNC_RETURN Gpio_WritePort(uint8_t Port, uint8_t value);
extern t_FUNC_RETURN Gpio_Inv(uint8_t PortPin);
extern t_FUNC_RETURN Gpio_PinConfig(uint8_t PortPin, uint32_t PinDriverMode, uint8_t defVal);
#endif /* GPIO_H_ */
