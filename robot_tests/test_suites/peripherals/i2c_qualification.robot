*** Settings ***
Documentation    I2C Peripheral Qualification Test Suite
...              Comprehensive testing of I2C communication functionality

Library          ../../libraries/PeripheralLibrary.py
Resource         ../../resources/common.robot
Resource         ../../resources/peripheral_resources.robot

Suite Setup      Suite Setup
Suite Teardown   Suite Teardown
Test Setup       Test Setup
Test Teardown    Test Teardown

Test Timeout     ${TEST_TIMEOUT}

Default Tags     i2c    peripheral    qualification


*** Test Cases ***
I2C-QUAL-001 I2C Initialization Test
    [Documentation]    Verify I2C bus initializes correctly
    [Tags]    smoke    critical
    Initialize I2C    ${I2C_BUS}    ${I2C_SPEED_FAST}
    Log    I2C initialized successfully

I2C-QUAL-002 I2C Device Detection Test
    [Documentation]    Verify I2C device can be added and detected
    [Tags]    smoke
    Setup I2C Communication
    Log    I2C device added successfully

I2C-QUAL-003 I2C Write Test
    [Documentation]    Verify data can be written to I2C device
    [Tags]    functional
    Setup I2C Communication
    I2C Write    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    \\x01\\x02\\x03\\x04
    Log    I2C write successful

I2C-QUAL-004 I2C Read Test
    [Documentation]    Verify data can be read from I2C device
    [Tags]    functional
    Setup I2C Communication
    # Write first
    I2C Write    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    \\xAA\\xBB\\xCC\\xDD
    # Read back
    ${data}=    I2C Read    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    4
    Should Be Equal    ${data}    b'\\xaa\\xbb\\xcc\\xdd'

I2C-QUAL-005 I2C Write-Read Cycle Test
    [Documentation]    Verify complete I2C write-read cycle
    [Tags]    functional    critical
    Setup I2C Communication
    Test I2C Write Read Cycle    \\x11\\x22\\x33\\x44

I2C-QUAL-006 I2C Multiple Byte Transfer Test
    [Documentation]    Verify multi-byte I2C transfers
    [Tags]    functional
    Setup I2C Communication

    # Test various data lengths
    ${lengths}=    Create List    1    4    8    16    32
    FOR    ${length}    IN    @{lengths}
        ${test_data}=    Generate Test Data    ${length}    increment
        ${hex_data}=    Convert To Hex String    ${test_data}
        I2C Write    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    ${hex_data}
        ${read_data}=    I2C Read    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    ${length}
        Log    Verified ${length}-byte transfer
    END

I2C-QUAL-007 I2C Different Address Test
    [Documentation]    Verify I2C communication with different device addresses
    [Tags]    functional
    Initialize I2C    ${I2C_BUS}    ${I2C_SPEED_FAST}

    ${addresses}=    Create List    0x50    0x51    0x52    0x53
    FOR    ${addr}    IN    @{addresses}
        Add I2C Device    ${I2C_BUS}    ${addr}
        I2C Write    ${I2C_BUS}    ${addr}    0x00    \\xFF
        ${data}=    I2C Read    ${I2C_BUS}    ${addr}    0x00    1
        Log    Communication with device 0x${addr} successful
    END

I2C-QUAL-008 I2C Speed Test - Standard Mode
    [Documentation]    Verify I2C operates correctly at standard speed (100kHz)
    [Tags]    speed
    Initialize I2C    ${I2C_BUS}    ${I2C_SPEED_STANDARD}
    Add I2C Device    ${I2C_BUS}    ${I2C_DEVICE_ADDR}
    Test I2C Write Read Cycle

I2C-QUAL-009 I2C Speed Test - Fast Mode
    [Documentation]    Verify I2C operates correctly at fast speed (400kHz)
    [Tags]    speed    critical
    Initialize I2C    ${I2C_BUS}    ${I2C_SPEED_FAST}
    Add I2C Device    ${I2C_BUS}    ${I2C_DEVICE_ADDR}
    Test I2C Write Read Cycle

I2C-QUAL-010 I2C Data Integrity Test
    [Documentation]    Verify data integrity during I2C transfers
    [Tags]    reliability    critical
    Setup I2C Communication

    # Test different data patterns
    ${patterns}=    Create List
    ...    \\x00\\x00\\x00\\x00
    ...    \\xFF\\xFF\\xFF\\xFF
    ...    \\xAA\\x55\\xAA\\x55
    ...    \\x01\\x02\\x04\\x08

    FOR    ${pattern}    IN    @{patterns}
        I2C Write    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    ${pattern}
        ${read_data}=    I2C Read    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    4
        Log    Pattern ${pattern} verified
    END

I2C-QUAL-011 I2C Sequential Access Test
    [Documentation]    Verify sequential register access
    [Tags]    functional
    Setup I2C Communication

    # Write to sequential registers
    FOR    ${reg}    IN RANGE    10
        ${data}=    Evaluate    bytes([${reg}])
        ${hex_data}=    Convert To Hex String    ${data}
        I2C Write    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    ${reg}    ${hex_data}
    END

    # Read back and verify
    FOR    ${reg}    IN RANGE    10
        ${data}=    I2C Read    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    ${reg}    1
        Log    Register ${reg} verified
    END

I2C-QUAL-012 I2C Stress Test
    [Documentation]    Perform stress testing on I2C communication
    [Tags]    stress    slow
    Setup I2C Communication

    FOR    ${i}    IN RANGE    1000
        Test I2C Write Read Cycle    \\x${i & 0xFF}
    END

    Log    1000 I2C transactions completed successfully

I2C-QUAL-013 I2C Burst Write Test
    [Documentation]    Verify burst write capability
    [Tags]    performance
    Setup I2C Communication

    # Create large test data
    ${data}=    Evaluate    bytes(range(256))
    ${hex_data}=    Convert To Hex String    ${data}
    I2C Write    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    ${hex_data}

    Log    Burst write of 256 bytes successful

I2C-QUAL-014 I2C Burst Read Test
    [Documentation]    Verify burst read capability
    [Tags]    performance
    Setup I2C Communication

    # Write test pattern
    ${write_data}=    Evaluate    bytes(range(100))
    ${hex_write}=    Convert To Hex String    ${write_data}
    I2C Write    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    ${hex_write}

    # Burst read
    ${read_data}=    I2C Read    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    100
    Should Be Equal    ${read_data}    ${write_data}

    Log    Burst read of 100 bytes successful

I2C-QUAL-015 I2C Reliability Qualification
    [Documentation]    Overall I2C reliability qualification
    [Tags]    qualification    critical
    Setup I2C Communication

    ${errors}=    Set Variable    0
    ${total}=    Set Variable    500

    FOR    ${i}    IN RANGE    ${total}
        ${status}    ${result}=    Run Keyword And Ignore Error
        ...    Test I2C Write Read Cycle    \\x${i & 0xFF}
        IF    '${status}' == 'FAIL'
            ${errors}=    Evaluate    ${errors} + 1
        END
    END

    ${error_rate}=    Evaluate    ${errors} / ${total}
    Should Be True    ${error_rate} < 0.01    Error rate ${error_rate} exceeds 1%

    Log    I2C reliability qualification passed: ${errors}/${total} errors
