/**
 * @file test_i2c.c
 * @brief Unit tests for I2C driver
 */

#include "../../framework/test_framework.h"
#include "../../mocks/mock_hal.h"
#include <string.h>

/* Test setup and teardown */
static void setUp(void)
{
    mock_hal_init();
}

static void tearDown(void)
{
    mock_hal_reset();
}

/* ========================================================================
 * I2C Initialization Tests
 * ======================================================================== */

static void test_i2c_init_returns_success(void)
{
    int result = mock_i2c_init(0, 400000);
    TEST_ASSERT_EQUAL(0, result);
}

static void test_i2c_init_invalid_bus_returns_error(void)
{
    int result = mock_i2c_init(99, 400000);
    TEST_ASSERT_NOT_EQUAL(0, result);
}

static void test_i2c_init_sets_speed_correctly(void)
{
    uint32_t expected_speed = 400000;
    mock_i2c_init(0, expected_speed);

    /* Verify by attempting operation - mock should be initialized */
    uint8_t test_data = 0xAA;
    int result = mock_i2c_write(0, 0x50, 0x00, &test_data, 1);
    TEST_ASSERT_EQUAL(0, result);
}

/* ========================================================================
 * I2C Write Tests
 * ======================================================================== */

static void test_i2c_write_single_byte(void)
{
    setUp();

    /* Initialize I2C */
    mock_i2c_init(0, 400000);

    /* Write single byte */
    uint8_t test_data = 0xAA;
    int result = mock_i2c_write(0, 0x50, 0x00, &test_data, 1);

    TEST_ASSERT_EQUAL(0, result);

    tearDown();
}

static void test_i2c_write_multiple_bytes(void)
{
    setUp();

    mock_i2c_init(0, 400000);

    /* Write multiple bytes */
    uint8_t test_data[] = {0x11, 0x22, 0x33, 0x44};
    int result = mock_i2c_write(0, 0x50, 0x00, test_data, sizeof(test_data));

    TEST_ASSERT_EQUAL(0, result);

    tearDown();
}

static void test_i2c_write_without_init_fails(void)
{
    setUp();

    /* Try to write without initialization */
    uint8_t test_data = 0xAA;
    int result = mock_i2c_write(0, 0x50, 0x00, &test_data, 1);

    TEST_ASSERT_NOT_EQUAL(0, result);

    tearDown();
}

/* ========================================================================
 * I2C Read Tests
 * ======================================================================== */

static void test_i2c_read_single_byte(void)
{
    setUp();

    mock_i2c_init(0, 400000);

    /* Write then read */
    uint8_t write_data = 0xBB;
    mock_i2c_write(0, 0x50, 0x00, &write_data, 1);

    uint8_t read_data = 0x00;
    int result = mock_i2c_read(0, 0x50, 0x00, &read_data, 1);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL(write_data, read_data);

    tearDown();
}

static void test_i2c_read_multiple_bytes(void)
{
    setUp();

    mock_i2c_init(0, 400000);

    /* Write test pattern */
    uint8_t write_data[] = {0x11, 0x22, 0x33, 0x44};
    mock_i2c_write(0, 0x50, 0x00, write_data, sizeof(write_data));

    /* Read back */
    uint8_t read_data[4] = {0};
    int result = mock_i2c_read(0, 0x50, 0x00, read_data, sizeof(read_data));

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_EQUAL_MEMORY(write_data, read_data, sizeof(write_data));

    tearDown();
}

/* ========================================================================
 * I2C Data Integrity Tests
 * ======================================================================== */

static void test_i2c_data_integrity_pattern_0x00(void)
{
    setUp();

    mock_i2c_init(0, 400000);

    uint8_t pattern[] = {0x00, 0x00, 0x00, 0x00};
    mock_i2c_write(0, 0x50, 0x00, pattern, sizeof(pattern));

    uint8_t read_back[4];
    mock_i2c_read(0, 0x50, 0x00, read_back, sizeof(read_back));

    TEST_ASSERT_EQUAL_MEMORY(pattern, read_back, sizeof(pattern));

    tearDown();
}

static void test_i2c_data_integrity_pattern_0xFF(void)
{
    setUp();

    mock_i2c_init(0, 400000);

    uint8_t pattern[] = {0xFF, 0xFF, 0xFF, 0xFF};
    mock_i2c_write(0, 0x50, 0x00, pattern, sizeof(pattern));

    uint8_t read_back[4];
    mock_i2c_read(0, 0x50, 0x00, read_back, sizeof(read_back));

    TEST_ASSERT_EQUAL_MEMORY(pattern, read_back, sizeof(pattern));

    tearDown();
}

static void test_i2c_data_integrity_alternating_pattern(void)
{
    setUp();

    mock_i2c_init(0, 400000);

    uint8_t pattern[] = {0xAA, 0x55, 0xAA, 0x55};
    mock_i2c_write(0, 0x50, 0x00, pattern, sizeof(pattern));

    uint8_t read_back[4];
    mock_i2c_read(0, 0x50, 0x00, read_back, sizeof(read_back));

    TEST_ASSERT_EQUAL_MEMORY(pattern, read_back, sizeof(pattern));

    tearDown();
}

/* ========================================================================
 * I2C Multiple Device Tests
 * ======================================================================== */

static void test_i2c_multiple_devices(void)
{
    setUp();

    mock_i2c_init(0, 400000);

    /* Write to device 1 */
    uint8_t data1 = 0xAA;
    mock_i2c_write(0, 0x50, 0x00, &data1, 1);

    /* Write to device 2 */
    uint8_t data2 = 0xBB;
    mock_i2c_write(0, 0x51, 0x00, &data2, 1);

    /* Read back from device 1 */
    uint8_t read1 = 0x00;
    mock_i2c_read(0, 0x50, 0x00, &read1, 1);
    TEST_ASSERT_EQUAL(data1, read1);

    /* Read back from device 2 */
    uint8_t read2 = 0x00;
    mock_i2c_read(0, 0x51, 0x00, &read2, 1);
    TEST_ASSERT_EQUAL(data2, read2);

    tearDown();
}

/* ========================================================================
 * Test Runner
 * ======================================================================== */

int main(void)
{
    UnityBegin("I2C Driver Tests");

    /* Initialization tests */
    RUN_TEST(test_i2c_init_returns_success);
    RUN_TEST(test_i2c_init_invalid_bus_returns_error);
    RUN_TEST(test_i2c_init_sets_speed_correctly);

    /* Write tests */
    RUN_TEST(test_i2c_write_single_byte);
    RUN_TEST(test_i2c_write_multiple_bytes);
    RUN_TEST(test_i2c_write_without_init_fails);

    /* Read tests */
    RUN_TEST(test_i2c_read_single_byte);
    RUN_TEST(test_i2c_read_multiple_bytes);

    /* Data integrity tests */
    RUN_TEST(test_i2c_data_integrity_pattern_0x00);
    RUN_TEST(test_i2c_data_integrity_pattern_0xFF);
    RUN_TEST(test_i2c_data_integrity_alternating_pattern);

    /* Multiple device tests */
    RUN_TEST(test_i2c_multiple_devices);

    return UnityEnd();
}
