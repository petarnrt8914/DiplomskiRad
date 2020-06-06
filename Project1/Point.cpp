#include "Point.h"


double Point::X::get() { return this->x; }
double Point::Y::get() { return this->y; }

void Point::X::set(double value) {
	x = value;
	IsEmpty = false;
}

void Point::Y::set(double value) {
	y = value;
	IsEmpty = false;
}

Point::Point(Point % p): x(p.X), y(p.Y), IsEmpty(p.IsEmpty) {}
Point::Point(System::Drawing::PointF % p) : x(p.X), y(p.Y), IsEmpty(p.IsEmpty) {}

Point Point::operator+(Point other) {
	Point result(this->x + other.X, this->y + other.Y);
	result.IsEmpty = this->IsEmpty && other.IsEmpty;
	return result;
}

Point Point::operator+(double factor) {
	Point result(this->x + factor, this->y + factor);
	result.IsEmpty = this->IsEmpty;
	return result;
}

Point Point::operator-(Point other) {
	Point result(this->x - other.X, this->y - other.Y);
	result.IsEmpty = this->IsEmpty && other.IsEmpty;
	return result;
}

Point Point::operator-(double factor) {
	Point result(this->x - factor, this->y - factor);
	result.IsEmpty = this->IsEmpty;
	return result;
}

Point Point::operator-() {
	Point result(-this->x, -this->y);
	result.IsEmpty = this->IsEmpty;
	return result;
}

Point Point::operator*(Point other) {
	Point result(this->x * other.X, this->y * other.Y);
	result.IsEmpty = this->IsEmpty && other.IsEmpty;
	return result;
}

Point Point::operator*(double factor) {
	Point result(this->x * factor, this->y * factor);
	result.IsEmpty = this->IsEmpty;
	return result;
}


PointF Point::toPointF() {
	return this->IsEmpty ? PointF::Empty : PointF(this->x, this->y);
}