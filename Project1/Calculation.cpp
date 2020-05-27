#include "Calculation.h"
using System::Math;

//#define HIGH_ACCURACY

double Calculation::LagrangeInterpolation(array<PointF^>^ points, double newPointX) {
	double newY = 0;

	for (int i=0, j; i<points->Length; i++) {
	#ifdef HIGH_ACCURACY

		double deljenikTemp = points[i]->Y;
		double delilacTemp = 1;
		for (j = 0; j<size; j++) {
			if (j!=i) {
				deljenikTemp *= (newPointX - points[j]->X);
				delilacTemp *= (static_cast<double>(points[i]->X) - points[j]->X);
			}
		}
		newY += deljenikTemp / delilacTemp;

	#else
		newY += points[i]->Y * LagrangeBasisPolynomial(points, i, newPointX);
	#endif
	}

	return newY;
}


inline double Calculation::LagrangeBasisPolynomial(array<PointF^>^ points, int index, double newPointX) {
	double basisPolynomial = 1.0;

	for (int j = 0; j<points->Length; j++)
		if (j!=index)
			basisPolynomial *= (newPointX - points[j]->X) / (static_cast<double>(points[index]->X) - points[j]->X);

	return basisPolynomial;
}


// True baricentric form
double Calculation::LagrangeInterpolation(array<PointF^>^ points, array<double>^ reciprocalBaricentricWeights, double newPointX) {
	double deljenik, delilac;
	deljenik = delilac = 0;
	if (reciprocalBaricentricWeights == nullptr)
		reciprocalBaricentricWeights = ReciprocalBaricentricWeights(points);

	for (int i = 0; i<points->Length; i++) {
		double temp = (1/reciprocalBaricentricWeights[i]) / (newPointX - points[i]->X);
		deljenik += temp * points[i]->Y;
		delilac += temp;
	}

	return deljenik/delilac;
}

array<double>^ Calculation::ReciprocalBaricentricWeights(array<PointF^>^ points) {
	//initializing
	auto weights = gcnew array<double>(points->Length);
	weights[0] = 1.0;
	//for (int j = 1; j<weights->Length; ++j) {
	//	weights[j] = points[0]->X - points[j]->X;
	//}
	for (int j = 1; j<weights->Length; ++j) {
		weights[0] *= points[0]->X - points[j]->X;
		weights[j] = points[j]->X - points[0]->X;
	}

	//calculating
	//for (int i = 1, j; i<points->Length; ++i) {
	//	for (j = 0; j<points->Length; ++j) {
	//		if (j != i) 
	//			weights[j] *= points[i]->X - points[j]->X;
	//	}
	//}
	for (int i=1, j; i<points->Length; ++i) {
		for (j=i+1; j<points->Length; ++j) {
			weights[i] *= points[i]->X - points[j]->X;
			weights[j] *= points[j]->X - points[i]->X;
		}
	}
	return weights;
}

void Calculation::AddBaricentricWeight(array<double>^% existingWeights, array<PointF^>^ points, int indexOfNewPoint) {
	auto newArrayForWeights = gcnew array<double>(existingWeights->Length+1);
	newArrayForWeights[indexOfNewPoint] = 1.0;

	for (int i = 0; i<indexOfNewPoint; ++i) {
		if (i == indexOfNewPoint) continue;

		int originalIndex = i<indexOfNewPoint ? i : i-1;
		newArrayForWeights[i] = existingWeights[originalIndex] * (points[i]->X - points[indexOfNewPoint]->X);
		newArrayForWeights[indexOfNewPoint] *= points[indexOfNewPoint]->X - points[i]->X;
	}
	delete existingWeights;
	existingWeights = newArrayForWeights;
}


double Calculation::NewtonInterpolation(array<PointF^>^ points, double newPointX) {
	return NewtonInterpolation(points, gcnew TriangularMatrix(points) , newPointX);
}

inline double Calculation::NewtonBasisPolynomial(array<PointF^>^ points, int length, double newX) {
	double result = 1.0;

	//if length==0 return 1
	for (int i = 0; i<length; i++)
		result *= newX - points[i]->X;

	return result;
}

//MAYBE split
//MAYBE join them to be the same
inline double Calculation::DividedDifference(array<PointF^>^ points, int start, int end) {
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


double Calculation::NewtonInterpolation(array<PointF^>^ points, TriangularMatrix^ DividedDifferenceTable, double newPointX) {

	double newY = 0.0;
	bool isEqualSpacing = true; //until proven otherwise
	bool isNewXCloserToStart = (newPointX - points[0]->X) <= (points[points->Length-1]->X - newPointX);

	//checking if the spacing is uniform
	double spacing = points[1]->X - points[0]->X;
	if (points->Length>=3) {
		for (int i=1; i<points->Length-1; i++)
			if (static_cast<double>(points[i+1]->X) - points[i]->X != spacing) {
				isEqualSpacing = false;
				break;
			}
	}

	if (DividedDifferenceTable == nullptr) DividedDifferenceTable =	gcnew TriangularMatrix(points);

	if (isEqualSpacing) { 
		double newXRatio;
		long double tmp = 1.0;

		if (isNewXCloserToStart) {
			newXRatio = (newPointX - points[0]->X) / spacing;
			for (int i=0; i<points->Length; i++) {
				//Newton forward divided difference formula
				tmp = DividedDifferenceTable[i,0];
				tmp *= System::Math::Pow(spacing, i);
				for (int j = 0; j<i; j++)		tmp *= newXRatio - j;

				newY += tmp;
			}
		}
		else {
			newXRatio = (newPointX - points[points->Length-1]->X) / spacing;
			int last = points->Length-1;
			for (int i=0; i<points->Length; i++) {
				//Newton backward divided difference formula
				tmp = DividedDifferenceTable[last, last-i];
				tmp *= System::Math::Pow(spacing, i);
				for (int j = 0; j<i; j++)		tmp *= newXRatio + j;

				newY += tmp;
			}
		}

	}
	else // if spacing not equal
		for (int i = 0; i<points->Length; i++) {
			double temp = DividedDifferenceTable[0,i];
									//isNewXCloaserToStart
										//? DividedDifference(points, 0, i)
										//: DividedDifference(points, i, 0);
			temp *= NewtonBasisPolynomial(points, i, newPointX);
			newY += temp;
		}

	return newY;
}


