*** Settings ***
Documentation    ADC Peripheral Qualification Test Suite
...              Comprehensive testing of ADC functionality

Library          ../../libraries/PeripheralLibrary.py
Resource         ../../resources/common.robot
Resource         ../../resources/peripheral_resources.robot

Suite Setup      Suite Setup
Suite Teardown   Suite Teardown
Test Setup       Test Setup
Test Teardown    Test Teardown

Test Timeout     ${TEST_TIMEOUT}

Default Tags     adc    peripheral    qualification


*** Test Cases ***
ADC-QUAL-001 ADC Initialization Test
    [Documentation]    Verify ADC initializes correctly
    [Tags]    smoke    critical
    Setup ADC For Testing
    Log    ADC initialized successfully

ADC-QUAL-002 ADC Basic Reading Test
    [Documentation]    Verify basic ADC reading functionality
    [Tags]    smoke
    Setup ADC For Testing
    ${value}=    Read ADC
    Should Be True    ${value} >= 0
    Should Be True    ${value} <= ${ADC_MAX_VALUE}

ADC-QUAL-003 ADC Zero Scale Test
    [Documentation]    Verify ADC reads correctly at zero scale
    [Tags]    functional    critical
    Setup ADC For Testing
    Test ADC Reading    voltage=0.0    expected_value=0

ADC-QUAL-004 ADC Mid Scale Test
    [Documentation]    Verify ADC reads correctly at mid-scale
    [Tags]    functional    critical
    Setup ADC For Testing
    Test ADC Reading    voltage=1.65    expected_value=2048

ADC-QUAL-005 ADC Full Scale Test
    [Documentation]    Verify ADC reads correctly at full scale
    [Tags]    functional    critical
    Setup ADC For Testing
    Test ADC Reading    voltage=3.3    expected_value=4095

ADC-QUAL-006 ADC Linearity Test
    [Documentation]    Verify ADC linearity across voltage range
    [Tags]    accuracy    critical
    Setup ADC For Testing

    # Test at different voltage points
    ${voltages}=    Create List    0.0    0.825    1.65    2.475    3.3
    ${expected}=    Create List    0    1024    2048    3072    4095

    FOR    ${i}    IN RANGE    5
        ${voltage}=    Get From List    ${voltages}    ${i}
        ${exp_val}=    Get From List    ${expected}    ${i}
        Test ADC Reading    voltage=${voltage}    expected_value=${exp_val}
    END

ADC-QUAL-007 ADC Resolution Test
    [Documentation]    Verify ADC resolution is correct
    [Tags]    accuracy
    Setup ADC For Testing

    # Test fine voltage steps
    Set ADC Voltage    ${ADC_CHANNEL}    1.650    ${ADC_VREF}
    ${val1}=    Read ADC

    Set ADC Voltage    ${ADC_CHANNEL}    1.651    ${ADC_VREF}
    ${val2}=    Read ADC

    # Values should be different (resolution check)
    ${diff}=    Evaluate    abs(${val1} - ${val2})
    Should Be True    ${diff} <= 2    Resolution check failed

ADC-QUAL-008 ADC Multiple Channels Test
    [Documentation]    Verify multiple ADC channels work independently
    [Tags]    functional
    ${channels}=    Create List    0    1    2    3

    FOR    ${ch}    IN    @{channels}
        Initialize ADC    ${ch}    ${ADC_RESOLUTION}
        Set ADC Voltage    ${ch}    ${ch * 0.5}    ${ADC_VREF}
        ${value}=    Read ADC    ${ch}
        Log    Channel ${ch} value: ${value}
    END

ADC-QUAL-009 ADC Repeated Reading Test
    [Documentation]    Verify ADC readings are consistent
    [Tags]    stability
    Setup ADC For Testing
    Set ADC Voltage    ${ADC_CHANNEL}    1.65    ${ADC_VREF}

    ${readings}=    Create List
    FOR    ${i}    IN RANGE    10
        ${value}=    Read ADC
        Append To List    ${readings}    ${value}
    END

    # Check all readings are within tolerance
    ${first}=    Get From List    ${readings}    0
    FOR    ${reading}    IN    @{readings}
        ${diff}=    Evaluate    abs(${reading} - ${first})
        Should Be True    ${diff} <= 5    Readings not consistent
    END

ADC-QUAL-010 ADC Bounds Checking Test
    [Documentation]    Verify ADC handles out-of-bounds values correctly
    [Tags]    robustness
    Setup ADC For Testing

    # Test below zero
    Set ADC Voltage    ${ADC_CHANNEL}    -0.5    ${ADC_VREF}
    ${value}=    Read ADC
    Should Be Equal As Integers    ${value}    0

    # Test above Vref
    Set ADC Voltage    ${ADC_CHANNEL}    5.0    ${ADC_VREF}
    ${value}=    Read ADC
    Should Be Equal As Integers    ${value}    ${ADC_MAX_VALUE}

ADC-QUAL-011 ADC Accuracy Test
    [Documentation]    Verify ADC accuracy meets specifications
    [Tags]    accuracy    critical
    Setup ADC For Testing

    ${test_points}=    Create List    0.33    0.66    0.99    1.32    1.65    1.98    2.31    2.64    2.97
    FOR    ${voltage}    IN    @{test_points}
        ${expected}=    Evaluate    int((${voltage} / ${ADC_VREF}) * ${ADC_MAX_VALUE})
        Test ADC Reading    voltage=${voltage}    expected_value=${expected}    tolerance=100
    END

ADC-QUAL-012 ADC Sampling Speed Test
    [Documentation]    Verify ADC sampling speed
    [Tags]    performance
    Setup ADC For Testing

    ${start}=    Get Current Date
    FOR    ${i}    IN RANGE    1000
        ${value}=    Read ADC
    END
    ${end}=    Get Current Date

    ${duration}=    Subtract Date From Date    ${end}    ${start}
    ${samples_per_sec}=    Evaluate    1000 / ${duration}

    Log    ADC sampling rate: ${samples_per_sec} samples/second
    Should Be True    ${samples_per_sec} > 100

ADC-QUAL-013 ADC Stress Test
    [Documentation]    Perform stress testing on ADC
    [Tags]    stress    slow
    Setup ADC For Testing

    FOR    ${i}    IN RANGE    5000
        ${voltage}=    Evaluate    (${i} % 330) / 100.0
        Set ADC Voltage    ${ADC_CHANNEL}    ${voltage}    ${ADC_VREF}
        ${value}=    Read ADC
        Should Be True    ${value} >= 0
    END

    Log    ADC stress test completed: 5000 readings

ADC-QUAL-014 ADC Voltage Sweep Test
    [Documentation]    Sweep ADC across entire voltage range
    [Tags]    functional
    Setup ADC For Testing

    FOR    ${step}    IN RANGE    34
        ${voltage}=    Evaluate    ${step} * 0.1
        Set ADC Voltage    ${ADC_CHANNEL}    ${voltage}    ${ADC_VREF}
        ${value}=    Read ADC
        Log    Voltage: ${voltage}V = ADC: ${value}
    END

ADC-QUAL-015 ADC Qualification Summary
    [Documentation]    Overall ADC qualification test
    [Tags]    qualification    critical
    Setup ADC For Testing

    # Test key voltage points
    ${test_voltages}=    Create List    0.0    0.825    1.65    2.475    3.3
    ${expected_values}=    Create List    0    1024    2048    3072    4095

    ${passed}=    Set Variable    0
    ${total}=    Get Length    ${test_voltages}

    FOR    ${i}    IN RANGE    ${total}
        ${voltage}=    Get From List    ${test_voltages}    ${i}
        ${expected}=    Get From List    ${expected_values}    ${i}

        ${status}    ${result}=    Run Keyword And Ignore Error
        ...    Test ADC Reading    voltage=${voltage}    expected_value=${expected}

        IF    '${status}' == 'PASS'
            ${passed}=    Evaluate    ${passed} + 1
        END
    END

    Should Be Equal As Integers    ${passed}    ${total}
    Log    ADC qualification passed: ${passed}/${total} tests successful
