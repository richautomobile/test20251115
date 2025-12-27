/**
 * @file test_framework.h
 * @brief Test framework wrapper and utilities
 *
 * Provides Unity framework integration and custom test utilities
 * for PSoC4 embedded testing.
 */

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Unity framework includes */
/* Note: Unity should be cloned from https://github.com/ThrowTheSwitch/Unity */
/* For now, we provide the essential macros and declarations */

#ifdef USE_UNITY_FRAMEWORK
#include "unity.h"
#else
/* Minimal Unity-compatible macros for standalone compilation */
#include <stdio.h>
#include <stdlib.h>

extern int unity_test_count;
extern int unity_test_failures;

#define TEST_ASSERT(condition) \
    do { \
        unity_test_count++; \
        if (!(condition)) { \
            printf("FAIL: %s:%d - %s\n", __FILE__, __LINE__, #condition); \
            unity_test_failures++; \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL(expected, actual) \
    do { \
        unity_test_count++; \
        if ((expected) != (actual)) { \
            printf("FAIL: %s:%d - Expected %d, got %d\n", __FILE__, __LINE__, (int)(expected), (int)(actual)); \
            unity_test_failures++; \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL_HEX(expected, actual) \
    TEST_ASSERT_EQUAL(expected, actual)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    TEST_ASSERT_EQUAL(expected, actual)

#define TEST_ASSERT_EQUAL_UINT8(expected, actual) \
    TEST_ASSERT_EQUAL(expected, actual)

#define TEST_ASSERT_EQUAL_UINT16(expected, actual) \
    TEST_ASSERT_EQUAL(expected, actual)

#define TEST_ASSERT_EQUAL_UINT32(expected, actual) \
    TEST_ASSERT_EQUAL(expected, actual)

#define TEST_ASSERT_NOT_EQUAL(expected, actual) \
    do { \
        unity_test_count++; \
        if ((expected) == (actual)) { \
            printf("FAIL: %s:%d - Values should not be equal: %d\n", __FILE__, __LINE__, (int)(actual)); \
            unity_test_failures++; \
        } \
    } while(0)

#define TEST_ASSERT_NULL(pointer) \
    TEST_ASSERT((pointer) == NULL)

#define TEST_ASSERT_NOT_NULL(pointer) \
    TEST_ASSERT((pointer) != NULL)

#define TEST_ASSERT_TRUE(condition) \
    TEST_ASSERT(condition)

#define TEST_ASSERT_FALSE(condition) \
    TEST_ASSERT(!(condition))

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    do { \
        unity_test_count++; \
        if (strcmp((expected), (actual)) != 0) { \
            printf("FAIL: %s:%d - Expected \"%s\", got \"%s\"\n", __FILE__, __LINE__, (expected), (actual)); \
            unity_test_failures++; \
        } \
    } while(0)

#define TEST_ASSERT_EQUAL_MEMORY(expected, actual, len) \
    do { \
        unity_test_count++; \
        if (memcmp((expected), (actual), (len)) != 0) { \
            printf("FAIL: %s:%d - Memory mismatch\n", __FILE__, __LINE__); \
            unity_test_failures++; \
        } \
    } while(0)

#define RUN_TEST(func) \
    do { \
        printf("Running: %s\n", #func); \
        func(); \
    } while(0)

#define UnityBegin(name) \
    do { \
        printf("\n========================================\n"); \
        printf("Test Suite: %s\n", name); \
        printf("========================================\n"); \
        unity_test_count = 0; \
        unity_test_failures = 0; \
    } while(0)

#define UnityEnd() \
    do { \
        printf("========================================\n"); \
        printf("Tests: %d, Failures: %d\n", unity_test_count, unity_test_failures); \
        printf("========================================\n"); \
        return unity_test_failures; \
    } while(0)

#endif /* USE_UNITY_FRAMEWORK */

/* Custom test utilities */

/**
 * @brief Assert value is within a range
 */
#define TEST_ASSERT_IN_RANGE(value, min, max) \
    do { \
        TEST_ASSERT((value) >= (min)); \
        TEST_ASSERT((value) <= (max)); \
    } while(0)

/**
 * @brief Assert floating point values are approximately equal
 */
#define TEST_ASSERT_FLOAT_WITHIN(delta, expected, actual) \
    do { \
        float diff = (actual) - (expected); \
        if (diff < 0) diff = -diff; \
        TEST_ASSERT(diff <= (delta)); \
    } while(0)

/**
 * @brief Assert bit is set
 */
#define TEST_ASSERT_BIT_HIGH(reg, bit) \
    TEST_ASSERT(((reg) & (1 << (bit))) != 0)

/**
 * @brief Assert bit is clear
 */
#define TEST_ASSERT_BIT_LOW(reg, bit) \
    TEST_ASSERT(((reg) & (1 << (bit))) == 0)

/**
 * @brief Test fixture setup function type
 */
typedef void (*test_setup_fn)(void);

/**
 * @brief Test fixture teardown function type
 */
typedef void (*test_teardown_fn)(void);

/**
 * @brief Test case function type
 */
typedef void (*test_fn)(void);

/* Global test counters */
extern int unity_test_count;
extern int unity_test_failures;

/* Test utilities */
void test_print_header(const char* suite_name);
void test_print_footer(void);
void test_print_case(const char* test_name);

#endif /* TEST_FRAMEWORK_H */
