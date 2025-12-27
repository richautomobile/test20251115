"""
PeripheralLibrary - Robot Framework keywords for peripheral testing.

Provides keywords for I2C, SPI, ADC, PWM, CAN-FD, Flash, and GPIO testing.
"""

import sys
from pathlib import Path
from robot.api.deco import keyword, library
from robot.api import logger

# Add parent directory to path
sys.path.insert(0, str(Path(__file__).parent.parent.parent / "tests"))

try:
    from tests.mocks.peripheral_mocks import (
        I2CMock, SPIMock, ADCMock, PWMMock, CANFDMock, FlashMock, GPIOMock
    )
except ImportError:
    logger.warn("Could not import peripheral mocks")


@library(scope='SUITE')
class PeripheralLibrary:
    """
    Robot Framework library for peripheral testing.

    Supports:
    - I2C communication
    - SPI communication
    - ADC sampling
    - PWM generation
    - CAN-FD messaging
    - Flash memory operations
    - GPIO control

    = Examples =

    | Initialize I2C | 0 | 400000 |
    | I2C Write | 0x50 | 0x00 | \\x01\\x02\\x03 |
    | ${data}= | I2C Read | 0x50 | 0x00 | 3 |
    """

    ROBOT_LIBRARY_SCOPE = 'SUITE'
    ROBOT_LIBRARY_VERSION = '1.0.0'

    def __init__(self):
        """Initialize PeripheralLibrary."""
        self.i2c_instances = {}
        self.spi_instances = {}
        self.adc_instances = {}
        self.pwm_instances = {}
        self.canfd_instances = {}
        self.flash_instances = {}
        self.gpio_instances = {}
        logger.info("PeripheralLibrary initialized")

    # ========================================================================
    # I2C Keywords
    # ========================================================================

    @keyword("Initialize I2C")
    def initialize_i2c(self, bus_id=0, speed=100000):
        """
        Initialize I2C bus.

        Args:
            bus_id: I2C bus identifier (default: 0)
            speed: I2C speed in Hz (default: 100000)

        Example:
        | Initialize I2C | 0 | 400000 |
        """
        bus_id = int(bus_id)
        speed = int(speed)

        i2c = I2CMock(bus_id=bus_id)
        result = i2c.init(speed=speed)

        if result != 0:
            raise AssertionError(f"I2C initialization failed: {result}")

        self.i2c_instances[bus_id] = i2c
        logger.info(f"I2C bus {bus_id} initialized at {speed}Hz")

    @keyword("Add I2C Device")
    def add_i2c_device(self, bus_id, device_address, memory_size=256):
        """
        Add a simulated I2C device to the bus.

        Args:
            bus_id: I2C bus identifier
            device_address: Device address (hex or decimal)
            memory_size: Device memory size in bytes (default: 256)

        Example:
        | Add I2C Device | 0 | 0x50 | 256 |
        """
        bus_id = int(bus_id)
        device_address = int(device_address, 0) if isinstance(device_address, str) else int(device_address)
        memory_size = int(memory_size)

        i2c = self._get_i2c(bus_id)
        i2c.add_device(device_address, memory_size)
        logger.info(f"Added I2C device at 0x{device_address:02X} on bus {bus_id}")

    @keyword("I2C Write")
    def i2c_write(self, bus_id, device_address, register_address, data):
        """
        Write data to I2C device.

        Args:
            bus_id: I2C bus identifier
            device_address: Device address
            register_address: Register address
            data: Data to write (bytes or hex string)

        Example:
        | I2C Write | 0 | 0x50 | 0x00 | \\x01\\x02\\x03 |
        """
        bus_id = int(bus_id)
        device_address = int(device_address, 0) if isinstance(device_address, str) else int(device_address)
        register_address = int(register_address, 0) if isinstance(register_address, str) else int(register_address)

        if isinstance(data, str):
            # Convert hex string to bytes
            data = bytes.fromhex(data.replace('\\x', ''))

        i2c = self._get_i2c(bus_id)
        result = i2c.write(device_address, register_address, data)

        if result != 0:
            raise AssertionError(f"I2C write failed: {result}")

        logger.info(f"I2C write: 0x{device_address:02X}[0x{register_address:02X}] = {data.hex()}")

    @keyword("I2C Read")
    def i2c_read(self, bus_id, device_address, register_address, length):
        """
        Read data from I2C device.

        Args:
            bus_id: I2C bus identifier
            device_address: Device address
            register_address: Register address
            length: Number of bytes to read

        Returns:
            Data read from device

        Example:
        | ${data}= | I2C Read | 0 | 0x50 | 0x00 | 4 |
        """
        bus_id = int(bus_id)
        device_address = int(device_address, 0) if isinstance(device_address, str) else int(device_address)
        register_address = int(register_address, 0) if isinstance(register_address, str) else int(register_address)
        length = int(length)

        i2c = self._get_i2c(bus_id)
        data = i2c.read(device_address, register_address, length)

        logger.info(f"I2C read: 0x{device_address:02X}[0x{register_address:02X}] = {data.hex()}")
        return data

    # ========================================================================
    # ADC Keywords
    # ========================================================================

    @keyword("Initialize ADC")
    def initialize_adc(self, channel=0, resolution=12):
        """
        Initialize ADC channel.

        Args:
            channel: ADC channel number (default: 0)
            resolution: ADC resolution in bits (default: 12)

        Example:
        | Initialize ADC | 0 | 12 |
        """
        channel = int(channel)
        resolution = int(resolution)

        adc = ADCMock(channel=channel, resolution=resolution)
        result = adc.init()

        if result != 0:
            raise AssertionError(f"ADC initialization failed: {result}")

        self.adc_instances[channel] = adc
        logger.info(f"ADC channel {channel} initialized ({resolution}-bit)")

    @keyword("Read ADC")
    def read_adc(self, channel=0):
        """
        Read ADC value.

        Args:
            channel: ADC channel number (default: 0)

        Returns:
            ADC value

        Example:
        | ${value}= | Read ADC | 0 |
        """
        channel = int(channel)
        adc = self._get_adc(channel)
        value = adc.read()
        logger.info(f"ADC channel {channel} read: {value}")
        return value

    @keyword("Set ADC Value")
    def set_adc_value(self, channel, value):
        """
        Set ADC reading value (for simulation).

        Args:
            channel: ADC channel number
            value: Value to set

        Example:
        | Set ADC Value | 0 | 2048 |
        """
        channel = int(channel)
        value = int(value)
        adc = self._get_adc(channel)
        adc.set_value(value)
        logger.info(f"ADC channel {channel} value set to {value}")

    @keyword("Set ADC Voltage")
    def set_adc_voltage(self, channel, voltage, vref=3.3):
        """
        Set ADC value based on voltage.

        Args:
            channel: ADC channel number
            voltage: Voltage to set
            vref: Reference voltage (default: 3.3V)

        Example:
        | Set ADC Voltage | 0 | 1.65 | 3.3 |
        """
        channel = int(channel)
        voltage = float(voltage)
        vref = float(vref)
        adc = self._get_adc(channel)
        adc.set_voltage(voltage, vref)
        logger.info(f"ADC channel {channel} voltage set to {voltage}V (Vref={vref}V)")

    # ========================================================================
    # PWM Keywords
    # ========================================================================

    @keyword("Initialize PWM")
    def initialize_pwm(self, channel=0, frequency=1000, duty=50):
        """
        Initialize PWM channel.

        Args:
            channel: PWM channel number (default: 0)
            frequency: PWM frequency in Hz (default: 1000)
            duty: Duty cycle percentage (default: 50)

        Example:
        | Initialize PWM | 0 | 1000 | 50 |
        """
        channel = int(channel)
        frequency = int(frequency)
        duty = int(duty)

        pwm = PWMMock(channel=channel)
        result = pwm.init(frequency=frequency, duty=duty)

        if result != 0:
            raise AssertionError(f"PWM initialization failed: {result}")

        self.pwm_instances[channel] = pwm
        logger.info(f"PWM channel {channel} initialized: {frequency}Hz, {duty}% duty")

    @keyword("Start PWM")
    def start_pwm(self, channel=0):
        """
        Start PWM output.

        Args:
            channel: PWM channel number (default: 0)

        Example:
        | Start PWM | 0 |
        """
        channel = int(channel)
        pwm = self._get_pwm(channel)
        result = pwm.start()

        if result != 0:
            raise AssertionError(f"PWM start failed: {result}")

        logger.info(f"PWM channel {channel} started")

    @keyword("Stop PWM")
    def stop_pwm(self, channel=0):
        """
        Stop PWM output.

        Args:
            channel: PWM channel number (default: 0)

        Example:
        | Stop PWM | 0 |
        """
        channel = int(channel)
        pwm = self._get_pwm(channel)
        result = pwm.stop()

        if result != 0:
            raise AssertionError(f"PWM stop failed: {result}")

        logger.info(f"PWM channel {channel} stopped")

    @keyword("Set PWM Duty Cycle")
    def set_pwm_duty_cycle(self, channel, duty):
        """
        Set PWM duty cycle.

        Args:
            channel: PWM channel number
            duty: Duty cycle percentage (0-100)

        Example:
        | Set PWM Duty Cycle | 0 | 75 |
        """
        channel = int(channel)
        duty = int(duty)
        pwm = self._get_pwm(channel)
        result = pwm.set_duty(duty)

        if result != 0:
            raise AssertionError(f"PWM duty cycle set failed: {result}")

        logger.info(f"PWM channel {channel} duty cycle set to {duty}%")

    # ========================================================================
    # GPIO Keywords
    # ========================================================================

    @keyword("Initialize GPIO")
    def initialize_gpio(self, instance_id=0):
        """
        Initialize GPIO.

        Args:
            instance_id: GPIO instance identifier (default: 0)

        Example:
        | Initialize GPIO | 0 |
        """
        instance_id = int(instance_id)
        gpio = GPIOMock()
        self.gpio_instances[instance_id] = gpio
        logger.info(f"GPIO instance {instance_id} initialized")

    @keyword("Configure GPIO Pin")
    def configure_gpio_pin(self, instance_id, pin, direction):
        """
        Configure GPIO pin direction.

        Args:
            instance_id: GPIO instance identifier
            pin: Pin number
            direction: 'input' or 'output'

        Example:
        | Configure GPIO Pin | 0 | 5 | output |
        """
        instance_id = int(instance_id)
        pin = int(pin)
        gpio = self._get_gpio(instance_id)
        result = gpio.init(pin, direction=direction)

        if result != 0:
            raise AssertionError(f"GPIO pin configuration failed: {result}")

        logger.info(f"GPIO pin {pin} configured as {direction}")

    @keyword("Write GPIO")
    def write_gpio(self, instance_id, pin, value):
        """
        Write to GPIO pin.

        Args:
            instance_id: GPIO instance identifier
            pin: Pin number
            value: True/False or 1/0

        Example:
        | Write GPIO | 0 | 5 | True |
        """
        instance_id = int(instance_id)
        pin = int(pin)
        value = bool(value) if isinstance(value, (bool, int)) else value.lower() == 'true'
        gpio = self._get_gpio(instance_id)
        result = gpio.write(pin, value)

        if result != 0:
            raise AssertionError(f"GPIO write failed: {result}")

        logger.info(f"GPIO pin {pin} set to {value}")

    @keyword("Read GPIO")
    def read_gpio(self, instance_id, pin):
        """
        Read from GPIO pin.

        Args:
            instance_id: GPIO instance identifier
            pin: Pin number

        Returns:
            Pin state (True/False)

        Example:
        | ${state}= | Read GPIO | 0 | 5 |
        """
        instance_id = int(instance_id)
        pin = int(pin)
        gpio = self._get_gpio(instance_id)
        value = gpio.read(pin)

        if value is None:
            raise AssertionError(f"GPIO read failed")

        logger.info(f"GPIO pin {pin} read: {value}")
        return value

    # ========================================================================
    # Helper Methods
    # ========================================================================

    def _get_i2c(self, bus_id):
        """Get I2C instance or raise error."""
        if bus_id not in self.i2c_instances:
            raise AssertionError(f"I2C bus {bus_id} not initialized")
        return self.i2c_instances[bus_id]

    def _get_adc(self, channel):
        """Get ADC instance or raise error."""
        if channel not in self.adc_instances:
            raise AssertionError(f"ADC channel {channel} not initialized")
        return self.adc_instances[channel]

    def _get_pwm(self, channel):
        """Get PWM instance or raise error."""
        if channel not in self.pwm_instances:
            raise AssertionError(f"PWM channel {channel} not initialized")
        return self.pwm_instances[channel]

    def _get_gpio(self, instance_id):
        """Get GPIO instance or raise error."""
        if instance_id not in self.gpio_instances:
            raise AssertionError(f"GPIO instance {instance_id} not initialized")
        return self.gpio_instances[instance_id]
