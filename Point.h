#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ")" << std::endl;
        return os;
    }
};

#endif  // __POINT_H__
