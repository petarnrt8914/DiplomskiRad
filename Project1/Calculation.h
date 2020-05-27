#pragma once
#include "TrangularMatrix.h"
//#include <list>
using System::Drawing::PointF;
using System::Collections::Generic::List;

ref class Calculation abstract
{
public:
	//Lagrange form
	static double LagrangeInterpolation(array<PointF^>^ points, double newPointX);
	static double LagrangeBasisPolynomial(array<PointF^>^ points, int index, double newPointX);

	//Baricentric form
	static double LagrangeInterpolation(array<PointF^>^ points, array<double>^ baricentricWeights, double newPointX);

	//reciprocal for better precision
	static array<double>^ ReciprocalBaricentricWeights(array<PointF^>^ points);

	static void AddBaricentricWeight(array<double>^ %existingWeights, array<PointF^>^ points, int indexOfNewPoint);

public:
	static double NewtonInterpolation(array<PointF^>^ points, double newPointX);
	static double NewtonInterpolation(array<PointF^>^ points, TriangularMatrix ^ DividedDifferenceTable, double newPointX);

private:
	static double NewtonBasisPolynomial(array<PointF^>^ points, int length, double newX);

	//static TriangularMatrix^ DividedDifferenceTable; //triangular matrix // will move to CalculationForm.cpp

private:
	static double DividedDifference(array<PointF^>^ points, int start, int end); //obsolete
	//static double ForwardDividedDifference(array<PointF^>^ points, int start, int end);
	//static double BackwardDividedDifference(array<PointF^>^ points, int start, int end);
	//
	//static int BinomialCoefficient(int n, int k);
	//static int Factorial(int value);

};

