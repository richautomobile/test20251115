"""
Unit tests for peripheral mock implementations.

These tests verify that the mock objects behave correctly.
"""

import pytest
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))

from mocks.peripheral_mocks import (
    I2CMock, SPIMock, ADCMock, PWMMock, CANFDMock, FlashMock, GPIOMock
)


@pytest.mark.unit
class TestI2CMockUnit:
    """Unit tests for I2CMock."""

    def test_creation(self):
        """Test I2CMock creation."""
        i2c = I2CMock(bus_id=1)
        assert i2c.bus_id == 1
        assert not i2c.initialized

    def test_initialization(self):
        """Test I2C initialization."""
        i2c = I2CMock()
        result = i2c.init(speed=400000)
        assert result == 0
        assert i2c.initialized
        assert i2c.speed == 400000


@pytest.mark.unit
class TestSPIMockUnit:
    """Unit tests for SPIMock."""

    def test_creation(self):
        """Test SPIMock creation."""
        spi = SPIMock(bus_id=0)
        assert spi.bus_id == 0
        assert not spi.initialized

    def test_transfer_length(self):
        """Test that transfer preserves length."""
        spi = SPIMock()
        spi.init()

        for length in [1, 4, 8, 16, 64]:
            tx_data = bytes(range(length))
            rx_data = spi.transfer(tx_data)
            assert len(rx_data) == length


@pytest.mark.unit
class TestADCMockUnit:
    """Unit tests for ADCMock."""

    def test_resolution(self):
        """Test ADC resolution handling."""
        for resolution in [8, 10, 12, 16]:
            adc = ADCMock(resolution=resolution)
            max_val = (1 << resolution) - 1
            assert adc.max_value == max_val

    def test_value_clamping(self):
        """Test that ADC values are clamped."""
        adc = ADCMock(resolution=12)
        adc.init()

        adc.set_value(10000)
        assert adc.read() <= 4095

        adc.set_value(-100)
        assert adc.read() >= 0


@pytest.mark.unit
class TestPWMMockUnit:
    """Unit tests for PWMMock."""

    def test_duty_cycle_range(self):
        """Test duty cycle validation."""
        pwm = PWMMock()
        pwm.init()

        assert pwm.set_duty(50) == 0
        assert pwm.set_duty(0) == 0
        assert pwm.set_duty(100) == 0
        assert pwm.set_duty(-1) != 0
        assert pwm.set_duty(101) != 0


@pytest.mark.unit
class TestCANFDMockUnit:
    """Unit tests for CANFDMock."""

    def test_queue_behavior(self):
        """Test FIFO queue behavior."""
        can = CANFDMock()
        can.init()

        # Queue frames
        can.queue_rx_frame(0x100, b"\x01")
        can.queue_rx_frame(0x200, b"\x02")

        # Should receive in order
        frame1 = can.receive()
        assert frame1.can_id == 0x100

        frame2 = can.receive()
        assert frame2.can_id == 0x200

        # Queue should be empty
        assert can.receive() is None


@pytest.mark.unit
class TestFlashMockUnit:
    """Unit tests for FlashMock."""

    def test_page_size(self):
        """Test page size handling."""
        flash = FlashMock(size=1024, page_size=64)
        assert flash.page_size == 64
        assert flash.size == 1024

    def test_erase_state(self):
        """Test that erased flash reads as 0xFF."""
        flash = FlashMock()
        # Fresh flash should be erased
        data = flash.read(0, 10)
        assert all(b == 0xFF for b in data)


@pytest.mark.unit
class TestGPIOMockUnit:
    """Unit tests for GPIOMock."""

    def test_uninitialized_pin(self):
        """Test that uninitialized pins return errors."""
        gpio = GPIOMock()

        result = gpio.write(99, True)
        assert result != 0

        value = gpio.read(99)
        assert value is None
