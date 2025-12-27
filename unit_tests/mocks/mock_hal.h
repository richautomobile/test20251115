/**
 * @file mock_hal.h
 * @brief Mock Hardware Abstraction Layer for unit testing
 *
 * Provides mock implementations of hardware functions for isolated unit testing.
 */

#ifndef MOCK_HAL_H
#define MOCK_HAL_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Mock call tracking */
typedef struct {
    const char* function_name;
    uint32_t call_count;
    int last_return_value;
    void* last_param;
} mock_call_record_t;

/* Mock HAL state */
typedef struct {
    bool initialized;
    uint32_t total_calls;
    mock_call_record_t* records;
    uint32_t record_count;
} mock_hal_state_t;

/* Global mock state */
extern mock_hal_state_t g_mock_hal_state;

/* Mock control functions */
void mock_hal_init(void);
void mock_hal_reset(void);
void mock_hal_set_return_value(const char* func_name, int value);
int mock_hal_get_call_count(const char* func_name);
void mock_hal_verify_called(const char* func_name, uint32_t expected_count);

/* Mock register access */
void mock_hal_write_register(uint32_t address, uint32_t value);
uint32_t mock_hal_read_register(uint32_t address);

/* Mock memory access */
void mock_hal_write_memory(uint32_t address, const uint8_t* data, uint32_t length);
void mock_hal_read_memory(uint32_t address, uint8_t* data, uint32_t length);

/* BSP mocks */
typedef uint32_t cy_rslt_t;
#define CY_RSLT_SUCCESS (0x00000000U)

cy_rslt_t cybsp_init(void);
void CY_ASSERT(uint8_t x);

/* GPIO mocks */
#define GPIO_HIGH 1
#define GPIO_LOW  0

typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_OPEN_DRAIN
} gpio_mode_t;

int mock_gpio_init(uint8_t pin, gpio_mode_t mode);
int mock_gpio_write(uint8_t pin, uint8_t value);
uint8_t mock_gpio_read(uint8_t pin);

/* I2C mocks */
typedef struct {
    uint8_t bus_id;
    uint32_t speed;
    bool initialized;
} mock_i2c_config_t;

int mock_i2c_init(uint8_t bus_id, uint32_t speed);
int mock_i2c_write(uint8_t bus_id, uint8_t dev_addr, uint8_t reg_addr,
                   const uint8_t* data, uint32_t length);
int mock_i2c_read(uint8_t bus_id, uint8_t dev_addr, uint8_t reg_addr,
                  uint8_t* data, uint32_t length);

/* SPI mocks */
typedef struct {
    uint8_t bus_id;
    uint32_t speed;
    uint8_t mode;
    bool initialized;
} mock_spi_config_t;

int mock_spi_init(uint8_t bus_id, uint32_t speed, uint8_t mode);
int mock_spi_transfer(uint8_t bus_id, const uint8_t* tx_data,
                      uint8_t* rx_data, uint32_t length);

/* ADC mocks */
typedef struct {
    uint8_t channel;
    uint16_t value;
    bool initialized;
} mock_adc_config_t;

int mock_adc_init(uint8_t channel);
uint16_t mock_adc_read(uint8_t channel);
void mock_adc_set_value(uint8_t channel, uint16_t value);

/* PWM mocks */
typedef struct {
    uint8_t channel;
    uint32_t frequency;
    uint8_t duty_cycle;
    bool running;
    bool initialized;
} mock_pwm_config_t;

int mock_pwm_init(uint8_t channel, uint32_t frequency, uint8_t duty);
int mock_pwm_start(uint8_t channel);
int mock_pwm_stop(uint8_t channel);
int mock_pwm_set_duty(uint8_t channel, uint8_t duty);

/* Timer mocks */
typedef struct {
    uint8_t timer_id;
    uint32_t count;
    bool running;
} mock_timer_state_t;

int mock_timer_init(uint8_t timer_id);
int mock_timer_start(uint8_t timer_id);
int mock_timer_stop(uint8_t timer_id);
uint32_t mock_timer_get_count(uint8_t timer_id);

/* Flash mocks */
#define MOCK_FLASH_SIZE (64 * 1024)
#define MOCK_FLASH_PAGE_SIZE (256)

int mock_flash_init(void);
int mock_flash_write(uint32_t address, const uint8_t* data, uint32_t length);
int mock_flash_read(uint32_t address, uint8_t* data, uint32_t length);
int mock_flash_erase_page(uint32_t page_number);

/* UART mocks */
typedef struct {
    uint8_t uart_id;
    uint32_t baudrate;
    uint8_t* rx_buffer;
    uint32_t rx_buffer_size;
    uint32_t rx_buffer_head;
    uint32_t rx_buffer_tail;
    bool initialized;
} mock_uart_config_t;

int mock_uart_init(uint8_t uart_id, uint32_t baudrate);
int mock_uart_write(uint8_t uart_id, const uint8_t* data, uint32_t length);
int mock_uart_read(uint8_t uart_id, uint8_t* data, uint32_t length);

/* Interrupt mocks */
typedef void (*interrupt_handler_t)(void);

void mock_enable_interrupts(void);
void mock_disable_interrupts(void);
void mock_register_interrupt(uint32_t irq_num, interrupt_handler_t handler);
void mock_trigger_interrupt(uint32_t irq_num);

/* System mocks */
void mock_delay_ms(uint32_t ms);
void mock_delay_us(uint32_t us);
uint32_t mock_get_tick_count(void);
void mock_system_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* MOCK_HAL_H */
