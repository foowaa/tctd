# tctd unit test & debug framework

[![GitHub license](http://7xs6jl.com1.z0.glb.clouddn.com/svgapache2.svg)](./LICENSE)
[![Build Status](https://travis-ci.org/foowaa/tctd.svg?branch=master)](https://travis-ci.org/foowaa/tctd)

## Unit Testing
The basic architecture of the framework on unit testing is [https://cmocka.org](https://cmocka.org "Cmocka"), but I always feel that it is complex to use. Therefore, I write the framework and the users needn't any compiling, it's a single header file and a source file. The weakness is the implemention and interface are fused, but plz accord to my document. 

The interfaces of unit testing are: Test, tctd\_utest, TCTDUTest, tctd\_run\_utests and assertions.

1. `Test` is the protocol for test instance. i.e.
	
	Test testFunc1(){...}

2. `tctd_utest` is wrapper of `Test` function. i.e.

	tctd_utest(testFunc1) 

3. Use type `TCTDUTest` to make test groups.

	TCTDUTest testGropu[2] = {tctd_utest(testFunc`), tctd_utest(testFunc2)}

4. `tctd_run_utests` is a macro running the test functions. tctd\_run\_utests(utest)

	tctd_run_utest(testGroup)

5. The assertions use [https://nemequ.github.io/munit/#assertions](https://nemequ.github.io/munit/#assertions "munit").


## Debugging
The modern IDEs (visual studio, eclipse,...) have good debugging tools, but in practice, step through is an important technique for deugging. In the complex code, coders often want to watch the variables value. The debugging offers the convenient temporary variables declaration.

1. #define TCTD_DEBUG
2. You can use DEBUG macro in the debuged functions.

	DEBUG temp_vars(t, float*, 3) 
	//This will declare 3 float* temp var.: t_1, t_2, t_3

3. The biggest number is 15.

## Utilities
The utilities serve both unit testing, debugging and the users. There are 4 parts:

1. ouput
2. memory control
3. benchmarking
4. random number generation

### Output
A global variables tctd_file is used here. FILE* tctd_file is the logging file used in the output. If define LOGGING, you shoule provide tctd_file to write. And file control is given to the users, open and close. If not define LOGGING, stdout and stderr are used.

### Memory control
Wrap the malloc and free which make them safe and friendly. Altogether, tctd can write the log of usage of malloc and free to tctd_file that is checked memory leak.

1.  tctd_malloc(type, size)
2.  tctd_free(ptr)
3.  tctd_malloc_log(type, size)
4.  tctd_free_log(ptr)

These functions also are convenient for memory leak inspection.
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
tctd_rand_seed() will set random seed.

tctd_rand_int_range_*() generate integers in different intervals.

tctd_urand() generates uniform distribution in [a,b].
