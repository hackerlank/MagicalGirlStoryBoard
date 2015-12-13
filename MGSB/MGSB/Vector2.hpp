#ifndef VECTOR2_HPP
#define VECTOR2_HPP

class Vector2 {
public:
	Vector2(double x = 0.0, double y = 0.0);
	double Magnitude();
	Vector2 Normalize();
	double Dot(Vector2 v);
	double AngleBetween(Vector2 v);

	// Rotates around a specified point
	Vector2 RotateAround(Vector2 origin, double rotation);
	Vector2 operator+(Vector2 v);
	Vector2 operator-(Vector2 v);
	Vector2 operator*(double multiple);
	Vector2 operator/(double division);
	void operator+=(Vector2 v);
	void operator-=(Vector2 v);
	void operator*=(double multiple);
	void operator/=(double division);

	double x;
	double y;
};

#endif//VECTOR2_HPP
