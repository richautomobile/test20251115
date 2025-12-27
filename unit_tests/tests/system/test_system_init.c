/**
 * @file test_system_init.c
 * @brief Unit tests for system initialization
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
 * BSP Initialization Tests
 * ======================================================================== */

static void test_cybsp_init_returns_success(void)
{
    cy_rslt_t result = cybsp_init();
    TEST_ASSERT_EQUAL(CY_RSLT_SUCCESS, result);
}

static void test_cybsp_init_increments_call_count(void)
{
    uint32_t before = g_mock_hal_state.total_calls;

    cybsp_init();

    uint32_t after = g_mock_hal_state.total_calls;

    TEST_ASSERT_EQUAL(before + 1, after);
}

/* ========================================================================
 * System Register Tests
 * ======================================================================== */

static void test_register_write_read(void)
{
    setUp();

    uint32_t test_addr = 0x40000000;
    uint32_t test_value = 0xDEADBEEF;

    mock_hal_write_register(test_addr, test_value);
    uint32_t read_value = mock_hal_read_register(test_addr);

    TEST_ASSERT_EQUAL_HEX(test_value, read_value);

    tearDown();
}

static void test_register_write_multiple(void)
{
    setUp();

    /* Write multiple registers */
    mock_hal_write_register(0x00, 0x11111111);
    mock_hal_write_register(0x04, 0x22222222);
    mock_hal_write_register(0x08, 0x33333333);

    /* Verify */
    TEST_ASSERT_EQUAL_HEX(0x11111111, mock_hal_read_register(0x00));
    TEST_ASSERT_EQUAL_HEX(0x22222222, mock_hal_read_register(0x04));
    TEST_ASSERT_EQUAL_HEX(0x33333333, mock_hal_read_register(0x08));

    tearDown();
}

/* ========================================================================
 * System Memory Tests
 * ======================================================================== */

static void test_memory_write_read(void)
{
    setUp();

    uint8_t test_data[] = {0x01, 0x02, 0x03, 0x04};
    uint32_t addr = 0x100;

    mock_hal_write_memory(addr, test_data, sizeof(test_data));

    uint8_t read_data[4] = {0};
    mock_hal_read_memory(addr, read_data, sizeof(read_data));

    TEST_ASSERT_EQUAL_MEMORY(test_data, read_data, sizeof(test_data));

    tearDown();
}

/* ========================================================================
 * System Tick Tests
 * ======================================================================== */

static void test_tick_count_increments(void)
{
    setUp();

    uint32_t before = mock_get_tick_count();

    mock_delay_ms(10);

    uint32_t after = mock_get_tick_count();

    TEST_ASSERT_EQUAL(before + 10, after);

    tearDown();
}

static void test_delay_ms_updates_tick(void)
{
    setUp();

    uint32_t delay = 100;

    uint32_t before = mock_get_tick_count();
    mock_delay_ms(delay);
    uint32_t after = mock_get_tick_count();

    TEST_ASSERT_EQUAL(delay, after - before);

    tearDown();
}

/* ========================================================================
 * System Reset Tests
 * ======================================================================== */

static void test_system_reset_clears_state(void)
{
    setUp();

    /* Set some state */
    mock_hal_write_register(0x00, 0xAAAAAAAA);
    mock_delay_ms(50);

    /* Reset */
    mock_system_reset();

    /* Verify cleared */
    TEST_ASSERT_TRUE(g_mock_hal_state.initialized);

    tearDown();
}

/* ========================================================================
 * Flash Memory Tests
 * ======================================================================== */

static void test_flash_init(void)
{
    int result = mock_flash_init();
    TEST_ASSERT_EQUAL(0, result);
}

static void test_flash_write_read(void)
{
    setUp();

    mock_flash_init();

    uint8_t write_data[] = "Test Data";
    uint32_t addr = 0x1000;

    int write_result = mock_flash_write(addr, write_data, sizeof(write_data));
    TEST_ASSERT_EQUAL(0, write_result);

    uint8_t read_data[16] = {0};
    int read_result = mock_flash_read(addr, read_data, sizeof(write_data));
    TEST_ASSERT_EQUAL(0, read_result);

    TEST_ASSERT_EQUAL_STRING((char*)write_data, (char*)read_data);

    tearDown();
}

static void test_flash_erase_page(void)
{
    setUp();

    mock_flash_init();

    /* Write data */
    uint8_t write_data[] = {0x11, 0x22, 0x33, 0x44};
    mock_flash_write(0, write_data, sizeof(write_data));

    /* Erase page 0 */
    int result = mock_flash_erase_page(0);
    TEST_ASSERT_EQUAL(0, result);

    /* Verify erased (should be 0xFF) */
    uint8_t read_data[4];
    mock_flash_read(0, read_data, sizeof(read_data));

    for (int i = 0; i < 4; i++) {
        TEST_ASSERT_EQUAL_HEX(0xFF, read_data[i]);
    }

    tearDown();
}

/* ========================================================================
 * Test Runner
 * ======================================================================== */

int main(void)
{
    UnityBegin("System Initialization Tests");

    /* BSP tests */
    RUN_TEST(test_cybsp_init_returns_success);
    RUN_TEST(test_cybsp_init_increments_call_count);

    /* Register tests */
    RUN_TEST(test_register_write_read);
    RUN_TEST(test_register_write_multiple);

    /* Memory tests */
    RUN_TEST(test_memory_write_read);

    /* Tick tests */
    RUN_TEST(test_tick_count_increments);
    RUN_TEST(test_delay_ms_updates_tick);

    /* Reset tests */
    RUN_TEST(test_system_reset_clears_state);

    /* Flash tests */
    RUN_TEST(test_flash_init);
    RUN_TEST(test_flash_write_read);
    RUN_TEST(test_flash_erase_page);

    return UnityEnd();
}
