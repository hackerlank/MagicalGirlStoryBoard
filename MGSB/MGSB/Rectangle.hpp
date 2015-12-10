#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "Vector2.hpp"

class Rectangle {
public:
	Rectangle(Vector2 size, Vector2 center, double rotation);
	Vector2 size;
	Vector2 center;
	double rotation;
};

#endif//RECTANGLE_HPP