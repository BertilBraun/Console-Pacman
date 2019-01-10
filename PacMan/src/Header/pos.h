#pragma once

#ifndef POS_H
#define POS_H

#include <iostream>

namespace POS {
	template <class T>
	class Pos {
	public:
		T x, y;

		inline Pos() : x(0), y(0) {}
		inline Pos(T x, T y) : x(x), y(y) {}
		inline Pos(const Pos& v) : x(v.x), y(v.y) {}

		inline Pos& operator=(const Pos& v) { x = v.x; y = v.y; return *this; }

		inline bool operator==(const Pos& v) { return (x == v.x && y == v.y); }
		inline bool operator!=(const Pos& v) { return (x != v.x || y != v.y); }

		inline bool operator<(const Pos& v) const { return (x < v.x || (x == v.x && y < v.y)); }
		inline bool operator>(const Pos& v) const { return (x > v.x || (x == v.x && y > v.y)); }

#ifdef VEC2_H
#include "vec2.h"
		inline Pos operator+(const VECTOR::Vec2& v) { return Pos(x + v.x, y + v.y); }
		inline Pos& operator+=(const VECTOR::Vec2& v) { x += v.x; y += v.y; return *this; }
#endif 

		inline Pos& operator+(double o) { return Pos(x + o, y + o); }
		inline Pos& operator-(double o) { return Pos(x - o, y - o); }
		inline Pos& operator*(double o) { return Pos(x * o, y * o); }
		inline Pos& operator/(double o) { return Pos(x / o, y / o); }
		
		inline Pos& operator+=(double o) { *this += o; return *this; }
		inline Pos& operator-=(double o) { *this -= o; return *this; }
		inline Pos& operator*=(double o) { *this *= o; return *this; }
		inline Pos& operator/=(double o) { *this /= o; return *this; }


		inline Pos& operator+(const Pos& v) { return Pos(x + v.x, y + v.y); }
		inline Pos& operator-(const Pos& v) { return Pos(x - v.x, y - v.y); }
		inline Pos& operator*(const Pos& v) { return Pos(x * v.x, y * v.y); }
		inline Pos& operator/(const Pos& v) { return Pos(x / v.x, y / v.y); }

		inline Pos& operator+=(const Pos& v) { *this += v; return *this; }
		inline Pos& operator-=(const Pos& v) { *this -= v; return *this; }
		inline Pos& operator*=(const Pos& v) { *this *= v; return *this; }
		inline Pos& operator/=(const Pos& v) { *this /= v; return *this; }

		inline friend std::ostream& operator<<(std::ostream& os, const Pos& v) {
			os << v.x << "x, " << v.y << "y" << std::endl;
			return os;
		}

		inline void Set(T x, T y) {
			this->x = x;
			this->y = y;
		}

		inline float dist(Pos v) {
			Pos d(v.x - x, v.y - y);
			return std::sqrt(d.x * d.x + d.y * d.y);
		}
	};
}

typedef POS::Pos<int> Pos;
typedef POS::Pos<float> Posf;
typedef POS::Pos<double> Posd;

#endif