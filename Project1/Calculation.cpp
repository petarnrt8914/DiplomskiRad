#include "Calculation.h"

//#define HIGH_ACCURACY

float Calculation::LagrangeInterpolation(array<PointF^>^ points, int size, float newPointX) {
	double result = 0;

	for (int i=0, j; i<size; i++) {
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

		for (j = 0; j<size; j++)
			if (j!=i)
				basisPolynomial *= double(newPointX - points[j]->X) / (points[i]->X - points[j]->X);

		result += basisPolynomial;
	#endif
	}

	return result;
}