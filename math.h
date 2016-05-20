#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>
#include <algorithm>

#define EPS 1e-8
#define PI 3.14159265358979323846

struct vector2d {
	double x, y;
	
	vector2d() {}
	
	vector2d(double _x, double _y) : x(_x), y(_y) {}
	
	double length2() const {
		return x * x + y * y;
	}
	
	double length() const {
		return sqrt(x * x + y * y);
	}
	
	void normalize() {
		double inv_len = 1.0 / length();
		x *= inv_len;
		y *= inv_len;
	}
	
	vector2d get_normal() {
		double inv_len = 1.0 / length();
		return vector2d(x * inv_len, y * inv_len);
	}
};

typedef vector2d v2d;

inline v2d operator + (const v2d &a, const v2d &b) {
	return v2d(a.x + b.x, a.y + b.y);
}

template <class T>
inline v2d & operator += (v2d &a, const T &b) {
	return (a = a + b);
}

inline v2d operator - (const v2d &a, const v2d &b) {
	return v2d(a.x - b.x, a.y - b.y);
}

template <class T>
inline v2d & operator -= (v2d &a, const T &b) {
	return (a = a - b);
}

inline v2d operator - (const v2d &a) {
	return v2d(-a.x, -a.y);
}

inline v2d operator * (const v2d &a, double x) {
	return v2d(a.x * x, a.y * x);
}

inline v2d operator * (double x, const v2d &a) {
	return v2d(a.x * x, a.y * x);
}

template <class T>
inline v2d & operator *= (v2d &a, const T &b) {
	return (a = a * b);
}

inline v2d operator / (const v2d &a, double x) {
	return a * (1.0 / x);
}

template <class T>
inline v2d & operator /= (v2d &a, const T &b) {
	return (a = a / b);
}

inline double Cross(const v2d &a, const v2d &b) {
	return a.x * b.y - a.y * b.x;
}

inline double Dot(const v2d &a, const v2d &b) {
	return a.x * b.x + a.y * b.y;
}

inline v2d Rotate(const v2d &a, double x) {
	double s = sin(x), c = cos(x);
	return v2d(a.x * c - a.y * s, a.x * s + a.y * c);
}

inline v2d Rotate(const v2d &a, double s, double c) {
	return v2d(a.x * c - a.y * s, a.x * s + a.y * c);
}

inline v2d Rotate_90(const v2d &a) {
	return v2d(-a.y, a.x);
}

template <class T>
inline T sqr(const T &x) {
	return x * x;
}

template <class T>
inline T abs(const T &x) {
	return x < 0 ? -x : x;
}

inline v2d min(const v2d &a, const v2d &b) {
	return v2d(std::min(a.x, b.x), std::min(a.y, b.y));
}

inline v2d max(const v2d &a, const v2d &b) {
	return v2d(std::max(a.x, b.x), std::max(a.y, b.y));
}

template <class T>
inline T clamp(const T &a, const T &L, const T &R) {
	return std::max(L, std::min(R, a));
}

#endif
