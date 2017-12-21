#include "SIMD.h"

#include "cpuid.h"


cpu_info_t cpu_info(int i)
{
    cpu_info_t data = {0};

    __get_cpuid (i, std::addressof(data[0]), std::addressof(data[1]), 
                 std::addressof(data[2]), std::addressof(data[3]));

    return data;
}



void mult(float const *a, float const *b, float *c)
{

}