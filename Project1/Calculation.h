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



};

