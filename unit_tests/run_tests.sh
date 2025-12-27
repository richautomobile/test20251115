#!/bin/bash
# Unit Test Runner Script

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo "================================================"
echo "PSoC4 Unit Test Runner"
echo "================================================"

# Parse command line arguments
TEST_TARGET=${1:-"all"}

echo ""
echo -e "${BLUE}Test Target: ${TEST_TARGET}${NC}"
echo "------------------------------------------------"
echo ""

case $TEST_TARGET in
    "all")
        echo "Running all unit tests..."
        make test
        ;;
    "i2c")
        echo "Running I2C unit tests..."
        make test-i2c
        ;;
    "adc")
        echo "Running ADC unit tests..."
        make test-adc
        ;;
    "gpio")
        echo "Running GPIO unit tests..."
        make test-gpio
        ;;
    "system")
        echo "Running System unit tests..."
        make test-system
        ;;
    "coverage")
        echo "Generating code coverage..."
        make coverage
        ;;
    "clean")
        echo "Cleaning build artifacts..."
        make clean
        ;;
    "build")
        echo "Building all tests..."
        make build
        ;;
    "ci")
        echo "Running CI pipeline..."
        make ci
        ;;
    *)
        echo -e "${RED}Unknown test target: $TEST_TARGET${NC}"
        echo ""
        echo "Usage: $0 [target]"
        echo ""
        echo "Available targets:"
        echo "  all      - Run all tests (default)"
        echo "  i2c      - Run I2C tests only"
        echo "  adc      - Run ADC tests only"
        echo "  gpio     - Run GPIO tests only"
        echo "  system   - Run system tests only"
        echo "  coverage - Generate code coverage report"
        echo "  build    - Build all tests"
        echo "  clean    - Clean build artifacts"
        echo "  ci       - Run CI pipeline (clean + test + coverage)"
        exit 1
        ;;
esac

# Check result
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}✓ Tests completed successfully!${NC}"
    echo "================================================"
    exit 0
else
    echo ""
    echo -e "${RED}✗ Tests failed!${NC}"
    echo "================================================"
    exit 1
fi
