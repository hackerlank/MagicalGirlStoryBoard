#ifndef VECTOR2_HPP
#define VECTOR2_HPP

class Vector2 {
public:
	Vector2(double x = 0.0, double y = 0.0);
  Vector2 normalize();
  Vector2 operator+(const Vector2& v);
  Vector2 operator-(const Vector2& v);
  Vector2 operator*(double multiple);
  Vector2 operator/(double division);

	double x;
	double y;
};

#endif//VECTOR2_HPP
