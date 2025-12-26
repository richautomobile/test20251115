#!/bin/bash
# Test runner script for PSoC4 integration tests

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo "==============================================="
echo "PSoC4 Integration Test Runner"
echo "==============================================="

# Check if Python is installed
if ! command -v python3 &> /dev/null; then
    echo -e "${RED}Error: Python 3 is not installed${NC}"
    exit 1
fi

# Check if pytest is installed
if ! python3 -c "import pytest" &> /dev/null; then
    echo -e "${YELLOW}Warning: pytest not found. Installing dependencies...${NC}"
    python3 -m pip install -r requirements.txt
fi

# Parse command line arguments
TEST_CATEGORY=${1:-"all"}

echo ""
echo "Test Category: $TEST_CATEGORY"
echo "-----------------------------------------------"
echo ""

case $TEST_CATEGORY in
    "all")
        echo "Running all tests..."
        python3 -m pytest
        ;;
    "unit")
        echo "Running unit tests..."
        python3 -m pytest -m unit
        ;;
    "integration")
        echo "Running integration tests..."
        python3 -m pytest -m integration
        ;;
    "smoke")
        echo "Running smoke tests..."
        python3 -m pytest -m smoke
        ;;
    "i2c")
        echo "Running I2C tests..."
        python3 -m pytest -m i2c
        ;;
    "spi")
        echo "Running SPI tests..."
        python3 -m pytest -m spi
        ;;
    "adc")
        echo "Running ADC tests..."
        python3 -m pytest -m adc
        ;;
    "pwm")
        echo "Running PWM tests..."
        python3 -m pytest -m pwm
        ;;
    "canfd")
        echo "Running CAN-FD tests..."
        python3 -m pytest -m canfd
        ;;
    "gpio")
        echo "Running GPIO tests..."
        python3 -m pytest -m gpio
        ;;
    "flash")
        echo "Running Flash tests..."
        python3 -m pytest -m flash
        ;;
    "system")
        echo "Running system tests..."
        python3 -m pytest -m system
        ;;
    "verbose")
        echo "Running all tests (verbose)..."
        python3 -m pytest -vv
        ;;
    "coverage")
        echo "Running tests with coverage..."
        python3 -m pytest --cov-report=term-missing --cov-report=html
        echo ""
        echo -e "${GREEN}Coverage report generated in htmlcov/index.html${NC}"
        ;;
    *)
        echo -e "${RED}Unknown test category: $TEST_CATEGORY${NC}"
        echo ""
        echo "Usage: $0 [category]"
        echo ""
        echo "Available categories:"
        echo "  all          - Run all tests (default)"
        echo "  unit         - Run unit tests"
        echo "  integration  - Run integration tests"
        echo "  smoke        - Run smoke tests"
        echo "  i2c          - Run I2C tests"
        echo "  spi          - Run SPI tests"
        echo "  adc          - Run ADC tests"
        echo "  pwm          - Run PWM tests"
        echo "  canfd        - Run CAN-FD tests"
        echo "  gpio         - Run GPIO tests"
        echo "  flash        - Run Flash tests"
        echo "  system       - Run system tests"
        echo "  verbose      - Run all tests with verbose output"
        echo "  coverage     - Run tests with coverage report"
        exit 1
        ;;
esac

# Check test result
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}✓ All tests passed!${NC}"
    echo "==============================================="
    exit 0
else
    echo ""
    echo -e "${RED}✗ Some tests failed!${NC}"
    echo "==============================================="
    exit 1
fi
