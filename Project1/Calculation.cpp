#include "Calculation.h"
using System::Math;

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


inline double Calculation::NewtonBasisPolynomial(array<PointF^>^ points, int length, double newX) {
	double result = 1.0;

	//if length==0 return 1
	for (int i=0; i<length; i++)
		result *= newX - points[i]->X;

	return result;
}

void Calculation::FillTable(array<PointF^>^ points, bool isNewXCloserToStart) {
	throw gcnew System::NotImplementedException();
	if (DividedDifferenceTable != nullptr) {
		for (int i=0; i<points->Length; i++)
			delete DividedDifferenceTable[i];

		delete DividedDifferenceTable;
	}
	else
		DividedDifferenceTable = gcnew List<List<double>^>(points->Length);

	if (isNewXCloserToStart)
		for (int i=0, j; i<points->Length; i++) {
			DividedDifferenceTable[i] = gcnew List<double>(points->Length);
			DividedDifferenceTable[i]->Insert(i, points[i]->Y);

			for (j=i-1; j>=0; j--) {
				double value = DividedDifferenceTable[j+1]->default[i] - DividedDifferenceTable[j]->default[i-1];
				value /= points[i]->X - points[j]->X;
				DividedDifferenceTable[j]->Insert(i, value);
			}
		}
	else
		for (int i = 0, j; i<points->Length; i++) {
			DividedDifferenceTable[i] = gcnew List<double>(i+1);
			DividedDifferenceTable[i]->Insert(i, points[i]->Y);

			for (j=i-1; j>=0; j--) {
				double value = DividedDifferenceTable[i]->default[j+1] - DividedDifferenceTable[i-1]->default[j];
				value /= points[i]->X - points[j]->X;
				DividedDifferenceTable[i]->Insert(j, value);
			}
		}
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
	if (points->Length==1) return points[0]->Y;

	double newY = 0.0;
	bool isEqualSpacing = true; //until proven otherwise
	bool isNewXCloserToStart = (newPointX - points[0]->X) <= (points[points->Length-1]->X - newPointX);

	//checking if the spacing is uniform
	double spacing = points[1]->X - points[0]->X;
	if (points->Length>=3) {
		for (int i=1; i<points->Length-1; i++)
			if (points[i]->X - points[i+1]->X != spacing) { //TODO check margin of error for subtraction of floats
				isEqualSpacing = false;
				break;
			}
	}

	if (isEqualSpacing) { 
		double newXRatio = (newPointX - points[0]->X) / spacing;

		FillTable(points, isNewXCloserToStart);
		if (isNewXCloserToStart) {
			for (int i = 0; i<points->Length; i++) {
				double tmp = 1.0;
				//Newton forward divided difference formula
				for (int j = 0; j<i; j++)		tmp *= newXRatio - j;
				tmp *= System::Math::Pow(spacing, i);
				tmp *= DividedDifferenceTable[0]->default[i];

				newY += tmp;
			}
		}
		else {
			//TODO Newton backward divided difference formula
			throw gcnew System::NotImplementedException();
		}
	}
	else for (int i = 0; i<points->Length; i++) {
		double temp = isNewXCloserToStart
									? DividedDifference(points, 0, i)
									: DividedDifference(points, i, 0);
		temp *= NewtonBasisPolynomial(points, i, newPointX);
		newY += temp;
	}

	return newY;
}


