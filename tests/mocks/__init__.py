"""
Mock modules for hardware abstraction layer testing.
"""

from .hal_mock import HALMock
from .peripheral_mocks import (
    I2CMock,
    SPIMock,
    ADCMock,
    PWMMock,
    CANFDMock,
    FlashMock,
    GPIOMock
)

__all__ = [
    'HALMock',
    'I2CMock',
    'SPIMock',
    'ADCMock',
    'PWMMock',
    'CANFDMock',
    'FlashMock',
    'GPIOMock'
]
