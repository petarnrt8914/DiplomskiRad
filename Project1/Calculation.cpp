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


// True baricentric form
float Calculation::LagrangeInterpolation(array<PointF^>^ points, int pointsCount,
																				 array<double>^ baricentricWeights, float newPointX) {
	double deljenik, delilac;
	deljenik = delilac = 0;

	for (int i = 0; i<pointsCount; i++) {
		double temp = baricentricWeights[i] / (newPointX - points[i]->X);
		deljenik += temp * points[i]->Y;
		delilac += temp;
	}

	return deljenik/delilac;
}


double Calculation::NewtonBasisPolynomial(array<PointF^>^ points, int length, double newX) {
	double result = 1.0;

	//if length==0 return 1
	for (int i=0; i<length; i++)
		result *= newX - points[i]->X;

	return result;
}

//MAYBE split
//MAYBE join them to be the same
double Calculation::DividedDifference(array<PointF^>^ points, int start, int end) {
	if (start==end) return points[start]->Y;

	double result;
	if (start<end) { //forward 
		result = DividedDifference(points, start+1, end) - DividedDifference(points, start, end-1);
		result /= points[end]->X - points[start]->X;
	}
	else { //backward
		result = DividedDifference(points, start, end+1) - DividedDifference(points, start-1, end);
		result /= points[start]->X - points[end]->X;
	}

	return result;
}


//MAYBE change newPointX to double
float Calculation::NewtonInterpolation(array<PointF^>^ points, float newPointX) {
	double newY = 1.0;
	bool isEqualSpacing = false;
	bool isNewXCloserToStart = newPointX - points[0]->X <= points[points->Length-1]->X - newPointX;

	//TODO uncomment
	//double spacing = points[1]->X - points[0]->X;
	//if (points->Length>=3) {
	//	isEqualSpacing = true;
	//	for (int i=1; i<points->Length-1; i++)
	//		if (points[i]->X - points[i+1]->X != spacing) {
	//			isEqualSpacing = false;
	//			break;
	//		}
	//}

	if (isEqualSpacing) {
		//TODO 
		//double newXRatio = (newPointX - points[0]->X) / spacing;

		//for (int i = 0; i<points->Length; i++) {
		//	newY *= 
		//}
	}
	else for (int i = 0; i<points->Length; i++) {
		newY *= isNewXCloserToStart ? DividedDifference(points, 0, i) : DividedDifference(points, i, 0);
		newY *= NewtonBasisPolynomial(points, i, newPointX);
	}
}


