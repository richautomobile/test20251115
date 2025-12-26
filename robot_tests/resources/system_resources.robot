*** Settings ***
Documentation    Resource file for system-level testing
Library          ../libraries/PSoC4Library.py    mode=mock
Resource         common.robot


*** Variables ***
${INIT_TIMEOUT}         5s
${MAX_CYCLES}           10000
${BURN_IN_CYCLES}       1000


*** Keywords ***
Initialize And Verify System
    [Documentation]    Initialize system and verify it's ready
    Initialize PSoC4 System
    Verify System Ready
    Log    System initialized and verified    level=INFO

Run System Qualification
    [Documentation]    Run complete system qualification sequence
    [Arguments]    ${cycles}=${BURN_IN_CYCLES}
    Log    Starting system qualification with ${cycles} cycles    level=INFO
    ${start}=    Get Current Date
    Run Main Loop    ${cycles}
    ${end}=    Get Current Date
    ${duration}=    Subtract Date From Date    ${end}    ${start}
    Log    Qualification completed in ${duration} seconds    level=INFO
    RETURN    ${duration}

Verify System Stability
    [Documentation]    Verify system remains stable during operation
    [Arguments]    ${duration_seconds}=10
    ${cycles}=    Evaluate    int(${duration_seconds} * 100)
    Run Main Loop    ${cycles}
    Verify System Ready

Stress Test System
    [Documentation]    Run stress test on system
    [Arguments]    ${duration}=3600
    Log    Starting stress test for ${duration} seconds    level=WARN
    ${cycles}=    Evaluate    ${duration} * 100
    Run Main Loop    ${cycles}
    Verify System Ready
    Log    Stress test completed successfully    level=INFO

Reset And Reinitialize
    [Documentation]    Reset system and reinitialize
    Reset System
    Sleep For    ${SHORT_DELAY}
    Initialize And Verify System

Perform Power Cycle Test
    [Documentation]    Simulate power cycle
    [Arguments]    ${count}=1
    FOR    ${i}    IN RANGE    ${count}
        Log    Power cycle ${i+1}/${count}    level=INFO
        Reset System
        Sleep For    ${MEDIUM_DELAY}
        Initialize And Verify System
        Run Main Loop    10
    END
    Log    All ${count} power cycles completed successfully    level=INFO

Monitor System Performance
    [Documentation]    Monitor and log system performance metrics
    [Arguments]    ${cycles}=100
    ${start}=    Get Current Date
    Run Main Loop    ${cycles}
    ${end}=    Get Current Date
    ${duration}=    Subtract Date From Date    ${end}    ${start}
    ${cycles_per_sec}=    Evaluate    ${cycles} / ${duration}
    Log    Performance: ${cycles_per_sec} cycles/second    level=INFO
    RETURN    ${cycles_per_sec}
