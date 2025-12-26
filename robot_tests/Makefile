# Makefile for Robot Framework Tests

ROBOT := robot
PABOT := pabot
OUTPUT_DIR := results
TEST_DIR := test_suites

.PHONY: help install test test-smoke test-system test-peripherals test-i2c test-adc \
        test-all test-critical test-parallel clean report metrics

help:
	@echo "PSoC4 Robot Framework Test Makefile"
	@echo "===================================="
	@echo ""
	@echo "Available targets:"
	@echo "  install          - Install dependencies"
	@echo "  test             - Run all tests"
	@echo "  test-smoke       - Run smoke tests"
	@echo "  test-system      - Run system qualification tests"
	@echo "  test-peripherals - Run peripheral tests"
	@echo "  test-i2c         - Run I2C tests"
	@echo "  test-adc         - Run ADC tests"
	@echo "  test-critical    - Run critical tests only"
	@echo "  test-parallel    - Run tests in parallel"
	@echo "  report           - Open HTML report"
	@echo "  metrics          - Generate test metrics"
	@echo "  clean            - Clean test artifacts"
	@echo ""

install:
	@echo "Installing Robot Framework dependencies..."
	pip install -r requirements.txt

test:
	@echo "Running all tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) $(TEST_DIR)/

test-smoke:
	@echo "Running smoke tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) --include smoke $(TEST_DIR)/

test-system:
	@echo "Running system tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) $(TEST_DIR)/system/

test-peripherals:
	@echo "Running peripheral tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) $(TEST_DIR)/peripherals/

test-i2c:
	@echo "Running I2C tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) $(TEST_DIR)/peripherals/i2c_qualification.robot

test-adc:
	@echo "Running ADC tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) $(TEST_DIR)/peripherals/adc_qualification.robot

test-acceptance:
	@echo "Running acceptance tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) $(TEST_DIR)/acceptance/

test-critical:
	@echo "Running critical tests only..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) --include critical $(TEST_DIR)/

test-qualification:
	@echo "Running qualification tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) --include qualification $(TEST_DIR)/

test-parallel:
	@echo "Running tests in parallel..."
	$(PABOT) --processes 4 --outputdir $(OUTPUT_DIR) $(TEST_DIR)/

test-verbose:
	@echo "Running tests with verbose output..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) --loglevel DEBUG $(TEST_DIR)/

report:
	@echo "Opening test report..."
	@which xdg-open > /dev/null && xdg-open $(OUTPUT_DIR)/report.html || \
	which open > /dev/null && open $(OUTPUT_DIR)/report.html || \
	echo "Please open $(OUTPUT_DIR)/report.html manually"

log:
	@echo "Opening test log..."
	@which xdg-open > /dev/null && xdg-open $(OUTPUT_DIR)/log.html || \
	which open > /dev/null && open $(OUTPUT_DIR)/log.html || \
	echo "Please open $(OUTPUT_DIR)/log.html manually"

metrics:
	@echo "Generating test metrics..."
	@which robotmetrics > /dev/null && robotmetrics --inputpath $(OUTPUT_DIR)/output.xml || \
	echo "robotmetrics not installed. Install with: pip install robotframework-metrics"

clean:
	@echo "Cleaning test artifacts..."
	rm -rf $(OUTPUT_DIR)
	rm -rf logs
	rm -rf __pycache__
	rm -rf libraries/__pycache__
	find . -type f -name "*.pyc" -delete
	find . -type d -name "__pycache__" -delete
	@echo "Clean complete"

libdoc:
	@echo "Generating library documentation..."
	python -m robot.libdoc libraries/PSoC4Library.py $(OUTPUT_DIR)/PSoC4Library.html
	python -m robot.libdoc libraries/PeripheralLibrary.py $(OUTPUT_DIR)/PeripheralLibrary.html
	@echo "Library documentation generated"

# Quick test (smoke + critical)
quick:
	@echo "Running quick tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) --include smokeORcritical $(TEST_DIR)/

# Rerun failed tests
rerun:
	@echo "Rerunning failed tests..."
	$(ROBOT) --outputdir $(OUTPUT_DIR) --rerunfailed $(OUTPUT_DIR)/output.xml $(TEST_DIR)/

# CI target
ci: install test-smoke test-critical
	@echo "CI pipeline complete"
