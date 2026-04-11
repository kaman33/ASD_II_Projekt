#include "Point.h"
#include "Funkcje/EPS.h"

Point::Point(double x, double y) : x(x), y(y) {}

// Operator mniejszosci
bool Point::operator<(const Point& other) const {
    int cmpX = cmpDouble(x, other.x);
    if (cmpX != 0) {
        return cmpX < 0;
    }
    return cmpDouble(y, other.y) < 0;
}

// Operator wiekszosci
bool Point::operator>(const Point& other) const {
    return other < *this;
}

// Operator rownosci
bool Point::operator==(const Point& other) const {
    return cmpDouble(x, other.x) == 0 && cmpDouble(y, other.y) == 0;
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
