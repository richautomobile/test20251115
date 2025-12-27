/**
 * @file test_runner_all.c
 * @brief Main test runner for all unit tests
 *
 * This file runs all unit test suites for the PSoC4 project.
 */

#include <stdio.h>
#include <stdlib.h>

/* External test functions */
extern int test_i2c_main(void);
extern int test_adc_main(void);
extern int test_gpio_main(void);
extern int test_system_init_main(void);

/* Test suite structure */
typedef struct {
    const char* name;
    int (*test_func)(void);
} test_suite_t;

/* Test suite registry */
static const test_suite_t test_suites[] = {
    {"I2C Driver Tests", NULL},  /* Would link to actual test */
    {"ADC Driver Tests", NULL},
    {"GPIO Driver Tests", NULL},
    {"System Init Tests", NULL},
    {NULL, NULL}  /* Terminator */
};

/**
 * @brief Main test runner entry point
 */
int main(int argc, char* argv[])
{
    int total_failures = 0;
    int total_suites = 0;
    int failed_suites = 0;

    printf("\n");
    printf("========================================\n");
    printf("PSoC4 Unit Test Suite\n");
    printf("========================================\n");
    printf("\n");

    /* Run each test suite */
    for (int i = 0; test_suites[i].name != NULL; i++) {
        total_suites++;

        if (test_suites[i].test_func != NULL) {
            printf("Running: %s\n", test_suites[i].name);
            printf("----------------------------------------\n");

            int failures = test_suites[i].test_func();

            if (failures > 0) {
                failed_suites++;
                total_failures += failures;
                printf("FAILED: %d test(s) failed\n\n", failures);
            } else {
                printf("PASSED: All tests passed\n\n");
            }
        }
    }

    /* Print summary */
    printf("========================================\n");
    printf("Test Summary\n");
    printf("========================================\n");
    printf("Total Suites:  %d\n", total_suites);
    printf("Passed Suites: %d\n", total_suites - failed_suites);
    printf("Failed Suites: %d\n", failed_suites);
    printf("Total Failures: %d\n", total_failures);
    printf("========================================\n");

    if (total_failures == 0) {
        printf("Result: ALL TESTS PASSED\n");
        printf("========================================\n");
        return EXIT_SUCCESS;
    } else {
        printf("Result: TESTS FAILED\n");
        printf("========================================\n");
        return EXIT_FAILURE;
    }
}
