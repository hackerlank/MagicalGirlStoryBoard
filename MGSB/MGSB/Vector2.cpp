#include "Vector2.hpp"
#include "math.h"

Vector2::Vector2(double x, double y)
	: x(x), y(y) {}

double Vector2::magnitude() {
	return sqrt(x * x + y * y);
}

Vector2 Vector2::normalize() {
	double mag = magnitude();
	return Vector2(x / mag, y / mag);
}

Vector2 Vector2::operator+(const Vector2& v) {
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(const Vector2& v) {
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(double multiple) {
	return Vector2(x * multiple, y * multiple);
}

Vector2 Vector2::operator/(double division) {
	return Vector2(x / division, y / division);
}

void Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
}

void Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
}

void Vector2::operator*=(double multiple) {
	x *= multiple;
	y *= multiple;
}

void Vector2::operator/=(double division) {
	x /= division;
	y /= division;
}