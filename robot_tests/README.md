# PSoC4 Robot Framework Qualification Tests

Comprehensive qualification test suite for PSoC4 embedded applications using Robot Framework.

## Overview

This test framework provides:
- **Keyword-driven testing** for embedded systems
- **Qualification test suites** for system and peripheral validation
- **Acceptance testing** for end-to-end verification
- **Detailed HTML reports** with pass/fail statistics
- **Integration with pytest mocks** for hardware simulation

## Directory Structure

```
robot_tests/
├── README.md                    # This file
├── QUICKSTART.md               # Quick start guide
├── requirements.txt            # Python dependencies
├── robot.yaml                  # Configuration file
├── run_tests.sh                # Test runner script
├── libraries/                  # Custom keyword libraries
│   ├── PSoC4Library.py        # System-level keywords
│   └── PeripheralLibrary.py   # Peripheral keywords
├── resources/                  # Reusable resources
│   ├── common.robot           # Common keywords
│   ├── system_resources.robot # System resources
│   └── peripheral_resources.robot
├── test_suites/               # Test suites
│   ├── system/                # System qualification
│   │   └── system_qualification.robot
│   ├── peripherals/           # Peripheral qualification
│   │   ├── i2c_qualification.robot
│   │   └── adc_qualification.robot
│   ├── acceptance/            # Acceptance tests
│   │   └── smoke_tests.robot
│   └── regression/            # Regression tests
├── test_data/                 # Test data and variables
│   └── test_variables.yaml
├── results/                   # Test results (generated)
└── logs/                      # Test logs (generated)
```

## Installation

### Prerequisites

- Python 3.8 or higher
- pip (Python package manager)

### Install Dependencies

```bash
cd robot_tests
pip install -r requirements.txt
```

This installs:
- Robot Framework 6.1.1
- Custom libraries (PSoC4Library, PeripheralLibrary)
- Reporting tools
- Integration libraries

## Quick Start

### Run All Tests

```bash
./run_tests.sh all
```

### Run Smoke Tests

```bash
./run_tests.sh smoke
```

### Run Specific Test Suite

```bash
# System qualification
./run_tests.sh system

# Peripheral tests
./run_tests.sh peripherals

# I2C tests only
./run_tests.sh i2c

# ADC tests only
./run_tests.sh adc
```

### View Results

After running tests, open:
- **HTML Report**: `results/report.html`
- **Test Log**: `results/log.html`
- **XML Output**: `results/output.xml`

## Test Categories

### 1. Smoke Tests (`test_suites/acceptance/smoke_tests.robot`)

Quick verification tests (< 1 minute):
- System boot test
- Basic peripheral initialization
- Simple communication tests

**Run:** `./run_tests.sh smoke`

### 2. System Qualification (`test_suites/system/system_qualification.robot`)

Comprehensive system testing:
- Initialization sequence
- Main loop execution
- Burn-in testing (1000+ cycles)
- Power cycle testing
- Stress testing
- Performance measurement

**Run:** `./run_tests.sh system`

**Test Cases:**
- SYS-QUAL-001: System Initialization
- SYS-QUAL-005: Burn-In Test
- SYS-QUAL-008: Power Cycle Test
- SYS-QUAL-010: Stress Test
- ...and more

### 3. I2C Qualification (`test_suites/peripherals/i2c_qualification.robot`)

I2C communication testing:
- Initialization and configuration
- Write/read operations
- Multi-byte transfers
- Different device addresses
- Speed testing (100kHz, 400kHz)
- Data integrity verification
- Stress testing

**Run:** `./run_tests.sh i2c`

**Test Cases:**
- I2C-QUAL-001: Initialization
- I2C-QUAL-005: Write-Read Cycle
- I2C-QUAL-010: Data Integrity
- I2C-QUAL-015: Reliability Qualification
- ...and more

### 4. ADC Qualification (`test_suites/peripherals/adc_qualification.robot`)

ADC functionality testing:
- Zero/mid/full scale testing
- Linearity verification
- Resolution testing
- Multi-channel testing
- Accuracy validation
- Sampling speed measurement

**Run:** `./run_tests.sh adc`

**Test Cases:**
- ADC-QUAL-003: Zero Scale Test
- ADC-QUAL-004: Mid Scale Test
- ADC-QUAL-006: Linearity Test
- ADC-QUAL-015: Qualification Summary
- ...and more

## Running Tests

### Command Line Options

```bash
# Run all tests
robot test_suites/

# Run specific suite
robot test_suites/system/system_qualification.robot

# Run tests with specific tag
robot --include smoke test_suites/

# Run critical tests only
robot --include critical test_suites/

# Generate custom report name
robot --outputdir results --report my_report.html test_suites/

# Set log level
robot --loglevel DEBUG test_suites/

# Run tests in parallel (requires pabot)
pabot --processes 4 test_suites/
```

### Using Test Runner Script

The `run_tests.sh` script provides convenient shortcuts:

```bash
./run_tests.sh all           # All tests
./run_tests.sh smoke         # Smoke tests
./run_tests.sh system        # System tests
./run_tests.sh peripherals   # All peripheral tests
./run_tests.sh i2c           # I2C tests
./run_tests.sh adc           # ADC tests
./run_tests.sh acceptance    # Acceptance tests
./run_tests.sh qualification # Qualification tests
./run_tests.sh critical      # Critical tests only
./run_tests.sh parallel      # Parallel execution
```

## Test Tags

Tests are organized using tags for easy filtering:

- `smoke` - Quick smoke tests
- `critical` - Critical functionality tests
- `functional` - Functional tests
- `performance` - Performance tests
- `stability` - Stability tests
- `stress` - Stress tests
- `qualification` - Qualification tests
- `slow` - Long-running tests
- `system` - System-level tests
- `peripheral` - Peripheral tests
- `i2c`, `adc`, `pwm`, etc. - Peripheral-specific

**Examples:**

```bash
# Run only critical tests
robot --include critical test_suites/

# Run all except slow tests
robot --exclude slow test_suites/

# Run smoke AND critical
robot --include smokeANDcritical test_suites/

# Run smoke OR critical
robot --include smokeORcritical test_suites/
```

## Custom Keywords

### System Keywords (PSoC4Library)

```robot
Initialize PSoC4 System
Verify System Ready
Run Main Loop    ${cycles}
Run Single Cycle
Reset System
Get System State
Sleep For    ${seconds}
System Should Be Initialized
```

### Peripheral Keywords (PeripheralLibrary)

**I2C:**
```robot
Initialize I2C    ${bus_id}    ${speed}
Add I2C Device    ${bus_id}    ${address}    ${size}
I2C Write    ${bus_id}    ${addr}    ${reg}    ${data}
I2C Read     ${bus_id}    ${addr}    ${reg}    ${length}
```

**ADC:**
```robot
Initialize ADC    ${channel}    ${resolution}
Read ADC    ${channel}
Set ADC Value    ${channel}    ${value}
Set ADC Voltage    ${channel}    ${voltage}    ${vref}
```

**PWM:**
```robot
Initialize PWM    ${channel}    ${freq}    ${duty}
Start PWM    ${channel}
Stop PWM    ${channel}
Set PWM Duty Cycle    ${channel}    ${duty}
```

**GPIO:**
```robot
Initialize GPIO    ${instance}
Configure GPIO Pin    ${instance}    ${pin}    ${direction}
Write GPIO    ${instance}    ${pin}    ${value}
Read GPIO    ${instance}    ${pin}
```

## Writing Custom Tests

### Basic Test Structure

```robot
*** Settings ***
Documentation    My Custom Test Suite
Library          ../libraries/PSoC4Library.py
Resource         ../resources/common.robot

Test Setup       Test Setup
Test Teardown    Test Teardown

*** Test Cases ***
My First Test
    [Documentation]    Test description
    [Tags]    custom
    Initialize PSoC4 System
    Run Main Loop    10
    Verify System Ready
```

### Using Resources

```robot
*** Settings ***
Resource    resources/system_resources.robot

*** Test Cases ***
Test With Resource
    Initialize And Verify System
    Run System Qualification    cycles=100
```

### Test with Variables

```robot
*** Variables ***
${MY_DEVICE}    0x50
${TEST_DATA}    \\x01\\x02\\x03

*** Test Cases ***
Test With Variables
    Setup I2C Communication
    I2C Write    0    ${MY_DEVICE}    0x00    ${TEST_DATA}
```

## Reporting

### HTML Reports

Robot Framework generates comprehensive HTML reports:

1. **report.html** - High-level test statistics
   - Pass/fail counts
   - Test duration
   - Tag statistics
   - Top 10 slowest tests

2. **log.html** - Detailed test log
   - Step-by-step execution
   - Keyword arguments
   - Screenshots (if enabled)
   - Error messages and stack traces

3. **output.xml** - Machine-readable results
   - For CI/CD integration
   - Custom report generation
   - Metrics calculation

### Metrics

Generate test metrics:

```bash
pip install robotframework-metrics
robotmetrics --inputpath results/output.xml
```

## Integration with CI/CD

### GitHub Actions Example

```yaml
name: Robot Framework Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - uses: actions/setup-python@v4
        with:
          python-version: '3.10'
      - name: Install dependencies
        run: |
          cd robot_tests
          pip install -r requirements.txt
      - name: Run smoke tests
        run: |
          cd robot_tests
          ./run_tests.sh smoke
      - name: Upload results
        if: always()
        uses: actions/upload-artifact@v3
        with:
          name: robot-results
          path: robot_tests/results/
```

### Jenkins Example

```groovy
stage('Robot Tests') {
    steps {
        sh 'cd robot_tests && ./run_tests.sh all'
    }
    post {
        always {
            robot outputPath: 'robot_tests/results'
        }
    }
}
```

## Configuration

Edit `robot.yaml` to customize:

```yaml
dut:
  serial_port: "/dev/ttyUSB0"
  baudrate: 115200

peripherals:
  i2c:
    speed: 400000

  adc:
    vref: 3.3

thresholds:
  adc_tolerance: 50
  voltage_tolerance: 0.1
```

## Troubleshooting

### Import Errors

If you get "ModuleNotFoundError":

```bash
# Check Python path
export PYTHONPATH="${PYTHONPATH}:$(pwd)"

# Or install in development mode
cd tests
pip install -e .
```

### Robot Framework Not Found

```bash
pip install robotframework
```

### Tests Timeout

Increase timeout in test file:

```robot
*** Settings ***
Test Timeout    120s    # 2 minutes
```

### Parallel Execution Issues

```bash
# Install pabot
pip install robotframework-pabot

# Reduce process count
pabot --processes 2 test_suites/
```

## Best Practices

1. **Use descriptive test names** - Prefix with ID (e.g., SYS-QUAL-001)
2. **Tag appropriately** - Use tags for filtering
3. **Add documentation** - Document test purpose and expected results
4. **Use resource files** - Reuse common keywords
5. **Keep tests independent** - Each test should run standalone
6. **Use setup/teardown** - Initialize and clean up properly
7. **Log important values** - Help with debugging
8. **Set appropriate timeouts** - Prevent hanging tests

## Performance Tips

- Run smoke tests first (fast feedback)
- Use `--exclude slow` during development
- Run qualification tests nightly
- Use parallel execution for large suites
- Profile slow tests and optimize

## Support and Documentation

- [Robot Framework User Guide](https://robotframework.org/robotframework/latest/RobotFrameworkUserGuide.html)
- [Robot Framework Standard Libraries](https://robotframework.org/robotframework/#standard-libraries)
- [Creating Custom Libraries](https://robotframework.org/robotframework/latest/RobotFrameworkUserGuide.html#creating-test-libraries)

## Examples

### Complete Test Example

```robot
*** Settings ***
Documentation    Complete example test
Library          libraries/PSoC4Library.py
Library          libraries/PeripheralLibrary.py
Resource         resources/common.robot

Test Setup       Initialize Test Environment
Test Teardown    Cleanup Test Environment

*** Variables ***
${I2C_DEVICE}    0x50

*** Test Cases ***
End-To-End Communication Test
    [Documentation]    Test complete communication flow
    [Tags]    integration    critical

    # Initialize system
    Initialize PSoC4 System
    Verify System Ready

    # Setup I2C
    Initialize I2C    0    400000
    Add I2C Device    0    ${I2C_DEVICE}

    # Test communication
    I2C Write    0    ${I2C_DEVICE}    0x00    \\x01\\x02\\x03\\x04
    ${data}=    I2C Read    0    ${I2C_DEVICE}    0x00    4

    # Verify
    Should Be Equal    ${data}    b'\\x01\\x02\\x03\\x04'

    # Run system
    Run Main Loop    10

    Log    Test completed successfully

*** Keywords ***
Initialize Test Environment
    Log    Setting up test environment
    Set Log Level    INFO

Cleanup Test Environment
    Log    Cleaning up test environment
    Reset System
```

---

**Happy Testing!** 🤖
