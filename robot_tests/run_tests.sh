#!/bin/bash
# Robot Framework Test Runner Script

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo "================================================"
echo "PSoC4 Robot Framework Test Runner"
echo "================================================"

# Check if Robot Framework is installed
if ! command -v robot &> /dev/null; then
    echo -e "${YELLOW}Warning: Robot Framework not found. Installing...${NC}"
    pip install -r requirements.txt
fi

# Default values
TEST_SUITE=${1:-"all"}
OUTPUT_DIR="results"
LOG_LEVEL="INFO"

# Create output directory
mkdir -p ${OUTPUT_DIR}

echo ""
echo -e "${BLUE}Test Suite: ${TEST_SUITE}${NC}"
echo -e "${BLUE}Output Directory: ${OUTPUT_DIR}${NC}"
echo "------------------------------------------------"
echo ""

# Function to run robot tests
run_robot_tests() {
    local suite_path=$1
    local tags=$2
    local name=$3

    echo -e "${GREEN}Running ${name}...${NC}"

    if [ -z "$tags" ]; then
        robot --outputdir ${OUTPUT_DIR} \
              --loglevel ${LOG_LEVEL} \
              --report report.html \
              --log log.html \
              ${suite_path}
    else
        robot --outputdir ${OUTPUT_DIR} \
              --loglevel ${LOG_LEVEL} \
              --include ${tags} \
              --report report.html \
              --log log.html \
              ${suite_path}
    fi
}

# Execute tests based on selection
case $TEST_SUITE in
    "all")
        echo "Running all test suites..."
        robot --outputdir ${OUTPUT_DIR} \
              --loglevel ${LOG_LEVEL} \
              test_suites/
        ;;
    "smoke")
        echo "Running smoke tests..."
        run_robot_tests "test_suites/" "smoke" "Smoke Tests"
        ;;
    "system")
        echo "Running system tests..."
        run_robot_tests "test_suites/system/" "" "System Tests"
        ;;
    "peripherals")
        echo "Running peripheral tests..."
        run_robot_tests "test_suites/peripherals/" "" "Peripheral Tests"
        ;;
    "i2c")
        echo "Running I2C tests..."
        run_robot_tests "test_suites/peripherals/i2c_qualification.robot" "" "I2C Tests"
        ;;
    "adc")
        echo "Running ADC tests..."
        run_robot_tests "test_suites/peripherals/adc_qualification.robot" "" "ADC Tests"
        ;;
    "acceptance")
        echo "Running acceptance tests..."
        run_robot_tests "test_suites/acceptance/" "" "Acceptance Tests"
        ;;
    "qualification")
        echo "Running qualification tests..."
        run_robot_tests "test_suites/" "qualification" "Qualification Tests"
        ;;
    "critical")
        echo "Running critical tests..."
        run_robot_tests "test_suites/" "critical" "Critical Tests"
        ;;
    "parallel")
        echo "Running tests in parallel..."
        pabot --processes 4 \
              --outputdir ${OUTPUT_DIR} \
              test_suites/
        ;;
    *)
        echo -e "${RED}Unknown test suite: $TEST_SUITE${NC}"
        echo ""
        echo "Usage: $0 [suite]"
        echo ""
        echo "Available test suites:"
        echo "  all           - Run all tests"
        echo "  smoke         - Run smoke tests only"
        echo "  system        - Run system qualification tests"
        echo "  peripherals   - Run all peripheral tests"
        echo "  i2c           - Run I2C tests"
        echo "  adc           - Run ADC tests"
        echo "  acceptance    - Run acceptance tests"
        echo "  qualification - Run qualification tests"
        echo "  critical      - Run critical tests only"
        echo "  parallel      - Run tests in parallel"
        exit 1
        ;;
esac

# Check result
if [ $? -eq 0 ]; then
    echo ""
    echo -e "${GREEN}✓ Tests completed successfully!${NC}"
    echo ""
    echo "Reports generated:"
    echo "  - ${OUTPUT_DIR}/report.html"
    echo "  - ${OUTPUT_DIR}/log.html"
    echo "  - ${OUTPUT_DIR}/output.xml"
    echo "================================================"
    exit 0
else
    echo ""
    echo -e "${RED}✗ Some tests failed!${NC}"
    echo "================================================"
    exit 1
fi
