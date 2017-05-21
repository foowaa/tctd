/*
=======================================================================
Tian C Testing & Debugging (TCTD)
The library impelment a useful C89(ANSI C) unit test framework:
Unit tests with simple assertions inspired from munit.
Safe memory control, random variables generation utilities.
A simple strategy for inspect memory leak in small projects.
Bencmarking inspired from minunit.
Debug tool: temporal variables declaration that convenient for step through.

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
#include "tctd1.h"

/**< When define LOGGING, tctd_fp is the global file handler, the informations write into tctd_fp. And the control of
     File is granted users like open file, close file, etc.. */
void vprint_info(const char* filename, const int line, const char* const format, va_list args) {
    char buffer[1024];
    vsprintf(buffer, format, args);
    #ifdef LOGGING
      fputs("[ INFO ]", tctd_fp);
      fprintf(tctd_fp,"FILE: %s\n",filename);
      fprintf(tctd_fp,"LINE: %d\n",line);
      fprintf(tctd_fp, buffer);
      fprintf(tctd_fp, "\n");
      fflush(tctd_fp);
    #else
      puts("[ INFO ]");
      printf("FILE: %s\n",filename);
      printf("LINE: %d\n", line);
      printf("%s\n", buffer);
      puts("\n");
      fflush(stdout);
    #endif
}

void vprint_warning(const char* filename, const int line, const char* const format, va_list args) {
    char buffer[1024];
    vsprintf(buffer, format, args);
    #ifdef LOGGING
      fputs("[ WARNING ]", tctd_fp);
      fprintf(tctd_fp,"FILE: %s\n",filename);
      fprintf(tctd_fp,"LINE: %d\n",line);
      fprintf(tctd_fp, buffer);
      fprintf(tctd_fp, "\n");
      fflush(tctd_fp);
    #else
      puts("[ WARNING ]");
      printf("FILE: %s\n",filename);
      printf("LINE: %d\n", line);
      printf("%s\n", buffer);
      puts("\n");
      fflush(stdout);
    #endif
}


void vprint_error(char* filename, const int line, const char* const format, va_list args) {
    char buffer[1024];
    vsprintf(buffer, format, args);
    #ifdef LOGGING
      fputs("[ ERROR ]\n", tctd_fp);
      fprintf(tctd_fp,"FILE: %s\n",filename);
      fprintf(tctd_fp,"LINE: %d\n",line);
      fprintf(tctd_fp, buffer);
      fprintf(tctd_fp, "\n");
      fflush(tctd_fp);
    #else
      fputs("[ ERROR ]\n", stderr);
      fprintf(stderr, "FILE: %s\n",filename);
      fprintf(stderr, "LINE: %d\n", line);
      fprintf(stderr, "%s\n", buffer);
      fputs("\n", stderr);
      fflush(stderr);
   #endif
}

void vprint_error_errno(const char* filename, const int line, const char* const format, va_list args){
    char buffer[1024];
    vsprintf(buffer, format, args);
    #ifdef LOGGING
      fputs("[ ERROR ]\n", tctd_fp);
      fprintf(tctd_fp,"FILE: %s\n",filename);
      fprintf(tctd_fp,"LINE: %d\n",line);
      fprintf(tctd_fp,"ERROR: %s------%d", strerror(errno), errno);
      fprintf(tctd_fp, buffer);
      fprintf(tctd_fp, "\n");
      fflush(tctd_fp);
    #else
      fputs("[ ERROR ]\n", stderr);
      fprintf(stderr, "FILE: %s\n",filename);
      fprintf(stderr, "LINE: %d\n", line);
      fprintf(stderr,"ERROR: %s------%d", strerror(errno), errno);
      errno = 0;
      fprintf(stderr, "%s\n", buffer);
      fputs("\n", stderr);
      fflush(stderr);
    #endif
}
void tctd_print_standard(TCTDLogLevel LogLevel, char* filename, int line, char* format, ...){
    va_list args;
	switch (LogLevel){
		case LOG_INFO:
			va_start(args, format);
			vprint_info(filename, line, format, args);
			va_end(args);
			break;
		case LOG_WARNING:
			va_start(args, format);
			vprint_warning(filename, line, format, args);
			va_end(args);
			break;
		case LOG_ERROR:
			va_start(args, format);
			vprint_error(filename, line, format, args);
			va_end(args);
			break;
		case LOG_ERROR_ERRNO:
			va_start(args, format);
			vprint_error_errno(filename, line, format, args);
			va_end(args);
			break;
	}
}

/**< memory control utilities */
void* _tctd_malloc(size_t n, size_t size){
    void* ptr = NULL;
    ptr = calloc(n, size);
    if(ptr == NULL){
        fprintf(stderr, "Fail to malloc!");
        exit(1);
    }
    return ptr;
}

void _tctd_free(void* ptr){
    if (ptr == NULL){
        fprintf(stderr, "Fail to free!");
        exit(1);
    }
    free(ptr);
    ptr = NULL;
}

void* _tctd_calloc(size_t n, size_t size){
    void* ptr;
    ptr = calloc(n, size);
    if (ptr == NULL){
        fprintf(stderr, "Fail to calloc!");
        exit(1);
    }
    return ptr;
}

void* _tctd_malloc_log(size_t n, size_t size, char* filename, int line){
    void* ptr = NULL;
    tctd_print_standard(LOG_INFO, filename, line, "%s", "Start to malloc!");
    ptr = calloc(n, size);
    if(ptr == NULL){
        tctd_print_standard(LOG_ERROR, filename, line, "%s", "Fail to malloc!");
        exit(1);
    }
    tctd_print_standard(LOG_INFO, filename, line, "%s", "Succeed to malloc!");
    return ptr;
}

void _tctd_free_log(void* ptr, char* filename, int line){
    tctd_print_standard(LOG_INFO, filename, line, "%s", "Start to free!");
    if (ptr == NULL){
        tctd_print_standard(LOG_ERROR, filename, line, "%s", "Fail to free!");
        exit(1);
    }
    free(ptr);
    ptr = NULL;
    tctd_print_standard(LOG_INFO, filename, line, "%s", "Succeed to free!");
}

void* _tctd_calloc_log(size_t n, size_t size, char* filename, int line){
    void* ptr = NULL;
    tctd_print_standard(LOG_INFO, filename, line, "%s", "Start to calloc!");
    ptr = calloc(n, size);
    if(ptr == NULL){
        tctd_print_standard(LOG_ERROR, filename, line, "%s", "Fail to calloc!");
        exit(1);
    }
    tctd_print_standard(LOG_INFO, filename, line, "%s", "Succeed to calloc!");
    return ptr;
}

/**< r.v. implementation */
void tctd_rand_seed(unsigned int seed){
    if (seed != 0)
        srand(seed);
    else
        srand(time(0));
}

int tctd_rand_int_range_1(int min, int max){
    return rand()%(max-min)+min;
}

int tctd_rand_int_range_2(int min, int max){
    return rand()%(max-min+1)+min;
}

int tctd_rand_int_range_3(int min, int max){
    return rand()%(max-min)+min+1;
}

int tctd_rand_int_range_4(int min, int max){
    return rand()%(max-min+1)+min-1;
}

double _surand(){
  return rand()/RAND_MAX;
}

double tctd_urand(double low, double high){
  return low+(high-low)*_surand();
}

/**< running tests here */
void _tctd_run_utests(char* group_name, TCTDUTest group_tests[], size_t num_elements){
    size_t i;
    errno = 0;
    printf("TCTD Unit Tests Group: %s Start!\n", group_name);
    puts("==========\n");
    for(i = 0; i<num_elements; ++i){
        printf("Test Case: %s...\n", group_tests[i].name);
        tctd_count[0] = 0; tctd_count[1] = 0;
        puts("----------\n");
        group_tests[i].test_func();
        puts("Test ends.\n");
        if (tctd_count[0] == 0){
            puts("No test cases run!");
        }
        else{
            printf("%.2f%% passed!\n", (float)((1-tctd_count[1])/tctd_count[0])*100);
        }

        puts("----------\n");
    }
    puts("==========\n");
    puts("All Tests Finish.\n");
}

/**< benchmarking */
time_t getTime(){
  time_t t;
  time(&t);
  return t;
}
void bench_start(bench* b){
  b->start = getTime();
}
void bench_stop(bench* b){
  b->end = getTime();
}
void bench_print_summary(bench *b) {
  printf("\n Benchmarking...\n");
    printf("-> %ld runs, time of duration is %lf seconds\n", b->R, difftime(b->end, b->start) );
    printf("-> every run spends %.2f sec\n", (float) (difftime(b->end, b->start)/ b->R) );
}
