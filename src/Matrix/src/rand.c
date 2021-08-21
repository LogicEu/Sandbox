#include <Matrix/Matrix.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592654

double rand_normal()
{
    return (double)rand() / (double)RAND_MAX;
}

double gaussrand()
{
	static double U, V;
	static int phase = 0;
	double Z;

	if (phase == 0) {
		U = (rand() + 1.) / (RAND_MAX + 2.);
		V = rand() / (RAND_MAX + 1.);
		Z = sqrt(-2 * log(U)) * sin(2 * PI * V);
	} else {
		Z = sqrt(-2 * log(U)) * cos(2 * PI * V);
    }

	phase = 1 - phase;
	return Z;
}

double rand_distrib(double standard_deviation, double mean)
{
    return gaussrand() * standard_deviation + mean;
}