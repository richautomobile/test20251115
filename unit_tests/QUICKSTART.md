# Quick Start - PSoC4 Unit Testing

Get unit testing running in 5 minutes!

## Step 1: Install Tools (1 minute)

```bash
# Ubuntu/Debian
sudo apt-get install gcc make

# macOS
brew install gcc make

# Optional (for coverage)
sudo apt-get install lcov  # Ubuntu
brew install lcov          # macOS
```

## Step 2: Build Tests (1 minute)

```bash
cd unit_tests
make build
```

Expected output:
```
Compiling framework/test_framework.c...
Compiling mocks/mock_hal.c...
Building I2C tests...
Building ADC tests...
Building GPIO tests...
Building System tests...
==========================================
All tests built successfully!
==========================================
```

## Step 3: Run Tests (1 minute)

```bash
make test
```

Expected output:
```
==========================================
Running All Unit Tests
==========================================

Running I2C tests...
========================================
Test Suite: I2C Driver Tests
========================================
Running: test_i2c_init_returns_success
Running: test_i2c_write_single_byte
...
========================================
Tests Run: 13
Failures:  0
Success:   13
Result:    PASS
========================================

==========================================
All Tests Completed
==========================================
```

## Step 4: Try Specific Tests (1 minute)

```bash
# Run I2C tests only
make test-i2c

# Run ADC tests only
make test-adc

# Run GPIO tests only
make test-gpio

# Run system tests only
make test-system
```

## Step 5: Generate Coverage (1 minute)

```bash
make coverage
```

Then open `coverage/html/index.html` in your browser to see the coverage report.

## Quick Command Reference

```bash
# Build
make build          # Build all tests
make clean          # Clean build artifacts

# Test
make test           # Run all tests
make test-i2c       # Run I2C tests
make test-adc       # Run ADC tests
make test-gpio      # Run GPIO tests
make test-system    # Run system tests

# Coverage
make coverage       # Generate coverage report
make coverage-report # Generate and open report

# Utilities
make help           # Show help
make ci             # Run full CI pipeline
```

## Using the Shell Script

```bash
# Make it executable (first time only)
chmod +x run_tests.sh

# Run tests
./run_tests.sh all      # All tests
./run_tests.sh i2c      # I2C only
./run_tests.sh coverage # Coverage report
```

## Understanding Test Output

### PASS:
```
========================================
Tests Run: 10
Failures:  0
Success:   10
Result:    PASS
========================================
```

### FAIL:
```
FAIL: tests/driver/test_i2c.c:45 - Expected 0, got -1
========================================
Tests Run: 10
Failures:  1
Success:   9
Result:    FAIL
========================================
```

## Writing Your First Test

Create `tests/driver/test_example.c`:

```c
#include "../../framework/test_framework.h"
#include "../../mocks/mock_hal.h"

static void test_example_passes(void)
{
    TEST_ASSERT_EQUAL(1, 1);
}

static void test_example_i2c(void)
{
    mock_hal_init();
    mock_i2c_init(0, 400000);

    uint8_t data = 0xAA;
    int result = mock_i2c_write(0, 0x50, 0x00, &data, 1);

    TEST_ASSERT_EQUAL(0, result);
    mock_hal_reset();
}

int main(void)
{
    UnityBegin("Example Tests");
    RUN_TEST(test_example_passes);
    RUN_TEST(test_example_i2c);
    return UnityEnd();
}
```

Build and run:

```bash
gcc -Wall -Iframework -Imocks \
    tests/driver/test_example.c \
    framework/test_framework.c \
    mocks/mock_hal.c \
    -o build/test_example

./build/test_example
```

## Test Patterns

### Basic Test
```c
static void test_basic(void)
{
    // Arrange
    int expected = 42;

    // Act
    int actual = 42;

    // Assert
    TEST_ASSERT_EQUAL(expected, actual);
}
```

### Test with Mocks
```c
static void test_with_mock(void)
{
    // Arrange
    mock_hal_init();
    mock_adc_init(0);
    mock_adc_set_value(0, 2048);

    // Act
    uint16_t value = mock_adc_read(0);

    // Assert
    TEST_ASSERT_EQUAL(2048, value);

    // Cleanup
    mock_hal_reset();
}
```

### Test Error Conditions
```c
static void test_error_condition(void)
{
    // Act - call with invalid parameter
    int result = mock_i2c_init(99, 400000);

    // Assert - should return error
    TEST_ASSERT_NOT_EQUAL(0, result);
}
```

## Common Assertions

```c
TEST_ASSERT_EQUAL(expected, actual)
TEST_ASSERT_NOT_EQUAL(a, b)
TEST_ASSERT_TRUE(condition)
TEST_ASSERT_FALSE(condition)
TEST_ASSERT_NULL(pointer)
TEST_ASSERT_NOT_NULL(pointer)
TEST_ASSERT_EQUAL_HEX(0xAA, value)
TEST_ASSERT_EQUAL_STRING("hello", str)
TEST_ASSERT_IN_RANGE(value, 0, 100)
TEST_ASSERT_BIT_HIGH(register, 5)
TEST_ASSERT_BIT_LOW(register, 3)
```

## Troubleshooting

**Build fails:**
```bash
make clean
make build
```

**Tests fail unexpectedly:**
```bash
# Run specific test for debugging
./build/test_i2c
```

**Need coverage:**
```bash
make coverage
open coverage/html/index.html
```

## Next Steps

1. ✅ Read full [README.md](README.md)
2. ✅ Examine example tests in `tests/`
3. ✅ Review mock implementations in `mocks/`
4. ✅ Write tests for your modules
5. ✅ Set up CI/CD integration

## Tips

- **Run tests frequently** - After each code change
- **Write tests first** - TDD (Test-Driven Development)
- **Keep tests simple** - One assertion per test when possible
- **Use descriptive names** - `test_i2c_write_single_byte`
- **Test error paths** - Not just happy paths
- **Mock hardware** - Isolate unit under test
- **Check coverage** - Aim for >80%

---

**Ready to test!** 🚀

Common workflow:
```bash
make build && make test && make coverage
```
