#pragma once
#include <list>
using System::Drawing::PointF;

class Calculation abstract
{
public:
	//Lagrange form
	static float LagrangeInterpolation(array<PointF^>^ points, int size, float newPointX);

	//Baricentric form
	static float LagrangeInterpolation(array<PointF^>^ points, int pointsCount,
																		 array<double>^ baricentricWeights, float newPointX);


	static float NewtonInterpolation(array<PointF^>^ points, float newPointX);

private:
	static double NewtonBasisPolynomial(array<PointF^>^ points, int length, double newX);

	static double DividedDifference(array<PointF^>^ points, int start, int end);
	static double ForwardDividedDifference(array<PointF^>^ points, int start, int end);
	static double BackwardDividedDifference(array<PointF^>^ points, int start, int end);
	
	static int BinomialCoefficient(int n, int k);
	static int Factorial(int value);
};

