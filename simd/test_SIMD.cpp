#include "gtest/gtest.h"
#include "SIMD.h"

#include <iostream>



TEST(SIMD, cpu_info)
{	
    auto const res = cpu_info(0);
    cpu_info_t ref = {13, 0, 0, 0};
    for (size_t i = 0; i < 1; ++i)
	    ASSERT_EQ(res[i], ref[i]) << "Incorrect CPU info at i = " << i << ": " << res[i];
}


TEST(SIMD, mult)
{	
    float const a[] = {1.f, 2.f, 3.f, 4.f};
    float const b[] = {5.f, 6.f, 3.f, 2.f};
    float c[4] = {0.f};
    mult(a, b, c);
	//ASSERT_NEAR(c[4], 5.f, 1.0E-6);
}



