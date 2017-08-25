#include "gtest/gtest.h"
#include "downhill_simplex.h"

TEST(downhill_simplex, test_one)
{
	auto const res = test_me(50.);
	ASSERT_NEAR(res, 50.*50., 1.0E-10);
}

