"""
Integration tests for peripheral drivers.

These tests verify I2C, SPI, ADC, PWM, CAN-FD, Flash, and GPIO functionality.
"""

import pytest
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from mocks.peripheral_mocks import (
    I2CMock, SPIMock, ADCMock, PWMMock, CANFDMock, FlashMock, GPIOMock
)


# ============================================================================
# I2C Integration Tests
# ============================================================================

class TestI2CIntegration:
    """Integration tests for I2C driver."""

    @pytest.fixture
    def i2c(self, sample_i2c_data):
        """Provide initialized I2C mock with a device."""
        i2c = I2CMock(bus_id=0)
        i2c.init(speed=400000)
        i2c.add_device(sample_i2c_data["device_address"])
        return i2c

    @pytest.mark.integration
    @pytest.mark.i2c
    def test_i2c_write_read_cycle(self, i2c, sample_i2c_data):
        """Test complete I2C write-read cycle."""
        addr = sample_i2c_data["device_address"]
        reg = sample_i2c_data["register_address"]
        data = sample_i2c_data["write_data"]

        # Write data
        result = i2c.write(addr, reg, data)
        assert result == 0, "I2C write should succeed"

        # Read back data
        read_data = i2c.read(addr, reg, len(data))
        assert read_data == data, "Read data should match written data"

    @pytest.mark.integration
    @pytest.mark.i2c
    def test_i2c_multiple_devices(self):
        """Test I2C with multiple devices on the bus."""
        i2c = I2CMock(bus_id=0)
        i2c.init()

        # Add multiple devices
        devices = [0x50, 0x51, 0x52]
        for addr in devices:
            i2c.add_device(addr)

        # Write to each device
        for addr in devices:
            result = i2c.write(addr, 0x00, bytes([addr]))
            assert result == 0

        # Verify each device
        for addr in devices:
            data = i2c.read(addr, 0x00, 1)
            assert data[0] == addr

    @pytest.mark.integration
    @pytest.mark.i2c
    def test_i2c_transaction_logging(self, i2c, sample_i2c_data):
        """Test I2C transaction logging."""
        addr = sample_i2c_data["device_address"]

        # Clear transaction history
        i2c.clear_transactions()

        # Perform operations
        i2c.write(addr, 0x00, b"\x11\x22")
        i2c.read(addr, 0x00, 2)

        # Check transactions
        transactions = i2c.get_transactions()
        assert len(transactions) == 2
        assert transactions[0].is_write
        assert not transactions[1].is_write

    @pytest.mark.integration
    @pytest.mark.i2c
    def test_i2c_error_handling(self):
        """Test I2C error conditions."""
        i2c = I2CMock(bus_id=0)

        # Write without init should fail
        result = i2c.write(0x50, 0x00, b"\x01")
        assert result != 0

        # Initialize
        i2c.init()

        # Write to non-existent device should fail
        result = i2c.write(0x99, 0x00, b"\x01")
        assert result != 0


# ============================================================================
# SPI Integration Tests
# ============================================================================

class TestSPIIntegration:
    """Integration tests for SPI driver."""

    @pytest.fixture
    def spi(self):
        """Provide initialized SPI mock."""
        spi = SPIMock(bus_id=0)
        spi.init(speed=1000000, mode=0)
        return spi

    @pytest.mark.integration
    @pytest.mark.spi
    def test_spi_transfer_echo(self, spi):
        """Test SPI transfer with echo."""
        tx_data = bytes([0x01, 0x02, 0x03, 0x04])
        rx_data = spi.transfer(tx_data)

        assert len(rx_data) == len(tx_data)
        assert rx_data == tx_data  # Default echo mode

    @pytest.mark.integration
    @pytest.mark.spi
    def test_spi_transfer_with_queued_data(self, spi):
        """Test SPI transfer with pre-queued RX data."""
        tx_data = bytes([0xAA, 0xBB])
        expected_rx = bytes([0x11, 0x22])

        # Queue RX data
        spi.queue_rx_data(expected_rx)

        # Transfer
        rx_data = spi.transfer(tx_data)
        assert rx_data == expected_rx

    @pytest.mark.integration
    @pytest.mark.spi
    def test_spi_multiple_transfers(self, spi):
        """Test multiple SPI transfers."""
        for i in range(10):
            tx_data = bytes([i, i+1, i+2])
            rx_data = spi.transfer(tx_data)
            assert len(rx_data) == 3


# ============================================================================
# ADC Integration Tests
# ============================================================================

class TestADCIntegration:
    """Integration tests for ADC driver."""

    @pytest.fixture
    def adc(self):
        """Provide initialized ADC mock."""
        adc = ADCMock(channel=0, resolution=12)
        adc.init()
        return adc

    @pytest.mark.integration
    @pytest.mark.adc
    def test_adc_read_default(self, adc):
        """Test ADC read with default value."""
        value = adc.read()
        assert 0 <= value <= 4095  # 12-bit range

    @pytest.mark.integration
    @pytest.mark.adc
    def test_adc_set_value(self, adc, sample_adc_values):
        """Test ADC with set values."""
        for name, expected in sample_adc_values.items():
            adc.set_value(expected)
            actual = adc.read()
            assert actual == expected, f"ADC {name} value mismatch"

    @pytest.mark.integration
    @pytest.mark.adc
    def test_adc_voltage_conversion(self, adc):
        """Test ADC voltage to value conversion."""
        test_cases = [
            (0.0, 0),
            (1.65, 2048),
            (3.3, 4095)
        ]

        for voltage, expected in test_cases:
            adc.set_voltage(voltage, vref=3.3)
            actual = adc.read()
            pytest.assert_in_range(actual, expected - 10, expected + 10,
                                   f"Voltage {voltage}V")

    @pytest.mark.integration
    @pytest.mark.adc
    def test_adc_bounds_checking(self, adc):
        """Test ADC value bounds checking."""
        # Test upper bound
        adc.set_value(9999)
        assert adc.read() == 4095

        # Test lower bound
        adc.set_value(-100)
        assert adc.read() == 0


# ============================================================================
# PWM Integration Tests
# ============================================================================

class TestPWMIntegration:
    """Integration tests for PWM driver."""

    @pytest.fixture
    def pwm(self):
        """Provide initialized PWM mock."""
        pwm = PWMMock(channel=0)
        pwm.init(frequency=1000, duty=50)
        return pwm

    @pytest.mark.integration
    @pytest.mark.pwm
    def test_pwm_start_stop(self, pwm):
        """Test PWM start and stop."""
        assert not pwm.running

        result = pwm.start()
        assert result == 0
        assert pwm.running

        result = pwm.stop()
        assert result == 0
        assert not pwm.running

    @pytest.mark.integration
    @pytest.mark.pwm
    def test_pwm_duty_cycle_range(self, pwm):
        """Test PWM duty cycle adjustment."""
        pwm.start()

        # Test various duty cycles
        for duty in [0, 25, 50, 75, 100]:
            result = pwm.set_duty(duty)
            assert result == 0
            assert pwm.duty_cycle == duty

    @pytest.mark.integration
    @pytest.mark.pwm
    def test_pwm_invalid_duty(self, pwm):
        """Test PWM with invalid duty cycle."""
        assert pwm.set_duty(-10) != 0
        assert pwm.set_duty(150) != 0

    @pytest.mark.integration
    @pytest.mark.pwm
    def test_pwm_frequency_setting(self):
        """Test PWM frequency configuration."""
        frequencies = [100, 1000, 10000, 100000]

        for freq in frequencies:
            pwm = PWMMock(channel=0)
            pwm.init(frequency=freq, duty=50)
            assert pwm.frequency == freq


# ============================================================================
# CAN-FD Integration Tests
# ============================================================================

class TestCANFDIntegration:
    """Integration tests for CAN-FD driver."""

    @pytest.fixture
    def canfd(self):
        """Provide initialized CAN-FD mock."""
        can = CANFDMock(bus_id=0)
        can.init(baudrate=500000)
        return can

    @pytest.mark.integration
    @pytest.mark.canfd
    def test_canfd_send_frame(self, canfd, sample_canfd_frame):
        """Test sending a CAN-FD frame."""
        result = canfd.send(
            sample_canfd_frame["id"],
            sample_canfd_frame["data"]
        )
        assert result == 0

        # Verify frame was logged
        assert len(canfd.tx_frames) == 1
        frame = canfd.tx_frames[0]
        assert frame.can_id == sample_canfd_frame["id"]
        assert frame.data == sample_canfd_frame["data"]

    @pytest.mark.integration
    @pytest.mark.canfd
    def test_canfd_receive_frame(self, canfd, sample_canfd_frame):
        """Test receiving a CAN-FD frame."""
        # Queue a frame
        canfd.queue_rx_frame(
            sample_canfd_frame["id"],
            sample_canfd_frame["data"]
        )

        # Receive it
        frame = canfd.receive()
        assert frame is not None
        assert frame.can_id == sample_canfd_frame["id"]
        assert frame.data == sample_canfd_frame["data"]

    @pytest.mark.integration
    @pytest.mark.canfd
    def test_canfd_multiple_frames(self, canfd):
        """Test sending and receiving multiple frames."""
        frames_to_send = [
            (0x100, b"\x01\x02\x03\x04"),
            (0x200, b"\x05\x06\x07\x08"),
            (0x300, b"\x09\x0A\x0B\x0C"),
        ]

        # Queue all frames
        for can_id, data in frames_to_send:
            canfd.queue_rx_frame(can_id, data)

        # Receive all frames
        for expected_id, expected_data in frames_to_send:
            frame = canfd.receive()
            assert frame is not None
            assert frame.can_id == expected_id
            assert frame.data == expected_data

    @pytest.mark.integration
    @pytest.mark.canfd
    def test_canfd_empty_queue(self, canfd):
        """Test receiving from empty queue."""
        frame = canfd.receive()
        assert frame is None


# ============================================================================
# Flash Integration Tests
# ============================================================================

class TestFlashIntegration:
    """Integration tests for Flash memory driver."""

    @pytest.fixture
    def flash(self):
        """Provide Flash mock."""
        return FlashMock(size=64 * 1024, page_size=256)

    @pytest.mark.integration
    @pytest.mark.flash
    def test_flash_write_read(self, flash):
        """Test Flash write and read."""
        test_data = b"PSoC4 Flash Test Data"
        address = 0x1000

        # Write
        result = flash.write(address, test_data)
        assert result == 0

        # Read
        read_data = flash.read(address, len(test_data))
        assert read_data == test_data

    @pytest.mark.integration
    @pytest.mark.flash
    def test_flash_erase(self, flash):
        """Test Flash page erase."""
        # Write some data
        flash.write(0, b"\x11\x22\x33\x44")

        # Erase page 0
        result = flash.erase_page(0)
        assert result == 0

        # Verify erased (should be 0xFF)
        data = flash.read(0, 4)
        assert data == b"\xFF\xFF\xFF\xFF"

    @pytest.mark.integration
    @pytest.mark.flash
    def test_flash_wear_tracking(self, flash):
        """Test Flash wear tracking."""
        assert flash.write_count == 0
        assert flash.erase_count == 0

        # Perform operations
        flash.write(0, b"\x01\x02")
        flash.erase_page(0)

        assert flash.write_count == 1
        assert flash.erase_count == 1


# ============================================================================
# GPIO Integration Tests
# ============================================================================

class TestGPIOIntegration:
    """Integration tests for GPIO driver."""

    @pytest.fixture
    def gpio(self):
        """Provide GPIO mock."""
        return GPIOMock()

    @pytest.mark.integration
    @pytest.mark.gpio
    def test_gpio_output(self, gpio):
        """Test GPIO output operations."""
        pin = 1

        # Initialize as output
        gpio.init(pin, direction="output")

        # Write values
        assert gpio.write(pin, True) == 0
        assert gpio.read(pin) == True

        assert gpio.write(pin, False) == 0
        assert gpio.read(pin) == False

    @pytest.mark.integration
    @pytest.mark.gpio
    def test_gpio_toggle(self, gpio):
        """Test GPIO toggle."""
        pin = 2

        gpio.init(pin, direction="output")
        gpio.write(pin, False)

        # Toggle
        gpio.toggle(pin)
        assert gpio.read(pin) == True

        gpio.toggle(pin)
        assert gpio.read(pin) == False

    @pytest.mark.integration
    @pytest.mark.gpio
    def test_gpio_multiple_pins(self, gpio):
        """Test multiple GPIO pins."""
        pins = [1, 2, 3, 4, 5]

        # Initialize all pins
        for pin in pins:
            gpio.init(pin, direction="output")

        # Set different values
        for i, pin in enumerate(pins):
            value = i % 2 == 0
            gpio.write(pin, value)

        # Verify
        for i, pin in enumerate(pins):
            expected = i % 2 == 0
            assert gpio.read(pin) == expected
