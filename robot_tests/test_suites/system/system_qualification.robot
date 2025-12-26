*** Settings ***
Documentation    System Qualification Test Suite
...              This suite performs comprehensive system-level qualification testing
...              including initialization, stability, stress testing, and power cycling.

Library          ../../libraries/PSoC4Library.py    mode=mock
Resource         ../../resources/common.robot
Resource         ../../resources/system_resources.robot

Suite Setup      Suite Setup
Suite Teardown   Suite Teardown
Test Setup       Test Setup
Test Teardown    Test Teardown

Test Timeout     ${TEST_TIMEOUT}

Default Tags     system    qualification


*** Test Cases ***
SYS-QUAL-001 System Initialization Test
    [Documentation]    Verify system initializes correctly
    [Tags]    smoke    critical
    Initialize PSoC4 System
    Verify System Ready
    ${state}=    Get System State
    Log Dictionary    ${state}
    Should Be Equal    ${state}[initialized]    ${True}

SYS-QUAL-002 Single Main Loop Cycle Test
    [Documentation]    Verify single main loop cycle executes successfully
    [Tags]    smoke
    Initialize And Verify System
    Run Single Cycle
    Verify System Ready

SYS-QUAL-003 Multiple Cycle Execution Test
    [Documentation]    Verify multiple main loop cycles execute successfully
    [Tags]    functional
    Initialize And Verify System
    Run Main Loop    100
    Verify System Ready

SYS-QUAL-004 Extended Operation Test
    [Documentation]    Verify system operates correctly for extended period
    [Tags]    stability    slow
    Initialize And Verify System
    Run Main Loop    1000
    Verify System Ready
    ${state}=    Get System State
    Should Be Equal As Integers    ${state}[routine_count]    1000

SYS-QUAL-005 Burn-In Test
    [Documentation]    Perform burn-in qualification test
    [Tags]    qualification    burn-in    slow
    Initialize And Verify System
    ${duration}=    Run System Qualification    ${BURN_IN_CYCLES}
    Log    Burn-in completed in ${duration} seconds
    Verify System Ready

SYS-QUAL-006 System Stability Test
    [Documentation]    Verify system remains stable during continuous operation
    [Tags]    stability
    Initialize And Verify System
    Verify System Stability    duration_seconds=10
    Verify System Ready

SYS-QUAL-007 Reset And Reinitialize Test
    [Documentation]    Verify system can be reset and reinitialized
    [Tags]    functional    reset
    Initialize And Verify System
    Run Main Loop    10
    Reset And Reinitialize
    Run Main Loop    10
    Verify System Ready

SYS-QUAL-008 Power Cycle Test
    [Documentation]    Verify system survives power cycling
    [Tags]    qualification    power    critical
    Perform Power Cycle Test    count=10
    # Final verification
    ${state}=    Get System State
    Should Be Equal    ${state}[initialized]    ${True}

SYS-QUAL-009 Performance Measurement Test
    [Documentation]    Measure system performance
    [Tags]    performance
    Initialize And Verify System
    ${perf}=    Monitor System Performance    cycles=1000
    Log    System performance: ${perf} cycles/second
    Should Be True    ${perf} > 0

SYS-QUAL-010 Stress Test
    [Documentation]    Perform stress testing on system
    [Tags]    stress    slow
    [Timeout]    120s
    Initialize And Verify System
    ${duration}=    Set Variable    60
    Stress Test System    duration=${duration}
    Verify System Ready

SYS-QUAL-011 Repeated Initialization Test
    [Documentation]    Verify system handles repeated initialization attempts
    [Tags]    robustness
    FOR    ${i}    IN RANGE    5
        Log    Initialization attempt ${i+1}
        IF    ${i} > 0
            Reset System
            Sleep For    ${SHORT_DELAY}
        END
        Initialize PSoC4 System
        Verify System Ready
    END

SYS-QUAL-012 Long Duration Operation Test
    [Documentation]    Verify system operates correctly over long duration
    [Tags]    endurance    slow
    [Timeout]    300s
    Initialize And Verify System
    Run Main Loop    10000
    Verify System Ready
    ${state}=    Get System State
    Should Be Equal As Integers    ${state}[routine_count]    10000

SYS-QUAL-013 System State Verification Test
    [Documentation]    Verify system state reporting is accurate
    [Tags]    functional
    Initialize And Verify System

    ${state_before}=    Get System State
    Run Main Loop    50
    ${state_after}=    Get System State

    ${diff}=    Evaluate    ${state_after}[routine_count] - ${state_before}[routine_count]
    Should Be Equal As Integers    ${diff}    50

SYS-QUAL-014 Continuous Operation Qualification
    [Documentation]    Qualify system for continuous operation
    [Tags]    qualification    continuous
    [Timeout]    180s
    Initialize And Verify System

    # Run for extended period
    FOR    ${batch}    IN RANGE    10
        Log    Batch ${batch+1}/10
        Run Main Loop    500
        Verify System Ready
    END

    Log    Continuous operation qualification passed

SYS-QUAL-015 Operation Count Verification
    [Documentation]    Verify operation counting is accurate
    [Tags]    functional
    Initialize And Verify System

    ${count_before}=    Get Operation Count
    Run Main Loop    25
    ${count_after}=    Get Operation Count

    ${increase}=    Evaluate    ${count_after} - ${count_before}
    Should Be True    ${increase} > 0
    Log    Operation count increased by ${increase}
