#pragma once

#ifndef VEC2_H
#define VEC2_H

#include <cmath>
#include <iostream>

namespace VECTOR {
	template <class T>
	class Vec2 {
	public:
		T x, y;
		
		inline Vec2() : x(0), y(0) {}
		inline Vec2(T x, T y) : x(x), y(y) {}
		inline Vec2(const Vec2& v) : x(v.x), y(v.y) {}

#ifdef POS_H
#include "pos.h"
		inline Vec2(const POS::Pos<T>& p1, const POS::Pos<T>& p2) : x(p2.x - p1.x) {
			y = p2.y - p1.y;
		}
#endif 
		inline Vec2& operator=(const Vec2& v) { x = v.x; y = v.y; return *this; }

		inline bool operator==(const Vec2& v) { return (x == v.x && y == v.y); }
		inline bool operator!=(const Vec2& v) { return (x != v.x || y != v.y); }

		inline bool operator<(Vec2& v) { return (length() < v.length()); }
		inline bool operator>(Vec2& v) { return (length() > v.length()); }

		inline Vec2& operator+(double o) { return Vec2(x + o, y + o); }
		inline Vec2& operator-(double o) { return Vec2(x - o, y - o); }
		inline Vec2& operator*(double o) { return Vec2(x * o, y * o); }
		inline Vec2& operator/(double o) { return Vec2(x / o, y / o); }
		
		inline Vec2& operator+=(double o) { *this = *this + o; return *this; }
		inline Vec2& operator-=(double o) { *this = *this - o; return *this; }
		inline Vec2& operator*=(double o) { *this = *this * o; return *this; }
		inline Vec2& operator/=(double o) { *this = *this / o; return *this; }

		inline Vec2& operator+(const Vec2& v) { return Vec2(x + v.x, y + v.y); }
		inline Vec2& operator-(const Vec2& v) { return Vec2(x - v.x, y - v.y); }
		inline Vec2& operator*(const Vec2& v) { return Vec2(x * v.x, y * v.y); }
		inline Vec2& operator/(const Vec2& v) { return Vec2(x / v.x, y / v.y); }

		inline Vec2& operator+=(const Vec2& v) { *this = *this + o; return *this; }
		inline Vec2& operator-=(const Vec2& v) { *this = *this - o; return *this; }
		inline Vec2& operator*=(const Vec2& v) { *this = *this * o; return *this; }
		inline Vec2& operator/=(const Vec2& v) { *this = *this / o; return *this; }

		inline friend std::ostream& operator<<(std::ostream& os, const Vec2& v) {
			os << v.x << "x, " << v.y << "y\n";
			return os;
		}

		inline void Set(T x, T y) {
			this->x = x;
			this->y = y;
		}

		inline void zero();

		inline void rotate(double deg);

		inline Vec2& normalize();
		inline float dist(Vec2 v);
		inline float length() {
			return std::sqrt(x * x + y * y);
		}

		inline Vec2 ortho();

		inline static float dot(Vec2 v1, Vec2 v2);			
		inline static float cross(Vec2 v1, Vec2 v2);
		inline static float angle(Vec2 v1, Vec2 v2);			

	};

}

typedef VECTOR::Vec2<float> Vec2;
typedef VECTOR::Vec2<int> Vec2i;
typedef VECTOR::Vec2<double> Vec2d;

#endif