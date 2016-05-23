#ifndef POINT_H
#define POINT_H

struct Point {
	int x, y;
	Point(int x, int y) : x(x), y(y) {}
	bool operator == (Point& p) {
		return x == p.x && y == p.y;
	}
};

#endif // POINT_H
