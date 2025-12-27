/**
 * @file mock_hal.c
 * @brief Mock HAL implementation
 */

#include "mock_hal.h"
#include <string.h>
#include <stdio.h>

/* Mock state */
mock_hal_state_t g_mock_hal_state;

/* Mock storage */
static uint32_t mock_registers[256];
static uint8_t mock_memory[1024];
static uint8_t mock_flash[MOCK_FLASH_SIZE];
static mock_gpio_config_t mock_gpio_state[32];
static mock_i2c_config_t mock_i2c_state[4];
static mock_spi_config_t mock_spi_state[4];
static mock_adc_config_t mock_adc_state[8];
static mock_pwm_config_t mock_pwm_state[8];
static mock_timer_state_t mock_timer_state[4];
static mock_uart_config_t mock_uart_state[4];

typedef struct {
    uint8_t pin;
    uint8_t value;
    gpio_mode_t mode;
} mock_gpio_config_t;

/* ========================================================================
 * Mock control functions
 * ======================================================================== */

void mock_hal_init(void)
{
    memset(&g_mock_hal_state, 0, sizeof(g_mock_hal_state));
    memset(mock_registers, 0, sizeof(mock_registers));
    memset(mock_memory, 0, sizeof(mock_memory));
    memset(mock_flash, 0xFF, sizeof(mock_flash));  // Erased state
    memset(mock_gpio_state, 0, sizeof(mock_gpio_state));
    memset(mock_i2c_state, 0, sizeof(mock_i2c_state));
    memset(mock_spi_state, 0, sizeof(mock_spi_state));
    memset(mock_adc_state, 0, sizeof(mock_adc_state));
    memset(mock_pwm_state, 0, sizeof(mock_pwm_state));
    memset(mock_timer_state, 0, sizeof(mock_timer_state));
    memset(mock_uart_state, 0, sizeof(mock_uart_state));

    g_mock_hal_state.initialized = true;
}

void mock_hal_reset(void)
{
    mock_hal_init();
}

/* ========================================================================
 * Mock register/memory access
 * ======================================================================== */

void mock_hal_write_register(uint32_t address, uint32_t value)
{
    uint32_t index = address & 0xFF;
    mock_registers[index] = value;
}

uint32_t mock_hal_read_register(uint32_t address)
{
    uint32_t index = address & 0xFF;
    return mock_registers[index];
}

void mock_hal_write_memory(uint32_t address, const uint8_t* data, uint32_t length)
{
    if (address + length <= sizeof(mock_memory)) {
        memcpy(&mock_memory[address], data, length);
    }
}

void mock_hal_read_memory(uint32_t address, uint8_t* data, uint32_t length)
{
    if (address + length <= sizeof(mock_memory)) {
        memcpy(data, &mock_memory[address], length);
    }
}

/* ========================================================================
 * BSP mocks
 * ======================================================================== */

cy_rslt_t cybsp_init(void)
{
    g_mock_hal_state.total_calls++;
    return CY_RSLT_SUCCESS;
}

void CY_ASSERT(uint8_t x)
{
    if (!x) {
        printf("ASSERTION FAILED!\n");
    }
}

/* ========================================================================
 * GPIO mocks
 * ======================================================================== */

int mock_gpio_init(uint8_t pin, gpio_mode_t mode)
{
    if (pin >= 32) return -1;

    mock_gpio_state[pin].pin = pin;
    mock_gpio_state[pin].mode = mode;
    return 0;
}

int mock_gpio_write(uint8_t pin, uint8_t value)
{
    if (pin >= 32) return -1;

    mock_gpio_state[pin].value = value;
    return 0;
}

uint8_t mock_gpio_read(uint8_t pin)
{
    if (pin >= 32) return 0;

    return mock_gpio_state[pin].value;
}

/* ========================================================================
 * I2C mocks
 * ======================================================================== */

int mock_i2c_init(uint8_t bus_id, uint32_t speed)
{
    if (bus_id >= 4) return -1;

    mock_i2c_state[bus_id].bus_id = bus_id;
    mock_i2c_state[bus_id].speed = speed;
    mock_i2c_state[bus_id].initialized = true;
    return 0;
}

int mock_i2c_write(uint8_t bus_id, uint8_t dev_addr, uint8_t reg_addr,
                   const uint8_t* data, uint32_t length)
{
    if (bus_id >= 4 || !mock_i2c_state[bus_id].initialized) return -1;

    /* Store in mock memory for verification */
    uint32_t addr = (dev_addr << 8) | reg_addr;
    if (addr + length <= sizeof(mock_memory)) {
        memcpy(&mock_memory[addr], data, length);
    }

    return 0;
}

int mock_i2c_read(uint8_t bus_id, uint8_t dev_addr, uint8_t reg_addr,
                  uint8_t* data, uint32_t length)
{
    if (bus_id >= 4 || !mock_i2c_state[bus_id].initialized) return -1;

    /* Read from mock memory */
    uint32_t addr = (dev_addr << 8) | reg_addr;
    if (addr + length <= sizeof(mock_memory)) {
        memcpy(data, &mock_memory[addr], length);
    }

    return 0;
}

/* ========================================================================
 * ADC mocks
 * ======================================================================== */

int mock_adc_init(uint8_t channel)
{
    if (channel >= 8) return -1;

    mock_adc_state[channel].channel = channel;
    mock_adc_state[channel].value = 2048;  // Mid-range default
    mock_adc_state[channel].initialized = true;
    return 0;
}

uint16_t mock_adc_read(uint8_t channel)
{
    if (channel >= 8 || !mock_adc_state[channel].initialized) return 0;

    return mock_adc_state[channel].value;
}

void mock_adc_set_value(uint8_t channel, uint16_t value)
{
    if (channel < 8) {
        mock_adc_state[channel].value = value;
    }
}

/* ========================================================================
 * PWM mocks
 * ======================================================================== */

int mock_pwm_init(uint8_t channel, uint32_t frequency, uint8_t duty)
{
    if (channel >= 8 || duty > 100) return -1;

    mock_pwm_state[channel].channel = channel;
    mock_pwm_state[channel].frequency = frequency;
    mock_pwm_state[channel].duty_cycle = duty;
    mock_pwm_state[channel].running = false;
    mock_pwm_state[channel].initialized = true;
    return 0;
}

int mock_pwm_start(uint8_t channel)
{
    if (channel >= 8 || !mock_pwm_state[channel].initialized) return -1;

    mock_pwm_state[channel].running = true;
    return 0;
}

int mock_pwm_stop(uint8_t channel)
{
    if (channel >= 8 || !mock_pwm_state[channel].initialized) return -1;

    mock_pwm_state[channel].running = false;
    return 0;
}

int mock_pwm_set_duty(uint8_t channel, uint8_t duty)
{
    if (channel >= 8 || !mock_pwm_state[channel].initialized || duty > 100) return -1;

    mock_pwm_state[channel].duty_cycle = duty;
    return 0;
}

/* ========================================================================
 * Flash mocks
 * ======================================================================== */

int mock_flash_init(void)
{
    memset(mock_flash, 0xFF, sizeof(mock_flash));
    return 0;
}

int mock_flash_write(uint32_t address, const uint8_t* data, uint32_t length)
{
    if (address + length > MOCK_FLASH_SIZE) return -1;

    memcpy(&mock_flash[address], data, length);
    return 0;
}

int mock_flash_read(uint32_t address, uint8_t* data, uint32_t length)
{
    if (address + length > MOCK_FLASH_SIZE) return -1;

    memcpy(data, &mock_flash[address], length);
    return 0;
}

int mock_flash_erase_page(uint32_t page_number)
{
    uint32_t start = page_number * MOCK_FLASH_PAGE_SIZE;

    if (start >= MOCK_FLASH_SIZE) return -1;

    uint32_t length = MOCK_FLASH_PAGE_SIZE;
    if (start + length > MOCK_FLASH_SIZE) {
        length = MOCK_FLASH_SIZE - start;
    }

    memset(&mock_flash[start], 0xFF, length);
    return 0;
}

/* ========================================================================
 * System mocks
 * ======================================================================== */

static uint32_t mock_tick_count = 0;

void mock_delay_ms(uint32_t ms)
{
    mock_tick_count += ms;
}

void mock_delay_us(uint32_t us)
{
    mock_tick_count += (us / 1000);
}

uint32_t mock_get_tick_count(void)
{
    return mock_tick_count;
}

void mock_system_reset(void)
{
    mock_hal_reset();
}
