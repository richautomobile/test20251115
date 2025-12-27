/**
 * @file test_adc.c
 * @brief Unit tests for ADC driver
 */

#include "../../framework/test_framework.h"
#include "../../mocks/mock_hal.h"

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
 * ADC Initialization Tests
 * ======================================================================== */

static void test_adc_init_returns_success(void)
{
    int result = mock_adc_init(0);
    TEST_ASSERT_EQUAL(0, result);
}

static void test_adc_init_invalid_channel_returns_error(void)
{
    int result = mock_adc_init(99);
    TEST_ASSERT_NOT_EQUAL(0, result);
}

static void test_adc_init_multiple_channels(void)
{
    TEST_ASSERT_EQUAL(0, mock_adc_init(0));
    TEST_ASSERT_EQUAL(0, mock_adc_init(1));
    TEST_ASSERT_EQUAL(0, mock_adc_init(2));
}

/* ========================================================================
 * ADC Read Tests
 * ======================================================================== */

static void test_adc_read_returns_default_value(void)
{
    setUp();

    mock_adc_init(0);
    uint16_t value = mock_adc_read(0);

    /* Default value should be mid-range (2048 for 12-bit) */
    TEST_ASSERT_EQUAL(2048, value);

    tearDown();
}

static void test_adc_read_without_init_returns_zero(void)
{
    setUp();

    /* Try to read without initialization */
    uint16_t value = mock_adc_read(0);

    TEST_ASSERT_EQUAL(0, value);

    tearDown();
}

static void test_adc_read_set_value(void)
{
    setUp();

    mock_adc_init(0);

    /* Set test value */
    uint16_t test_value = 1024;
    mock_adc_set_value(0, test_value);

    /* Read back */
    uint16_t read_value = mock_adc_read(0);

    TEST_ASSERT_EQUAL(test_value, read_value);

    tearDown();
}

/* ========================================================================
 * ADC Range Tests
 * ======================================================================== */

static void test_adc_zero_scale(void)
{
    setUp();

    mock_adc_init(0);
    mock_adc_set_value(0, 0);

    uint16_t value = mock_adc_read(0);

    TEST_ASSERT_EQUAL(0, value);

    tearDown();
}

static void test_adc_full_scale(void)
{
    setUp();

    mock_adc_init(0);
    mock_adc_set_value(0, 4095);  // 12-bit max

    uint16_t value = mock_adc_read(0);

    TEST_ASSERT_EQUAL(4095, value);

    tearDown();
}

static void test_adc_mid_scale(void)
{
    setUp();

    mock_adc_init(0);
    mock_adc_set_value(0, 2048);

    uint16_t value = mock_adc_read(0);

    TEST_ASSERT_EQUAL(2048, value);

    tearDown();
}

static void test_adc_quarter_scale(void)
{
    setUp();

    mock_adc_init(0);
    mock_adc_set_value(0, 1024);

    uint16_t value = mock_adc_read(0);

    TEST_ASSERT_EQUAL(1024, value);

    tearDown();
}

/* ========================================================================
 * ADC Multi-Channel Tests
 * ======================================================================== */

static void test_adc_multiple_channels_independent(void)
{
    setUp();

    /* Initialize multiple channels */
    mock_adc_init(0);
    mock_adc_init(1);
    mock_adc_init(2);

    /* Set different values */
    mock_adc_set_value(0, 1000);
    mock_adc_set_value(1, 2000);
    mock_adc_set_value(2, 3000);

    /* Verify each channel */
    TEST_ASSERT_EQUAL(1000, mock_adc_read(0));
    TEST_ASSERT_EQUAL(2000, mock_adc_read(1));
    TEST_ASSERT_EQUAL(3000, mock_adc_read(2));

    tearDown();
}

/* ========================================================================
 * ADC Value Stability Tests
 * ======================================================================== */

static void test_adc_repeated_reads_consistent(void)
{
    setUp();

    mock_adc_init(0);
    mock_adc_set_value(0, 1500);

    /* Read multiple times */
    uint16_t value1 = mock_adc_read(0);
    uint16_t value2 = mock_adc_read(0);
    uint16_t value3 = mock_adc_read(0);

    /* All reads should return same value */
    TEST_ASSERT_EQUAL(value1, value2);
    TEST_ASSERT_EQUAL(value2, value3);

    tearDown();
}

/* ========================================================================
 * Test Runner
 * ======================================================================== */

int main(void)
{
    UnityBegin("ADC Driver Tests");

    /* Initialization tests */
    RUN_TEST(test_adc_init_returns_success);
    RUN_TEST(test_adc_init_invalid_channel_returns_error);
    RUN_TEST(test_adc_init_multiple_channels);

    /* Read tests */
    RUN_TEST(test_adc_read_returns_default_value);
    RUN_TEST(test_adc_read_without_init_returns_zero);
    RUN_TEST(test_adc_read_set_value);

    /* Range tests */
    RUN_TEST(test_adc_zero_scale);
    RUN_TEST(test_adc_full_scale);
    RUN_TEST(test_adc_mid_scale);
    RUN_TEST(test_adc_quarter_scale);

    /* Multi-channel tests */
    RUN_TEST(test_adc_multiple_channels_independent);

    /* Stability tests */
    RUN_TEST(test_adc_repeated_reads_consistent);

    return UnityEnd();
}
