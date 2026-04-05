#include "point.h"

Point::Point(float x, float y) : x(x), y(y) {}

// Operator mniejszosci
bool Point::operator<(const Point& other) const {
    if (x != other.x) {
        return x < other.x;
    }
    return y < other.y;
}

// Operator wiekszosci
bool Point::operator>(const Point& other) const {
    return other < *this;
}

// Wypisanie punktu
std::ostream& operator<<(std::ostream& stream, const Point& p){
    stream << "(" << p.x << ", " << p.y << ")";
    return stream;
}