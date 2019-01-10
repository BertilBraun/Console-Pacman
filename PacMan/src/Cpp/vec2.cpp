
#include "vec2.h"
#include <math.h>
#include <corecrt_math_defines.h>

//namespace VECTOR {
	void Vec2::zero() {
		x = y = 0;
	}

	void Vec2::rotate(double deg) {
		double theta = deg / 180.0 * M_PI;
		x = x * cos(theta) - y * sin(theta);
		y = x * sin(theta) + y * cos(theta);
	}

	Vec2& Vec2::normalize() {
		if (length() == 0) return *this;
		*this *= (1.0 / length());
		return *this;
	}

	float Vec2::dist(Vec2 v) {
		Vec2 d(v.x - x, v.y - y);
		return d.length();
	}
	/*
	float Vec2::length() {
	}
	*/
	Vec2 Vec2::ortho() {
		return Vec2(y, -x);
	}

	float Vec2::dot(Vec2 v1, Vec2 v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	float Vec2::cross(Vec2 v1, Vec2 v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	float Vec2::angle(Vec2 v1, Vec2 v2) {
		//	return std::acos(((v1 * v2) / (v1.length() * v2.length())));
	}
//}