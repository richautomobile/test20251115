"""
Integration tests for system-level functionality.

These tests verify the system initialization sequence and main loop behavior.
"""

import pytest
import sys
from pathlib import Path

# Add parent directory to path
sys.path.insert(0, str(Path(__file__).parent.parent))

from wrappers.system_wrapper import SystemWrapper
from mocks.hal_mock import HALMock


class TestSystemInitialization:
    """Test system initialization sequence."""

    @pytest.fixture
    def system(self):
        """Provide a SystemWrapper instance."""
        return SystemWrapper()  # Mock mode

    @pytest.mark.integration
    @pytest.mark.system
    def test_variable_init_succeeds(self, system):
        """Test that variable initialization succeeds."""
        result = system.variable_init()
        assert result == 0, "Variable initialization should return success"

    @pytest.mark.integration
    @pytest.mark.system
    def test_hw_init_succeeds(self, system):
        """Test that hardware initialization succeeds."""
        result = system.hw_init()
        assert result == 0, "Hardware initialization should return success"

    @pytest.mark.integration
    @pytest.mark.system
    def test_project_init_succeeds(self, system):
        """Test that project initialization succeeds."""
        result = system.project_init()
        assert result == 0, "Project initialization should return success"

    @pytest.mark.integration
    @pytest.mark.system
    def test_full_init_sequence(self, system):
        """Test complete initialization sequence."""
        assert system.full_init_sequence(), "Full initialization should succeed"
        assert system.get_init_count() == 3, "Should have 3 initialization calls"

    @pytest.mark.integration
    @pytest.mark.system
    def test_init_order_matters(self, system):
        """Test that initialization order is correct."""
        # Variables should be initialized first
        assert system.variable_init() == 0

        # Then hardware
        assert system.hw_init() == 0

        # Finally project-specific init
        assert system.project_init() == 0


class TestSystemMainLoop:
    """Test system main loop functionality."""

    @pytest.fixture
    def initialized_system(self):
        """Provide an initialized SystemWrapper."""
        system = SystemWrapper()
        system.full_init_sequence()
        return system

    @pytest.mark.integration
    @pytest.mark.system
    def test_single_routine_call(self, initialized_system):
        """Test a single routine call."""
        result = initialized_system.routine_call()
        assert result == 0, "Routine call should succeed"

    @pytest.mark.integration
    @pytest.mark.system
    def test_multiple_routine_calls(self, initialized_system):
        """Test multiple routine calls."""
        for i in range(100):
            result = initialized_system.routine_call()
            assert result == 0, f"Routine call {i} should succeed"

    @pytest.mark.integration
    @pytest.mark.system
    def test_run_n_cycles(self, initialized_system):
        """Test running N cycles."""
        cycles = 50
        successful = initialized_system.run_n_cycles(cycles)
        assert successful == cycles, f"All {cycles} cycles should succeed"

    @pytest.mark.integration
    @pytest.mark.system
    @pytest.mark.slow
    def test_extended_runtime(self, initialized_system):
        """Test extended runtime (stress test)."""
        cycles = 10000
        successful = initialized_system.run_n_cycles(cycles)
        assert successful == cycles, f"All {cycles} cycles should succeed"


class TestSystemWithHAL:
    """Test system integration with HAL mock."""

    @pytest.fixture
    def hal(self):
        """Provide HAL mock."""
        return HALMock()

    @pytest.fixture
    def system_with_hal(self, hal):
        """Provide system with HAL."""
        system = SystemWrapper()
        return system, hal

    @pytest.mark.integration
    @pytest.mark.system
    @pytest.mark.simulation
    def test_bsp_initialization(self, system_with_hal):
        """Test BSP initialization through HAL."""
        system, hal = system_with_hal

        # Initialize BSP
        result = hal.cybsp_init()
        assert result == 0, "BSP init should succeed"
        assert hal.state.initialized, "HAL should be marked as initialized"

    @pytest.mark.integration
    @pytest.mark.system
    @pytest.mark.simulation
    def test_system_with_hal_tracking(self, system_with_hal):
        """Test system operations with HAL operation tracking."""
        system, hal = system_with_hal

        # Clear any existing operations
        hal.clear_operations()

        # Perform initialization
        hal.cybsp_init()

        # Verify operations were logged
        operations = hal.get_operations()
        assert len(operations) > 0, "HAL operations should be logged"
        assert "cybsp_init" in operations[0]

    @pytest.mark.integration
    @pytest.mark.system
    @pytest.mark.simulation
    def test_memory_operations(self, hal):
        """Test memory read/write through HAL."""
        test_data = b"Hello PSoC4!"
        address = 0x1000

        # Write data
        result = hal.write_memory(address, test_data)
        assert result == 0, "Memory write should succeed"

        # Read back data
        read_data = hal.read_memory(address, len(test_data))
        assert read_data == test_data, "Read data should match written data"

    @pytest.mark.integration
    @pytest.mark.system
    @pytest.mark.simulation
    def test_register_operations(self, hal):
        """Test register read/write through HAL."""
        address = 0x40000000
        test_value = 0xDEADBEEF

        # Write register
        result = hal.write_register(address, test_value)
        assert result == 0, "Register write should succeed"

        # Read register
        value = hal.read_register(address)
        assert value == test_value, "Register value should match"


class TestErrorHandling:
    """Test error handling in system initialization."""

    @pytest.mark.integration
    @pytest.mark.system
    def test_hal_reinitialization_error(self):
        """Test that reinitializing HAL produces error."""
        hal = HALMock()

        # First init should succeed
        assert hal.cybsp_init() == 0

        # Second init should fail
        assert hal.cybsp_init() != 0

    @pytest.mark.integration
    @pytest.mark.system
    def test_memory_bounds_checking(self):
        """Test memory bounds checking."""
        hal = HALMock()

        # Try to read beyond memory bounds
        with pytest.raises(ValueError):
            hal.read_memory(len(hal.memory) + 100, 10)

        # Try to write beyond memory bounds
        with pytest.raises(ValueError):
            hal.write_memory(len(hal.memory) + 100, b"test")


@pytest.mark.integration
@pytest.mark.system
@pytest.mark.smoke
class TestSmokeTests:
    """Quick smoke tests for basic functionality."""

    def test_system_creation(self):
        """Test that system wrapper can be created."""
        system = SystemWrapper()
        assert system is not None

    def test_hal_creation(self):
        """Test that HAL mock can be created."""
        hal = HALMock()
        assert hal is not None

    def test_basic_init_flow(self):
        """Test basic initialization flow."""
        system = SystemWrapper()
        assert system.full_init_sequence()

    def test_basic_routine_call(self):
        """Test basic routine call."""
        system = SystemWrapper()
        system.full_init_sequence()
        assert system.routine_call() == 0
