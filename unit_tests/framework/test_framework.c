/**
 * @file test_framework.c
 * @brief Test framework implementation
 */

#include "test_framework.h"
#include <stdio.h>

/* Global test counters */
int unity_test_count = 0;
int unity_test_failures = 0;

/**
 * @brief Print test suite header
 */
void test_print_header(const char* suite_name)
{
    printf("\n");
    printf("========================================\n");
    printf("Test Suite: %s\n", suite_name);
    printf("========================================\n");
    unity_test_count = 0;
    unity_test_failures = 0;
}

/**
 * @brief Print test suite footer
 */
void test_print_footer(void)
{
    printf("========================================\n");
    printf("Tests Run: %d\n", unity_test_count);
    printf("Failures:  %d\n", unity_test_failures);
    printf("Success:   %d\n", unity_test_count - unity_test_failures);
    if (unity_test_failures == 0) {
        printf("Result:    PASS\n");
    } else {
        printf("Result:    FAIL\n");
    }
    printf("========================================\n");
}

/**
 * @brief Print test case name
 */
void test_print_case(const char* test_name)
{
    printf("  • %s ... ", test_name);
}
