#include "SIMD.h"

#include "cpuid.h"
#include <pmmintrin.h>
#include <immintrin.h>

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


void add()
{
    auto ra = _mm_set_ps(1, 2, 3, 4);
    auto rb = _mm_set_ps(4, 3, 2, 1);
    auto rc = _mm_add_ps(ra, rb);
}