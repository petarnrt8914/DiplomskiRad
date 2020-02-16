#include "Calculation.h"

//#define HIGH_ACCURACY

float Calculation::LagrangeInterpolation(array<PointF^>^ points, float newPointX) {
	double result = 0;

	for (int i=0, j; i<points->Length; i++) {
	#ifdef HIGH_ACCURACY

		double deljenikTemp = points[i]->Y;
		double delilacTemp = 1;
		for (j = 0; j<size; j++) {
			if (j!=i) {
				deljenikTemp *= (newPointX - points[j]->X);
				delilacTemp *= (points[i]->X - points[j]->X);
			}
		}
		result += deljenikTemp / delilacTemp;

	#else
		double basisPolynomial = points[i]->Y;

		for (j = 0; j<points->Length; j++)
			if (j!=i)
				basisPolynomial *= double(newPointX - points[j]->X) / (points[i]->X - points[j]->X);

		result += basisPolynomial;
	#endif
	}

	return result;
}


// True baricentric form
float Calculation::LagrangeInterpolation(array<PointF^>^ points, array<double>^ baricentricWeights, float newPointX) {
	double deljenik, delilac;
	deljenik = delilac = 0;

	for (int i = 0; i<points->Length; i++) {
		double temp = baricentricWeights[i] / (newPointX - points[i]->X);
		deljenik += temp * points[i]->Y;
		delilac += temp;
	}

	return deljenik/delilac;
}


