# tctd unit test and debug framework

## Unit Testing
The basic architecture of the framework on unit testing is [https://cmocka.org](https://cmocka.org "Cmocka"), but I always feel that it is complex though it is uncomlicated. Therefore, I write the framework and the users needn't any compiling, it's a single header file. The weakness is the implemention and interface are fused, but plz accord to my document. Firstly, define TCTD\_UTEST which setup the unit testing.

The interfaces of unit testing are: test\_func, TCTD\_UTEST, tctd\_run\_utests and assertions.

1. test\_func is wrapper of test function. The agreement is: void test\_func(). Please use the wrapper.

2. tctd\_utest is a macro which wraps the test function. tctd\_utest(test_func1)

3. TCTDUTest is a struct containing tctd_utest. TCTDUTest utest = {tctd_utest(f1), tctd_utest(f2)};

4. tctd\_run\_utests is a macro running the test functions. tctd\_run\_utests(utest);

5. The assertions use [https://nemequ.github.io/munit/#assertions](https://nemequ.github.io/munit/#assertions "munit").


## Debugging
The modern IDEs (visual studio, eclipse,...) have good debugging tools, but in practice, step through is an important technique for deugging. In the complex code, coders often want to watch the variables value. The debugging offers the convenient temporary variables declaration.

1. #define TCTD_DEBUG
2. You can use DEBUG macro in the debuged functions. DEBUG int debugedFunc()
3. temp_vars(name, type, number) is the main macro (1<=number<=15). temp_vars(temp1, int*, 3) will declare int* temp1_1, int* temp1_2, int*temp1_3.

## Utilities
The utilities serve both unit testing, debugging and the users. There are 4 parts:

1. ouput
2. memory control
3. benchmarking
4. random number generation

### Output
A global variables tctd_file is used here. FILE* tctd_file is the logging file used in the output. If define LOGGING, you shoule provide tctd_file to write. And file control is given to the users, open and close. If not define LOGGING, stdout and stderr are used.

1. TCTDLogLevel is the levels of logging: LOG_INFO, LOG_WARNING, LOG_ERROR, LOG_ERROR_ERRNO
2. tctd_print_std(LogLevel, format, message)
3. tctd_print_simple(LogLevel,  message)
4. tctd_error(msg), tctd_info(msg), tctd_warning(msg), tctd_error_errno(msg)
5. _errno will use <errno.h> to get errno.

### Memory control
Wrap the malloc and free which make them safe and friendly. Altogether, tctd can write the log of usage of malloc and free to tctd_file that is checked memory leak.

1.  tctd_malloc(type, size)
2.  tctd_free(ptr)
3.  tctd_malloc_log(type, size)
4.  tctd_free_log(ptr)

### Benchmarking
2 types of interfaces are provided: macro and fucntions.

1. 

	bench B;

	TCTD\_INIT\_BENCH(B, 3)

	func();

	TCTD\_END\_BENCH(B)

	TCTD\_SUM\_BENCH(B)

2. getTime(), bench_start(), bench_stop(), bench_print_summary() also provide interface to users.


### Random number generation.
tctd\_rand\_seed(unsigned int seed), tctd_rand_int_range(int min, int max) will set seed and rand integer generating.
