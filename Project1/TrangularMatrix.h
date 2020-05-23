#pragma once
using System::Drawing::PointF;

ref class TriangularMatrix
{
protected:
	array<array<double>^>^ matrix;
	array<PointF^>^ points;

public:
	//TriangularMatrix(int size);
	TriangularMatrix(array<PointF^>^);

protected:
	void FillTable();

	//void AddPoint(double x, double y);
	void AddPoint(PointF^, int index);

public:
	property double default[int, int] {
		double get(int,int);
		void set(int,int,double value);
	}

	property int Length {
		int get();
	}

	bool Expand(PointF^);
};

