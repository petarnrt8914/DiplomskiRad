#pragma once
#include "pch.h"

using System::Drawing::PointF;

ref class TriangularMatrix
{
protected:
	array<array<double>^>^ matrix;
	array<PointF>^ points;

public:
	//TriangularMatrix(int size);
	TriangularMatrix(array<PointF>^);
	~TriangularMatrix();

protected:
	void FillTable();

	//void AddPoint(double x, double y);
	void AddPoint(PointF^, int index);

public:
	property double default[int, int] {
public:		double get(int,int);
protected:	void set(int,int,double value);
	}

public:
	property int Length {
		int get();
	}

	bool Expand(PointF);
};

