#include "gtest/gtest.h"
#include "downhill_simplex.h"


TEST(minimization, downhill_simplex)
{
	auto const res = amoeba(50.);
	ASSERT_NEAR(res, 0., 1.0E-10);
}


TEST(minimization, downhill_simplex_m)
{
	auto const res = amoeba(50.);
	ASSERT_NEAR(res, 0., 1.0E-10);
}
