#pragma once
using System::String;
using System::Drawing::PointF;

ref class Point
{
public:
	double x;
	double y;
	bool IsEmpty;

	property double X	{ double get(); void set(double value); };
	property double Y	{ double get(); void set(double value); };

public:
	Point() : IsEmpty(true) {}
	Point(double newX, double newY): x(newX), y(newY), IsEmpty(false) {}
	Point(Point %p);
	Point(System::Drawing::PointF %p);

	bool operator==(Point other) {
		return this->x==other.x && this->y==other.y;
	}
	bool operator!=(Point other) {
		return this->x!=other.x || this->y!=other.y;
	}

	Point operator+(Point other);
	Point operator+(double factor);
	Point operator-(Point other);
	Point operator-(double factor);
	Point operator-();
	Point operator*(Point other);
	Point operator*(double factor);

	PointF toPointF();


	String^ ToString() override { //to be tested
		return String::Format("({0:G6}, {1:G6})", this->x, this->y);
	}
};

