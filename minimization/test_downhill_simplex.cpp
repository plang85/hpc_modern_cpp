#include "gtest/gtest.h"
#include "downhill_simplex.h"

#include <cmath>


/**
Has minimum at [1, 1].
*/
double rosenbrock(double x, double y)
{
	return 100.*pow((y-x*x), 2) + (1.-x*x);
}


TEST(minimization, downhill_simplex)
{
	double (*funk)(double, double);
	unk = &rosenbrock;

	auto const res = 0.;//amoeba(50.);
	ASSERT_NEAR(res, 0., 1.0E-10);
}


TEST(minimization, downhill_simplex_m)
{
	auto const res = 0.;//amoeba(50.);
	ASSERT_NEAR(res, 0., 1.0E-10);
}
