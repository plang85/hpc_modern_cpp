#include "gtest/gtest.h"

#include <omp.h>
#include <math.h>
#include <iostream>

int num_threads = 2;

void ompone()
{
    omp_set_num_threads(num_threads);

    int const len = 1024 * 1024 * 64;
    float *a = new float [len];
    float *b = new float [len];
    float *c = new float [len];
    float *r = new float [len];

    # pragma omp parallel for
    for(int i = 0; i < len; ++i)
    {
        r[i] = a[i] + b[i] * erff(c[i]);
    }

    delete []a;
    delete []b;
    delete []c;
    delete []r;
};


void omptwo()
{
#pragma omp parallel sections
{
    #pragma omp section
    {
        for (int i = 0; i < 1000; ++i)
        {
            std::cout << i;
        }
    }
    #pragma omp section
    {
        for (int i = 0; i < 1000; ++i)
        {
            std::cout << static_cast<char>('a' + (i % 26));
        }
    }
}
}


void ompthree()
{
#pragma omp parallel
{
#pragma omp single
    printf("gathering input: %d\n", omp_get_thread_num());
    printf("in parallel on: %d\n", omp_get_thread_num());

#pragma omp barrier

#pragma omp master
    printf("output on: %d\n", omp_get_thread_num());
}
}

int sum_sync()
{
    int sum = 0;
    #pragma omp parallel for num_threads(128)
    for (int i = 0; i < 100; ++i)
    {
        #pragma omp atomic
        ++sum;
    }

    return sum;
}


int sharing()
{
    int i = 10;
    #pragma omp parallel for firstprivate(i)
    for (int j = 0; j < 10; ++j)
    {
        i = 1000 + omp_get_thread_num();
    }

    return i;
}



TEST(OPENMP, one)
{	
    ompone();
    omptwo();
    ompthree();
    ASSERT_EQ(sum_sync(), 100);
    ASSERT_EQ(sharing(), 10);
}
