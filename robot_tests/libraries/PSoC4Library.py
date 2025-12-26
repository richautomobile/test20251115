"""
PSoC4Library - Robot Framework keyword library for PSoC4 system testing.

Provides high-level keywords for system initialization, configuration, and control.
"""

import sys
import time
import logging
from pathlib import Path
from robot.api.deco import keyword, library
from robot.api import logger

# Add parent directory to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent.parent / "tests"))

try:
    from tests.wrappers.system_wrapper import SystemWrapper
    from tests.mocks.hal_mock import HALMock
except ImportError:
    logger.warn("Could not import test mocks. Hardware mode may be required.")
    SystemWrapper = None
    HALMock = None


@library(scope='SUITE', auto_keywords=True)
class PSoC4Library:
    """
    Robot Framework library for PSoC4 system testing.

    This library provides keywords for:
    - System initialization and configuration
    - Firmware control
    - Status monitoring
    - Error handling

    = Examples =

    | Initialize PSoC4 System |
    | Verify System Ready |
    | Run Main Loop | 10 |
    """

    ROBOT_LIBRARY_SCOPE = 'SUITE'
    ROBOT_LIBRARY_VERSION = '1.0.0'

    def __init__(self, mode='mock'):
        """
        Initialize PSoC4Library.

        Args:
            mode: 'mock' for simulation, 'hardware' for real hardware
        """
        self.mode = mode
        self.system = None
        self.hal = None
        self.initialized = False
        logger.info(f"PSoC4Library initialized in {mode} mode")

    @keyword("Initialize PSoC4 System")
    def initialize_system(self):
        """
        Initialize the PSoC4 system.

        Performs complete initialization sequence:
        - Variable initialization
        - Hardware initialization
        - Project initialization

        Example:
        | Initialize PSoC4 System |
        """
        logger.info("Initializing PSoC4 system...")

        if self.mode == 'mock':
            if SystemWrapper is None:
                raise RuntimeError("SystemWrapper not available")
            self.system = SystemWrapper()
            self.hal = HALMock() if HALMock else None
        else:
            # Hardware mode - would use actual hardware interface
            raise NotImplementedError("Hardware mode not yet implemented")

        # Perform initialization
        success = self.system.full_init_sequence()

        if not success:
            raise AssertionError("System initialization failed")

        self.initialized = True
        logger.info("PSoC4 system initialized successfully")

    @keyword("Verify System Ready")
    def verify_system_ready(self):
        """
        Verify that the system is ready for operation.

        Example:
        | Verify System Ready |
        """
        if not self.initialized:
            raise AssertionError("System not initialized")

        if self.mode == 'mock':
            init_count = self.system.get_init_count()
            if init_count != 3:
                raise AssertionError(f"Expected 3 initializations, got {init_count}")

        logger.info("System is ready")

    @keyword("Run Main Loop")
    def run_main_loop(self, cycles=1):
        """
        Run the main loop for specified number of cycles.

        Args:
            cycles: Number of cycles to run (default: 1)

        Example:
        | Run Main Loop | 10 |
        """
        if not self.initialized:
            raise AssertionError("System not initialized")

        cycles = int(cycles)
        logger.info(f"Running main loop for {cycles} cycles...")

        successful = self.system.run_n_cycles(cycles)

        if successful != cycles:
            raise AssertionError(
                f"Only {successful}/{cycles} cycles completed successfully"
            )

        logger.info(f"Main loop completed {cycles} cycles successfully")

    @keyword("Run Single Cycle")
    def run_single_cycle(self):
        """
        Run a single main loop cycle.

        Example:
        | Run Single Cycle |
        """
        if not self.initialized:
            raise AssertionError("System not initialized")

        result = self.system.routine_call()

        if result != 0:
            raise AssertionError(f"Routine call failed with error code: {result}")

        logger.info("Single cycle completed successfully")

    @keyword("Reset System")
    def reset_system(self):
        """
        Reset the system to initial state.

        Example:
        | Reset System |
        """
        logger.info("Resetting system...")

        if self.hal:
            self.hal.reset()

        self.initialized = False
        self.system = None

        logger.info("System reset complete")

    @keyword("Get System State")
    def get_system_state(self):
        """
        Get current system state.

        Returns:
            Dictionary with system state information

        Example:
        | ${state}= | Get System State |
        | Log | ${state} |
        """
        state = {
            'initialized': self.initialized,
            'mode': self.mode,
        }

        if self.mode == 'mock' and self.system:
            state['init_count'] = self.system.get_init_count()
            state['routine_count'] = self.system.get_routine_count()

        logger.info(f"System state: {state}")
        return state

    @keyword("Sleep For")
    def sleep_for(self, seconds):
        """
        Sleep for specified duration.

        Args:
            seconds: Duration to sleep in seconds

        Example:
        | Sleep For | 1.5 |
        """
        seconds = float(seconds)
        logger.info(f"Sleeping for {seconds} seconds...")
        time.sleep(seconds)

    @keyword("System Should Be Initialized")
    def system_should_be_initialized(self):
        """
        Verify that system is initialized.

        Fails if system is not initialized.

        Example:
        | System Should Be Initialized |
        """
        if not self.initialized:
            raise AssertionError("System is not initialized")
        logger.info("System is initialized")

    @keyword("Get Operation Count")
    def get_operation_count(self):
        """
        Get the number of operations performed (mock mode only).

        Returns:
            Number of operations

        Example:
        | ${count}= | Get Operation Count |
        """
        if self.mode != 'mock' or not self.hal:
            logger.warn("Operation count only available in mock mode")
            return 0

        count = self.hal.get_operation_count()
        logger.info(f"Operation count: {count}")
        return count
