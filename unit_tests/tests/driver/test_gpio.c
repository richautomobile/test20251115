/**
 * @file test_gpio.c
 * @brief Unit tests for GPIO driver
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
 * GPIO Initialization Tests
 * ======================================================================== */

static void test_gpio_init_output_mode(void)
{
    int result = mock_gpio_init(5, GPIO_MODE_OUTPUT);
    TEST_ASSERT_EQUAL(0, result);
}

static void test_gpio_init_input_mode(void)
{
    int result = mock_gpio_init(5, GPIO_MODE_INPUT);
    TEST_ASSERT_EQUAL(0, result);
}

static void test_gpio_init_invalid_pin_returns_error(void)
{
    int result = mock_gpio_init(99, GPIO_MODE_OUTPUT);
    TEST_ASSERT_NOT_EQUAL(0, result);
}

/* ========================================================================
 * GPIO Write Tests
 * ======================================================================== */

static void test_gpio_write_high(void)
{
    setUp();

    mock_gpio_init(5, GPIO_MODE_OUTPUT);
    int result = mock_gpio_write(5, GPIO_HIGH);

    TEST_ASSERT_EQUAL(0, result);

    tearDown();
}

static void test_gpio_write_low(void)
{
    setUp();

    mock_gpio_init(5, GPIO_MODE_OUTPUT);
    int result = mock_gpio_write(5, GPIO_LOW);

    TEST_ASSERT_EQUAL(0, result);

    tearDown();
}

static void test_gpio_write_invalid_pin_returns_error(void)
{
    setUp();

    int result = mock_gpio_write(99, GPIO_HIGH);

    TEST_ASSERT_NOT_EQUAL(0, result);

    tearDown();
}

/* ========================================================================
 * GPIO Read Tests
 * ======================================================================== */

static void test_gpio_read_after_write_high(void)
{
    setUp();

    mock_gpio_init(5, GPIO_MODE_OUTPUT);
    mock_gpio_write(5, GPIO_HIGH);

    uint8_t value = mock_gpio_read(5);

    TEST_ASSERT_EQUAL(GPIO_HIGH, value);

    tearDown();
}

static void test_gpio_read_after_write_low(void)
{
    setUp();

    mock_gpio_init(5, GPIO_MODE_OUTPUT);
    mock_gpio_write(5, GPIO_LOW);

    uint8_t value = mock_gpio_read(5);

    TEST_ASSERT_EQUAL(GPIO_LOW, value);

    tearDown();
}

static void test_gpio_read_default_state(void)
{
    setUp();

    mock_gpio_init(5, GPIO_MODE_INPUT);

    uint8_t value = mock_gpio_read(5);

    /* Default state should be LOW */
    TEST_ASSERT_EQUAL(GPIO_LOW, value);

    tearDown();
}

/* ========================================================================
 * GPIO Toggle Tests
 * ======================================================================== */

static void test_gpio_toggle_from_low_to_high(void)
{
    setUp();

    mock_gpio_init(5, GPIO_MODE_OUTPUT);

    /* Start LOW */
    mock_gpio_write(5, GPIO_LOW);
    TEST_ASSERT_EQUAL(GPIO_LOW, mock_gpio_read(5));

    /* Toggle to HIGH */
    mock_gpio_write(5, GPIO_HIGH);
    TEST_ASSERT_EQUAL(GPIO_HIGH, mock_gpio_read(5));

    tearDown();
}

static void test_gpio_toggle_from_high_to_low(void)
{
    setUp();

    mock_gpio_init(5, GPIO_MODE_OUTPUT);

    /* Start HIGH */
    mock_gpio_write(5, GPIO_HIGH);
    TEST_ASSERT_EQUAL(GPIO_HIGH, mock_gpio_read(5));

    /* Toggle to LOW */
    mock_gpio_write(5, GPIO_LOW);
    TEST_ASSERT_EQUAL(GPIO_LOW, mock_gpio_read(5));

    tearDown();
}

/* ========================================================================
 * GPIO Multiple Pin Tests
 * ======================================================================== */

static void test_gpio_multiple_pins_independent(void)
{
    setUp();

    /* Initialize multiple pins */
    mock_gpio_init(0, GPIO_MODE_OUTPUT);
    mock_gpio_init(1, GPIO_MODE_OUTPUT);
    mock_gpio_init(2, GPIO_MODE_OUTPUT);

    /* Set different values */
    mock_gpio_write(0, GPIO_HIGH);
    mock_gpio_write(1, GPIO_LOW);
    mock_gpio_write(2, GPIO_HIGH);

    /* Verify each pin */
    TEST_ASSERT_EQUAL(GPIO_HIGH, mock_gpio_read(0));
    TEST_ASSERT_EQUAL(GPIO_LOW, mock_gpio_read(1));
    TEST_ASSERT_EQUAL(GPIO_HIGH, mock_gpio_read(2));

    tearDown();
}

/* ========================================================================
 * GPIO Pattern Tests
 * ======================================================================== */

static void test_gpio_write_read_pattern(void)
{
    setUp();

    /* Initialize 8 pins */
    for (uint8_t i = 0; i < 8; i++) {
        mock_gpio_init(i, GPIO_MODE_OUTPUT);
    }

    /* Write pattern: 10101010 */
    for (uint8_t i = 0; i < 8; i++) {
        mock_gpio_write(i, (i % 2) ? GPIO_LOW : GPIO_HIGH);
    }

    /* Verify pattern */
    for (uint8_t i = 0; i < 8; i++) {
        uint8_t expected = (i % 2) ? GPIO_LOW : GPIO_HIGH;
        TEST_ASSERT_EQUAL(expected, mock_gpio_read(i));
    }

    tearDown();
}

/* ========================================================================
 * Test Runner
 * ======================================================================== */

int main(void)
{
    UnityBegin("GPIO Driver Tests");

    /* Initialization tests */
    RUN_TEST(test_gpio_init_output_mode);
    RUN_TEST(test_gpio_init_input_mode);
    RUN_TEST(test_gpio_init_invalid_pin_returns_error);

    /* Write tests */
    RUN_TEST(test_gpio_write_high);
    RUN_TEST(test_gpio_write_low);
    RUN_TEST(test_gpio_write_invalid_pin_returns_error);

    /* Read tests */
    RUN_TEST(test_gpio_read_after_write_high);
    RUN_TEST(test_gpio_read_after_write_low);
    RUN_TEST(test_gpio_read_default_state);

    /* Toggle tests */
    RUN_TEST(test_gpio_toggle_from_low_to_high);
    RUN_TEST(test_gpio_toggle_from_high_to_low);

    /* Multiple pin tests */
    RUN_TEST(test_gpio_multiple_pins_independent);

    /* Pattern tests */
    RUN_TEST(test_gpio_write_read_pattern);

    return UnityEnd();
}
