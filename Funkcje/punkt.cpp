struct Punkt{
    float x;
    float y;

    Punkt(float x, float y) : x(x), y(y) {}

    // Operator mniejszosci
    bool operator<(const Punkt& other) const {
        if(x != other.x){ return x < other.x;}
        return y < other.y;
    }

    // Operator wiekszosci
    bool operator>(const Punkt& other) const {
        return other < *this;
    }

};



