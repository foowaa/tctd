/*
A simple test and example
 */

#define TCTD_UTEST
#define TCTD_DEBUG
#include "tctd1.h"

//A function
int ceilInt(int a, int b){
    double resultFloat;
    int resultInt;
    if (b == 0){
        fprintf(stderr, "b==0\n");
        exit(1);
    }
    else{
        resultInt = a/b;
        resultFloat = a/b;
        if (resultFloat > 0)
            return resultInt+1;
        else
            return resultInt;
    }
}

//benchmarking test instance
void benchTest(){
    int i;
    for (i=0; i<1000000000; i++);
}

//define test case 1
Test testf1(){
    int a = 1;
    int b = 2;
    tctd_assert_int(a, ==, b);
}
//define test case 2
Test testf2(){
    int k = ceilInt(7, 3);
    tctd_assert_int(k, ==, 3);
}


int main(){
    int* x;
    bench B; //for benchmarking
    //declare temp variables
    temp_vars(temp, float* ,4) //temp_1, temp_2, temp_3 and tmep_4. Type: float*
    //define TCTDUTest with tctd_utest macro and run it in tctd_run_utests
    TCTDUTest utest[2] = {tctd_utest(testf1), tctd_utest(testf2)};
    tctd_run_utests(utest);
    //safe malloc with logging
    x = tctd_malloc_log(int, 3);
    //benchmarking
    TCTD_INIT_BENCH(B, 3)
    benchTest();
    TCTD_END_BENCH(B)
    TCTD_SUM_BENCH(B)
    return 0;
}
