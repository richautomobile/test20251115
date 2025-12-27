# Makefile for PSoC4 Unit Tests
# Supports compilation, execution, and code coverage

# Compiler and tools
CC := gcc
AR := ar
GCOV := gcov
LCOV := lcov
GENHTML := genhtml

# Directories
BUILD_DIR := build
COV_DIR := coverage
FRAMEWORK_DIR := framework
MOCKS_DIR := mocks
TESTS_DIR := tests
RUNNERS_DIR := runners

# Compiler flags
CFLAGS := -Wall -Wextra -Werror -std=c11 -g
CFLAGS += -I$(FRAMEWORK_DIR)
CFLAGS += -I$(MOCKS_DIR)
CFLAGS += -I.

# Coverage flags
COV_CFLAGS := $(CFLAGS) --coverage -fprofile-arcs -ftest-coverage
COV_LDFLAGS := --coverage -fprofile-arcs -ftest-coverage

# Test executables
TEST_I2C := $(BUILD_DIR)/test_i2c
TEST_ADC := $(BUILD_DIR)/test_adc
TEST_GPIO := $(BUILD_DIR)/test_gpio
TEST_SYSTEM := $(BUILD_DIR)/test_system_init

ALL_TESTS := $(TEST_I2C) $(TEST_ADC) $(TEST_GPIO) $(TEST_SYSTEM)

# Source files
FRAMEWORK_SRCS := $(FRAMEWORK_DIR)/test_framework.c
MOCK_SRCS := $(MOCKS_DIR)/mock_hal.c

# Object files
FRAMEWORK_OBJS := $(BUILD_DIR)/test_framework.o
MOCK_OBJS := $(BUILD_DIR)/mock_hal.o
COMMON_OBJS := $(FRAMEWORK_OBJS) $(MOCK_OBJS)

# Coverage files
COV_FRAMEWORK_OBJS := $(BUILD_DIR)/test_framework_cov.o
COV_MOCK_OBJS := $(BUILD_DIR)/mock_hal_cov.o
COV_COMMON_OBJS := $(COV_FRAMEWORK_OBJS) $(COV_MOCK_OBJS)

.PHONY: all clean test coverage help build mkdir

# Default target
all: build

# Help target
help:
	@echo "PSoC4 Unit Test Makefile"
	@echo "========================"
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build all tests (default)"
	@echo "  build      - Build all test executables"
	@echo "  test       - Build and run all tests"
	@echo "  test-i2c   - Run I2C tests"
	@echo "  test-adc   - Run ADC tests"
	@echo "  test-gpio  - Run GPIO tests"
	@echo "  test-system - Run system tests"
	@echo "  coverage   - Generate code coverage report"
	@echo "  clean      - Clean build artifacts"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make test          # Run all tests"
	@echo "  make test-i2c      # Run only I2C tests"
	@echo "  make coverage      # Generate coverage report"
	@echo ""

# Build all tests
build: mkdir $(ALL_TESTS)
	@echo "=========================================="
	@echo "All tests built successfully!"
	@echo "=========================================="

# Create build directory
mkdir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(COV_DIR)

# Framework objects
$(BUILD_DIR)/test_framework.o: $(FRAMEWORK_DIR)/test_framework.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_framework_cov.o: $(FRAMEWORK_DIR)/test_framework.c
	@echo "Compiling $< (with coverage)..."
	@$(CC) $(COV_CFLAGS) -c $< -o $@

# Mock objects
$(BUILD_DIR)/mock_hal.o: $(MOCKS_DIR)/mock_hal.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/mock_hal_cov.o: $(MOCKS_DIR)/mock_hal.c
	@echo "Compiling $< (with coverage)..."
	@$(CC) $(COV_CFLAGS) -c $< -o $@

# Test executables
$(TEST_I2C): $(TESTS_DIR)/driver/test_i2c.c $(COMMON_OBJS)
	@echo "Building I2C tests..."
	@$(CC) $(CFLAGS) $< $(COMMON_OBJS) -o $@

$(TEST_ADC): $(TESTS_DIR)/driver/test_adc.c $(COMMON_OBJS)
	@echo "Building ADC tests..."
	@$(CC) $(CFLAGS) $< $(COMMON_OBJS) -o $@

$(TEST_GPIO): $(TESTS_DIR)/driver/test_gpio.c $(COMMON_OBJS)
	@echo "Building GPIO tests..."
	@$(CC) $(CFLAGS) $< $(COMMON_OBJS) -o $@

$(TEST_SYSTEM): $(TESTS_DIR)/system/test_system_init.c $(COMMON_OBJS)
	@echo "Building System tests..."
	@$(CC) $(CFLAGS) $< $(COMMON_OBJS) -o $@

# Coverage builds
$(BUILD_DIR)/test_i2c_cov: $(TESTS_DIR)/driver/test_i2c.c $(COV_COMMON_OBJS)
	@echo "Building I2C tests (with coverage)..."
	@$(CC) $(COV_CFLAGS) $< $(COV_COMMON_OBJS) -o $@ $(COV_LDFLAGS)

$(BUILD_DIR)/test_adc_cov: $(TESTS_DIR)/driver/test_adc.c $(COV_COMMON_OBJS)
	@echo "Building ADC tests (with coverage)..."
	@$(CC) $(COV_CFLAGS) $< $(COV_COMMON_OBJS) -o $@ $(COV_LDFLAGS)

$(BUILD_DIR)/test_gpio_cov: $(TESTS_DIR)/driver/test_gpio.c $(COV_COMMON_OBJS)
	@echo "Building GPIO tests (with coverage)..."
	@$(CC) $(COV_CFLAGS) $< $(COV_COMMON_OBJS) -o $@ $(COV_LDFLAGS)

$(BUILD_DIR)/test_system_init_cov: $(TESTS_DIR)/system/test_system_init.c $(COV_COMMON_OBJS)
	@echo "Building System tests (with coverage)..."
	@$(CC) $(COV_CFLAGS) $< $(COV_COMMON_OBJS) -o $@ $(COV_LDFLAGS)

# Run all tests
test: build
	@echo ""
	@echo "=========================================="
	@echo "Running All Unit Tests"
	@echo "=========================================="
	@echo ""
	@$(MAKE) test-i2c
	@$(MAKE) test-adc
	@$(MAKE) test-gpio
	@$(MAKE) test-system
	@echo ""
	@echo "=========================================="
	@echo "All Tests Completed"
	@echo "=========================================="

# Run individual tests
test-i2c: $(TEST_I2C)
	@echo "Running I2C tests..."
	@./$(TEST_I2C)
	@echo ""

test-adc: $(TEST_ADC)
	@echo "Running ADC tests..."
	@./$(TEST_ADC)
	@echo ""

test-gpio: $(TEST_GPIO)
	@echo "Running GPIO tests..."
	@./$(TEST_GPIO)
	@echo ""

test-system: $(TEST_SYSTEM)
	@echo "Running System tests..."
	@./$(TEST_SYSTEM)
	@echo ""

# Code coverage
coverage: mkdir $(COV_COMMON_OBJS)
	@echo "Building tests with coverage..."
	@$(MAKE) $(BUILD_DIR)/test_i2c_cov
	@$(MAKE) $(BUILD_DIR)/test_adc_cov
	@$(MAKE) $(BUILD_DIR)/test_gpio_cov
	@$(MAKE) $(BUILD_DIR)/test_system_init_cov
	@echo ""
	@echo "Running tests for coverage..."
	@./$(BUILD_DIR)/test_i2c_cov > /dev/null 2>&1 || true
	@./$(BUILD_DIR)/test_adc_cov > /dev/null 2>&1 || true
	@./$(BUILD_DIR)/test_gpio_cov > /dev/null 2>&1 || true
	@./$(BUILD_DIR)/test_system_init_cov > /dev/null 2>&1 || true
	@echo ""
	@echo "Generating coverage report..."
	@if command -v lcov > /dev/null 2>&1; then \
		lcov --capture --directory $(BUILD_DIR) --output-file $(COV_DIR)/coverage.info; \
		lcov --remove $(COV_DIR)/coverage.info '/usr/*' --output-file $(COV_DIR)/coverage.info; \
		genhtml $(COV_DIR)/coverage.info --output-directory $(COV_DIR)/html; \
		echo ""; \
		echo "Coverage report generated in $(COV_DIR)/html/index.html"; \
	else \
		echo ""; \
		echo "Warning: lcov not found. Install with: sudo apt-get install lcov"; \
		echo "Generating basic coverage with gcov..."; \
		cd $(BUILD_DIR) && gcov *.gcda; \
		echo ""; \
		echo "Coverage files generated in $(BUILD_DIR)"; \
	fi
	@echo ""
	@echo "=========================================="
	@echo "Coverage Report Generated"
	@echo "=========================================="

# Open coverage report
coverage-report: coverage
	@if command -v xdg-open > /dev/null 2>&1; then \
		xdg-open $(COV_DIR)/html/index.html; \
	elif command -v open > /dev/null 2>&1; then \
		open $(COV_DIR)/html/index.html; \
	else \
		echo "Please open $(COV_DIR)/html/index.html manually"; \
	fi

# Clean
clean:
	@echo "Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
	@rm -rf $(COV_DIR)
	@rm -f *.gcda *.gcno *.gcov
	@echo "Clean complete."

# Continuous Integration target
ci: clean test coverage
	@echo ""
	@echo "=========================================="
	@echo "CI Pipeline Completed Successfully"
	@echo "=========================================="

# Static analysis (if cppcheck is available)
static-analysis:
	@if command -v cppcheck > /dev/null 2>&1; then \
		echo "Running static analysis..."; \
		cppcheck --enable=all --inconclusive --std=c11 \
			$(FRAMEWORK_DIR) $(MOCKS_DIR) $(TESTS_DIR); \
	else \
		echo "cppcheck not found. Install with: sudo apt-get install cppcheck"; \
	fi

# Memory check (if valgrind is available)
memcheck: build
	@if command -v valgrind > /dev/null 2>&1; then \
		echo "Running memory check..."; \
		valgrind --leak-check=full --show-leak-kinds=all \
			--track-origins=yes --verbose \
			./$(TEST_I2C); \
	else \
		echo "valgrind not found. Install with: sudo apt-get install valgrind"; \
	fi
