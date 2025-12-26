*** Settings ***
Documentation    Common resource file with shared keywords and variables
Library          OperatingSystem
Library          String
Library          Collections
Library          DateTime


*** Variables ***
# Test configuration
${TEST_TIMEOUT}         60s
${LOG_LEVEL}           INFO

# Device configuration
${DUT_NAME}            PSoC4 Development Board
${TARGET}              CY8CKIT-041S-MAX
${FIRMWARE_VERSION}    1.0.0

# Timing constants
${SHORT_DELAY}         0.1
${MEDIUM_DELAY}        0.5
${LONG_DELAY}          1.0

# Test thresholds
${ADC_TOLERANCE}       50
${VOLTAGE_TOLERANCE}   0.1
${TIMING_TOLERANCE}    0.05


*** Keywords ***
Test Setup
    [Documentation]    Common setup for all tests
    Log    Starting test: ${TEST NAME}    level=INFO
    Log    Test suite: ${SUITE NAME}    level=INFO
    Set Log Level    ${LOG_LEVEL}

Test Teardown
    [Documentation]    Common teardown for all tests
    Log    Test completed: ${TEST NAME}    level=INFO
    Run Keyword If Test Failed    Log Test Failure Details

Suite Setup
    [Documentation]    Common setup for test suite
    Log    ============================================    level=INFO
    Log    Starting Test Suite: ${SUITE NAME}    level=INFO
    Log    ============================================    level=INFO
    Set Suite Variable    ${SUITE_START_TIME}    ${EMPTY}
    ${start}=    Get Current Date
    Set Suite Variable    ${SUITE_START_TIME}    ${start}

Suite Teardown
    [Documentation]    Common teardown for test suite
    ${end}=    Get Current Date
    ${duration}=    Subtract Date From Date    ${end}    ${SUITE_START_TIME}
    Log    ============================================    level=INFO
    Log    Test Suite Duration: ${duration} seconds    level=INFO
    Log    Test Suite Completed: ${SUITE NAME}    level=INFO
    Log    ============================================    level=INFO

Log Test Failure Details
    [Documentation]    Log detailed information when test fails
    Log    TEST FAILED!    level=ERROR
    ${test_msg}=    Set Variable    ${TEST MESSAGE}
    Log    Error Message: ${test_msg}    level=ERROR

Wait For Condition
    [Documentation]    Wait for a condition to become true
    [Arguments]    ${keyword}    ${timeout}=10s    ${interval}=0.5s
    Wait Until Keyword Succeeds    ${timeout}    ${interval}    ${keyword}

Verify Value In Range
    [Documentation]    Verify that a value is within expected range
    [Arguments]    ${actual}    ${min}    ${max}    ${message}=Value out of range
    ${in_range}=    Evaluate    ${min} <= ${actual} <= ${max}
    Should Be True    ${in_range}    ${message}: ${actual} not in [${min}, ${max}]

Convert To Hex String
    [Documentation]    Convert bytes to hex string
    [Arguments]    ${data}
    ${hex}=    Evaluate    ${data}.hex()
    RETURN    ${hex}

Repeat Keyword N Times
    [Documentation]    Repeat a keyword N times
    [Arguments]    ${keyword}    ${count}    @{args}
    FOR    ${i}    IN RANGE    ${count}
        Run Keyword    ${keyword}    @{args}
    END

Should Be Approximately Equal
    [Documentation]    Verify values are approximately equal within tolerance
    [Arguments]    ${actual}    ${expected}    ${tolerance}
    ${diff}=    Evaluate    abs(${actual} - ${expected})
    Should Be True    ${diff} <= ${tolerance}
    ...    Values differ by ${diff}, tolerance is ${tolerance}

Log Dictionary
    [Documentation]    Log all items in a dictionary
    [Arguments]    ${dict}    ${level}=INFO
    FOR    ${key}    IN    @{dict.keys()}
        Log    ${key}: ${dict}[${key}]    level=${level}
    END

Generate Test Data
    [Documentation]    Generate test data pattern
    [Arguments]    ${length}    ${pattern}=increment
    ${data}=    Create List
    FOR    ${i}    IN RANGE    ${length}
        IF    '${pattern}' == 'increment'
            Append To List    ${data}    ${i}
        ELSE IF    '${pattern}' == 'constant'
            Append To List    ${data}    0xAA
        ELSE IF    '${pattern}' == 'random'
            ${random}=    Evaluate    random.randint(0, 255)    modules=random
            Append To List    ${data}    ${random}
        END
    END
    RETURN    ${data}
