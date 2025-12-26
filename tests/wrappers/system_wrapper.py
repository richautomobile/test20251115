"""
Python wrapper for system-level C functions.

This module provides Python bindings for testing the system initialization
and routine functions.
"""

import ctypes
import logging
from pathlib import Path
from typing import Optional

logger = logging.getLogger(__name__)


class SystemWrapper:
    """
    Wrapper for system C functions.

    This class provides Python access to the system initialization
    and main loop functions for integration testing.
    """

    def __init__(self, lib_path: Optional[str] = None):
        """
        Initialize the system wrapper.

        Args:
            lib_path: Path to the compiled shared library (.so/.dll)
                     If None, operates in mock mode
        """
        self.lib = None
        self.mock_mode = lib_path is None

        if not self.mock_mode:
            try:
                self.lib = ctypes.CDLL(lib_path)
                self._setup_function_signatures()
                logger.info(f"Loaded C library: {lib_path}")
            except Exception as e:
                logger.warning(f"Failed to load C library: {e}. Using mock mode.")
                self.mock_mode = True

        if self.mock_mode:
            logger.info("SystemWrapper running in mock mode")
            self._init_count = 0
            self._routine_count = 0

    def _setup_function_signatures(self):
        """Setup C function signatures."""
        if self.lib is None:
            return

        # Sys_VariableInit() -> int
        self.lib.Sys_VariableInit.restype = ctypes.c_int
        self.lib.Sys_VariableInit.argtypes = []

        # Sys_HwInit() -> int
        self.lib.Sys_HwInit.restype = ctypes.c_int
        self.lib.Sys_HwInit.argtypes = []

        # Sys_ProjectInit() -> int
        self.lib.Sys_ProjectInit.restype = ctypes.c_int
        self.lib.Sys_ProjectInit.argtypes = []

        # Sys_RoutineCall() -> int
        self.lib.Sys_RoutineCall.restype = ctypes.c_int
        self.lib.Sys_RoutineCall.argtypes = []

    # ========================================================================
    # System Functions
    # ========================================================================

    def variable_init(self) -> int:
        """
        Initialize system variables.

        Returns:
            0 on success, non-zero on error
        """
        if self.mock_mode:
            logger.debug("Mock: Sys_VariableInit()")
            self._init_count += 1
            return 0

        result = self.lib.Sys_VariableInit()
        logger.debug(f"Sys_VariableInit() -> {result}")
        return result

    def hw_init(self) -> int:
        """
        Initialize hardware.

        Returns:
            0 on success, non-zero on error
        """
        if self.mock_mode:
            logger.debug("Mock: Sys_HwInit()")
            self._init_count += 1
            return 0

        result = self.lib.Sys_HwInit()
        logger.debug(f"Sys_HwInit() -> {result}")
        return result

    def project_init(self) -> int:
        """
        Initialize project-specific functionality.

        Returns:
            0 on success, non-zero on error
        """
        if self.mock_mode:
            logger.debug("Mock: Sys_ProjectInit()")
            self._init_count += 1
            return 0

        result = self.lib.Sys_ProjectInit()
        logger.debug(f"Sys_ProjectInit() -> {result}")
        return result

    def routine_call(self) -> int:
        """
        Call the main routine (one iteration of main loop).

        Returns:
            0 on success, non-zero on error
        """
        if self.mock_mode:
            logger.debug("Mock: Sys_RoutineCall()")
            self._routine_count += 1
            return 0

        result = self.lib.Sys_RoutineCall()
        logger.debug(f"Sys_RoutineCall() -> {result}")
        return result

    # ========================================================================
    # Test Utilities
    # ========================================================================

    def full_init_sequence(self) -> bool:
        """
        Perform complete initialization sequence.

        Returns:
            True if all initializations succeeded
        """
        try:
            if self.variable_init() != 0:
                logger.error("Variable initialization failed")
                return False

            if self.hw_init() != 0:
                logger.error("Hardware initialization failed")
                return False

            if self.project_init() != 0:
                logger.error("Project initialization failed")
                return False

            logger.info("Full initialization sequence completed successfully")
            return True

        except Exception as e:
            logger.error(f"Initialization sequence failed: {e}")
            return False

    def run_n_cycles(self, n: int) -> int:
        """
        Run N main loop cycles.

        Args:
            n: Number of cycles to run

        Returns:
            Number of successful cycles
        """
        successful = 0
        for i in range(n):
            if self.routine_call() == 0:
                successful += 1
            else:
                logger.warning(f"Routine call {i} failed")
                break

        return successful

    def get_init_count(self) -> int:
        """Get initialization count (mock mode only)."""
        return self._init_count if self.mock_mode else -1

    def get_routine_count(self) -> int:
        """Get routine call count (mock mode only)."""
        return self._routine_count if self.mock_mode else -1
