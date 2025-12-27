"""
Mock implementations for specific peripherals (I2C, SPI, ADC, PWM, etc.).
"""

import logging
from typing import Dict, List, Optional
from dataclasses import dataclass, field
from collections import deque

logger = logging.getLogger(__name__)


# ============================================================================
# I2C Mock
# ============================================================================

@dataclass
class I2CTransaction:
    """Record of an I2C transaction."""
    device_addr: int
    register_addr: Optional[int]
    data: bytes
    is_write: bool


class I2CMock:
    """Mock I2C peripheral."""

    def __init__(self, bus_id: int = 0):
        """
        Initialize I2C mock.

        Args:
            bus_id: I2C bus identifier
        """
        self.bus_id = bus_id
        self.initialized = False
        self.speed = 100000  # Default 100kHz
        self.devices: Dict[int, bytearray] = {}  # Device address -> memory
        self.transactions: List[I2CTransaction] = []
        logger.info(f"I2C Mock {bus_id} created")

    def init(self, speed: int = 100000) -> int:
        """Initialize I2C bus."""
        self.initialized = True
        self.speed = speed
        logger.info(f"I2C {self.bus_id} initialized at {speed}Hz")
        return 0

    def add_device(self, address: int, memory_size: int = 256):
        """Add a simulated I2C device."""
        self.devices[address] = bytearray(memory_size)
        logger.info(f"Added I2C device at address 0x{address:02X}")

    def write(self, device_addr: int, register_addr: int, data: bytes) -> int:
        """Write to I2C device."""
        if not self.initialized:
            logger.error("I2C not initialized")
            return -1

        if device_addr not in self.devices:
            logger.error(f"I2C device 0x{device_addr:02X} not found")
            return -1

        # Write data to device memory
        device_mem = self.devices[device_addr]
        for i, byte in enumerate(data):
            if register_addr + i < len(device_mem):
                device_mem[register_addr + i] = byte

        self.transactions.append(
            I2CTransaction(device_addr, register_addr, data, is_write=True)
        )
        logger.debug(f"I2C write: dev=0x{device_addr:02X}, reg=0x{register_addr:02X}, data={data.hex()}")
        return 0

    def read(self, device_addr: int, register_addr: int, length: int) -> bytes:
        """Read from I2C device."""
        if not self.initialized:
            logger.error("I2C not initialized")
            return bytes()

        if device_addr not in self.devices:
            logger.error(f"I2C device 0x{device_addr:02X} not found")
            return bytes()

        # Read data from device memory
        device_mem = self.devices[device_addr]
        data = bytes(device_mem[register_addr:register_addr + length])

        self.transactions.append(
            I2CTransaction(device_addr, register_addr, data, is_write=False)
        )
        logger.debug(f"I2C read: dev=0x{device_addr:02X}, reg=0x{register_addr:02X}, data={data.hex()}")
        return data

    def get_transactions(self) -> List[I2CTransaction]:
        """Get all I2C transactions."""
        return self.transactions.copy()

    def clear_transactions(self):
        """Clear transaction history."""
        self.transactions.clear()


# ============================================================================
# SPI Mock
# ============================================================================

class SPIMock:
    """Mock SPI peripheral."""

    def __init__(self, bus_id: int = 0):
        """Initialize SPI mock."""
        self.bus_id = bus_id
        self.initialized = False
        self.speed = 1000000  # Default 1MHz
        self.mode = 0
        self.rx_buffer: deque = deque()
        self.tx_log: List[bytes] = []
        logger.info(f"SPI Mock {bus_id} created")

    def init(self, speed: int = 1000000, mode: int = 0) -> int:
        """Initialize SPI bus."""
        self.initialized = True
        self.speed = speed
        self.mode = mode
        logger.info(f"SPI {self.bus_id} initialized at {speed}Hz, mode {mode}")
        return 0

    def transfer(self, tx_data: bytes) -> bytes:
        """Transfer data over SPI (full-duplex)."""
        if not self.initialized:
            logger.error("SPI not initialized")
            return bytes()

        self.tx_log.append(tx_data)

        # Generate mock RX data (echo or from buffer)
        if self.rx_buffer:
            rx_data = bytes([self.rx_buffer.popleft() for _ in range(len(tx_data))])
        else:
            rx_data = tx_data  # Echo by default

        logger.debug(f"SPI transfer: TX={tx_data.hex()}, RX={rx_data.hex()}")
        return rx_data

    def queue_rx_data(self, data: bytes):
        """Queue data to be returned in next transfer."""
        self.rx_buffer.extend(data)


# ============================================================================
# ADC Mock
# ============================================================================

class ADCMock:
    """Mock ADC peripheral."""

    def __init__(self, channel: int = 0, resolution: int = 12):
        """Initialize ADC mock."""
        self.channel = channel
        self.resolution = resolution
        self.max_value = (1 << resolution) - 1
        self.initialized = False
        self.samples: List[int] = []
        self.current_value = self.max_value // 2  # Mid-range default
        logger.info(f"ADC Mock channel {channel} created ({resolution}-bit)")

    def init(self) -> int:
        """Initialize ADC."""
        self.initialized = True
        logger.info(f"ADC channel {self.channel} initialized")
        return 0

    def read(self) -> int:
        """Read ADC value."""
        if not self.initialized:
            logger.error("ADC not initialized")
            return 0

        self.samples.append(self.current_value)
        logger.debug(f"ADC read: {self.current_value}")
        return self.current_value

    def set_value(self, value: int):
        """Set the ADC reading value (for testing)."""
        if value > self.max_value:
            value = self.max_value
        if value < 0:
            value = 0
        self.current_value = value

    def set_voltage(self, voltage: float, vref: float = 3.3):
        """Set ADC value based on voltage."""
        value = int((voltage / vref) * self.max_value)
        self.set_value(value)


# ============================================================================
# PWM Mock
# ============================================================================

class PWMMock:
    """Mock PWM peripheral."""

    def __init__(self, channel: int = 0):
        """Initialize PWM mock."""
        self.channel = channel
        self.initialized = False
        self.running = False
        self.frequency = 1000  # Hz
        self.duty_cycle = 0  # 0-100%
        self.period = 1000  # counts
        self.compare = 0  # compare value
        logger.info(f"PWM Mock channel {channel} created")

    def init(self, frequency: int = 1000, duty: int = 50) -> int:
        """Initialize PWM."""
        self.initialized = True
        self.frequency = frequency
        self.duty_cycle = duty
        self.compare = int(self.period * duty / 100)
        logger.info(f"PWM {self.channel} initialized: {frequency}Hz, {duty}% duty")
        return 0

    def start(self) -> int:
        """Start PWM output."""
        if not self.initialized:
            logger.error("PWM not initialized")
            return -1

        self.running = True
        logger.info(f"PWM {self.channel} started")
        return 0

    def stop(self) -> int:
        """Stop PWM output."""
        self.running = False
        logger.info(f"PWM {self.channel} stopped")
        return 0

    def set_duty(self, duty: int) -> int:
        """Set PWM duty cycle (0-100%)."""
        if duty < 0 or duty > 100:
            logger.error(f"Invalid duty cycle: {duty}")
            return -1

        self.duty_cycle = duty
        self.compare = int(self.period * duty / 100)
        logger.debug(f"PWM {self.channel} duty set to {duty}%")
        return 0


# ============================================================================
# CAN-FD Mock
# ============================================================================

@dataclass
class CANFrame:
    """CAN-FD frame."""
    can_id: int
    data: bytes
    dlc: int
    flags: int = 0


class CANFDMock:
    """Mock CAN-FD peripheral."""

    def __init__(self, bus_id: int = 0):
        """Initialize CAN-FD mock."""
        self.bus_id = bus_id
        self.initialized = False
        self.baudrate = 500000
        self.tx_frames: List[CANFrame] = []
        self.rx_queue: deque = deque()
        logger.info(f"CAN-FD Mock {bus_id} created")

    def init(self, baudrate: int = 500000) -> int:
        """Initialize CAN-FD."""
        self.initialized = True
        self.baudrate = baudrate
        logger.info(f"CAN-FD {self.bus_id} initialized at {baudrate} bps")
        return 0

    def send(self, can_id: int, data: bytes, flags: int = 0) -> int:
        """Send CAN frame."""
        if not self.initialized:
            logger.error("CAN-FD not initialized")
            return -1

        frame = CANFrame(can_id, data, len(data), flags)
        self.tx_frames.append(frame)
        logger.debug(f"CAN-FD TX: ID=0x{can_id:03X}, data={data.hex()}")
        return 0

    def receive(self) -> Optional[CANFrame]:
        """Receive CAN frame."""
        if not self.initialized:
            logger.error("CAN-FD not initialized")
            return None

        if not self.rx_queue:
            return None

        frame = self.rx_queue.popleft()
        logger.debug(f"CAN-FD RX: ID=0x{frame.can_id:03X}, data={frame.data.hex()}")
        return frame

    def queue_rx_frame(self, can_id: int, data: bytes, flags: int = 0):
        """Queue a frame for reception (testing)."""
        frame = CANFrame(can_id, data, len(data), flags)
        self.rx_queue.append(frame)


# ============================================================================
# Flash Memory Mock
# ============================================================================

class FlashMock:
    """Mock Flash memory."""

    def __init__(self, size: int = 64 * 1024, page_size: int = 256):
        """Initialize Flash mock."""
        self.size = size
        self.page_size = page_size
        self.memory = bytearray([0xFF] * size)  # Erased state
        self.write_count = 0
        self.erase_count = 0
        logger.info(f"Flash Mock created: {size} bytes, {page_size}-byte pages")

    def write(self, address: int, data: bytes) -> int:
        """Write to flash."""
        if address + len(data) > self.size:
            logger.error("Flash write out of bounds")
            return -1

        self.memory[address:address + len(data)] = data
        self.write_count += 1
        logger.debug(f"Flash write: addr=0x{address:06X}, len={len(data)}")
        return 0

    def read(self, address: int, length: int) -> bytes:
        """Read from flash."""
        if address + length > self.size:
            logger.error("Flash read out of bounds")
            return bytes()

        data = bytes(self.memory[address:address + length])
        logger.debug(f"Flash read: addr=0x{address:06X}, len={length}")
        return data

    def erase_page(self, page_num: int) -> int:
        """Erase a flash page."""
        start = page_num * self.page_size
        if start >= self.size:
            logger.error("Flash erase out of bounds")
            return -1

        end = min(start + self.page_size, self.size)
        self.memory[start:end] = bytearray([0xFF] * (end - start))
        self.erase_count += 1
        logger.debug(f"Flash erase: page {page_num}")
        return 0


# ============================================================================
# GPIO Mock
# ============================================================================

class GPIOMock:
    """Mock GPIO peripheral."""

    def __init__(self):
        """Initialize GPIO mock."""
        self.pins: Dict[int, bool] = {}
        self.directions: Dict[int, str] = {}
        logger.info("GPIO Mock created")

    def init(self, pin: int, direction: str = "output") -> int:
        """Initialize a GPIO pin."""
        self.pins[pin] = False
        self.directions[pin] = direction
        logger.debug(f"GPIO pin {pin} initialized as {direction}")
        return 0

    def write(self, pin: int, value: bool) -> int:
        """Write to GPIO pin."""
        if pin not in self.pins:
            logger.error(f"GPIO pin {pin} not initialized")
            return -1

        if self.directions.get(pin) != "output":
            logger.error(f"GPIO pin {pin} not configured as output")
            return -1

        self.pins[pin] = value
        logger.debug(f"GPIO pin {pin} = {value}")
        return 0

    def read(self, pin: int) -> Optional[bool]:
        """Read from GPIO pin."""
        if pin not in self.pins:
            logger.error(f"GPIO pin {pin} not initialized")
            return None

        value = self.pins[pin]
        logger.debug(f"GPIO pin {pin} read: {value}")
        return value

    def toggle(self, pin: int) -> int:
        """Toggle GPIO pin."""
        if pin not in self.pins:
            return -1

        self.pins[pin] = not self.pins[pin]
        return 0
