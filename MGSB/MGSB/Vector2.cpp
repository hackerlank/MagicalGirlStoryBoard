#include "Vector2.hpp"

Vector2::Vector2(double x, double y)
	: x(x), y(y) {}

double length() {
  return sqrt(x * x + y * y);
}

Vector2 normalize() {
  double length = length();
  return Vector2(x / length, y / length);
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
