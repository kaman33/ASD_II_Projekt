#include "punkt.h"

Punkt::Punkt(float x, float y) : x(x), y(y) {}

// Operator mniejszosci
bool Punkt::operator<(const Punkt& other) const {
    if (x != other.x) {
        return x < other.x;
    }
    return y < other.y;
}

// Operator wiekszosci
bool Punkt::operator>(const Punkt& other) const {
    return other < *this;
}

// Wypisanie punktu
std::ostream& operator<<(std::ostream& stream, const Punkt& p){
    stream << "(" << p.x << ", " << p.y << ")";
    return stream;
}