"""
Pytest configuration and shared fixtures for PSoC4 embedded testing.

This module provides:
- Hardware abstraction layer (HAL) mocks
- Common test fixtures
- Test utilities and helpers
"""

import os
import sys
import pytest
import logging
from pathlib import Path
from typing import Dict, Any
from unittest.mock import Mock, MagicMock

# Add project paths
PROJECT_ROOT = Path(__file__).parent.parent
sys.path.insert(0, str(PROJECT_ROOT / "tests"))
sys.path.insert(0, str(PROJECT_ROOT / "tests" / "wrappers"))
sys.path.insert(0, str(PROJECT_ROOT / "tests" / "mocks"))

# Configure logging
logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s - %(name)s - %(levelname)s - %(message)s'
)
logger = logging.getLogger(__name__)


# ============================================================================
# Session-scoped fixtures (run once per test session)
# ============================================================================

@pytest.fixture(scope="session")
def project_root():
    """Provide the project root directory path."""
    return PROJECT_ROOT


@pytest.fixture(scope="session")
def build_config():
    """Provide build configuration for tests."""
    return {
        "target": "CY8CKIT-041S-MAX",
        "toolchain": "GCC_ARM",
        "build_type": "Debug",
        "optimization": "O0"
    }


# ============================================================================
# Module-scoped fixtures (run once per test module)
# ============================================================================

@pytest.fixture(scope="module")
def hardware_config():
    """Provide hardware configuration for testing."""
    return {
        "clock_freq": 48000000,  # 48 MHz
        "peripherals": {
            "i2c": {"enabled": True, "speed": 400000},
            "spi": {"enabled": True, "speed": 1000000},
            "canfd": {"enabled": True, "baudrate": 500000},
            "uart": {"enabled": True, "baudrate": 115200},
            "adc": {"enabled": True, "resolution": 12},
            "pwm": {"enabled": True, "frequency": 1000}
        }
    }


# ============================================================================
# Function-scoped fixtures (run for each test function)
# ============================================================================

@pytest.fixture
def mock_hal():
    """
    Provide a mock Hardware Abstraction Layer (HAL).

    This fixture creates mock objects for all hardware interfaces,
    allowing tests to run without actual hardware.
    """
    hal = MagicMock()

    # BSP (Board Support Package) mocks
    hal.cybsp_init = Mock(return_value=0)  # CY_RSLT_SUCCESS
    hal.CY_ASSERT = Mock()

    # GPIO mocks
    hal.gpio_write = Mock()
    hal.gpio_read = Mock(return_value=0)
    hal.gpio_init = Mock(return_value=0)

    # I2C mocks
    hal.i2c_master_init = Mock(return_value=0)
    hal.i2c_master_write = Mock(return_value=0)
    hal.i2c_master_read = Mock(return_value=0)
    hal.i2c_slave_init = Mock(return_value=0)

    # SPI mocks
    hal.spi_init = Mock(return_value=0)
    hal.spi_transfer = Mock(return_value=0)

    # ADC mocks
    hal.adc_init = Mock(return_value=0)
    hal.adc_read = Mock(return_value=2048)  # Mid-range value

    # PWM mocks
    hal.pwm_init = Mock(return_value=0)
    hal.pwm_start = Mock(return_value=0)
    hal.pwm_set_duty = Mock(return_value=0)

    # Timer mocks
    hal.timer_init = Mock(return_value=0)
    hal.timer_start = Mock(return_value=0)
    hal.timer_get_count = Mock(return_value=0)

    # Flash memory mocks
    hal.flash_init = Mock(return_value=0)
    hal.flash_write = Mock(return_value=0)
    hal.flash_read = Mock(return_value=0)
    hal.flash_erase = Mock(return_value=0)

    # CAN-FD mocks
    hal.canfd_init = Mock(return_value=0)
    hal.canfd_send = Mock(return_value=0)
    hal.canfd_receive = Mock(return_value=0)

    return hal


@pytest.fixture
def mock_system():
    """Provide mock system functions."""
    system = MagicMock()
    system.Sys_VariableInit = Mock(return_value=0)
    system.Sys_HwInit = Mock(return_value=0)
    system.Sys_ProjectInit = Mock(return_value=0)
    system.Sys_RoutineCall = Mock(return_value=0)
    return system


@pytest.fixture
def test_data_dir(tmp_path):
    """
    Provide a temporary directory for test data.

    Args:
        tmp_path: Pytest's built-in tmp_path fixture

    Returns:
        Path to temporary test data directory
    """
    data_dir = tmp_path / "test_data"
    data_dir.mkdir()
    return data_dir


@pytest.fixture
def sample_i2c_data():
    """Provide sample I2C test data."""
    return {
        "device_address": 0x50,
        "register_address": 0x00,
        "write_data": bytes([0x01, 0x02, 0x03, 0x04]),
        "read_data": bytes([0xAA, 0xBB, 0xCC, 0xDD]),
    }


@pytest.fixture
def sample_canfd_frame():
    """Provide sample CAN-FD frame data."""
    return {
        "id": 0x123,
        "data": bytes([0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88]),
        "dlc": 8,
        "flags": 0x00
    }


@pytest.fixture
def sample_adc_values():
    """Provide sample ADC test values."""
    return {
        "zero": 0,
        "quarter": 1024,
        "half": 2048,
        "three_quarters": 3072,
        "full": 4095
    }


# ============================================================================
# Autouse fixtures (automatically used by all tests)
# ============================================================================

@pytest.fixture(autouse=True)
def reset_mocks():
    """Reset all mock objects before each test."""
    yield
    # Cleanup happens after test execution


@pytest.fixture(autouse=True)
def test_logger(request):
    """Provide logging for each test."""
    test_name = request.node.name
    logger.info(f"Starting test: {test_name}")
    yield logger
    logger.info(f"Finished test: {test_name}")


# ============================================================================
# Pytest hooks
# ============================================================================

def pytest_configure(config):
    """Configure pytest with custom settings."""
    config.addinivalue_line(
        "markers", "hardware: mark test as requiring actual hardware"
    )
    logger.info("Pytest configuration complete")


def pytest_collection_modifyitems(config, items):
    """Modify test collection to add markers automatically."""
    for item in items:
        # Auto-mark tests based on file location
        if "integration" in str(item.fspath):
            item.add_marker(pytest.mark.integration)
        if "unit" in str(item.fspath):
            item.add_marker(pytest.mark.unit)

        # Auto-mark based on test name patterns
        if "i2c" in item.name.lower():
            item.add_marker(pytest.mark.i2c)
        if "canfd" in item.name.lower():
            item.add_marker(pytest.mark.canfd)
        if "adc" in item.name.lower():
            item.add_marker(pytest.mark.adc)
        if "pwm" in item.name.lower():
            item.add_marker(pytest.mark.pwm)


def pytest_report_header(config):
    """Add custom header to pytest report."""
    return [
        "PSoC4 Embedded C Integration Tests",
        f"Project: {PROJECT_ROOT}",
        "=" * 60
    ]


# ============================================================================
# Helper functions
# ============================================================================

def assert_in_range(value, min_val, max_val, message=""):
    """Assert that a value is within a specified range."""
    assert min_val <= value <= max_val, (
        f"{message}: {value} not in range [{min_val}, {max_val}]"
    )


def assert_bytes_equal(actual, expected, message=""):
    """Assert that two byte sequences are equal."""
    assert actual == expected, (
        f"{message}: bytes mismatch\n"
        f"Expected: {expected.hex()}\n"
        f"Actual:   {actual.hex()}"
    )


# Export helper functions
pytest.assert_in_range = assert_in_range
pytest.assert_bytes_equal = assert_bytes_equal
