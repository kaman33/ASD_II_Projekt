#ifndef POINT_H
#define POINT_H

#include <ostream>

struct Point{
    double x;
    double y;

    Point(double x, double y);

    bool operator<(const Point& other) const;
    bool operator>(const Point& other) const;
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    bool operator<=(const Point& other) const;
    bool operator>=(const Point& other) const;
    Point& operator=(const Point& other);

    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

#endif
