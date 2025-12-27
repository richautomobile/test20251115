/**
 * @file unity_config.h
 * @brief Unity test framework configuration for PSoC4 project
 *
 * This file configures Unity test framework settings for embedded testing.
 * Unity framework should be obtained from: https://github.com/ThrowTheSwitch/Unity
 */

#ifndef UNITY_CONFIG_H
#define UNITY_CONFIG_H

/* Unity Configuration */
#define UNITY_EXCLUDE_FLOAT
#define UNITY_EXCLUDE_DOUBLE
#define UNITY_INCLUDE_EXEC_TIME

/* Output configuration */
#define UNITY_OUTPUT_CHAR(a)    putchar(a)
#define UNITY_OUTPUT_START()    /* No special start */
#define UNITY_OUTPUT_COMPLETE() /* No special complete */

/* Memory configuration for embedded systems */
#define UNITY_EXCLUDE_SETJMP_H

/* Test fixture support */
#define UNITY_SUPPORT_TEST_CASES

/* Weak linking for embedded systems */
#ifdef __GNUC__
    #define UNITY_WEAK __attribute__((weak))
#else
    #define UNITY_WEAK
#endif

/* Include guards */
#ifdef __cplusplus
extern "C" {
#endif

/* Custom assertion messages */
#define UNITY_PRINT_EOL()    UNITY_OUTPUT_CHAR('\n')

#ifdef __cplusplus
}
#endif

#endif /* UNITY_CONFIG_H */
