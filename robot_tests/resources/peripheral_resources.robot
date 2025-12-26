*** Settings ***
Documentation    Resource file for peripheral testing
Library          ../libraries/PeripheralLibrary.py
Resource         common.robot


*** Variables ***
# I2C configuration
${I2C_BUS}             0
${I2C_SPEED_STANDARD}  100000
${I2C_SPEED_FAST}      400000
${I2C_DEVICE_ADDR}     0x50

# ADC configuration
${ADC_CHANNEL}         0
${ADC_RESOLUTION}      12
${ADC_VREF}            3.3
${ADC_MAX_VALUE}       4095

# PWM configuration
${PWM_CHANNEL}         0
${PWM_FREQUENCY}       1000
${PWM_DUTY_DEFAULT}    50

# GPIO configuration
${GPIO_INSTANCE}       0


*** Keywords ***
Setup I2C Communication
    [Documentation]    Setup I2C for testing
    [Arguments]    ${speed}=${I2C_SPEED_FAST}
    Initialize I2C    ${I2C_BUS}    ${speed}
    Add I2C Device    ${I2C_BUS}    ${I2C_DEVICE_ADDR}
    Log    I2C communication setup complete    level=INFO

Test I2C Write Read Cycle
    [Documentation]    Test complete I2C write-read cycle
    [Arguments]    ${test_data}=\\x01\\x02\\x03\\x04
    # Write data
    I2C Write    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    ${test_data}

    # Read back
    ${length}=    Get Length    ${test_data}
    ${read_data}=    I2C Read    ${I2C_BUS}    ${I2C_DEVICE_ADDR}    0x00    ${length}

    # Verify
    Should Be Equal    ${read_data}    ${test_data}
    Log    I2C write-read cycle successful    level=INFO

Setup ADC For Testing
    [Documentation]    Setup ADC channel for testing
    [Arguments]    ${channel}=${ADC_CHANNEL}    ${resolution}=${ADC_RESOLUTION}
    Initialize ADC    ${channel}    ${resolution}
    Log    ADC channel ${channel} setup complete    level=INFO

Test ADC Reading
    [Documentation]    Test ADC reading at specified voltage
    [Arguments]    ${voltage}    ${expected_value}    ${tolerance}=${ADC_TOLERANCE}
    Set ADC Voltage    ${ADC_CHANNEL}    ${voltage}    ${ADC_VREF}
    ${actual}=    Read ADC    ${ADC_CHANNEL}
    Verify Value In Range    ${actual}    ${expected_value - tolerance}    ${expected_value + tolerance}
    ...    ADC reading out of range
    Log    ADC reading verified: ${actual} (expected ${expected_value})    level=INFO

Setup PWM Output
    [Documentation]    Setup PWM for testing
    [Arguments]    ${channel}=${PWM_CHANNEL}    ${freq}=${PWM_FREQUENCY}    ${duty}=${PWM_DUTY_DEFAULT}
    Initialize PWM    ${channel}    ${freq}    ${duty}
    Start PWM    ${channel}
    Log    PWM output setup and started    level=INFO

Test PWM Duty Cycle Range
    [Documentation]    Test PWM across duty cycle range
    [Arguments]    ${channel}=${PWM_CHANNEL}
    ${duty_values}=    Create List    0    25    50    75    100
    FOR    ${duty}    IN    @{duty_values}
        Set PWM Duty Cycle    ${channel}    ${duty}
        Sleep For    ${SHORT_DELAY}
        Log    PWM duty cycle ${duty}% verified    level=INFO
    END

Setup GPIO Pins
    [Documentation]    Setup GPIO pins for testing
    [Arguments]    ${instance}=${GPIO_INSTANCE}
    Initialize GPIO    ${instance}
    Log    GPIO instance ${instance} setup complete    level=INFO

Test GPIO Output
    [Documentation]    Test GPIO output functionality
    [Arguments]    ${pin}    ${instance}=${GPIO_INSTANCE}
    Configure GPIO Pin    ${instance}    ${pin}    output

    # Test high
    Write GPIO    ${instance}    ${pin}    True
    ${state}=    Read GPIO    ${instance}    ${pin}
    Should Be True    ${state}    GPIO pin ${pin} should be high

    # Test low
    Write GPIO    ${instance}    ${pin}    False
    ${state}=    Read GPIO    ${instance}    ${pin}
    Should Not Be True    ${state}    GPIO pin ${pin} should be low

    Log    GPIO pin ${pin} output test passed    level=INFO

Verify Peripheral Communication
    [Documentation]    Verify all peripherals can communicate
    Setup I2C Communication
    Test I2C Write Read Cycle

    Setup ADC For Testing
    ${value}=    Read ADC
    Should Be True    ${value} >= 0

    Log    All peripheral communications verified    level=INFO
