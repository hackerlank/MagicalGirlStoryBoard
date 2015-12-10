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

double Vector2::dot(Vector2 v) {
	return x * v.x + y * v.y;
}

double Vector2::angleBetween(Vector2 v) {
	double dotProd = this->dot(v);
	dotProd /= this->magnitude() * v.magnitude();
	return acos(dotProd);
}

Vector2 Vector2::operator+(Vector2 v) {
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator-(Vector2 v) {
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator*(double multiple) {
	return Vector2(x * multiple, y * multiple);
}

Vector2 Vector2::operator/(double division) {
	return Vector2(x / division, y / division);
}

void Vector2::operator+=(Vector2 v) {
	x += v.x;
	y += v.y;
}

void Vector2::operator-=(Vector2 v) {
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