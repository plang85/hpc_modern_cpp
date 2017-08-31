#include "downhill_simplex.h"

#include <cstdlib>
#include <cmath>

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


/**
Extrapolates by a factor fac through the face of the simplex across from the point, 
tries it, and replaces the high point if the new point is better.
*/
double amotry(double **p, double y[], double psum[], int ndim, 
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


double amoeba(double **p, double y[], int ndim, double ftol, double (*funk)(double []), int *nfunk)
{
	int i, ihi, ilo, inhi, j, mpts=ndim+1;
	double rtol, sum, swap, ysave, ytry, *psum;

	psum = vector(1, ndim);
	*nfunk = 0;
	GET_PSUM
	for (;;)
	{
		ilo = 1;
		// first determine which point is the highest (worst), next-highest, and lowest
		// (best) by looping over the points in the simplex
		ihi = y[1] > y[2] ? (ihi=2,1) : (ihi=1,2);
		for (i=1; i <= mpts; i++)
		{
			if(y[i] <= y[ilo]) ilo = i;
			if(y[i] > y[ihi])
			{
				inhi = ihi;
				ihi = i;
			}
			else if (y[i] > y[ihi] && i != ihi)
			{
				inhi = i;
			}
		}
		rtol = 2.0 * fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo])+TINY);
		// compute the fractional range from highest to lowest and return if good
		if(rtol < ftol)
		{
			SWAP(y[i], y[ilo])
			for (i = 1; i <= ndim; i++) SWAP(p[1][i], p[ilo][i])
			break;
		}
		// ignore NMAX violation here
		*nfunk += 2;
		// begin a new iteration by first extrapolating by a factor -1 through the 
		// face of the simplex across from the high point, i.e., reflect simplex
		ytry = amotry(p, y, psum, ndim, funk, ihi, 2.);
		if (ytry <= y[ilo])
		{
			// gives a result better than the best point, so try an additional extrapolation
			// by a factor of two
			ytry = amotry(p, y, psum, ndim, funk, ihi, 2.);
		}
		else if (ytry >= y[inhi])
		{
			// the reflected point is worse than the second-highest, so look for an intermediate
			// lower point, i.e., do a one-dimensional contraction
			ysave = y[ihi];
			ytry = amotry(p, y, psum, ndim, funk, ihi, 0.5);
			if (ytry >= ysave)
			{
				// cant seem to get rid of that high point, better contract around the lowest pt
				for (i=1; i <= mpts; i++)
				{
					if (i != ilo)
					{
						for (j=1; j <= ndim; j++)
							p[i][j] = psum[j] = 0.5 * (p[i][j]+p[ilo][j]);
						y[i] = (*funk)(psum);
					}
				}
				*nfunk += ndim;
				GET_PSUM
			} else --(*nfunk);
		}
	free_vector(psum, 1, ndim);
	}
	return 0.;
}


