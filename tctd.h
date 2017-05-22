/*
=======================================================================
Tian C Testing & Debugging (TCTD)
The library impelment a useful C89(ANSI C) unit test framework:
Unit tests with simple assertions inspired from munit.
Safe memory control, random variables generation utilities.
A simple strategy for inspect memory leak in small projects.
Bencmarking inspired from minunit.
Debug tool: temporary variables declaration that convenient for step through.

Author: cltian
Email: tianchunlin123@gmail.com
Version: 0.1
========================================================================

/*

Copyright cltian

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/
#ifndef __TCTD_H__
#define __TCTD_H__

#ifdef __cplusplus
	extern "C" {
#endif


/**< C89 Libraries *//*
All C89 libraries
 */
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>


#ifdef __cplusplus
	extern "C" {
#endif

/** \brief Define the Logging level. LOG_ERROOR_ERRNO is the same as LOG_ERROR except for errno defined in errno.h
 */

typedef enum {
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
  LOG_ERROR_ERRNO
} TCTDLogLevel;

// No __VA_ARGS__ in C89
/** \brief The following is print functions with line and filename
 *
 * \param  __FILE__
 * \param  __LINE__
 * \param  format
 * \param  message
 * \return no-return
 *
 */

FILE* tctd_fp ; // global var., Logging file handler

void vprint_info(const char* filename, const int line, const char* const format, va_list args);
void vprint_warning(const char* filename, const int line, const char* const format, va_list args);
void vprint_error(char* filename, const int line, const char* const format, va_list args);
void vprint_error_errno(const char* filename, const int line, const char* const format, va_list args);
/**< Main print function */
void tctd_print_standard(TCTDLogLevel LogLevel, char* filename, int line, char* format, ...);

unsigned int tctd_count[2]; //global var., counts the passing rate
/**< wrapper for test case */
#define tctd_utest(f) { #f, &f}
typedef void (*Test)(); //void* pointer
/**< the test substance */
typedef struct TCTDUTest {
    const char *name;
    Test test_func;
}TCTDUTest;

/** \brief Unit Test
 *
 */

#ifdef TCTD_UTEST

#define Test void
/**< run test macro */
# define tctd_run_utests(group_tests) \
        _tctd_run_utests(#group_tests, group_tests, sizeof(group_tests) / sizeof(group_tests[0]))

/** \brief Followings are assertion, I use munit assertions. See https://github.com/nemequ/munit/blob/master/munit.h
 *
 */

/**< assert an expression */
#define tctd_assert(expr) \
  do { \
    tctd_count[0]++;\
    if (!(expr)) { \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s", "assertion failed: " #expr);\
    } \
  } while (0)

/**< assert an expression if true */
#define tctd_assert_true(expr) \
  do { \
    if (!(expr)) { \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s", "assertion failed: " #expr " is not true"); \
    } \
  } while (0)
/**< same as tctd_assert_true */
#define tctd_assert_false(expr) \
  do { \
    if (expr) { \
      tatd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s", "assertion failed: " #expr " is not false"); \
    } \
  } while (0)
/**< the base macro */
#define tctd_assert_type_full(prefix, suffix, T, fmt, a, op, b)   \
  do { \
    T tctd_tmp_a_ = (a); \
    T tctd_tmp_b_ = (b); \
    tctd_count[0]++;\
    if (!(tctd_tmp_a_ op tctd_tmp_b_)) {                               \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s", "assertion failed: " #a " " #op " " #b " (" prefix "%" fmt suffix " " #op " " prefix "%" fmt suffix ")", \
                   tctd_tmp_a_, tctd_tmp_b_); \
      tctd_count[1]++;\
    } \
  } while (0)
/**< macro wrapper */
#define tctd_assert_type(T, fmt, a, op, b) \
  tctd_assert_type_full("", "", T, fmt, a, op, b)

/**< char and int  */
#define tctd_assert_char(a, op, b) \
  tctd_assert_type_full("'\\x", "'", char, "02" tctd_CHAR_MODIFIER "x", a, op, b, tctd_count)
#define tctd_assert_uchar(a, op, b) \
  tctd_assert_type_full("'\\x", "'", unsigned char, "02" tctd_CHAR_MODIFIER "x", a, op, b, tctd_count)
#define tctd_assert_short(a, op, b) \
  tctd_assert_type(short, tctd_SHORT_MODIFIER "d", a, op, b)
#define tctd_assert_ushort(a, op, b) \
  tctd_assert_type(unsigned short, tctd_SHORT_MODIFIER "u", a, op, b)
#define tctd_assert_int(a, op, b) \
  tctd_assert_type(int, "d", a, op, b)
#define tctd_assert_uint(a, op, b) \
  tctd_assert_type(unsigned int, "u", a, op, b)
#define tctd_assert_long(a, op, b) \
  tctd_assert_type(long int, "ld", a, op, b)
#define tctd_assert_ulong(a, op, b) \
  tctd_assert_type(unsigned long int, "lu", a, op, b)
#define tctd_assert_llong(a, op, b) \
  tctd_assert_type(long long int, "lld", a, op, b)
#define tctd_assert_ullong(a, op, b) \
  tctd_assert_type(unsigned long long int, "llu", a, op, b)
#define tctd_assert_size(a, op, b) \
  tctd_assert_type(size_t, tctd_SIZE_MODIFIER "u", a, op, b)
/**< float and double except for equal  */
#define tctd_assert_float(a, op, b) \
  tctd_assert_type(float, "f", a, op, b)
#define tctd_assert_double(a, op, b) \
  tctd_assert_type(double, "g", a, op, b)
#define tctd_assert_ptr(a, op, b) \
  tctd_assert_type(const void*, "p", a, op, b)
/**< float and double equal and not equal */
#define tctd_assert_float_equal(a, b, precision) \
  do { \
    tctd_count[0]++;\
    float tctd_tmp_a_ = (a); \
    float tctd_tmp_b_ = (b); \
    float tctd_tmp_diff_ = ((tctd_tmp_a_ - tctd_tmp_b_) < 0) ? \
      -(tctd_tmp_a_ - tctd_tmp_b_) : \
      (tctd_tmp_a_ - tctd_tmp_b_); \
    if ((tctd_tmp_diff_ > 1e-##precision)) { \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s","assertion failed: " #a " == " #b " (%0." #precision "g == %0." #precision "g)", \
           tctd_tmp_a_, tctd_tmp_b_); \
      tctd_count[1]++;\
    } \
  } while (0)

#define tctd_assert_double_equal(a, b, precision) \
  do { \
    tctd_count[0]++;\
    double tctd_tmp_a_ = (a); \
    double tctd_tmp_b_ = (b); \
    double tctd_tmp_diff_ = ((tctd_tmp_a_ - tctd_tmp_b_) < 0) ? \
      -(tctd_tmp_a_ - tctd_tmp_b_) : \
      (tctd_tmp_a_ - tctd_tmp_b_); \
    if ((tctd_tmp_diff_ > 1e-##precision)) { \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s","assertion failed: " #a " == " #b " (%0." #precision "g == %0." #precision "g)", \
           tctd_tmp_a_, tctd_tmp_b_); \
      tctd_count[1]++;\
    } \
  } while (0)

  #define tctd_assert_float_not_equal(a, b, precision) \
  do { \
    tctd_count[0]++;\
    float tctd_tmp_a_ = (a); \
    float tctd_tmp_b_ = (b); \
    float tctd_tmp_diff_ = ((tctd_tmp_a_ - tctd_tmp_b_) < 0) ? \
      -(tctd_tmp_a_ - tctd_tmp_b_) : \
      (tctd_tmp_a_ - tctd_tmp_b_); \
    if ((tctd_tmp_diff_ < 1e-##precision)) { \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s","assertion failed: " #a " == " #b " (%0." #precision "g == %0." #precision "g)", \
           tctd_tmp_a_, tctd_tmp_b_); \
      tctd_count[1]++;\
    } \
  } while (0)

  #define tctd_assert_double_equal(a, b, precision) \
  do { \
    tctd_count[0]++;\
    double tctd_tmp_a_ = (a); \
    double tctd_tmp_b_ = (b); \
    double tctd_tmp_diff_ = ((tctd_tmp_a_ - tctd_tmp_b_) < 0) ? \
      -(tctd_tmp_a_ - tctd_tmp_b_) : \
      (tctd_tmp_a_ - tctd_tmp_b_); \
    if ((tctd_tmp_diff_ < 1e-##precision)) { \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s","assertion failed: " #a " == " #b " (%0." #precision "g == %0." #precision "g)", \
           tctd_tmp_a_, tctd_tmp_b_); \
      tctd_count[1]++;\
    } \
  } while (0)
  /**< string equal and not equal  */
#define tctd_assert_string_equal(a, b) \
  do { \
    char* tctd_tmp_a_ = a; \
    char* tctd_tmp_b_ = b; \
    tctd_count[0]++;\
    if ((strcmp(tctd_tmp_a_, tctd_tmp_b_) != 0)) { \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s", "assertion failed: string " #a " == " #b " (\"%s\" == \"%s\")", \
                   tctd_tmp_a_, tctd_tmp_b_); \
      tctd_count[1]++;\
    } \
  } while (0)

#define tctd_assert_string_not_equal(a, b) \
  do { \
    char* tctd_tmp_a_ = a; \
    char* tctd_tmp_b_ = b; \
    tctd_count[0]++;\
    if ((strcmp(tctd_tmp_a_, tctd_tmp_b_) == 0)) { \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s", "assertion failed: string " #a " != " #b " (\"%s\" == \"%s\")", \
                   tctd_tmp_a_, tctd_tmp_b_); \
      tctd_count[1]++;\
    } \
  } while (0)
/**< memory equal and not equal */
#define tctd_assert_memory_equal(size, a, b) \
  do { \
    unsigned char* tctd_tmp_a_ = (const unsigned char*) (a); \
    unsigned char* tctd_tmp_b_ = (const unsigned char*) (b); \
    size_t tctd_tmp_size_ = (size); \
    tctd_count[0]++;\
    if ((memcmp(tctd_tmp_a_, tctd_tmp_b_, tctd_tmp_size_)) != 0) { \
      size_t tctd_tmp_pos_; \
      for (tctd_tmp_pos_ = 0 ; tctd_tmp_pos_ < tctd_tmp_size_ ; tctd_tmp_pos_++) { \
        if (tctd_tmp_a_[tctd_tmp_pos_] != tctd_tmp_b_[tctd_tmp_pos_]) { \
          tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s", "assertion failed: memory " #a " == " #b ", at offset %" tctd_SIZE_MODIFIER "u", tctd_tmp_pos_); \
          tctd_count[1]++;\
          break; \
        } \
      } \
    } \
  } while (0)

#define tctd_assert_memory_not_equal(size, a, b) \
  do { \
    unsigned char* tctd_tmp_a_ = (const unsigned char*) (a); \
    unsigned char* tctd_tmp_b_ = (const unsigned char*) (b); \
    size_t tctd_tmp_size_ = (size); \
    tctd_count[0]++;\
    if ((memcmp(tctd_tmp_a_, tctd_tmp_b_, tctd_tmp_size_)) == 0) { \
      tctd_print_standard(LOG_ERROR, __FILE__, __LINE__, "%s", "assertion failed: memory" #a " != " #b " (%zu bytes)", tctd_tmp_size_); \
      tctd_count[1]++;\
    } \
  } while (0)
/**< assert pointers */
#define tctd_assert_ptr_equal(a, b) \
  tctd_assert_ptr(a, ==, b)
#define tctd_assert_ptr_not_equal(a, b) \
  tctd_assert_ptr(a, !=, b)
#define tctd_assert_null(ptr) \
  tctd_assert_ptr(ptr, ==, NULL)
#define tctd_assert_not_null(ptr) \
  tctd_assert_ptr(ptr, !=, NULL)
#define tctd_assert_ptr_null(ptr) \
  tctd_assert_ptr(ptr, ==, NULL)
#define tctd_assert_ptr_not_null(ptr) \
  tctd_assert_ptr(ptr, !=, NULL)

/**< aliases of assertions when define TCTD_ENABLE_ASSERT_ALIASES  */
#if defined(TCTD_ENABLE_ASSERT_ALIASES)
#define assert_true(expr) tctd_assert_true(expr)
#define assert_false(expr) tctd_assert_false(expr)
#define assert_char(a, op, b) tctd_assert_char(a, op, b)
#define assert_uchar(a, op, b) tctd_assert_uchar(a, op, b)
#define assert_short(a, op, b) tctd_assert_short(a, op, b)
#define assert_ushort(a, op, b) tctd_assert_ushort(a, op, b)
#define assert_int(a, op, b) tctd_assert_int(a, op, b)
#define assert_uint(a, op, b) tctd_assert_uint(a, op, b)
#define assert_long(a, op, b) tctd_assert_long(a, op, b)
#define assert_ulong(a, op, b) tctd_assert_ulong(a, op, b)
#define assert_llong(a, op, b) tctd_assert_llong(a, op, b)
#define assert_ullong(a, op, b) tctd_assert_ullong(a, op, b)
#define assert_size(a, op, b) tctd_assert_size(a, op, b)
#define assert_float(a, op, b) tctd_assert_float(a, op, b)
#define assert_double(a, op, b) tctd_assert_double(a, op, b)
#define assert_ptr(a, op, b) tctd_assert_ptr(a, op, b)
#define assert_double_equal(a, b, precision) tctd_assert_double_equal(a, b, precision)
#define assert_string_equal(a, b) tctd_assert_string_equal(a, b)
#define assert_string_not_equal(a, b) tctd_assert_string_not_equal(a, b)
#define assert_memory_equal(size, a, b) tctd_assert_memory_equal(size, a, b)
#define assert_memory_not_equal(size, a, b) tctd_assert_memory_not_equal(size, a, b)
#define assert_ptr_equal(a, b) tctd_assert_ptr_equal(a, b)
#define assert_ptr_not_equal(a, b) tctd_assert_ptr_not_equal(a, b)
#define assert_ptr_null(ptr) tctd_assert_null_equal(ptr)
#define assert_ptr_not_null(ptr) tctd_assert_not_null(ptr)
#define assert_null(ptr) tctd_assert_null(ptr)
#define assert_not_null(ptr) tctd_assert_not_null(ptr)

#endif /* defined(tctd_ENABLE_ASSERT_ALIASES) */

/**< running instance */
void _tctd_run_utests(char* group_name, TCTDUTest group_tests[], size_t num_elements);

#endif // TCTD_UTEST


/** \brief Debug Mode. The main tool is define temporary var. for step through.
 *
 */
#ifdef TCTD_DEBUG
/**< temp_vars macro can declare the temp variables less than 16 */
#define DEBUG //NULL macro for debug
#define temp_vars(name, type, number) _generate_temp_vars_##number(name, type)
#define _generate_temp_vars_1(name, type)\
            type name;
#define _generate_temp_vars_2(name, type)\
            type name##_1;\
            type name##_2;
#define _generate_temp_vars_3(name, type)\
            _generate_temp_vars_2(name, type)\
            type name##_3;
#define _generate_temp_vars_4(name, type)\
            _generate_temp_vars_3(name, type);\
            type name##_4;
#define _generate_temp_vars_5(name, type)\
            _generate_temp_vars_4(name, type);\
            type name##_5;
#define _generate_temp_vars_6(name, type)\
            _generate_temp_vars_5(name, type);\
            type name##_6;
#define _generate_temp_vars_7(name, type)\
            _generate_temp_vars_6(name, type);\
            type name##_7;
#define _generate_temp_vars_8(name, type)\
            _generate_temp_vars_7(name, type);\
            type name##_8;
#define _generate_temp_vars_9(name, type)\
            _generate_temp_vars_8(name, type);\
            type name##_9;
#define _generate_temp_vars_10(name, type)\
            _generate_temp_vars_9(name, type);\
            type name##_10;
#define _generate_temp_vars_11(name, type)\
            _generate_temp_vars_10(name, type);\
            type name##_11;
#define _generate_temp_vars_12(name, type)\
            _generate_temp_vars_11(name, type);\
            type name##_12;
#define _generate_temp_vars_13(name, type)\
            _generate_temp_vars_12(name, type);\
            type name##_13;
#define _generate_temp_vars_14(name, type)\
            _generate_temp_vars_13(name, type);\
            type name##_14;
#define _generate_temp_vars_15(name, type)\
            _generate_temp_vars_14(name, type);\
            type name##_15;

#endif

/** \brief Some memory control utilities safe wrappers. Besides the _log version is convenient for find memory leak.
 *
 */

#define tctd_malloc_(type, size) _tctd_malloc(size, sizeof(type))
#define tctd_malloc(type, size) (type*)tctd_malloc_(type, size)
#define tctd_free(ptr) _tctd_free(ptr)
#define tctd_calloc(type, size) _tctd_calloc(size, sizeof(type))
/**< Logging is convenient for find memory leak as the number of malloc must equal free. */
//with log
#define tctd_malloc_log(type, size) _tctd_malloc_log(size, sizeof(type), __FILE__, __LINE__)
#define tctd_free_log(ptr) __tctd_free_log(ptr, __FILE__, __LINE__)
#define tctd_calloc_log(type, size) _tctd_calloc_log(size, sizeof(type), __FILE__, __LINE__)

void* _tctd_malloc(size_t n, size_t size);
void _tctd_free(void* ptr);
void* _tctd_calloc(size_t n, size_t size);
void* _tctd_malloc_log(size_t n, size_t size, char* filename, int line);
void _tctd_free_log(void* ptr, char* filename, int line);
void* _tctd_calloc_log(size_t n, size_t size, char* filename, int line);

/** \brief Random number generation utilities.
 *
 */

/**< set random seed, when seed is 0, use current time to set seed; when seed is not 0, use seed */
void tctd_rand_seed(unsigned int seed);

/**< (a,b) integer*/
int tctd_rand_int_range_1(int min, int max);
/**< [a,b] integer */
int tctd_rand_int_range_2(int min, int max);
/**< (a,b] integer */
int tctd_rand_int_range_3(int min, int max);
/**< [a,b) integer */
int tctd_rand_int_range_4(int min, int max);
/**< normalization of rand() */
double _surand();
/**< [low, high] uniform r.v. */
double tctd_urand(double low, double high);


/** \brief Benchmarking
 *
 * \param bench B
 * \param a function
 *
 */

 /**
  *
   example:

   bench B;
   TCTD_INIT_BENCH(B, 3)
   func();
   TCTD_END_BENCH(B)
   TCTD_SUM_BENCH(B)
  *
  */
typedef struct {
    long R; // runs
    time_t start;
    time_t end;
} bench;

time_t getTime();
void bench_start(bench* b);
void bench_stop(bench* b);
void bench_print_summary(bench *b);

#define TCTD_INIT_BENCH(B, RUN) \
        B.R = RUN>1?RUN:1; \
        int r_##B;\
        bench_start(&B); \
        for (r_##B = 0; r_##B < B.R ; r_##B++ ) { \

#define TCTD_END_BENCH(B) \
            } \
        bench_stop(&B);

#define TCTD_SUM_BENCH(B) bench_print_summary(&B);

#ifdef __cplusplus
 }
#endif

#endif
