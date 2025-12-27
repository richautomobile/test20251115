# Quick Start - Robot Framework Qualification Tests

Get started with Robot Framework testing in 5 minutes!

## Step 1: Install (1 minute)

```bash
cd robot_tests
pip install -r requirements.txt
```

## Step 2: Run Your First Test (1 minute)

```bash
# Run smoke tests (fastest)
./run_tests.sh smoke
```

Expected output:
```
================================================
PSoC4 Robot Framework Test Runner
================================================

Test Suite: smoke
------------------------------------------------

Running Smoke Tests...
==============================================================================
Smoke Tests
==============================================================================
SMOKE-001 System Boots Successfully                                 | PASS |
SMOKE-002 System Runs Single Cycle                                  | PASS |
SMOKE-003 I2C Communication Works                                   | PASS |
...
==============================================================================
Smoke Tests                                                         | PASS |
10 tests, 10 passed, 0 failed
==============================================================================

✓ Tests completed successfully!
```

## Step 3: View the Report (1 minute)

```bash
# Open the HTML report in your browser
open results/report.html
# or on Linux: xdg-open results/report.html
```

The report shows:
- ✅ Pass/Fail statistics
- ⏱️ Execution times
- 📊 Tag statistics
- 📝 Detailed logs

## Step 4: Try Different Test Suites (1 minute)

```bash
# System qualification tests
./run_tests.sh system

# I2C peripheral tests
./run_tests.sh i2c

# ADC peripheral tests
./run_tests.sh adc

# All tests
./run_tests.sh all
```

## Step 5: Write Your First Test (1 minute)

Create `test_suites/my_test.robot`:

```robot
*** Settings ***
Documentation    My First Robot Test
Library          libraries/PSoC4Library.py    mode=mock
Resource         resources/common.robot

*** Test Cases ***
My First Test Case
    [Documentation]    Test system initialization
    [Tags]    custom
    Initialize PSoC4 System
    Verify System Ready
    Run Main Loop    10
    Log    My test passed!
```

Run it:
```bash
robot test_suites/my_test.robot
```

## Common Commands

```bash
# Smoke tests (quick)
./run_tests.sh smoke

# System tests
./run_tests.sh system

# Peripheral tests
./run_tests.sh peripherals

# Critical tests only
./run_tests.sh critical

# Parallel execution
./run_tests.sh parallel

# All tests
./run_tests.sh all
```

## Test Structure Overview

```
robot_tests/
├── libraries/          # Custom keywords
│   ├── PSoC4Library.py        # System keywords
│   └── PeripheralLibrary.py   # Peripheral keywords
├── resources/          # Reusable keywords
│   ├── common.robot           # Common utilities
│   ├── system_resources.robot # System helpers
│   └── peripheral_resources.robot
├── test_suites/       # Your tests
│   ├── system/              # System tests
│   ├── peripherals/         # Peripheral tests
│   └── acceptance/          # Acceptance tests
└── results/           # Test reports
```

## Available Keywords

### System Keywords
```robot
Initialize PSoC4 System
Verify System Ready
Run Main Loop    10
Run Single Cycle
Reset System
```

### I2C Keywords
```robot
Initialize I2C    0    400000
Add I2C Device    0    0x50
I2C Write    0    0x50    0x00    \\x01\\x02
I2C Read     0    0x50    0x00    2
```

### ADC Keywords
```robot
Initialize ADC    0    12
Set ADC Voltage    0    1.65    3.3
Read ADC    0
```

### PWM Keywords
```robot
Initialize PWM    0    1000    50
Start PWM    0
Set PWM Duty Cycle    0    75
Stop PWM    0
```

## Example Test

```robot
*** Settings ***
Library    libraries/PeripheralLibrary.py

*** Test Cases ***
I2C Communication Test
    [Tags]    i2c
    # Setup
    Initialize I2C    0    400000
    Add I2C Device    0    0x50

    # Execute
    I2C Write    0    0x50    0x00    \\xAA\\xBB\\xCC
    ${data}=    I2C Read    0    0x50    0x00    3

    # Verify
    Should Be Equal    ${data}    b'\\xaa\\xbb\\xcc'
```

## Understanding Test Results

### Report.html
- Summary statistics
- Test execution timeline
- Tag-based grouping
- Trends over time

### Log.html
- Detailed test logs
- Step-by-step execution
- Keyword arguments
- Error messages and traces

### Output.xml
- Machine-readable format
- For CI/CD integration
- Custom reporting

## Tips

1. **Start with smoke tests** - Get quick feedback
2. **Use tags** - Filter tests easily
3. **Check logs** - Detailed debugging info
4. **Read examples** - Learn from existing tests
5. **Use resources** - Reuse common keywords

## Next Steps

1. ✅ Read the full [README.md](README.md)
2. ✅ Explore test suites in `test_suites/`
3. ✅ Check out resources in `resources/`
4. ✅ Review custom libraries in `libraries/`
5. ✅ Write your own tests

## Troubleshooting

**Problem:** "robot: command not found"
```bash
pip install robotframework
```

**Problem:** Import errors
```bash
export PYTHONPATH="${PYTHONPATH}:$(pwd)/.."
```

**Problem:** Tests fail
```bash
# Check the log file
cat results/log.html
# Or view in browser
open results/log.html
```

## Getting Help

- Full documentation: [README.md](README.md)
- Robot Framework: https://robotframework.org
- User Guide: https://robotframework.org/robotframework/latest/RobotFrameworkUserGuide.html

---

**Ready to test!** 🚀

Quick command reference:
```bash
./run_tests.sh smoke        # Quick (30s)
./run_tests.sh system       # System tests (2-5 min)
./run_tests.sh i2c          # I2C tests (1-2 min)
./run_tests.sh all          # Everything (5-10 min)
```
