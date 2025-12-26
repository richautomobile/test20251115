# Makefile for running pytest-based integration tests
# PSoC4 Embedded C Testing Framework

PYTHON := python3
PIP := $(PYTHON) -m pip
PYTEST := $(PYTHON) -m pytest

# Directories
TEST_DIR := .
REPORT_DIR := $(TEST_DIR)/reports
HTMLCOV_DIR := $(TEST_DIR)/htmlcov

# Test categories
UNIT_TESTS := $(TEST_DIR)/unit
INTEGRATION_TESTS := $(TEST_DIR)/integration
ALL_TESTS := $(TEST_DIR)

.PHONY: help install test test-unit test-integration test-all test-verbose \
        test-smoke test-hardware test-simulation coverage clean \
        test-i2c test-spi test-adc test-pwm test-canfd test-gpio test-flash

# Default target
help:
	@echo "PSoC4 Integration Test Makefile"
	@echo "================================"
	@echo ""
	@echo "Available targets:"
	@echo "  install           - Install Python dependencies"
	@echo "  test              - Run all tests (default)"
	@echo "  test-unit         - Run unit tests only"
	@echo "  test-integration  - Run integration tests only"
	@echo "  test-smoke        - Run quick smoke tests"
	@echo "  test-verbose      - Run tests with verbose output"
	@echo "  test-parallel     - Run tests in parallel"
	@echo ""
	@echo "Peripheral-specific tests:"
	@echo "  test-i2c          - Run I2C tests only"
	@echo "  test-spi          - Run SPI tests only"
	@echo "  test-adc          - Run ADC tests only"
	@echo "  test-pwm          - Run PWM tests only"
	@echo "  test-canfd        - Run CAN-FD tests only"
	@echo "  test-gpio         - Run GPIO tests only"
	@echo "  test-flash        - Run Flash tests only"
	@echo ""
	@echo "Other targets:"
	@echo "  coverage          - Generate coverage report"
	@echo "  coverage-html     - Open HTML coverage report"
	@echo "  clean             - Clean test artifacts"
	@echo "  lint              - Run code quality checks"
	@echo ""

# Install dependencies
install:
	@echo "Installing Python dependencies..."
	$(PIP) install -r requirements.txt

# Run all tests
test:
	@echo "Running all tests..."
	$(PYTEST) $(ALL_TESTS)

# Run unit tests
test-unit:
	@echo "Running unit tests..."
	$(PYTEST) -m unit $(UNIT_TESTS)

# Run integration tests
test-integration:
	@echo "Running integration tests..."
	$(PYTEST) -m integration $(INTEGRATION_TESTS)

# Run smoke tests (quick verification)
test-smoke:
	@echo "Running smoke tests..."
	$(PYTEST) -m smoke

# Run tests with verbose output
test-verbose:
	@echo "Running tests (verbose)..."
	$(PYTEST) -vv $(ALL_TESTS)

# Run tests in parallel
test-parallel:
	@echo "Running tests in parallel..."
	$(PYTEST) -n auto $(ALL_TESTS)

# Peripheral-specific tests
test-i2c:
	@echo "Running I2C tests..."
	$(PYTEST) -m i2c

test-spi:
	@echo "Running SPI tests..."
	$(PYTEST) -m spi

test-adc:
	@echo "Running ADC tests..."
	$(PYTEST) -m adc

test-pwm:
	@echo "Running PWM tests..."
	$(PYTEST) -m pwm

test-canfd:
	@echo "Running CAN-FD tests..."
	$(PYTEST) -m canfd

test-gpio:
	@echo "Running GPIO tests..."
	$(PYTEST) -m gpio

test-flash:
	@echo "Running Flash tests..."
	$(PYTEST) -m flash

test-system:
	@echo "Running system tests..."
	$(PYTEST) -m system

# Test simulation vs hardware
test-simulation:
	@echo "Running simulation tests..."
	$(PYTEST) -m simulation

test-hardware:
	@echo "Running hardware tests..."
	$(PYTEST) -m hardware

# Coverage
coverage:
	@echo "Generating coverage report..."
	$(PYTEST) --cov-report=term-missing --cov-report=html

coverage-html: coverage
	@echo "Opening HTML coverage report..."
	@which xdg-open > /dev/null && xdg-open $(HTMLCOV_DIR)/index.html || \
	which open > /dev/null && open $(HTMLCOV_DIR)/index.html || \
	echo "Please open $(HTMLCOV_DIR)/index.html manually"

# Clean artifacts
clean:
	@echo "Cleaning test artifacts..."
	rm -rf $(HTMLCOV_DIR)
	rm -rf .pytest_cache
	rm -rf __pycache__
	rm -rf */__pycache__
	rm -rf */*/__pycache__
	rm -f .coverage
	rm -f coverage.xml
	rm -f report.html
	rm -f assets/
	find . -type f -name "*.pyc" -delete
	find . -type d -name "__pycache__" -delete
	@echo "Clean complete"

# Lint and code quality
lint:
	@echo "Running linters..."
	@which pylint > /dev/null && pylint mocks/ wrappers/ || echo "pylint not installed"
	@which flake8 > /dev/null && flake8 mocks/ wrappers/ --max-line-length=100 || echo "flake8 not installed"

# Watch for changes and re-run tests
watch:
	@echo "Watching for changes..."
	@which pytest-watch > /dev/null && pytest-watch || \
	echo "pytest-watch not installed. Install with: pip install pytest-watch"

# Generate test report
report:
	@echo "Generating test report..."
	$(PYTEST) --html=report.html --self-contained-html

# Quick test (smoke + unit)
quick:
	@echo "Running quick tests..."
	$(PYTEST) -m "smoke or unit" --maxfail=3

# Continuous Integration target
ci: clean install test coverage
	@echo "CI pipeline complete"
