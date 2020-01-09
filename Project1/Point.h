#pragma once
using System::String;

ref class Point
{
public:
	float x;
	float y;

public:
	Point() : x(0), y(0) {}
	Point(float newX, float newY): x(newX), y(newY) {}

	bool operator==(Point const other) override {
		return this->x==other.x && this->y==other.y;
	}
	bool operator!=(Point const other) override {
		return this->x!=other.x || this->y!=other.y;
	}

	String^ ToString() override { //to be tested
		return String::Format("({0:N2}, {1:N2})", this->x, this->y);
	}
};

