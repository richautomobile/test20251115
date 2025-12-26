# PSoC4 Integration Testing with Pytest

A comprehensive integration test framework for PSoC4 embedded C applications using Python and pytest.

## Overview

This testing framework allows you to:
- Test embedded C code without requiring hardware
- Mock hardware peripherals (I2C, SPI, ADC, PWM, CAN-FD, etc.)
- Run automated integration and unit tests
- Generate coverage reports
- Integrate with CI/CD pipelines

## Directory Structure

```
tests/
├── conftest.py              # Pytest configuration and fixtures
├── requirements.txt         # Python dependencies
├── Makefile                 # Test automation targets
├── run_tests.sh            # Test runner script
├── README.md               # This file
├── integration/            # Integration tests
│   ├── test_system_integration.py
│   └── test_driver_integration.py
├── unit/                   # Unit tests
│   └── test_peripheral_mocks.py
├── mocks/                  # Hardware mocks
│   ├── hal_mock.py
│   ├── peripheral_mocks.py
│   └── __init__.py
├── wrappers/               # C-Python bindings
│   ├── system_wrapper.py
│   └── __init__.py
├── fixtures/               # Test fixtures and data
└── utils/                  # Test utilities
```

## Quick Start

### 1. Install Dependencies

```bash
cd tests
pip install -r requirements.txt
```

Or using make:

```bash
make install
```

### 2. Run Tests

Run all tests:
```bash
pytest
```

Or using the test runner:
```bash
./run_tests.sh all
```

Or using make:
```bash
make test
```

### 3. View Results

Test results will be displayed in the terminal, and reports will be generated:
- HTML report: `tests/report.html`
- Coverage report: `tests/htmlcov/index.html`

## Running Specific Tests

### By Category

```bash
# Unit tests only
pytest -m unit
make test-unit

# Integration tests only
pytest -m integration
make test-integration

# Smoke tests (quick verification)
pytest -m smoke
make test-smoke
```

### By Peripheral

```bash
# I2C tests
pytest -m i2c
make test-i2c

# SPI tests
pytest -m spi
make test-spi

# ADC tests
pytest -m adc
make test-adc

# PWM tests
pytest -m pwm
make test-pwm

# CAN-FD tests
pytest -m canfd
make test-canfd

# GPIO tests
pytest -m gpio
make test-gpio

# Flash tests
pytest -m flash
make test-flash

# System tests
pytest -m system
make test-system
```

### By Test Type

```bash
# Simulation tests (using mocks)
pytest -m simulation

# Hardware tests (requires actual hardware)
pytest -m hardware

# Slow tests
pytest -m slow
```

## Test Markers

Tests are organized using pytest markers:

- `@pytest.mark.unit` - Unit tests
- `@pytest.mark.integration` - Integration tests
- `@pytest.mark.hardware` - Requires actual hardware
- `@pytest.mark.simulation` - Uses hardware mocks
- `@pytest.mark.slow` - Long-running tests
- `@pytest.mark.smoke` - Quick smoke tests
- `@pytest.mark.i2c` - I2C-specific tests
- `@pytest.mark.spi` - SPI-specific tests
- `@pytest.mark.adc` - ADC-specific tests
- `@pytest.mark.pwm` - PWM-specific tests
- `@pytest.mark.canfd` - CAN-FD-specific tests
- `@pytest.mark.gpio` - GPIO-specific tests
- `@pytest.mark.flash` - Flash-specific tests
- `@pytest.mark.system` - System-level tests

## Writing Tests

### Example Integration Test

```python
import pytest
from wrappers.system_wrapper import SystemWrapper
from mocks.peripheral_mocks import I2CMock

@pytest.mark.integration
@pytest.mark.i2c
def test_i2c_communication():
    """Test I2C write-read cycle."""
    # Create I2C mock
    i2c = I2CMock(bus_id=0)
    i2c.init(speed=400000)
    i2c.add_device(0x50)

    # Write data
    result = i2c.write(0x50, 0x00, b"\x01\x02\x03")
    assert result == 0

    # Read back
    data = i2c.read(0x50, 0x00, 3)
    assert data == b"\x01\x02\x03"
```

### Example Unit Test

```python
import pytest
from mocks.peripheral_mocks import ADCMock

@pytest.mark.unit
def test_adc_value_bounds():
    """Test ADC value clamping."""
    adc = ADCMock(resolution=12)
    adc.init()

    # Test upper bound
    adc.set_value(9999)
    assert adc.read() == 4095

    # Test lower bound
    adc.set_value(-100)
    assert adc.read() == 0
```

## Using Fixtures

Common fixtures are defined in `conftest.py`:

```python
def test_with_hal(mock_hal):
    """Test using the mock HAL fixture."""
    result = mock_hal.cybsp_init()
    assert result == 0

def test_with_i2c_data(sample_i2c_data):
    """Test using sample I2C data fixture."""
    addr = sample_i2c_data["device_address"]
    data = sample_i2c_data["write_data"]
    # Use the test data...
```

Available fixtures:
- `mock_hal` - Mock hardware abstraction layer
- `mock_system` - Mock system functions
- `sample_i2c_data` - Sample I2C test data
- `sample_canfd_frame` - Sample CAN-FD frame
- `sample_adc_values` - Sample ADC values
- `test_data_dir` - Temporary directory for test data
- `hardware_config` - Hardware configuration
- `build_config` - Build configuration

## Coverage Reports

Generate coverage report:

```bash
pytest --cov-report=html --cov-report=term
make coverage
```

View HTML report:
```bash
make coverage-html
```

## Continuous Integration

For CI/CD pipelines, use:

```bash
make ci
```

This will:
1. Clean previous artifacts
2. Install dependencies
3. Run all tests
4. Generate coverage reports

## Advanced Usage

### Running Tests in Parallel

```bash
pytest -n auto
make test-parallel
```

### Verbose Output

```bash
pytest -vv
make test-verbose
```

### Stopping on First Failure

```bash
pytest -x
```

### Running Specific Test File

```bash
pytest tests/integration/test_system_integration.py
```

### Running Specific Test Function

```bash
pytest tests/integration/test_system_integration.py::test_variable_init_succeeds
```

### Debugging Tests

```bash
# Show local variables on failure
pytest -l

# Start debugger on failure
pytest --pdb

# Show print statements
pytest -s
```

## Hardware Testing

To run tests on actual hardware:

1. Compile C code to shared library:
```bash
# Your build process here
# Example: gcc -shared -fPIC -o libpsoc4.so source/*.c
```

2. Update wrapper to use the library:
```python
system = SystemWrapper(lib_path="path/to/libpsoc4.so")
```

3. Run hardware tests:
```bash
pytest -m hardware
```

## Mocking Hardware

The framework provides comprehensive mocks for:

- **HAL** - Hardware Abstraction Layer
- **I2C** - I2C Master/Slave
- **SPI** - SPI Master
- **ADC** - Analog-to-Digital Converter
- **PWM** - Pulse Width Modulation
- **CAN-FD** - CAN with Flexible Data-Rate
- **Flash** - Flash Memory
- **GPIO** - General Purpose I/O

Example usage:

```python
from mocks.peripheral_mocks import I2CMock, ADCMock

# I2C Mock
i2c = I2CMock(bus_id=0)
i2c.init(speed=400000)
i2c.add_device(0x50)
i2c.write(0x50, 0x00, b"\x12\x34")
data = i2c.read(0x50, 0x00, 2)

# ADC Mock
adc = ADCMock(channel=0, resolution=12)
adc.init()
adc.set_voltage(1.65, vref=3.3)  # Set to half of Vref
value = adc.read()  # Returns ~2048 (12-bit mid-range)
```

## Troubleshooting

### Import Errors

If you get import errors, ensure the tests directory is in Python path:
```bash
export PYTHONPATH="${PYTHONPATH}:$(pwd)/tests"
```

### Pytest Not Found

Install pytest:
```bash
pip install -r tests/requirements.txt
```

### Permission Denied on run_tests.sh

Make it executable:
```bash
chmod +x tests/run_tests.sh
```

## Best Practices

1. **Use markers** - Tag tests appropriately for easy filtering
2. **Use fixtures** - Reuse common setup code
3. **Mock hardware** - Don't depend on actual hardware for most tests
4. **Test one thing** - Each test should verify one specific behavior
5. **Descriptive names** - Use clear, descriptive test function names
6. **Document tests** - Add docstrings explaining what each test verifies
7. **Fast tests** - Keep tests fast; mark slow tests with `@pytest.mark.slow`
8. **Independent tests** - Tests should not depend on each other

## Integration with Project Build

You can add test targets to your main project Makefile:

```makefile
.PHONY: test test-all

test:
	$(MAKE) -C tests test

test-all:
	$(MAKE) -C tests test-all

coverage:
	$(MAKE) -C tests coverage
```

## Further Reading

- [Pytest Documentation](https://docs.pytest.org/)
- [Python unittest.mock](https://docs.python.org/3/library/unittest.mock.html)
- [PSoC4 Documentation](https://www.infineon.com/cms/en/product/microcontroller/32-bit-psoc-arm-cortex-microcontroller/psoc-4-32-bit-arm-cortex-m0-mcu/)

## Support

For issues or questions:
1. Check this README
2. Review existing tests for examples
3. Check pytest documentation
4. Create an issue in your project repository

---

**Happy Testing!** 🧪
