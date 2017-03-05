/*
*Copyright 2017 cltian

*Licensed under the Apache License, Version 2.0 (the "License");
*you may not use this file except in compliance with the License.
*You may obtain a copy of the License at

*    http://www.apache.org/licenses/LICENSE-2.0

*Unless required by applicable law or agreed to in writing, software
*distributed under the License is distributed on an "AS IS" BASIS,
*WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*See the License for the specific language governing permissions and
*limitations under the License.
 */

/**
 * @author cltian
 * @date 2017
 */

#ifndef __TCTD_H__
#define __TCTD_H__

#ifdef __cplusplus
	extern "C" {
#endif

#if defined(_WIN32)
#include <Windows.h>
#endif

/*
All C89 libraries
 */
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <setjmp.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
//2 types of testing: invasive or non-invasive: TCTD_UTEST
//Debug mode: TCTD_DEBUG

/*

 ********* Testing **********

*/
 #ifdef TCTD_UTEST
/*
以下主要实现测试功能，一部分想法来自于CMoka

1. 当需要使用单元测试功能时，需要#define TCTD_UTEST
2. 待测试函数放在一个void test_func()中，这是一个公共接口，必须遵守
3. 函数的第二个接口是tctd_utest(测试函数名)
4. 之后将他们放入TCTDUTest的结构体中，如
	TCTDUTest utest = {tctd_utest(f1), tctd_utest(f2)};
5. 最后将这个结构体放入tctd_run_utests中，如
	tctd_run_utests(utest);
6. assertion主要采用了munit: https://nemequ.github.io/munit/#assertions
7. 如果#define TCTD_ENABLE_ASSERT_ALIASES, 则可以采取简称assertion
*/
#define tctd_utest(f) { #f, &f}
typedef void (*Test)() = NULL; //这是一个包装函数
typedef struct TCTDUTest {
    const char *name;
    Test test_func;
}TCTDUTest;


# define tctd_run_utests(group_tests) \
        _tctd_run_utests(#group_tests, group_tests, sizeof(group_tests) / sizeof(group_tests)[0])


//

#define tctd_assert(expr) \
  do { \
    if (!(expr)) { \
      tctd_error("assertion failed: " #expr); \
    } \
  } while (0)

#define tctd_assert_true(expr) \
  do { \
    if (!(expr)) { \
      tctd_error("assertion failed: " #expr " is not true"); \
    } \
  } while (0)
      
#define tctd_assert_false(expr) \
  do { \
    if (expr) { \
      tctd_error("assertion failed: " #expr " is not false"); \
    } \
  } while (0)

#define tctd_assert_type_full(prefix, suffix, T, fmt, a, op, b)   \
  do { \
    T tctd_tmp_a_ = (a); \
    T tctd_tmp_b_ = (b); \
    if (!(tctd_tmp_a_ op tctd_tmp_b_)) {                               \
      print_std(ERROR, "assertion failed: " #a " " #op " " #b " (" prefix "%" fmt suffix " " #op " " prefix "%" fmt suffix ")", \
                   tctd_tmp_a_, tctd_tmp_b_); \
    } \
  } while (0)


#define tctd_assert_type(T, fmt, a, op, b) \
  tctd_assert_type_full("", "", T, fmt, a, op, b)

#define tctd_assert_char(a, op, b) \
  tctd_assert_type_full("'\\x", "'", char, "02" tctd_CHAR_MODIFIER "x", a, op, b)
#define tctd_assert_uchar(a, op, b) \
  tctd_assert_type_full("'\\x", "'", unsigned char, "02" tctd_CHAR_MODIFIER "x", a, op, b)
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

#define tctd_assert_float(a, op, b) \
  tctd_assert_type(float, "f", a, op, b)
#define tctd_assert_double(a, op, b) \
  tctd_assert_type(double, "g", a, op, b)
#define tctd_assert_ptr(a, op, b) \
  tctd_assert_type(const void*, "p", a, op, b)


#define tctd_assert_double_equal(a, b, precision) \
  do { \
    const double tctd_tmp_a_ = (a); \
    const double tctd_tmp_b_ = (b); \
    const double tctd_tmp_diff_ = ((tctd_tmp_a_ - tctd_tmp_b_) < 0) ? \
      -(tctd_tmp_a_ - tctd_tmp_b_) : \
      (tctd_tmp_a_ - tctd_tmp_b_); \
    if ((tctd_tmp_diff_ > 1e-##precision)) { \
      tctd_errorf("assertion failed: " #a " == " #b " (%0." #precision "g == %0." #precision "g)", \
           tctd_tmp_a_, tctd_tmp_b_); \
    } \
  } while (0)

#include <string.h>
#define tctd_assert_string_equal(a, b) \
  do { \
    const char* tctd_tmp_a_ = a; \
    const char* tctd_tmp_b_ = b; \
    if ((strcmp(tctd_tmp_a_, tctd_tmp_b_) != 0)) { \
      tctd_errorf("assertion failed: string " #a " == " #b " (\"%s\" == \"%s\")", \
                   tctd_tmp_a_, tctd_tmp_b_); \
    } \
  } while (0)

#define tctd_assert_string_not_equal(a, b) \
  do { \
    const char* tctd_tmp_a_ = a; \
    const char* tctd_tmp_b_ = b; \
    if ((strcmp(tctd_tmp_a_, tctd_tmp_b_) == 0)) { \
      tctd_errorf("assertion failed: string " #a " != " #b " (\"%s\" == \"%s\")", \
                   tctd_tmp_a_, tctd_tmp_b_); \
    } \
  } while (0)

#define tctd_assert_memory_equal(size, a, b) \
  do { \
    const unsigned char* tctd_tmp_a_ = (const unsigned char*) (a); \
    const unsigned char* tctd_tmp_b_ = (const unsigned char*) (b); \
    const size_t tctd_tmp_size_ = (size); \
    if ((memcmp(tctd_tmp_a_, tctd_tmp_b_, tctd_tmp_size_)) != 0) { \
      size_t tctd_tmp_pos_; \
      for (tctd_tmp_pos_ = 0 ; tctd_tmp_pos_ < tctd_tmp_size_ ; tctd_tmp_pos_++) { \
        if (tctd_tmp_a_[tctd_tmp_pos_] != tctd_tmp_b_[tctd_tmp_pos_]) { \
          tctd_errorf("assertion failed: memory " #a " == " #b ", at offset %" tctd_SIZE_MODIFIER "u", tctd_tmp_pos_); \
          break; \
        } \
      } \
    } \
  } while (0)

#define tctd_assert_memory_not_equal(size, a, b) \
  do { \
    const unsigned char* tctd_tmp_a_ = (const unsigned char*) (a); \
    const unsigned char* tctd_tmp_b_ = (const unsigned char*) (b); \
    const size_t tctd_tmp_size_ = (size); \
    if ((memcmp(tctd_tmp_a_, tctd_tmp_b_, tctd_tmp_size_)) == 0) { \
      tctd_errorf("assertion failed: memory" #a " != " #b " (%zu bytes)", tctd_tmp_size_); \
    } \
  } while (0)

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
#define assert_not_null(ptr) tctd_assert_not_null(ptr

#endif /* defined(tctd_ENABLE_ASSERT_ALIASES) */

void _tctd_run_utests(char* group_name, TCTDUTest group_tests[], size_t num_elements){
    size_t i;
    tctd_info("TCTD Unit Tests: %s Start!",group_name);
    for(i = 0; i<num_elements; ++i){
        tctd_print_std(INFO, "Testing: %s...", group_tests[i].name);
        group_tests[i].test_func();
        tctd_info("Testing ends.");
        puts("----------\n");
    }
    puts("All testing end.\n");
} 
#endif
/*
 ********* Debug *********
 */
#ifdef TCTD_DEBUG
/*
1. 使用前先#define TCTD_DEBUG
2. 调试功能主要解决在实际的单步调试中，经常见到的变量查看问题，提供temp_vars接口，如
	temp_vars(temp1, int*, 3)  //声明temp1_1, temp1_2, temp1_3三个整型指针临时变量
3. number从1~15，大于15没用定义
*/
#define DEBUG //NULL macro for debug

#define temp_vars(name, type, number) _generate_temp_vars(name, type, number)

#define _generate_temp_vars(name, type, number) \
            do{\
            if (number == 1)   _generate_temp_vars_1(name, type);\
            else if (number == 2) _generate_temp_vars_2(name, type);\
            else if (number == 3) _generate_temp_vars_3(name, type);\
            else if (number == 4) _generate_temp_vars_4(name, type);\
            else if (number == 5) _generate_temp_vars_5(name, type);\
            else if (number == 6) _generate_temp_vars_6(name, type);\
            else if (number == 7) _generate_temp_vars_7(name, type);\
            else if (number == 8) _generate_temp_vars_8(name, type);\
            else if (number == 9) _generate_temp_vars_9(name, type);\
            else if (number == 10) _generate_temp_vars_10(name, type);\
            else if (number == 11) _generate_temp_vars_11(name, type);\
            else if (number == 12) _generate_temp_vars_12(name, type);\
            else if (number == 13) _generate_temp_vars_13(name, type);\
            else if (number == 14) _generate_temp_vars_14(name, type);\
            else if (number == 15) _generate_temp_vars_15(name, type);\
            else _generate_temp_vars_cannot_deal(name, type);\
            }while(0);

#define _generate_temp_vars_1(name, type)\
            type name;
#define _generate_temp_vars_2(name, type)\
            type name_1;\
            type name_2;
#define _generate_temp_vars_3(name, type)\
            _generate_temp_vars_2(name, type)\
            type name_3;
#define _generate_temp_vars_4(name, type)\
            _generate_temp_vars_3(name, type);\
            type name_4;
#define _generate_temp_vars_5(name, type)\
            _generate_temp_vars_4(name, type);\
            type name_5;
#define _generate_temp_vars_6(name, type)\
            _generate_temp_vars_5(name, type);\
            type name_6;
#define _generate_temp_vars_7(name, type)\
            _generate_temp_vars_6(name, type);\
            type name_7;
#define _generate_temp_vars_8(name, type)\
            _generate_temp_vars_7(name, type);\
            type name_8;
#define _generate_temp_vars_9(name, type)\
            _generate_temp_vars_8(name, type);\
            type name_9;
#define _generate_temp_vars_10(name, type)\
            _generate_temp_vars_9(name, type);\
            type name_10;
#define _generate_temp_vars_11(name, type)\
            _generate_temp_vars_10(name, type);\
            type name_11;
#define _generate_temp_vars_12(name, type)\
            _generate_temp_vars_11(name, type);\
            type name_12;
#define _generate_temp_vars_13(name, type)\
            _generate_temp_vars_12(name, type);\
            type name_13;
#define _generate_temp_vars_14(name, type)\
            _generate_temp_vars_13(name, type);\
            type name_14;
#define _generate_temp_vars_15(name, type)\
            _generate_temp_vars_14(name, type);\
            type name_15;       
#define _generate_temp_vars_cannot_deal(name, type)

#endif

/*
 ******** Utilities *********
 */

/*benchmarking
 */

/**
 * @brief Benchmarking utilities has 1 sets of interface: TCTD_INIT_BENCH(), TCTD_END_BENCH()s and TCTD_SUM_BENCH().
 *
 * TCTD_INIT_BENCH() initilizes the macro, TCTD_END_BENCH() ends the macro, TCTD_SUM_BENCH() prints info.. They need to use sequentially.
 *
 * @param[in] B is the name of benchmarking.
 * 
 * @param[in] RUN is the number of runs
 *
 */

/*
example1:
bench B;
TCTD_INIT_BENCH(B, 3)
func();
TCTD_END_BENCH(B)
TCTD_SUM_BENCH(B)

example2:
getTime(), bench_start(), bench_stop(), bench_print_summary() also provide interface to users.
 */
typedef struct {
    long R; // runs
    time_t start;
    time_t end;
} bench;

time_t getTime();
void bench_start(bench* b);
void bench_stop(bench* b);
void bench_print_summary(bench* b);

#define TCTD_INIT_BENCH(B, RUN) \
        bench B; \
        B.R = RUN>1?RUN:1; \
        int r_##B;\
        bench_start(&B); \
        for (r_##B = 0; r_##B < B.R ; r_##B++ ) { \

#define TCTD_END_BENCH(B) \
            } \
        bench_stop(&B);

#define TCTD_SUM_BENCH(B) bench_print_summary(&B);

/*
1. TCTDLogLevel是输出的级别，LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_ERROR_ERRNO, 最后一种是含有error number的
2. 主要提供7个接口：tctd_print_details可以传入file和line信息，tctd_print_std可以传入format信息，tctd_print_simple不需传入format信息，tctd_info, tctd_warning, tctd_error_errno是tctd__print_simple的封装
3. 在不定义的情况下，输出到标准输出流，如果#define LOGGING，则输出到一个文件中，这个文件是一个FILE*的全局变量，名字是tctd_file，请用户自行管理文件，并且不要忘记关闭文件。
*/

typedef enum {
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
  LOG_ERROR_ERRNO
} TCTDLogLevel;

#define tctd_print_details(LogLevel, file, line, message)\
          tctd_print_standard(LogLevel, file, line, "%s", message)
#define tctd_print_std(LogLevel, format, message) \
          tctd_print_standard(LogLevel,  __FILE__, __LINE__, format, message )
#define tctd_print_simple(LogLevel,  message)\
          tctd_print_std(LogLevel, "%s", message)
#define tctd_error(msg) \
  tctd_print_simple(LOG_ERROR, msg)
#define tctd_info(msg)\
  tctd_print_simple(LOG_INFO, msg)
#define tctd_warning(msg)\
  tctd_print_simple(LOG_WARNING, msg)
#define tctd_error_errno(msg, errno)\
  tctd_print_simple(LOG_ERROR_ERRNO, msg)
//tctd_fp 是一个全局变量
static void tctd_print_standard(TCTDLogLevel type,
                              const char* filename,
                              const int line,
                              const char* format,
                              const char *message)
{
    switch (type) {
    case LOG_INFO:
        if (message != NULL){
          print_info(filename, line, format, message);
          break;
        }
        else{
          print_info(filename, line, format, "This is an INFO.");
          break;
        }
    case LOG_WARNING:
        if (message != NULL) {
            print_warning(filename, line, format, message);
            break;
        }
        else{
          print_warning(filename, line, format, "This is a WARNING.");
          break;
        }
    case LOG_ERROR:
        if (message != NULL) {
            print_error(filename, line, format, message);
            break;
        }
        else{
          print_error(filename, line, format, "This is an ERROR.");
          break;
        }
    case LOG_ERROR_ERRNO:
        if (message != NULL){
          print_error_errno(filename, line, errno, format, message);
          break;
        }
        else{
          print_error_errno(filename, line, errno, format, "This is an ERROR.");
          break;
        }
    }
}


void print_info(const char* filename, const int line, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    vprint_info(filename, line, format, args);
    va_end(args);
}


void vprint_info(const char* filename, const int line, const char* const format, va_list args) {
    char buffer[1024];
    vsprintf(buffer, format, args);
    #ifdef LOGGING
      tctd_fputs("[ INFO ]\n", tctd_fp);
      tctd_fprintf(tctd_fp,"FILE: %s\n",filename);
      tctd_fprintf(tctd_fp,"LINE: %d\n",line);
      tctd_fprintf(tctd_fp, buffer);
      tctd_fprintf(tctd_fp, "\n");
      fflush(tctd_fp);    
    #else
      puts("[ INFO ]\n");
      printf("FILE: %s\n",filename);
      printf("LINE: %d\n", line);
      printf("%s\n", buffer);
      fflush(stdout);
    #endif
}


void print_warning(const char* filename, const int line, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    vprint_warning(filename, line, format, args);
    va_end(args);
}

void vprint_warning(const char* filename, const int line, const char* const format, va_list args) {
    char buffer[1024];
    vsprintf(buffer, format, args);
    #ifdef LOGGING
      tctd_fputs("[ WARNING ]\n", tctd_fp);
      tctd_fprintf(tctd_fp,"FILE: %s\n",filename);
      tctd_fprintf(tctd_fp,"LINE: %d\n",line);
      tctd_fprintf(tctd_fp, buffer);
      tctd_fprintf(tctd_fp, "\n");
      fflush(tctd_fp);
    #else
      puts("[ WARNING ]\n");
      printf("FILE: %s\n",filename);
      printf("LINE: %d\n", line);
      printf("%s\n", buffer);
      fflush(stdout);
    #endif
}

void print_error(const char* filename, const int line, const char* const format, ...) {
    va_list args;
    va_start(args, format);
    vprint_error(filename, line, format, args);
    va_end(args);
}

void vprint_error(const int line, const char* const format, va_list args) {
    char buffer[1024];
    vsprintf(buffer, format, args);
    #ifdef LOGGING
      tctd_fputs("[ ERROR ]\n", tctd_fp);
      tctd_fprintf(tctd_fp,"FILE: %s\n",filename);
      tctd_fprintf(tctd_fp,"LINE: %d\n",line);
      tctd_fprintf(tctd_fp, buffer);
      tctd_fprintf(tctd_fp, "\n");
      fflush(tctd_fp);
    #else
      tctd_fputs(stderr, "[ ERROR ]\n");
      tctd_fprintf(stderr, "FILE: %s\n",filename);
      tctd_fprintf(stderr, "LINE: %d\n", line);
      tctd_fprintf(stderr, "%s\n", buffer);
      fflush(stderr);
   #endif
}

void print_error_errno(const char* filename, const int line, int errno, const char* const format, ...){
    va_list args;
    va_start(args, format);
    vprint_error_errno(filename, line, errno, format, args);
    va_end(args);
}

void vprint_error_errno(const char* filename, const int line, const char* const format, int errno, va_list args){
    char buffer[1024];
    vsprintf(buffer, format, args);
    #ifdef LOGGING
      tctd_fputs("[ ERROR ]\n", tctd_fp);
      tctd_fprintf(tctd_fp,"FILE: %s\n",filename);
      tctd_fprintf(tctd_fp,"LINE: %d\n",line);
      tctd_fprintf(tctd_fp,"ERROR: %s------%d", strerror(errno), errno);
      tctd_fprintf(tctd_fp, buffer);
      tctd_fprintf(tctd_fp, "\n");
      fflush(tctd_fp);
    #else
      tctd_fputs(stderr, "[ ERROR ]\n");
      tctd_fprintf(stderr, "FILE: %s\n",filename);
      tctd_fprintf(stderr, "LINE: %d\n", line);
      tctd_fprintf(stderr,"ERROR: %s------%d", strerror(errno), errno);
      tctd_fprintf(stderr, "%s\n", buffer);
      fflush(stderr);    
    #endif 
}
/*
此处主要是定义了一些安全的内存管理机制，如需内存调试，请使用tctd_malloc_log, tctd_free_log，这将会将所有使用过的记录放在文件中

*/

#define tctd_malloc_(type, size) _tctd_malloc(size, sizeof(type))
#define tctd_malloc(type, size) (type*)tctd_malloc_(type, size)
#define tctd_free(ptr) _tctd_free(ptr)
#define tctd_calloc(type, size) _tctd_calloc(size, sizeof(type))

//with log
#define tctd_malloc_log(type, size) _tctd_malloc_log(size, sizeof(type), __FILE__, __LINE__)
#define tctd_free_log(ptr) __tctd_free_log(ptr, __FILE__, __LINE__)
#define tctd_calloc_log(type, size) _tctd_calloc_log(size, sizeof(type), __FILE__, __LINE__)

void* _tctd_malloc(size_t size); 
void _tctd_free(void* ptr);
void* _tctd_calloc(size_t size);
void* _tctd_malloc_log(size_t size, const char* funname, const char* filename, int line);
void _tctd_free_log(void* ptr, const char* funname, const char* filename, int line);
void* _tctd_calloc_log(size_t size, const char* funname, const char* filename, int line);


//http://blog.csdn.net/hackbuteer1/article/details/6789164
//http://www.cnblogs.com/QG-whz/p/5140930.html
void* _tctd_malloc(size_t n, size_t size){
    void* ptr = NULL;
    ptr = calloc(n, size);
    if(ptr == NULL){
        tctd_error("Fail to malloc!");
        abort();
    }
    return ptr;
}

void _tctd_free(void* ptr){
    if (ptr == NULL){
        tctd_error("Fail to free!");
        abort();
    }
    free(ptr);
    ptr = NULL;
}

void* _tctd_calloc(size_t n, size_t size){
    void* ptr;
    ptr = calloc(n, size);
    if (ptr == NULL){
        tctd_error("Fail to calloc!");
        abort();
    }
    return ptr;
}

void* _tctd_malloc_log(size_t n, size_t size, const char* filename, const int line){
    void* ptr = NULL;
    tctd_print_details(INFO, filename, line, "Start to malloc!");
    ptr = calloc(n, size);
    if(ptr == NULL){
        tctd_print_details(ERROR, filename, line, "Fail to malloc!");
        abort();
    }
    tctd_print_details(INFO, filename, line, "Succeed to malloc!");
    return ptr;
}

void _tctd_free_log(size_t n, size_t size, const char* filename, const int line){
    tctd_print_details(INFO, filename, line, "Start to free!");
    if (ptr == NULL){
        tctd_print_details(ERROR, filename, line, "Fail to free!");
        abort();
    }
    free(ptr);
    ptr = NULL;
    tctd_print_details(INFO, filename, line, "Succeed to free!");
}

void* _tctd_calloc_log(size_t n, size_t size, const char* filename, const int line){
    void* ptr = NULL;
    tctd_print_details(INFO, filename, line, "Start to calloc!");
    ptr = calloc(n, size);
    if(ptr == NULL){
        tctd_print_details(ERROR, filename, line, "Fail to calloc!");
        abort();
    }
    tctd_print_details(INFO, filename, line, "Succeed to calloc!");
    return ptr;
}
/*** Random number generation ***/

void tctd_rand_seed(unsigned int seed);
int tctd_rand_int_range(int min, int max);

void tctd_rand_seed(unsigned int seed){
    if (seed != NULL)
        srand(seed);
    else
        srand(time(0));
}

int tctd_rand_int_range(int max, int min){
    return rand()%min+max;
}


#ifdef __cplusplus
 }
#endif