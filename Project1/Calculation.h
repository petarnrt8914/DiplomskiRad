#pragma once
//#include <list>
using System::Drawing::PointF;

class Calculation abstract
{
public:
	//Lagrange form
	static float LagrangeInterpolation(array<PointF^>^ points, float newPointX);

	//Baricentric form
	static float LagrangeInterpolation(array<PointF^>^ points, array<double>^ baricentricWeights, float newPointX);


	static float NewtonInterpolation();
};

