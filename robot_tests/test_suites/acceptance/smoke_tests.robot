*** Settings ***
Documentation    Smoke Test Suite
...              Quick verification tests to ensure basic functionality

Library          ../../libraries/PSoC4Library.py    mode=mock
Library          ../../libraries/PeripheralLibrary.py
Resource         ../../resources/common.robot

Suite Setup      Suite Setup
Suite Teardown   Suite Teardown
Test Setup       Test Setup
Test Teardown    Test Teardown

Test Timeout     30s

Default Tags     smoke    acceptance


*** Test Cases ***
SMOKE-001 System Boots Successfully
    [Documentation]    Verify system can boot
    [Tags]    critical
    Initialize PSoC4 System
    Log    System boot successful

SMOKE-002 System Runs Single Cycle
    [Documentation]    Verify system can execute one main loop cycle
    [Tags]    critical
    Initialize PSoC4 System
    Run Single Cycle
    Log    Single cycle execution successful

SMOKE-003 I2C Communication Works
    [Documentation]    Verify basic I2C communication
    [Tags]    critical
    Initialize I2C    0    400000
    Add I2C Device    0    0x50
    I2C Write    0    0x50    0x00    \\xFF
    ${data}=    I2C Read    0    0x50    0x00    1
    Should Be Equal    ${data}    b'\\xff'

SMOKE-004 ADC Reading Works
    [Documentation]    Verify basic ADC reading
    [Tags]    critical
    Initialize ADC    0    12
    ${value}=    Read ADC    0
    Should Be True    ${value} >= 0
    Should Be True    ${value} <= 4095

SMOKE-005 PWM Output Works
    [Documentation]    Verify basic PWM functionality
    [Tags]    critical
    Initialize PWM    0    1000    50
    Start PWM    0
    Stop PWM    0
    Log    PWM basic operation successful

SMOKE-006 GPIO Control Works
    [Documentation]    Verify basic GPIO control
    [Tags]    critical
    Initialize GPIO    0
    Configure GPIO Pin    0    5    output
    Write GPIO    0    5    True
    ${state}=    Read GPIO    0    5
    Should Be True    ${state}

SMOKE-007 System Can Run Multiple Cycles
    [Documentation]    Verify system can run multiple cycles
    [Tags]    critical
    Initialize PSoC4 System
    Run Main Loop    10
    Verify System Ready

SMOKE-008 System State Can Be Retrieved
    [Documentation]    Verify system state can be queried
    Initialize PSoC4 System
    ${state}=    Get System State
    Should Be Equal    ${state}[initialized]    ${True}
    Should Be Equal    ${state}[mode]    mock

SMOKE-009 All Peripherals Can Initialize
    [Documentation]    Verify all peripherals can be initialized
    Initialize I2C    0    400000
    Initialize ADC    0    12
    Initialize PWM    0    1000    50
    Initialize GPIO    0
    Log    All peripherals initialized successfully

SMOKE-010 Basic End-to-End Test
    [Documentation]    Complete end-to-end smoke test
    [Tags]    critical    end-to-end
    # Initialize system
    Initialize PSoC4 System

    # Initialize peripherals
    Initialize I2C    0    400000
    Add I2C Device    0    0x50
    Initialize ADC    0    12

    # Run operations
    Run Main Loop    5
    I2C Write    0    0x50    0x00    \\xAA
    ${adc_val}=    Read ADC    0

    # Verify
    Verify System Ready
    Log    End-to-end smoke test passed
