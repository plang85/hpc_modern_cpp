#include "downhill_simplex.h"

#include <cstdlib>

#define NR_END 1
#define FREE_ARG char*

#define TINY 1.0E-10
#define NMAX 5000
#define GET_PSUM \
  for (j = 1; j <= ndim; j++) {\
	for (sum = 0.0, i = 1; i <= mpts; i++) sum += p[i][j];\
	psum[j] = sum;}
#define SWAP(a, b) { swap = (a); (a) = (b); (b) = swap; }



double *vector(long nl, long nh)
{
	double *v = (double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	// ignore allocation error
	return v-nl+NR_END;

}


void free_vector(double *v, long nl, long nh)
{
	free((FREE_ARG) (v+nl-NR_END));
}


double amoeba(double **p, double y[], int ndim, double ftol, double (*funk)(double []), int *nfunk)
{
	return 0.;
}


/**
Extrapolates by a factor fac through the face of the simplex across from the point, 
tries it, and replaces the high point if the new point is better.
*/
double ammotry(double **p, double y[], double psum[], int ndim, 
  double (*funk)(double []), int ihi, double fac)
{
	int j;
	double fac1, fac2, ytry, *ptry;

	ptry = vector(1, ndim);
	fac1 = (1.-fac)/ndim;
	fac2 = fac1-fac;
	for (j=1; j<=ndim; j++)
	{
		ptry[j] = psum[j]*fac1-p[ihi][j]*fac2;
	}
	ytry = (*funk)(ptry);
	if (ytry < y[ihi])
	{
		y[ihi] = ytry;
		for (j=1; j<=ndim; j++)
		{
			psum[j] += ptry[j]-p[ihi][j];
			p[ihi][j] = ptry[j];
		}
	}
	free_vector(ptry, 1, ndim);
	return ytry;
}

