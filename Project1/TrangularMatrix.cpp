#include "TrangularMatrix.h"

//inline TriangularMatrix::TriangularMatrix(int size) {
//	points = gcnew array<PointF^>(size);
//
//	matrix = gcnew array<array<double>^>(size);
//	for (int i = 0; i<size; i++)
//		matrix[i] = gcnew array<double>(i);
//}

TriangularMatrix::TriangularMatrix(array<PointF>^ inputPoints) : points(inputPoints) {
	matrix = gcnew array<array<double>^>(inputPoints->Length);
	for (int i = 0; i<inputPoints->Length; i++)
		matrix[i] = gcnew array<double>(i+1);

	FillTable();
}

TriangularMatrix::~TriangularMatrix() {
	delete points;

	for (int i = 0; i<Length; ++i) delete matrix[i];

	delete matrix;
}

inline void TriangularMatrix::FillTable() {
	for (int i=0; i<points->Length; i++)
		AddPoint(points[i], i);
}

inline void TriangularMatrix::AddPoint(PointF^ newPoint, int index) {
	int i=index; // for readability
	if (matrix[i] == nullptr) matrix[i] = gcnew array<double>(index+1);

	matrix[i][i] = newPoint->Y;

	for (int j = i-1; j>=0; j--) {
		long double value = matrix[i][j+1] - matrix[i-1][j];
		value /= points[i].X - points[j].X;
		matrix[i][j] = value;
	}
}


double TriangularMatrix::default::get(int row, int column) {
	int i ,j;
	if (row>column) {
		i = row; 		j = column;
	}
	else {
		i = column;		j = row;
	}

	return matrix[i][j];
}

void TriangularMatrix::default::set(int row, int column, double value) {
	int i, j;
	if (row>column) {
		i = row; 		j = column;
	}
	else {
		i = column;		j = row;
	}

	matrix[i][j] = value;
}


bool TriangularMatrix::Expand(PointF newPoint) {

	//add column to the bottom of the matrix
	matrix->Resize(matrix, matrix->Length+1);
	matrix[matrix->Length-1] = gcnew array<double>(matrix->Length);

	if (newPoint.X < points[0].X) {
		//add newPoint to points
		auto temp = gcnew array<PointF>(points->Length+1);
		points->CopyTo(temp, 1);
		delete points;
		points = temp;
		points[0] = newPoint;

		//shift everything to bottom right //practically add column to the left of the matrix
		for (int i = matrix->Length-1; i>0; --i) {
			for (int j = i; j>0; --j)
				matrix[i][j] = matrix[i-1][j-1];
		}

		matrix[0][0] = newPoint.Y;
		for (int i = 1; i<matrix->Length; ++i) {
			long double value = matrix[i][1] - matrix[i-1][0];
			value /= points[i].X - points[0].X;
			matrix[i][0] = value;
		}
	}
	else if (newPoint.X > points[points->Length-1].X) {
		//add newPoint to points
		auto temp = gcnew array<PointF>(points->Length+1);
		points->CopyTo(temp, 0);
		temp[temp->Length-1] = newPoint;
		delete points;
		points = temp;
		
		AddPoint(newPoint, matrix->Length-1);
	}
	else // if new point is not outside of current range, don't expand
		return false;

	return true;

}

int TriangularMatrix::Length::get() {
	return matrix->Length;
}