"""
Hardware Abstraction Layer (HAL) Mock Implementation.

This module provides a complete mock of the PSoC4 HAL for testing
embedded C code without requiring actual hardware.
"""

import logging
from typing import Dict, List, Optional, Any
from dataclasses import dataclass, field
from enum import IntEnum

logger = logging.getLogger(__name__)


class ResultCode(IntEnum):
    """PSoC result codes."""
    SUCCESS = 0
    ERROR = 1
    TIMEOUT = 2
    INVALID_PARAM = 3
    BUSY = 4


@dataclass
class HALState:
    """Track the state of the HAL."""
    initialized: bool = False
    error_count: int = 0
    warning_count: int = 0
    operation_log: List[str] = field(default_factory=list)


class HALMock:
    """
    Mock implementation of PSoC4 Hardware Abstraction Layer.

    This class simulates hardware behavior for testing purposes.
    """

    def __init__(self):
        """Initialize the HAL mock."""
        self.state = HALState()
        self.registers: Dict[int, int] = {}
        self.memory: bytearray = bytearray(1024 * 1024)  # 1MB simulated memory
        logger.info("HAL Mock initialized")

    def reset(self):
        """Reset the HAL to initial state."""
        self.state = HALState()
        self.registers.clear()
        self.memory = bytearray(1024 * 1024)
        logger.info("HAL Mock reset")

    def log_operation(self, operation: str):
        """Log an operation for debugging."""
        self.state.operation_log.append(operation)
        logger.debug(f"HAL Operation: {operation}")

    # ========================================================================
    # Board Support Package (BSP) Functions
    # ========================================================================

    def cybsp_init(self) -> int:
        """
        Initialize the board support package.

        Returns:
            ResultCode.SUCCESS on success
        """
        self.log_operation("cybsp_init")
        if self.state.initialized:
            logger.warning("BSP already initialized")
            return ResultCode.ERROR

        self.state.initialized = True
        logger.info("BSP initialized successfully")
        return ResultCode.SUCCESS

    def CY_ASSERT(self, condition: bool):
        """
        Assert a condition (mock implementation).

        Args:
            condition: Condition to assert
        """
        self.log_operation(f"CY_ASSERT({condition})")
        if not condition:
            self.state.error_count += 1
            logger.error("Assertion failed!")
            raise AssertionError("CY_ASSERT failed")

    # ========================================================================
    # Memory Operations
    # ========================================================================

    def read_memory(self, address: int, size: int) -> bytes:
        """
        Read from simulated memory.

        Args:
            address: Memory address
            size: Number of bytes to read

        Returns:
            Bytes read from memory
        """
        self.log_operation(f"read_memory(0x{address:08X}, {size})")
        if address + size > len(self.memory):
            raise ValueError(f"Memory read out of bounds: 0x{address:08X}")
        return bytes(self.memory[address:address + size])

    def write_memory(self, address: int, data: bytes) -> int:
        """
        Write to simulated memory.

        Args:
            address: Memory address
            data: Data to write

        Returns:
            ResultCode
        """
        self.log_operation(f"write_memory(0x{address:08X}, {len(data)} bytes)")
        if address + len(data) > len(self.memory):
            raise ValueError(f"Memory write out of bounds: 0x{address:08X}")

        self.memory[address:address + len(data)] = data
        return ResultCode.SUCCESS

    # ========================================================================
    # Register Operations
    # ========================================================================

    def read_register(self, address: int) -> int:
        """
        Read a hardware register.

        Args:
            address: Register address

        Returns:
            Register value
        """
        self.log_operation(f"read_register(0x{address:08X})")
        return self.registers.get(address, 0)

    def write_register(self, address: int, value: int) -> int:
        """
        Write to a hardware register.

        Args:
            address: Register address
            value: Value to write

        Returns:
            ResultCode
        """
        self.log_operation(f"write_register(0x{address:08X}, 0x{value:08X})")
        self.registers[address] = value & 0xFFFFFFFF
        return ResultCode.SUCCESS

    # ========================================================================
    # Utility Functions
    # ========================================================================

    def get_operation_count(self) -> int:
        """Get the number of operations performed."""
        return len(self.state.operation_log)

    def get_operations(self) -> List[str]:
        """Get the list of operations performed."""
        return self.state.operation_log.copy()

    def clear_operations(self):
        """Clear the operation log."""
        self.state.operation_log.clear()

    def inject_error(self, error_type: str):
        """
        Inject an error for testing error handling.

        Args:
            error_type: Type of error to inject
        """
        self.state.error_count += 1
        logger.warning(f"Injected error: {error_type}")

    def __repr__(self) -> str:
        """String representation of HAL state."""
        return (
            f"HALMock(initialized={self.state.initialized}, "
            f"operations={self.get_operation_count()}, "
            f"errors={self.state.error_count})"
        )
