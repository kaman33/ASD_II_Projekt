#include "Point.h"

Point::Point(double x, double y) : x(x), y(y) {}

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

// Operator rownosci
bool Point::operator==(const Point& other) const {
    return ((x == other.x) && (y == other.y));
}

// Operator nierownosci
bool Point::operator!=(const Point& other) const {
    return !(*this == other);
}

// Operator mniejszosci lub rownosci
bool Point::operator<=(const Point& other) const {
    return (*this < other) || (*this == other);
}

// Operator wiekszosci lub rownosci
bool Point::operator>=(const Point& other) const {
    return (*this > other) || (*this == other);
}

// Operator przypisania
Point& Point::operator=(const Point& other) {
    if(this != &other){
        x = other.x;
        y = other.y;
    }
    
    return *this;
}

// Wypisanie punktu
std::ostream& operator<<(std::ostream& stream, const Point& p){
    stream << "(" << p.x << ", " << p.y << ")";
    return stream;
}