# PSoC4 Unit Testing Solution

Professional C unit testing framework for PSoC4 embedded applications following industry best practices.

## Overview

This unit test solution provides:
- **Isolated unit testing** for embedded C code
- **Mock framework** for hardware abstraction
- **Code coverage** analysis with gcov/lcov
- **Automated test execution** with make and shell scripts
- **CI/CD integration** support
- **Industry standard practices** for embedded testing

## Directory Structure

```
unit_tests/
├── README.md                # This file
├── Makefile                 # Build and test automation
├── run_tests.sh            # Test runner script
├── framework/              # Test framework
│   ├── test_framework.h   # Test macros and utilities
│   ├── test_framework.c   # Test framework implementation
│   └── unity_config.h     # Unity configuration
├── mocks/                  # Hardware abstraction mocks
│   ├── mock_hal.h         # Mock HAL header
│   └── mock_hal.c         # Mock HAL implementation
├── tests/                  # Unit tests
│   ├── driver/            # Driver tests
│   │   ├── test_i2c.c
│   │   ├── test_adc.c
│   │   └── test_gpio.c
│   └── system/            # System tests
│       └── test_system_init.c
├── runners/                # Test runners
│   └── test_runner_all.c
├── build/                  # Build artifacts (generated)
└── coverage/               # Coverage reports (generated)
```

## Quick Start

### Prerequisites

- GCC compiler
- GNU Make
- (Optional) lcov for coverage reports
- (Optional) valgrind for memory checking

```bash
# On Ubuntu/Debian
sudo apt-get install gcc make lcov valgrind

# On macOS
brew install gcc make lcov
```

### Build and Run Tests

```bash
# Build all tests
make build

# Run all tests
make test

# Run specific test
make test-i2c
make test-adc
make test-gpio
make test-system
```

### Using the Test Runner Script

```bash
# Run all tests
./run_tests.sh all

# Run specific tests
./run_tests.sh i2c
./run_tests.sh adc
./run_tests.sh gpio
./run_tests.sh system

# Generate coverage
./run_tests.sh coverage
```

## Test Framework

### Unity-Compatible Test Macros

```c
TEST_ASSERT(condition)
TEST_ASSERT_EQUAL(expected, actual)
TEST_ASSERT_EQUAL_INT(expected, actual)
TEST_ASSERT_EQUAL_HEX(expected, actual)
TEST_ASSERT_NOT_EQUAL(expected, actual)
TEST_ASSERT_NULL(pointer)
TEST_ASSERT_NOT_NULL(pointer)
TEST_ASSERT_TRUE(condition)
TEST_ASSERT_FALSE(condition)
TEST_ASSERT_EQUAL_STRING(expected, actual)
TEST_ASSERT_EQUAL_MEMORY(expected, actual, len)
TEST_ASSERT_IN_RANGE(value, min, max)
TEST_ASSERT_BIT_HIGH(reg, bit)
TEST_ASSERT_BIT_LOW(reg, bit)
```

### Example Test

```c
#include "../../framework/test_framework.h"
#include "../../mocks/mock_hal.h"

static void setUp(void)
{
    mock_hal_init();
}

static void tearDown(void)
{
    mock_hal_reset();
}

static void test_example(void)
{
    // Arrange
    mock_i2c_init(0, 400000);

    // Act
    uint8_t data = 0xAA;
    int result = mock_i2c_write(0, 0x50, 0x00, &data, 1);

    // Assert
    TEST_ASSERT_EQUAL(0, result);
}

int main(void)
{
    UnityBegin("Example Tests");
    RUN_TEST(test_example);
    return UnityEnd();
}
```

## Mock Framework

### Available Mocks

**BSP/System:**
- `cybsp_init()` - Board support package initialization
- `CY_ASSERT(x)` - Assertion macro
- `mock_system_reset()` - System reset
- `mock_delay_ms(ms)` - Millisecond delay
- `mock_get_tick_count()` - Get system tick count

**GPIO:**
- `mock_gpio_init(pin, mode)` - Initialize GPIO pin
- `mock_gpio_write(pin, value)` - Write GPIO pin
- `mock_gpio_read(pin)` - Read GPIO pin

**I2C:**
- `mock_i2c_init(bus_id, speed)` - Initialize I2C bus
- `mock_i2c_write(bus, addr, reg, data, len)` - Write to I2C device
- `mock_i2c_read(bus, addr, reg, data, len)` - Read from I2C device

**ADC:**
- `mock_adc_init(channel)` - Initialize ADC channel
- `mock_adc_read(channel)` - Read ADC value
- `mock_adc_set_value(channel, value)` - Set test value

**PWM:**
- `mock_pwm_init(channel, freq, duty)` - Initialize PWM
- `mock_pwm_start(channel)` - Start PWM output
- `mock_pwm_stop(channel)` - Stop PWM output
- `mock_pwm_set_duty(channel, duty)` - Set duty cycle

**Flash:**
- `mock_flash_init()` - Initialize flash
- `mock_flash_write(addr, data, len)` - Write to flash
- `mock_flash_read(addr, data, len)` - Read from flash
- `mock_flash_erase_page(page)` - Erase flash page

**Register/Memory Access:**
- `mock_hal_write_register(addr, value)` - Write register
- `mock_hal_read_register(addr)` - Read register
- `mock_hal_write_memory(addr, data, len)` - Write memory
- `mock_hal_read_memory(addr, data, len)` - Read memory

### Mock Control

```c
// Initialize mock framework
mock_hal_init();

// Reset mock state
mock_hal_reset();

// Set ADC test value
mock_adc_set_value(0, 2048);

// Verify GPIO state
uint8_t value = mock_gpio_read(5);
TEST_ASSERT_EQUAL(GPIO_HIGH, value);
```

## Code Coverage

### Generate Coverage Report

```bash
# Generate coverage with HTML report
make coverage

# Open coverage report
make coverage-report

# Or manually open
open coverage/html/index.html  # macOS
xdg-open coverage/html/index.html  # Linux
```

### Coverage Targets

- **Framework code**: Test framework itself
- **Mock code**: Hardware abstraction mocks
- **Test code**: Unit test coverage

### Understanding Coverage

The coverage report shows:
- **Line coverage**: % of lines executed
- **Function coverage**: % of functions called
- **Branch coverage**: % of branches taken

## Best Practices

### 1. Test Structure

Follow the Arrange-Act-Assert (AAA) pattern:

```c
static void test_example(void)
{
    // Arrange - Setup test conditions
    mock_i2c_init(0, 400000);
    uint8_t expected_data = 0xAA;

    // Act - Execute the code under test
    uint8_t test_data = 0xAA;
    mock_i2c_write(0, 0x50, 0x00, &test_data, 1);
    uint8_t read_data;
    mock_i2c_read(0, 0x50, 0x00, &read_data, 1);

    // Assert - Verify the results
    TEST_ASSERT_EQUAL(expected_data, read_data);
}
```

### 2. Setup and Teardown

Use setup/teardown functions for common initialization:

```c
static void setUp(void)
{
    mock_hal_init();
    // Common setup code
}

static void tearDown(void)
{
    mock_hal_reset();
    // Common cleanup code
}
```

### 3. Test Naming

Use descriptive test names:

```c
test_i2c_write_single_byte()
test_i2c_write_multiple_bytes()
test_i2c_write_without_init_fails()
test_adc_read_returns_default_value()
test_gpio_toggle_from_low_to_high()
```

### 4. Test Independence

Each test should be independent:

```c
// Good - Each test manages its own state
static void test_1(void)
{
    setUp();
    // Test code
    tearDown();
}

static void test_2(void)
{
    setUp();
    // Test code
    tearDown();
}

// Bad - Tests depend on each other
static int global_state;

static void test_1(void)
{
    global_state = 1;  // Affects test_2
}

static void test_2(void)
{
    // Assumes global_state == 1
}
```

### 5. Test Data Patterns

Test boundary conditions and common patterns:

```c
// Boundary values
TEST_ASSERT_EQUAL(0, adc_read());      // Minimum
TEST_ASSERT_EQUAL(4095, adc_read());   // Maximum

// Common patterns
uint8_t pattern_0x00[] = {0x00, 0x00, 0x00, 0x00};
uint8_t pattern_0xFF[] = {0xFF, 0xFF, 0xFF, 0xFF};
uint8_t pattern_alt[] = {0xAA, 0x55, 0xAA, 0x55};
```

### 6. Error Conditions

Always test error conditions:

```c
// Test successful path
TEST_ASSERT_EQUAL(0, mock_i2c_init(0, 400000));

// Test error path
TEST_ASSERT_NOT_EQUAL(0, mock_i2c_init(99, 400000));
```

## Writing New Tests

### 1. Create Test File

```c
/**
 * @file test_my_module.c
 * @brief Unit tests for my module
 */

#include "../../framework/test_framework.h"
#include "../../mocks/mock_hal.h"

static void setUp(void)
{
    mock_hal_init();
}

static void tearDown(void)
{
    mock_hal_reset();
}

static void test_my_function(void)
{
    // Your test here
    TEST_ASSERT_TRUE(true);
}

int main(void)
{
    UnityBegin("My Module Tests");
    RUN_TEST(test_my_function);
    return UnityEnd();
}
```

### 2. Add to Makefile

```makefile
TEST_MY_MODULE := $(BUILD_DIR)/test_my_module

$(TEST_MY_MODULE): $(TESTS_DIR)/my_module/test_my_module.c $(COMMON_OBJS)
	@$(CC) $(CFLAGS) $< $(COMMON_OBJS) -o $@

test-my-module: $(TEST_MY_MODULE)
	@./$(TEST_MY_MODULE)
```

### 3. Build and Run

```bash
make test-my-module
```

## Continuous Integration

### GitHub Actions Example

```yaml
name: Unit Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - name: Install dependencies
        run: sudo apt-get install -y gcc make lcov
      - name: Build tests
        run: cd unit_tests && make build
      - name: Run tests
        run: cd unit_tests && make test
      - name: Generate coverage
        run: cd unit_tests && make coverage
      - name: Upload coverage
        uses: codecov/codecov-action@v3
        with:
          files: ./unit_tests/coverage/coverage.info
```

## Troubleshooting

### Build Errors

**Problem**: "undefined reference to..."
```bash
# Solution: Check that all source files are included in Makefile
make clean
make build
```

**Problem**: "No such file or directory"
```bash
# Solution: Verify paths in #include statements
# Use relative paths from test file location
#include "../../framework/test_framework.h"
```

### Test Failures

**Problem**: Tests fail unexpectedly
```bash
# Run with verbose output
./build/test_i2c

# Check mock state
# Add debug prints in test code
printf("Debug: value = %d\n", value);
```

### Coverage Issues

**Problem**: Coverage report not generated
```bash
# Check if lcov is installed
lcov --version

# If not installed
sudo apt-get install lcov

# Manual coverage with gcov
cd build
gcov *.gcda
```

## Memory Checking

Check for memory leaks with valgrind:

```bash
make memcheck
```

Or manually:

```bash
valgrind --leak-check=full ./build/test_i2c
```

## Static Analysis

Run static analysis with cppcheck:

```bash
make static-analysis
```

## Performance

For performance-critical code, use benchmark tests:

```c
static void test_performance_i2c_write(void)
{
    mock_i2c_init(0, 400000);

    uint32_t start = mock_get_tick_count();

    for (int i = 0; i < 1000; i++) {
        uint8_t data = i & 0xFF;
        mock_i2c_write(0, 0x50, 0x00, &data, 1);
    }

    uint32_t end = mock_get_tick_count();
    uint32_t duration = end - start;

    printf("1000 writes took %u ms\n", duration);
    TEST_ASSERT_TRUE(duration < 100);  // Should be fast
}
```

## Integration with Unity

To use the full Unity framework instead of minimal macros:

1. Clone Unity:
```bash
git clone https://github.com/ThrowTheSwitch/Unity.git framework/Unity
```

2. Update Makefile:
```makefile
CFLAGS += -DUSE_UNITY_FRAMEWORK
CFLAGS += -Iframework/Unity/src
UNITY_SRC := framework/Unity/src/unity.c
```

3. Build and run as normal

## Support

For issues or questions:
1. Check this README
2. Review example tests
3. Examine mock implementations
4. Check Unity documentation: https://github.com/ThrowTheSwitch/Unity

---

**Happy Testing!** ✅
